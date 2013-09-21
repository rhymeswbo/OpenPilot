/**
 ******************************************************************************
 *
 * @file       mapconfiguration.cpp
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

#include "mapconfiguration.h"
#include "utils/pathutils.h"
#include <QDir>

MapConfiguration::MapConfiguration(QString classId,
		QSettings *qSettings, QObject *parent) :
		IUAVGadgetConfiguration(classId, parent),
		m_navigationMapProvider("GoogleMap"),
		m_environmentMapProvider("GoogleSatellite"),
		m_defaultLatitude(48.79871220),
		m_defaultLongitude(9.53042100),
		m_useOpenGL(false),
		m_accessMode("ServerAndCache"),
		m_useMemoryCache(true),
		m_cacheLocation(Utils::PathUtils().GetStoragePath() + "mapscache" + QDir::separator()),
		m_uavSymbol(QString::fromUtf8(":/uavs/images/mapquad.png")),
		m_settings(qSettings)
{
	// if a saved configuration exists load it
	if (qSettings == 0 && qSettings != 0) {
		QString navigationMapProvider = qSettings->value(
				"navigationMapProvider").toString();
		QString environmentMapProvider = qSettings->value(
				"environmentMapProvider").toString();
		double latitude = qSettings->value("defaultLatitude").toDouble();
		double longitude = qSettings->value("defaultLongitude").toDouble();
		bool useOpenGL = qSettings->value("useOpenGL").toBool();
		QString accessMode = qSettings->value("accessMode").toString();
		bool useMemoryCache = qSettings->value("useMemoryCache").toBool();
		QString cacheLocation = qSettings->value("cacheLocation").toString();
		QString uavSymbol = qSettings->value("uavSymbol").toString();

		if (!navigationMapProvider.isEmpty()) {
			m_navigationMapProvider = navigationMapProvider;
		}
		if (!environmentMapProvider.isEmpty()) {
			m_environmentMapProvider = environmentMapProvider;
		}
		m_defaultLatitude = latitude;
		m_defaultLongitude = longitude;
		m_useOpenGL = useOpenGL;
        m_uavSymbol = uavSymbol;

		if (!accessMode.isEmpty()) {
			m_accessMode = accessMode;
		}
		m_useMemoryCache = useMemoryCache;
		if (!cacheLocation.isEmpty()) {
			m_cacheLocation = Utils::PathUtils().InsertStoragePath(
					cacheLocation);
		}
	}
}

IUAVGadgetConfiguration *MapConfiguration::clone() {
	MapConfiguration *m = new MapConfiguration(
			this->classId());

	m->m_navigationMapProvider = m_navigationMapProvider;
	m->m_environmentMapProvider = m_environmentMapProvider;
	m->m_defaultLatitude = m_defaultLatitude;
	m->m_defaultLongitude = m_defaultLongitude;
	m->m_useOpenGL = m_useOpenGL;
	m->m_accessMode = m_accessMode;
	m->m_useMemoryCache = m_useMemoryCache;
	m->m_cacheLocation = m_cacheLocation;
	m->m_uavSymbol = m_uavSymbol;

	return m;
}

void MapConfiguration::saveConfig() const {
	if (!m_settings) {
		return;
	}
	m_settings->setValue("navigationMapProvider", m_navigationMapProvider);
	m_settings->setValue("environemntMapProvider", m_environmentMapProvider);
	m_settings->setValue("defaultLatitude", m_defaultLatitude);
	m_settings->setValue("defaultLongitude", m_defaultLongitude);
	m_settings->setValue("useOpenGL", m_useOpenGL);
	m_settings->setValue("accessMode", m_accessMode);
	m_settings->setValue("useMemoryCache", m_useMemoryCache);
	m_settings->setValue("uavSymbol", m_uavSymbol);
	m_settings->setValue("cacheLocation",
			Utils::PathUtils().RemoveStoragePath(m_cacheLocation));
}

void MapConfiguration::saveConfig(QSettings *qSettings) const {
	qSettings->setValue("navigationMapProvider", m_navigationMapProvider);
	qSettings->setValue("environemntMapProvider", m_environmentMapProvider);
	qSettings->setValue("defaultLatitude", m_defaultLatitude);
	qSettings->setValue("defaultLongitude", m_defaultLongitude);
	qSettings->setValue("useOpenGL", m_useOpenGL);
	qSettings->setValue("accessMode", m_accessMode);
	qSettings->setValue("useMemoryCache", m_useMemoryCache);
	qSettings->setValue("uavSymbol", m_uavSymbol);
	qSettings->setValue("cacheLocation",
			Utils::PathUtils().RemoveStoragePath(m_cacheLocation));
}

void MapConfiguration::setCacheLocation(QString cacheLocation) {
	m_cacheLocation = cacheLocation;
}
