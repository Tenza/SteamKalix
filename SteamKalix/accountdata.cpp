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

#include "accountdata.h"

/**
 *@brief Anonymous namespace
 *      This namespace is used as a "private section".
 *      It is anonymous and therefore can only accessed within file scope.
 *@date
 *      Created:  Filipe, 26 Apr 2014
 *      Modified: Filipe, 23 May 2014
 */
namespace
{
    QMutex mutex;
    QVector<QString> usernames;
    QVector<QString> communutiy_ids;
    QVector<QString> profile_urls;
    QVector<QString> emails;
    QVector<QString> steam_ids;
    QVector<QString> steam_ids64;
    QVector<QString> avatar_urls;
    QVector<int> wallet_balances;

    int get_index(const QString &username)
    {
        int index = 0; //Returns the first if not found.
        for(int i = 0; i < usernames.size(); i++)
        {
            if(username == usernames.at(i))
            {
                index = i;
                break;
            }
        }

        return index;
    }
}

/**
 * @brief AccountData::add_account
 *      Adds a new account to the namespace.
 * @param username
 * @param communutiy_id
 * @param profile_url
 * @param wallet_balance
 * @param email
 * @param steam_id
 * @param steam_id64
 * @param avatar_url
 * @remarks
 *      This function should be thread-safe.
 * @date
 *      Created:  Filipe, 26 Apr 2014
 *      Modified: Filipe, 23 May 2014
 */
void AccountData::add_account(const QString &username,
                              const QString &communutiy_id,
                              const QString &profile_url,                              
                              const QString &email,
                              const QString &steam_id,
                              const QString &steam_id64,
                              const QString &avatar_url,
                              const int &wallet_balance)
{
    mutex.lock();
    usernames.append(username);
    communutiy_ids.append(communutiy_id);
    profile_urls.append(profile_url);
    emails.append(email);
    steam_ids.append(steam_id);
    steam_ids64.append(steam_id64);
    avatar_urls.append(avatar_url);
    wallet_balances.append(wallet_balance);
    mutex.unlock();
}

/**
 * @brief AccountData::Setters/Getters
 *      The following functions are used to retrive or edit the information regarding a specific account.
 * @param username
 *      The username of the account to be updated.
 * @param X
 *      The new value in case of a 'set' function.
 * @remarks
 *      The following functions should be thread-safe.
 * @date
 *      Created:  Filipe, 26 Apr 2014
 *      Modified: Filipe, 26 Apr 2014
 */
void AccountData::set_communutiy_id(const QString &username, const QString &communutiy_id)
{
    if(!communutiy_ids.isEmpty())
    {
        mutex.lock();
        communutiy_ids[get_index(username)] = communutiy_id;
        mutex.unlock();
    }
}

QString AccountData::get_communutiy_id(const QString &username)
{
    if(!communutiy_ids.isEmpty())
    {
        return communutiy_ids[get_index(username)];
    }
    else
    {
        return "";
    }
}

void AccountData::set_profile_url(const QString &username, const QString &profile_url)
{
    if(!profile_urls.isEmpty())
    {
        mutex.lock();
        profile_urls[get_index(username)] = profile_url;
        mutex.unlock();
    }
}

QString AccountData::get_profile_url(const QString &username)
{
    if(!profile_urls.isEmpty())
    {
        return profile_urls[get_index(username)];
    }
    else
    {
        return "";
    }
}

void AccountData::set_email(const QString &username, const QString &email)
{
    if(!emails.isEmpty())
    {
        mutex.lock();
        emails[get_index(username)] = email;
        mutex.unlock();
    }
}

QString AccountData::get_email(const QString &username)
{
    if(!emails.isEmpty())
    {
        return emails[get_index(username)];
    }
    else
    {
        return "";
    }
}

void AccountData::set_steam_id(const QString &username, const QString &steam_id)
{
    if(!steam_ids.isEmpty())
    {
        mutex.lock();
        steam_ids[get_index(username)] = steam_id;
        mutex.unlock();
    }
}

QString AccountData::get_steam_id(const QString &username)
{
    if(!steam_ids.isEmpty())
    {
        return steam_ids[get_index(username)];
    }
    else
    {
        return "";
    }
}

void AccountData::set_steam_id64(const QString &username, const QString &steam_id64)
{
    if(!steam_ids64.isEmpty())
    {
        mutex.lock();
        steam_ids64[get_index(username)] = steam_id64;
        mutex.unlock();
    }
}

QString AccountData::get_steam_id64(const QString &username)
{
    if(!steam_ids64.isEmpty())
    {
        return steam_ids64[get_index(username)];
    }
    else
    {
        return "";
    }
}

void AccountData::set_avatar_url(const QString &username, const QString &avatar_url)
{
    if(!avatar_urls.isEmpty())
    {
        mutex.lock();
        avatar_urls[get_index(username)] = avatar_url;
        mutex.unlock();
    }
}

QString AccountData::get_avatar_url(const QString &username)
{
    if(!avatar_urls.isEmpty())
    {
        return avatar_urls[get_index(username)];
    }
    else
    {
        return "";
    }
}

void AccountData::set_wallet_balance(const QString &username, const int &wallet_balance)
{
    if(!wallet_balances.isEmpty())
    {
        mutex.lock();
        wallet_balances[get_index(username)] = wallet_balance;
        mutex.unlock();
    }
}

int AccountData::get_wallet_balance(const QString &username)
{
    if(!wallet_balances.isEmpty())
    {
        return wallet_balances[get_index(username)];
    }
    else
    {
        return 0;
    }
}
