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

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <openssl/ssl.h>

#include <QUrlQuery>
#include <QNetworkProxy>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QMutex>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

#include "defines.h"
#include "persistentcookiejar.h"
#include "replytimeout.h"

/**
 * @brief The NetworkManager class
 *      This class extends the functionality of QNetworkAccessManager by
 *      implementing a persistent storage, a way to save and load data without
 *      IO operations and also implements a custom timeout system. Further more,
 *      this class also handles custom headers to the default and proxy requests.
 *      Multiple network users with multiple proxys are suported within the same instance.
 *      A single instance can throttle the requests automaticaly between users and proxys.
 * @date
 *      Created:  Filipe, 17 Mar 2014
 *      Modified: Filipe, 11 Apr 2014
 */
class NetworkManager : public QNetworkAccessManager
{
    Q_OBJECT

public_construct:
    explicit NetworkManager(QObject *parent = 0);
    explicit NetworkManager(QString user, QObject *parent = 0);
    explicit NetworkManager(QStringList users, QObject *parent = 0);
    explicit NetworkManager(QStringList users, bool load_cookies, QObject *parent = 0);

public_methods:
    QNetworkReply* getHTTP(QUrl link,
                           const QUrlQuery &get_parameters = QUrlQuery(),
                           const int &timeout = 0);

    QNetworkReply* postHTTP(QUrl link,
                            const QUrlQuery &post_parameters,
                            const QUrlQuery &get_parameters = QUrlQuery(),
                            const int &timeout = 0,
                            const QVariantHash &temp_settings = QVariantHash());

    QNetworkRequest& request();
    PersistentCookieJar* cookiejar() const;
    QList<QNetworkCookie> cookiesForUrl(const QString &user, const QUrl &url) const;

    void set_request(const QVariantHash &settings);
    void set_proxy(const QNetworkProxy::ProxyType &type,
                   const QString &hostname = "",
                   const quint16 &hostport = 0,
                   const QString &username = "",
                   const QString &password = "");

    void save_settings(const QString &user);
    void load_settings(const QString &user, const bool &load_cookies = true);

    static void add_proxy(const QString &user, QNetworkProxy &proxy);
    static void remove_proxy(const QString &user, const QNetworkProxy &proxy);
    static void save_proxys(const QString &user, QList<QNetworkProxy> &proxys, const bool &clear_current = false);
    static void parse_proxy_headers(const int &user_index, QNetworkProxy &proxy);

    QString print() const;

private_methods:
    void throttle_settings();
    static int validate_user(const QString &user);

private_members:
    static QMutex mutex;
    QStringList users;

private_data_members:
    QString current_manager;
    QNetworkProxy proxy_manager;
    QNetworkRequest request_manager;
    PersistentCookieJar *cookies_manager;

    static QList<QString> user_settings;
    static QList<QNetworkRequest> request_settings;
    static QList<QList<QNetworkProxy> > proxy_settings;
    static QList<QList<QNetworkCookie> > cookies_settings;

};

#endif // NETWORKMANAGER_H
