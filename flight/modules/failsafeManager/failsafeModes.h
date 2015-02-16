//
//  failsafeModes.h
//  OpenPilotOSX
//
//  Created by GIOVANNI REY on 2/10/15.
//  Copyright (c) 2015 OpenPilot. All rights reserved.
//

#ifndef __OpenPilotOSX__failsafeModes__
#define __OpenPilotOSX__failsafeModes__

#include <stdio.h>

typedef enum {
    FS0=0,
    FS1FLY=1,
    FS2LOITER=2,
    FS3RTH=3,
    FS4LAND=4,
    FS5KILL=5,
    POWERDOWN=6
} failsafeModeDefinitions;


int8_t FSEscalate( int8_t newFSMode );
bool FS5Conditions();
bool FS4Conditions();
bool FS3Conditions();
bool FS2Conditions();
bool FS1Conditions();

#endif /* defined(__OpenPilotOSX__failsafeModes__) */
