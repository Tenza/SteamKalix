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

#include "threadmanager.h"

/**
 * @brief ThreadManager::ThreadManager
 *      Initializes default parameters and creates the buyer and seller object/thread.
 * @param parent
 *      To use signals and slots, to set the parent of this class.
 * @date
 *      Created:  Filipe, 22 Mar 2014
 *      Modified: Filipe, 23 Apr 2014
 */
ThreadManager::ThreadManager(QObject *parent) :
    QObject(parent),
    thread_id(Helper::get_thread_id()),
    listing_id(0),
    pending_close(0)
{
//    buyer = new Buy();
//    buyer_thread = new QThread();

//    //Connect thread/object signals.
//    connect(buyer_thread, SIGNAL(started()), this, SLOT(buyer_started()));
//    connect(buyer_thread, SIGNAL(finished()), this, SLOT(buyer_finished()));
//    connect(buyer, SIGNAL(console(const QString&)), this, SIGNAL(console(const QString&)));

//    //Stoping thread mechanism
//    connect(buyer, SIGNAL(finished()), buyer_thread, SLOT(quit()));
//    connect(buyer_thread, SIGNAL(finished()), buyer, SLOT(deleteLater()));
//    connect(buyer_thread, SIGNAL(finished()), buyer_thread, SLOT(deleteLater()));

//    //Push object to thread and start thread
//    buyer->moveToThread(buyer_thread);
//    buyer_thread->start(QThread::TimeCriticalPriority);

//    seller = new Sell();
//    seller_thread = new QThread();

//    //Connect thread/object signals.
//    connect(seller_thread, SIGNAL(started()), this, SLOT(seller_started()));
//    connect(seller_thread, SIGNAL(finished()), this, SLOT(seller_finished()));
//    connect(seller, SIGNAL(console(const QString&)), this, SIGNAL(console(const QString&)));

//    //Stoping thread mechanism
//    connect(seller, SIGNAL(finished()), seller_thread, SLOT(quit()));
//    connect(seller_thread, SIGNAL(finished()), seller, SLOT(deleteLater()));
//    connect(seller_thread, SIGNAL(finished()), seller_thread, SLOT(deleteLater()));

//    //Push object to thread and start thread
//    seller->moveToThread(seller_thread);
//    seller_thread->start(QThread::HighPriority);
}

/**
 * @brief ThreadManager::~ThreadManager
 *      Terminates the buyer thread.
 *      Terminates the seller thread.
 *      Waits for pending listing threads.
 *      Emits signals to all active listing threads to terminate.
 *      Keeps the eventloop running to process the signals from stopping the mechanism.
 *      Removes all object and thread pointers.
 * @remarks
 *      It is not possible to use QThread::wait, because blocking the calling thread,
 *      will also block the eventloop which will block the stopping mechanism.
 * @date
 *      Created:  Filipe, 22 Mar 2014
 *      Modified: Filipe, 23 Apr 2014
 */
//ThreadManager::~ThreadManager()
//{
//    //Terminates buyer thread
//    buyer->thread_terminate();
//    while(buyer_thread->isRunning())
//    {
//        if(QCoreApplication::hasPendingEvents())
//        {
//            QCoreApplication::processEvents();
//        }
//    }

//    //Terminates seller thread
//    seller->thread_terminate();
//    while(seller_thread->isRunning())
//    {
//        if(QCoreApplication::hasPendingEvents())
//        {
//            QCoreApplication::processEvents();
//        }
//    }

//    //Waits for pending listing threads
//    while(pending_close != 0)
//    {
//        if(QCoreApplication::hasPendingEvents())
//        {
//            QCoreApplication::processEvents();
//        }
//    }

//    //Terminates active listing threads
//    for(int i = 0; i < listings.size(); i++)
//    {
//        emit listing_terminate(listings.at(i)->get_id());

//        while(listings_threads.at(i)->isRunning())
//        {
//            if(QCoreApplication::hasPendingEvents())
//            {
//                QCoreApplication::processEvents();
//            }
//        }

//        listings.removeAt(i);
//        listings_threads.removeAt(i);
//    }
//}

/**
 * @brief ThreadManager::listing_add
 *      Creates a new listing object and thread.
 * @param options
 *      Options set by the used in the UI, this be passed to the new thread.
 * @remarks Stoping mechanism
 *      User command.
 *      Changes variable to signal internal timer.
 *      Timer continues to run until all asynchronous replys from the network arrive.
 *      Timer emits finished() signal that will triger the quit() slot on the thread.
 *      On quit() the thread will emit a finished() signal that will triger the deleteLater() on the object and thread.
 *      From QThread: The signal finished can be connected to QObject::deleteLater(), to free objects in that thread.
 * @return
 *      ID of the thread to be added to the table on the UI.
 * @date
 *      Created:  Filipe, 22 Mar 2014
 *      Modified: Filipe, 17 Apr 2014
 */
//int ThreadManager::listing_add(const QVariantHash &options)
//{
//    output("Starting listing.", 2);

//    int id = listing_id++;

//    listings_threads.append(new QThread());
//    listings.append(new Listing(id, options));

//    Listing *listing = listings.at(listings.size() - 1);
//    QThread *listing_thread = listings_threads.at(listings_threads.size() - 1);

//    //Connect internal timer with controls
//    connect(this, SIGNAL(listing_terminate(const int&)), listing, SLOT(thread_terminate(const int&)));
//    connect(this, SIGNAL(listing_start(const int&)), listing, SLOT(timer_start(const int&)));
//    connect(this, SIGNAL(listing_stop(const int&)), listing, SLOT(timer_stop(const int&)));

