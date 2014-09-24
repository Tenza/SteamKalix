/*
 * SteamKalix
 * Copyright (C) 2014 Filipe Carvalho
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

#ifndef LISTINGSMANAGER_H
#define LISTINGSMANAGER_H

#include <QObject>
#include <QTimer>

#include "defines.h"
#include "output.h"
#include "helper.h"
#include "accountdata.h"
#include "networkmanager.h"

class ListingsManager : public QObject
{
    Q_OBJECT

public_construct:
    explicit ListingsManager(QObject *parent = 0);

public_methods:
    void thread_terminate();

private_methods:

private_members:
    bool terminate;
    QString thread_id;

private_data_members:
    QTimer *timer;
    NetworkManager *network_manager;  

public slots:

private slots:
    void work();
    void process_request(QNetworkReply *reply);

signals:
    void console(const QString &message);
    void finished();

};

#endif // LISTINGSMANAGER_H
