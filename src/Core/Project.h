/*
    This file is part of Rocs.
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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


#ifndef PROJECT_H
#define PROJECT_H

#include <boost/scoped_ptr.hpp>

class ProjectPrivate;
class QString;

/**
 * \class Project
 * \brief A "Project" object represents the compilation of file pointers that form a Rocs project.
 *
 * Use this class to
 */
class Project
{

public:
    Project();
    virtual ~Project();

    void setName(QString name);
    QString name() const;

private:
    boost::scoped_ptr<ProjectPrivate> d;
};

#endif // PROJECT_H
