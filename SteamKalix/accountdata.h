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

#ifndef ACCOUNTDATA_H
#define ACCOUNTDATA_H

#include <QVector>
#include <QString>
#include <QMutex>

/**
 * @brief The AccountData namespace
 *      This namespace is used to store and manage the variables for each account.
 *      This gives access to all classes about the information aquiered during the login.
 * @date
 *      Created:  Filipe, 26 Apr 2014
 *      Modified: Filipe, 26 Apr 2014
 */
namespace AccountData
{
    void add_account(const QString &username,
                     const QString &communutiy_id,
                     const QString &profile_url,
                     const QString &email,
                     const QString &steam_id,
                     const QString &steam_id64,
                     const QString &avatar_url,
                     const int &wallet_balance);

    void set_communutiy_id(const QString &username, const QString &communutiy_id);
    QString get_communutiy_id(const QString &username);

    void set_profile_url(const QString &username, const QString &profile_url);
    QString get_profile_url(const QString &username);

    void set_email(const QString &username, const QString &email);
    QString get_email(const QString &username);

    void set_steam_id(const QString &username, const QString &steam_id);
    QString get_steam_id(const QString &username);

    void set_steam_id64(const QString &username, const QString &steam_id64);
    QString get_steam_id64(const QString &username);

    void set_avatar_url(const QString &username, const QString &avatar_url);
    QString get_avatar_url(const QString &username);

    void set_wallet_balance(const QString &username, const int &wallet_balance);
    int get_wallet_balance(const QString &username);

}

#endif // ACCOUNTDATA_H
