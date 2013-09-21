/**
 ******************************************************************************
 *
 * @file       monitorgadgetfactory.cpp
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
#include "monitorgadgetfactory.h"

#include "monitorgadgetwidget.h"
#include "monitorgadget.h"
#include "monitorconfiguration.h"
#include "monitorcontentprovider.h"

#include "monitoroptionspage.h"

MonitorGadgetFactory::MonitorGadgetFactory(QObject *parent) :
		IUAVGadgetFactory(QString("Monitor"), tr("Monitor"), parent) {
}

MonitorGadgetFactory::~MonitorGadgetFactory() {
}

Core::IUAVGadget* MonitorGadgetFactory::createGadget(QWidget *parent) {
	MonitorGadgetWidget* gadgetWidget = new MonitorGadgetWidget(parent);
	MonitorGadget *gadget = new MonitorGadget(QString("Monitor"), gadgetWidget, parent);
	return gadget;
}

IUAVGadgetConfiguration *MonitorGadgetFactory::createConfiguration(
		QSettings *qSettings) {
	return new MonitorConfiguration(QString("Monitor"), qSettings);
}

IOptionsPage *MonitorGadgetFactory::createOptionsPage(
		IUAVGadgetConfiguration *config) {
	return NULL;
//	return new MonitorOptionsPage(qobject_cast<MonitorConfiguration *>(config),
//			this->parent());
}
