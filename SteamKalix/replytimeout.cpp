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

#include "replytimeout.h"

/**
 * @brief ReplyTimeout::ReplyTimeout
 *      This constructor initializes the local pointer with the reply
 *      and parents that same reply (QNetworkReply) in order to have automatic memory management.
 * @param new_reply
 *      The reply from the GET or POST.
 * @param timeout
 *      The timeout of the reply.
 * @date
 *      Created:  Filipe, 1 Abr 2014
 *      Modified: Filipe, 1 Abr 2014
 */
ReplyTimeout::ReplyTimeout(QNetworkReply* new_reply, const int &timeout) :
    QObject(new_reply),
    reply(new_reply)
{
    if(reply)
    {
        QTimer::singleShot(timeout, this, SLOT(timeout()));
    }
}

/**
 * @brief ReplyTimeout::timeout
 *      This funtion is called a single time by the singleshot timer.
 *      It closes the current connection.
 * @date
 *      Created:  Filipe, 1 Abr 2014
 *      Modified: Filipe, 1 Abr 2014
 */
void ReplyTimeout::timeout()
{
    if (reply->isRunning())
    {
        reply->close();
    }
}
