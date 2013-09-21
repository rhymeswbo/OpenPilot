/**
 ******************************************************************************
 *
 * @file       monitorcontent.cpp
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2013.
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup MonitorPlugin Monitor Plugin
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

#include "monitorcontent.h"

#include <QTimer>
#include "monitorconfiguration.h"

#include "QtDebug"

MonitorContent::MonitorContent() {
	m_type = INVALID;
	m_updateTimer = new QTimer();
	m_updateRate =10000;	// default 10 seconds
}

MonitorContent::MonitorContent(CONTENT_TYPE type) : QObject() {
	m_type = type;
	m_updateTimer = new QTimer();
	m_updateRate =10000;	// default 10 seconds
}

MonitorContent::~MonitorContent() {
	qDebug() << "entering ~MonitorContent()";
	// delete the QTimer object
	if(m_updateTimer) {
		m_updateTimer->stop();
		disconnect(m_updateTimer, 0, 0, 0);
		delete m_updateTimer;
		m_updateTimer = NULL;
	}
	qDebug() << "leaving ~MonitorContent()";
}

void MonitorContent::applyConfiguration(MonitorConfiguration *config) {
	// TODO einbauen der generellen Monitorkonfiguration
	;
}
