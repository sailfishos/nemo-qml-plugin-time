/*
 * Copyright (C) 2012 Jolla Ltd.
 * Contact: Martin Jones <martin.jones@jollamobile.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Jolla Ltd. nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include <QtGlobal>

#include <QtQml>
#include <QQmlEngine>
#include <QQmlExtensionPlugin>

#include "nemowallclock.h"


class Q_DECL_EXPORT NemoTimePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Nemo.Time")

public:
    virtual ~NemoTimePlugin() { }

    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(engine);
        Q_ASSERT(uri == QLatin1String("Nemo.Time") || uri == QLatin1String("org.nemomobile.time"));
    }

    void registerTypes(const char *uri)
    {
        Q_ASSERT(uri == QLatin1String("Nemo.Time") || uri == QLatin1String("org.nemomobile.time"));
        if (uri == QLatin1String("org.nemomobile.time")) {
            qWarning() << "org.nemomobile.time is deprecated qml module name and subject to be removed. Please migrate to Nemo.Time";
        }
        qmlRegisterType<WallClock>(uri, 1, 0, "WallClock");
    }
};

#include "plugin.moc"
