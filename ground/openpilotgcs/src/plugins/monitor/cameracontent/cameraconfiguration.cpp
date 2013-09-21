/**
 ******************************************************************************
 *
 * @file       cameraconfiguration.cpp
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

#include "cameraconfiguration.h"

CameraConfiguration::CameraConfiguration(QString classId,
		QSettings *qSettings, QObject *parent) :
		IUAVGadgetConfiguration(classId, parent),
		m_settings(qSettings)
{
	// if a saved configuration exists load it
	if (qSettings != 0) {
		;
	}

}

IUAVGadgetConfiguration *CameraConfiguration::clone() {
	CameraConfiguration *m = new CameraConfiguration(
			this->classId());

	return m;
}

void CameraConfiguration::saveConfig() const {
	if (!m_settings) {
		return;
	}
}

void CameraConfiguration::saveConfig(QSettings *qSettings) const {
}
