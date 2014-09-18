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

#include "networkmanager.h"

/**
 * @brief NetworkManager::user_settings
 * @brief NetworkManager::request_settings
 * @brief NetworkManager::proxy_settings
 * @brief NetworkManager::cookies_settings
 *      Definition of static members at file scope.
 * @date
 *      Created:  Filipe, 24 Mar 2014
 *      Modified: Filipe, 3 Apr 2014
 */
QList<QString> NetworkManager::user_settings;
QList<QNetworkRequest> NetworkManager::request_settings;
QList<QList<QNetworkProxy> > NetworkManager::proxy_settings;
QList<QList<QNetworkCookie> > NetworkManager::cookies_settings;
QMutex NetworkManager::mutex;

/**
 * @brief NetworkManager::NetworkManager
 *      All of the constructors initialize the cookies manager.
 *      The other parameters are used to load the static variables,
 *      either just load one. Or an array of accounts with multiple proxys.
 * @remarks
 *      setParent, the initialization list cannot be used, this is a indirect base ( : QObject(parent)).
 *      The object QNetworkAccessManager will take ownership when setCookieJar is called, no need to destroy.
 * @date
 *      Created:  Filipe, 24 Mar 2014
 *      Modified: Filipe, 3 Sep 2014
 */
NetworkManager::NetworkManager(QObject *parent) :
    current_manager(""),
    cookies_manager(new PersistentCookieJar())
{
    setParent(parent);
    setCookieJar(cookies_manager);
}

NetworkManager::NetworkManager(QString user, QObject *parent) :
    current_manager(""),
    cookies_manager(new PersistentCookieJar())
{
    setParent(parent);
    setCookieJar(cookies_manager);

    if(!user.isEmpty())
    {
        this->users = QStringList(user);
        load_settings(users.at(0));
    }
}

NetworkManager::NetworkManager(QStringList users, QObject *parent) :
    current_manager(""),
    cookies_manager(new PersistentCookieJar())
{
    setParent(parent);
    setCookieJar(cookies_manager);

    if(!users.isEmpty())
    {        
        this->users = users;
        load_settings(users.at(0));
    }
}

NetworkManager::NetworkManager(QStringList users, bool load_cookies, QObject *parent) :
    current_manager(""),
    cookies_manager(new PersistentCookieJar())
{
    setParent(parent);
    setCookieJar(cookies_manager);

    if(!users.isEmpty())
    {
        this->users = users;
        load_settings(users.at(0), load_cookies);
    }
}

/**
 * @brief NetworkManager::getHTTP
 *      Makes get requests.
 *      Extends the default implementation by adding support for timeouts and managment of the request.
 * @param link
 *      URL to perform the request. Parametes will be overwritten.
 * @param get_parameters
 *      Get parameters to be appended to the URL.
 * @param timeout
 *      Amount of time until de request timeouts.
 * @return
 *      The pointer for the reply of this request.
 * @date
 *      Created:  Filipe, 2 Apr 2014
 *      Modified: Filipe, 3 Apr 2014
 */
QNetworkReply* NetworkManager::getHTTP(QUrl link, const QUrlQuery &get_parameters, const int &timeout)
{
    if(!users.isEmpty())
    {
        throttle_settings();
    }

    //Add GET parameters
    link.setQuery(get_parameters);
    request_manager.setUrl(link);

    //Perform GET request
    QNetworkReply* reply = get(request_manager);

    if(timeout > 0)
    {
        new ReplyTimeout(reply, timeout);
    }

    return reply;
}

/**
 * @brief NetworkManager::postHTTP
 *      Makes post requests.
 *      Extends the default implementation by adding support for timeouts and managment of the request.
 *      This also simplifys the use of custom headers by backingup the current one, using and then swaping.
 * @param link
 *      URL to perform request. Parametes will be overwritten.
 * @param post_parameters
 *      Post parameters to be sent to the server.
 * @param get_parameters
 *      Get parameters to be appended to the URL.
 * @param timeout
 *      Amount of time until de request timeouts
 * @param temp_settings
 *      Temporary settings, that are applied only for this request.
 * @return
 *      The pointer for the reply of this request.
 * @date
 *      Created:  Filipe, 2 Apr 2014
 *      Modified: Filipe, 13 Apr 2014
 */
