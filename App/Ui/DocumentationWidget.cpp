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


#include "DocumentationWidget.h"
#include <QWebHistory>
#include <QIcon>

DocumentationWidget::DocumentationWidget(QWidget* parent)
    : QWidget(parent)
{
    ui = new Ui::DocumentationWidget;
    ui->setupUi(this);

    ui->buttonHome->setIcon(QIcon("go-home"));
    ui->buttonBack->setIcon(QIcon("go-previous"));
    ui->buttonForward->setIcon(QIcon("go-next"));

    _handbook = QUrl(QString::fromLatin1("http://docs.kde.org/stable/en/kdeedu/rocs/index.html"));

    connect(ui->buttonBack, SIGNAL(pressed()), ui->viewer, SLOT(back()));
    connect(ui->buttonForward, SIGNAL(pressed()), ui->viewer, SLOT(forward()));
    connect(ui->buttonHome, SIGNAL(pressed()), this, SLOT(goHome()));
    connect(ui->viewer, SIGNAL(urlChanged(QUrl)), this, SLOT(updateButtonStates()));

    updateButtonStates();
}

void DocumentationWidget::goHome()
{
    ui->viewer->setUrl(_handbook);
}

void DocumentationWidget::updateButtonStates()
{
    QWebHistory* history = ui->viewer->history();
    ui->buttonBack->setEnabled(history->canGoBack());
    ui->buttonForward->setEnabled(history->canGoForward());
}
