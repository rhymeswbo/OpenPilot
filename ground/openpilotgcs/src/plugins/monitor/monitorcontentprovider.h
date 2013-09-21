/**
 ******************************************************************************
 *
 * @file       monitorcontentprovider.h
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

#ifndef MONITORCONTENTPROVIDER_H
#define MONITORCONTENTPROVIDER_H

#include <QObject>

#include "monitorcontent.h"

class MonitorGadget;
class MonitorConfiguration;

class MonitorContentProvider: public QObject {
	Q_OBJECT
public:
	// singleton
	static MonitorContentProvider *instance();
	MonitorContent *setContent(MonitorGadget *monitor, CONTENT_TYPE type);
	void setConfiguration(MonitorConfiguration *config);
	void applyConfiguration();

private:
	void applyContent(MonitorGadget *monitor, CONTENT_TYPE type);
	CONTENT_TYPE removeContent(MonitorGadget *monitor);
	MonitorGadget *freeContent(CONTENT_TYPE type);
	MonitorConfiguration *m_config;

	// static instance and private constructor/destructor for singleton
	static MonitorContentProvider *sInstance;
	MonitorContentProvider();
	~MonitorContentProvider();

	// arrays for content management
	MonitorContent *m_content[MAX_CONTENT];
	bool m_isUsed[MAX_CONTENT];
	MonitorGadget *m_monitor[MAX_CONTENT];
};
#endif // MONITORCONTENTPROVIDER_H
