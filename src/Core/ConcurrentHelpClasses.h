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
#include <boost/shared_ptr.hpp>

struct DataColorSetted {
    DataColorSetted(const QColor& c) : m_color(c) {}
    typedef void result_type;
    void operator()(DataPtr d) {
        d->setColor(m_color);
    }
    const QColor& m_color;
};

struct PointerColorSetted {
    PointerColorSetted(const QColor& c) : m_color(c) {}
    typedef void result_type;
    void operator()(PointerPtr d) {
        d->setColor(m_color);
    }
    const QColor& m_color;
};

struct PointerNameVisibilitySetted {
    PointerNameVisibilitySetted(bool visibility) : m_visibility(visibility) {}
    typedef void result_type;
    void operator()(PointerPtr d) {
        d->setNameVisible(m_visibility);
    }
    bool m_visibility;
};

struct PointerValueVisibilitySetted {
    PointerValueVisibilitySetted(bool visibility) : m_visibility(visibility) {}
    typedef void result_type;
    void operator()(PointerPtr d) {
        d->setValueVisible(m_visibility);
    }
    bool m_visibility;
};

struct DataVisibilitySetted {
    DataVisibilitySetted(bool visibility) : m_visibility(visibility) {}
    typedef void result_type;
    void operator()(DataPtr d) {
        d->setVisible(m_visibility);
    }
    bool m_visibility;
};

struct PointerVisibilitySetted {
    PointerVisibilitySetted(bool visibility) : m_visibility(visibility) {}
    typedef void result_type;
    void operator()(PointerPtr d) {
        d->setVisible(m_visibility);
    }
    bool m_visibility;
};

#endif
