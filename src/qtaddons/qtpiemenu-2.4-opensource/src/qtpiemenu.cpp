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
#include "qtpieaction.h"

#include <QtGui/QApplication>
#include <QtGui/QImage>
#include <QtGui/QBitmap>
#include <QtCore/QPointer>
#include <QtGui/QCursor>
#include <QtGui/QStyle>
#include <QtGui/QPainter>
#include <QtGui/QIcon>
#include <QtGui/QTextLayout>
#include <QtGui/QLayout>
#include <QtGui/QDesktopWidget>
#include <QtGui/QKeyEvent>
#include <QtGui/QPalette>
#include <QtGui/QFontMetrics>

#include <math.h>
#include <stdio.h>

const double PI = 3.14159265358979323846264338327950288419717;
const double TWOPI = 2.0*PI;

enum MouseButtonStatus {
    Pressed,
    Released
};

static MouseButtonStatus mouseButtonStatus = Released;

/*! \class QtPieMenu qtpiemenu.h

    \brief The QtPieMenu class provides a pie menu popup widget.

    A pie menu is a popup menu that is usually invoked as a context
    menu, and that supports several forms of navigation.

    Using conventional navigation, menu items can be highlighted
    simply by moving the mouse over them, or by single-clicking them,
    or by using the keyboard's arrow keys. Menu items can be chosen
    (invoking a submenu or the menu item's action), by clicking a
    highlighted menu item, or by pressing \key{Enter}.

    Pie menus can also be navigated using \e{gestures}.
    Gesture navigation is where a menu item is chosen as the result of
    moving the mouse in a particular sequence of movements, for
    example, up-left-down, without the user having to actually
    read the menu item text.

    The user can cancel a pie menu in the conventional way by clicking
    outside of the pie menu, or by clicking the center of the pie (the
    "cancel zone"), or by pressing \key{Esc}.

    Pie menus are faster to navigate with the mouse than conventional
    menus because all the menu items are available at an equal
    distance from the origin of the mouse pointer.

    The circular layout and the length of the longest menu text
    imposes a natural limit on how many items can be displayed at a
    time. For this reason, submenus are very commonly used.

    Use popup() to pop up the pie menu. Note that QtPieMenu is
    different from QPopupMenu in that it pops up with the mouse cursor
    in the center of the menu (i.e. over the cancel zone), instead of
    having the cursor at the top left corner.

    Pie menus can only be used as popup menus; they cannot be used as
    normal widgets.

    A pie menu contains of a list of items, where each item is
    displayed as a slice of a pie. Items are added with insertItem(),
    and are displayed with a richtext text label, an icon or with both.
    The items are laid out automatically, starting from the top at
    index 0 and going counter clockwise. Each item can either pop up a
    submenu, or perform an action when activated. To get an item at
    a particular position, use itemAt().

    When inserting action items, you specify a receiver and a slot.
    The slot is invoked in the receiver when the action is activated.
    In the following example, a pie menu is created with three items:
    "Open" and "Close" are actions, and "New" pops up a submenu. The
    submenu has two items, "New project" and "New dialog", which are
    both actions.

    \code
    Editor::Editor(QWidget *parent, const char *name)
      : QTextEdit(parent)
    {
        setObjectName(name);

        // Create a root menu and insert two action items.
        QtPieMenu *rootMenu = new QtPieMenu(tr("Root menu"), this);
        rootMenu->insertItem(tr("Open"), storage, SLOT(open()));
        rootMenu->insertItem(tr("Close"), storage, SLOT(close());

        // Now create a submenu and insert two action items.
        QtPieMenu *subMenu = new QtPieMenu(tr("New"), rootMenu);
        subMenu->insertItem(tr("New project"), formEditor, SLOT(newProject()));
        subMenu->insertItem(tr("New dialog"), formEditor, SLOT(newDialog()));

        // Finally add the submenu to the root menu.
        rootMenu->insertItem(subMenu);
    }
    \endcode

    By default, each slice of the pie takes up an equal amount of
    space.  Sometimes it can be useful to have certain slices take up
    more space than others. QtPieItem::setItemWeight() changes an item's
    space weighting, which by default is 1. QtPieMenu uses the
    weightings when laying out the pie slices. Each slice gets a share
    of the size proportional to its weight.

    \img weighted.png

    At the center of a pie menu is a cancel zone, which cancels the
    menu if the user clicks in it. The radius of the whole pie menu
    and of the cancel zone are set in the constructor, or with
    setOuterRadius() and setInnerRadius().

    Any shape or layout of items is possible by subclassing QtPieMenu
    and reimplementing paintEvent(), indexAt(), generateMask() and
    reposition().

    \img qtpiemenu.png
*/

/*! \enum QtPieMenu::ActivateReason

    This enum describes the reason for the activation of an item in a
    pie menu.

    \value MousePress The activation was caused by a mouse button press.
    \value MouseRelease The activation was caused by a mouse button release.
    \value KeyPress The activation was caused by a key press.
    \value Hovering The activation was caused by hovering the mouse pointer
        over an item.
    \value Dragging The activation was caused by dragging.

    \sa activateItem()
*/

/*! \enum QtPieMenu::ItemType

    \value Invalid The item has an invalid type or doesn't exist; for
      example if QtPiePenu::itemType() is called with an invalid
      index.

    \value SubMenu Activating the item will pop up a submenu.
    \value Action Activating this item will trigger an action.
*/

/*! \fn QtPieMenu::activated()

    This signal is emitted when a pie menu action item is activated,
    causing the popup menu to hide. It is used internally by
    QtPieMenu. Most users will find activated(int) more useful.
*/

/*! \fn QtPieMenu::activated(int i)

    \overload

    This signal is emitted when the action item at index position \a i
    in the pie menu's list of menu items is activated.

    \sa highlighted()
*/

/*! \fn QtPieMenu::highlighted(int i)

    This signal is emitted when the item at index position \a i in the
    pie menu's list of menu items is highlighted.

    \sa activated()
*/

/*! \fn QtPieMenu::canceled()

    This signal is emitted when the pie menu is canceled. Only the
    menu that emits this signal is canceled; any parent menus are
    still shown.

    \sa canceledAll()
*/

/*! \fn QtPieMenu::canceledAll()

    This signal is emitted when the pie menu is canceled in such a way
    that the entire menu (including submenus) hides. For example, this
    would happen if the user clicked somewhere outside all the pie
    menus.

    \sa canceled()
*/

