/*
 * Copyright (C) 2011-2017 Tycho Softworks.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VIEWER_HPP_
#define VIEWER_HPP_

#include <QTextEdit>
#include <QKeyEvent>
#include <QTableWidget>

#include "compiler.hpp"
#include "find.hpp"

class Viewer final : public QTextEdit
{
    Q_DISABLE_COPY(Viewer)

public:
    Viewer(QTabWidget *tabs, QString &path);

    inline const QString currentSearch() const {
        return findText;
    }

    inline void release() {
        if(find) {
            find->close();
        }
    }

    void setDirection(bool forward, bool sensitivity);

    inline static bool timestamps() {
        return Timestamps;
    }

    inline static void setTimestamps(bool flag) {
        Timestamps = flag;
    }

private:
    QTextDocument::FindFlags findBy;
    QString findText;
    Find *find;
    int tab;

    void searchText();

    static bool Timestamps;

public slots:
    void activateFind();
    void findNextText();
    
private slots:
    void onUpdateSearch(const QString& search);
    void closeFind(QObject *object);
};

/*!
 * A searchable text view of the currently selected text file.
 * \file viewer.hpp
 */

#endif


