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

#ifndef PERSISTENTCOOKIEJAR_H
#define PERSISTENTCOOKIEJAR_H

#include <QNetworkCookie>
#include <QNetworkCookieJar>

#include "defines.h"
#include "settingsmanager.h"

/**
 * @brief The PersistentCookieJar class
 *      This class derives from "QNetworkCookieJar" in order to implement a presistent way to store and handle cookies.
 *      This class has no parent. It is supposed to be used with the function 'setCookieJar' which will take the ownership of this object.
 * @date
 *      Created:  Filipe, 6 Mar 2014
 *      Modified: Filipe, 15 Apr 2014
 */
class PersistentCookieJar : public QNetworkCookieJar
{

public_construct:
    PersistentCookieJar(bool autosave = false);
    ~PersistentCookieJar();

public_methods:
    int load(const QString &name, bool clear_oookies = false);
    bool save() const;
    void clear();
    void clear(const QStringList &exclude);
    int count();

    void set_all_cookies(const QList<QNetworkCookie> &cookies_list);
    QList<QNetworkCookie> all_cookies() const;
    QByteArray print() const;

private_members:
    QString name;
    bool autosave;

};

#endif // PERSISTENTCOOKIEJAR_H