/*! \fn QtPieMenu::aboutToShow()

    This signal is emitted just before the pie menu is displayed. You
    can connect it to any slot that sets up the menu contents (e.g. to
    ensure that the correct items are enabled).

    \sa aboutToHide(), insertItem(), removeItemAt()
*/

/*!
    \fn void QtPieMenu::aboutToHide()

    This signal is emitted just before the pie menu is hidden after it
    has been displayed.

    \warning Do not open a widget in a slot connected to this signal.

    \sa aboutToShow(), insertItem(), removeItemAt()
*/


/*!
    Constructs a pie menu. The \a innerRadius argument is the radius
    of the cancel zone at the center of the pie. The \a outerRadius is
    the radius of the outer edge of the pie.

    If this pie menu is a submenu, the richtext \a text argument is
    displayed by its parent menu.

    The \a parent argument is passed on to QWidget's
    constructor. The object name is set to the \a name argument.
*/
QtPieMenu::QtPieMenu(const QString &text, QWidget *parent, const char *name,
                             uint innerRadius, uint outerRadius)
      :  QtPieItem(text, 1, parent),
        pix(outerRadius * 2 + 1, outerRadius * 2 + 1), hoverTimer(this),
        innerRad(innerRadius), outerRad(outerRadius)
{
    init(name);
}

/*!
    Constructs a pie menu. The \a innerRadius argument is the radius
    of the cancel zone at the center of the pie. The \a outerRadius is
    the radius of the outer edge of the pie.

    If this pie menu is a submenu, the \a icon argument is displayed
    by its parent menu.

    The \a parent argument is passed on to QWidget's
    constructor. The object name is set to the \a name argument.
*/
QtPieMenu::QtPieMenu(const QIcon &icon, QWidget *parent, const char *name,
                     uint innerRadius, uint outerRadius)
             : QtPieItem(icon, QString(), 1, parent),
               pix(outerRadius * 2 + 1, outerRadius * 2 + 1), hoverTimer(this),
               innerRad(innerRadius), outerRad(outerRadius)

{
    init(name);
}

/*!
    Constructs a pie menu. The \a innerRadius argument is the radius
    of the cancel zone at the center of the pie. The \a outerRadius is
    the radius of the outer edge of the pie.

    If this pie menu is a submenu, the \a icon and richtext \a text
    arguments are displayed by its parent menu.

    The \a parent argument is passed on to QWidget's
    constructor. The object name is set to the \a name argument.
*/
QtPieMenu::QtPieMenu(const QIcon &icon, const QString &text, QWidget *parent,
                     const char *name, uint innerRadius, uint outerRadius)
             : QtPieItem(icon, text, 1, parent),
                pix(outerRadius * 2 + 1, outerRadius * 2 + 1), hoverTimer(this),
                innerRad(innerRadius), outerRad(outerRadius)
{
    init(name);
}

/*! \internal

    Creates the pie mask and initializes the pie menu.
*/
void QtPieMenu::init(const QString &name)
{
#if defined(QTSOLUTIONS_BUILDEVAL)
    void checkEval();
    checkEval();
#endif
    hasMask = false;
    setMouseTracking(true);

    hItem = -2;
    shownSubMenu = -1;
    shaded = false;
    ignoreNextMouseReleaseIfNotDragging = false;
    lastActivatedItem = -1;
    syncMenu = false;

    setObjectName(name);

    connect(&hoverTimer, SIGNAL(timeout()), SLOT(hoverAlert()));
}

/*!
    Destructs the pie menu.
*/
QtPieMenu::~QtPieMenu()
{
    deleteItems();
}

/*!
    Sets the text of the pie menu item at position \a index to \a
    text. Currently QtPieMenu only supports plain text, and HTML tags will be stripped
    from \a text.

    \code
        pieMenu->setItemText("Undo", 0);
    \endcode

    \sa itemText() setItemIcon()
*/
void QtPieMenu::setItemText(const QString &text, int index)
{
    if (!itemAt(index)) {
        qWarning("QtPieMenu::setItemText(\"%s\", %i) index of of range", text.toLatin1().constData(), index);
        return;
    }

    itemAt(index)->setText(text);
}

/*!
    Returns the text of the pie menu item at position \a index. The
    string may be a rich text string.

    \sa setItemText() itemIcon()
*/
QString QtPieMenu::itemText(int index) const
{
    if (!itemAt(index)) {
        qWarning("QtPieMenu::itemText(%i) index of of range", index);
        return "";
    }

    return itemAt(index)->text();
}

/*!
    Sets the icon of the pie menu item at position \a index to \a
    icon.

    \sa itemIcon() setItemText()
*/
void QtPieMenu::setItemIcon(const QIcon &icon, int index)
{
    if (!itemAt(index)) {
        qWarning("QtPieMenu::setItemIcon(%p, %i) index of of range", &icon, index);
        return;
    }

    itemAt(index)->setIcon(icon);
}

/*!
    Returns the icon set of the pie menu item at position \a index. If
    no icon has been set an invalid icon is returned.

    \sa setItemIcon() itemText()
*/
QIcon QtPieMenu::itemIcon(int index) const
{
    if (!itemAt(index)) {
        qWarning("QtPieMenu::itemIcon(%i) index of of range", index);
        return QIcon();
    }

    return itemAt(index)->icon();
}

/*!
    Sets the relative weight of the pie menu item at position \a index
    to \a weight. The weight of an item decides how big its slice of
    the pie menu will be. By default, all pie menu items have a weight
    of 1. If one item has a weight of 2, it will be twice as big as
    the other items.

    \img weighted.png

    The pie menu in this image has three pie menu items with icons
    and no text. The item at position 0 has a weight of 2, and the
    rest have the default weight of 1.

    \sa itemWeight()
*/
void QtPieMenu::setItemWeight(int weight, int index)
{
    if (!itemAt(index)) {
        qWarning("QtPieMenu::setItemWeight(%i, %i) index of of range", weight, index);
        return;
    }

    itemAt(index)->setWeight(weight);
}

/*!
    Returns the weight of the pie menu item at position \a index.

    \sa setItemWeight()
*/
int QtPieMenu::itemWeight(int index) const
{
    if (!itemAt(index)) {
        qWarning("QtPieMenu::itemWeight(%i) index of of range", index);
        return 0;
    }

    return itemAt(index)->weight();
}

/*!
    If \a enabled is true, this function enables the pie menu item at
    position \a index; otherwise the item is disabled. When an item is
    disabled, it will not respond to mouse clicks or releases. All
    all items are enabled by default.

    \sa isItemEnabled()
*/
void QtPieMenu::setItemEnabled(bool enabled, int index)
{
    if (!itemAt(index)) {
        qWarning("QtPieMenu::setItemEnabled(%s, %i) index of of range", enabled ? "true" : "false", index);
        return;
    }

    itemAt(index)->setEnabled(enabled);
}

