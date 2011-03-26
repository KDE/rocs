/*
    Rocs-Tools-Plugin: Generate graphs by specific patterns
    Copyright (C) 2011  Andreas Cord-Landwehr

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef GENERATEGRAPHTOOLSPLUGIN_H
#define GENERATEGRAPHTOOLSPLUGIN_H

#include "../ToolsPluginInterface.h"

class QObject;

class GenerateGraphToolPlugin : public ToolsPluginInterface
{
    Q_OBJECT
//   Q_INTERFACES(ToolsPluginInterface)

public:
    GenerateGraphToolPlugin(QObject* parent, const QList< QVariant >&);
    ~GenerateGraphToolPlugin();
    QString run(QObject* parent = 0) const;
};

#endif // GENERATEGRAPHTOOLSPLUGIN_H
