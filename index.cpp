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

#include "index.hpp"

#include <QString>
#include <QFile>
#include <QDir>
#include <QDebug>

class Loader final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Loader)
public:
    Loader(QObject *parent, const QString basename, const QStringList& ext, const QString& match, bool casefilter);

private:
    QObject *parent;
    QString basename;
    QStringList ext;
    QString match;
    bool caseflag;

signals:
    void createIndex(Index *result);
};

Index::Index(QString basename, const QStringList& ext, const QString& match, bool caseFlag) :
QAbstractTableModel()
{
    auto thread = new QThread();
    moveToThread(thread);

    connect(this, &Index::finished, thread, &QThread::quit);
    connect(thread, &QThread::started, this, &Index::run);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    savedMatch = match;
    savedCase = caseFlag;

    rows = 0;

    if(basename.right(1) == "$")
        basename.chop(1);
    else if(basename.right(1) != "*")
        basename += "*";

    for(int pos = 0; pos < ext.size(); ++pos)
        filters << (basename + ext[pos]);

    if(ext.size() < 1) {
        if(basename.isEmpty())
            filters << "*";
        else
            filters << basename;
    }
}

bool Index::grep(const QString& path, const QString& match, bool casefilter)
{
    bool result = false;
    Qt::CaseSensitivity cs = Qt::CaseInsensitive;

    if(casefilter)
        cs = Qt::CaseSensitive;

    qDebug() << "Grepping" << match;
    if(match.isEmpty())
        return true;

    QString text;
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while(!file.atEnd()) {
            text = file.readLine();
            if(text.indexOf(match, 0, cs) > -1) {
                result = true;
                break;
            }
        }
    }

    return result;
}

void Index::run()
{
    scan("", savedMatch, savedCase);
    emit updateIndex(this);
    emit finished();
}

void Index::scan(const QString& path, const QString &match, bool casefilter)
{
    QString spec;
    QString name;
    if(path.isEmpty())
        spec = QDir::currentPath();
    else
        spec = QDir::currentPath() + QDir::separator() + path;

    qDebug() << "scanning" << spec << "for" << match;

    QDir dir(spec);
    if(!dir.exists())
        return;

    QDir::Filters filter = QDir::Files | QDir::NoDot;
    QDir::SortFlags sorting = QDir::SortFlags(QDir::Name);

    if(!casefilter)
        sorting |= QDir::IgnoreCase;

    auto dirs = dir.entryList(QDir::AllDirs, sorting);
    dir.setNameFilters(filters);
    auto list = dir.entryList(filter, sorting);

    for(int pos = 0; pos < dirs.size(); ++pos) {
        if(dirs[pos][0] == '.')
            continue;
        if(path.isEmpty())
            scan(dirs[pos], match, casefilter);
        else
            scan(path + QDir::separator() + dirs[pos], match, casefilter);
    }

    for(int pos = 0; pos < list.size(); ++pos) {
        if(list[pos][0] == '.')
            continue;
        if(path.isEmpty())
            name = list[pos];
        else
            name = path + QDir::separator() + list[pos];

        if(!match.isEmpty() && !grep(name, match, casefilter))
            continue;

        names << name;
        ++rows;
    }
}

Index::~Index() = default;

int Index::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return rows;
}

int Index::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant Index::data(const QModelIndex& index, int role) const
{
    int row = index.row();

    if(row >= rows || row < 0 || role != Qt::DisplayRole)
        return QVariant();

    return names[row];
}

QVariant Index::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(role);
    Q_UNUSED(orientation);
    return QVariant();
}

const QString Index::name(int row) const
{
    if(row >= rows)
        return QString();

    return names[row];
}

