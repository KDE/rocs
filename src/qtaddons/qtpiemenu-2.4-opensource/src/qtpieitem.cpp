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

#include "qtpieitem.h"
#include "qtpiemenu.h"

#include <QtCore/QString>
#include <QtGui/QIcon>

/*!
    \class QtPieItem
    \brief The QtPieItem class is the base class for QtPieMenu.

    It has a very simple interface, providing the base implementation
    of functions common for QtPieMenu and QtPieAction.

    The text of an item is set with setText() and retrieved with
    text(). Similarly, setIcon() and icon() set and get the icon set
    associated with the item.

    QtPieMenu uses weight() to determine how much space an item should
    claim on a pie. The weights of the items are compared so that an
    item with a weight of 2 claims twice as much space as one of
    weight 1. Use setWeight() to set the weight of an item.

    type() returns the type of the item. This is used by QtPieMenu to
    determine wheter an item triggers a simple action or whether it
    pops up a submenu.
*/

/*!
    Constructs a QtPieItem. The \a title and \a weight
    arguments are stored as member data. The \a parent argument is
    passed to QWidget's constructor.
*/
QtPieItem::QtPieItem(const QString &title, unsigned int weight, QWidget *parent)
    : QWidget(parent, Qt::Popup), t(title), e(true), w(weight)
{
    setText(title);
}

/*! 
    Destructs a QtPieItem.
*/
QtPieItem::~QtPieItem()
{
}

/*! 
    Constructs a QtPieItem. The \a icon, \a text and \a weight arguments are
    stored as member data. The \a parent argument is passed to QWidget's
    constructor.
*/
QtPieItem::QtPieItem(const QIcon &icon, const QString &text, unsigned int weight, QWidget *parent)
    : QWidget(parent, Qt::Popup), i(icon), e(true), w(weight)
{
    setText(text);
}

/*! 
    Sets the item's text to \a text.
*/
void QtPieItem::setText(const QString &text)
{    
    t = text;    
    if (text.length() > t.replace(QRegExp(QLatin1String("<[^>/]*br[^>]*>")), " ").length() || 
        text.length() > t.replace(QRegExp(QLatin1String("<[^>/]*p[^>]*>")), " ").length() || 
        text.length() > t.remove(QRegExp(QLatin1String("<[^>]+>"))).length())
        qWarning("QtPieItem::setText: HTML support not implemented until Qt 4.1. All tags removed from text.");    

}

/*! 
    Returns the title of this QtPieItem.
*/
QString QtPieItem::text() const
{
    return t;
}

/*! 
    Sets the item's icon to \a icon.
*/
void QtPieItem::setIcon(const QIcon &icon)
{
    i = icon;
}

/*! 
    Returns a reference to the icon for this item.
*/
QIcon QtPieItem::icon() const
{
    return i;
}

/*! 
    Sets the item's weight to \a weight.
*/
void QtPieItem::setWeight(int weight)
{
    w = weight;
}

/*! 
    Returns the weight of the item.
*/
int QtPieItem::weight() const
{
    return w;
}

/*! 
    Returns \e true if this menu is enabled, otherwise returns \e false.
*/
bool QtPieItem::isEnabled() const
{
    return e;
}

/*! 
    Enables this item if \a enabled is \e true, otherwise disables
    this item.
*/
void QtPieItem::setEnabled(bool enabled)
{
    e = enabled;
}

/*! 
    Returns the type of node as a QtPieItem::Type. Valid values are
    Invalid, Action and SubMenu.
*/
int QtPieItem::type() const
{
    return QtPieMenu::Invalid;
}
