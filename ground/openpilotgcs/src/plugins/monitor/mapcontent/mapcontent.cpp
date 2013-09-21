/**
 ******************************************************************************
 *
 * @file       mapcontent.cpp
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2013.
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup MonitorPlugin Monitor Plugin
 * @{
 * @brief A monitor plugins map
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
#include <math.h>
#include <QMutex>
#include <QMutexLocker>

#include "mapcontent.h"
#include "monitorconfiguration.h"
#include "mapconfiguration.h"

#include "opmapcontrol/opmapcontrol.h"
#include "utils/coordinateconversions.h"
#include "uavobjectutilmanager.h"
#include "uavobjectmanager.h"
#include "uavobject.h"
#include "objectpersistence.h"
#include "utils/stylehelper.h"
#include "utils/homelocationutil.h"

#include "positionstate.h"
#include "homelocation.h"
#include "gpspositionsensor.h"
#include "gyrostate.h"
#include "attitudestate.h"
#include "positionstate.h"
#include "velocitystate.h"
#include "airspeedstate.h"


#include "QtDebug"

#define earth_mean_radius   6371    // kilometers
#define deg_to_rad          ((double)M_PI / 180.0)
#define rad_to_deg          (180.0 / (double)M_PI)

#define NAV_ZOOM 15 // Zoom factor is overview level
#define SAT_ZOOM 19  // Zoom factor is detail level
#define NAV_UPDATE_RATE 500 // milliseconds
#define SAT_UPDATE_RATE 100 // milliseconds

MapContent::MapContent(CONTENT_TYPE mapType) :
		MonitorContent(mapType) {

	// initialize pointers
	pm = NULL;
	obm = NULL;
	obum = NULL;
	m_map = NULL;

	// get plugin manager and object manager
	pm = ExtensionSystem::PluginManager::instance();
	Q_ASSERT(pm);
	obm = pm->getObject<UAVObjectManager>();
	Q_ASSERT(obm);
	obum = pm->getObject<UAVObjectUtilManager>();
	Q_ASSERT(obum);

	// get current location
	double latitude = 0.0;
	double longitude = 0.0;
	double altitude = 0.0;
	getUAVPosition(latitude, longitude, altitude);
	internals::PointLatLng pos_lat_lon = internals::PointLatLng(latitude,
			longitude);

	// default home position
	m_home_position.coord = pos_lat_lon;
	m_home_position.altitude = altitude;
	m_home_position.locked = false;

	m_map = new mapcontrol::OPMapWidget(); // create the map object
	Q_ASSERT(m_map);
	m_map->setFrameStyle(QFrame::NoFrame); // no border frame
	m_map->setBackgroundBrush(QBrush(Utils::StyleHelper::baseColor())); // tile background
	m_map->SetFollowMouse(false); // we don't want a contiuous mouse position reading
	m_map->SetShowUAV(true); // display the UAV position on the map
	m_map->SetShowCompass(false); // we don't want a compass because this is environment view not navigation

	if (mapType == NAV) {
		m_map->SetShowHome(true); // display the HOME position on the map
		m_map->Home->SetShowSafeArea(false); // don't show the safe area  //SHOULDN'T THE DEFAULT BE USER DEFINED?
		m_map->UAV->SetMapFollowType(UAVMapFollowType::CenterMap); // map is centering
		m_map->SetRotate(0); // reset map rotation to 0deg
		m_currentZoom = m_defaultZoom = NAV_ZOOM;
		m_updateRate = NAV_UPDATE_RATE;
	} else if (mapType == SAT) {
		m_map->SetShowHome(false); // do not display the HOME position on the map
		m_map->Home->SetSafeArea(3); // set radius (meters) //SHOULDN'T THE DEFAULT BE USER DEFINED?
		m_map->Home->SetShowSafeArea(true); // don't show the safe area  //SHOULDN'T THE DEFAULT BE USER DEFINED?
		m_map->UAV->SetMapFollowType(UAVMapFollowType::CenterAndRotateMap); // map is centering and rotating
		m_currentZoom = m_defaultZoom = SAT_ZOOM;
		m_updateRate = SAT_UPDATE_RATE;
	} else {
		// give a message, delete the created map and bail out
		QMessageBox::information(0, "MapContent",
				"Tried to create an invalid MapContent!");
		delete m_map;
		return;
	}
	m_map->setOverlayOpacity(0.5); // medium transparency of UAV etc.
	m_map->SetZoom(m_currentZoom);
//	connect(m_map, SIGNAL(OnCurrentPositionChanged(internals::PointLatLng)),
//			this, SLOT(OnCurrentPositionChanged(internals::PointLatLng))); // map position change signals
	m_map->SetCurrentPosition(m_home_position.coord); // set the map position
	m_map->Home->SetCoord(m_home_position.coord); // set the HOME position
	m_map->UAV->SetUAVPos(m_home_position.coord, 0.0); // set the UAV position
	m_map->UAV->update();
	if (m_map->GPS)
		m_map->GPS->SetUAVPos(m_home_position.coord, 0.0); // set the GPS position

	// connect to the UAVObject updates we require to become a bit aware of our environment:
	if (pm) {
		// Register for Home Location state changes
		if (obm) {
			UAVDataObject *obj = dynamic_cast<UAVDataObject *>(obm->getObject(
					QString("HomeLocation")));
			if (obj) {
				connect(obj, SIGNAL(objectUpdated(UAVObject *)), this,
					SLOT(homePositionUpdated(UAVObject *)));
			}
		}
	}

	// connect the timer at the end!
	m_updateTimer->setInterval(m_updateRate);
	connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updatePosition()));
}

MapContent::~MapContent() {
	qDebug() << "entering ~MapContent()";
	if(m_updateTimer) {
		m_updateTimer->stop();
		disconnect(m_updateTimer, 0, 0, 0);
	}
	if (m_map) {
		disconnect(m_map, 0, 0, 0);
		m_map->SetShowHome(false); // doing this appears to stop the map lib crashing on exit
		m_map->SetShowUAV(false); //   "          "
		if (m_map) {
			delete m_map;
			m_map = NULL;
		}
	}
	qDebug() << "leaving ~MapContent()";
}

void MapContent::applyConfiguration(MonitorConfiguration *config) {
	// use the map part of MonitorConfiguration
	MapConfiguration *mapConfig = config->mapConfig();
	setUseOpenGL(mapConfig->useOpenGL());
	setAccessMode(mapConfig->accessMode());
	setUseMemoryCache(mapConfig->useMemoryCache());
	setCacheLocation(mapConfig->cacheLocation());
	setUavPic(mapConfig->uavSymbol());
	setPosition(QPointF(mapConfig->longitude(), mapConfig->latitude()));
	setHomePosition(
			QPointF(mapConfig->longitude(), mapConfig->latitude()));

	if (m_type == SAT)
		setMapProvider(mapConfig->environmentMapProvider());
	else if (m_type == NAV)
		setMapProvider(mapConfig->navigationMapProvider());
	else // default to nav map provider
		setMapProvider(mapConfig->navigationMapProvider());
}

BUTTONSET MapContent::getModeButtons() {
	BUTTONSET b = DISABLE_BUTTONS;
	if (m_map->UAV->GetMapFollowType() != UAVMapFollowType::None)
		b += B1;
	if (m_map->UAV->GetMapFollowType() != UAVMapFollowType::CenterMap)
		b += B2;
	if (m_map->UAV->GetMapFollowType() != UAVMapFollowType::CenterAndRotateMap)
		b += B3;

	return b;
}

void MapContent::mode1() {
	m_map->UAV->SetMapFollowType(UAVMapFollowType::None);
	m_map->SetRotate(0); // reset map rotation to 0deg
	m_map->setInteractive(true);
}

void MapContent::mode2() {
	m_map->UAV->SetMapFollowType(UAVMapFollowType::CenterMap);
	m_map->SetRotate(0); // reset map rotation to 0deg
	m_map->setInteractive(false);
	updatePosition();
}

void MapContent::mode3() {
	m_map->UAV->SetMapFollowType(UAVMapFollowType::CenterAndRotateMap);
	m_map->setInteractive(false);
	updatePosition();
}

void MapContent::zoomIn() {
	m_map->SetZoom(++m_currentZoom);
}

void MapContent::zoomReset() {
	m_map->SetZoom(m_currentZoom = m_defaultZoom);
}

void MapContent::zoomOut() {
	m_map->SetZoom(--m_currentZoom);
}

BUTTONSET MapContent::getCursorButtons() {
	if (m_map->UAV->GetMapFollowType() != UAVMapFollowType::None)
		return 0;
	return (B1 + B2 + B3 + B4 + B5);
}

void MapContent::moveUp() {
	return;
}

void MapContent::moveDown() {
	return;
}

void MapContent::moveLeft() {
	return;
}

void MapContent::moveRight() {
	return;
}

void MapContent::doReturn() {
	return;
}

bool MapContent::getUAVPosition(double &latitude, double &longitude,
		double &altitude) {
    double NED[3];
    double LLA[3];
    double homeLLA[3];

    Q_ASSERT(obm != NULL);

    PositionState *positionState = PositionState::GetInstance(obm);
    Q_ASSERT(positionState != NULL);
    PositionState::DataFields positionStateData = positionState->getData();
    if (positionStateData.North == 0 && positionStateData.East == 0 && positionStateData.Down == 0) {
        GPSPositionSensor *gpsPositionObj = GPSPositionSensor::GetInstance(obm);
        Q_ASSERT(gpsPositionObj);

        GPSPositionSensor::DataFields gpsPositionData = gpsPositionObj->getData();
        latitude  = gpsPositionData.Latitude / 1.0e7;
        longitude = gpsPositionData.Longitude / 1.0e7;
        altitude  = gpsPositionData.Altitude;
        return true;
    }
    HomeLocation *homeLocation = HomeLocation::GetInstance(obm);
    Q_ASSERT(homeLocation != NULL);
    HomeLocation::DataFields homeLocationData = homeLocation->getData();

    homeLLA[0] = homeLocationData.Latitude / 1.0e7;
    homeLLA[1] = homeLocationData.Longitude / 1.0e7;
    homeLLA[2] = homeLocationData.Altitude;

    NED[0]     = positionStateData.North;
    NED[1]     = positionStateData.East;
    NED[2]     = positionStateData.Down;

    Utils::CoordinateConversions().NED2LLA_HomeLLA(homeLLA, NED, LLA);

    latitude  = LLA[0];
    longitude = LLA[1];
    altitude  = LLA[2];

    if (latitude != latitude) {
        latitude = 0; // nan detection
    } else if (latitude > 90) {
        latitude = 90;
    } else if (latitude < -90) {
        latitude = -90;
    }

    if (longitude != longitude) {
        longitude = 0; // nan detection
    } else if (longitude > 180) {
        longitude = 180;
    } else if (longitude < -180) {
        longitude = -180;
    }

    if (altitude != altitude) {
        altitude = 0; // nan detection
    }
    return true;
}

double MapContent::getUAV_Yaw() {
    if (!obm) {
        return 0;
    }

    UAVObject *obj = dynamic_cast<UAVDataObject *>(obm->getObject(QString("AttitudeState")));
    double yaw     = obj->getField(QString("Yaw"))->getDouble();

    if (yaw != yaw) {
        yaw = 0; // nan detection
    }
    while (yaw < 0) {
        yaw += 360;
    }
    while (yaw >= 360) {
        yaw -= 360;
    }

    return yaw;
}

/**
 Updates the UAV position on the map. It is called at a user-defined frequency,
 as set inside the constructor.
 */
