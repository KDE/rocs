#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>
#include <KTabBar>
#include <QList>
#include "MainWindow.h"

namespace KTextEditor {
class Document;
class View;
class Editor;
}
class QVBoxLayout;
class QStackedWidget;

class CodeEditor : public QWidget {
    Q_OBJECT
public:
    CodeEditor(MainWindow *parent = 0);
    const char * text() const;
    KTextEditor::Editor *editor() const {
        return _editor;
    }
    KTextEditor::Document *document() const {
        return _activeDocument;
    }
    KTextEditor::View *view() const {
        return _activeView;
    }

public slots:
    void newScript();
    void saveScript();
    void openScript();
    void saveScriptAs();

  private slots:
    void closeDocument(int index);
    void changeCurrentDocument(int index);
private:
    
    QStackedWidget *_docArea;
    KTabBar *_tabDocs; //! the tabs of the opened documents.
    QList<KTextEditor::View*> _docViews; //! this is the view where you edit your scripts
    QList<KTextEditor::Document*> _scriptDocs; //! the document that you are editing
    KTextEditor::Editor *_editor;
    QVBoxLayout* _layout;
    
    KTextEditor::Document *_activeDocument;
    KTextEditor::View     *_activeView;
};

#endif
