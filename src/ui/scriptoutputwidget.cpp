/*
 *  Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License or (at your option) version 3 or any later version
 *  accepted by the membership of KDE e.V. (or its successor approved
 *  by the membership of KDE e.V.), which shall act as a proxy
 *  defined in Section 14 of version 3 of the license.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "scriptoutputwidget.h"
#include <QWidget>
#include <QDebug>
#include <QIcon>

using namespace GraphTheory;

ScriptOutputWidget::ScriptOutputWidget(QWidget* parent)
    : QWidget(parent),
    m_console(0)
{
    ui = new Ui::ScriptOutputWidget;
    ui->setupUi(this);

    // set icon
    ui->buttonEnableDebugOutput->setIcon(QIcon::fromTheme("tools-report-bug"));
    ui->buttonDisableClear->setIcon(QIcon::fromTheme("document-decrypt"));
    ui->buttonClear->setIcon(QIcon::fromTheme("edit-clear-list"));

    connect(ui->buttonEnableDebugOutput, SIGNAL(clicked(bool)), this, SLOT(showDebugOutput(bool)));
    connect(ui->buttonDisableClear, SIGNAL(clicked(bool)), this, SLOT(updateFixOutputButton()));
    connect(ui->buttonClear, SIGNAL(clicked(bool)), this, SLOT(clear()));
}

void ScriptOutputWidget::unsetConsoleInterface()
{
    if (m_console) {
        m_console->disconnect(this);
    }
    m_console = 0;
}

void ScriptOutputWidget::setConsoleInterface(ConsoleModule* console)
{
    unsetConsoleInterface();
    m_console = console;

    connect(console, SIGNAL(backlogChanged(ConsoleModule::MessageType,QString)),
            this, SLOT(appendOutput(ConsoleModule::MessageType,QString)));
}

ConsoleModule * ScriptOutputWidget::consoleInterface() const
{
    return m_console;
}

void ScriptOutputWidget::updateFixOutputButton()
{
    if (ui->buttonDisableClear->isChecked() == true) {
        ui->buttonDisableClear->setIcon(QIcon::fromTheme("document-encrypt"));
    }
    else {
        ui->buttonDisableClear->setIcon(QIcon::fromTheme("document-decrypt"));
    }
}

void ScriptOutputWidget::clear()
{
    ui->dbgOutput->clear();
    ui->txtOutput->clear();
}

bool ScriptOutputWidget::isOutputClearEnabled() const
{
    return !ui->buttonDisableClear->isChecked();
}

void ScriptOutputWidget::appendOutput(ConsoleModule::MessageType type, const QString& message)
{
    switch(type)
    {
    case ConsoleModule::Log:
        ui->txtOutput->append(message);
        ui->dbgOutput->append("<i>" + message + "</i>");
        break;
    case ConsoleModule::Debug:
        ui->dbgOutput->append("<span style=\"color: green\">" + message + "</span>");
        break;
    case ConsoleModule::Error:
        ui->txtOutput->append("<b style=\"color: red\">" + message + "</b>");
        ui->dbgOutput->append("<b style=\"color: red\">" + message + "</b>");
        break;
    default:
        qWarning() << "Unknown message type, aborting printing.";
    }
}

void ScriptOutputWidget::showDebugOutput(bool show)
{
    if (show && !ui->buttonEnableDebugOutput->isChecked()) {
        ui->buttonEnableDebugOutput->toggle();
    }
    if (!show && ui->buttonEnableDebugOutput->isChecked()) {
        ui->buttonEnableDebugOutput->toggle();
    }
    if (show) {
        ui->output->setCurrentIndex(1);
    } else {
        ui->output->setCurrentIndex(0);
    }
}
