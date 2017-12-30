/**
 ** Copyright (C) 2011-2017 Tycho Softworks.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "main.hpp"
#include "find.hpp"
#include "ui_options.h"

static Ui::Options ui;

Options::Options(QTabWidget *tabs) :
QDialog()
{
    ui.setupUi(static_cast<QDialog *>(this));
    ui.checkFilename->setChecked(Main::sensitive());
    ui.checkSensitive->setChecked(Find::sensitive());

    tab = tabs->count();
    tabs->addTab(this, tr("Options"));

    connect(ui.checkFilename, &QCheckBox::stateChanged, this, &Options::setFilesSensitive);
    connect(ui.checkSensitive, &QCheckBox::stateChanged, this, &Options::setViewerSensitive);
}

Options::~Options()
{
}

void Options::setViewerSensitive(int state)
{
    if(state)
        Find::setSensitive(true);
    else
        Find::setSensitive(false);
}

void Options::setFilesSensitive(int state)
{
    if(state)
        Main::setSensitive(true);
    else
        Main::setSensitive(false);
}
