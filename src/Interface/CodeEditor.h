/* 
    This file is part of Rocs.
    Copyright 2009-2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2011       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
    QString text() const;
    KTextEditor::Editor *editor() const {
        return _editor;
    }
    KTextEditor::Document *document() const {
        return _activeDocument;
    }
    KTextEditor::View *view() const {
        return _activeView;
    }
    /** This method gives modification state of code-editor texts.
     * \return true if any document in the code-editor is modified, otherwise false
     */
    bool isModified() const;

public slots:
    void newScript();
    void saveActiveScript();
    void saveActiveScriptAs();
    void saveAllScripts();
    void openScript();

private slots:
    void closeDocument(int index);
    void changeCurrentDocument(int index);
    void updateTabText(KTextEditor::Document* text);

private:
    /** Save the given script or if no text document is given, the currently active script is
     *  saved.
     * \param doc optional document to be saved
     */
    void saveScript(KTextEditor::Document *doc);
    /** Save given script as new file or if no text document given, the currently active script
     *  is saved as a new file.
     * \param doc optional document to be saved
     */
    void saveScriptAs(KTextEditor::Document *doc);
    /** Call \see saveScript for all documents in the code editor */
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
