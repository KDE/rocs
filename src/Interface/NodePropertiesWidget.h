#ifndef SUI_NODEPROPERTIESWIDGET_H
#define SUI_NODEPROPERTIESWIDGET_H

/* This file is part of Rocs,
	 Copyright (C) 2008 by:
	 Tomaz Canabrava <tomaz.canabrava@gmail.com>
	 Ugo Sangiori <ugorox@gmail.com>

	 Rocs is free software; you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation; either version 2 of the License, or
	 (at your option) any later version.

	 Rocs is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Step; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
*/
#include <QWidget>
#include "ui_NodePropertiesWidget.h"

class Node;
class MainWindow;
class NodeItem;
#include <QPointF>

/*! \brief Properties Area, not used yet. */
class NodePropertiesWidget : public QWidget, public Ui::NodePropertiesWidget
{
    Q_OBJECT
public:
    NodePropertiesWidget (MainWindow* parent = 0 );
    void setNode(NodeItem *n, QPointF pos);
    void disconnectNode(Node* arg1);


private slots:
    void on__color_activated(const QColor& c);
    void on__images_activated(const QString& s);
    void reflectAttributes();
    void updateAutomateAttributes(bool b);
    void on__addProperty_clicked();
    
private:

    Node *_node;
    MainWindow *_mainWindow;
    NodeItem *_item;
    QString _svgFile;
    
};
#endif