QNetworkReply* NetworkManager::postHTTP(QUrl link, const QUrlQuery &post_parameters, const QUrlQuery &get_parameters, const int &timeout, const QVariantHash &temp_settings)
{
    QNetworkRequest original_request;

    if(!users.isEmpty())
    {
        throttle_settings();
    }

    //Backup and set current request settings
    if(!temp_settings.isEmpty())
    {
        original_request = request_manager;
        set_request(temp_settings);
    }

    //Add GET parameters
    link.setQuery(get_parameters);
    request_manager.setUrl(link);

    //Perform POST request
    QNetworkReply* reply = post(request_manager, post_parameters.query().toUtf8());

    if(timeout > 0)
    {
        new ReplyTimeout(reply, timeout);
    }

    //Restore settings
    if(!temp_settings.isEmpty())
    {
        request_manager.swap(original_request);
    }

    return reply;
}

/**
 * @brief NetworkManager::request
 *      Used to get the internal object of the request.
 *      This is used as an interface to the QNetworkRequest.
 *      This is used to set the definitions of the request. The definitions set with this, will be used for all
 *      post and get requests. You can also use the temporary_header field, on the POST to set a header for a specific request.
 * @return
 *      The request object.
 * @date
 *      Created:  Filipe, 2 Apr 2014
 *      Modified: Filipe, 3 Apr 2014
 */
QNetworkRequest& NetworkManager::request()
{
    return request_manager;
}

/**
 * @brief NetworkManager::get_cookiejar
 *      Used to get the internal object of persistent storage.
 *      This is used as an interface to the PersistentCookieJar class.
 * @return
 *      The object with cookies.
 * @date
 *      Created:  Filipe, 24 Mar 2014
 *      Modified: Filipe, 24 Mar 2014
 */
PersistentCookieJar* NetworkManager::cookiejar() const
{
    return cookies_manager;
}

/**
 * @brief NetworkManager::cookiesForUrl
 *      Used to get the cookies from a user that is not currently set.
 * @param user
 *      The user
 * @param url
 *      The url
 * @return
 *      List of cookies from the specific user and url.
 * @date
 *      Created:  Filipe, 3 Jul 2014
 *      Modified: Filipe, 3 Jul 2014
 */
QList<QNetworkCookie> NetworkManager::cookiesForUrl(const QString &user, const QUrl &url) const
{
    int index = validate_user(user);
    PersistentCookieJar *cookies = new PersistentCookieJar();

    if(index >= 0)
    {
        cookies->set_all_cookies(cookies_settings.at(index));
    }

    return cookies->cookiesForUrl(url);
}

/**
 * @brief NetworkManager::set_request
 *      This provides another way of setting the request. (The 'request()' function should be used instead)
 *      The purpose of this is just to provide an easy way of setting temporary settings with the POST and GET requests,
 *      that is why it has a single argument.
 * @param settings
 *      All the settings to be set.
 * @remarks
 *      This function is ready to add more settings with ease.
 * @date
 *      Created:  Filipe, 13 Apr 2014
 *      Modified: Filipe, 13 Apr 2014
 */
void NetworkManager::set_request(const QVariantHash &settings)
{
    QList<QString> settings_keys = settings.keys();
    for(int i = 0; i < settings_keys.size(); i++)
    {
        if(settings_keys.at(i) == "setPriority")
        {
            QNetworkRequest::Priority priority = QNetworkRequest::NormalPriority;
            int priority_aux = settings.value("setPriority").toInt();

            if(priority_aux == 1)
            {
                priority = QNetworkRequest::HighPriority;
            }
            else if(priority_aux == 5)
            {
                priority = QNetworkRequest::LowPriority;
            }

            request_manager.setPriority(priority);
        }
        else
        {
            request_manager.setRawHeader(settings_keys.at(i).toUtf8(), settings.value(settings_keys.at(i)).toByteArray());
        }
    }
}

/**
 * @brief NetworkManager::set_proxy
 *      Sets a proxy in the manager, and copys all headers from the request.
 * @param type
 * @param hostname
 * @param hostport
 *      Is quint16 to match the setPort function.
 * @param username
 * @param password
 * @date
 *      Created:  Filipe, 2 Apr 2014
 *      Modified: Filipe, 13 Apr 2014
 */
