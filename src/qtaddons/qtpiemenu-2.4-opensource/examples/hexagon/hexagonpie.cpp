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

/*
    This example demonstrates subclassing of QtPieMenu in order to
    provide a different shape. It reimplements the functions
    generateMask(), reposition() and paintEvent().

    The basic shape is a hexagon.
*/
#include "hexagonpie.h"
#include <QtCore/QObject>
#include <QtGui/QPainter>
#include <QtGui/QBitmap>
#include <QtGui/QFontMetrics>
#include <QtGui/QPolygon>
#include <math.h>

/*!
    Constructs a hexagon pie menu. The \a title, \a parent and \a name
    arguments are passed to QtPieMenu's constructor. \a color sets the
    color of the pie menu. This color will also be used to draw the
    pie item of any parent menu.
*/
HexagonPie::HexagonPie(const QString &title, const QColor &color,
                       QWidget *parent, const char *name)
    : QtPieMenu(title, parent, name), col(color)
{
}

/*!
    Draws a hexagon shape on \a mask.
*/
void HexagonPie::generateMask(QBitmap *mask)
{
    QPainter p(mask);

    p.setPen(Qt::color1);
    p.setBrush(Qt::color1);

    int x = pix.rect().center().x();
    int y = pix.rect().center().y();

    for (int i = 0; i < 6; ++i) {
        double a = indexToAngle(i, 6, true);
        double b = indexToAngle(i + 1, 6, true);
	    double ax = outerRadius() * cos(a);
	    double ay = - outerRadius() * sin(a);
	    double bx = outerRadius() * cos(b);
	    double by = - outerRadius() * sin(b);
	    double cx = innerRadius() * cos(a);
	    double cy = - innerRadius() * sin(a);
	    double dx = innerRadius() * cos(b);
	    double dy = - innerRadius() * sin(b);

        QPolygon poly(4);
        poly.setPoint(0, x + (int) ax, y + (int) ay);
        poly.setPoint(1, x + (int) bx, y + (int) by);
        poly.setPoint(2, x + (int) dx, y + (int) dy);
        poly.setPoint(3, x + (int) cx, y + (int) cy);
        p.drawPolygon(poly);
    }
}

/*!
    The hexagon shaped pie menu has a special rule for placement; we
    want the sub menus to be positioned precisely next to the parent.
    Since this hexagon piemenu has a fixed default size, we can do a
    precise repositioning before the menu is shown.
*/
void HexagonPie::reposition()
{
    if (!parent()->inherits("HexagonPie"))
        return;

    HexagonPie *parentPie = reinterpret_cast<HexagonPie *>(parent());

    QPoint parentCenter = mapToGlobal(parentPie->pos());

    if (parentPie->subMenuAt(0) == this) {
        move(mapFromGlobal(QPoint(parentCenter.x(), parentCenter.y() - 128)));
    } else if (parentPie->subMenuAt(1) == this) {
        move(mapFromGlobal(QPoint(parentCenter.x() - 112, parentCenter.y() - 64)));
    } else if (parentPie->subMenuAt(2) == this) {
        move(mapFromGlobal(QPoint(parentCenter.x() - 112, parentCenter.y() + 64)));
    } else if (parentPie->subMenuAt(3) == this) {
        move(mapFromGlobal(QPoint(parentCenter.x(), parentCenter.y() + 128)));
    } else if (parentPie->subMenuAt(4) == this) {
        move(mapFromGlobal(QPoint(parentCenter.x() + 112, parentCenter.y() + 64)));
    } else if (parentPie->subMenuAt(5) == this) {
        move(mapFromGlobal(QPoint(parentCenter.x() + 112, parentCenter.y() - 64)));
    }
}

/*!
    Draws the hexagon shape, taking shading and highlighting into
    consideration.
*/
void HexagonPie::paintEvent(QPaintEvent *)
{
    QPainter p(&pix);
    pix.fill();

    int x = pix.rect().center().x();
    int y = pix.rect().center().y();

    // Draw the segments
    QFontMetrics fm(font());
    for (int i = 0; i < 6; ++i) {
        p.setPen(Qt::black);
        // Find the brush we should use for drawing the segments of
        // the menu. Each segment has its own color.
        QColor baseColor = col;
        if (itemType(i) == SubMenu)
            baseColor = ((HexagonPie *)subMenuAt(i))->col;

        if (i == highlightedItem()) {
            QColor tmp = Qt::white;
            if (baseColor.isValid()) {
                QColor merge((tmp.red()*3 + baseColor.red()*2)/5,
                             (tmp.green()*3 + baseColor.green()*2)/5,
                             (tmp.blue()*3 + baseColor.blue()*2)/5);
                p.setBrush(QBrush(merge));
            } else {
                p.setBrush(QBrush(tmp));
            }
        } else {
            QColor tmp = palette().background().color();
            if (baseColor.isValid()) {
                QColor merge((tmp.red()*3 + baseColor.red()*2)/5,
                             (tmp.green()*3 + baseColor.green()*2)/5,
                             (tmp.blue()*3 + baseColor.blue()*2)/5);
                p.setBrush(QBrush(merge));
            } else {
                p.setBrush(QBrush(tmp));
            }
        }

        // Draw the polygon
        double a = indexToAngle(i, 6, true);
        double b = indexToAngle(i + 1, 6, true);
	    double ax = outerRadius() * cos(a);
	    double ay = - outerRadius() * sin(a);
	    double bx = outerRadius() * cos(b);
	    double by = - outerRadius() * sin(b);
	    double cx = innerRadius() * cos(a);
	    double cy = - innerRadius() * sin(a);
	    double dx = innerRadius() * cos(b);
	    double dy = - innerRadius() * sin(b);

        QPolygon pointArray(4);
        pointArray.putPoints(0, 4,
                             x + (int) ax, y + (int) ay,
                             x + (int) bx, y + (int) by,
                             x + (int) dx, y + (int) dy,
                             x + (int) cx, y + (int) cy);

        p.drawPolygon(pointArray);

        if (!itemText(i).isEmpty()) {
            QFont font = p.font();
            if (i == highlightedItem()) {
                p.setPen(Qt::white);
                font.setBold(true);
            } else {
                p.setPen(palette().text().color());
                font.setBold(false);
            }

            p.setFont(font);

            // Draw the title text
            QString t = itemText(i);
            p.drawText((int) (x + (ax + bx + cx + dx) / 4 - fm.width(t) / 2),
                       (int) (y + (ay + by + cy + dy) / 4), t);

            font.setBold(false);
            p.setFont(font);
        }
    }

    QPainter p2(this);
    p2.drawPixmap(QPoint(0, 0), pix);
}
