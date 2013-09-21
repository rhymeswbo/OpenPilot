/**
 ******************************************************************************
 *
 * @file       gpscontent.cpp
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
// TODO implementieren des GPS-Contents (Idee noch nicht wirklich vorhanden)
// Form der Darstellung?
// Was wird dargestellt?
// mehrere Varianten?
#include "gpscontent.h"

GpsContent::GpsContent(CONTENT_TYPE contentType) :
		MonitorContent(contentType) {
	m_gps = new GpsContentWidget();

	// connect the timer at the end!
	m_updateTimer->setInterval(m_updateRate);
//	connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateFunction()));
}

GpsContent::~GpsContent() {
	if(m_updateTimer) {
		m_updateTimer->stop();
		disconnect(m_updateTimer, 0, 0, 0);
	}
	if (m_gps)
		delete m_gps;
	m_gps = NULL;
}

void GpsContent::applyConfiguration(MonitorConfiguration *config) {

}
