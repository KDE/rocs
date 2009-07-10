#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>

namespace KTextEditor {
  class Document;
  class View;
}

class CodeEditor : public QWidget{
public:
  CodeEditor(QWidget *parent = 0);
  const char * text() const;

private:
   KTextEditor::View *_docView; //! this is the view where you edit your scripts
   KTextEditor::Document *_scriptDoc; //! the document that you are editing
};

#endif