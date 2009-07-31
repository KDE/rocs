#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>
#include "SUI_MainWindow.h"

namespace KTextEditor {
  class Document;
  class View;
  class Editor;
}
class QHBoxLayout;

class CodeEditor : public QWidget{
  Q_OBJECT
public:
  CodeEditor(MainWindow *parent = 0);
  const char * text() const;
  KTextEditor::Editor *editor() const { return _editor; }
  KTextEditor::Document *document() const { return _scriptDoc; }
  
public slots:
      void newScript();
      void saveScript();
      void openScript();
      void saveScriptAs();
      
private:
   KTextEditor::View *_docView; //! this is the view where you edit your scripts
   KTextEditor::Document *_scriptDoc; //! the document that you are editing
   KTextEditor::Editor *_editor;
    QHBoxLayout* _layout;
};

#endif