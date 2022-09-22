/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "scriptoutputwidget.h"
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
        QIcon::fromTheme(ui->buttonDisableClear->isChecked() ? QStringLiteral("document-encrypt") : QStringLiteral("document-decrypt")));
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

void ScriptOutputWidget::processMessage(const QString &message, Kernel::MessageType type)
{
    switch (type) {
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
