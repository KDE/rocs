/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef SCRIPTAPIWIDGET_H
#define SCRIPTAPIWIDGET_H

#include "ui_scriptapiwidget.h"
#include <QWidget>

class QUrl;
class ScriptApiManager;
class ScriptApiModel;

/**
 * \class ScriptApiWidget
 * This Widget loads API information about all script interfaces and displays them.
 */
class ScriptApiWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Default constructor.
     */
    explicit ScriptApiWidget(QWidget *parent);
    void showHtmlOutline(bool logHistory);

public Q_SLOTS:
    void showTreeOutline();
    void showHtmlOutline();
    void showDetails(const QModelIndex &index);
    void showObjectApi(const QString &id, bool logHistory);
    void showObjectApi(const QUrl &aliasPage);
    void historyGoBack();
    void historyGoForward();

private:
    ScriptApiManager *m_manager;
    ScriptApiModel *m_model;
    QUrl m_baseUrl;
    QList<QString> m_history;
    int m_historyPointer;
    Ui::ScriptApiWidget *ui;
};

#endif
