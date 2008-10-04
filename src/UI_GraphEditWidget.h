#ifndef UI_GRAPH_EDIT_WIDGET_H
#define UI_GRAPH_EDIT_WIDGET_H

#include <QWidget>
#include "ui_GraphEditWidget.h"

namespace KTextEditor{
  class View;
  class Editor;
  class Document;
};

class GraphEditWidget : public QWidget, public Ui::GraphEditWidget{
public:
  GraphEditWidget(QWidget *parent = 0);
  QString text() const;
  void setDocument ( KTextEditor::Document *d );

private:
  KTextEditor::Editor *_editor;
  KTextEditor::View *_txtEditScriptView;
  KTextEditor::Document *_txtEditScriptDocument;
};

#endif