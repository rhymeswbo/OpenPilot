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

#include "failsafeManager.h"
#include "revosettings.h"
#include <mathmisc.h>
#include "taskinfo.h"

// Private constants
#define STACK_SIZE_BYTES    550
#define TASK_PRIORITY       (tskIDLE_PRIORITY + 1)


// Private types

// Private variables
static xTaskHandle taskHandle;

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

    failsafeMangerSettingsConnectCallback(&SettingsUpdatedCb);
    
    SettingsUpdatedCb(NULL);
    return 0;
}
MODULE_INITCALL(failsafeManagerInitialize, failsafeManagerStart);
/**
 * Module thread, should not return.
 */
static void failsafeManagerTask(__attribute__((unused)) void *parameters)
{

    static portTickType lastSysTime;
    
    lastSysTime = xTaskGetTickCount();
    //This data struct is contained in the automatically generated UAVObject code
    failsafeManagerSettingsData settingsData;
    failsafeManagerStatusData statusData;
    
    //Populate the data struct with the UAVObject's current values
    failsafeManageerSettingsGet(data);
    failsafeManageerStatusGet(data);

    
    while(1) {
        //Get a new reading from the thermocouple
        statusData.failsafeTimeStamp = lastSysTime;
        statusData.failsafeLevel = 0;
        
        //Update the UAVObject data. The updated values can be viewed in the GCS.
        failsafeManagerStatusSet(&data);
        
        // Delay until it is time to read the next sample
        vTaskDelayUntil(&lastSysTime, UPDATE_PERIOD / portTICK_RATE_MS);
/*
 levels 
 0 - normal operation
 
 1 - FLY:  continue on steady heading and altitude
   > entry condition:  
        0 - loss of RC input
        1 - in PH or auto and loss of GPS
   - qualifiers:
        0 - this level is enabled in settings
        1 - limited time in this state, then either raise or lower failsafe level
        2 - no significant altitude change
        3 - attitude is close to zero
        4 - altitude (from home) is > 5 meters.
   < exit conditions: 
        1 - regain control input & GPS(if needed)
        2 - go to failsafe2 just hovering anway.
        3 - qualifiers fail to pass then escalate.
  
 2 - LOITER: PH and wait in place at this or slightly higher standard altitude.
   > entry conditions:
        0 - escalation of 1
   - qualifiers:
   < exit conditions:
 
 3 - RTH:   return to home and land
   > entry conditions:
        0 - escalation of 3
   - qualifiers:
   < exit conditions:
 
 4 - LAND:  PH or zero speed and controled slow decient
   > entry conditions:
        0 - escalation of 4
   - qualifiers:
   < exit conditions:
 
 5 - KILL:  cut power immediately and crash land
   > entry conditions:
        0 - escalation of 5
   - qualifiers:
   < exit conditions:
 
 
 */
        
        
        
    }
}

static void SettingsUpdatedCb(__attribute__((unused)) UAVObjEvent *ev)
{

}
/**
 * @}
 * @}
 */
