/**
 ******************************************************************************
 *
 * @file       mapcontent.h
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

#ifndef MAPCONTENT_H
#define MAPCONTENT_H

#include "monitorcontent.h"

#include "extensionsystem/pluginmanager.h"
#include "opmapcontrol/opmapcontrol.h"

using namespace mapcontrol;

// TODO das hier ist eher C-Stil - umarbeiten in Objektorientierung
typedef struct t_home {
	internals::PointLatLng coord;
	double altitude;
	bool locked;
} t_home;

class QMutex;
class UAVObject;
class UAVObjectManager;
class UAVObjectUtilManager;
class MonitorConfiguration;

class MapContent : public MonitorContent {
	Q_OBJECT
public:
	MapContent(CONTENT_TYPE mapType);
	virtual ~MapContent();

	virtual BUTTONSET getModeButtons();
	virtual void mode1();
	virtual void mode2();
	virtual void mode3();
	virtual BUTTONSET getZoomButtons() { return (B1 + B2 + B3); }
	virtual void zoomIn();
	virtual void zoomReset();
	virtual void zoomOut();
	virtual BUTTONSET getCursorButtons();
	virtual void moveUp();
	virtual void moveDown();
	virtual void moveLeft();
	virtual void moveRight();
	virtual void doReturn();


	virtual void applyConfiguration(MonitorConfiguration *config);
	virtual QGraphicsView *display() {return m_map;}

	// TODO prüfen, wie man die aus public rauskriegt
	bool getGPSPositionSensor(double &latitude, double &longitude, double &altitude);
	void setHome(QPointF pos);
	void setHome(internals::PointLatLng pos_lat_lon, double altitude);
	// TODO das hier in applyConfiguration verwenden
    void setPosition(QPointF pos);
    void setMapProvider(QString provider);
    void setUseOpenGL(bool useOpenGL);
    void setAccessMode(QString accessMode);
    void setUseMemoryCache(bool useMemoryCache);
    void setCacheLocation(QString cacheLocation);
    void setUavPic(QString UAVPic);
    void setHomePosition(QPointF pos);

public slots:
	void homePositionUpdated(UAVObject *);

private:
	ExtensionSystem::PluginManager *pm;
	UAVObjectManager *obm;
	UAVObjectUtilManager *obum;
	mapcontrol::OPMapWidget *m_map;
	QMutex m_map_mutex;
	t_home m_home_position;
	int m_currentZoom;
	int m_defaultZoom;

	bool getUAVPosition(double &latitude, double &longitude, double &altitude);
	double getUAV_Yaw();

private slots:
	void updatePosition();

};
#endif // MAPCONTENT_H
