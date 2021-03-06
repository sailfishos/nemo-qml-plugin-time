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

#ifndef WALLCLOCK_P_H
#define WALLCLOCK_P_H

#include <QDateTime>
#include <QPauseAnimation>
#include "nemowallclock.h"

class WallClockPrivate : public QPauseAnimation
{
public:
    WallClockPrivate(WallClock *wallClock);
    virtual ~WallClockPrivate();

    WallClock::UpdateFrequency updateFrequency() const { return m_updateFreq; }
    virtual void setUpdateFrequency(WallClock::UpdateFrequency);

    bool ready() const { return m_ready; }

    bool enabled() const { return m_enabled; }
    virtual void setEnabled(bool);

    bool suspended() const { return m_suspended; }

    virtual QDateTime time() const;
    virtual QString timezone() const;
    virtual QString timezoneAbbreviation() const;
    virtual int timezoneOffsetFromUtc() const;

protected:
    virtual void update();
    virtual void setSuspended(bool);
    void readyChanged();
    void timezoneChanged();
    void timezoneAbbreviationChanged();
    void timezoneOffsetFromUtcChanged();
    void timeChanged();
    void systemTimeChanged();
    WallClock *wallClock() { return q; }

private:
    virtual void updateCurrentTime(int);

    WallClock *q;
    WallClock::UpdateFrequency m_updateFreq;
    bool m_enabled;
    bool m_suspended;
    bool m_ready;
};

#endif // WALLCLOCK_P_H
