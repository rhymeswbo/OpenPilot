/**
 ******************************************************************************
 *
 * @file       monitoroptionspage.cpp
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

#include "monitoroptionspage.h"
#include "monitorconfiguration.h"
#include "mapcontent/mapconfiguration.h"
#include "mapcontent/mapoptionspage.h"

#include <QtGui/QLabel>

#include "ui_monitoroptionspage.h"

MonitorOptionsPage::MonitorOptionsPage(MonitorConfiguration *config, QObject *parent) :
    IOptionsPage(parent),
    m_config(config)
{
	//	MapOptionsPage *page = new MapOptionsPage(
	//			(qobject_cast<MonitorConfiguration *>(config))->mapConfig(),
	//			this->parent);

}

MonitorOptionsPage::~MonitorOptionsPage() {
}


QString MonitorOptionsPage::id() const {
	return QLatin1String("General");
}

QString MonitorOptionsPage::trName() const {
	return tr("General");
}

QString MonitorOptionsPage::category() const {
	return QLatin1String("Monitor");
}

QString MonitorOptionsPage::trCategory() const {
	return tr("Monitor");
}

QWidget *MonitorOptionsPage::createPage(QWidget *parent)
{
    m_page = new Ui::MonitorOptionsPage();
    QWidget *w = new QWidget(parent);
    m_page->setupUi(w);

    return w;
}

void MonitorOptionsPage::apply()
{
//    m_config->setNavigationMapProvider(m_page->navProviderComboBox->currentText());
}

void MonitorOptionsPage::finish()
{
    delete m_page;
}
