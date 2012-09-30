/*
    This file is part of Rocs.
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>

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

#include "GraphPlugin.h"

#include "GraphStructure.h"
#include "NodeItem.h"
#include "EdgeItem.h"
#include "Data.h"

#include <KAboutData>
#include <KPluginFactory>
#include <knuminput.h>
#include <KMessageBox>
#include <KComboBox>

#include <QtGui/QGridLayout>
#include <QtGui/QSpinBox>
#include <QtGui/QLabel>

static const KAboutData aboutdata("rocs_GraphStructure", 0, ki18nc("@title Displayed plugin name", "Graph Structure"), "0.1");
using namespace Rocs;

K_PLUGIN_FACTORY(DSPluginFactory, registerPlugin<GraphPlugin>();)
K_EXPORT_PLUGIN(DSPluginFactory(aboutdata))

GraphPlugin::GraphPlugin(QObject* parent, const QList<QVariant>& /*args*/)
    : DataStructurePluginInterface(DSPluginFactory::componentData(), parent)
{
}

GraphPlugin::~GraphPlugin()
{
}

DataStructurePtr GraphPlugin::convertToDataStructure(DataStructurePtr ds, Document * parent)
{
    return GraphStructure::create(ds, parent);
}

DataStructurePtr GraphPlugin::createDataStructure(Document* parent)
{
    return GraphStructure::create(parent);
}

QGraphicsItem*  GraphPlugin::dataItem(DataPtr node) const
{
    return new NodeItem(node);
}

QGraphicsItem*  GraphPlugin::pointerItem(PointerPtr edge) const
{
    return new EdgeItem(edge);
}
QLayout* GraphPlugin::dataExtraProperties(DataPtr node, QWidget* parentWidget) const
{
    QGridLayout * lay = new QGridLayout(parentWidget);
    QSpinBox * y = new QSpinBox(parentWidget);
    QSpinBox * x = new QSpinBox(parentWidget);
    KDoubleNumInput * size = new KDoubleNumInput(parentWidget);
    y->setMinimum(-9999);
    y->setMaximum(9999);
    x->setMinimum(-9999);
    x->setMaximum(9999);
    size->setMinimum(0.15);
    size->setMaximum(3.0);
    size->setSingleStep(0.15);
    y->setValue(node->y());
    x->setValue(node->x());
    size->setValue(node->width());

    connect(x,            SIGNAL(valueChanged(int)),             node.get(), SLOT(setX(int)));
    connect(y,            SIGNAL(valueChanged(int)),             node.get(), SLOT(setY(int)));
    connect(size,         SIGNAL(valueChanged(double)), node.get(), SLOT(setWidth(double)));

    QFrame * line = new QFrame(parentWidget);
    line->setFrameShape(QFrame::VLine);

    lay->addWidget(new QLabel(i18nc("coordinate axis", "x"), parentWidget), 0, 0);
    lay->addWidget(new QLabel(i18nc("coordinate axis", "y"), parentWidget), 1, 0);
    lay->addWidget(new QLabel(i18n("Size"), parentWidget), 0, 3);
    lay->addWidget(line, 0, 2, 2, 1);
    lay->addWidget(x, 0, 1);
    lay->addWidget(y, 1, 1);
    lay->addWidget(size, 0, 4);

    return lay;
}

QLayout* GraphPlugin::pointerExtraProperties(PointerPtr arg1, QWidget* arg2) const
{
    return DataStructurePluginInterface::pointerExtraProperties(arg1, arg2);
}

QLayout* GraphPlugin::dataStructureExtraProperties(DataStructurePtr graph, QWidget* parentWidget) const
{
    QGridLayout *lay = new QGridLayout(parentWidget);
    QLabel * _graphTypeText = new QLabel(i18n("Graph Type:"));
    KComboBox * _graphTypeCombo = new KComboBox(parentWidget);
    _graphTypeCombo->insertItem(GraphStructure::Graph, i18nc("@label:inlistbox", "Graph"));
    _graphTypeCombo->insertItem(GraphStructure::Multigraph,
        i18nc("@label:inlistbox graph for which several edges between same nodes may exist", "Multigraph"));

    lay->addWidget(_graphTypeText, 0, 0);
    lay->addWidget(_graphTypeCombo, 0, 1);

    GraphStructure * tmp = dynamic_cast<GraphStructure *>(boost::static_pointer_cast<GraphStructure>(graph).get());
    _graphTypeCombo->setCurrentIndex(tmp->graphType());

    connect(_graphTypeCombo, SIGNAL(currentIndexChanged(int)), tmp, SLOT(setGraphType(int)));

    return lay;
}

bool GraphPlugin::canConvertFrom(Document*) const
{
    return true;
}
