#include "CodeEditor.h"

#include <ktexteditor/view.h>
#include <ktexteditor/editorchooser.h>
#include <KMessageBox>
#include <KLocale>
#include <QHBoxLayout>

CodeEditor::CodeEditor(QWidget *parent) : QWidget(parent){
  QHBoxLayout *l = new QHBoxLayout();
    KTextEditor::Editor *editor = KTextEditor::EditorChooser::editor();
    if (!editor) {
        KMessageBox::error(this, i18n("A KDE Text Editor could not be found, \n please, check your installation"));
        exit(1);
    }
    editor->setSimpleMode(true);
    _scriptDoc = editor->createDocument(0);
    _docView = qobject_cast<KTextEditor::View*>(_scriptDoc->createView(this));
  
  #ifdef USING_QTSCRIPT
   _scriptDoc->setMode("JavaScript");	
  #endif
  l->addWidget(_docView);
  setLayout(l);
}

const char* CodeEditor::text() const{
  return _docView->document()->text().toAscii();
}