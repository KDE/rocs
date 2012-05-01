/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010-2012      Wagner Reck <wagner.reck@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TOOLSPLUGININTERFACE_H
#define TOOLSPLUGININTERFACE_H

#include <kxmlguiclient.h>
#include <QtCore/QObject>
#include <kdemacros.h>
#include "rocslib_export.h"

#include <kplugininfo.h>

/**
 * @brief Base class for all rocs plugins.
 * This plugin system is based on the kopete's one
 * To create a plugin, you need to create a .desktop file which looks like that:
 * \verbatim
[Desktop Entry]
Encoding=UTF-8
Type=Service
X-Kopete-Version=1000900
Icon=icon
ServiceTypes=Kopete/Plugin
X-KDE-Library=kopete_myplugin
X-KDE-PluginInfo-Author=Your Name
X-KDE-PluginInfo-Email=your@mail.com
X-KDE-PluginInfo-Name=kopete_myplugin
X-KDE-PluginInfo-Version=0.0.1
X-KDE-PluginInfo-Website=http://yoursite.com
X-KDE-PluginInfo-Category=Plugins
X-KDE-PluginInfo-Depends=
X-KDE-PluginInfo-License=GPL
X-KDE-PluginInfo-EnabledByDefault=false
Name=MyPlugin
Comment=Plugin that do some nice stuff
 \endverbatim
 *
 * The constructor of your plugin should looks like this:
 *
 * \code
    typedef KGenericFactory<MyPlugin> MyPluginFactory;
    static const KAboutData aboutdata("kopete_myplugin", 0, ki18n("MyPlugin") , "1.0" );
    K_EXPORT_COMPONENT_FACTORY( kopete_myplugin, MyPluginFactory( &aboutdata )  )

    MyPlugin::MyPlugin( QObject *parent, const char *name, const QStringList &  args  )
        : Kopete::Plugin( MyPluginFactory::componentData(), parent, name )
    {
        //...
    }
 \endcode
  */

class ROCSLIB_EXPORT ToolsPluginInterface : public QObject, public KXMLGUIClient
{
    Q_OBJECT

public:
    ToolsPluginInterface(const KComponentData &instance, QObject* parent);
    virtual ~ToolsPluginInterface();

    /**
     * Returns the KPluginInfo object associated with this plugin
     */
    KPluginInfo pluginInfo();

    /**
     * Get the name of the icon for this plugin. The icon name is taken from the
     * .desktop file.
     *
     * May return an empty string if the .desktop file for this plugin specifies
     * no icon name to use.
     *
     * This is a convenience method that simply calls @ref pluginInfo()->icon().
     */
    QString pluginIcon() ;

    /**
     * Returns the display name of this plugin.
     *
     * This is a convenience method that simply calls @ref pluginInfo()->name().
     */
    QString displayName() ;

    /**
     * @brief Get the plugin id
     * @return the plugin's id which is gotten by calling QObject::metaObject()->className().
     */
    QString pluginId() ;
    
    /** @brief returns the set o suported data structures
     * If this plugin only will be actived if this set is empty
     * (backwards compatibility) or if active data struture internaName
     * is contanied in it.
     * To set this added following line to you .desktop
     * \verbatim
     * X-Rocs-SupportedDataStructures=DS1,DS2
     * \endverbatim
     */ 
    QStringList supportedDataStructures();

    /** @brief Implements this function and return the script to run
    * @param document is actual Document (graph).
    */
    virtual QString run(QObject * document) const  = 0;
    
private:
    class Private;
    Private * const d;
};

#endif
