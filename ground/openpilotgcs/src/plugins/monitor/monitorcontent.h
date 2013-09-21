/**
 ******************************************************************************
 *
 * @file       monitorcontent.h
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

#ifndef MONITORCONTENT_H
#define MONITORCONTENT_H

#include <QTimer>
#include <QtGui/QGraphicsView>

class MonitorConfiguration;

enum CONTENT_TYPE {
	INVALID = -1,
	SAT,
	NAV,
	GPS,
	CAM,
	EMPTY // EMPTY must be always last element!!
};
#define MAX_CONTENT EMPTY

// defines binary values for buttons
enum BUTTONS {
	DISABLE_BUTTONS = 0,
	B1 = 1,
	B2 = 2,
	B3 = 4,
	B4 = 8,
	B5 = 16,
	B6 = 32,
	B7 = 64,
	B8 = 128
};
#define BUTTONSET char

class MonitorContent: public QObject {
	Q_OBJECT
public:
	MonitorContent();
	MonitorContent(CONTENT_TYPE type);
	virtual ~MonitorContent();
	void start() {m_updateTimer->start();}
	void stop() {m_updateTimer->stop();}

public slots:
	virtual BUTTONSET getZoomButtons() { return 0; }
	virtual void zoomIn() {}
	virtual void zoomReset() {}
	virtual void zoomOut() {}
	virtual BUTTONSET getCursorButtons() { return 0; }
	virtual void moveUp() {}
	virtual void moveDown() {}
	virtual void moveLeft() {}
	virtual void moveRight() {}
	virtual void doReturn() {}
	virtual BUTTONSET getModeButtons() { return 0; }
	virtual void mode1() {}
	virtual void mode2() {}
	virtual void mode3() {}
	virtual BUTTONSET getRightTopButtons() { return 0; }
	virtual void doRT1() {}
	virtual void doRT2() {}
	virtual void doRT3() {}
	virtual BUTTONSET getRightMiddleButtons() { return 0; }
	virtual void doRM1() {}
	virtual void doRM2() {}
	virtual void doRM3() {}
	virtual BUTTONSET getRightBottomButtons() { return 0; }
	virtual void doRB1() {}
	virtual void doRB2() {}
	virtual void doRB3() {}
	virtual void doRB4() {}
	virtual void doRB5() {}

	virtual void applyConfiguration(MonitorConfiguration *config);
	virtual QGraphicsView *display() = 0; // {return NULL;}

	CONTENT_TYPE type() {return m_type;}

protected:
	CONTENT_TYPE m_type;
	QTimer *m_updateTimer;
	int m_updateRate;	// milliseconds
};
#endif // MONITORCONTENT_H
