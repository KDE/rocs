#ifndef DATASTRUCTUREPLUGININTERFACE_H
#define DATASTRUCTUREPLUGININTERFACE_H

#include <kplugininfo.h>

//Qt classes
class QLayout;
class QGraphicsItem;

//Rocs Classes
class DataStructure;
class Data;
class Pointer;

class Document;

// KClasses
class KComponentData;

#include "rocslib_export.h"


class ROCSLIB_EXPORT DataStructurePluginInterface: public QObject
{
  Q_OBJECT

public:
  DataStructurePluginInterface(const KComponentData &instance, QObject* parent);

  virtual ~DataStructurePluginInterface();
  virtual DataStructure* createDataStructure(Document * parent) = 0;
  virtual DataStructure* changeToDataStructure(DataStructure*, Document* parent) = 0;

  QString name();

  virtual QGraphicsItem * dataItem (Data *data) const = 0;
  virtual QGraphicsItem * pointerItem (Pointer *pointer) const = 0;

  virtual QLayout* dataExtraProperties ( Data* data, QWidget* widget ) const;
  virtual QLayout* pointerExtraProperties ( Pointer* pointer, QWidget* widget )const;
  virtual QLayout* dataStructureExtraProperties ( DataStructure *dataStructure, QWidget* widget )const;

};


#endif // DataStructurePLUGININTERFACE_H
