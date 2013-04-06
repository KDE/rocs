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
#include <KDebug>
#include <KStandardDirs>

ApiDocWidget::ApiDocWidget(QWidget* parent)
    : QWidget(parent)
    , _manager(new ApiDocManager(this))
{
    _baseUrl = KUrl::fromPath(KGlobal::dirs()->findResourceDir("appdata", QString("plugin/apidoc/objectApi.html")));
    _baseUrl.addPath("plugin/apidoc/");

    ui = new Ui::ApiDocWidget;
    ui->setupUi(this);
    ui->buttonTree->setIcon(KIcon("view-sidetree"));
    ui->buttonHome->setIcon(KIcon("go-home"));
    ui->buttonPrev->setIcon(KIcon("go-previous-view"));
    ui->buttonNext->setIcon(KIcon("go-next-view"));

    _manager->loadLocalData();
    _model = new ApiDocModel(_manager->objectApiList(), this);

    connect(ui->buttonTree, SIGNAL(clicked(bool)), this, SLOT(showTreeOutline()));
    connect(ui->buttonHome, SIGNAL(clicked(bool)), this, SLOT(showHtmlOutline()));
    connect(ui->docTree, SIGNAL(clicked(QModelIndex)), this, SLOT(showDetails(QModelIndex)));

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
    ui->docDetails->setHtml(_manager->apiOverviewDocument(), _baseUrl);
    ui->pageStack->setCurrentIndex(1);
}

void ApiDocWidget::showDetails(const QModelIndex &index)
{
    QString htmlDocument = _manager->objectApiDocument(_model->data(index, ApiDocModel::DocumentRole).toString());
    ui->docDetails->setHtml(htmlDocument, _baseUrl);
    ui->pageStack->setCurrentIndex(1);

    // TODO jump to anchor
    // _model->data(index, ApiDocModel::AnchorRole).toString();
}

void ApiDocWidget::showObjectApi(const QString &id)
{
    QString htmlDocument = _manager->objectApiDocument(id);
    ui->docDetails->setHtml(htmlDocument, _baseUrl);
    ui->pageStack->setCurrentIndex(1);
}

void ApiDocWidget::showObjectApi(const QUrl &aliasPage)
{
    QString path = aliasPage.toString();
    int len = path.length() - 1;
    while (path.at(len) != '/' && len >= 0) {
        --len;
    }

    QString id = path.mid(len + 1);
    showObjectApi(id);
}