/*!
    Returns true if the pie menu item at position \a index is enabled;
    otherwise returns false.

    \sa setItemEnabled()
*/
bool QtPieMenu::isItemEnabled(int index) const
{
    if (!itemAt(index)) {
        qWarning("QtPieMenu::isItemEnabled(%i) index of of range", index);
        return false;
    }

    return itemAt(index)->isEnabled();
}

/*! \internal

    Returns \e true if this is the top level menu, otherwise returns
    \e false.
*/
bool QtPieMenu::isTopLevelMenu() const
{
    return !parent()->inherits("QtPieMenu");
}

/*! \property QtPieMenu::innerRadius

    \brief the inner radius of the pie menu

    The radius in pixels of the cancel zone (in the center) of the pie
    menu. Setting the radius to 0 disables the cancel zone.

    \sa outerRadius
*/
void QtPieMenu::setInnerRadius(int r)
{
    innerRad = r;
}

int QtPieMenu::innerRadius() const
{
    return innerRad;
}

/*! \property QtPieMenu::outerRadius

    \brief the outer radius of the pie menu

    The outer radius of the pie menu in pixels.

    \sa innerRadius
*/
void QtPieMenu::setOuterRadius(int r)
{
    outerRad = r;
    pix = pix.scaled(outerRad * 2 + 1, outerRad * 2 + 1);
}

int QtPieMenu::outerRadius() const
{
    return outerRad;
}

/*! \internal

    Returns the type of QtPieItem. In this case, always returns SubMenu.
*/
int QtPieMenu::type() const
{
    return SubMenu;
}

/*!
    Displays the pie menu with the center of the pie at the global
    position \a pos.

    To translate a widget's contents coordinates into global
    coordinates, use QWidget::mapToGlobal().
*/
void QtPieMenu::popup(const QPoint &pos)
{
    hItem = indexAt(mapFromGlobal(QCursor::pos()));
    lastMousePos = pos;
    lastActivatedItem = -1;

    shaded = false;

    // Force whole pie menu to be inside desktop.
    int x = pos.x() - sizeHint().width() / 2;
    int y = pos.y() - sizeHint().height() / 2;

    const QRect screenGeometry = qApp->desktop()->screenGeometry(qApp->desktop()->screenNumber(pos));

    if (x < screenGeometry.left())
        x = screenGeometry.left();
    if (y < screenGeometry.top())
        y = screenGeometry.top();

    if (x + sizeHint().width() > screenGeometry.right())
            x = screenGeometry.right() - sizeHint().width();
    if (y + sizeHint().height() > screenGeometry.bottom())
            y = screenGeometry.bottom() - sizeHint().height();

    move(x, y);
    reposition();
    show();
}

/*
    Synchronously executes this pie menu.

    Pops up the pie menu with its center at the global position \a
    pos. (To translate a widget's local coordinates into global
    coordinates, use QWidget::mapToGlobal().)

    The return value is the index position of the selected item in
    either the popup menu or one of its submenus, or -1 if no item is
    selected (normally because the user pressed Esc).

    Note that all signals are emitted as usual. If you connect a menu
    item to a slot and call the menu's exec(), you get the result both
    via the signal-slot connection and in the return value of exec().

    Common usage is to position the popup at the current mouse position:

    \code
    exec(QCursor::pos());
    \endcode
*/
/*
int QtPieMenu::exec(const QPoint &pos)
{
    syncMenu = true;
    QGuardedPtr<QtPieMenu> that = this;
    popup(pos);
    qApp->enter_loop();
    if (that) {
        syncMenu = false;
        return lastActivatedItem;
    } else {
        return -1;
    }
}
*/

/*!
    \overload

    Adds the submenu \a item to this pie menu at position \a index.
    The items are ordered by their ordinal position, starting from the
    top of the pie and counting counter clockwise.

    \code
        QtPieMenu *subMenu = new QtPieMenu("Undo", this, SLOT(undo()));
        rootMenu->insertItem(subMenu);
    \endcode
*/
void QtPieMenu::insertItem(QtPieMenu *item, int index)
{
    // Out of bounds indexes are treated as appends.
    if (index < 0 || index > items.size())
            index = items.size();

    items.insert(index, item);

    connect(item, SIGNAL(activated()), SLOT(subMenuSelected()));
    connect(item, SIGNAL(canceled()), SLOT(subMenuCanceled()));
    connect(item, SIGNAL(canceledAll()), SLOT(allCanceled()));
}

/*!
    \overload

    Adds an action item to this pie menu at position \a index. The
    \a text is used as the item's text, or the text that is
    displayed on this item's slice in the pie. The \a receiver gets
    notified through the signal or slot in \a member when the item is
    activated.

    \code
        rootMenu->insertItem("Save", this, SLOT(save()));
    \endcode

    The items are ordered by their ordinal position, starting from the
    top of the pie and going counter clockwise.
*/
void QtPieMenu::insertItem(const QString &text, QObject *receiver,
                           const char *member, int index)
{
    QtPieAction *action = new QtPieAction(text, receiver, member);

    // Out of bounds indexes are treated as appends.
    if (index < 0 || index > int(items.size()))
            index = items.size();

    items.insert(index, action);
}

/*!
    Adds an action item to this pie menu at position \a index. The \a
    icon is displayed on this item's slice in the pie. The \a receiver
    gets notified through the signal or slot in \a member when the
    item is activated.

    \code
        QIcon icon(QPixmap("save.png"));
        rootMenu->insertItem(icon, this, SLOT(save()));
    \endcode

    The items are ordered by their ordinal position, starting from the
    top of the pie and going counter clockwise.
*/
void QtPieMenu::insertItem(const QIcon &icon, QObject *receiver, const char *member, int index)
{
    QtPieAction *action = new QtPieAction(icon, QString::null, receiver, member);

    // Out of bounds indexes are treated as appends.
    if (index < 0 || index > int(items.size()))
            index = items.size();

    items.insert(index, action);
}

