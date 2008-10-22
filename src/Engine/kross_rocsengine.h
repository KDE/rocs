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
