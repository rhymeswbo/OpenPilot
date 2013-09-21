/**
 ******************************************************************************
 *
 * @file       monitorgadgetwidget.cpp
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

#include "monitorgadgetwidget.h"

#include "monitorgadget.h"
#include "monitorcontent.h"

static int displayCounter = 0;

MonitorGadgetWidget::MonitorGadgetWidget(QWidget *parent) :
		QWidget(parent) {
	// inits
	m_content = NULL;
	m_monitor = NULL;
	// give a number to the monitor as an ID
	displayID = displayCounter++;

	// create the monitor widget from definition file
	m_widget = new Ui_MonitorGadgetWidget();
	m_widget->setupUi(this);

	// connect all buttons to their methods
	connectButtons();
}

void MonitorGadgetWidget::setMonitor(MonitorGadget *monitor) {
	m_monitor = monitor;
}

void MonitorGadgetWidget::initialize() {
	if (m_monitor == NULL)
		return;
	// first content is dependant on ID. Should be configurable
	if (displayID == 0)
		buttonSAT();
	else if (displayID == 1)
		buttonNAV();
	else
		buttonOff();
}

MonitorGadgetWidget::~MonitorGadgetWidget() {
	// remove content from widget
}

/**
 * Connect all Buttons to their methods
 */
void MonitorGadgetWidget::connectButtons() {
	connect(m_widget->pushButtonM1, SIGNAL(clicked()), this,
			SLOT(buttonM1()));
	connect(m_widget->pushButtonM2, SIGNAL(clicked()), this,
			SLOT(buttonM2()));
	connect(m_widget->pushButtonM3, SIGNAL(clicked()), this,
			SLOT(buttonM3()));

	connect(m_widget->pushButtonZoomIn, SIGNAL(clicked()), this,
			SLOT(buttonZoomIn()));
	connect(m_widget->pushButtonZoomReset, SIGNAL(clicked()), this,
			SLOT(buttonZoomReset()));
	connect(m_widget->pushButtonZoomOut, SIGNAL(clicked()), this,
			SLOT(buttonZoomOut()));

	connect(m_widget->pushButtonUp, SIGNAL(clicked()), this,
			SLOT(buttonUp()));
	connect(m_widget->pushButtonDown, SIGNAL(clicked()), this,
			SLOT(buttonDown()));
	connect(m_widget->pushButtonReturn, SIGNAL(clicked()), this,
			SLOT(buttonReturn()));
	connect(m_widget->pushButtonLeft, SIGNAL(clicked()), this,
			SLOT(buttonLeft()));
	connect(m_widget->pushButtonRight, SIGNAL(clicked()), this,
			SLOT(buttonRight()));

	connect(m_widget->pushButtonSAT, SIGNAL(clicked()), this,
			SLOT(buttonSAT()));
	connect(m_widget->pushButtonNAV, SIGNAL(clicked()), this,
			SLOT(buttonNAV()));
	connect(m_widget->pushButtonGPS, SIGNAL(clicked()), this,
			SLOT(buttonGPS()));
	connect(m_widget->pushButtonCAM, SIGNAL(clicked()), this,
			SLOT(buttonCAM()));
	connect(m_widget->pushButtonOff, SIGNAL(clicked()), this,
			SLOT(buttonOff()));

	connect(m_widget->pushButtonRB1, SIGNAL(clicked()), this,
			SLOT(buttonRB1()));
	connect(m_widget->pushButtonRB2, SIGNAL(clicked()), this,
			SLOT(buttonRB2()));
	connect(m_widget->pushButtonRB3, SIGNAL(clicked()), this,
			SLOT(buttonRB3()));
	connect(m_widget->pushButtonRB4, SIGNAL(clicked()), this,
			SLOT(buttonRB4()));
	connect(m_widget->pushButtonRB5, SIGNAL(clicked()), this,
			SLOT(buttonRB5()));

	connect(m_widget->pushButtonRM1, SIGNAL(clicked()), this,
			SLOT(buttonRM1()));
	connect(m_widget->pushButtonRM2, SIGNAL(clicked()), this,
			SLOT(buttonRM2()));
	connect(m_widget->pushButtonRM3, SIGNAL(clicked()), this,
			SLOT(buttonRM3()));

	connect(m_widget->pushButtonRT1, SIGNAL(clicked()), this,
			SLOT(buttonRT1()));
	connect(m_widget->pushButtonRT2, SIGNAL(clicked()), this,
			SLOT(buttonRT2()));
	connect(m_widget->pushButtonRT3, SIGNAL(clicked()), this,
			SLOT(buttonRT3()));

	return;
}

// TODO beim entfernen bleibt noch was stehen - das muss noch korrigiert werden
// endgültig verschwinden tut der inhalt erst, wenn er von einem anderen widget angezogen wird
MonitorContent *MonitorGadgetWidget::removeContent() {
	MonitorContent *retVal = m_content;
	if (m_content && m_widget) {
		m_widget->monitorLayout->removeWidget(m_content->display());
		m_widget->monitorLayout->update();
	}
	m_content = NULL;
	handleButtons();
	return retVal;
}

void MonitorGadgetWidget::setContent(MonitorContent* content) {
	m_content = content;
	if (m_content && m_widget) {
		m_widget->monitorLayout->addWidget(m_content->display());
		m_widget->monitorLayout->update();
	}
	handleButtons();
	return;
}

