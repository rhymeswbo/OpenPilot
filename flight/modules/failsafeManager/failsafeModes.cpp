//
//  failsafeModes.cpp
//  OpenPilotOSX
//
//  Created by GIOVANNI REY on 2/10/15.
//  Copyright (c) 2015 OpenPilot. All rights reserved.
//

#include "failsafeModes.h"

/*
 * 5 - KILL:  cut power immediately and crash land
 *   > entry conditions:
 *       0 - escalation of 4 or 2
 *   - qualifiers:
 *       0 - in air ?
 *   < exit conditions:
 *       0 - reset / turn off everything(?)
 */

bool FS5Conditions(){
    //?
    return true;
}

/*
 * 4 - LAND:  PH or zero speed and controled slow decient
 *   > entry conditions:
 *       0 - escalation of 3 or 2
 *   - qualifiers:
 *       0 - same qualifiers as LOITER and FLY
 *       1 - altitude lower then onlast tick.
 *   < exit conditions:
 *       0 - regain control input & GPS(if needed)
 *       1 - qualifiers fail to pass then escalate to failsafe4:LAND
 *       2 - landed and shut down/disarm
 */
bool FS4Conditions(){
    
}

bool FS3Conditions(){
    
}

bool FS2Conditions(){
    
}

bool FS1Conditions(){
    
}

bool FS0Conditions(){
    // direct control mode... and lost rc
    if(FlightModeSettinsFlightModePosition<=FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_POSITIONHOLD &&
       ManualControlCommandConnected==MANUALCONTROLCOMMAND_CONNECTED_FALSE){
        return false;
    }
    
    // auto modes, and lost GPS link
    if(FlightModeSettinsFlightModePosition>=FLIGHTMODESETTINGS_FLIGHTMODEPOSITION_POSITIONHOLD &&
       (GPSPositionSensorStatus==GPSPOSITIONSENSOR_STATUS_NOFIX || GPSPositionSensorStatus==GPSPOSITIONSENSOR_STATUS_NOGPS)){
        return false;
    }
}


/*
 * - set up the conditions for autopilot to execute the new failsafe mode newFSMode
 * - set up the failsafeManager state for newFSMode
 * - return newFSMode
 */

int8_t FSEscalate( int8_t newFSMode ){
    switch (newFSMode) {
        case FS0::
            <#statements#>
            break;
        case FS1FLY:
            <#statements#>
            break;
        case FS2LOITER:
            <#statements#>
            break;
        case FS3RTH:
            <#statements#>
            break;
        case FS4LAND:
            <#statements#>
            break;
        case FS5KILL:
            <#statements#>
            break;
        default:
            //disarm.
            break;
    }
    return newFSMode;
}




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
 0 - regain control input & GPS(if needed)
 1 - qualifiers fail to pass then escalate to failsafe2:LOITER
 
 
 2 - LOITER: PH and wait in place at this or slightly higher standard altitude.
 > entry conditions:
 0 - failed qualifiers of FLY
 - qualifiers:
 0 - GPS lock required
 1 - same qualifiers as FLY 0-4
 < exit conditions:
 0 - regain control input & GPS(if needed)
 1 - qualifiers fail to pass then escalate to failsafe3:RTH
 
 
 3 - RTH:   return to home and land
 > entry conditions:
 0 - failed qualifiers of LOITER
 - qualifiers:
 0 - since last tick we have gotten closer to home (or climbed toward cruise alt)
 1 - same qualifiers as FLY and LOITER
 2 - ***time limit changed.
 3 - ***attitude limits loosened
 < exit conditions:
 0 - regain control input & GPS(if needed)
 1 - qualifiers fail to pass then escalate to failsafe4:LAND
 
 
 4 - LAND:  PH or zero speed and controled slow decient
 > entry conditions:
 0 - escalation of 3 or 2
 - qualifiers:
 0 - same qualifiers as LOITER and FLY
 1 - altitude lower then onlast tick.
 < exit conditions:
 0 - regain control input & GPS(if needed)
 1 - qualifiers fail to pass then escalate to failsafe4:LAND
 2 - landed and shut down/disarm
 
 5 - KILL:  cut power immediately and crash land
 > entry conditions:
 0 - escalation of 4 or 2
 - qualifiers:
 0 - none
 < exit conditions:
 0 - reset.
 
 
 */

