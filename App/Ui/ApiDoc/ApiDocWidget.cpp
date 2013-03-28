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


#include "ApiDocWidget.h"
#include "ApiDocManager.h"
#include "ApiDocModel.h"

#include <KDebug>

ApiDocWidget::ApiDocWidget(QWidget* parent)
    : QWidget(parent)
    , _manager(new ApiDocManager(this))
{
    ui = new Ui::ApiDocWidget;
    ui->setupUi(this);

    ui->buttonHome->setIcon(KIcon("go-home"));
    ui->buttonBack->setIcon(KIcon("go-previous"));
    ui->buttonForward->setIcon(KIcon("go-next"));

    _manager->loadLocalData();
    _model = new ApiDocModel(_manager->objectApiList(), this);

    connect(ui->buttonHome, SIGNAL(clicked(bool)), this, SLOT(goHome()));
    connect(ui->docTree, SIGNAL(clicked(QModelIndex)), this, SLOT(showDetails(QModelIndex)));

    ui->docTree->setModel(_model);
}

void ApiDocWidget::goHome()
{
    ui->pageStack->setCurrentIndex(0);
}

void ApiDocWidget::showDetails(const QModelIndex &index)
{
    kDebug() << _model->data(index, ApiDocModel::DocumentRole);
    kDebug() << _model->data(index, ApiDocModel::AnchorRole);
}

