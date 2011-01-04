#include "DataStructurePluginInterface.h"
#include "DataStructurePluginManager.h"

DataStructurePluginInterface::DataStructurePluginInterface ( const KComponentData& /*instance*/, QObject* parent ) 
: QObject ( parent )
{
}

DataStructurePluginInterface::~DataStructurePluginInterface()
{
}


QLayout* DataStructurePluginInterface::pointerExtraProperties ( Pointer* arg1, QWidget* arg2 ) const {
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    return 0;
}

QLayout* DataStructurePluginInterface::dataStructureExtraProperties ( DataStructure* arg1, QWidget* arg2 ) const {
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    return 0;
}

QLayout* DataStructurePluginInterface::dataExtraProperties ( Data* arg1, QWidget* arg2 ) const {
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    return 0;
}

QString DataStructurePluginInterface::name()
{
    if(DataStructurePluginManager::self()->pluginInfo(this).isValid()){
      return DataStructurePluginManager::self()->pluginInfo(this).name();
    }
    return QString();
  }