/*!
    \overload

    Adds an action item to this pie menu at position \a index. The
    \a icons and \a text are used as the item's text
    and icon, or the text and icon that is displayed on this item's
    slice in the pie. The \a receiver gets notified through the signal
    or slot in \a member when the item is activated.

    \code
        QIcon icon(QPixmap("save.png"));
        rootMenu->insertItem(icon, "Save", this, SLOT(save()));
    \endcode

    The items are ordered by their ordinal position, starting from the
    top of the pie and going counter clockwise.
*/
void QtPieMenu::insertItem(const QIcon &icons, const QString &text,
                           QObject *receiver, const char *member, int index)
{
    QtPieAction *action = new QtPieAction(icons, text, receiver, member);

    // Out of bounds indexes are treated as appends.
    if (index < 0 || index > int(items.size()))
            index = items.size();

    items.insert(index, action);
}

/*!
    Returns a pointer to the item at position \a index if there is
    one; otherwise returns 0. This is useful if weights, texts or
    icons need to be changed at run-time.

    \code
        pieMenu->itemAt(0)->setText("Delete");
    \endcode
*/
QtPieItem *QtPieMenu::itemAt(int index) const
{
    // Out of bounds indexes are treated as appends.
    if (index < 0 || index > int(items.size()))
        return 0;

    return (static_cast<const QtPieMenu * >(this))->items[index];
}

/*!
    Returns the number of items in this pie menu.
*/
int QtPieMenu::count() const
{
    return items.size();
}

/*!
    Removes all the items from the pie menu.
*/
void QtPieMenu::clear()
{
    hideShownSubMenu();

    deleteItems();

    hItem = -2;
}

/*!
    If the item at position \a index is a submenu, a pointer to the
    corresponding QtPieMenu is returned; otherwise returns 0.
*/
QtPieMenu *QtPieMenu::subMenuAt(int index) const
{
    QtPieItem *item = (static_cast<const QtPieMenu *>(this))->items[index];
    if (item && item->type() == SubMenu)
        return static_cast<QtPieMenu *>(item);

    return 0;
}

/*!
    Removes the item at position \a index.
*/
void QtPieMenu::removeItemAt(int index)
{
    if (index >= int(items.size()) || index < 0) {
            qWarning("Attempt to remove item %i from QtPieMenu with %i items.",
                    index, items.size());
            return;
    }

    deleteItems(index);
}

/*!
    Highlights the item at position \a index, then schedules a display
    update.
*/
void QtPieMenu::setHighlightedItem(int index)
{
    hItem = index;
    update();
}

/*! \reimp
*/
QSize QtPieMenu::sizeHint() const
{
    return QSize(outerRad * 2 + 1, outerRad * 2 + 1);
}

/*!
    Draws the outline of the pie menu using the bitmap \a mask. This
    is used as a QWidget mask.

    When subclassing QtPieMenu, this function should be reimplemented
    if the shape of the new menu is different from QtPieMenu's shape.
*/
void QtPieMenu::generateMask(QBitmap *mask)
{
    QPainter p(mask);
    p.setPen(Qt::color1);
    p.setBrush(Qt::color1);
    p.drawEllipse(mask->rect().adjusted(0, 0, -1, -1));
}

/*!
    Repositions a pie menu. This function is called in popup() after
    the pie menu's initial position has been set. Reimplement this
    function when subclassing QtPieMenu to gain finer control of where
    submenus are positioned when popped up.

    The default implementation does nothing.
*/
void QtPieMenu::reposition()
{
}

/*! \reimp

    Notes on keyboard navigation: Move selection clockwise with the
    keys \key{Right}, \key{Down} and \key{Tab}. Move counter clockwise
    with \key{Left}, \key{Up} and \key{Backtab}. To select an item,
    use \key{Return}, \key{Enter} or \key{Space}. To cancel a menu,
    use \key{Backspace} or \key{Escape}.
*/
void QtPieMenu::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Right:
    case Qt::Key_Down:
    case Qt::Key_Tab:
            --hItem;
            if (hItem < 0)
                hItem += items.size();
            update();
            break;
    case Qt::Key_Left:
    case Qt::Key_Up:
    case Qt::Key_Backtab:
            ++hItem;
            if (hItem >= char(items.size()))
                hItem -= items.size();
            update();
            break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_Space:
            motion = 0;
            activateItem(KeyPress);
            if (shownSubMenu != -1) {
                ((QtPieMenu *)shownItemPtr())->setHighlightedItem(0);
            }

            break;
    case Qt::Key_Escape:
    case Qt::Key_Backspace:
        {
            emit aboutToHide();
            hide();
        }
            break;
    default:
        break;
    }
}

/*! \internal

    Handles mouse presses. A mouse press always activates or cancels a
    submenu.
*/
void QtPieMenu::mousePressEvent(QMouseEvent *e)
{
    hoverTimer.stop();

    mouseButtonStatus = Pressed;
    lastMousePos = mapFromGlobal(e->globalPos());

    double mousexdist = lastMousePos.x() - rect().center().x();
    double mouseydist = lastMousePos.y() - rect().center().y();
    double mouserad = sqrt(mousexdist * mousexdist + mouseydist * mouseydist);
    double mouseAngle = acos(mousexdist / mouserad);
    if (mouseydist >= 0)
            mouseAngle = TWOPI - mouseAngle;
    hItem = indexAt(lastMousePos);

    if (shaded &&  shownSubMenu != hItem)
        hideShownSubMenu();

    activateItem(MousePress);
    motion = 0;
}

/*! \internal

    Handles mouse releases. A mouse release will activate a menu in
    dragging mode.
*/
void QtPieMenu::mouseReleaseEvent(QMouseEvent *e)
{
    hoverTimer.stop();

    // Solve the case where a submenu is canceled by clicking inside
    // the cancel zone, and the next mouse release comes to us.
    if (ignoreNextMouseReleaseIfNotDragging) {
            ignoreNextMouseReleaseIfNotDragging = false;

            if (!dragging()) {
                mouseButtonStatus = Released;
                return;
            }
    }

    lastMousePos = mapFromGlobal(e->globalPos());
    hoverTimer.stop();

    if (!shaded)
        activateItem(MouseRelease | (dragging() ? Dragging : 0));

    mouseButtonStatus = Released;
    motion = 0;
}

