/**
 ******************************************************************************
 * @file
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @see        The GNU Public License (GPL) Version 3
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup uavsettingsmanagerplugin
 * @{
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

#ifndef UAVSETTINGSMANAGERGADGET_H_
#define UAVSETTINGSMANAGERGADGET_H_

#include <coreplugin/iuavgadget.h>
#include "uavsettingsmanagergadgetwidget.h"

class IUAVGadget;
class QWidget;
class QString;
class UAVSettingsManagerGadgetWidget;

using namespace Core;

class UAVSETTINGSMANAGER_EXPORT UAVSettingsManagerGadget : public Core::IUAVGadget
{
    Q_OBJECT
public:
    UAVSettingsManagerGadget(QString classId, UAVSettingsManagerGadgetWidget *widget, QWidget *parent = 0);
    ~UAVSettingsManagerGadget();

    QWidget *widget() {
        return m_widget;
    }
    void loadConfiguration(IUAVGadgetConfiguration* config);

private:
    UAVSettingsManagerGadgetWidget *m_widget;
};


#endif // UAVSETTINGSMANAGERGADGET_H_

/**
 * @}
 * @}
 */
