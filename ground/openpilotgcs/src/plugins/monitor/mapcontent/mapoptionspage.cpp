/**
 ******************************************************************************
 *
 * @file       mapoptionspage.cpp
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

#include "mapoptionspage.h"
#include "ui_mapoptionspage.h"
#include "mapconfiguration.h"

#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QFileDialog>

#include "opmapcontrol/opmapcontrol.h"
#include "utils/pathutils.h"

//using namespace Core;
//using namespace Core::Internal;

MapOptionsPage::MapOptionsPage(MapConfiguration *config, QObject *parent) :
		IOptionsPage(parent), m_config(config) {
}

MapOptionsPage::~MapOptionsPage() {
}

QString MapOptionsPage::id() const {
	return QLatin1String("Maps");
}

QString MapOptionsPage::trName() const {
	return tr("Maps");
}

QString MapOptionsPage::category() const {
	return QLatin1String("Monitor");
}

QString MapOptionsPage::trCategory() const {
	return tr("Monitor");
}

QWidget *MapOptionsPage::createPage(QWidget *parent) {
	int index;

	m_page = new Ui_MapOptionsPage();
	QWidget *w = new QWidget(parent);
	m_page->setupUi(w);
	// populate the map provider combobox
	m_page->navProviderComboBox->clear();
	m_page->navProviderComboBox->addItems(mapcontrol::Helper::MapTypes());

	m_page->envProviderComboBox->clear();
	m_page->envProviderComboBox->addItems(mapcontrol::Helper::MapTypes());

	index = m_page->navProviderComboBox->findText(
			m_config->navigationMapProvider());
	index = (index >= 0) ? index : 0;
	m_page->navProviderComboBox->setCurrentIndex(index);

	index = m_page->envProviderComboBox->findText(
			m_config->environmentMapProvider());
	index = (index >= 0) ? index : 0;
	m_page->envProviderComboBox->setCurrentIndex(index);

	m_page->latitudeSpinBox->setValue(m_config->latitude());
	m_page->longitudeSpinBox->setValue(m_config->longitude());

	m_page->checkBoxUseOpenGL->setChecked(m_config->useOpenGL());

	// populate the access mode combobox
	m_page->accessModeComboBox->clear();
	m_page->accessModeComboBox->addItems(mapcontrol::Helper::AccessModeTypes());

	index = m_page->accessModeComboBox->findText(m_config->accessMode());
	index = (index >= 0) ? index : 0;
	m_page->accessModeComboBox->setCurrentIndex(index);

	m_page->checkBoxUseMemoryCache->setChecked(m_config->useMemoryCache());

	m_page->lineEditCacheLocation->setExpectedKind(
			Utils::PathChooser::Directory);
	m_page->lineEditCacheLocation->setPromptDialogTitle(
			tr("Choose Cache Directory"));
	m_page->lineEditCacheLocation->setPath(m_config->cacheLocation());

	// populate UAV picture combo box
	QDir dir(":/uavs/images/");
	QStringList list = dir.entryList();
	foreach(QString i, list) {
		QIcon icon(QPixmap(":/uavs/images/" + i));

		m_page->uavSymbolComboBox->addItem(icon, QString(), i);
	}
	for (int x = 0; x < m_page->uavSymbolComboBox->count(); ++x) {
		if (m_page->uavSymbolComboBox->itemData(x).toString()
				== m_config->uavSymbol()) {
			m_page->uavSymbolComboBox->setCurrentIndex(x);
		}
	}

	// connect the default button for Server and Cache
	connect(m_page->pushButtonCacheDefaults, SIGNAL(clicked()), this,
			SLOT(on_pushButtonCacheDefaults_clicked()));

	return w;
}

void MapOptionsPage::apply() {
	m_config->setNavigationMapProvider(
			m_page->navProviderComboBox->currentText());
	m_config->setEnvironmentMapProvider(
			m_page->envProviderComboBox->currentText());
	m_config->setLatitude(m_page->latitudeSpinBox->value());
	m_config->setLongitude(m_page->longitudeSpinBox->value());
	m_config->setUseOpenGL(m_page->checkBoxUseOpenGL->isChecked());
	m_config->setAccessMode(m_page->accessModeComboBox->currentText());
	m_config->setUseMemoryCache(m_page->checkBoxUseMemoryCache->isChecked());
	m_config->setCacheLocation(m_page->lineEditCacheLocation->path());
	m_config->setUavSymbol(
			m_page->uavSymbolComboBox->itemData(
					m_page->uavSymbolComboBox->currentIndex()).toString());
}

void MapOptionsPage::finish() {
	if (m_page)
		delete m_page;
	m_page = NULL;
}

void MapOptionsPage::on_pushButtonCacheDefaults_clicked() {
	int index = m_page->accessModeComboBox->findText("ServerAndCache");

	index = (index >= 0) ? index : 0;
	m_page->accessModeComboBox->setCurrentIndex(index);

	m_page->checkBoxUseMemoryCache->setChecked(true);
	m_page->lineEditCacheLocation->setPath(
			Utils::PathUtils().GetStoragePath() + "mapscache"
					+ QDir::separator());
}
