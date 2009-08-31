/****************************************************************************
**
** This file is part of a Qt Solutions component.
** 
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
** 
** Contact:  Qt Software Information (qt-info@nokia.com)
** 
** Commercial Usage  
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage 
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** Please note Third Party Software included with Qt Solutions may impose
** additional restrictions and it is the user's responsibility to ensure
** that they have met the licensing requirements of the GPL, LGPL, or Qt
** Solutions Commercial license and the relevant license of the Third
** Party Software they are using.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** 
****************************************************************************/

#include "demo.h"
#include <QtCore/QFile>
#include <QtGui/QFileDialog>
#include <QtGui/QLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QMouseEvent>

TextEditWithPieMenu::TextEditWithPieMenu(QWidget *parent, const char *name)
    : QTextEdit(parent)
{
    setObjectName(name);

    p = new QtPieMenu("Root menu", this, "Root menu");

    QtPieMenu *submenu = new QtPieMenu("File", p, "File menu");
    submenu->insertItem("Open", this, SIGNAL(openDocument()));

    QtPieMenu *s3 = new QtPieMenu("New", submenu, "New menu");
    s3->insertItem("Project", this, SIGNAL(newProject()));
    s3->insertItem("Document", this, SIGNAL(newDocument()));

    submenu->insertItem(s3);
    submenu->insertItem("Save", this, SLOT(ignoreAction()));
    submenu->insertItem("Close", this, SLOT(ignoreAction()));
    p->insertItem(submenu);
    p->insertItem("Undo", this, SLOT(undo()));

    QtPieMenu *toolsMenu = new QtPieMenu("Tools", p, "Tools menu");
    toolsMenu->insertItem("Spelling", this, SLOT(ignoreAction()));

    QtPieMenu *grammarMenu = new QtPieMenu("Set lang", toolsMenu, "Grammar menu");
    grammarMenu->insertItem(QIcon(QPixmap(":/icons/SWDN001.png")), this, SLOT(ignoreAction()));

    QtPieMenu *indiaMenu = new QtPieMenu(QIcon(QPixmap(":/icons/INDA001.png")),
					 grammarMenu, "India menu");

    indiaMenu->insertItem("Assamese", this, SLOT(ignoreAction()));
    indiaMenu->insertItem("Bengali", this, SLOT(ignoreAction()));
    indiaMenu->insertItem("Gujarati", this, SLOT(ignoreAction()));
    indiaMenu->insertItem("Hindi", this, SLOT(ignoreAction()));
    indiaMenu->insertItem("Kannada", this, SLOT(ignoreAction()));
    grammarMenu->insertItem(indiaMenu);
    QFont fon = indiaMenu->font();
    fon.setPointSize(8);
    indiaMenu->setFont(fon);

    QtPieMenu *norskMenu = new QtPieMenu(QIcon(QPixmap(":/icons/NORW001.png")),
					 grammarMenu, "Grammar menu");

    norskMenu->insertItem("Nynorsk", this, SLOT(ignoreAction()));
    norskMenu->insertItem("Bokmål", this, SLOT(ignoreAction()));
    grammarMenu->insertItem(norskMenu);

    grammarMenu->insertItem(QIcon(QPixmap(":/icons/GERM001.png")), this, SLOT(ignoreAction()));
    grammarMenu->insertItem(QIcon(QPixmap(":/icons/ITAL001.png")), this, SLOT(ignoreAction()));
    toolsMenu->insertItem(grammarMenu);
    toolsMenu->setItemWeight(2, 1);

    QtPieMenu *iconMenu = new QtPieMenu("Apps", toolsMenu, "Apps menu");
    iconMenu->insertItem(QIcon(QPixmap(":/icons/kformula.png")), this, SLOT(ignoreAction()));
    iconMenu->insertItem(QIcon(QPixmap(":/icons/kontour.png")), this, SLOT(ignoreAction()));
    iconMenu->insertItem(QIcon(QPixmap(":/icons/kugar.png")), this, SLOT(ignoreAction()));
    toolsMenu->insertItem(iconMenu);

    QtPieMenu *alphabetMenu = new QtPieMenu("Alpha", toolsMenu, "Alphabet menu");
    alphabetMenu->insertItem("a", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("b", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("c", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("d", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("e", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("f", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("g", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("h", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("i", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("j", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("k", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("l", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("m", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("n", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("o", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("p", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("q", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("r", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("s", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("t", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("u", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("v", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("w", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("x", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("y", this, SLOT(ignoreAction()));
    alphabetMenu->insertItem("z", this, SLOT(ignoreAction()));
    toolsMenu->insertItem(alphabetMenu);

    p->insertItem(toolsMenu);

    p->insertItem("Redo", this, SLOT(redo()));

    p->hide();
}

QSize TextEditWithPieMenu::sizeHint() const
{
    return QSize(200, 150);
}

void TextEditWithPieMenu::undo()
{
    document()->undo();
}

void TextEditWithPieMenu::redo()
{
    document()->redo();
}

void TextEditWithPieMenu::contextMenuEvent(QContextMenuEvent *e)
{
    p->popup(e->globalPos());
}

void TextEditWithPieMenu::ignoreAction()
{
}

Editor::Editor(QWidget *parent, const char *name) : QWidget(parent)
{
    setObjectName(name);

    QVBoxLayout *layout = new QVBoxLayout(this);
    edit = new TextEditWithPieMenu(this, "Edit in Editor");
    label = new QLabel("Document", this);
    label->setObjectName("Label in Editor");
    layout->addWidget(edit);
    layout->addWidget(label);

    connect(edit, SIGNAL(newDocument()), SLOT(setNewDocument()));
    connect(edit, SIGNAL(newProject()), SLOT(setNewProject()));
    connect(edit, SIGNAL(openDocument()), SLOT(doOpenDocument()));
}

void Editor::setNewDocument()
{
    edit->clear();
    label->setText("Document");
}

void Editor::setNewProject()
{
    edit->clear();
    label->setText("Project");
}

void Editor::doOpenDocument()
{
    QString s = QFileDialog::getOpenFileName(this, "Choose a file", ".", "All files (*)");

    if (s == "")
    	return;

    QFile f(s);
    if (f.open(QFile::ReadOnly)) {
	    QByteArray arr = f.readAll();

	    edit->document()->setPlainText(QString(arr));
	    f.close();
    }
}
