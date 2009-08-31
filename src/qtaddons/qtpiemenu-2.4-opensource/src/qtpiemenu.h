/****************************************************************************
**
** This file is part of a Qt Solutions component.
** 
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
** 
** Contact:  Qt Software Information (qt-info@nokia.com)
** 
** Commercial Usage  
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage 
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** Please note Third Party Software included with Qt Solutions may impose
** additional restrictions and it is the user's responsibility to ensure
** that they have met the licensing requirements of the GPL, LGPL, or Qt
** Solutions Commercial license and the relevant license of the Third
** Party Software they are using.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** 
****************************************************************************/

//depot/addons/main/widgets/qtpiemenu/src/qtpiemenu.h#31 - edit change 282028 (text)
#ifndef QTPIEMENU_H
#define QTPIEMENU_H

#include "qtpieitem.h"

#include <QtCore/QTimer>
#include <QtGui/QPixmap>
#include <QtGui/QWidget>
#include <QtCore/QEvent>
#include <QtGui/QIcon>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtCore/QSize>

class QPainter;
class QtPieMenu;
class QBitmap;
class QPaintEvent;
class QMouseEvent;
class QShowEvent;
class QHideEvent;
class QKeyEvent;

#if defined(Q_WS_WIN)
#  if !defined(QT_QTPIEMENU_EXPORT) && !defined(QT_QTPIEMENU_IMPORT)
#    define QT_QTPIEMENU_EXPORT
#  elif defined(QT_QTPIEMENU_IMPORT)
#    if defined(QT_QTPIEMENU_EXPORT)
#      undef QT_QTPIEMENU_EXPORT
#    endif
#    define QT_QTPIEMENU_EXPORT __declspec(dllimport)
#  elif defined(QT_QTPIEMENU_EXPORT)
#    undef QT_QTPIEMENU_EXPORT
#    define QT_QTPIEMENU_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTPIEMENU_EXPORT
#endif

class QT_QTPIEMENU_EXPORT QtPieMenu : public QtPieItem
{
    Q_OBJECT
    Q_PROPERTY(int innerRadius READ innerRadius WRITE setInnerRadius)
    Q_PROPERTY(int outerRadius READ outerRadius WRITE setOuterRadius)

public:
    QtPieMenu(const QString &title, QWidget *parent = 0,
	      const char *name = 0, 
          uint innerRad = 15, uint outerRad = 75);
    QtPieMenu(const QIcon &icon, QWidget *parent = 0,
	      const char *name = 0, 
          uint innerRad = 15, uint outerRad = 75);
    QtPieMenu(const QIcon &icon, const QString &title,
	      QWidget *parent = 0, const char *name = 0,
          uint innerRad = 15, uint outerRad = 75);
    virtual ~QtPieMenu();

    void setItemText(const QString &text, int index);
    QString itemText(int index) const;

    void setItemIcon(const QIcon &icon, int index);
    QIcon itemIcon(int index) const;

    void setItemWeight(int weight, int index);
    int itemWeight(int index) const;

    void setItemEnabled(bool enabled, int index);
    bool isItemEnabled(int index) const;

    void popup(const QPoint &pos);
    int count() const;

    virtual int indexAt(const QPoint &pos);

    void insertItem(const QIcon &icon,
		    QObject *receiver, const char *member, int index = -1);
    void insertItem(const QString &text,
		    QObject *receiver, const char *member, int index = -1);
    void insertItem(const QIcon &icon, const QString &text,
		    QObject *receiver, const char *member, int index = -1);
    void insertItem(QtPieMenu *item, int index = -1);

    QtPieMenu *subMenuAt(int index) const;
    void removeItemAt(int index);
    void clear();

    void setHighlightedItem(int index);
    int highlightedItem();

    void setInnerRadius(int r);
    int innerRadius() const;

    void setOuterRadius(int r);
    int outerRadius() const;

    bool isShaded() const;

    enum ItemType {
        Invalid,
        SubMenu,
        Action
    };

    int itemType(int index) const;

    QSize sizeHint() const;

Q_SIGNALS:
    void activated();
    void activated(int);
    void highlighted(int);
    void canceled();
    void canceledAll();
    void aboutToShow();
    void aboutToHide();

protected Q_SLOTS:
    void hoverAlert();
    void subMenuCanceled();
    void subMenuSelected();
    void allCanceled();

protected:
    virtual void deleteItems(int index = -1);
    virtual void generateMask(QBitmap *mask);
    virtual void reposition();

    double radiusAt(const QPoint &pos) const;
    double angleAt(const QPoint &pos) const;
    double indexToAngle(int item, int total, bool shifted = false);
    int angleToIndex(double rad, int total);

    void init(const QString &name);

    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);

    enum ActivateReason {
	    MousePress = 0x01,
	    MouseRelease = 0x02,
	    KeyPress = 0x04,
	    Hovering = 0x08,
	    Dragging = 0x10
    };

    void activateItem(int reason);

    bool isTopLevelMenu() const;

    bool dragging() const;
    void hideShownSubMenu();

    int type() const;

    QPixmap pix;

private:
    QtPieItem *highlightedItemPtr();
    QtPieItem *shownItemPtr();
    QtPieItem *itemAt(int index) const;

    QList<QtPieItem *> items;

    int motion;
    QTimer hoverTimer;
    QPoint lastMousePos;

    int innerRad, outerRad;

    int hItem;
    int shownSubMenu;

    bool shaded;

    bool ignoreNextMouseReleaseIfNotDragging;

    int lastActivatedItem;
    bool syncMenu;
    bool hasMask;
};

#endif
