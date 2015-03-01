/**
 ******************************************************************************
 * @addtogroup OpenPilotModules OpenPilot Modules
 * @{
 * @addtogroup ReceiverModule Manual Control Module
 * @brief Provide manual control or allow it alter flight mode.
 * @{
 *
 * Reads in the ManualControlCommand from receiver then
 * pass it to ManualControl
 *
 * @file       receiver.c
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2014.
 * @brief      Receiver module. Handles safety R/C link and flight mode.
 *
 * @see        The GNU Public License (GPL) Version 3
 *
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <openpilot.h>
#include <accessorydesired.h>
#include <manualcontrolsettings.h>
#include <manualcontrolcommand.h>
#include <receiveractivity.h>
#include <flightstatus.h>
#include <flighttelemetrystats.h>
#include <flightmodesettings.h>
#include <systemsettings.h>
#include <taskinfo.h>

#if defined(PIOS_INCLUDE_USB_RCTX)
#include "pios_usb_rctx.h"
#endif /* PIOS_INCLUDE_USB_RCTX */

// Private constants
#if defined(PIOS_RECEIVER_STACK_SIZE)
#define STACK_SIZE_BYTES  PIOS_RECEIVER_STACK_SIZE
#else
#define STACK_SIZE_BYTES  1152
#endif

#define TASK_PRIORITY     (tskIDLE_PRIORITY + 3) // 3 = flight control
#define UPDATE_PERIOD_MS  20
#define THROTTLE_FAILSAFE -0.1f
#define ARMED_THRESHOLD   0.50f
// safe band to allow a bit of calibration error or trim offset (in microseconds)
#define CONNECTION_OFFSET 250

// Private types

// Private variables
static xTaskHandle taskHandle;
static portTickType lastSysTime;


// Private functions
static void receiverTask(void *parameters);


#define RCVR_ACTIVITY_MONITOR_CHANNELS_PER_GROUP 12
#define RCVR_ACTIVITY_MONITOR_MIN_RANGE          10
struct rcvr_activity_fsm {
    ManualControlSettingsChannelGroupsOptions group;
    uint16_t prev[RCVR_ACTIVITY_MONITOR_CHANNELS_PER_GROUP];
    uint8_t sample_count;
};

#define assumptions \
    ( \
        ((int)MANUALCONTROLCOMMAND_CHANNEL_NUMELEM == (int)MANUALCONTROLSETTINGS_CHANNELGROUPS_NUMELEM) && \
        ((int)MANUALCONTROLCOMMAND_CHANNEL_NUMELEM == (int)MANUALCONTROLSETTINGS_CHANNELNUMBER_NUMELEM) && \
        ((int)MANUALCONTROLCOMMAND_CHANNEL_NUMELEM == (int)MANUALCONTROLSETTINGS_CHANNELMIN_NUMELEM) && \
        ((int)MANUALCONTROLCOMMAND_CHANNEL_NUMELEM == (int)MANUALCONTROLSETTINGS_CHANNELMAX_NUMELEM) && \
        ((int)MANUALCONTROLCOMMAND_CHANNEL_NUMELEM == (int)MANUALCONTROLSETTINGS_CHANNELNEUTRAL_NUMELEM))

/**
 * Module starting
 */
int32_t ReceiverStart()
{
    // Start main task
    xTaskCreate(receiverTask, "Receiver", STACK_SIZE_BYTES / 4, NULL, TASK_PRIORITY, &taskHandle);
    PIOS_TASK_MONITOR_RegisterTask(TASKINFO_RUNNING_RECEIVER, taskHandle);
#ifdef PIOS_INCLUDE_WDG
    PIOS_WDG_RegisterFlag(PIOS_WDG_MANUAL);
#endif

    return 0;
}

/**
 * Module initialization
 */
int32_t ReceiverInitialize()
{
    /* Check the assumptions about uavobject enum's are correct */
    PIOS_STATIC_ASSERT(assumptions);

    ManualControlCommandInitialize();
    ReceiverActivityInitialize();
    ManualControlSettingsInitialize();
    FlightStatusInitialize();

    

    return 0;
}
MODULE_INITCALL(ReceiverInitialize, ReceiverStart);

