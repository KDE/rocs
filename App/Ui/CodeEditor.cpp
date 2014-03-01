/*
    This file is part of Rocs.
    Copyright 2009-2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "CodeEditor.h"

#include <KTextEditor/View>
#include <KTextEditor/Editor>
#include <KTextEditor/Document>
#include <KMessageBox>
#include <KLocale>
#include <QVBoxLayout>
#include "MainWindow.h"
#include <KFileDialog>
#include <QIcon>
#include <QDebug>
#include <QStackedWidget>

CodeEditor::CodeEditor(MainWindow *parent) : QWidget(parent)
{
    _layout = new QVBoxLayout();
    _editor = KTextEditor::Editor::instance();
    if (!_editor) {
        KMessageBox::error(this, i18n("A KDE Text Editor could not be found, \n please, check your installation"));
        exit(1);
    }
    _tabDocs = new KTabBar(this);
    _docArea = new QStackedWidget(this);
    connect(_tabDocs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeDocument(int)));
    connect(_tabDocs, SIGNAL(currentChanged(int)), this, SLOT(changeCurrentDocument(int)));
    connect(_tabDocs, SIGNAL(newTabRequest()), this, SLOT(newScript()));

    _tabDocs->setTabsClosable(false);

//     _editor->setSimpleMode(false); //FIXME

    _layout->addWidget(_tabDocs);
    _layout->addWidget(_docArea);

    _layout->setSpacing(0);
//     _layout->setMargin(0); //FIXME

    setLayout(_layout);
}

void CodeEditor::closeAllScripts()
{
    while (_scriptDocs.size() != 0) {
        closeDocument(0);
    }

}


void CodeEditor::closeDocument(int index)
{
    // change tab to next document, if present
    if (index == 0) {
        if (_scriptDocs.size() >= 2) {
            _activeDocument = _scriptDocs.at(1);
            _activeView = _docViews.at(1);
            _docArea->setCurrentIndex(1);
        }
    } else {
        _activeDocument = _scriptDocs.at(index - 1);
        _activeView = _docViews.at(index - 1);
        _docArea->setCurrentIndex(index - 1);
        _tabDocs->setCurrentIndex(index - 1);
    }

    // remove document
    _scriptDocs.at(index)->deleteLater();
    _scriptDocs.removeAt(index);

    _docArea->removeWidget(_docArea->widget(index));
    _docViews.removeAt(index);
    _tabDocs->removeTab(index);
}


void CodeEditor::changeCurrentDocument(int index)
{
    if (_scriptDocs.size() < index || index < 0) {
        qDebug() << "Do not change to non-existing code document.";
        return;
    }
    _activeDocument = _scriptDocs.at(index);
    _activeView = _docViews.at(index);
    _docArea->setCurrentIndex(index);
    _tabDocs->setCurrentIndex(index);
}


KTextEditor::Document* CodeEditor::newScript()
{
    _scriptDocs  << _editor->createDocument(0);

#ifdef USING_QTSCRIPT
    _scriptDocs.last()->setMode("JavaScript");
#endif

    _docViews << qobject_cast<KTextEditor::View*>(_scriptDocs.last()->createView(this));

    _tabDocs->addTab(_scriptDocs.last()->documentName());
    _docArea->addWidget(_docViews.last());
    changeCurrentDocument(_docViews.count() - 1);
    connect(_activeDocument, SIGNAL(documentNameChanged(KTextEditor::Document*)), this, SLOT(updateTabText(KTextEditor::Document*)));
    connect(_activeDocument, SIGNAL(modifiedChanged(KTextEditor::Document*)), this, SLOT(updateTabText(KTextEditor::Document*)));

    updateTabText(_scriptDocs.last());
    qDebug() << "New script created.";

    return _scriptDocs.last();
}


KTextEditor::Document* CodeEditor::newScript(const QUrl& file)
{
    KTextEditor::Document* script = newScript();
    script->saveAs(file);
    return script;
}


void CodeEditor::updateTabText(KTextEditor::Document* text)
{
    int index = _scriptDocs.indexOf(text);
    _tabDocs->setTabText(index, text->documentName());

    // tell user if current modifications are unsaved
    if (text->isModified()) {
        _tabDocs->setTabIcon(index, QIcon("document-save"));
        return;
    }

    if (text->documentName().endsWith(QLatin1String(".js"), Qt::CaseInsensitive)) {
        _tabDocs->setTabIcon(index, QIcon("application-javascript"));
    } else if (text->documentName().endsWith(QLatin1String(".py"), Qt::CaseInsensitive)) {
        _tabDocs->setTabIcon(index, QIcon("text-x-python"));
    } else {
        _tabDocs->setTabIcon(index, QIcon("text-x-generic"));
    }
}


void CodeEditor::openScript(const QUrl& fileUrl)
{
    if (!fileUrl.isValid()) {
        return;
    }

    KTextEditor::Document *d = _editor->createDocument(0);
    d->openUrl(fileUrl);

#ifdef USING_QTSCRIPT
    d->setMode("JavaScript");
#endif
    _scriptDocs << d;
    _docViews << qobject_cast<KTextEditor::View*>(_scriptDocs.last()->createView(this));
    _tabDocs->addTab(_scriptDocs.last()->documentName());
    _docArea->addWidget(_docViews.last());

    connect(d, SIGNAL(documentNameChanged(KTextEditor::Document*)), this, SLOT(updateTabText(KTextEditor::Document*)));
    connect(d, SIGNAL(modifiedChanged(KTextEditor::Document*)), this, SLOT(updateTabText(KTextEditor::Document*)));
    updateTabText(d);
    changeCurrentDocument(_docViews.count() - 1);

    qDebug() << "Being Called";
}


void CodeEditor::saveScript(KTextEditor::Document *doc = 0)
{
    qDebug() << "my doc is: " << doc;
    if ((_docViews.empty()) || (_scriptDocs.empty())) {
        return;
    }
    if (doc == 0) {
        doc = _activeDocument;
    }
    if (doc->isModified() == false) {
        return;
    }
    if (doc->url().isEmpty()) {
        saveScriptAs(doc);
        return;
    }
    doc->documentSave();
    updateTabText(doc);
}


void CodeEditor::saveScriptAs(KTextEditor::Document *doc = 0)
{
    if ((_docViews.empty()) || (_scriptDocs.empty())) {
        return;
    }
    if (doc == 0) {
        doc = _activeDocument;
    }
    doc->documentSaveAs();
    updateTabText(doc);
}


void CodeEditor::saveAllScripts()
{
    //FIXME this should only happen for scripts with url
    if (_scriptDocs.empty()) {
        return;
    }
    foreach(KTextEditor::Document * text, _scriptDocs) {
        if (text->url().isEmpty()) {
            if (text->isModified() == false) {
                continue;
            }
            saveScriptAs(text);
        } else {
            saveScript(text);
        }
    }
}


void CodeEditor::saveActiveScript()
{
    saveScript();
}


void CodeEditor::saveActiveScriptAs()
{
    saveScriptAs();
}


QString CodeEditor::text() const
{
    return _activeDocument->text().toAscii();
}


bool CodeEditor::isModified() const
{
    bool modified = false;
    foreach(KTextEditor::Document * text, _scriptDocs) {
        if (text->isModified()) modified = true;
    }
    return modified;
}
