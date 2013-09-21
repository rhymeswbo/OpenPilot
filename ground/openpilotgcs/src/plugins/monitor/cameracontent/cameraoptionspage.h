/**
 ******************************************************************************
 *
 * @file       cameraoptionspage.h
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

#ifndef CAMERAOPTIONSPAGE_H
#define CAMERAOPTIONSPAGE_H

#include <coreplugin/dialogs/ioptionspage.h>

#include <QtCore/QObject>
#include <QtGui/QKeySequence>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QKeyEvent>

namespace Core {
class IUAVGadgetConfiguration;
}

namespace Ui {
class MapOptionsPage;
}

using namespace Core;

class CameraConfiguration;

//QT_BEGIN_NAMESPACE
class Ui_CameraOptionsPage;
//QT_END_NAMESPACE

class CameraOptionsPage : public Core::IOptionsPage {
    Q_OBJECT

public:
    explicit CameraOptionsPage(CameraConfiguration *config = 0, QObject *parent = 0);
   ~CameraOptionsPage();

    // IOptionsPage
    QString id() const;
    QString trName() const;
    QString category() const;
    QString trCategory() const;

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();

private:
    CameraConfiguration *m_config;
    Ui_CameraOptionsPage *m_page;

};

#endif // CAMERAOPTIONSPAGE_H
