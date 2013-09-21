/**
 ******************************************************************************
 *
 * @file       monitorgadgetwidget.h
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

#ifndef MONITORGADGETWIDGET_H
#define MONITORGADGETWIDGET_H

#include <QtGui/QWidget>

#include "ui_monitorgadgetwidget.h"
#include "monitorcontent.h"

class MonitorGadget;
class QLayout;

class MonitorGadgetWidget: public QWidget {
	Q_OBJECT

public:
	MonitorGadgetWidget(QWidget *parent = 0);
	void setMonitor(MonitorGadget *monitor);
	void initialize();
	~MonitorGadgetWidget();

	CONTENT_TYPE getContentType() const {return (m_content ? m_content->type() : INVALID);}
	MonitorContent *getContent() const {return m_content;}
	void setContent(MonitorContent* content);
	MonitorContent *removeContent();

private:
	int displayID;
	Ui_MonitorGadgetWidget *m_widget;
	MonitorContent *m_content;
	MonitorGadget *m_monitor; // TODO schauen, wie man den hier vermeiden kann

	void connectButtons(); // connect all buttons to their slot
	void handleButtons(); // configure Buttons
	void disableSelectionButton(CONTENT_TYPE selection); // disable button for selected content
	void handleZoomButtons(BUTTONSET buttons);
	void handleCursorButtons(BUTTONSET buttons);
	void handleModeButtons(BUTTONSET buttons);
	void handleRightTopButtons(BUTTONSET buttons);
	void handleRightMiddleButtons(BUTTONSET buttons);
	void handleRightBottomButtons(BUTTONSET buttons);

private slots:
	// the actions for the buttons - if possible define them here (inline)
	void buttonM1(){m_content->mode1();handleButtons();}
	void buttonM2(){m_content->mode2();handleButtons();}
	void buttonM3(){m_content->mode3();handleButtons();}

	void buttonZoomIn() {m_content->zoomIn();handleButtons();}
	void buttonZoomReset() {m_content->zoomReset();handleButtons();}
	void buttonZoomOut() {m_content->zoomOut();handleButtons();}

	void buttonUp(){m_content->moveUp();handleButtons();}
	void buttonDown(){m_content->moveDown();handleButtons();}
	void buttonReturn(){m_content->doReturn();handleButtons();}
	void buttonLeft(){m_content->moveLeft();handleButtons();}
	void buttonRight(){m_content->moveRight();handleButtons();}

	void buttonSAT();
	void buttonNAV();
	void buttonGPS();
	void buttonCAM();
	void buttonOff();

	void buttonRT1(){m_content->doRT1();handleButtons();}
	void buttonRT2(){m_content->doRT2();handleButtons();}
	void buttonRT3(){m_content->doRT3();handleButtons();}

	void buttonRM1(){m_content->doRM1();handleButtons();}
	void buttonRM2(){m_content->doRM2();handleButtons();}
	void buttonRM3(){m_content->doRM3();handleButtons();}

	void buttonRB1(){m_content->doRB1();handleButtons();}
	void buttonRB2(){m_content->doRB2();handleButtons();}
	void buttonRB3(){m_content->doRB3();handleButtons();}
	void buttonRB4(){m_content->doRB4();handleButtons();}
	void buttonRB5(){m_content->doRB5();handleButtons();}
};

#endif // MONITORGADGETWIDGET_H
