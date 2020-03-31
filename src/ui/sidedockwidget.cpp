/*
 *  Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *  Copyright 2019       Tomaz Canabrava <tcanabrava@kde.org>
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

#include "sidedockwidget.h"

#include <KAcceleratorManager>
#include <QStyle>
#include <QStyleOptionToolButton>
#include <QStylePainter>
#include <QToolBar>
#include <QLabel>
#include <QAction>
#include <QStackedLayout>
#include <QLayout>
#include <QObject>
#include <QWidget>
#include <QIcon>
#include <KLocalizedString>
#include <QDebug>


SideToolButton::SideToolButton(QWidget *parent)
    : QToolButton(parent)
{
    setFocusPolicy(Qt::NoFocus);
    KAcceleratorManager::setNoAccel(this);
    setCheckable(true);
    setAutoRaise(true);
    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    wasChecked = false;
}

Qt::Orientation SideToolButton::orientation() const
{
    return Qt::Vertical;
}

QSize SideToolButton::sizeHint() const
{
    ensurePolished();

    QStyleOptionToolButton option;
    initStyleOption(&option);

    QFontMetrics fm = fontMetrics();

    // calculate example character width
    const int charWidth = fm.boundingRect(QLatin1Char('x')).width();

    // compute text size
    QSize textSize = fm.size(Qt::TextShowMnemonic, option.text);
    textSize.rwidth() += 2 * charWidth;

    // compute icon size
    const int spacing = 2;
    const int iconwidth = option.iconSize.height();
    const int iconheight = option.iconSize.width();

    // compute final size. height and width are rotated.
    return {
        4 + qMax(textSize.height(), iconheight) + spacing,
        4 + textSize.width() + iconwidth + spacing
    };
}

void SideToolButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    // rotated paint
    QStylePainter painter(this);
    QStyleOptionToolButton option;
    initStyleOption(&option);

    // first draw normal frame and not text/icon
    option.text.clear();
    option.icon = QIcon();
    painter.drawComplexControl(QStyle::CC_ToolButton, option);

    // rotate the options
    QSize size(option.rect.size());
    size.transpose();
    option.rect.setSize(size);

    // rotate the painter
    painter.translate(width(), 0);
    painter.rotate(90);

    // paint text and icon
    option.text = text();
    QIcon::Mode iconMode = (option.state & QStyle::State_MouseOver) ? QIcon::Active : QIcon::Normal;
    QPixmap ic = icon().pixmap(option.iconSize, iconMode, QIcon::On);
    QTransform transform;
    transform = transform.rotate(-90); // rotate right
    option.icon = ic.transformed(transform, Qt::SmoothTransformation);
    painter.drawControl(QStyle::CE_ToolButtonLabel, option);
    painter.end();
}

SidedockWidget::SidedockWidget(QWidget* parent)
    : QWidget(parent)
    , _btnGroup(new QButtonGroup(this))
    , _showDock(false)
{
    Q_ASSERT(parent); // we need a parent widget

    _btnGroup->setExclusive(true);

    _toolBar = new QToolBar(i18nc("@title", "Side Toolbar"), this);
    _toolBar->setFloatable(false);
    _toolBar->setMovable(false);
    _toolBar->setObjectName("sidebar");
    _toolBar->setIconSize(QSize(16,16));
    _toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    setLayout(new QStackedLayout);
}

void SidedockWidget::addDock(QWidget* widget, const QString& title, const QIcon& icon)
{
    widget->setVisible(false);
    layout()->addWidget(widget);

    SideToolButton* button = new SideToolButton(this);

    button->setText(title);
    button->setToolTip(i18n("Toggle '%1' view.", title));
    button->setIcon(icon);
    button->setShortcut(QKeySequence());
    button->setChecked(false); // initially do not check

    _btnGroup->addButton(button);

    // only request action on user set action
    const int idx = _widgets.count();

    connect(button, &SideToolButton::clicked, this,
        [this, button, idx] {

        // Force uncheck the button because of the exclusive
        // Without this the toolbar buttons cannot be all unchecked
        // after one is clicked4
        if (button->wasChecked) {
            _btnGroup->setExclusive(false);
            button->setChecked(false);
            _btnGroup->setExclusive(true);

            button->wasChecked = false;
        } else {
            button->wasChecked = true;
        }

        showDock(button->isChecked(), idx);
    });

    // register and add to list
    _toolBar->addWidget(button);
    _widgets.append(widget);
}

void SidedockWidget::showDock(bool show, int nr)
{
    _showDock = show;
    setVisible(show);
    auto *stackLayout = qobject_cast<QStackedLayout*>(layout());
    stackLayout->setCurrentIndex(nr);
    emit visibilityChanged(show);
}

QToolBar* SidedockWidget::toolbar() const
{
    return _toolBar;
}
