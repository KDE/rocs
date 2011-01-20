 #ifndef GROUP_H
#define GROUP_H

#include <QObject>


class Group : QObject{
    Q_OBJECT
public:
  Group(QObject *parent = 0);
  void setName(const QString& name);
  const QString& name() const;
  
private:
  QString _name;
};

#endif