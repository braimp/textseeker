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

#include "viewer.hpp"
#include "toolbar.hpp"
#include <QDebug>

Viewer::Viewer(QTabWidget *tabs, QString& path) :
QTextEdit()
{
    QString text, temp;
    int pos;
    QFile file(path);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while(!file.atEnd()) {
            temp = file.readLine();
            while((pos = temp.indexOf("\002")) > -1) {
                temp.remove(pos, 1);
            }
            while((pos = temp.indexOf("\003")) > -1) {
                temp.remove(pos, 3);
            }
            while((pos = temp.indexOf("\010")) > -1) {
                temp.remove(pos, 1);
            }
            while((pos = temp.indexOf("\017")) > -1) {
                temp.remove(pos, 1);
            }
            text += temp;
        }
    }

    findText = "";
    findBy = 0;
    find = nullptr;

    setReadOnly(true);
    setEnabled(true);
    setText(text);

    tab = tabs->count();
    tabs->addTab(this, path);
    tabs->setCurrentIndex(tab);
    tabs->setTabsClosable(true);
}

void Viewer::activateFind()
{
    if(find)
        return;

    find = new Find(this, findText);
    connect(find, &Find::updateSearch, this, &Viewer::onUpdateSearch);
    connect(find, &QObject::destroyed, this, &Viewer::closeFind);
}

void Viewer::findNextText()
{
    if(findText.isEmpty())
        activateFind();
    else
        searchText();
}

void Viewer::closeFind(QObject *object)
{
    Q_UNUSED(object);
    find = nullptr;
}

void Viewer::setDirection(bool forward, bool sensitivity)
{
    if(forward)
        findBy = 0;
    else
        findBy = QTextDocument::FindBackward;

    if(sensitivity)
        findBy |= QTextDocument::FindCaseSensitively;
}

void Viewer::searchText()
{
    if(!findText.isEmpty()) {
        qDebug() << "Searching for" << findText;
        if(!QTextEdit::find(findText, findBy))
            findText = "";
    }
}

void Viewer::onUpdateSearch(const QString& search)
{
    findText = search;
    searchText();    
}