/*! \internal

    Handles mouse move events. Mouse tracking is enabled, so we get
    these continuously. Monitors mouse position, distance from center
    and angle, and uses these to decide the section that is currently
    benieth the cursor, and wether we are crossing a section boundary.
*/
void QtPieMenu::mouseMoveEvent(QMouseEvent *e)
{
    lastMousePos = mapFromGlobal(e->globalPos());
    double mouseRad = radiusAt(lastMousePos);
    QtPieMenu* parentMenu = qobject_cast<QtPieMenu*>(parent());
    if (parentMenu && mouseRad > outerRad) {
        parentMenu->mouseMoveEvent(e); // pos() will be wrong, but is not used
        return;
    }

    ++motion;
    mouseButtonStatus = (e->buttons()) != 0 ? Pressed : Released;

    // Detect changes in highlighting. The logics are as follows: If
    // the mouse pointer is outside the cancel zone and inside the
    // pie's outer radius, then highlight the section that the mouse
    // pointer is hovering over. The exception is that we also
    // highlight the section if the mouse pointer is outside the pie,
    // if a submenu is shown. In all other cases, no item is
    // highlighted.
    int prevHighlightedItem = hItem;
    if (mouseRad >= innerRad && (shownSubMenu != -1 || (mouseRad <= outerRad || dragging()))) {
        hItem = indexAt(lastMousePos);
    } else {
            if (mouseRad < innerRad)
                hItem = -1;
            else
                hItem = -2;
    }

    // If highlight changes, order an update.
    if (prevHighlightedItem != hItem) {
            emit highlighted(hItem);
            update();

            // And in this case, if we are in dragging mode, start the
            // timer so that in case the highlighted item is a submenu, it
            // will pop up in 0.25 seconds. If the mouse hovers over an
            // action item, any shown submenu is closed.
        hoverTimer.setSingleShot(true);
            hoverTimer.start(250);

    }

    // In dragging mode, always order an activation of a submenu if
    // the mouse has moved far enough away from the pie. This allows
    // more snappy response as we do not have to wait 250ms.
    if (highlightedItemPtr() && highlightedItemPtr()->type() == SubMenu && mouseRad > outerRad && dragging()) {
            hoverTimer.stop();
            activateItem(Hovering | Dragging);
    }
}

/*! \internal

    When our pie menu is shown, we reset the motion and anything we
    know about items.
*/
void QtPieMenu::showEvent(QShowEvent *e)
{
    emit aboutToShow();

    if (!hasMask) {
            // Create mask.
            QBitmap mask(pix.size());
            mask.fill(Qt::color0);
            generateMask(&mask);
            pix.setMask(mask);
            setMask(mask);
            hasMask = true;
    }

    // motion is used to detect dragging.
    motion = 0;

    // Determine what item in this menu should be highlighted from the
    // current mouse position. With quick gestures, the mouse cursor
    // may already have moved and stopped at a new item before we have
    // shown this submenu. We need to highlight the relevant item.
    lastMousePos = mapFromGlobal(QCursor::pos());
    double rad = radiusAt(lastMousePos);
    if (rad >= innerRad && rad <= outerRad) {
        hItem = angleToIndex(angleAt(lastMousePos), items.size());
    } else {
            if (rad < innerRad)
                hItem = -1;
            else
                hItem = -2;
    }

    QWidget::showEvent(e);
}

/*! \internal

*/
void QtPieMenu::hideEvent(QHideEvent *e)
{
    if (lastActivatedItem == -1)
        emit canceled();
    hItem = -2;
    hideShownSubMenu();
    QWidget::hideEvent(e);

    /*if (syncMenu)
            qApp->exit_loop();*/
}

/*! \internal

    A slot which is called by the timer. Triggers a menu activation if
    we are not shaded.
*/
void QtPieMenu::hoverAlert()
{
    activateItem(Hovering | (dragging() ? Dragging : 0));
}

/*!
    Activates an item. The reason for the activation is given in \a
    reason. Activating an item either causes an action to be
    activated, a submenu to popup, or the menu to be canceled.

    \sa QtPieMenu::ActivateReason
*/
void QtPieMenu::activateItem(int reason)
{
    // Ignore stuff that happens when the menu is not shown. This is
    // to avoid submenus from popping up after a parent menu has been
    // closed.
    if (!isVisible())
        return;

    // As a menu is activated, the timer must be stopped or we might
    // get two activations.
    hoverTimer.stop();

    double mouseRad = radiusAt(lastMousePos);
    bool isInCancelZone = mouseRad < innerRad;
    bool isOutsidePie = mouseRad > outerRad;

    if (reason & Dragging) {
        // Don't cancel the menu if hovering over the cancel zone.
        if (isInCancelZone) {
            if (shownSubMenu != -1) {
                // Hide shown submenu
                hideShownSubMenu();
            }

            // Restart the timer. If the pointer moves out of the
            // cancel zone, we still want to detect hovering.
            if (reason & Hovering) {
                hoverTimer.setSingleShot(true);
                hoverTimer.start(250);
            }

            return;
        }

        // If we're hovering over an item, or if this activation was
        // caused by a mouse release, then activate.
    } else if (!(reason & KeyPress)) {
        // A mouse release can never activate a menu when not in
        // dragging mode.
        if (reason & MouseRelease)
            return;

        // Cancel the menu if we got a MousePress in the cancel zone
        // or outside the pie. Any other activity in the cancel zone
        // when not in dragging mode is ignored.
        if (isInCancelZone || isOutsidePie) {
            if (reason & Hovering && shownSubMenu != -1) {
                // Hide shown submenu
                hideShownSubMenu();
            }

            if (reason & MousePress) {
                if (!isInCancelZone)
                    emit canceledAll();
                emit aboutToHide();
                hide();
            }

            return;
        }
    }

    // ### Investigate this special case further. It should not
    // ### happen.
    if (hItem < 0)
        return;


    // Ignore all operations on disabled items.
    if (!items.at(hItem)->isEnabled())
        return;

    int acthItem = hItem;
    // Show new child
    if (highlightedItemPtr()->type() == SubMenu) {
        // Avoid flickering
        if (acthItem == shownSubMenu)
            return;

        QtPieMenu *pie = (QtPieMenu *)highlightedItemPtr();

        // Hide shown submenu
        hideShownSubMenu();

        emit activated(acthItem);

        shaded = true;

        double angle;
        if (count() == 1) {
            // If there is only one pie menu item, and the parent does
            // not inherit QtPieMenu (qualifies as the rool level pie
            // menu), then the submenu should open straight north. If
            // this is not the root menu however, the submenu should
            // open on the same axis that intersects both the parent's
            // center and this menu's center, at the point where it
            // intersects with this menu's outer radius.
            if (!parent()->inherits("QtPieMenu")) {
                angle = 0;
            } else {
                QtPieMenu *parentPie = (QtPieMenu *)parent();
                QPoint ppos = mapToGlobal(parentPie->pos());
                ppos.setX(ppos.x() + parentPie->width()/2);
                ppos.setY(ppos.y() + parentPie->height()/2);

                QPoint opos = mapToGlobal(pos());
                opos.setX(opos.x() + width()/2);
                opos.setY(opos.y() + height()/2);

                double xxdist = (double)(opos.x() - ppos.x());
                double yydist = (double)(opos.y() - ppos.y());
                double rrad = sqrt(xxdist * xxdist + yydist * yydist);
                angle = acos(xxdist / rrad);

                if (yydist > 0)
                    angle = TWOPI - angle;
            }
        } else {
            double a = indexToAngle(acthItem, count(), true);
            double b = indexToAngle(acthItem + 1, count(), true);
            if (b > a)
                angle = (a + b) / 2;
            else
                angle = (a + b + TWOPI) / 2;
        }

        double b = ((double)(outerRadius() + pie->outerRadius())) * cos(angle) * 0.90;
        double c = - (double)(outerRadius() + pie->outerRadius()) * sin(angle) * 0.90;

        hoverTimer.stop();

        // If hovering in non-dragging mode, show the submenu outside
        // the main menu.
        shownSubMenu = acthItem;
        pie->popup(mapToGlobal(QPoint(rect().center().x() + (int) b,
                                      rect().center().y() + (int) c)));

        update();
    } else {
        if (shownSubMenu != -1) {
            // Hide shown submenu
            hideShownSubMenu();
        }

        // Hovering never activates an action
        if (reason & Hovering)
            return;

        emit activated(acthItem);

        QtPieAction *pie = (QtPieAction *)highlightedItemPtr();
        pie->activate();

        lastActivatedItem = acthItem;
        subMenuSelected();
    }
}

