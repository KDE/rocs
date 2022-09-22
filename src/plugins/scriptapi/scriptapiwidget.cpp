/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "scriptapiwidget.h"
#include "scriptapimanager.h"
#include "scriptapimodel.h"

#include <QDebug>
#include <QIcon>
#include <QStandardPaths>
#include <QUrl>

ScriptApiWidget::ScriptApiWidget(QWidget *parent)
    : QWidget(parent)
    , m_manager(new ScriptApiManager(this))
    , m_historyPointer(-1)
{
    m_baseUrl = QUrl::fromLocalFile(
        QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("rocs/plugin/apidoc/"), QStandardPaths::LocateDirectory));

    ui = new Ui::ScriptApiWidget;
    ui->setupUi(this);
    ui->buttonTree->setIcon(QIcon::fromTheme("view-sidetree"));
    ui->buttonHome->setIcon(QIcon::fromTheme("go-home"));
    ui->buttonPrev->setIcon(QIcon::fromTheme("go-previous-view"));
    ui->buttonNext->setIcon(QIcon::fromTheme("go-next-view"));

    ui->buttonPrev->setEnabled(false);
    ui->buttonNext->setEnabled(false);

    m_manager->loadLocalData();
    m_model = new ScriptApiModel(m_manager->objectApiList(), this);

    connect(ui->buttonTree, &QPushButton::clicked, this, &ScriptApiWidget::showTreeOutline);
    connect(ui->buttonHome, &QPushButton::clicked, this, static_cast<void (ScriptApiWidget::*)(bool)>(&ScriptApiWidget::showHtmlOutline));
    connect(ui->docTree, &QTreeView::clicked, this, &ScriptApiWidget::showDetails);
    connect(ui->buttonNext, &QPushButton::clicked, this, &ScriptApiWidget::historyGoForward);
    connect(ui->buttonPrev, &QPushButton::clicked, this, &ScriptApiWidget::historyGoBack);

    // listen to all links for ids
    connect(ui->docDetails, &QTextBrowser::anchorClicked, this, static_cast<void (ScriptApiWidget::*)(const QUrl &)>(&ScriptApiWidget::showObjectApi));
    // this option has the following idea:
    // * handle relative anchor calls directly in the web engine
    // * use for switching between object pages the path "http://virtual/<object-id>"
    //   such that that path is handles as external and progapages to this widget
    // drawback: history only works for object pages, not for anchors
    ui->docDetails->setOpenExternalLinks(false);

    ui->docTree->setModel(m_model);
    showHtmlOutline();
}

void ScriptApiWidget::showTreeOutline()
{
    ui->pageStack->setCurrentIndex(0);
}

void ScriptApiWidget::showHtmlOutline()
{
    showHtmlOutline(true);
}

void ScriptApiWidget::showHtmlOutline(bool logHistory)
{
    ui->docDetails->setHtml(m_manager->apiOverviewDocument());
    ui->pageStack->setCurrentIndex(1);

    if (!logHistory) {
        return;
    }

    // clear forward history
    if (m_historyPointer < m_history.count() - 1) {
        while (m_historyPointer < m_history.count() - 1) {
            m_history.removeAt(m_history.count() - 1);
        }
        ui->buttonNext->setEnabled(false);
    }

    ++m_historyPointer;
    m_history.append("m_outline"); // use this identifier for the script api html outline
    if (m_historyPointer > 0) {
        ui->buttonPrev->setEnabled(true);
    }
}

void ScriptApiWidget::showDetails(const QModelIndex &index)
{
    showObjectApi(m_model->data(index, ScriptApiModel::DocumentRole).toString(), true);
    ui->pageStack->setCurrentIndex(1);

    // TODO jump to anchor
    // m_model->data(index, ApiDocModel::AnchorRole).toString();
}

void ScriptApiWidget::showObjectApi(const QString &id, bool logHistory = true)
{
    QString htmlDocument = m_manager->objectApiDocument(id);
    ui->docDetails->setHtml(htmlDocument);
    ui->pageStack->setCurrentIndex(1);

    if (logHistory) {
        // update history
        if (m_historyPointer < m_history.count() - 1) {
            while (m_historyPointer < m_history.count() - 1) {
                m_history.removeAt(m_history.count() - 1);
            }
            ui->buttonNext->setEnabled(false);
        }
        m_history.append(id);
        ++m_historyPointer;
        if (m_historyPointer > 0) {
            ui->buttonPrev->setEnabled(true);
        }
    }
}

void ScriptApiWidget::showObjectApi(const QUrl &aliasPage)
{
    if (aliasPage.toString().isEmpty()) {
        qCritical() << "No path given, aborting.";
        return;
    }

    // distinguish anchors and pages
    const QString path = aliasPage.toString();
    if (path.startsWith('#')) {
        ui->docDetails->scrollToAnchor(path);
    } else {
        showObjectApi(path);
    }
}

void ScriptApiWidget::historyGoBack()
{
    if (m_historyPointer <= 0) {
        qCritical() << "Cannot go back in history, none exist";
        return;
    }
    --m_historyPointer;
    if (m_history.at(m_historyPointer) == "m_outline") {
        showHtmlOutline(false);
    } else {
        showObjectApi(m_history.at(m_historyPointer), false);
    }

    // set buttons
    ui->buttonNext->setEnabled(true);
    if (m_historyPointer <= 0) {
        ui->buttonPrev->setEnabled(false);
    }
}

void ScriptApiWidget::historyGoForward()
{
    if (m_historyPointer >= m_history.length() - 1) {
        qCritical() << "Cannot go forward in history, none exist";
        return;
    }
    ++m_historyPointer;
    if (m_history.at(m_historyPointer) == "m_outline") {
        showHtmlOutline(false);
    } else {
        showObjectApi(m_history.at(m_historyPointer), false);
    }
    // set buttons
    ui->buttonPrev->setEnabled(true);
    if (m_historyPointer >= m_history.count() - 1) {
        ui->buttonNext->setEnabled(false);
    }
}
