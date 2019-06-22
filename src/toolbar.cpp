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
#include "ui_toolbar.h"

#include <QtWidgets>

static Ui::Toolbar ui;

Toolbar *Toolbar::Instance = nullptr;

Toolbar::Toolbar(QWidget *parent, QToolBar *bar, QMenuBar *mp) :
QWidget(parent)
{
    Q_ASSERT(Instance == nullptr);
    Instance = this;

    menu = mp;
    moving = false;

    bar->addWidget(this);
    bar->installEventFilter(this);

    ui.setupUi(this);
    ui.searchText->setAttribute(Qt::WA_MacShowFocusRect, false);

    Main *main = Main::instance();
    connect(ui.searchText, &QLineEdit::returnPressed, main, &Main::reloadMatches);
}

void Toolbar::clearSearch()
{
    ui.searchText->setText("");
    ui.searchText->setFocus();
}

void Toolbar::enableSearch()
{
    ui.searchText->setEnabled(true);
    clearSearch();
}

void Toolbar::homeOnSearch()
{
    ui.searchText->setPlaceholderText("");
    enableSearch();
}

void Toolbar::disableSearch()
{
    auto text = ui.searchText->text();
    if(text.count() > 0)
        ui.searchText->setPlaceholderText(text);
    ui.searchText->clear();
    ui.searchText->setEnabled(false);
}

QString Toolbar::searching()
{
    return ui.searchText->text();
}

void Toolbar::onRefocus()
{
    if(ui.searchText->isEnabled())
        ui.searchText->setFocus();
}

bool Toolbar::eventFilter(QObject *object, QEvent *event)
{
    auto mouse = static_cast<QMouseEvent *>(event);
    auto window = static_cast<QWidget *>(Main::instance());
    QPoint pos;

    switch(event->type()) {
    case QEvent::MouseMove:
        if(!moving)
            break;

        pos = window->pos();
        pos.rx() += mouse->globalPos().x() - mpos.x();
        pos.ry() += mouse->globalPos().y() - mpos.y();
        window->move(pos);
        mpos = mouse->globalPos();
        event->accept();
        return true;

    case QEvent::MouseButtonPress:
        moving = false;
#if defined(Q_OS_WIN)
        if(menu && menu->isVisible())
            break;
#endif
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
        if(mouse->button() != Qt::LeftButton)
            break;

        moving = true;
        mpos = mouse->globalPos();
        event->accept();
        return true;
#else
        break;      // use native window manager on linux, bsd, etc...
#endif
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseTrackingChange:
        if(!moving)
            break;

        moving = false;
        event->accept();
        return true;
    default:
        break;
    }
    return QObject::eventFilter(object, event);
}

