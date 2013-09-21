/**
 ******************************************************************************
 *
 * @file       monitorconfiguration.cpp
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

#include "monitorconfiguration.h"
#include "mapcontent/mapconfiguration.h"
#include "cameracontent/cameraconfiguration.h"

#include "utils/pathutils.h"
#include <QDir>

MonitorConfiguration::MonitorConfiguration(QString classId,
		QSettings *qSettings, QObject *parent) :
		IUAVGadgetConfiguration(classId, parent), m_settings(qSettings)
{
	// if a saved configuration exists load it
	if (qSettings != 0) {
//		QString accessMode = qSettings->value("accessMode").toString();
		;
	}
	m_mapConfig = new MapConfiguration(QString("MonitorMaps"), qSettings);
	m_cameraConfig = new CameraConfiguration(QString("MonitorCams"), qSettings);
}

IUAVGadgetConfiguration *MonitorConfiguration::clone() {
	MonitorConfiguration *m = new MonitorConfiguration(this->classId());

//	m->m_navigationMapProvider = m_navigationMapProvider;
	m->m_mapConfig = qobject_cast<MapConfiguration *>(m_mapConfig->clone());
	m->m_cameraConfig = qobject_cast<CameraConfiguration *>(m_cameraConfig->clone());
	return m;
}

void MonitorConfiguration::saveConfig() const {
	if (!m_settings) {
		return;
	}
//	m_settings->setValue("navigationMapProvider", m_navigationMapProvider);
}

void MonitorConfiguration::saveConfig(QSettings *qSettings) const {
//	qSettings->setValue("navigationMapProvider", m_navigationMapProvider);
}
