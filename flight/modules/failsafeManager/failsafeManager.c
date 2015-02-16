/**
 ******************************************************************************
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

/**
 * Output object: pathplanner, flightmode, attituderequested(?),failsafeManagerStatus
 *
 * This module will periodically update the value of the BaroSensor object.
 *
 */

#include <openpilot.h>

//UAVObject includes
#include "failsafeManagerStatus.h"
#include "failsafeManagerSettings.h"
#include "gpspositionsensor.h"
#include "ManualControlCommand.h"
#include "flightcontrolsettings.h"

//Module Includes
#include "failsafeModes.h"
#include <mathmisc.h>
#include "taskinfo.h"

// Private constants
#define STACK_SIZE_BYTES 500
#define TASK_PRIORITY    (tskIDLE_PRIORITY + 1)
#define UPDATE_PERIOD    50

// Private types

// Private variables
static xTaskHandle taskHandle;
static portTickType lastSysTime;

//This data struct is contained in the automatically generated UAVObject code
failsafeManagerSettingsData settingsData;
failsafeManagerStatusData statusData;
int8_t newfailsafeLevel=0;




uint8_t GPSPositionSensorStatus = 0;
/*typedef enum {
 GPSPOSITIONSENSOR_STATUS_NOGPS=0,
 GPSPOSITIONSENSOR_STATUS_NOFIX=1,
 GPSPOSITIONSENSOR_STATUS_FIX2D=2,
 GPSPOSITIONSENSOR_STATUS_FIX3D=3
 } GPSPositionSensorStatusOptions;*/

uint8_t ManualControlCommandConnected=0;
/*typedef enum {
 MANUALCONTROLCOMMAND_CONNECTED_FALSE=0,
 MANUALCONTROLCOMMAND_CONNECTED_TRUE=1
 } ManualControlCommandConnectedOptions;*/

uint8_t FlightModeSettingsStabilization3Settings;
/*typedef enum {
 FLIGHTMODESETTINGS_STABILIZATION3SETTINGS_MANUAL=0,
 FLIGHTMODESETTINGS_STABILIZATION3SETTINGS_RATE=1,
 FLIGHTMODESETTINGS_STABILIZATION3SETTINGS_ATTITUDE=2,
 FLIGHTMODESETTINGS_STABILIZATION3SETTINGS_AXISLOCK=3,
 FLIGHTMODESETTINGS_STABILIZATION3SETTINGS_WEAKLEVELING=4,
 FLIGHTMODESETTINGS_STABILIZATION3SETTINGS_VIRTUALBAR=5,
 FLIGHTMODESETTINGS_STABILIZATION3SETTINGS_ACRO=6,
 FLIGHTMODESETTINGS_STABILIZATION3SETTINGS_RATTITUDE=7,
 FLIGHTMODESETTINGS_STABILIZATION3SETTINGS_ALTITUDEHOLD=8,
 FLIGHTMODESETTINGS_STABILIZATION3SETTINGS_ALTITUDEVARIO=9,
 FLIGHTMODESETTINGS_STABILIZATION3SETTINGS_CRUISECONTROL=10
 } FlightModeSettingsStabilization3SettingsOptions;*/

uint8_t FlightModeSettinsFlightModePosition;
/*typedef enum {
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_MANUAL=0,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_STABILIZED1=1,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_STABILIZED2=2,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_STABILIZED3=3,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_STABILIZED4=4,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_STABILIZED5=5,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_STABILIZED6=6,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_AUTOTUNE=7,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_POSITIONHOLD=8,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_POSITIONVARIOFPV=9,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_POSITIONVARIOLOS=10,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_POSITIONVARIONSEW=11,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_RETURNTOBASE=12,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_LAND=13,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_PATHPLANNER=14,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_POI=15,
 FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_AUTOCRUISE=16
 } FlightModeSettingsFlightModePositionOptions;*/

ManualControlSettingsFailsafeChannelData *ManualControlSettingsFailsafeChannel;
/*typedef struct __attribute__ ((__packed__)) {
 float Throttle;
 float Roll;
 float Pitch;
 float Yaw;
 float Collective;
 float Accessory0;
 float Accessory1;
 float Accessory2;
 }  ManualControlSettingsFailsafeChannelData ;
 typedef struct __attribute__ ((__packed__)) {
 float array[8];
 }  ManualControlSettingsFailsafeChannelDataArray ;*/


// Private functions
static void failsafeManagerTask(void *parameters);
static void SettingsUpdatedCb(UAVObjEvent *ev);

/**
 * Initialise the module, called on startup
 * \returns 0 on success or -1 if initialisation failed
 */
int32_t failsafeManagerStart()
{
    // Start main task
    xTaskCreate(failsafeManagerTask, "failsafeManager", STACK_SIZE_BYTES / 4, NULL, TASK_PRIORITY, &taskHandle);
    PIOS_TASK_MONITOR_RegisterTask(TASKINFO_RUNNING_ALTITUDE, taskHandle);

    return 0;
}


/**
 * Initialise the module, called on startup
 * \returns 0 on success or -1 if initialisation failed
 */
int32_t failsafeManagerInitialize()
{
    failsafeManagerStatusInitialize();
    failsafeManagerSettingsInitialize();
    GPSPositionSensorInitialize();
    FlightModeSettings();
    ManualControlCommand();
    
    failsafeManagerStatusConnectCallback(&SettingsUpdatedCb);
    failsafeManagerSettingsConnectCallback(&SettingsUpdatedCb);
    
    SettingsUpdatedCb(NULL);
    return 0;
}
MODULE_INITCALL(failsafeManagerInitialize, failsafeManagerStart);

/**
 * Module thread, should not return.
 */

static void failsafeManagerTask(__attribute__((unused)) void *parameters)
{

    
    while(1) {
        lastSysTime = xTaskGetTickCount();
        statusData.failsafeTimeStamp = lastSysTime;
        statusData.failsafeLevel = 0;
        
        //Populate the data struct with the UAVObject's current values
        failsafeManagerSettingsGet(&settingsData);
        failsafeManagerStatusGet(&statusData);
        GPSPositionSensorStatusGet(&GPSPositionSensorStatus);
        ManualControlCommandConnectedGet(&ManualControlCommandConnected);
        FlightModeSettingsFlightModePositionGet(&FlightModeSettinsFlightModePosition);
        

        // start at the worst FS mode and then if that is ok, check out the next gentler one.
        if (!FS5Conditions()){
            FSEscalate(POWERDOWN);
            
        } else if (!FS4Conditions()){
            FSEscalate(FS5KILL);
            
        } else if (!FS3Conditions()){
            FSEscalate(FS4LAND);
            
        } else if (!FS2Conditions()){
            FSEscalate(FS3RTH);
            
        } else if (!FS1Conditions()){
            FSEscalate(FS2LOITER);
            
        } else if (!FS0Conditions()) {
            FSEscalate(FS1FLY);
            
        } else {
            FSEscalate(FS0); //nomral flight.
        }
        
        
 
        //Update the UAVObject data. The updated values can be viewed in the GCS.
        statusData.failsafeLevel=newfailsafeLevel;
        failsafeManagerStatusSet(&statusData);
        
        // Delay until it is time to read the next sample
        vTaskDelayUntil(&lastSysTime, UPDATE_PERIOD / portTICK_RATE_MS);
        
        
    }
}

static void SettingsUpdatedCb(__attribute__((unused)) UAVObjEvent *ev)
{

}
/**
 * @}
 * @}
 */
