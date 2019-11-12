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
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SCRIPTOUTPUTWIDGET_H
#define SCRIPTOUTPUTWIDGET_H

#include "ui_scriptoutputwidget.h"
#include "libgraphtheory/kernel/kernel.h"
#include <QWidget>

/**
 * \class ScriptOutputWidget
 *
 * This widget displays output and debug messages from a ConsoleInterface object.
 * Add this widget to your UI, add a ConsoleInterface object and register that object at your
 * script engine.
 */
class ScriptOutputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptOutputWidget(QWidget *parent = 0);
    bool isOutputClearEnabled() const;

public Q_SLOTS:
    void processMessage(const QString &message, GraphTheory::Kernel::MessageType type);
    void showDebugOutput(bool show = true);
    void clear();

private Q_SLOTS:
    void updateFixOutputButton();

private:
    Ui::ScriptOutputWidget* ui;
};

#endif
