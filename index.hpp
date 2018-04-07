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

#ifndef INDEX_HPP_
#define INDEX_HPP_

#include <QAbstractTableModel>
#include <QThread>

#include "compiler.hpp"

class Index final : public QAbstractTableModel
{
    Q_OBJECT
    Q_DISABLE_COPY(Index)

public:
    Index(QString basename, const QStringList &ext, const QString &match, bool caseFlag);
    ~Index() final;

    void start() {
        thread()->start();
    }

    void scan(const QString &path, const QString& match, bool casefilter);
    bool grep(const QString &path, const QString &match, bool casefilter);

    const QString name(int row) const;

private:
    QString savedMatch;
    bool savedCase;
    QStringList filters;
    QStringList names;
    int rows;

    int rowCount(const QModelIndex& parent) const final;
    int columnCount(const QModelIndex& parent) const final;
    QVariant data(const QModelIndex &index, int role) const final;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const final;

signals:
    void updateIndex(Index *result);
    void finished();

private slots:
    void run();
};


/*!
 * Data model for index of search results.
 * \file index.hpp
 */

#endif

