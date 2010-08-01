#ifndef DSPLUGININTERFACE_H
#define DSPLUGININTERFACE_H

#include "rocslib_export.h"

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

  virtual Graph* createDS(GraphDocument * parent) = 0;

  virtual Graph* changeToDS(Graph*) = 0;
// QString name();


  QString name() {
    if(DSPluginManager::New()->pluginInfo(this).isValid()){
      return DSPluginManager::New()->pluginInfo(this).name();
    }
    return QString();
  }

  virtual QGraphicsItem* nodeItem(Node*)=0;

};
}
#endif // DSPLUGININTERFACE_H