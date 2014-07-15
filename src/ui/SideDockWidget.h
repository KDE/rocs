/*
    This file is part of Rocs.
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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


#ifndef SIDEDOCKWIDGET_H
#define SIDEDOCKWIDGET_H

#include <QDockWidget>
#include <QToolButton>
#include <QHash>

class QIcon;
class QToolBar;
class QAction;

/**
 * \class SideToolButton
 *
 * Used for vertical tool button bars.
 */
class SideToolButton: public QToolButton
{
    Q_OBJECT

public:
    explicit SideToolButton(QWidget* parent = 0);
    Qt::Orientation orientation() const;
    virtual QSize sizeHint() const;

protected:
    virtual void paintEvent(QPaintEvent *event);
};

/**
 * \class SideDockWidget
 *
 * Docking widget for side panel.
 */
class SideDockWidget: public QWidget
{
    Q_OBJECT

public:
    explicit SideDockWidget(QWidget* parent);
    QToolBar * toolbar() const;
    void addDock(QWidget* dock, const QString& title, const QIcon& icon);
    void showDock(bool show, QWidget* widget);

public slots:
    void buttonToggled(bool state);

signals:
    void visibilityChanged(bool visible);

private:
    QToolBar *_toolBar;
    QHash<SideToolButton*, QWidget*> _widgets;
    bool _showDock;
};


#endif // SIDEDOCKWIDGET_H
