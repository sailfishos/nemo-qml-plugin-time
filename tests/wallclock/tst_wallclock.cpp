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

#include <QObject>
#include <QDateTime>
#include <QtTest>

#include <QtQml>
#include <QQmlEngine>
#include <QQmlComponent>


// Kind of tricky to test much of this without messing with system setttings.

class tst_WallClock: public QObject
{
    Q_OBJECT

private slots:
    void update();
};

void tst_WallClock::update()
{
    QQmlEngine engine;
    QQmlComponent comp(&engine, QUrl::fromLocalFile(TEST_DIRECTORY"/update.qml"));
    QObject *obj = comp.create();
    QVERIFY(obj);

    QSignalSpy updateSpy(obj, SIGNAL(timeChanged()));
    QSignalSpy readySpy(obj, SIGNAL(readyChanged()));
    QSignalSpy timezoneSpy(obj, SIGNAL(timezoneChanged()));
    QSignalSpy timezoneAbbreviationSpy(obj, SIGNAL(timezoneAbbreviationChanged()));
    QSignalSpy timezoneOffsetSpy(obj, SIGNAL(timezoneOffsetFromUtcChanged()));

    QVERIFY(!obj->property("ready").toBool());
    QVERIFY(obj->property("timezone").toString().isEmpty());
    QVERIFY(obj->property("timezoneAbbreviation").toString().isEmpty());

    // Expect an initial update
    QTRY_VERIFY(!readySpy.isEmpty());
    QTRY_VERIFY(!updateSpy.isEmpty());
    QTRY_VERIFY(!timezoneSpy.isEmpty());
    QTRY_VERIFY(!timezoneAbbreviationSpy.isEmpty());
    QTRY_VERIFY(!timezoneOffsetSpy.isEmpty());

    QVERIFY(obj->property("ready").toBool());
    QVERIFY(!obj->property("timezone").toString().isEmpty());
    QVERIFY(!obj->property("timezoneAbbreviation").toString().isEmpty());

    updateSpy.clear();
    timezoneSpy.clear();
    timezoneAbbreviationSpy.clear();
    timezoneOffsetSpy.clear();

    int currentSecs = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000;

    QDateTime time = obj->property("time").toDateTime();
    QVERIFY(time.isValid());
    int timeSecs = time.toMSecsSinceEpoch() / 1000;
    // giving some leeway
    QVERIFY(currentSecs == timeSecs || currentSecs == timeSecs + 1);

    int upCount = updateSpy.count();
    int tryCount = 10;
    while (tryCount) {
        if (updateSpy.count() == upCount + 1)
            break;
        --tryCount;
        QTest::qWait(100);
    }
    QVERIFY(updateSpy.count() == upCount + 1);

    currentSecs = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000;
    time = obj->property("time").toDateTime();
    QVERIFY(time.isValid());
    timeSecs = time.toMSecsSinceEpoch() / 1000;
    QVERIFY(currentSecs == timeSecs || currentSecs == timeSecs + 1);

    // would be nice to also test minute and day updates,
    // but who wants to wait 24hrs for a test to run.

    // disable and ensure updates don't happen.
    obj->setProperty("enabled", QVariant(false));

    upCount = updateSpy.count();
    QTest::qWait(1100);
    QVERIFY(updateSpy.count() == upCount);

    // enable and ensure updates happen.
    obj->setProperty("enabled", QVariant(true));

    // should get an update immediately.
    QVERIFY(updateSpy.count() == upCount + 1);

    // and another within a second
    QTest::qWait(1100);
    QVERIFY(updateSpy.count() > upCount + 1);
}


#include "tst_wallclock.moc"
QTEST_MAIN(tst_WallClock)
