/**
 ******************************************************************************
 *
 * @file       monitorcontentprovider.cpp
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2013.
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup MonitorPlugin Monitor Plugin
 * @{
 * @brief Content provider for the monitor plugin
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

#include "monitorcontentprovider.h"
#include "monitorgadget.h"
#include "monitorconfiguration.h"
#include "monitorcontent.h"
#include "mapcontent/mapcontent.h"
#include "emptycontent/emptycontent.h"
#include "gpscontent/gpscontent.h"
#include "cameracontent/cameracontent.h"

// init of static instance holder
MonitorContentProvider *MonitorContentProvider::sInstance =
		new MonitorContentProvider();

MonitorContentProvider *MonitorContentProvider::instance() {
	return sInstance;
}

MonitorContentProvider::MonitorContentProvider() {
	// initialize content management
	for (int i = 0; i < MAX_CONTENT; i++) {
		m_content[i] = NULL;
		m_monitor[i] = NULL;
		m_isUsed[i] = false;
	}
}

MonitorContentProvider::~MonitorContentProvider() {
	for (int i = 0; i < MAX_CONTENT; i++) {
		// first remove content from Monitor
		if (m_monitor[i]) {
			if (m_content[i]) {
				m_content[i]->stop();
			}
			m_monitor[i]->removeContent();
		}
		if (m_content[i]) {
			delete m_content[i];
		}
		m_isUsed[i] = false;
	}
}

MonitorContent *MonitorContentProvider::setContent(MonitorGadget *monitor,
		CONTENT_TYPE newType) {
//	QMessageBox::critical(0, tr("Debug"), tr("1"), QMessageBox::Ok);
	// security check of *monitor
	if (monitor == NULL) {
		QMessageBox::critical(0, tr("MonitorContentProvider"),
				tr("Call of setContent(...) without MonitorGadget"),
				QMessageBox::Ok);
		return NULL;
	}
	// check newType for valid value
	if (newType <= INVALID || newType > EMPTY) {
		QMessageBox::critical(0, tr("MonitorContentProvider"),
				tr("Wrong newType in setContent()"), QMessageBox::Ok);
		return NULL;
	}

	// remove content current monitor but remember content type
	CONTENT_TYPE oldType = removeContent(monitor);

	MonitorGadget *otherMonitor = NULL;
	MonitorContent *retVal = NULL;

	// use existing content or create create new content
	if (newType < EMPTY && m_content[newType] != NULL) {
		// free content from possible usage elsewhere
		otherMonitor = freeContent(newType);
		// now we can set content in our monitor
		applyContent(monitor, newType);
	} else {
		// create new content dependant on newType
		// EmptyContent is not unique, all others are unique
		switch (newType) {
		case (SAT):
		case (NAV):
			retVal = new MapContent(newType);
			break;
		case (GPS):
			retVal = new GpsContent(newType);
			break;
		case (CAM):
			retVal = new CameraContent(newType);
			break;
		case (EMPTY):
			retVal = new EmptyContent(newType);
			break;
		case (INVALID):
			// just do nothing but throw a message
			QMessageBox::critical(0, tr("MonitorContentProvider"),
					tr("Invalid newType in setContent()"), QMessageBox::Ok);
			break;
		default:
			// ok, here we reached an unimplemented content
			// just do nothing but throw a message
			QMessageBox::critical(0, tr("MonitorContentProvider"),
					tr("Unimplemented newType in setContent()"),
					QMessageBox::Ok);
			break;
		} // end switch
		if (retVal) {
			Q_ASSERT(m_config);
			retVal->applyConfiguration(m_config);
			// EmptyContent is not unique, all others are unique
			if (newType != EMPTY) {
				m_content[newType] = retVal;
				applyContent(monitor, newType);
			} else {
				monitor->setContent(retVal);
			}
		}
	}
	// if our monitor had a previous content, place that content in other monitor
	if (otherMonitor && oldType > INVALID && oldType < EMPTY) {
		applyContent(otherMonitor, oldType);
	}
	return retVal;
}

/**
 * applies content to monitor and updates content management
 */
void MonitorContentProvider::applyContent(MonitorGadget *monitor,
		CONTENT_TYPE type) {
	monitor->setContent(m_content[type]);
	m_monitor[type] = monitor;
	m_isUsed[type] = true;
	m_content[type]->start();
	return;
}

/**
 * removes current content from monitor and updates content management
 * returns current content type
 */
CONTENT_TYPE MonitorContentProvider::removeContent(MonitorGadget *monitor) {
	CONTENT_TYPE type = monitor->getContentType();
	if (type != INVALID) {
		m_content[type]->stop();
		MonitorContent *content = monitor->removeContent();
		// empty content must be deleted because it is not unique
		if (type == EMPTY) {
			delete content;
			content = NULL;
		} else {
			m_monitor[type] = NULL;
			m_isUsed[type] = false;
		}
	}
	return type;
}

/**
 * removes certain content from using monitor and updates content management
 * also the monitor is actualized e.g. enabling/disabling buttons
 * returns monitor that used type
 */
MonitorGadget * MonitorContentProvider::freeContent(CONTENT_TYPE type) {
	MonitorGadget *monitor = NULL;
	m_content[type]->stop();
	if (m_isUsed[type]) {
		if (m_monitor[type] != NULL) {
			monitor = m_monitor[type];
			m_monitor[type]->removeContent();
			m_monitor[type] = NULL;
			m_isUsed[type] = false;
		} else {
			QMessageBox::critical(0, tr("MonitorContentProvider"),
					tr("Empty monitor[type] in freeContent()"),
					QMessageBox::Ok);
			return NULL;
		}
	}
	return monitor;
}

void MonitorContentProvider::setConfiguration(MonitorConfiguration *config) {
	m_config = config;
	// update all configurations for static content
	for (int i = 0; i < MAX_CONTENT; i++) {
		// ensure that content is initialized
		if (m_content[i] != NULL) {
			m_content[i]->stop();
			m_content[i]->applyConfiguration(m_config);
			m_content[i]->start();
		}
	}
}
