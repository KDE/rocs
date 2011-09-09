/*
    This file is part of Rocs.
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

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

#include "ListPlugin.h"
#include "ListStructure.h"
#include <KPluginFactory>
#include <KAboutData>
#include "NodeListItem.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <kcombobox.h>
#include "LinkedListPointerItem.h"
#include "Pointer.h"
#include "Data.h"
#include <Document.h>
#include <KDebug>
#include <DocumentManager.h>
#include <KMessageBox>

static const KAboutData aboutdata("rocs_ListStructure", 0, ki18n("Linked List Structure") , "0.1" );
using namespace Rocs;

K_PLUGIN_FACTORY( DSPluginFactory, registerPlugin< ListPlugin>(); )
K_EXPORT_PLUGIN( DSPluginFactory(aboutdata) )


ListPlugin::ListPlugin ( QObject* parent, const QList< QVariant >& /*args*/ )
       : DataStructurePluginInterface( DSPluginFactory::componentData(), parent)
{

}

ListPlugin::~ListPlugin()
{

}

DataStructurePtr ListPlugin::convertToDataStructure ( DataStructurePtr ds, Document* parent) {
    return ListStructure::create(ds, parent);
}

DataStructurePtr ListPlugin::createDataStructure ( Document* parent )
{
  return ListStructure::create(parent);
}

QGraphicsItem* Rocs::ListPlugin::dataItem(DataPtr node ) const
{
    return (new NodeItem(node));
}

QGraphicsItem* Rocs::ListPlugin::pointerItem ( PointerPtr edge) const
{
    return new LinkedListPointerItem (edge);
}

QLayout* Rocs::ListPlugin::nodeExtraProperties ( DataPtr node, QWidget* parentWidget) const
{
  QGridLayout * lay = new QGridLayout(parentWidget);
  QLabel *_value = new QLabel(i18n("Front value"), parentWidget);
  QLineEdit *_valueLine = new QLineEdit(parentWidget);
  _valueLine->setReadOnly(true);
  if (node->out_pointers().count() == 1){
    _valueLine->setText(node->out_pointers().at(0)->to()->value().toString());
  }
  lay->addWidget(_value,0,0);
  lay->addWidget(_valueLine,0,1);


  return lay;
}

bool ListPlugin::canConvertFrom(Document* doc) const
{
  QStringList errors;
  foreach (DataStructurePtr ds, doc->dataStructures()){
    foreach (DataPtr data, ds->dataList()){
      if(data->out_pointers().count() > 1)
        errors.append(i18n("Data \'%1\' had more than one(1) out pointers;").arg(data->name()));
    }
  }

  //convert if no errors or user click continue
  if (errors.isEmpty()
      || KMessageBox::Continue == KMessageBox::warningContinueCancelList(0,
                                                                    i18n("Cannot convert document \'%1\'")
                                                                                        .arg(doc->name()),
                                                                    errors)){
    return true;
  }
  return false;
}

