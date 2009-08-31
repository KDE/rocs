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

#include "hexagonpie.h"

#include <QtGui/QApplication>
#include <QtGui/QTextEdit>
#include <QtGui/QMouseEvent>

class MyEditor : public QTextEdit
{
    Q_OBJECT
public:
    MyEditor()
    {
        // Create a pie menu with six submenus, and no action items.
        pie = new HexagonPie("Root", Qt::black, this, "Root");
        pie->insertItem(new HexagonPie("N", Qt::red, pie, "N"));
        pie->insertItem(new HexagonPie("NW", Qt::green, pie, "NW"));
        pie->insertItem(new HexagonPie("SW", Qt::blue, pie, "SW"));
        pie->insertItem(new HexagonPie("S", Qt::yellow, pie, "S"));
        pie->insertItem(new HexagonPie("SE", Qt::gray, pie, "SE"));
        pie->insertItem(new HexagonPie("NE", Qt::cyan, pie, "NE"));

        for (int i = 0; i < 6; ++i) {
            HexagonPie *subPie = (HexagonPie *)pie->subMenuAt(i);
            subPie->insertItem("A", this, SLOT(noop()));
            subPie->insertItem("B", this, SLOT(noop()));
            subPie->insertItem("C", this, SLOT(noop()));
            subPie->insertItem("D", this, SLOT(noop()));
            subPie->insertItem("E", this, SLOT(noop()));
            subPie->insertItem("F", this, SLOT(noop()));
        }
    }

public slots:
    void noop()
    {
    }

protected:
    void mousePressEvent(QMouseEvent *e) 
    {
        if ( e->button() & Qt::RightButton ) {
            pie->popup(e->globalPos());
            return;
        }

        QTextEdit::mousePressEvent( e );
    }

private:
    HexagonPie *pie;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MyEditor editor;
    editor.show();

    return app.exec();
}

#include "main.moc"
