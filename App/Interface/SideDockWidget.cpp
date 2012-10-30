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


#include "SideDockWidget.h"

#include <KAcceleratorManager>
#include <QStyle>
#include <QStyleOptionToolButton>
#include <QStylePainter>
#include <QToolBar>
#include <QLabel>
#include <QAction>
#include <QVBoxLayout>
#include <QLayout>
#include <QObject>
#include <QWidget>
#include <KIcon>
#include <KAction>
#include <KLocale>
#include <KDebug>


SideToolButton::SideToolButton(QWidget *parent)
    : QToolButton(parent)
{
    setFocusPolicy(Qt::NoFocus);
    KAcceleratorManager::setNoAccel(this);
    setCheckable(true);
    setAutoRaise(true);
    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
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
    const int charWidth = fm.width(QLatin1Char('x'));

    // compute text size
    QSize textSize;
    textSize = fm.size(Qt::TextShowMnemonic, option.text);
    textSize.rwidth() += 2 * charWidth;

    // compute icon size
    const int spacing = 2;
    int iconwidth = 0, iconheight = 0;
    iconwidth = option.iconSize.height();
    iconheight = option.iconSize.width();

    // compute final size
    int width = 4 + textSize.width() + iconwidth + spacing;
    int height = 4 + qMax(textSize.height(), iconheight) + spacing;

    return QSize(height, width);
}

void SideToolButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

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


SideDockWidget::SideDockWidget(QWidget* parent)
    : QWidget(parent)
    , _showDock(false)
{
    Q_ASSERT(parent); // we need a parent widget

    _toolBar = new QToolBar(i18nc("@title", "Side Toolbar"), this);
    _toolBar->setFloatable(false);
    _toolBar->setMovable(false);
    _toolBar->setObjectName("sidebar");
    _toolBar->setIconSize(QSize(16,16));
    _toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    setLayout(new QVBoxLayout);
}

void SideDockWidget::addDock(QWidget* widget, const QString& title, const KIcon& icon)
{
    widget->setVisible(false);
    layout()->addWidget(widget);

    SideToolButton* button = new SideToolButton(this);

    button->setText(title);
    button->setToolTip(i18n("Toggle '%1' view.", title));
    button->setIcon(icon);
    button->setShortcut(QKeySequence());
    button->setChecked(false); // initially do not check

    // only request action on user set action
    connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonToggled(bool)));

    // register and add to list
    _toolBar->addWidget(button);
    _widgets.insert(button, widget);
}

void SideDockWidget::buttonToggled(bool state)
{
    Q_UNUSED(state);

    SideToolButton* button = qobject_cast<SideToolButton*>(sender());
    if (!button) {
        kWarning() << "Wrong sender for side bar toggle action, aborting";
        return;
    }

    Q_ASSERT(button->isChecked() == state);
    Q_ASSERT(_widgets.contains(button));

    // if button is toggled off, close dock
    if (button->isChecked() == false) {
        showDock(false, _widgets[button]);
    } else {
        showDock(true, _widgets[button]);
    }

    // only one button is allowed to be toggled
    QHash<SideToolButton*, QWidget*>::iterator iter = _widgets.begin();
    while (iter != _widgets.end()) {
        if (iter.key() != button) {
            iter.key()->setChecked(false);
            (*iter)->setVisible(false);
        }
        ++iter;
    }
}

void SideDockWidget::showDock(bool show, QWidget* widget)
{
    _showDock = show;
    if (show == false) {
        widget->setVisible(false);
        parentWidget()->setVisible(false);
    }

    if (show == true) {
        widget->setVisible(true);
        parentWidget()->setVisible(true);;
    }
    emit visibilityChanged(show);
}

QToolBar* SideDockWidget::toolbar() const
{
    return _toolBar;
}
