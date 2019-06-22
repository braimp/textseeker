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

#ifndef TOOLBAR_HPP_
#define TOOLBAR_HPP_

#include <QObject>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>

#include "compiler.hpp"

class Toolbar final : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(Toolbar)

public:
    Toolbar(QWidget *parent, QToolBar *bar, QMenuBar *mp = nullptr);

    void clearSearch();
    void disableSearch();
    void enableSearch();
    void homeOnSearch();
    QString searching();

    static inline Toolbar *instance() {
        return Instance;
    }

private:
    QPoint mpos;
    bool moving;
    QMenuBar *menu;

    static Toolbar *Instance;

    bool eventFilter(QObject *obj, QEvent *evt) final;

public slots:
    void onRefocus();
};

/*!
 * Toolbar manager.  The Toolbar is at the top of the main window.  This is
 * used both for the toolbar application buttons such as reload, quit, about,
 * and to manage the global search input box.
 * \file toolbar.hpp
 */

#endif
