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

class QtScriptBackendPrivate;
class QScriptEngineDebugger;
class Document;
class ToolsPluginInterface;
class QSignalSpy;

/**
 * \class QtScriptBackend
 *
 * This class provides the script backend for script executions. Usually, one script backend
 * is associated with one document.
 */
class ROCSLIB_EXPORT QtScriptBackend : public QObject
{
    Q_OBJECT
public:
    /**
     * Default constructor.
     */
    explicit QtScriptBackend(QObject* parent);

    /**
     * Default destructor.
     */
    ~QtScriptBackend();

    /**
     * Set the script \p script and the corresponding document \p document
     * to be executed on next run.
     */
    void setScript(const QString& script, Document *document);

    /**
     * \see QtScriptEngine::loadFile().
     */
    void loadFile(const QString& file);

    /**
     * Register \p qobject as global object under name \p name at engine for next simulation run.
     */
    QScriptValue registerGlobalObject(QObject *qobject, const QString &name);
    
    /**
     * Output the given string \p message as debug output.
     *
     * \param message the string to be print
     * \deprecated
     */
    void debug(const QString& message);

    /**
     * Output the given string \p message as program output.
     *
     * \param message the string to be print
     * \deprecated
     */
    void output(const QString& message);

    /**
     * Interrupt script execution. This method should be used from the scrpting interface.
     */
    void interrupt();

    /**
     * \return script engine
     */
    QScriptEngine *engine() const;

    /**
     * Include script from other script file.
     */
    void include(const QString& filePath);

    /**
     * Return true if engine is currently evaluating a script, otherwise false.
     */
    bool isRunning() const;

    /**
     * \return reference to the include manager dialog
     */
    IncludeManager& includeManager() const;

signals:
    void sendOutput(const QString &message);
    void sendDebug(const QString &message);
    void scriptError(const QString &message);
    void scriptInfo(const QString &message);
    void engineCreated(QScriptEngine* e);
    void finished();

public slots:
    /**
     * Execute the currently set script. This method also sets and afterwards pops the execution context.
     * Emits SIGNAL finished() when execution is finished.
     *
     * \return result from script engine
     */
    QString execute();

    /**
     * Execute the preset script up to a call of \see interrupt(). This method also sets the execution context.
     * Use \see continueExecutionStep() to continue after interrupting.
     */
    void executeStep();

    /**
     * Continue execution of interrupted script.
     */
    void continueExecutionStep();

    /**
     * Abort script evaluation.
     */
    void stop();

private:
    /**
     * \internal
     * d-pointer
     */
    const QScopedPointer<QtScriptBackendPrivate> d;
};

#endif
