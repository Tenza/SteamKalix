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

#ifndef REPLYTIMEOUT_H
#define REPLYTIMEOUT_H

#include <QObject>
#include <QNetworkReply>
#include <QTimer>

#include "defines.h"

/**
 * @brief The ReplyTimeout class
 *      This class implements the basic functionality of a timeout system.
 *      This functionalty could be created with each request, but the purpose of
 *      having a class is to parent the QNetworkReply, in order to have
 *      automatic memory management when the function reply->deleteLater(); is called
 * @date
 *      Created:  Filipe, 1 Abr 2014
 *      Modified: Filipe, 1 Abr 2014
 */
class ReplyTimeout : public QObject
{
    Q_OBJECT

public_construct:
    explicit ReplyTimeout(QNetworkReply *new_reply, const int &timeout);

private_data_members:
    QNetworkReply *reply;

private slots:
    void timeout();

};

#endif // REPLYTIMEOUT_H
