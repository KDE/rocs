/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef SIDEDOCKWIDGET_H
#define SIDEDOCKWIDGET_H

#include <QButtonGroup>
#include <QDockWidget>
#include <QToolButton>

class QIcon;
class QToolBar;
class QAction;

/**
 * \class SideToolButton
 *
 * Used for vertical tool button bars.
 */
class SideToolButton : public QToolButton
{
    Q_OBJECT

public:
    explicit SideToolButton(QWidget *parent = nullptr);
    Qt::Orientation orientation() const;
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
};

/**
 * \class SidedockWidget
 *
 * Docking widget for side panel.
 */
class SidedockWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SidedockWidget(QWidget *parent);
    QToolBar *toolbar() const;
    void addDock(QWidget *dock, const QString &title, const QIcon &icon);

private:
    QToolBar *_toolBar{nullptr};
    QButtonGroup *_btnGroup{nullptr};
    QList<QWidget *> _widgets;
};

#endif