void NetworkManager::set_proxy(const QNetworkProxy::ProxyType &type,
                               const QString &hostname,
                               const quint16 &hostport,
                               const QString &username,
                               const QString &password)
{
    proxy_manager.setType(type);
    proxy_manager.setHostName(hostname);
    proxy_manager.setPort(hostport);
    proxy_manager.setUser(username);
    proxy_manager.setPassword(password);

    QList<QByteArray> raw_headers = request_manager.rawHeaderList();
    for(int i = 0; i < raw_headers.size(); i++)
    {
        proxy_manager.setRawHeader(raw_headers.at(i), request_manager.rawHeader(raw_headers.at(i)));
    }

    setProxy(proxy_manager);
}

/**
 * @brief NetworkManager::save_settings
 *      Saves the network object settings in static variables.
 * @param user
 *      The name of the setting to be saved.
 *      This is obligatory, all settigs must have a name/user.
 * @remarks
 *      This method writes data to the static arrays it should be thread-safe.
 * @date
 *      Created:  Filipe, 24 Mar 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void NetworkManager::save_settings(const QString &user)
{
    if(validate_user(user) < 0)
    {
        mutex.lock();
        QList<QNetworkProxy> proxys;
        proxys.prepend(this->proxy());

        user_settings.append(user);
        proxy_settings.append(proxys);
        request_settings.append(request_manager);
        cookies_settings.append(cookies_manager->all_cookies());
        mutex.unlock();
    }
}

/**
 * @brief NetworkManager::load_settings
 *      Loads the network object with the static variables.
 * @param user
 *      Name of the setting to be loaded.
 * @param proxy_index
 *      Index of the proxy.
 * @date
 *      Created:  Filipe, 24 Mar 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void NetworkManager::load_settings(const QString &user, const bool &load_cookies)
{
    int index = validate_user(user);

    if(index >= 0)
    {
        if(current_manager != user)
        {
            current_manager = user_settings.value(index);
            request_manager = request_settings.value(index);

            if(load_cookies)
            {
                cookies_manager->set_all_cookies(cookies_settings.value(index));
            }
        }

        //Proxy is always set, even with 1 user and 1 proxy.
        //This is done because the current proxy might be removed in the middle of the throttle, this will restore the balance.
        proxy_manager = proxy_settings.at(index).value(0);
        setProxy(proxy_manager);
    }
}

/**
 * @brief NetworkManager::add_proxy
 *      Adds a proxys to the given user.
 * @param user
 *      The user to find.
 * @param proxy
 *      The proxy to be added. To add multiple proxys use the save_proxys function.
 * @date
 *      Created:  Filipe, 3 Jul 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void NetworkManager::add_proxy(const QString &user, QNetworkProxy &proxy)
{
    int index = validate_user(user);

    if(index >= 0)
    {
        mutex.lock();
        parse_proxy_headers(index, proxy);
        proxy_settings[index].append(proxy);
        mutex.unlock();
    }
}

/**
 * @brief NetworkManager::remove_proxy
 *      Removes a proxys from a given user.
 * @param user
 *      The user to  find.
 * @param proxy
 *      The proxy to be removed.
 * @date
 *      Created:  Filipe, 3 Jul 2014
 *      Modified: Filipe, 3 Jul 2014
 */
void NetworkManager::remove_proxy(const QString &user, const QNetworkProxy &proxy)
{
    int index = validate_user(user);

    if(index >= 0)
    {
        //Never remove the first one (set by the login)
        for(int i = 1; i < proxy_settings.at(index).size(); i++)
        {
            if(proxy_settings.at(index).at(i) == proxy)
            {
                mutex.lock();
                proxy_settings[index].removeAt(i);
                mutex.unlock();

                break;
            }
        }
    }
}

/**
 * @brief NetworkManager::save_proxys
 *      Saves the list of proxys for a specific network user.
 *      This is used when the new network user is created.
 * @param user
 *      The user.
 * @param proxys
 *      Proxys to set.
 * @param clear_current
 *      If the current proxys are to be cleared or just append.
 * @date
 *      Created:  Filipe, 20 Jun 2014
 *      Modified: Filipe, 20 Jun 2014
 */
