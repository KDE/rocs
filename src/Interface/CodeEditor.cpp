#include "CodeEditor.h"

#include <ktexteditor/view.h>
#include <ktexteditor/editorchooser.h>
#include <KMessageBox>
#include <KLocale>
#include <KXMLGUIFactory>
#include <QHBoxLayout>
#include "MainWindow.h"
#include <KParts/ReadOnlyPart>
#include <KFileDialog>

CodeEditor::CodeEditor(MainWindow *parent) : QWidget(parent) {
    _layout = new QHBoxLayout();
    _editor = KTextEditor::EditorChooser::editor();
    _docView = 0;
    _scriptDoc = 0;
    if (!_editor) {
        KMessageBox::error(this, i18n("A KDE Text Editor could not be found, \n please, check your installation"));
        exit(1);
    }
    _editor->setSimpleMode(true);
    newScript();
    setLayout(_layout);
}

void CodeEditor::newScript() {
    if (_docView) {
        _layout->removeWidget(_docView);
        delete _docView;
        _docView = 0;
    }
    if (_scriptDoc) {
        delete _scriptDoc;
        _scriptDoc =0;
    }

    _scriptDoc = _editor->createDocument(0);
#ifdef USING_QTSCRIPT
    _scriptDoc->setMode("JavaScript");
#endif
    _docView = qobject_cast<KTextEditor::View*>(_scriptDoc->createView(this));
    _layout->addWidget(_docView);
    kDebug() << "New script created.";
}

void CodeEditor::saveScript() {
    if ((!_docView) ||  (!_scriptDoc)) {
        return;
    }

    _scriptDoc->documentSave();
}

void CodeEditor::openScript() {
    _scriptDoc->openUrl(KFileDialog::getOpenUrl() );
#ifdef USING_QTSCRIPT
    _scriptDoc->setMode("JavaScript");
#endif
}

void CodeEditor::saveScriptAs() {
    if ((!_docView) ||  (!_scriptDoc)) {
        return;
    }

    _scriptDoc->documentSaveAs();
}

const char* CodeEditor::text() const {
    return _docView->document()->text().toAscii();
}
