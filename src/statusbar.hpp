/*
 * Copyright (C) 2017 Tycho Softworks.
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

#ifndef STATUSBAR_HPP_
#define STATUSBAR_HPP_

#include <QObject>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QShortcut>

#include "compiler.hpp"

class Statusbar final : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(Statusbar)

public:
    Statusbar(QWidget *parent, QStatusBar *bar, bool showDisabled = false);

    void disableSearch();
    void enableSearch();
    void enableSettings();
    void disableSettings();
    void enableHome();
    void disableHome();
    void clearIndicators();

    static inline Statusbar *instance() {
        return Instance;
    }

private:
    QShortcut *f3Key, *f6Key, *f8Key, *findKey, *closeKey;
    unsigned visibleIndicators;
    bool visibleDisabled;
    static Statusbar *Instance;
};

/*!
 * Status bar manager.  The status bar is at the bottom of the main window.
 * This is used both to show error messages/notices, and to manage the visual
 * indicators for shortcut function keys, which are shown and enabled based on
 * the state of the main tab view.  This also provides all global application
 * shortcuts.
 * \file statusbar.hpp
 */

#endif