//    //Connect thread/object signals.
//    connect(listing_thread, SIGNAL(started()), this, SLOT(listings_started()));
//    connect(listing, SIGNAL(console(const QString&)), this, SIGNAL(console(const QString&)));
//    connect(listing, SIGNAL(buy_item(const QVariantHash&)), buyer, SLOT(buy_item(const QVariantHash&)));
//    connect(listing, SIGNAL(sell_item(const QVariantHash&)), seller, SLOT(sell_item(const QVariantHash&)));
//    connect(buyer, SIGNAL(result(const int&, const int&, const bool&)), listing, SLOT(buy_result(const int&, const int&, const bool&)));
//    connect(seller, SIGNAL(result(const int&, const int&, const bool&)), listing, SLOT(sell_result(const int&, const int&, const bool&)));

//    //Stoping thread mechanism
//    connect(listing, SIGNAL(finished()), listing_thread, SLOT(quit()));
//    connect(listing_thread, SIGNAL(finished()), this, SLOT(listings_finished()));
//    connect(listing_thread, SIGNAL(finished()), listing, SLOT(deleteLater()));
//    connect(listing_thread, SIGNAL(finished()), listing_thread, SLOT(deleteLater()));

//    //Push object to thread and start thread
//    listing->moveToThread(listing_thread);
//    listing_thread->start();

//    //Start the timer inside the object that is inside the thread. (yo dawg)
//    emit listing_start(id);

//    return id;
//}

/**
 * @brief ThreadManager::listing_remove
 *      Signals the thread to destruct itself when is ready.
 *      Removes the pointers from the array.
 *      Increments pending_close, so that the class know how many threads,
 *      are waiting for proper termination. (This happens when the user removes a listing
 *      and then closes the program. With pending_close, the application will wait for proper termination)
 * @param id
 *      ID of the thread requested to terminate.
 * @date
 *      Created:  Filipe, 22 Mar 2014
 *      Modified: Filipe, 1 Apr 2014
 */
//void ThreadManager::listing_remove(const int &id)
//{
//    emit listing_terminate(id);

//    for(int i = 0; i < listings.size(); i++)
//    {
//        if(listings.at(i)->get_id() == id)
//        {
//            listings.removeAt(i);
//            listings_threads.removeAt(i);
//            pending_close++;
//            break;
//        }
//    }
//}

/**
 * @brief ThreadManager::buyer_finished
 *      This slot is received from the started() signal of the thread.
 * @date
 *      Created:  Filipe, 18 Apr 2014
 *      Modified: Filipe, 18 Apr 2014
 */
void ThreadManager::buyer_started()
{
    output("Buyer thread started.", 2);
}

/**
 * @brief ThreadManager::buyer_finished
 *      This slot is received from the finished() signal of the thread.
 * @date
 *      Created:  Filipe, 18 Apr 2014
 *      Modified: Filipe, 18 Apr 2014
 */
void ThreadManager::buyer_finished()
{
    output("Buyer thread finnished.", 2);
}

/**
 * @brief ThreadManager::seller_started
 *      This slot is received from the started() signal of the thread.
 * @date
 *      Created:  Filipe, 23 Apr 2014
 *      Modified: Filipe, 23 Apr 2014
 */
void ThreadManager::seller_started()
{
    output("Seller thread started.", 2);
}

/**
 * @brief ThreadManager::seller_finished
 *      This slot is received from the finished() signal of the thread.
 * @date
 *      Created:  Filipe, 23 Apr 2014
 *      Modified: Filipe, 23 Apr 2014
 */
void ThreadManager::seller_finished()
{
    output("Seller thread finnished.", 2);
}

/**
 * @brief ThreadManager::thread_start
 *      This slot is received from the started() signal of the thread.
 * @date
 *      Created:  Filipe, 27 Mar 2014
 *      Modified: Filipe, 27 Mar 2014
 */
void ThreadManager::listings_started()
{
    output("Listing thread started.", 2);
}

/**
 * @brief ThreadManager::thread_stop
 *      This slot is received from the finished() signal of the thread.
 *      The listing is completly removed, so we can decrease the pending_close, and allow to program
 *      to terminate properly, if that is the case.
 * @date
 *      Created:  Filipe, 27 Mar 2014
 *      Modified: Filipe, 27 Mar 2014
 */
void ThreadManager::listings_finished()
{
    pending_close--;
    output("Listing thread finnished.", 2);
}

/**
 * @brief ThreadManager::output
 *      Provides some abstraction to the signal console.
 *      This method is present on every class that needs an output.
 *      The parameters are custom on each class, but they all use the output namespace for processing.
 * @param message
 *      The message to print. This is obrigatory.
 * @param verbose
 *      The verbose level of this message. This is obrigatory.
 * @param ID's
 *      ID's associated with the message to be printed. Can be a string or int. Use -1 to ignore.
 * @param log
 *      Indicate that this message should be saved to persistent storage.
 * @param load_thread_id
 *      Defines that the thread_id should be calculated.
 *      Only add this if needed, this ID should be calculated at creation.
 * @remarks
 *      The verbose check is made here in order to filter all the unnessesary messages without building the QHash.
 * @date
 *      Created:  Filipe, 8 Apr 2014
 *      Modified: Filipe, 28 May 2014
 */
void ThreadManager::output(const QString &message, const int &verbose)
{
    if(Output::get_verbose() >= verbose)
    {
        QVariantHash data;
        data.insert("message", message);
        data.insert("verbose", verbose);
        data.insert("ID1", thread_id);

        QString print = Output::builder(data);

        if(!print.isEmpty())
        {
            emit console(print);
        }
    }
}