/*! \internal

    Draw the pie menu and blit it onto the screen.
*/
void QtPieMenu::paintEvent(QPaintEvent *)
{
    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing, true);
    QRect adjustedRect = pix.rect().adjusted(0, 0, -1, -1);

    // Special case: 0 or 1 items
    if (items.size() < 2) {
        if (hItem == 0 && items.at(0)->isEnabled())
            p.setBrush(palette().highlight());
        else
            p.setBrush(palette().background());

        if (shaded)
            p.setBrush(QBrush(p.brush().color().dark(125)));

        p.fillRect(rect(), p.brush());
    }

    // Draw the pie with borders and selected slice.
    if (items.size() > 1) {
        for (int i = 0; i < items.size(); ++i) {
            if (i == hItem && items.at(i)->isEnabled())
                p.setBrush(QBrush(palette().highlight()));
            else
                p.setBrush(QBrush(palette().background()));

            p.setPen(palette().mid().color());
            if (shaded) {
                p.setPen(QPen(p.pen().color().dark(125)));
                p.setBrush(QBrush(p.brush().color().dark(125)));
            }

            double a = indexToAngle(i, items.size(), true);
            double b = indexToAngle(i + 1, items.size(), true);

            if (b < a)
                b += TWOPI;
            double size = b - a;
            if (size < 0)
                size += TWOPI;
            int startAngle = (int) ((a * 360.0 * 16.0) / TWOPI);

            p.drawPie(adjustedRect, startAngle, (int) ((size * 360.0 * 16.0) / TWOPI));
        }
    }

    QColor background;
    QColor dark = palette().dark().color();
    QColor light = palette().light().color();
    if (hItem == -1)
        background = palette().highlight().color();
    else
        background = palette().background().color();

    if (shaded) {
        background = background.dark(125);
        dark = dark.dark(125);
        light = light.dark(125);
    }

    // Shading
    QRect innerRect(adjustedRect.center().x() - innerRad, adjustedRect.center().y() - innerRad,
                    innerRad * 2 + 1, innerRad * 2 + 1);

    p.setPen(Qt::NoPen);
    p.setBrush(background);

    p.drawPie(innerRect, 0, 360 * 16);

    light.setAlpha(128);
    p.setPen(QPen(light, 1));
    p.setBrush(Qt::NoBrush);
    p.drawArc(innerRect, 225 * 16, 180 * 16);

    p.setPen(QPen(dark, 1));
    p.drawArc(innerRect, 45 * 16, 180 * 16);

    p.setPen(QPen(light, 1));
    p.setBrush(Qt::NoBrush);
    p.drawArc(adjustedRect, 45 * 16, 180 * 16);
    p.setPen(QPen(dark, 1));
    p.drawArc(adjustedRect, 225 * 16, 180 * 16);

    // Draw cancel zone
    if (innerRad > 0) {
        innerRect.setLeft(innerRect.left() + 1);
        innerRect.setRight(innerRect.right() - 1);
        innerRect.setTop(innerRect.top() + 1);
        innerRect.setBottom(innerRect.bottom() - 1);

        p.setPen(QPen(dark, 1));
        p.drawArc(innerRect, 225 * 16, 180 * 16);
        p.setPen(QPen(light, 1));
        p.drawArc(innerRect, 45 * 16, 180 * 16);
        p.setPen(QPen(dark, 1));

        p.setBrush(background);

        p.drawArc(innerRect, 225 * 16, 180 * 16);
        p.setPen(QPen(light, 1));
        p.drawArc(innerRect, 45 * 16, 180 * 16);
    }

    QRect r = innerRect;
    innerRect.setLeft(r.center().x() + ((r.left() - r.center().x()) / 3) * 1);
    innerRect.setRight(r.center().x() + ((r.right() - r.center().x()) / 3) * 1);
    innerRect.setTop(r.center().y() + ((r.top() - r.center().y()) / 3) * 1);
    innerRect.setBottom(r.center().y() + ((r.bottom() - r.center().y()) / 3) * 1);

    QColor text = hItem == -1 ? palette().highlightedText().color() : palette().text().color();
    if (shaded)
        text = text.dark(125);

    p.setPen(QPen(text, 2));
    p.drawLine(innerRect.topLeft(), innerRect.bottomRight());
    p.drawLine(innerRect.topRight(), innerRect.bottomLeft());

    // Distance from edge of pie to tip of arrow.
    int h1 = 2;

    // Height of arrow.
    int h2 = 6;

    // Width of arrow in radians.
    double w = 0.1;

    // Draw little arrows on the edge of the items that hide a
    // submenu.
    int i;
    for (i = 0; i < items.size(); ++i) {
        if (!items.at(i) || items.at(i)->type() != SubMenu)
            continue;

        if (i == hItem) {
            p.setBrush(palette().highlightedText());

            // Smooth edges of the arrow by drawing with a pen that
            // has the average color of the foreground and the
            // background.
            QColor c = palette().highlightedText().color();
            QColor d = palette().highlight().color();
            p.setPen(QColor((c.red() + d.red()) / 2,
                            (c.green() + d.green()) / 2,
                            (c.blue() + d.blue()) / 2));
        } else {
            p.setBrush(palette().text());

            QColor c = palette().text().color();
            QColor d = palette().background().color();
            p.setPen(QColor((c.red() + d.red()) / 2,
                            (c.green() + d.green()) / 2,
                            (c.blue() + d.blue()) / 2));
        }

        if (shaded) {
            p.setPen(QPen(p.pen().color().dark(125)));
            p.setBrush(QBrush(p.brush().color().dark(125)));
        }

        double angle;
        if (count() == 1) {
            // If there is only one pie menu item, and the parent does
            // not inherit QtPieMenu (qualifies as the rool level pie
            // menu), then the arrow should be drawn straight
            // north. If this is not the root menu however, the arrow
            // should be drawn on the same axis that intersects both
            // the parent's center and this menu's center, at the
            // point where it intersects with this menu's outer
            // radius.
            if (!parent()->inherits("QtPieMenu")) {
                angle = 0;
            } else {
                QtPieMenu *parentPie = (QtPieMenu *)parent();
                QPoint ppos = mapToGlobal(parentPie->pos());
                ppos.setX(ppos.x() + parentPie->width()/2);
                ppos.setY(ppos.y() + parentPie->height()/2);

                QPoint opos = mapToGlobal(pos());
                opos.setX(opos.x() + width()/2);
                opos.setY(opos.y() + height()/2);

                double xxdist = (double)(opos.x() - ppos.x());
                double yydist = (double)(opos.y() - ppos.y());
                double rrad = sqrt(xxdist * xxdist + yydist * yydist);
                angle = acos(xxdist / rrad);

                if (yydist > 0)
                    angle = TWOPI - angle;
            }
        } else {
            double a = indexToAngle(i, count(), true);
            double b = indexToAngle(i + 1, count(), true);
            if (b > a)
                angle = (a + b) / 2;
            else
                angle = (a + b + TWOPI) / 2;
        }

        double trad = (double)(outerRad - h1);
        double tb = trad * cos(angle);
        double tc = - trad * sin(angle);
        double lrad = (double)(outerRad - h2);
        double lb = lrad * cos(angle + w);
        double lc = - lrad * sin(angle + w);
        double rrad = (double)(outerRad - h2);
        double rb = rrad * cos(angle - w);
        double rc = - rrad * sin(angle - w);

        QPolygon ar(3);
        ar.setPoint(0, pix.rect().center().x() + (int) tb, pix.rect().center().y() + (int) tc);
        ar.setPoint(1, pix.rect().center().x() + (int) rb, pix.rect().center().y() + (int) rc);
        ar.setPoint(2, pix.rect().center().x() + (int) lb, pix.rect().center().y() + (int) lc);

        p.drawPolygon(ar);
    }

    // Draw the text and/or icon on each slice.
    for (i = 0; i < items.size(); ++i) {
        QString text = items.at(i)->text();

        double angle1 = indexToAngle(i, items.size(), true);
        double angle2 = indexToAngle(i + 1, items.size(), true);
        if (angle2 < angle1)
            angle2 += TWOPI;
        double angle = (angle1 + angle2) / 2.0;
        double rad = (double)(innerRad + outerRad) / 2.0;
        double b = rad * cos(angle);
        double c = - rad * sin(angle);

        QFontMetrics fontMetrics = p.fontMetrics();
        QRect boundingRect;
        if (!text.isEmpty())
            boundingRect = fontMetrics.boundingRect(text);
        int textWidth = boundingRect.width();

        QPixmap icon = items.at(i)->icon().pixmap(QSize(32, 32),
                                                  QIcon::Normal,
                                                  QIcon::On);
        int x = pix.rect().center().x();
        int y = pix.rect().center().y();

        // Draw icon
        if (!items.at(i)->icon().isNull()) {
            QRect r(x + int(b) - (icon.width() + textWidth) / 2,
                    y + int(c) - icon.height() / 2,
                    icon.width(), icon.height());

            if (shaded) {
                // Only paint every second pixel.
                QBitmap bitmap(icon.width(), icon.height());
                QPainter mapper(&bitmap);
                int xorer = 0;
                for (int b = 0; b < icon.height(); ++b) {
                    xorer = b & 1;
                    for (int a = 0; a < icon.width(); ++a) {
                        mapper.setPen(xorer++ & 1 ? Qt::color1 : Qt::color0);
                        mapper.drawPoint(a, b);
                    }
                }

                icon.setMask(bitmap);
            }

            p.drawPixmap(r, icon);
        }

        // Draw text
        if (!text.isEmpty()) {
            QBrush pen;
            QBrush brush;

            if (i == hItem) {
                if (highlightedItemPtr()->isEnabled()) {
                    pen = palette().highlightedText();
                    brush = palette().highlight();
                } else {
                    pen = palette().mid();
                    brush = palette().background();
                }
            } else {
                if (items.at(i)->isEnabled())
                    pen = palette().text();
                else
                    pen = palette().mid();

                brush = palette().background();
            }

            if (shaded) {
                pen = QBrush(pen.color().dark(125));
                brush = QBrush(brush.color().dark(125));
            }

            // Draw text horizontally in center of slice.
            double angle1 = indexToAngle(i, items.size(), true);
            double angle2 = indexToAngle(i + 1, items.size(), true);
            if (angle2 < angle1)
                angle2 += TWOPI;
            double angle = (angle1 + angle2) / 2.0;

            double rad = (double)(innerRad + outerRad) / 2.0;
            double b = rad * cos(angle);
            double c = - rad * sin(angle);

            // Draw rich text, always centered.
            QRect r(x + (int) b - (icon.width() + textWidth) / 2 + icon.width(),
                    y + (int) c - boundingRect.height() / 2, textWidth, boundingRect.height());

            p.setPen(pen.color());
            p.setBrush(brush);
            p.drawText(r.left(), r.top() + boundingRect.height(), text);
        }
    }

    QPainter p2(this);
    p2.drawPixmap(QPoint(0, 0), pix);
}