void MapContent::updatePosition() {
	   double uav_latitude, uav_longitude, uav_altitude, uav_yaw;
	    double gps_latitude, gps_longitude, gps_altitude, gps_heading;

	    internals::PointLatLng uav_pos;
	    internals::PointLatLng gps_pos;

	    if (!m_map) {
	        return;
	    }

	    QMutexLocker locker(&m_map_mutex);

	    // *************
	    // get the current UAV details

	    // get current UAV position
	    if (!getUAVPosition(uav_latitude, uav_longitude, uav_altitude)) {
	        return;
	    }

	    // get current UAV heading
	    uav_yaw = getUAV_Yaw();

	    uav_pos = internals::PointLatLng(uav_latitude, uav_longitude);

	    // *************
	    // get the current GPS position and heading
	    GPSPositionSensor *gpsPositionObj = GPSPositionSensor::GetInstance(obm);
	    Q_ASSERT(gpsPositionObj);

	    GPSPositionSensor::DataFields gpsPositionData = gpsPositionObj->getData();

	    gps_heading   = gpsPositionData.Heading;
	    gps_latitude  = gpsPositionData.Latitude;
	    gps_longitude = gpsPositionData.Longitude;
	    gps_altitude  = gpsPositionData.Altitude;

	    gps_pos = internals::PointLatLng(gps_latitude * 1e-7, gps_longitude * 1e-7);

	    // **********************
	    // get the current position and heading estimates
	    AttitudeState *attitudeStateObj = AttitudeState::GetInstance(obm);
	    PositionState *positionStateObj = PositionState::GetInstance(obm);
	    VelocityState *velocityStateObj = VelocityState::GetInstance(obm);
	    AirspeedState *airspeedStateObj = AirspeedState::GetInstance(obm);

	    GyroState *gyroStateObj = GyroState::GetInstance(obm);

	    Q_ASSERT(attitudeStateObj);
	    Q_ASSERT(positionStateObj);
	    Q_ASSERT(velocityStateObj);
	    Q_ASSERT(airspeedStateObj);
	    Q_ASSERT(gyroStateObj);

	    AttitudeState::DataFields attitudeStateData = attitudeStateObj->getData();
	    PositionState::DataFields positionStateData = positionStateObj->getData();
	    VelocityState::DataFields velocityStateData = velocityStateObj->getData();
	    AirspeedState::DataFields airspeedStateData = airspeedStateObj->getData();

	    GyroState::DataFields gyroStateData = gyroStateObj->getData();

	    double NED[3]  = { positionStateData.North, positionStateData.East, positionStateData.Down };
	    double vNED[3] = { velocityStateData.North, velocityStateData.East, velocityStateData.Down };

	    // Set the position and heading estimates in the painter module
	    m_map->UAV->SetNED(NED);
	    m_map->UAV->SetCAS(airspeedStateData.CalibratedAirspeed);
	    m_map->UAV->SetGroundspeed(vNED, m_updateRate);

	    // Convert angular velocities into a rotationg rate around the world-frame yaw axis. This is found by simply taking the dot product of the angular Euler-rate matrix with the angular rates.
	    float psiRate_dps = 0 * gyroStateData.z + sin(attitudeStateData.Roll * deg_to_rad) / cos(attitudeStateData.Pitch * deg_to_rad) * gyroStateData.y + cos(attitudeStateData.Roll * deg_to_rad) / cos(attitudeStateData.Pitch * deg_to_rad) * gyroStateData.z;

	    // Set the angular rate in the painter module
	    m_map->UAV->SetYawRate(psiRate_dps); // Not correct, but I'm being lazy right now.

	    // *************
	    // set the UAV icon position on the map

	    m_map->UAV->SetUAVPos(uav_pos, uav_altitude); // set the maps UAV position
	// qDebug()<<"UAVPOSITION"<<uav_pos.ToString();
	    m_map->UAV->SetUAVHeading(uav_yaw); // set the maps UAV heading

	    // *************
	    // set the GPS icon position on the map
	    if (m_map->GPS) {
	        m_map->GPS->SetUAVPos(gps_pos, gps_altitude); // set the maps GPS position
	        m_map->GPS->SetUAVHeading(gps_heading); // set the maps GPS heading
	        m_map->GPS->update();
	    }
	    m_map->UAV->updateTextOverlay();
	    m_map->UAV->update();
	    // *************
}

