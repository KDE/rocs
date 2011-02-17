#ifndef LISTNODEITEM_H
#define LISTNODEITEM_H

#include "DataItem.h"

class Data;

class NodeItem : public DataItem{
  Q_OBJECT
  public:
    NodeItem(Data *n);
    virtual ~NodeItem();
};

#endif
