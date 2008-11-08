/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef KROSS_ROCSENGINE_H
#define KROSS_ROCSENGINE_H

#include <kross/core/action.h>
#include <kross/core/manager.h>

namespace Kross{
  class Action;
}

class GraphDocument;
class KTextBrowser;

class kross_rocsengine{
public:
  kross_rocsengine(KTextBrowser *debugArea);
  void setDocument( GraphDocument *gd);
  void setEngine(const QString& engine);
  void execute(const QString& script);

private:
   GraphDocument *_graphDocument;
  QString _engine;
  KTextBrowser *_debugArea;
};

#endif