bool MapContent::getGPSPositionSensor(double &latitude, double &longitude,
		double &altitude) {
	double LLA[3];

	if (!obum) {
		return false;
	}

	if (obum->getGPSPositionSensor(LLA) < 0) {
		return false; // error
	}

	// typically we should get values. 0.0 indicates missing GPS
	if (LLA[0] == 0.0 && LLA[1] == 0.0)
		return false;

	latitude = LLA[0];
	longitude = LLA[1];
	altitude = LLA[2];

	return true;
}

/**
 Sets the home position on the map widget
 */
void MapContent::setHome(QPointF pos) {

	double latitude = pos.x();
	double longitude = pos.y();

	if (latitude > 90)
		latitude = 90;
	else if (latitude < -90)
		latitude = -90;

	if (longitude != longitude)
		longitude = 0; // nan detection
	else if (longitude > 180)
		longitude = 180;
	else if (longitude < -180)
		longitude = -180;

	setHome(internals::PointLatLng(latitude, longitude), 0);
}

void MapContent::setHome(internals::PointLatLng pos_lat_lon, double altitude) {

	if (pos_lat_lon.Lat() != pos_lat_lon.Lat()
			|| pos_lat_lon.Lng() != pos_lat_lon.Lng())
		return; // nan prevention

	double latitude = pos_lat_lon.Lat();
	double longitude = pos_lat_lon.Lng();

	if (latitude != latitude)
		latitude = 0; // nan detection
	else if (latitude > 90)
		latitude = 90;
	else if (latitude < -90)
		latitude = -90;

	if (longitude != longitude)
		longitude = 0; // nan detection
	else if (longitude > 180)
		longitude = 180;
	else if (longitude < -180)
		longitude = -180;

	m_home_position.coord = internals::PointLatLng(latitude, longitude);
	m_home_position.altitude = altitude;

	if (!m_map) {
		m_map->Home->SetCoord(m_home_position.coord);
		m_map->Home->SetAltitude(altitude);
		m_map->Home->RefreshPos();
	}
}

