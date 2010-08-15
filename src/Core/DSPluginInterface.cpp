#include "DSPluginInterface.h"
#include "DSPluginManager.h"

using namespace Rocs;

DSPluginInterface::DSPluginInterface ( const KComponentData& /*instance*/, QObject* parent ) : QObject ( parent )
{

}

DSPluginInterface::~DSPluginInterface()
{

}


QLayout* Rocs::DSPluginInterface::pointerExtraProperties ( Pointer* arg1, QWidget* arg2 ) const {
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    return 0;
}

QLayout* Rocs::DSPluginInterface::dataTypeExtraProperties ( DataType* arg1, QWidget* arg2 ) const {
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    return 0;
}

QLayout* Rocs::DSPluginInterface::datumExtraProperties ( Datum* arg1, QWidget* arg2 ) const {
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    return 0;
}

QString DSPluginInterface::name()
{
    if(DSPluginManager::instance()->pluginInfo(this).isValid()){
      return DSPluginManager::instance()->pluginInfo(this).name();
    }
    return QString();
  }
