/*
  Copyright (C) 2014 Jürg Billeter <j@bitron.ch>
  Copyright (C) 2012 Robin Burchell <robin+mer@viroteck.net>

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Nemo Mobile nor the names of its contributors
      may be used to endorse or promote products derived from this
      software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <QCoreApplication>
#include <QDebug>

#include <extendedcalendar.h>
#include <extendedstorage.h>
#include <notebook.h>
#include <icalformat.h>

void invalidUsage(const QString &app)
{
    qWarning("Usage: %s [-e | --export] <filename>", qPrintable(app));
    ::exit(1);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    bool import = true;
    QString filename;

    const QString app(QString::fromLatin1(argv[0]));

    for (int i = 1; i < argc; ++i) {
        const QString arg(QString::fromLatin1(argv[i]));
        if (arg.startsWith('-')) {
            if (!filename.isNull()) {
                invalidUsage(app);
            } else if (arg == QString::fromLatin1("-e") || arg == QString::fromLatin1("--export")) {
                import = false;
            } else {
                qWarning("%s: unknown option: '%s'", qPrintable(app), qPrintable(arg));
                invalidUsage(app);
            }
        } else {
            filename = arg;
        }
    }

    if (filename.isNull()) {
        qWarning("%s: filename must be specified", qPrintable(app));
        invalidUsage(app);
    }

    mKCal::ExtendedCalendar::Ptr calendar = mKCal::ExtendedCalendar::Ptr(new mKCal::ExtendedCalendar(KDateTime::Spec::LocalZone()));

    mKCal::ExtendedStorage::Ptr storage = calendar->defaultStorage(calendar);
    if (!storage->open()) {
        qWarning() << "calendar storage cannot be opened";
        ::exit(1);
    }

    mKCal::Notebook::Ptr notebook = storage->defaultNotebook();
    if (!storage->loadNotebookIncidences(notebook->uid())) {
        qWarning() << "loadNotebookIncidences failed";
        ::exit(1);
    }

    KCalCore::ICalFormat icf;

    if (import) {
        if (!icf.load((KCalCore::Calendar::Ptr) calendar, filename)) {
            qWarning() << "unable to load calendar from disk";
            ::exit(1);
        }
        if (!storage->save()) {
            qWarning() << "unable to save updated calendar";
            ::exit(1);
        }
    } else {
        if (!icf.save((KCalCore::Calendar::Ptr) calendar, filename)) {
            qWarning() << "unable to write calendar to disk";
            ::exit(1);
        }
    }

    return 0;
}

