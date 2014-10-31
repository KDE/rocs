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

#include <QWebView>
#include <QDebug>
#include <QStandardPaths>
#include <QIcon>

ApiDocWidget::ApiDocWidget(QWidget* parent)
    : QWidget(parent)
    , _manager(new ApiDocManager(this))
    , _historyPointer(-1)
{
    _baseUrl = QUrl::fromLocalFile(
        QStandardPaths::locate(QStandardPaths::DataLocation, "plugin/apidoc/objectApi.html", QStandardPaths::LocateDirectory));

    ui = new Ui::ApiDocWidget;
    ui->setupUi(this);
    ui->buttonTree->setIcon(QIcon::fromTheme("view-sidetree"));
    ui->buttonHome->setIcon(QIcon::fromTheme("go-home"));
    ui->buttonPrev->setIcon(QIcon::fromTheme("go-previous-view"));
    ui->buttonNext->setIcon(QIcon::fromTheme("go-next-view"));

    ui->buttonPrev->setEnabled(false);
    ui->buttonNext->setEnabled(false);

    _manager->loadLocalData();
    _model = new ApiDocModel(_manager->objectApiList(), this);

    connect(ui->buttonTree, SIGNAL(clicked(bool)), this, SLOT(showTreeOutline()));
    connect(ui->buttonHome, SIGNAL(clicked(bool)), this, SLOT(showHtmlOutline()));
    connect(ui->docTree, SIGNAL(clicked(QModelIndex)), this, SLOT(showDetails(QModelIndex)));
    connect(ui->buttonNext, SIGNAL(clicked(bool)), this, SLOT(historyGoForward()));
    connect(ui->buttonPrev, SIGNAL(clicked(bool)), this, SLOT(historyGoBack()));

    // listen to all links for ids
    connect(ui->docDetails, SIGNAL(linkClicked(QUrl)), this, SLOT(showObjectApi(QUrl)));
    // this option has the following idea:
    // * handle relative anchor calls directly in the web engine
    // * use for switching between object pages the path "http://virtual/<object-id>"
    //   such that that path is handles as external and progapages to this widget
    // drawback: history only works for object pages, not for anchors
    ui->docDetails->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);

    ui->docTree->setModel(_model);
}

void ApiDocWidget::showTreeOutline()
{
    ui->pageStack->setCurrentIndex(0);
}

void ApiDocWidget::showHtmlOutline()
{
    showHtmlOutline(true);
}

void ApiDocWidget::showHtmlOutline(bool logHistory)
{
    ui->docDetails->setHtml(_manager->apiOverviewDocument(), _baseUrl);
    ui->pageStack->setCurrentIndex(1);

    if (!logHistory) {
        return;
    }

    // clear forward history
    if (_historyPointer < _history.count() - 1) {
        while (_historyPointer < _history.count() - 1) {
            _history.removeAt(_history.count() - 1);
        }
        ui->buttonNext->setEnabled(false);
    }

    ++_historyPointer;
    _history.append("_outline"); // use this identifier for the script api html outline
    if (_historyPointer > 0) {
        ui->buttonPrev->setEnabled(true);
    }
}

void ApiDocWidget::showDetails(const QModelIndex &index)
{
    showObjectApi(_model->data(index, ApiDocModel::DocumentRole).toString(), true);
    ui->pageStack->setCurrentIndex(1);

    // TODO jump to anchor
    // _model->data(index, ApiDocModel::AnchorRole).toString();
}

void ApiDocWidget::showObjectApi(const QString &id, bool logHistory=true)
{
    QString htmlDocument = _manager->objectApiDocument(id);
    ui->docDetails->setHtml(htmlDocument, _baseUrl);
    ui->pageStack->setCurrentIndex(1);

    if (logHistory) {
        // update history
        if (_historyPointer < _history.count() - 1) {
            while (_historyPointer < _history.count() -1) {
                _history.removeAt(_history.count() - 1);
            }
            ui->buttonNext->setEnabled(false);
        }
        _history.append(id);
        ++_historyPointer;
        if (_historyPointer > 0) {
            ui->buttonPrev->setEnabled(true);
        }
    }
}

void ApiDocWidget::showObjectApi(const QUrl &aliasPage)
{
    if (aliasPage.toString().isEmpty()) {
        qCritical() << "No path given, aborting.";
        return;
    }

    QString path = aliasPage.toString();
    int len = path.length() - 1;
    while (path.at(len) != '/' && len >= 0) {
        --len;
    }

    if (len <= 0) {
        showObjectApi(path);
    } else {
        QString id = path.mid(len + 1);
        showObjectApi(id);
    }
}

void ApiDocWidget::historyGoBack()
{
    if (_historyPointer <= 0) {
        qCritical() << "Cannot go back in history, none exist";
        return;
    }
    --_historyPointer;
    if (_history.at(_historyPointer) == "_outline") {
        showHtmlOutline(false);
    } else {
        showObjectApi(_history.at(_historyPointer), false);
    }

    // set buttons
    ui->buttonNext->setEnabled(true);
    if (_historyPointer <= 0) {
        ui->buttonPrev->setEnabled(false);
    }
}

void ApiDocWidget::historyGoForward()
{
    if (_historyPointer >= _history.length() - 1) {
        qCritical() << "Cannot go forward in history, none exist";
        return;
    }
    ++_historyPointer;
    if (_history.at(_historyPointer) == "_outline") {
        showHtmlOutline(false);
    } else {
        showObjectApi(_history.at(_historyPointer), false);
    }
    // set buttons
    ui->buttonPrev->setEnabled(true);
    if (_historyPointer >= _history.count() - 1) {
        ui->buttonNext->setEnabled(false);
    }
}