void NetworkManager::save_proxys(const QString &user, QList<QNetworkProxy> &proxys, const bool &clear_current)
{
    int index = validate_user(user);

    if(index >= 0)
    {
        mutex.lock();
        if(clear_current)
        {
            //Never remove the first one (set by the login).
            for(int i = 1; i < proxy_settings.at(index).size(); i++)
            {
                proxy_settings[index].removeAt(i);
            }
        }

        for(int i = 0; i < proxys.size(); i++)
        {
            parse_proxy_headers(index, proxys[i]);
            proxy_settings[index].append(proxys[i]);
        }
        mutex.unlock();
    }
}

/**
 * @brief NetworkManager::parse_proxy_headers
 *      Copys the headers from the request, that are set by the login to the passed proxy.
 * @param user_index
 *      The user index to locate the request.
 * @param proxy
 *      Proxy to be modifyed.
 * @remarks
 *      TEST
 */
void NetworkManager::parse_proxy_headers(const int &user_index, QNetworkProxy &proxy)
{
    QList<QByteArray> raw_headers = request_settings[user_index].rawHeaderList();
    for(int i = 0; i < raw_headers.size(); i++)
    {
        proxy.setRawHeader(raw_headers.at(i), request_settings[user_index].rawHeader(raw_headers.at(i)));
    }
}

/**
 * @brief NetworkManager::throttle_settings
 *      If this instance has users, this function will throttle between all users and all proxys.
 * @date
 *      Created:  Filipe, 21 Jun 2014
 *      Modified: Filipe, 21 Jun 2014
 */
void NetworkManager::throttle_settings()
{
    bool found = false;
    int current_index = validate_user(current_manager);

    if(current_index >= 0 && !proxy_settings.at(current_index).isEmpty())
    {
        if(proxy_manager != proxy_settings.at(current_index).last()) //Change proxy.
        {
            for(int i = 0; i < proxy_settings.at(current_index).size() - 1; i++)
            {
                if(proxy_manager == proxy_settings.at(current_index).at(i))
                {
                    //Set NEXT proxy, current user.
                    proxy_manager = proxy_settings.at(current_index).at(i + 1);
                    setProxy(proxy_manager);
                    found = true;
                    break;
                }
            }
            if(!found)
            {
                //Proxy was removed, reset cycle.
                load_settings(users.at(0));
            }
        }
        else //Change user.
        {
            if(current_manager != users.last())
            {
                for(int i = 0; i < users.size() - 1; i++)
                {
                    if(current_manager == users.at(i))
                    {
                        //Set NEXT user, first proxy.
                        load_settings(users.at(i + 1));
                        break;
                    }
                }
            }
            else
            {
                //Set FIRST user, first proxy.
                load_settings(users.at(0));
            }
        }
    }
}

/**
 * @brief NetworkManager::validate_user
 *      The passed user exists? What is the index?
 * @param user
 *      User to be tested
 * @return
 *      Index of the user. -1 = Does not exit.
 * @date
 *      Created:  Filipe, 20 Jun 2014
 *      Modified: Filipe, 20 Jun 2014
 */
int NetworkManager::validate_user(const QString &user)
{
    int index = -1;

    for(int i = 0; i < user_settings.size(); i++)
    {
        if(user_settings.value(i) == user)
        {
            index = i;
            break;
        }
    }

    return index;
}

/**
 * @brief NetworkManager::print
 *      Creates a string with the information of the current request.
 *      Use and output method that supports HTML.
 * @return
 *      Data to be displayed
 * @date
 *      Created:  Filipe, 22 Jun 2014
 *      Modified: Filipe, 22 Jun 2014
 */
/**
 * @brief NetworkManager::print
 * @return
 */
QString NetworkManager::print() const
{
    QString request_print;

    if(current_manager.isEmpty())
    {
        request_print.append("Account: None - ");
    }
    else
    {
        if(cookies_manager->count() > 0)
        {
            request_print.append("Account: " + current_manager + " - Cookies: Yes - ");
        }
        else
        {
            request_print.append("Account: " + current_manager + " - Cookies: No - ");
        }

    }

    if(proxy_manager.hostName().isEmpty())
    {
        request_print.append("Connection: Direct");
    }
    else
    {
        request_print.append("Connection: " + proxy_manager.hostName() + ":");
        request_print.append(QString::number(proxy_manager.port()) + " ");
        request_print.append(proxy_manager.user() + " ");
        request_print.append(proxy_manager.password());
    }

    return request_print;
}
