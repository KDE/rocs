/*
    kopeteplugin.cpp - Kopete Plugin API

    Copyright (c) 2001-2002 by Duncan Mac-Vicar P. <duncan@kde.org>
    Copyright (c) 2002-2004 by Olivier Goffart  <ogoffart @tiscalinet.be>

    Copyright (c) 2002-2004 by the Kopete developers  <kopete-devel@kde.org>

    *************************************************************************
    *                                                                       *
    * This library is free software; you can redistribute it and/or         *
    * modify it under the terms of the GNU Lesser General Public            *
    * License as published by the Free Software Foundation; either          *
    * version 2 of the License, or (at your option) any later version.      *
    *                                                                       *
    *************************************************************************
*/

#include "ToolsPluginInterface.h"


#include <kplugininfo.h>
#include "PluginManager.h"


namespace Rocs{

class  ToolsPluginInterface::Private{
public:
	KPluginInfo *_pluginInfo;
	QStringList addressBookFields;
	QString indexField;
};

ToolsPluginInterface::ToolsPluginInterface( const KComponentData& instance, QObject* parent )
: QObject( parent ),KXMLGUIClient(), d(new Private)
{
	setComponentData( instance );

}

ToolsPluginInterface::~ToolsPluginInterface()
{
	delete d;
}

QString ToolsPluginInterface::pluginId() 
{
	return QString::fromLatin1( metaObject()->className() );
}


QString ToolsPluginInterface::displayName() 
{
	return pluginInfo().isValid() ? pluginInfo().name() : QString();
}

QString ToolsPluginInterface::pluginIcon() 
{
	return pluginInfo().isValid() ? pluginInfo().icon() : QString();
}


KPluginInfo ToolsPluginInterface::pluginInfo()  
{
    return PluginManager::instance()->pluginInfo(this);
// 	return *(d->_pluginInfo);
}


}

