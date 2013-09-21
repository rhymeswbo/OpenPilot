/**
 ******************************************************************************
 *
 * @file       cameraoptionspage.cpp
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

#include "cameraoptionspage.h"
#include "ui_cameraoptionspage.h"
#include "cameraconfiguration.h"

CameraOptionsPage::CameraOptionsPage(CameraConfiguration *config, QObject *parent) :
		IOptionsPage(parent), m_config(config) {
}

CameraOptionsPage::~CameraOptionsPage() {
}

QString CameraOptionsPage::id() const {
	return QLatin1String("VCameras");
}

QString CameraOptionsPage::trName() const {
	return tr("VCameras");
}

QString CameraOptionsPage::category() const {
	return QLatin1String("Monitor");
}

QString CameraOptionsPage::trCategory() const {
	return tr("Monitor");
}

QWidget *CameraOptionsPage::createPage(QWidget *parent) {
	m_page = new Ui_CameraOptionsPage();
	QWidget *w = new QWidget(parent);
	m_page->setupUi(w);

	return w;
}

void CameraOptionsPage::apply() {
}

void CameraOptionsPage::finish() {
	if (m_page)
		delete m_page;
	m_page = NULL;
}
