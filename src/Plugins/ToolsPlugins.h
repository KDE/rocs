
#ifndef TOOLSPLUGININTERFACE_H
#define TOOLSPLUGININTERFACE_H

#include <QtPlugin>

class QWidget;
class QString;
class QStringList;

class ToolsPluginInterface{
//   Q_OBJECT
public:
   virtual ~ToolsPluginInterface() {} 
   virtual QString name() const = 0; //Retorna nome que vai no menu
   virtual QString tooltip() const = 0; //Dica qnd mouse fica em cima.
   virtual QString about() const = 0; 
   virtual QString category() = 0; //Categoria para separar futuros plugins
   virtual QStringList authors() const = 0; 
public slots:
   virtual QString run(QWidget *parent=0) const = 0; //função principal, pode chamar dialogos e interagir com o usuário e devolve um script que será rodado.
};

Q_DECLARE_INTERFACE(ToolsPluginInterface,
                     "org.kde.kdeedu.ToolsPluginInterface/0.1")
                     
#endif
		