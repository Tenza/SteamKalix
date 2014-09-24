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

#include "listingsmanager.h"

ListingsManager::ListingsManager(QObject *parent) :
    QObject(parent),
    terminate(false),
    thread_id(""),
    timer(new QTimer(this)),
    network_manager(new NetworkManager(this))
{
    connect(timer, SIGNAL(timeout()), this, SLOT(work()));
    connect(network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(process_request(QNetworkReply*)));

}

void ListingsManager::work()
{

}

void ListingsManager::process_request(QNetworkReply *reply)
{
    reply->deleteLater();
}
