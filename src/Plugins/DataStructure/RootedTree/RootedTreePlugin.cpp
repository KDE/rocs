/*
    This file is part of RootedTree (Rocs Plugin).
    Copyright 2012  Wagner Reck <wagner.reck@gmail.com>

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

#include "RootedTreePlugin.h"

#include "RootedTreeStructure.h"
#include "RootedTreeNodeItem.h"
#include "RootedTreeEdgeItem.h"

#include "Data.h"

#include <KAboutData>
#include <KPluginFactory>
#include <knuminput.h>
#include <KMessageBox>
#include <KComboBox>
#include <QVBoxLayout>
#include <QCheckBox>


static const KAboutData aboutdata("rocs_RootedTreeStructure", 0, ki18nc("@title Displayed plugin name", "RootedTree Structure") , "0.1" );
using namespace Rocs;

K_PLUGIN_FACTORY( DSPluginFactory, registerPlugin< RootedTreePlugin>(); )
K_EXPORT_PLUGIN( DSPluginFactory(aboutdata) )


RootedTreePlugin::RootedTreePlugin(QObject* parent, const QList< QVariant >& /*args*/ )
    : DataStructurePluginInterface(DSPluginFactory::componentData(), parent)
{
}

RootedTreePlugin::~RootedTreePlugin()
{
}

DataStructurePtr RootedTreePlugin::convertToDataStructure(DataStructurePtr ds, Document * parent)
{
    return RootedTreeStructure::create(ds, parent);
}

DataStructurePtr RootedTreePlugin::createDataStructure ( Document* parent )
{
    return RootedTreeStructure::create(parent);
}

QGraphicsItem*  RootedTreePlugin::dataItem(DataPtr node) const
{
    return new RootedTreeNodeItem(node);
}

QGraphicsItem*  RootedTreePlugin::pointerItem(PointerPtr edge) const
{
    return new RootedTreeEdgeItem(edge);
}
QLayout* RootedTreePlugin::dataExtraProperties(DataPtr /*node*/, QWidget* /*parentWidget*/) const
{
    return 0;
}

QLayout*  RootedTreePlugin::pointerExtraProperties(PointerPtr arg1, QWidget* arg2) const
{
    return  DataStructurePluginInterface::pointerExtraProperties(arg1, arg2);
}

QLayout*  RootedTreePlugin::dataStructureExtraProperties(DataStructurePtr graph, QWidget* parentWidget) const
{
//    if (!m_layout){
    RootedTreeStructure * ds = qobject_cast< RootedTreeStructure* >(graph.get());
    QVBoxLayout* layout = new QVBoxLayout(parentWidget);
    QCheckBox * showPointer = new QCheckBox(i18nc("@option:check", "Show all pointers"),parentWidget);
    layout->addWidget(showPointer);
    connect(showPointer, SIGNAL(toggled(bool)), ds, SLOT(setShowAllPointers(bool)));
    connect(ds, SIGNAL(showPointersChanged(bool)), showPointer, SLOT(setChecked(bool)));
//    }

   showPointer->setChecked(ds->isShowingAllPointers());
   return layout;
}


bool RootedTreePlugin::canConvertFrom(Document* doc) const
{
    QStringList errors;
    QSet<Data*> visited;
    QSet<Data*> cycles;
    QQueue<DataPtr> queue;
    foreach (DataStructurePtr ds, doc->dataStructures()){
        foreach(DataPtr p, ds->dataList()){
            if (visited.contains(p.get())) {
                continue;
            }
            visited.insert(p.get());
            queue.enqueue(p);
            while (!queue.isEmpty()){
                DataPtr c = queue.dequeue();
                foreach ( DataPtr n, c->adjacentDataList()){
                    if (visited.contains(n.get())){
                        if (!cycles.contains(n.get())){
                            errors << i18n("There are cycles at node %1. Data will be lost by conversion.", n->identifier());
                            cycles.insert(n.get());
                        }
                    }else{
                        queue.enqueue(n);
                        visited.insert(n.get());
                    }
                }
            }
        }
    }
    if (!errors.isEmpty()){
        if (KMessageBox::Continue != KMessageBox::warningContinueCancelList(0,
                                                                        i18n("Cannot convert document \'%1\'", doc->name()),
                                                                        errors))
        {
            return false;
        }
    }
    return true;
}
