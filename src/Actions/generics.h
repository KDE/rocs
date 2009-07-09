#ifndef GENERICS_H
#define GENERICS_H

#include "../GraphicsItem/graphicsitem_Node.h"

bool leftLessThan(NodeItem *n1, NodeItem *n2){        return n1->x() < n2->x(); }
bool rightLessThan(NodeItem *n1, NodeItem *n2){      return n1->x() > n2->x(); }
bool topLessThan(NodeItem *n1, NodeItem *n2){        return n1->y() < n2->y(); }
bool bottomLessThan(NodeItem *n1, NodeItem *n2){ return n1->y() > n2->y(); }

#endif