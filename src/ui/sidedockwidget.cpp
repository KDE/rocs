/*
    SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
    SPDX-FileCopyrightText: 2019 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "sidedockwidget.h"
#include <KAcceleratorManager>
#include <KLocalizedString>
#include <QAction>
#include <QDebug>
#include <QIcon>
#include <QLabel>
#include <QLayout>
#include <QObject>
#include <QStackedLayout>
#include <QStyle>
#include <QStyleOptionToolButton>
#include <QStylePainter>
#include <QToolBar>
#include <QWidget>

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
    const int charWidth = fm.boundingRect(QLatin1Char('x')).width();

    // compute text size
    QSize textSize = fm.size(Qt::TextShowMnemonic, option.text);
    textSize.rwidth() += 2 * charWidth;

    // compute icon size
    const int spacing = 2;
    const int iconwidth = option.iconSize.height();
    const int iconheight = option.iconSize.width();

    // compute final size. height and width are rotated.
    return {4 + qMax(textSize.height(), iconheight) + spacing, 4 + textSize.width() + iconwidth + spacing};
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

SidedockWidget::SidedockWidget(QWidget *parent)
    : QWidget(parent)
    , _btnGroup(new QButtonGroup(this))
{
    Q_ASSERT(parent); // we need a parent widget

    _btnGroup->setExclusive(true);

    _toolBar = new QToolBar(i18nc("@title", "Side Toolbar"), this);
    _toolBar->setFloatable(false);
    _toolBar->setMovable(false);
    _toolBar->setObjectName("sidebar");
    _toolBar->setIconSize(QSize(16, 16));
    _toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    setLayout(new QStackedLayout);

    connect(_btnGroup, &QButtonGroup::idClicked, this, [this](int index) {
        auto *stackLayout = qobject_cast<QStackedLayout *>(layout());
        if (stackLayout->currentIndex() == index) {
            setVisible(!isVisible());
        } else {
            auto *stackLayout = qobject_cast<QStackedLayout *>(layout());
            stackLayout->setCurrentIndex(index);
            setVisible(true);
        }
    });
}

void SidedockWidget::addDock(QWidget *widget, const QString &title, const QIcon &icon)
{
    widget->setVisible(false);
    layout()->addWidget(widget);

    SideToolButton *button = new SideToolButton(this);

    button->setText(title);
    button->setToolTip(i18n("Toggle '%1' view.", title));
    button->setIcon(icon);
    button->setShortcut(QKeySequence());
    button->setChecked(false); // initially do not check
    const int index = _widgets.count();
    _btnGroup->addButton(button, index);

    if (index == 0) { // set widget of ElementTypes as standard
        button->setChecked(true);
    }

    // register and add to list
    _toolBar->addWidget(button);
    _widgets.append(widget);
}

QToolBar *SidedockWidget::toolbar() const
{
    return _toolBar;
}

#include "moc_sidedockwidget.cpp"