// Updates the Home position icon whenever the HomePosition object is updated
void MapContent::homePositionUpdated(UAVObject *hp) {
	Q_UNUSED(hp);
	if (!obum)
		return;
	bool set;
	double LLA[3];
	if (obum->getHomeLocation(set, LLA) < 0)
		return; // error
	setHome(internals::PointLatLng(LLA[0], LLA[1]), LLA[2]);
}

void MapContent::setPosition(QPointF pos) {
	if (!m_map) {
		return;
	}

	double latitude = pos.y();
	double longitude = pos.x();

	if (latitude != latitude || longitude != longitude) {
		return; // nan prevention
	}
	if (latitude > 90) {
		latitude = 90;
	} else if (latitude < -90) {
		latitude = -90;
	}

	if (longitude > 180) {
		longitude = 180;
	} else if (longitude < -180) {
		longitude = -180;
	}

	m_map->SetCurrentPosition(internals::PointLatLng(latitude, longitude));
}

void MapContent::setMapProvider(QString provider) {
	if (!m_map) {
		return;
	}

	m_map->SetMapType(mapcontrol::Helper::MapTypeFromString(provider));
}

void MapContent::setAccessMode(QString accessMode) {
	if (!m_map) {
		return;
	}

	m_map->configuration->SetAccessMode(
			mapcontrol::Helper::AccessModeFromString(accessMode));
}

