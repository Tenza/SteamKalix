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

#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QCoreApplication>

#include "defines.h"
//#include "buy.h"
//#include "sell.h"
//#include "listing.h"
#include "helper.h"
#include "output.h"

/**
 * @brief The ThreadManager class
 *      This class is responsible for creation, termination and management of all the threads of the application.
 * @remarks Path of execution
 *      Create a buyer and selling threads.
 *      Create listing threads on demand.
 *      Terminate all threads on demand.
 *      On application termination, this handle all active and pending threads.
 *      Each thread will then handle their current network requests for proper termination.
 * @date
 *      Created:  Filipe, 22 Mar 2014
 *      Modified: Filipe, 18 Apr 2014
 */
class ThreadManager : public QObject
{
    Q_OBJECT

public_construct:
    explicit ThreadManager(QObject *parent = 0);
//    ~ThreadManager();

public_methods:
//    int listing_add(const QVariantHash &options);
//    void listing_remove(const int &id);

private_methods:
    void output(const QString &message, const int &verbose);

private_members:
    QString thread_id;

    int listing_id;
    int pending_close;

private_data_members:
//    Buy* buyer;
//    QThread* buyer_thread;

//    Sell* seller;
//    QThread* seller_thread;

//    QList<Listing*> listings;
//    QList<QThread*> listings_threads;

private slots:
    void buyer_started();
    void buyer_finished();

    void seller_started();
    void seller_finished();

    void listings_started();
    void listings_finished();

signals:
    void lock_listings();
    void console(const QString &message);

    void listing_start(const int &id);
    void listing_stop(const int &id);
    void listing_terminate(const int &id);

};

#endif // THREADMANAGER_H
