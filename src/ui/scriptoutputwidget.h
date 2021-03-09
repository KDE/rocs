/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
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
