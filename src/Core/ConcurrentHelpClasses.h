#ifndef CONCURRENTHELPCLASSES_H
#define CONCURRENTHELPCLASSES_H

#include "Data.h"
#include "Pointer.h"
#include "Group.h"

struct DataColorSetted {
  DataColorSetted(const QColor& c) : m_color(c){}
  typedef void result_type;
  void operator()(Data* &d){  d->setColor(m_color);  }
  const QColor& m_color;
};

struct PointerColorSetted {
  PointerColorSetted(const QColor& c) : m_color(c){}
  typedef void result_type;
  void operator()(Pointer* &d){ d->setColor(m_color); }
  const QColor& m_color;
};

struct DataDynamicPropertySetted {
  DataDynamicPropertySetted(const QString& property, const QVariant& value) : m_property(property), m_value(value){}
  typedef void result_type;
  void operator()(Data* &d){   d->addDynamicProperty(m_property, m_value);  }
  const QString& m_property;
  const QVariant& m_value;
};

struct PointerDynamicPropertySetted {
  PointerDynamicPropertySetted(const QString& property, const QVariant& value) : m_property(property), m_value(value){}
  typedef void result_type;
  void operator()(Pointer* &d){  d->addDynamicProperty(m_property, m_value);  }
  const QString& m_property;
  const QVariant& m_value;
};

struct DataDynamicPropertyUnSetted {
  DataDynamicPropertyUnSetted(const QString& property) : m_property(property) {}
  typedef void result_type;
  void operator()(Data* &d){ d->removeDynamicProperty(m_property);   }
  const QString& m_property;
};

struct PointerDynamicPropertyUnSetted {
  PointerDynamicPropertyUnSetted(const QString& property) : m_property(property){}
  typedef void result_type; 
  void operator()(Pointer* &d){ d->removeDynamicProperty(m_property); }
  const QString& m_property;
};

struct DataNameVisibilitySetted {
  DataNameVisibilitySetted(bool visibility) : m_visibility(visibility){}
  typedef void result_type; 
  void operator()(Data* &d){ d->setShowName(m_visibility); }
  bool m_visibility;
};

struct PointerNameVisibilitySetted {
  PointerNameVisibilitySetted(bool visibility) : m_visibility(visibility){}
  typedef void result_type; 
  void operator()(Pointer* &d){ d->hideName(m_visibility); }
  bool m_visibility;
};

struct DataValueVisibilitySetted {
  DataValueVisibilitySetted(bool visibility) : m_visibility(visibility){}
  typedef void result_type; 
  void operator()(Data* &d){ d->setShowValue(m_visibility); }
  bool m_visibility;
};

struct PointerValueVisibilitySetted {
  PointerValueVisibilitySetted(bool visibility) : m_visibility(visibility){}
  typedef void result_type; 
  void operator()(Pointer* &d){ d->hideValue(m_visibility); }
  bool m_visibility;
};

#endif