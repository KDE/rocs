/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "GraphPlugin.h"
#include "GraphStructure.h"
#include <KPluginFactory>
#include <KAboutData>
#include <NodeItem.h>
#include <QGridLayout>
#include <QSpinBox>
#include <QLabel>
#include <knuminput.h>
#include <QCheckBox>
#include "OrientedEdgeItem.h"

static const KAboutData aboutdata("rocs_GraphStructure", 0, ki18n("Graph Structure") , "0.1" );
using namespace Rocs;

K_PLUGIN_FACTORY( DSPluginFactory, registerPlugin< GraphPlugin>(); )
K_EXPORT_PLUGIN( DSPluginFactory(aboutdata) )


GraphPlugin::GraphPlugin ( QObject* parent, const QList< QVariant >& /*args*/ )
       : DSPluginInterface( DSPluginFactory::componentData(), parent)
{

}

GraphPlugin::~GraphPlugin()
{

}

DataStructureBase* GraphPlugin::changeToDS ( DataStructureBase* graph) {
    return new GraphStructure(*graph);
}

DataStructureBase* GraphPlugin::createDS ( GraphDocument* parent )
{
  return new GraphStructure(parent);
//     return new Graph(parent);
}

QGraphicsItem* Rocs::GraphPlugin::nodeItem(Node* node) const
{
    QGraphicsItem * item = new NodeItem(node);
    return item;
}

QGraphicsItem* Rocs::GraphPlugin::edgeItem ( Edge* edge) const
{
  return new OrientedEdgeItem(edge);
}
QLayout* GraphPlugin::nodeExtraProperties ( Node* node, QWidget* parentWidget ) const
{
  QGridLayout * lay = new QGridLayout(parentWidget);
  QSpinBox * y = new QSpinBox(parentWidget);
  QSpinBox * x = new QSpinBox(parentWidget);
  KDoubleNumInput * size = new KDoubleNumInput(parentWidget);
  y->setMinimum(0);
  y->setMaximum(9999);
  x->setMinimum(0);
  x->setMaximum(9999);
  size->setMinimum(0.5);
  size->setMaximum(999.0);
  size->setSingleStep(0.25);
  y->setValue(node->y());
  x->setValue(node->x());
  size->setValue(node->width());

  connect( x,            SIGNAL(valueChanged(int)),      node, SLOT(setX(int)));
  connect( y,            SIGNAL(valueChanged(int)),      node, SLOT(setY(int)));
  connect( size,        SIGNAL(valueChanged(double)),   node, SLOT(setWidth(double)));
  QFrame * line = new QFrame(parentWidget);
  line->setFrameShape(QFrame::VLine);

  lay->addWidget(new QLabel(i18n("x"), parentWidget), 0,0);
  lay->addWidget(new QLabel(i18n("y"), parentWidget), 1,0);
  lay->addWidget(new QLabel(i18n("Size"), parentWidget), 0,3);
  lay->addWidget(line, 0, 2);
  lay->addWidget(x, 0,1);
  lay->addWidget(y, 1,1);
  lay->addWidget(size, 0,4);

  return lay;
}

QLayout* Rocs::GraphPlugin::edgeExtraProperties ( Edge* arg1, QWidget* arg2 ) const
{
    return Rocs::DSPluginInterface::edgeExtraProperties ( arg1, arg2 );
}

QLayout* Rocs::GraphPlugin::graphExtraProperties ( DataStructureBase* graph, QWidget* parentWidget ) const
{
  QGridLayout *lay = new QGridLayout(parentWidget);
    QCheckBox * _graphOriented = new QCheckBox (i18n("Graph is oriented"), parentWidget);
    _graphOriented->setCheckState( graph->directed()  ?   Qt::Checked  :   Qt::Unchecked);
    lay->addWidget(_graphOriented, 0,0);
    connect( _graphOriented, SIGNAL(toggled(bool)), graph, SLOT(setDirected(bool)));
    return lay;
}
