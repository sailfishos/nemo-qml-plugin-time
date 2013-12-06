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
#include <timed-qt5/interface>
#include <timed-qt5/wallclock>
#include "nemowallclock_p.h"

#include <mce/dbus-names.h>
#include <mce/mode-names.h>

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
    Maemo::Timed::Interface ifc;
    QDBusReply<Maemo::Timed::WallClock::Info> result = ifc.get_wall_clock_info_sync();
    if (result.isValid())
        info = result.value();
    else
        qWarning() << "Failed to connect to timed";
    ifc.settings_changed_connect(this, SLOT(settingsChanged(const Maemo::Timed::WallClock::Info &, bool)));
    if (!QDBusConnection::systemBus().connect(MCE_SERVICE,
                                         MCE_SIGNAL_PATH,
                                         MCE_SIGNAL_IF,
                                         MCE_DISPLAY_SIG,
                                         this,
                                         SLOT(onDisplayStatusChanged(QString))))
        qWarning() << "Can't connect to mce";

    QDBusReply<QString> reply = QDBusConnection::systemBus().call(QDBusMessage::createMethodCall(MCE_SERVICE,
                                                                                                 MCE_REQUEST_PATH,
                                                                                                 MCE_REQUEST_IF,
                                                                                                 MCE_DISPLAY_STATUS_GET));
    if (reply.isValid())
        onDisplayStatusChanged(reply.value());
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

    if (tzChange)
        timezoneChanged();
    if (tzaChange)
        timezoneAbbreviationChanged();
    if (time_changed)
        systemTimeChanged();
    if (tzaChange || tzaChange || time_changed || hourModeChange)
        timeChanged();
}

#include "nemowallclock_meego.moc"
