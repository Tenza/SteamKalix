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

#include "persistentcookiejar.h"

/**
 * @brief PersistentCookieJar::PersistentCookieJar
 *      Initializes members.
 * @param autosave
 *      Chooses if the class should save the cookies on destruction.
 * @date
 *      Created:  Filipe, 6 Mar 2014
 *      Modified: Filipe, 9 Mar 2014
 */
PersistentCookieJar::PersistentCookieJar(bool autosave)
{
    this->autosave = autosave;
    this->name = "";
}

/**
 * @brief PersistentCookieJar::~PersistentCookieJar
 *      If enabled, saves current cookies.
 *      There are no resources to delete.
 * @date
 *      Created:  Filipe, 6 Mar 2014
 *      Modified: Filipe, 9 Mar 2014
 */
PersistentCookieJar::~PersistentCookieJar()
{
    if(autosave)
    {
        save();
    }
}

/**
 * @brief PersistentCookieJar::load
 *      Loads the cookies according to the passed user.
 * @param name
 *      The name in which the saved cookies correspond to.
 * @param clear_oookies
 *      Flags if the current cookies are to be cleared.
 * @return
 *      The number of loaded cookies.
 * @date
 *      Created:  Filipe, 6 Mar 2014
 *      Modified: Filipe, 15 Apr 2014
 */
int PersistentCookieJar::load(const QString &name, bool clear_oookies)
{
    this->name = name;

    QByteArray cookie_storage = SettingsManager::read("Cookies/" + name).toByteArray();
    QList<QNetworkCookie> cookie_list = QNetworkCookie::parseCookies(cookie_storage);

    if(clear_oookies)
    {
        setAllCookies(cookie_list);
    }
    else
    {
        for(int i = 0; i < cookie_list.size(); i++)
        {
            insertCookie(cookie_list.at(i));
        }
    }

    return cookie_list.size();
}

/**
 * @brief PersistentCookieJar::save
 *      Saves all the current cookies if the user is already set.
 * @return
 *      True if saved, false if not.
 * @date
 *      Created:  Filipe, 6 Mar 2014
 *      Modified: Filipe, 9 Mar 2014
 */
bool PersistentCookieJar::save() const
{
    bool result = false;

    if(name != "")
    {
        QList<QNetworkCookie> cookie_list = allCookies();
        QByteArray cookie_storage;

        for(int i = 0; i < cookie_list.size(); i++)
        {
            cookie_storage.append(cookie_list[i].toRawForm() + "\n");
        }

        SettingsManager::write("Cookies/" + name, cookie_storage);

        result = true;
    }

    return result;
}

/**
 * @brief PersistentCookieJar::clear
 *      Clears all the cookies by loading an empty cookie list.
 * @date
 *      Created:  Filipe, 6 Mar 2014
 *      Modified: Filipe, 6 Mar 2014
 */
void PersistentCookieJar::clear()
{
    QList<QNetworkCookie> cookie_list;
    setAllCookies(cookie_list);
}

/**
 * @brief PersistentCookieJar::clear
 *      Clears all the cookies except the ones passed on the list.
 * @param exclude
 *      The list of cookies that cannot be deleted.
 * @date
 *      Created:  Filipe, 15 Apr 2014
 *      Modified: Filipe, 15 Apr 2014
 */
void PersistentCookieJar::clear(const QStringList &exclude)
{
    QList<QNetworkCookie> cookie_list = allCookies();
    bool found = false;

    for(int i = 0; i < cookie_list.size(); i++)
    {
        for(int j = 0; j < exclude.length(); j++)
        {
            if(cookie_list.at(i).name().contains(exclude.at(j).toUtf8()))
            {
                found = true;
                break;
            }
        }

        if(!found)
        {
            deleteCookie(cookie_list.at(i));
        }

        found = false;
    }
}

/**
 * @brief PersistentCookieJar::count
 *      Returns the number of cookies currently set.
 * @return
 *      The number of cookies.
 * @date
 *      Created:  Filipe, 5 Set 2014
 *      Modified: Filipe, 5 Set 2014
 */
int PersistentCookieJar::count()
{
    QList<QNetworkCookie> cookie_list = allCookies();
    return cookie_list.size();
}

/**
 * @brief PersistentCookieJar::set_all_cookies
 *      Gives access to set all cookies.
 * @remarks
 *      This is done because the function setAllCookies() is protected in the base class.
 *      Therefore, it can only be access by this derived class.
 * @param cookies_list
 *      List with all the cookies.
 * @date
 *      Created:  Filipe, 23 Mar 2014
 *      Modified: Filipe, 23 Mar 2014
 */
void PersistentCookieJar::set_all_cookies(const QList<QNetworkCookie> &cookies_list)
{
    setAllCookies(cookies_list);
}

/**
 * @brief PersistentCookieJar::all_cookies
 *      Gives access to all the cookies.
 * @remarks
 *      This is done because the function allCookies() is protected in the base class.
 *      Therefore, it can only be access by this derived class.
 * @return
 *      List with all the cookies.
 * @date
 *      Created:  Filipe, 6 Mar 2014
 *      Modified: Filipe, 6 Mar 2014
 */
QList<QNetworkCookie> PersistentCookieJar::all_cookies() const
{
    return allCookies();
}

/**
 * @brief PersistentCookieJar::print
 *      Creates an array with the raw data of the cookies.
 *      Use and output method that supports HTML.
 * @return
 *      Data to be displayed
 * @date
 *      Created:  Filipe, 6 Mar 2014
 *      Modified: Filipe, 7 Mar 2014
 */
QByteArray PersistentCookieJar::print() const
{
    QList<QNetworkCookie> cookie_list = allCookies();
    QByteArray cookie_storage;

    for(int i = 0; i < cookie_list.size(); i++)
    {
        cookie_storage.append("<p>" + cookie_list[i].toRawForm()+ "</p>");
    }

    return cookie_storage;
}
