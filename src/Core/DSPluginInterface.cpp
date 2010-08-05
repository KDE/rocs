#include "DSPluginInterface.h"



using namespace Rocs;

DSPluginInterface::DSPluginInterface ( const KComponentData& /*instance*/, QObject* parent ) : QObject ( parent )
{

}

DSPluginInterface::~DSPluginInterface()
{

}


QLayout* Rocs::DSPluginInterface::edgeExtraProperties ( Edge* arg1, QWidget* arg2 ) const {
    return 0;
}

QLayout* Rocs::DSPluginInterface::graphExtraProperties ( Graph* arg1, QWidget* arg2 ) const {
  return 0;
}

QLayout* Rocs::DSPluginInterface::nodeExtraProperties ( Node* arg1, QWidget* arg2 ) const {
  return 0;
}
