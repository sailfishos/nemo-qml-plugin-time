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

#include <QtDBus/QDBusReply>
#include <QQmlEngine>
#include <qqml.h>
#include <functional>
#include <timed-qt5/interface>
#include <timed-qt5/wallclock>
#include "nemowallclock_p.h"

#include <mce/dbus-names.h>
#include <mce/mode-names.h>

namespace {

// copied from statefs-providers common headers, it is better to move
// this functionality to the qtaround library and use it directly
template <typename OnValue, typename T>
bool callback_or_error(QDBusPendingReply<T> const &reply, OnValue on_value)
{
    if (reply.isError()) {
        auto err = reply.error();
        qWarning() << "D-Bus request error " << err.name()
                   << ": " << err.message();
        return false;
    }

    on_value(reply.value());
    return true;
}

template <typename T, typename OnValue>
void async(QObject *parent, QDBusPendingCallWatcher *watcher, OnValue on_value)
{
    parent->connect(watcher, &QDBusPendingCallWatcher::finished
                  , [on_value](QDBusPendingCallWatcher *w) {
                        QDBusPendingReply<T> reply = *w;
                        callback_or_error(reply, on_value);
                        w->deleteLater();
                    });
}

template <typename T, typename OnValue>
void async(QObject *parent, QDBusPendingCall const &call, OnValue &&on_value)
{
    auto watcher = new QDBusPendingCallWatcher(call, parent);
    async<T>(parent, watcher, std::forward<OnValue>(on_value));
}

} // anon namespace

class WallClockPrivateMeego : public WallClockPrivate
{
    Q_OBJECT
public:
    WallClockPrivateMeego(WallClock *);
    ~WallClockPrivateMeego();

    virtual QString timezone() const;
    virtual QString timezoneAbbreviation() const;

private slots:
    void settingsChanged(const Maemo::Timed::WallClock::Info &info, bool time_changed);
    void onDisplayStatusChanged(const QString &status);

private:
    Maemo::Timed::WallClock::Info info;
};

WallClockPrivate *nemoCreateWallClockPrivate(WallClock *wc)
{
    return new WallClockPrivateMeego(wc);
}

WallClockPrivateMeego::WallClockPrivateMeego(WallClock *wc)
    : WallClockPrivate(wc)
{
    using Maemo::Timed::WallClock::Info;
    Maemo::Timed::Interface ifc;
    ifc.settings_changed_connect(this, SLOT(settingsChanged(const Maemo::Timed::WallClock::Info &, bool)));

    auto onInfo = [this](Info const &res) {
        info = res;
        if (!QDBusConnection::systemBus().connect
            (MCE_SERVICE, MCE_SIGNAL_PATH, MCE_SIGNAL_IF, MCE_DISPLAY_SIG,
             this, SLOT(onDisplayStatusChanged(QString))))
            qWarning() << "Can't connect to mce";

        QDBusPendingCall call = QDBusConnection::systemBus().asyncCall
        (QDBusMessage::createMethodCall
         (MCE_SERVICE, MCE_REQUEST_PATH, MCE_REQUEST_IF, MCE_DISPLAY_STATUS_GET));
        using namespace std::placeholders;
        async<QString>(this, call,
                       std::bind(&WallClockPrivateMeego::onDisplayStatusChanged,
                                 this, _1));
    };
    async<Info>(this, ifc.get_wall_clock_info_async(), onInfo);
}

WallClockPrivateMeego::~WallClockPrivateMeego()
{
    Maemo::Timed::Interface ifc;
    ifc.settings_changed_disconnect(this, SLOT(settingsChanged(const Maemo::Timed::WallClock::Info &, bool)));
}

void WallClockPrivateMeego::onDisplayStatusChanged(const QString &status)
{
    setSuspended(status == MCE_DISPLAY_OFF_STRING);
}

QString WallClockPrivateMeego::timezone() const
{
    return info.humanReadableTz();
}

QString WallClockPrivateMeego::timezoneAbbreviation() const
{
    return info.tzAbbreviation();
}

void WallClockPrivateMeego::settingsChanged(const Maemo::Timed::WallClock::Info &newInfo, bool time_changed)
{
    bool tzChange = newInfo.humanReadableTz() != info.humanReadableTz();
    bool tzaChange = newInfo.tzAbbreviation() != info.tzAbbreviation();
    bool hourModeChange = newInfo.flagFormat24() != info.flagFormat24();

    info = newInfo;

    if (tzChange || tzaChange) {
        QQmlEngine *engine = QtQml::qmlEngine(wallClock());
        // Notify qml engine timezone before emitting signals, so handler code has it already up to date
        if (engine) {
            engine->evaluate("Date.timeZoneUpdated()");
        }
    }

    if (tzChange)
        timezoneChanged();
    if (tzaChange)
        timezoneAbbreviationChanged();
    if (time_changed)
        systemTimeChanged();
    if (tzChange || tzaChange || time_changed || hourModeChange)
        timeChanged();
}

#include "nemowallclock_meego.moc"