void MapContent::setUseOpenGL(bool useOpenGL) {
	if (!m_map) {
		return;
	}

	m_map->SetUseOpenGL(useOpenGL);
}

void MapContent::setUseMemoryCache(bool useMemoryCache) {
	if (!m_map) {
		return;
	}

	m_map->configuration->SetUseMemoryCache(useMemoryCache);
}

void MapContent::setCacheLocation(QString cacheLocation) {
	if (!m_map) {
		return;
	}

	cacheLocation = cacheLocation.simplified(); // remove any surrounding spaces

	if (cacheLocation.isEmpty()) {
		return;
	}

	if (!cacheLocation.endsWith(QDir::separator())) {
		cacheLocation += QDir::separator();
	}

	QDir dir;
	if (!dir.exists(cacheLocation)) {
		if (!dir.mkpath(cacheLocation)) {
			return;
		}
	}
	m_map->configuration->SetCacheLocation(cacheLocation);
}
void MapContent::setHomePosition(QPointF pos) {
	if (!m_map) {
		return;
	}

	double latitude = pos.y();
	double longitude = pos.x();

	if (latitude != latitude || longitude != longitude) {
		return; // nan prevention
	}
	if (latitude > 90) {
		latitude = 90;
	} else if (latitude < -90) {
		latitude = -90;
	}

	if (longitude > 180) {
		longitude = 180;
	} else if (longitude < -180) {
		longitude = -180;
	}

	m_map->Home->SetCoord(internals::PointLatLng(latitude, longitude));
}

void MapContent::setUavPic(QString UAVPic) {
	m_map->SetUavPic(UAVPic);
}