/*! \internal

*/
void QtPieMenu::subMenuSelected()
{
    emit activated();
    emit aboutToHide();
    hide();
}

/*! \internal

    This slot is called when a submenu has been canceled. This pie
    menu is unshaded, and an update is ordered.
*/
void QtPieMenu::subMenuCanceled()
{
    shownSubMenu = -1;

    // reset motion counter.
    motion = 0;
    hItem = -2;

    // prevent delayed popup.
    hoverTimer.stop();

    shaded = false;

    // When a submenu is canceled, the following mouse release will
    // go to us.
    ignoreNextMouseReleaseIfNotDragging = true;

    update();
}

/*! \internal

    This slot is called when the whole menu has been canceled. This
    pie menu is hidden, and an update is ordered.
*/
void QtPieMenu::allCanceled()
{
    shownSubMenu = -1;

    motion = 0;
    hItem = -2;

    // prevent delayed popup.
    hoverTimer.stop();

    shaded = false;

    ignoreNextMouseReleaseIfNotDragging = true;

    emit aboutToHide();
    hide();
    emit canceledAll();

    update();
}

/*! \internal

    Given the item index \a item (ranging from 0 and up) and the total
    number of items \a total, calculate the angle in radians from the
    base [0,0] - [1,0] line of the pie to the end of the \a item
    section.
*/
double QtPieMenu::indexToAngle(int item, int total, bool shifted)
{
    // Wrap index.
    item %= total;

    // Sum up all the weights, and find the base angle, or the angle
    // of an item of weight 1.
    double totalWeight = 0;
    if (items.size() != 0) {
        for (int i = 0; i < items.size(); ++i)
            totalWeight += items.at(i)->weight();
    } else {
        totalWeight = total;
    }

    double baseAngle = TWOPI / (double) totalWeight;

    // Find the start angle of the first item.
    double rad = PI/2;

    if (shifted) {
        if (!items.isEmpty())
            rad -= ((double)items.at(0)->weight() * baseAngle) / 2.0;
        else
            rad -= baseAngle / 2.0;
    }
    while (rad < 0)
        rad += TWOPI;

    // Add the base angle multiplied by the items' weights until we
    // reach the item we're queried for.
    for (int index = 0; index < item; ++index) {
        if (index >= items.size())
            rad += baseAngle;
        else
            rad += double(items.at(index)->weight()) * baseAngle;
    }

        // Adjust accordingly.
    while (rad > TWOPI)
        rad -= TWOPI;

    return rad;
}

