/* This file is part of Rocs,  
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <KAction>
#include <QEvent>
#include <QToolButton>
#include <QActionGroup>
#include <QScrollArea>
#include <KLocale>
#include <KIcon>

#include "UI_PaletteBar.h"
#include "UI_MainWindow.h"
#include "UI_PaletteScrollArea.h"
#include "UI_PaletteLayout.h"
#include "UI_Separator.h"

#include "UI_GraphScene.h"

#include "settings.h" //! AUTO GENERATED!  ( *Hate* this u_u )

#include "UI_PaletteBar.moc" //! AUTO GENERATED! ( *Hate* this >_< )


PaletteBar::PaletteBar(QWidget* parent, Qt::WindowFlags flags) : QDockWidget(i18n("Palette"), parent, flags), _widget(0)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setFeatures(QDockWidget::NoDockWidgetFeatures);

    QWidget* topWidget = new QWidget(this);

    _scrollArea = new PaletteScrollArea(topWidget);
    _scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _scrollArea->setFrameShape(QFrame::NoFrame);

    _widget = new QWidget(_scrollArea);
    _layout = new PaletteLayout(_widget);
    _layout->setSpacing(0);
    _layout->setOneLine(Settings::showButtonText());

    _actionGroup = new QActionGroup(this);
    _actionGroup->setExclusive(true);

    KAction* tmpAction = new KAction(i18n("Pointer"), this);
    tmpAction->setToolTip(i18n("Selection pointer"));
    tmpAction->setIcon(KIcon("pointer"));
    tmpAction->setCheckable(true);
    tmpAction->setChecked(true);
    tmpAction->setProperty("rocs_action", GraphScene::SingleSelection );
    _actionGroup->addAction(tmpAction);
    createToolButton(tmpAction);

    tmpAction = new KAction(i18n("Square Selection"), this);
    tmpAction->setToolTip(i18n("Square Selection tool"));
    tmpAction->setIcon(KIcon("pointer"));
    tmpAction->setCheckable(true);
    tmpAction->setChecked(false);
    tmpAction->setProperty("rocs_action", GraphScene::SquareSelection);
    _actionGroup->addAction(tmpAction);
    createToolButton(tmpAction);

    tmpAction = new KAction(i18n("Move"), this);
    tmpAction->setToolTip(i18n("Move Selected"));
    tmpAction->setIcon(KIcon("pointer"));
    tmpAction->setCheckable(true);
    tmpAction->setChecked(false);
    tmpAction->setProperty("rocs_action", GraphScene::Move);
    _actionGroup->addAction(tmpAction);
    createToolButton(tmpAction);

    createSeparator();

    tmpAction = new KAction(i18n("Add Node"), this);
    tmpAction->setToolTip(i18n("Creates a new node at the click position on the drawing area."));
    tmpAction->setIcon(KIcon("pointer"));
    tmpAction->setCheckable(true);
    tmpAction->setChecked(false);
    tmpAction->setProperty("rocs_action", GraphScene::AddNode);
    _actionGroup->addAction(tmpAction);
    createToolButton(tmpAction);

    tmpAction = new KAction(i18n("Add Edge"), this);
    tmpAction->setToolTip(i18n("Connects two nodes with an edge"));
    tmpAction->setIcon(KIcon("pointer"));
    tmpAction->setCheckable(true);
    tmpAction->setChecked(false);
    tmpAction->setProperty("rocs_action", GraphScene::AddEdge1);
    _actionGroup->addAction(tmpAction);
    createToolButton(tmpAction);

    tmpAction = new KAction(i18n("Add Arrowed Edge"), this);
    tmpAction->setToolTip(i18n("Connects two nodes with an arrowed edge"));
    tmpAction->setIcon(KIcon("pointer"));
    tmpAction->setCheckable(true);
    tmpAction->setChecked(false);
    tmpAction->setProperty("rocs_action", GraphScene::AddEdge2);
    _actionGroup->addAction(tmpAction);
    createToolButton(tmpAction);

    tmpAction = new KAction(i18n("Create Complete Graph"), this);
    tmpAction->setToolTip(i18n("Creates a Graph with N vertices interconected"));
    tmpAction->setIcon(KIcon("pointer"));
    tmpAction->setCheckable(true);
    tmpAction->setChecked(false);
    tmpAction->setProperty("rocs_action", GraphScene::MakeKGraph);
    _actionGroup->addAction(tmpAction);
    createToolButton(tmpAction);

    tmpAction = new KAction(i18n("Create a Circular Graph"), this);
    tmpAction->setToolTip(i18n("Create a Graph with N vertices, connected as a ring"));
    tmpAction->setIcon(KIcon("pointer"));
    tmpAction->setCheckable(true);
    tmpAction->setChecked(false);
    tmpAction->setProperty("rocs_action", GraphScene::MakeCGraph);
    _actionGroup->addAction(tmpAction);
    createToolButton(tmpAction);


    _scrollArea->setWidget(_widget);
    _scrollArea->setMinimumWidth(_widget->minimumSizeHint().width());

    QVBoxLayout* topLayout = new QVBoxLayout(topWidget);
    topLayout->addWidget(_scrollArea);
    setWidget(topWidget);

    QObject::connect(_actionGroup, SIGNAL(triggered(QAction*)), this, SLOT(actionTriggered(QAction*)));

    QAction* showText = new QAction(i18n("Show text"), this);
    showText->setCheckable(true);
    showText->setChecked(Settings::showButtonText());
    QObject::connect(showText, SIGNAL(toggled(bool)), this, SLOT(showButtonTextToggled(bool)));

    _widget->addAction(showText);
    _widget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void PaletteBar::createToolButton(QAction* action)
{
    QToolButton* button = new QToolButton(_widget);
    button->setToolButtonStyle(Settings::showButtonText() ? 
                    Qt::ToolButtonTextBesideIcon : Qt::ToolButtonIconOnly);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    button->setAutoRaise(true);
    button->setIconSize(QSize(22,22));
    button->setDefaultAction(action);
    _toolButtons.append(button);
    _layout->addWidget(button);
}

void PaletteBar::createSeparator()
{
    QAction* action = new QAction(this);
    action->setSeparator(true);
    _actionGroup->addAction(action);
    _layout->addWidget(new Separator(_widget));
}

void PaletteBar::showButtonTextToggled(bool b)
{
    Settings::setShowButtonText(b);
    Settings::self()->writeConfig();
    foreach(QToolButton* button, _toolButtons)
    {
        button->setToolButtonStyle(b ? Qt::ToolButtonTextBesideIcon : Qt::ToolButtonIconOnly);
    }
    _layout->setOneLine(b);
    _scrollArea->setMinimumWidth(_widget->minimumSizeHint().width());
}

void PaletteBar::actionTriggered(QAction* action)
{
    emit beginAddItem(action->property("rocs_action").toString());
}

void PaletteBar::endAddItem(const QString& name, bool /*success*/)
{
    if(name == _actionGroup->checkedAction()->property("rocs_action").toString())
        _pointerAction->setChecked(true);
}

bool PaletteBar::event(QEvent* event)
{
    return QDockWidget::event(event);
}


