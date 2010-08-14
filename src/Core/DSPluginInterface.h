#ifndef DSPLUGININTERFACE_H
#define DSPLUGININTERFACE_H

#include "rocslib_export.h"

class QLayout;
class QGraphicsItem;
class GraphDocument;
class Graph;
#include <KComponentData>
#include "DSPluginManager.h"
#include <kplugininfo.h>

namespace Rocs{
class ROCSLIB_EXPORT DSPluginInterface: public QObject
{
  Q_OBJECT
public:
  DSPluginInterface(const KComponentData &instance, QObject* parent);

  virtual ~DSPluginInterface();

  virtual DataStructureBase* createDS(GraphDocument * parent) = 0;

  virtual DataStructureBase* changeToDS(DataStructureBase*) = 0;
// QString name();


  QString name() {
    if(DSPluginManager::New()->pluginInfo(this).isValid()){
      return DSPluginManager::New()->pluginInfo(this).name();
    }
    return QString();
  }

  virtual QGraphicsItem * nodeItem ( Node*)const = 0;
  virtual QGraphicsItem * edgeItem ( Edge*)const = 0;

  virtual QLayout* nodeExtraProperties ( Node* arg1, QWidget* arg2 ) const;
  virtual QLayout* edgeExtraProperties ( Edge* arg1, QWidget* arg2 )const;
  virtual QLayout* graphExtraProperties ( DataStructureBase* arg1, QWidget* arg2 )const;



};
}
#endif // DSPLUGININTERFACE_H