#include "UI_GraphEditWidget.h"

#include <ktexteditor/editor.h>
#include <ktexteditor/editorchooser.h>

GraphEditWidget::GraphEditWidget(QWidget *parent) : QWidget(parent)
{
  setupUi(this);
  
  _editor = KTextEditor::EditorChooser::editor();
  _editor -> setSimpleMode ( true );
}