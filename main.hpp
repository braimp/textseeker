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

#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include <QApplication>
#include <QObject>
#include <QMainWindow>
#include <QActionGroup>
#include <QStringList>
#include <QDir>
#include <QSettings>

#include "compiler.hpp"
#include "config.hpp"
#include "index.hpp"
#include "toolbar.hpp"
#include "statusbar.hpp"
#include "options.hpp"

class Main final : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(Main)

public:
    Main(const QStringList& args, bool reset = false);
    ~Main();

    inline static Main *instance() {
        Q_ASSERT(Instance != nullptr);
        return Instance;
    }

    inline static bool sensitive() {
        return Sensitive;
    }

    inline static void setSensitive(bool flag) {
        Sensitive = flag;
    }

private:
    Statusbar *statusbar;
    Toolbar *toolbar;
    Options *options;
    QSettings settings;
    QStringList history;
    QDir dir;
    Index *index;

    void status(const QString& text);
    void error(const QString& text);
    void sync();

    static Main *Instance;
    static bool Sensitive;

public slots:
    void reloadMatches();
    void searchText();
    void enableFind();

private slots:
    void about();
    void clear();

    void openViewer();
    void openDocument();
    void openAt(const QModelIndex& item);
    void openContext(const QPoint& pos);

    void selectDir(int index);
    void changeDir();
    void changeSettings();
    void closeTab(int tab);
    void changeTab(int tab);
    void showContextMenu(const QPoint& pos);
};

/*!
 * \mainpage TextSeeker
 * This is a text search utility, originally written around 2011, originally
 * for Qt4 as part of Coastal-Qt.  It was later updated to compile on both
 * Qt4 and 5.  This current version is a new and only slightly refactored
 * version of the original, but for Qt5 only now.  There is further work that
 * can be done to improve this old application, including use of threaded
 * dispatch for the index loader.
 */

/*!
 * The main instance of the application.  This includes a Main singleton that
 * is both the main window and controller for the application, and the main()
 * function for argument parsing.
 * \file main.hpp
 */

#endif