// TODO Slots eventuell zusammenfassen mit Parameter int type
// das scheint nicht zu funktionieren (erster einfacher Versuch ging schief)

void MonitorGadgetWidget::buttonSAT() {
	MonitorContent *c = m_monitor->setContent(SAT);
	if (c) {
		m_content = c;
	}
	return;
}

void MonitorGadgetWidget::buttonNAV() {
	MonitorContent *c = m_monitor->setContent(NAV);
	if (c) {
		m_content = c;
	}
	return;
}

void MonitorGadgetWidget::buttonGPS() {
	MonitorContent *c = m_monitor->setContent(GPS);
	if (c) {
		m_content = c;
	}
	return;
}

void MonitorGadgetWidget::buttonCAM() {
	MonitorContent *c = m_monitor->setContent(CAM);
	if (c) {
		m_content = c;
	}
	return;
}

void MonitorGadgetWidget::buttonOff() {
	MonitorContent *c = m_monitor->setContent(EMPTY);
	if (c) {
		m_content = c;
	}
	return;
}

/**
 * Sets all the buttons to enabled or disabled corresponding to the MonitorContent shown
 */
void MonitorGadgetWidget::handleButtons() {
	if(m_content) {
		disableSelectionButton(m_content->type());
		handleZoomButtons(m_content->getZoomButtons());
		handleCursorButtons(m_content->getCursorButtons());
		handleModeButtons(m_content->getModeButtons());
		handleRightTopButtons(m_content->getRightTopButtons());
		handleRightMiddleButtons(m_content->getRightMiddleButtons());
		handleRightBottomButtons(m_content->getRightBottomButtons());
	} else {
		disableSelectionButton(EMPTY);
		handleZoomButtons(DISABLE_BUTTONS);
		handleCursorButtons(DISABLE_BUTTONS);
		handleModeButtons(DISABLE_BUTTONS);
		handleRightTopButtons(DISABLE_BUTTONS);
		handleRightMiddleButtons(DISABLE_BUTTONS);
		handleRightBottomButtons(DISABLE_BUTTONS);
	}
}

/**
 * Takes care of zoom buttons
 */
void MonitorGadgetWidget::handleZoomButtons(BUTTONSET buttons) {
	m_widget->pushButtonZoomIn->setEnabled(buttons & B1);
	m_widget->pushButtonZoomReset->setEnabled(buttons & B2);
	m_widget->pushButtonZoomOut->setEnabled(buttons & B3);
	return;
}

/**
 * Takes care of cursor buttons
 */
void MonitorGadgetWidget::handleCursorButtons(BUTTONSET buttons) {
	m_widget->pushButtonUp->setEnabled(buttons & B1);
	m_widget->pushButtonDown->setEnabled(buttons & B2);
	m_widget->pushButtonReturn->setEnabled(buttons & B3);
	m_widget->pushButtonLeft->setEnabled(buttons & B4);
	m_widget->pushButtonRight->setEnabled(buttons & B5);
	return;
}

/**
 * Takes care of cursor buttons
 */
void MonitorGadgetWidget::handleModeButtons(BUTTONSET buttons) {
	m_widget->pushButtonM1->setEnabled(buttons & B1);
	m_widget->pushButtonM2->setEnabled(buttons & B2);
	m_widget->pushButtonM3->setEnabled(buttons & B3);
}

/**
 * Takes care of right top buttons
 */
void MonitorGadgetWidget::handleRightTopButtons(BUTTONSET buttons) {
	m_widget->pushButtonRT1->setEnabled(buttons & B1);
	m_widget->pushButtonRT2->setEnabled(buttons & B2);
	m_widget->pushButtonRT3->setEnabled(buttons & B3);
}

/**
 * Takes care of right middle buttons
 */
void MonitorGadgetWidget::handleRightMiddleButtons(BUTTONSET buttons) {
	m_widget->pushButtonRM1->setEnabled(buttons & B1);
	m_widget->pushButtonRM2->setEnabled(buttons & B2);
	m_widget->pushButtonRM3->setEnabled(buttons & B3);
}

/**
 * Takes care of right bottom buttons
 */
void MonitorGadgetWidget::handleRightBottomButtons(BUTTONSET buttons) {
	m_widget->pushButtonRB1->setEnabled(buttons & B1);
	m_widget->pushButtonRB2->setEnabled(buttons & B2);
	m_widget->pushButtonRB3->setEnabled(buttons & B3);
	m_widget->pushButtonRB4->setEnabled(buttons & B4);
	m_widget->pushButtonRB5->setEnabled(buttons & B5);
}

/**
 * Takes care of display selection buttons
 */
void MonitorGadgetWidget::disableSelectionButton(CONTENT_TYPE selection) {
	if (selection == SAT)
		m_widget->pushButtonSAT->setEnabled(false);
	else
		m_widget->pushButtonSAT->setEnabled(true);

	if (selection == NAV)
		m_widget->pushButtonNAV->setEnabled(false);
	else
		m_widget->pushButtonNAV->setEnabled(true);

	if (selection == GPS)
		m_widget->pushButtonGPS->setEnabled(false);
	else
		m_widget->pushButtonGPS->setEnabled(true);

	if (selection == CAM)
		m_widget->pushButtonCAM->setEnabled(false);
	else
		m_widget->pushButtonCAM->setEnabled(true);

	if (selection == EMPTY)
		m_widget->pushButtonOff->setEnabled(false);
	else
		m_widget->pushButtonOff->setEnabled(true);

	return;
}
