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

#ifndef __INDEX_HPP__
#define __INDEX_HPP__

#include <QAbstractTableModel>

#include "compiler.hpp"

class Index final : public QAbstractTableModel
{
    Q_OBJECT
    Q_DISABLE_COPY(Index)

public:
    Index(QObject *parent, QString basename, QStringList ext, QString match, bool casefilter);
    ~Index();

    void scan(QString path, QString match, bool casefilter);
    bool grep(QString& path, QString& match, bool casefilter);

    const QString name(int row) const;

private:
    QStringList filters;
    QStringList names;
    int rows;

    int rowCount(const QModelIndex& parent) const final;
    int columnCount(const QModelIndex& parent) const final;
    QVariant data(const QModelIndex &index, int role) const final;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const final;
};


/*!
 * Data model for index of search results.
 * \file index.hpp
 */

#endif

