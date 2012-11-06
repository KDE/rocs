/*
    This file is part of Rocs.
    Copyright 2004-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef QTSCRIPTBACKEND_H
#define QTSCRIPTBACKEND_H

#include <QtScript>
#include <QScriptValue>
#include <QScriptString>
#include "RocsCoreExport.h"
#include "IncludeManager.h"

class QScriptEngineDebugger;
class Document;
class ToolsPluginInterface;
class QSignalSpy;

class  ROCSLIB_EXPORT QtScriptBackend : public QObject
{
    Q_OBJECT
public:
//     QtScriptBackend();
    QtScriptBackend(QObject* parent = 0);
    void setScript(const QString& s, Document *document);
    void loadFile(const QString& file);

    /**
     * Output the given string \p s as debug output.
     *
     * \param s the string to be print
     */
    void debug(const QString& s);

    /**
     * Output the given string \p s as program output.
     *
     * \param s the string to be print
     */
    void output(const QString& s);

    /**
     * Interrupt script execution. This method should be used from the scrpting interface.
     */
    void interrupt();

    QScriptEngine *engine() {
        return _engine;
    }

    /**
     * Include script from other script file.
     */
    void include(const QString&);

    /** return true if is evaluating a script or running a tool script. */
    bool isRunning();

    IncludeManager& includeManager() {
        return m_includeManager;
    }


signals:
    void sendOutput(const QString& s);
    void sendDebug(const QString& s);
    void scriptError();
    void engineCreated(QScriptEngine* e);
    void finished();

public slots:
    /**
     * Execute the preset script. This method also sets and afterwards pops the execution context.
     * Emits SIGNAL finished() when execution is finished.
     *
     * \return result from script engine
     */
    QString execute();
    void executeStep();
    void continueExecutionStep();


    /** abort script evaluation. In case of a too is running, stop will not stop tool. But the script resulting from tool will not be runned.
    */
    void stop();


private:
    void createGraphList();

    QString _script;
    Document *_document;
    QScriptEngine *_engine;
    QScriptEngineDebugger *_engineSteps;    // used for stepped execution

    IncludeManager m_includeManager;
    bool _runningTool;

};

#endif
