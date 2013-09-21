/**
 ******************************************************************************
 *
 * @file       monitorgadget.h
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

#ifndef MONITORGADGET_H
#define MONITORGADGET_H

#include <coreplugin/iuavgadget.h>
#include "monitorgadgetwidget.h"
#include "monitorcontentprovider.h"
#include "monitorcontent.h"
//#include "monitorconfiguration.h"

// we only need to know that these classes exist
class QWidget;
class QString;
class MonitorConfiguration;

using namespace Core;

class MonitorGadget: public Core::IUAVGadget {
	Q_OBJECT
public:
	MonitorGadget(QString classId, MonitorGadgetWidget *widget,	QWidget *parent = 0);
	~MonitorGadget();
	void loadConfiguration(IUAVGadgetConfiguration* config);

	CONTENT_TYPE getContentType() const {return (m_widget ? m_widget->getContentType() : INVALID);}
	void setContent(MonitorContent *content) {m_widget->setContent(content);}
	MonitorContent *getContent() const {return (m_widget ? m_widget->getContent() : NULL);}
	MonitorContent *setContent(CONTENT_TYPE newType) {return MonitorContentProvider::instance()->setContent(this, newType);}
	MonitorContent *removeContent() {return m_widget->removeContent();}

	// virtual method from IUAVGadget
	MonitorGadgetWidget *widget() {return m_widget;}

private:
	MonitorGadgetWidget *m_widget;
	MonitorConfiguration *m_config;
};

#endif // MONITORGADGET_H
