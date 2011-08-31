/* 
    This file is part of Rocs.
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of 
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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