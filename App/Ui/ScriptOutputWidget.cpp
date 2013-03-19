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

#include "ScriptOutputWidget.h"
#include "DocumentManager.h"
#include <QWidget>
#include <QtScriptBackend.h>

ScriptOutputWidget::ScriptOutputWidget(QWidget* parent)
    : QWidget(parent),
    _engine(0)
{
    ui = new Ui::ScriptOutputWidget;
    ui->setupUi(this);

    // set icon
    ui->buttonEnableDebugOutput->setIcon(KIcon("tools-report-bug"));
    ui->buttonDisableClear->setIcon(KIcon("document-decrypt"));
    ui->buttonClear->setIcon(KIcon("edit-clear-list"));

    connect(ui->buttonEnableDebugOutput, SIGNAL(clicked(bool)), this, SLOT(showDebugOutput(bool)));
    connect(ui->buttonDisableClear, SIGNAL(clicked(bool)), this, SLOT(updateFixOutputButton()));
    connect(ui->buttonClear, SIGNAL(clicked(bool)), this, SLOT(clear()));
    connect(&DocumentManager::self(), SIGNAL(documentRemoved(Document*)), this, SLOT(unsetEngine()));
}

void ScriptOutputWidget::unsetEngine()
{
    if (_engine) {
        _engine->disconnect(this);
    }
    _engine = 0;
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

void ScriptOutputWidget::updateFixOutputButton()
{
    if (ui->buttonDisableClear->isChecked() == true) {
        ui->buttonDisableClear->setIcon(KIcon("document-encrypt"));
    }
    else {
        ui->buttonDisableClear->setIcon(KIcon("document-decrypt"));
    }
}

void ScriptOutputWidget::clear()
{
    ui->dbgOutput->clear();
    ui->txtOutput->clear();
}

void ScriptOutputWidget::setEngine(QtScriptBackend* engine)
{
    unsetEngine();
    _engine = engine;

    connect(engine, SIGNAL(sendDebug(QString)), this,  SLOT(appendDebugOutput(QString)));
    connect(engine, SIGNAL(scriptError()), this, SLOT(showDebugOutput()));
    connect(engine, SIGNAL(sendOutput(QString)), this, SLOT(appendOutput(QString)));
}

bool ScriptOutputWidget::isOutputClearEnabled() const
{
    return !ui->buttonDisableClear->isChecked();
}

void ScriptOutputWidget::appendOutput(const QString& string)
{
    ui->txtOutput->append(string);
}

void ScriptOutputWidget::appendDebugOutput(const QString& string)
{
    ui->dbgOutput->append(string);
}
