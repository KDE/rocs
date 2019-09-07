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

ScriptOutputWidget::ScriptOutputWidget(QWidget *parent)
    : QWidget(parent)
{
    ui = new Ui::ScriptOutputWidget;
    ui->setupUi(this);

    // set icon
    ui->buttonEnableDebugOutput->setIcon(QIcon::fromTheme("tools-report-bug"));
    ui->buttonDisableClear->setIcon(QIcon::fromTheme("document-decrypt"));
    ui->buttonClear->setIcon(QIcon::fromTheme("edit-clear-list"));

    connect(ui->buttonEnableDebugOutput, &QPushButton::clicked, this, &ScriptOutputWidget::showDebugOutput);
    connect(ui->buttonDisableClear, &QPushButton::clicked, this, &ScriptOutputWidget::updateFixOutputButton);
    connect(ui->buttonClear, &QPushButton::clicked, this, &ScriptOutputWidget::clear);
}

void ScriptOutputWidget::updateFixOutputButton()
{
    ui->buttonDisableClear->setIcon(
        QIcon::fromTheme(
            ui->buttonDisableClear->isChecked()
            ? QStringLiteral("document-encrypt")
            : QStringLiteral("document-decrypt")));
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

void ScriptOutputWidget::processMessage(const QString& message, Kernel::MessageType type)
{
    switch(type)
    {
    case Kernel::MessageType::InfoMessage:
        ui->txtOutput->append(message);
        ui->dbgOutput->append("<i>" + message + "</i>");
        break;
    case Kernel::MessageType::WarningMessage:
        ui->dbgOutput->append("<span style=\"color: green\">" + message + "</span>");
        break;
    case Kernel::MessageType::ErrorMessage:
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
