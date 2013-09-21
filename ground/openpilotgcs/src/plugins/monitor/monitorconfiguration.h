/**
 ******************************************************************************
 *
 * @file       monitorconfiguration.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2013.
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup  MonitorPlugin Monitor Plugin
 * @{
 * @brief A monitor plugin
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

#ifndef MONITORCONFIGURATION_H
#define MONITORCONFIGURATION_H

#include <coreplugin/iuavgadgetconfiguration.h>
//#include <QtCore/QString>

class MapConfiguration;
class CameraConfiguration;

//using namespace Core;

class MonitorConfiguration : public Core::IUAVGadgetConfiguration {
    Q_OBJECT

public:
    explicit MonitorConfiguration(QString classId, QSettings *qSettings = 0, QObject *parent = 0);

    void saveConfig(QSettings *settings) const;
    void saveConfig() const;
    IUAVGadgetConfiguration *clone();

    MapConfiguration *mapConfig() {
    	return m_mapConfig;
    }

    CameraConfiguration *cameraConfig() {
    	return m_cameraConfig;
    }

private:
    QSettings *m_settings;
    MapConfiguration *m_mapConfig;
    CameraConfiguration *m_cameraConfig;
};

#endif // MONITORCONFIGURATION_H
