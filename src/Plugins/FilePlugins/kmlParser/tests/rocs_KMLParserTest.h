#ifndef rocs_KMLParserTEST_H
#define rocs_KMLParserTEST_H

#include <QtCore/QObject>

class rocs_KMLParserTest : public QObject
{
Q_OBJECT
private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();

    void someTest();
};

#endif // rocs_KMLParserTEST_H
