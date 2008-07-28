#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    KAboutData aboutData("Rocs",
                         0,
                         ki18n("Rocs 1"),
                         "1.0",
                         ki18n("Graph Theory Tool"),
                         KAboutData::License_GPL,
                         ki18n("(c) 2008"),
                         ki18n("Thanks to Ugo Sangiori"),
                         "",
                         "submit@bugs.kde.org");
 
  KCmdLineArgs::init( argc, argv, &aboutData );
  KApplication app;

  MainWindow window;
  window.show();

  return app.exec();
}

