#ifndef KROSS_ROCSENGINE_H
#define KROSS_ROCSENGINE_H

#include <kross/core/action.h>

namespace Kross{
  class Action;
}
namespace libgraph{
  class GraphDocument;
}

class kross_rocsengine{
public:
  kross_rocsengine();
  void setDocument(libgraph::GraphDocument *gd);
  void setEngine(const QString& engine);
  void execute(const QString& script);

private:
  void configureForJavaScript(Kross::Action& action);
  void configureForPython(Kross::Action& action);
  void configureForRuby(Kross::Action& action);


  libgraph::GraphDocument *_graphDocument;
  QString _engine;
};

#endif