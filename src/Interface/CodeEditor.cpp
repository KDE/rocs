#include "CodeEditor.h"

#include <ktexteditor/view.h>
#include <ktexteditor/editorchooser.h>
#include <KMessageBox>
#include <KLocale>
#include <QVBoxLayout>
#include "MainWindow.h"
#include <KFileDialog>
#include <QStackedWidget>

CodeEditor::CodeEditor(MainWindow *parent) : QWidget(parent) {
    _layout = new QVBoxLayout();
    _editor = KTextEditor::EditorChooser::editor();
    if (!_editor) {
        KMessageBox::error(this, i18n("A KDE Text Editor could not be found, \n please, check your installation"));
        exit(1);
    }
    _tabDocs = new KTabBar(this);
    _docArea = new QStackedWidget(this);
    connect( _tabDocs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeDocument(int)));
    connect( _tabDocs, SIGNAL(currentChanged(int)), this, SLOT( changeCurrentDocument(int)));
    connect( _tabDocs, SIGNAL(newTabRequest()), this, SLOT(newScript()));
    
    _tabDocs->setTabsClosable(true);
    
    _editor->setSimpleMode(false);
    
    _layout->addWidget(_tabDocs);
    _layout->addWidget(_docArea);
    
    newScript();
    setLayout(_layout);
}

void CodeEditor::closeDocument(int index){
  kDebug() << _scriptDocs.size();
  if(_scriptDocs.size() == 1){
    kDebug() << "Just one, creating new script";
    newScript();
    _scriptDocs.removeAt(0);
    _docArea->removeWidget(_docArea->widget(0));
    _docViews.removeAt(0);
    _tabDocs->removeTab(0);
  }
  else if (index == 0){
      kDebug() << "Deleting the first";
      _activeDocument = _scriptDocs.at(1);
      _activeView = _docViews.at(1);
      _docArea->setCurrentIndex(1);
      _scriptDocs.removeAt(0);
      _docArea->removeWidget(_docArea->widget(0));
      _docViews.removeAt(0);
      _tabDocs->removeTab(0);
 }else {
      kDebug() << "Deleting in the middle / end ";
      _activeDocument = _scriptDocs.at( index - 1 );
      _activeView = _docViews.at(index - 1);
      _docArea->setCurrentIndex(index - 1);
      _tabDocs->setCurrentIndex(index - 1);
      
      _scriptDocs.removeAt( index );
      _docArea->removeWidget(_docArea->widget( index));
      _docViews.removeAt( index );
      _tabDocs->removeTab( index );
 }
}

void CodeEditor::changeCurrentDocument(int index){
  _activeDocument = _scriptDocs.at(index);
  _activeView = _docViews.at(index);
  _docArea->setCurrentIndex(index);
}

void CodeEditor::newScript() {
    _scriptDocs  << _editor->createDocument(0);
    
 #ifdef USING_QTSCRIPT
    _scriptDocs.last()->setMode("JavaScript");
 #endif

    _docViews << qobject_cast<KTextEditor::View*>(_scriptDocs.last()->createView(this));
    
    _tabDocs->addTab(_scriptDocs.last()->documentName());
    _docArea->addWidget(_docViews.last());
    _activeDocument = _scriptDocs.last();
    _activeView = _docViews.last();
    
    kDebug()<< "New script created.";
}

void CodeEditor::saveScript() {
    if ((_docViews.empty()) ||  (_scriptDocs.empty())) {
        return;
    }
    
    _activeDocument->documentSave();
}

void CodeEditor::openScript() {
    KUrl fileUrl = KFileDialog::getOpenUrl();
    if (!fileUrl.isValid()) return;
    
    KTextEditor::Document *d = _editor->createDocument(0);
    d->openUrl( fileUrl );
    
#ifdef USING_QTSCRIPT
    d->setMode("JavaScript");
#endif
    
    
}

void CodeEditor::saveScriptAs() {
    if ((_docViews.empty()) ||  (_scriptDocs.empty())) {
        return;
    }

    _activeDocument->documentSaveAs();
}

const char* CodeEditor::text() const {
    return _activeDocument->text().toAscii();
}