/*! \internal

    An inverse of indexToAngle(): Given an angle in radians from the
    base [0,0] - [1,0] line of the pie, find the section in which this
    angle resides.
*/
int QtPieMenu::angleToIndex(double angle, int total)
{
    if (total == 0)
        return -1;

    // Sum up all the weights, and find the base angle, or the angle
    // of an item of weight 1.
    double totalWeight = 0;
    for (int i = 0; i < items.size(); ++i)
        totalWeight += items.at(i)->weight();
    double baseAngle = TWOPI / (double) totalWeight;

    // Find the start angle of the first item.
    double rad = PI/2;

    if (!items.isEmpty())
        rad -= (double(items.at(0)->weight()) * baseAngle) / 2.0;
    while (rad < 0) rad += TWOPI;

    // Add the base angle multiplied by the items' weights until we
    // reach the item we're queried for.
    double oldrad = rad;
    for (int index = 0; index < items.size(); ++index) {
        rad += double(items.at(index)->weight()) * baseAngle;
        if (angle > oldrad && angle <= rad) {
            return index;
        } else if (angle+TWOPI > oldrad && angle+TWOPI <= rad) {
            return index;
        }

        oldrad = rad;
    }

    return -1;
}

/*!
    Returns the index of the item that is highlighted when the cursor
    is at position \a pos.

    This function is reimplemented if a subclass of QtPieMenu provides
    a new item layout.
*/
int QtPieMenu::indexAt(const QPoint &pos)
{
    return angleToIndex(angleAt(pos), items.size());
}

/*!
    Returns the distance from \a pos to the center of the pie menu.
 */
double QtPieMenu::radiusAt(const QPoint &pos) const
{
    double mousexdist = pos.x() - rect().center().x();
    double mouseydist = pos.y() - rect().center().y();

    return sqrt(mousexdist * mousexdist + mouseydist * mouseydist);
}

/*!
    With origin set to the center of the pie, this function returns the
    angle in radians between the line that starts at (0,0) and ends at
    (1,0) and the line that starts at (0,0) and ends at \a pos.
*/
double QtPieMenu::angleAt(const QPoint &pos) const
{
    double mousexdist = pos.x() - rect().center().x();
    double mouseydist = pos.y() - rect().center().y();
    double mouserad = sqrt(mousexdist * mousexdist + mouseydist * mouseydist);
    double angle = acos(mousexdist / mouserad);
    if (mouseydist >= 0)
            angle = TWOPI - angle;

    return angle;
}

/*! \internal

    If the mouse is being dragged with the right mouse button down,
    returns \e true. Otherwise returns \e false.
*/
bool QtPieMenu::dragging() const
{
    return mouseButtonStatus == Pressed && motion > 6;
}

/*! \internal

    If a submenu is currently shown, hide it.
*/
void QtPieMenu::hideShownSubMenu()
{
    if (shownSubMenu != -1) {
            QtPieMenu *menu = static_cast<QtPieMenu *>(items[shownSubMenu]);
            if (!menu) {
                // ### Dead code
                return;
            }

        emit menu->aboutToHide();
        menu->hide();

            shownSubMenu = -1;
            shaded = false;
            update();
    }
}

/*!
    Returns the index of the item that is currently selected, or -1 if
    no item is selected.
*/
int QtPieMenu::highlightedItem()
{
    return hItem;
}

/*! \internal
    Deletes the object at index \a index in the internal list. If \a index is -1
    then all objects in the list will be deleted.
*/
void QtPieMenu::deleteItems(int index)
{
    if (index >= 0) {
        delete items[index];
        items.removeAt(index);
    } else {
        for (int i=0; i<items.size(); ++i)
            delete items[i];
        items.clear();
    }
}

/*! \internal
    Returns a pointer to the item that is currently selected, or 0 if
    none is selected.
*/
QtPieItem *QtPieMenu::highlightedItemPtr()
{
    if (hItem < 0)
        return 0;
    else
        return items[hItem];
}

/*! \internal

    Returns a pointer to the item that is currently selected, or 0 if
    none is selected.
*/
QtPieItem *QtPieMenu::shownItemPtr()
{
    if (shownSubMenu == -1)
            return 0;
    else
            return items[shownSubMenu];
}

/*!
    Returns true if the menu is shaded (inactive); otherwise returns false.
*/
bool QtPieMenu::isShaded() const
{
    return shaded;
}

/*!
    Returns the type of the item at position \a index, or
    QtPieItem::Invalid if there is no item at position \a index.
*/
int QtPieMenu::itemType(int index) const
{
    if (!itemAt(index))
        return Invalid;
    return (ItemType) itemAt(index)->type();
}
