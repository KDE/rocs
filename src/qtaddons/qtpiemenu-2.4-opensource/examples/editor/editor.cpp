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

#include "editor.h"
#include <qtpiemenu.h>


#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QTextCursor>


Editor::Editor(QWidget *parent, const char *name)
    : QTextEdit(tr("Mark some text and activate the context menu!"), parent)
{
    setObjectName(name);        
    document()->setUndoRedoEnabled(false);
    
    p = new QtPieMenu("Root menu", this, "Root menu", 10, 50);

    markMenu = new QtPieMenu("Edit", p, "Edit menu", 10, 50);
    markMenu->insertItem("All", this, SLOT(selectAll()));
    markMenu->insertItem("Cut", this, SLOT(cut()));
    markMenu->insertItem("Del", this, SLOT(del()));
    markMenu->insertItem("Copy", this, SLOT(copy()));
    markMenu->setItemWeight(2, 0);
    markMenu->setItemWeight(2, 2);

    p->insertItem(markMenu);

    p->insertItem("Undo", document(), SLOT(undo()));
    p->insertItem("Paste", this, SLOT(paste()));
    p->insertItem("Redo", document(), SLOT(redo()));
    
    connect(this, SIGNAL(undoAvailable(bool)), SLOT(setUndoAvailable(bool)));
    connect(this, SIGNAL(redoAvailable(bool)), SLOT(setRedoAvailable(bool)));
    connect(this, SIGNAL(copyAvailable(bool)), SLOT(setCopyAvailable(bool)));
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), SLOT(setPasteAvailable()));    
   
    setUndoAvailable(false);
    setRedoAvailable(false);
    setCopyAvailable(false);
    setPasteAvailable();
    document()->setUndoRedoEnabled(true);
}

QSize Editor::sizeHint() const
{
    return QSize(200, 150);
}

void Editor::mousePressEvent(QMouseEvent *e)
{
    if ( e->button() & Qt::RightButton ) {
	    p->popup(e->globalPos());
	    return;
    }
    QTextEdit::mousePressEvent( e );
}

void Editor::contextMenuEvent(QContextMenuEvent *)
{
}

void Editor::setUndoAvailable(bool enabled)
{
    p->setItemEnabled(enabled, 1);
}

void Editor::setRedoAvailable(bool enabled)
{
    p->setItemEnabled(enabled, 3);
}

void Editor::setCopyAvailable(bool enabled)
{
    markMenu->setItemEnabled(enabled, 1);
    markMenu->setItemEnabled(enabled, 2);
    markMenu->setItemEnabled(enabled, 3);
}

void Editor::setPasteAvailable()
{
    bool enabled = !QApplication::clipboard()->text(QClipboard::Clipboard).isEmpty();
        
    p->setItemEnabled(enabled, 2);
}

void Editor::del()
{
    textCursor().removeSelectedText();
}