/**
 * Module task
 */
static void receiverTask(__attribute__((unused)) void *parameters)
{
    ManualControlSettingsData settings;
    ManualControlCommandData cmd;
    FlightStatusData flightStatus;


    // For now manual instantiate extra instances of Accessory Desired.  In future should be done dynamically
    // this includes not even registering it if not used
    AccessoryDesiredCreateInstance();
    AccessoryDesiredCreateInstance();

    // Whenever the configuration changes, make sure it is safe to fly

    ManualControlCommandGet(&cmd);


    

    // Main task loop
    lastSysTime = xTaskGetTickCount();

    SystemSettingsThrustControlOptions thrustType;

    while (1) {
        // Wait until next update
        vTaskDelayUntil(&lastSysTime, UPDATE_PERIOD_MS / portTICK_RATE_MS);
#ifdef PIOS_INCLUDE_WDG
        PIOS_WDG_UpdateFlag(PIOS_WDG_MANUAL);
#endif

        // Read settings
        ManualControlSettingsGet(&settings);
        FlightStatusGet(&flightStatus);
        ManualControlCommandGet(&cmd);
        SystemSettingsThrustControlGet(&thrustType);

 
        if (cmd.GCSControl && !flightStatus.GCSControlTimeout) {
            AlarmsSet(SYSTEMALARMS_ALARM_RECEIVER, SYSTEMALARMS_ALARM_WARNING);
        } else {
            AlarmsSet(SYSTEMALARMS_ALARM_RECEIVER, SYSTEMALARMS_ALARM_RECEIVER);

        }
   

        if  (cmd.GCSControl && flightStatus.GCSControlTimeout) {
            if (ManualControlCommandReadOnly()) {
                FlightTelemetryStatsData flightTelemStats;
                FlightTelemetryStatsGet(&flightTelemStats);
                
                /* trying to fly via GCS and lost connection.  fall back to transmitter */
                UAVObjMetadata metadata;
                ManualControlCommandGetMetadata(&metadata);
                UAVObjSetAccess(&metadata, ACCESS_READWRITE);
                ManualControlCommandSetMetadata(&metadata);

            }
            flightStatus.FailsafeLevel=FLIGHTSTATUS_FAILSAFELEVEL_FAILSAFE1;
 
            
            cmd.Throttle   = settings.FailsafeChannel.Throttle;
            cmd.Roll       = settings.FailsafeChannel.Roll;
            cmd.Pitch      = settings.FailsafeChannel.Pitch;
            cmd.Yaw = settings.FailsafeChannel.Yaw;
            cmd.Collective = settings.FailsafeChannel.Collective;
            switch (thrustType) {
            case SYSTEMSETTINGS_THRUSTCONTROL_THROTTLE:
                cmd.Thrust = cmd.Throttle;
                break;
            case SYSTEMSETTINGS_THRUSTCONTROL_COLLECTIVE:
                cmd.Thrust = cmd.Collective;
                break;
            default:
                break;
            }
            
            // ??? take a look at this, what?
            if (settings.FailsafeFlightModeSwitchPosition >= 0 && settings.FailsafeFlightModeSwitchPosition < settings.FlightModeNumber) {
                cmd.FlightModeSwitchPosition = (uint8_t)settings.FailsafeFlightModeSwitchPosition;
            }
            
            AlarmsSet(SYSTEMALARMS_ALARM_RECEIVER, SYSTEMALARMS_ALARM_WARNING);


        } else { // valid control input
            
            flightStatus.FailsafeLevel=FLIGHTSTATUS_FAILSAFELEVEL_NONE;
            AlarmsClear(SYSTEMALARMS_ALARM_RECEIVER);

        }

        // Update cmd object
        ManualControlCommandSet(&cmd);
        FlightStatusSet(&flightStatus);
    }
}

/**
 * @}
 * @}
 */
