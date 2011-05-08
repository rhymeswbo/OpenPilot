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

#ifndef UAVSETTINGSMANAGERPLUGIN_H_
#define UAVSETTINGSMANAGERPLUGIN_H_

#include <extensionsystem/iplugin.h>
#include "uavsettingsmanager_global.h"

class UAVSettingsManagerGadgetFactory;

class UAVSETTINGSMANAGER_EXPORT UAVSettingsManagerPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT

public:
    UAVSettingsManagerPlugin();
    ~UAVSettingsManagerPlugin();

    void extensionsInitialized();
    bool initialize(const QStringList & arguments, QString * errorString);
    void shutdown();
private:
    UAVSettingsManagerGadgetFactory *mf;

private slots:
    void importExport();

};
#endif /* UAVSETTINGSMANAGERPLUGIN_H_ */
/**
 * @}
 * @}
 */
