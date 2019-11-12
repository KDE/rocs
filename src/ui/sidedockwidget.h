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

#ifndef SIDEDOCKWIDGET_H
#define SIDEDOCKWIDGET_H

#include <QDockWidget>
#include <QToolButton>
#include <QHash>
#include <QButtonGroup>

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
    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

/**
 * \class SidedockWidget
 *
 * Docking widget for side panel.
 */
class SidedockWidget: public QWidget
{
    Q_OBJECT

public:
    explicit SidedockWidget(QWidget* parent);
    QToolBar * toolbar() const;
    void addDock(QWidget* dock, const QString& title, const QIcon& icon);
    void showDock(bool show, int nr);

signals:
    void visibilityChanged(bool visible);

private:
    QToolBar *_toolBar;
    QButtonGroup *_btnGroup;
    QList<QWidget*> _widgets;
    bool _showDock;
};


#endif
