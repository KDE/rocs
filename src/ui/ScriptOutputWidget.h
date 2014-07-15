/*
    This file is part of Rocs.
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef SCRIPTOUTPUTWIDGET_H
#define SCRIPTOUTPUTWIDGET_H

#include <EngineModules/Console/ConsoleModule.h>
#include <QWidget>
#include "ui_ScriptOutputWidget.h"

/**
 * \class ScriptOutputWidget
 *
 * This widget displays output and debug messages from a ConsoleInterface object.
 * Add this widget to your UI, add a ConsoleInterface object and registert that object at your
 * script engine.
 */
class ScriptOutputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptOutputWidget(QWidget *parent = 0);
    void setConsoleInterface(ConsoleModule* console);
    ConsoleModule * consoleInterface() const;
    bool isOutputClearEnabled() const;

public slots:
    void unsetConsoleInterface();
    void appendOutput(ConsoleModule::MessageType type, const QString& message);
    void showDebugOutput(bool show = true);
    void clear();

private slots:
    void updateFixOutputButton();

private:
    ConsoleModule* _console;
    Ui::ScriptOutputWidget* ui;
};

#endif
