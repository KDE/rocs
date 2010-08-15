#ifndef DSPLUGININTERFACE_H
#define DSPLUGININTERFACE_H

#include <kplugininfo.h>

//Qt classes
class QLayout;
class QGraphicsItem;

//Rocs Classes
class DataType;
class Datum;
class Pointer;

class DataTypeDocument;

// KClasses
class KComponentData;

#include "rocslib_export.h"

namespace Rocs{
class DSPluginManager;

class ROCSLIB_EXPORT DSPluginInterface: public QObject
{
  Q_OBJECT

public:
  DSPluginInterface(const KComponentData &instance, QObject* parent);

  virtual ~DSPluginInterface();
  virtual DataType* createDS(DataTypeDocument * parent) = 0;
  virtual DataType* changeToDS(DataType*) = 0;

  QString name();

  virtual QGraphicsItem * datumItem ( Datum*)const = 0;
  virtual QGraphicsItem * pointerItem ( Pointer*)const = 0;

  virtual QLayout* datumExtraProperties ( Datum* arg1, QWidget* arg2 ) const;
  virtual QLayout* pointerExtraProperties ( Pointer* arg1, QWidget* arg2 )const;
  virtual QLayout* dataTypeExtraProperties ( DataType* arg1, QWidget* arg2 )const;

};


};

#endif // DSPLUGININTERFACE_H