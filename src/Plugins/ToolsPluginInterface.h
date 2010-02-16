
#ifndef TOOLSPLUGININTERFACE_H
#define TOOLSPLUGININTERFACE_H
/*
#include <QtPlugin>

class QWidget;
class QString;
class QStringList;

class ToolsPluginInterface{
//   Q_OBJECT
public:
   virtual ~ToolsPluginInterface() {} 
   virtual QString name() const = 0; //Retorna nome que vai no menu
   virtual QString tooltip() const = 0; //Dica qnd mouse fica em cima.
   virtual QString about() const = 0; 
   virtual QString category() = 0; //Categoria para separar futuros plugins
   virtual QStringList authors() const = 0; 
public slots:
   virtual QString run(QWidget *parent=0) const = 0; //função principal, pode chamar dialogos e interagir com o usuário e devolve um script que será rodado.
};

Q_DECLARE_INTERFACE(ToolsPluginInterface,
                     "org.kde.kdeedu.ToolsPluginInterface/0.1")
   */                  
// #endif
		
		/*
    kopeteplugin.h - Kopete Plugin API

    Copyright (c) 2001-2002 by Duncan Mac-Vicar Prett <duncan@kde.org>
    Copyright (c) 2002-2003 by Martijn Klingens       <klingens@kde.org>
    Copyright (c) 2002-2005 by Olivier Goffart        <ogoffart@kde.org>

    Copyright (c) 2002-2003 by the Kopete developers  <kopete-devel@kde.org>

    *************************************************************************
    *                                                                       *
    * This library is free software; you can redistribute it and/or         *
    * modify it under the terms of the GNU Lesser General Public            *
    * License as published by the Free Software Foundation; either          *
    * version 2 of the License, or (at your option) any later version.      *
    *                                                                       *
    *************************************************************************
*/

// #ifndef KOPETEPLUGIN_H
// #define KOPETEPLUGIN_H

#include <kxmlguiclient.h>
#include <QtCore/QObject>
#include <kdemacros.h>
#include "rocslib_export.h"
// #include "kopete_export.h"

class KPluginInfo;
namespace Rocs{

/**
 * @brief Base class for all plugins or protocols.
 *
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
	ToolsPluginInterface(const KComponentData &instance, QObject* parent );
	virtual ~ToolsPluginInterface();

	/**
	 * Returns the KPluginInfo object associated with this plugin
	 */
	KPluginInfo pluginInfo() const;

	/**
	 * Get the name of the icon for this plugin. The icon name is taken from the
	 * .desktop file.
	 *
	 * May return an empty string if the .desktop file for this plugin specifies
	 * no icon name to use.
	 *
	 * This is a convenience method that simply calls @ref pluginInfo()->icon().
	 */
	QString pluginIcon() const;

	/**
	 * Returns the display name of this plugin.
	 *
	 * This is a convenience method that simply calls @ref pluginInfo()->name().
	 */
	QString displayName() const;

	/**
	 * @brief Get the plugin id
	 * @return the plugin's id which is gotten by calling QObject::metaObject()->className().
	 */
	QString pluginId() const;

	/** @brief Implements this function and return the script to run
	* @param document is actual Document (graph).
	*/
	virtual QString run (QObject * document) const  = 0;
private:
	class Private;
	Private * const d;
};



};
#endif
