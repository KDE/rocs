
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
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "UI_GraphLayers.h"
#include "UI_MainWindow.h"
#include "Graph.h"
#include "GraphCollection.h"

#include <KPushButton>
#include <KLocale>

#include <QTreeWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>

class GraphCollection;
GraphLayers::GraphLayers ( MainWindow *parent ) 
: QWidget ( parent ) 
{
	QStringList q; q << i18n("Show") << i18n("Color") << i18n("Name");
	_centralArea = new QTreeWidget(this);
	_centralArea->setHeaderHidden(false);
	_centralArea->setHeaderLabels(q);
	_centralArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	_btnAdd = new KPushButton(i18n("Add"), this);
	_btnDuplicate = new KPushButton(i18n("Clone"), this);
	_btnDelete = new KPushButton(i18n("Remove"), this);

	QHBoxLayout *hboxLayout1 = new QHBoxLayout( );
	hboxLayout1->addWidget(_btnAdd);
	hboxLayout1->addWidget(_btnDuplicate);
	hboxLayout1->addWidget(_btnDelete);

	QVBoxLayout *vboxLayout1 = new QVBoxLayout(this);
	vboxLayout1->addWidget( _centralArea );	
	vboxLayout1->addLayout( hboxLayout1 );
	setLayout(vboxLayout1);
}

void GraphLayers::setGraphs(GraphCollection *graphs)
{
	// do nothing if the graph collection is the same as the active graphs.
	if (_graphCollection == graphs)
	{
		return;
	}

	_centralArea->clear();
	
	QList<Graph*> gList = _graphCollection->graphs();
	foreach(Graph *g, gList)
	{
		QTreeWidgetItem *i = new QTreeWidgetItem(_centralWidget);
		i->setFlags(Qt::ItemIsUserCheckable);
	}
}