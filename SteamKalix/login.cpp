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

#include "login.h"

/**
 * @brief Login::Login
 *      Initializes members and sets headers to use in the HTTP requests.
 * @param parent QObject
 *      To use signals and slots, to set the parent of this class.
 * @remarks
 *      The headers "Accept-Encoding" and "Accept-Language" are set by default.
 *      They cannot be removed. If we add our own they will be duplicated.
 *      In addition to that, steam throws errors loading the page items if they are dupped.
 * @date
 *      Created:  Filipe, 2 Jan 2014
 *      Modified: Filipe, 15 Apr 2014
 */
Login::Login(QObject *parent) :
    QObject(parent),
    thread_id(Helper::get_thread_id()),
    state(persistent),
    username(""),
    password(""),
    encrypted_password(""),
    captcha_id(""),
    captcha_text(""),
    guard_code(""),
    guard_name(""),
    guard_email(""),
    timestamp(""),
    remember_login(true)
{
    url_getrsakey.setUrl("https://store.steampowered.com/login/getrsakey/");
    url_dologin.setUrl("https://store.steampowered.com/login/dologin/");   
    url_captcha.setUrl("https://store.steampowered.com/public/captcha.php");
    url_logout.setUrl("http://store.steampowered.com/logout/");
    url_store.setUrl("http://store.steampowered.com/");
    url_community.setUrl("http://steamcommunity.com/");
    url_eligibility.setUrl("https://steamcommunity.com/market/eligibilitycheck/");
    url_account.setUrl("https://store.steampowered.com/account/");
    url_profile_id.append("http://steamcommunity.com/id/");
    url_profile_number.append("http://steamcommunity.com/profiles/");

    network_manager = new NetworkManager(this);

    network_manager->request().setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    network_manager->request().setRawHeader("Cache-Control", "max-age=0");
    network_manager->request().setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
    network_manager->request().setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:28.0) Gecko/20100101 Firefox/28.0");
    network_manager->request().setRawHeader("DNT", "1");

//    I was having a problem with the request, and these cookies were the only ones that
//    showed up when I anaylsed the network and I didnt have set. Turned out that this was not the problem.

//    QNetworkCookie insert_cookie;
//    insert_cookie.setName("timezoneOffset");
//    insert_cookie.setValue("3600,0");
//    insert_cookie.setDomain("steamcommunity.com");
//    insert_cookie.setPath("/");
//    insert_cookie.setExpirationDate(QDateTime(QDate(2014,05,15)));
//    network_manager->cookiejar()->insertCookie(insert_cookie);

//    QNetworkCookie insert_cookie;
//    insert_cookie.setName("strInventoryLastContext");
//    insert_cookie.setValue("730_2");
//    insert_cookie.setDomain("steamcommunity.com");
//    insert_cookie.setPath("/");
//    insert_cookie.setExpirationDate(QDateTime(QDate(2014,05,15)));
//    network_manager->cookiejar()->insertCookie(insert_cookie);

}

/**
 * @brief Login::do_login
 *      Sets all UI parameters and calls a function to proceed according to the state of the login.
 * @param username
 * @param password
 * @param proxy_hostname
 * @param proxy_hostport
 * @param proxy_username
 * @param proxy_password
 * @date
 *      Created:  Filipe, 2 Jan 2014
 *      Modified: Filipe, 7 Mar 2014
 */
void Login::do_login(const QVariantHash &options)
{
    //Password or user change, the process has to be repeted.
    if(username != options.value("username").toString() || password != options.value("password").toString())
    {
        state = persistent;
        username = options.value("username").toString();
        password = options.value("password").toString();
    }

    //Set proxy settings.
    if(options.value("proxy_hostname", "").toString() != "" || options.value("proxy_hostport", "").toString() != "")
    {
        network_manager->set_proxy(Helper::proxy_type(options.value("proxy_type").toString()),
                                   options.value("proxy_hostname").toString(),
                                   options.value("proxy_hostport").toInt(),
                                   options.value("proxy_username").toString(),
                                   options.value("proxy_password").toString());
    }
    else
    {
        network_manager->set_proxy(QNetworkProxy::NoProxy);
    }

    output(network_manager->print(), 3);

    //Set captcha and steamguard code.
    captcha_text = options.value("captcha").toString();
    guard_code = options.value("guard_code").toString();
    guard_name = options.value("guard_name").toString();
    remember_login = options.value("remember_login").toBool();

    //Proceed acording to state.
    process_state();
}

/**
 * @brief Login::do_logout
 *      Requests the server to logout.
 * @date
 *      Created:  Filipe, 7 Mar 2014
 *      Modified: Filipe, 7 Jun 2014
 */
void Login::do_logout(const QString &username_logout)
{
    output("Logging out of " + username_logout + ".", 1);

    if(state != persistent && state != complete)
    {
        output("The previous login was not completed. It's state was discarted.", 2);
    }

    username = username_logout;
    network_manager->load_settings(username);

    disconnect(network_manager, SIGNAL(finished(QNetworkReply*)), this, NULL);
    connect(network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(process_logout(QNetworkReply*)));

    network_manager->getHTTP(url_logout);
}

/**
 * @brief Login::process_logout
 *      Deletes cookies, restores the class and UI to the initial state.
 * @remarks
 *      DEPRECATED: Delete cookies if 'remember_login' is true, because they are cleaned in complete_login if not.
 *      UPDATE:     Cookies will always be cleared because with multiple logins we dont know the option chosen at login.
 * @param reply
 *      The reply from the logout request.
 * @date
 *      Created:  Filipe, 7 Mar 2014
 *      Modified: Filipe, 7 Jun 2014
 */
void Login::process_logout(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        if(network_manager->cookiejar()->load(username, true) > 0)
        {
            network_manager->cookiejar()->clear(QStringList("steamMachineAuth"));
            network_manager->cookiejar()->save();
        }
        else
        {
            output("Could not load the cookies from this account.", 1);
        }

        output("Logged out of " + username + ".", 1);

        state = persistent;
        emit remove_account(username);
    }
    else
    {
        output("Network Error: " + reply->errorString(), 1);
        emit unlock_login();
    }

    reply->deleteLater();
}

/**
 * @brief Login::process_state
 *      Calls the right function according to the state.
 * @date
 *      Created:  Filipe, 7 Mar 2014
 *      Modified: Filipe, 7 Mar 2014
 */
void Login::process_state()
{
    if(state == persistent)
    {
        request_persistent();
    }
    else if(state == rsa)
    {
        request_rsa();
    }
    else if(state == login)
    {
        request_login();
    }
    else if(state == cookies)
    {
        request_cookies();
    }
    else if(state == profile)
    {
        request_profile();
    }
    else if(state == complete)
    {
        login_complete();
    }
}

/**
 * @brief Login::request_persistent
 *      Loads the cookies, and makes a request to test the login.
 *      If the cookies cannot be loaded, it proceeds with the normal login.
 * @remarks
 *      If the cookies are more than 2, we test them, if they fail, no problem, revert to normal login.
 *      If the cookies are more than 0, we assume that the user logged out, and the cookies steamMachineAuth were preserved.
 *      So we load them and proceed with a normal login. This is to prevent SteamGuard from asking a verification code.
 *      Else, we clear ALL the cookies because we want a fresh login (No record in file, new machine).
 * @date
 *      Created:  Filipe, 6 Mar 2014
 *      Modified: Filipe, 27 Apr 2014
 */
void Login::request_persistent()
{
    int loaded_cookies = network_manager->cookiejar()->load(username, true);

    if(loaded_cookies > 2)
    {
        output("Logging in with cookies...", 1);

        disconnect(network_manager, SIGNAL(finished(QNetworkReply*)), this, NULL);
        connect(network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(process_persistent(QNetworkReply*)));

        network_manager->getHTTP(url_account);
    }
    else if(loaded_cookies > 0)
    {
        output("Relogging...", 1);

        state = rsa;
        process_state();
    }
    else
    { 
        output("Starting new login...", 1);

        network_manager->cookiejar()->clear(); //New login, clear all cookies from the network
        state = rsa;
        process_state();
    }
}

/**
 * @brief Login::process_persistent
 *      Checks if the login was successful.
 *      If the reply is empty, it means that the server has redirected the request to the login page.
 *      This means that the cookies could not log the user, and a new login has to be made.
 *      If the reply is NOT empty, the server replied with the HTML data from the "account" page.
 * @param reply
 *      The replay from the test login.
 * @date
 *      Created:  Filipe, 6 Mar 2014
 *      Modified: Filipe, 6 Mar 2014
 */
void Login::process_persistent(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray buffer = reply->read(reply->bytesAvailable());

        if(buffer == "")
        {
            state = rsa;
        }
        else
        {
            state = profile;
            account_page = buffer;
        }

        process_state();
    }
    else
    {
        output("Network Error: " + reply->errorString(), 1);
        emit unlock_login();
    }

    reply->deleteLater();
}

/**
 * @brief Login::request_rsa
 *      Query the server for and RSA key and triggers the process_rsa slot.
 * @date
 *      Created:  Filipe, 5 Feb 2014
 *      Modified: Filipe, 12 Feb 2014
 */
void Login::request_rsa()
{
    output("Requesting RSA data...", 2);

    QUrlQuery parameters;
    parameters.addQueryItem("username", username);
    parameters.addQueryItem("l", "english"); 

    disconnect(network_manager, SIGNAL(finished(QNetworkReply*)), this, NULL);
    connect(network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(process_rsa(QNetworkReply*)));

    network_manager->postHTTP(url_getrsakey, parameters);
}

/**
 * @brief Login::process_rsa
 *      Processes RSA data, encrypts the password and sends the request for the authentication.
 * @param reply
 *      The result from the RSA data request.
 * @date
 *      Created:  Filipe, 3 Jan 2014
 *      Modified: Filipe, 7 Mar 2014
 */
void Login::process_rsa(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray buffer = reply->read(reply->bytesAvailable());
        QJsonObject json_object = QJsonDocument::fromJson(buffer).object();

        if(json_object.value("success").toBool())
        {
            output("Generating RSA public key.", 2);
            output("Key modulus: " + json_object.value("publickey_mod").toString(), 3);
            output("Key exponent: " + json_object.value("publickey_exp").toString(), 3);

            RSA *publickey = generate_rsa_publickey(json_object.value("publickey_mod").toString(), json_object.value("publickey_exp").toString());
            timestamp = json_object.value("timestamp").toString();

            if (publickey != NULL)
            {
                output("Encrypting password with RSA key.", 2);
                QScopedArrayPointer<unsigned char> encrypted(encrypt_rsa_pkcs1v15(password, publickey));

                if (encrypted != NULL)
                {
                    output("Encoding password in Base64.", 2);
                    QScopedArrayPointer<char> encoded(base64_encode(encrypted.data(), RSA_size(publickey)));

                    if (encoded != NULL)
                    {
                        encrypted_password = QString::fromUtf8(encoded.data());
                        output("Encrypted password: " + encrypted_password, 3);

                        state = login;
                        process_state();
                    }
                    else
                    {
                        output("Base64 encoding failed.", 1);
                        emit unlock_login();
                    }
                }
                else
                {
                    output("RSA encryption failed.", 1);
                    emit unlock_login();
                }

                RSA_free(publickey); //Also frees BIGNUM modulus and exponent
            }
            else
            {
                output("Public key generation failed.", 1);
                emit unlock_login();
            }
        }
        else
        {
            output("Could not get RSA data. (Did you forget the username?)", 1);
            emit unlock_login();
        }
    }
    else
    {
        output("Network Error: " + reply->errorString(), 1);
        emit unlock_login();
    }

    reply->deleteLater();
}

/**
 * @brief Login::request_login
 *      Query the server for the user authentication and triggers the process_login slot
 * @date
 *      Created:  Filipe, 5 Feb 2014
 *      Modified: Filipe, 12 Feb 2014
 */
void Login::request_login()
{
    output("Requesting user authentication...", 2);

    QUrlQuery parameters;
    parameters.addQueryItem("username", username);
    parameters.addQueryItem("password", encrypted_password.toUtf8().toPercentEncoding());
    parameters.addQueryItem("emailauth", guard_code);
    parameters.addQueryItem("loginfriendlyname", guard_name);
    parameters.addQueryItem("captchagid", captcha_id);
    parameters.addQueryItem("captcha_text", captcha_text);
    parameters.addQueryItem("emailsteamid", guard_email);
    parameters.addQueryItem("rsatimestamp", timestamp);
    parameters.addQueryItem("remember_login", remember_login  ? "true" : "false");
    parameters.addQueryItem("l", "english");   

    disconnect(network_manager, SIGNAL(finished(QNetworkReply*)), this, NULL);
    connect(network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(process_login(QNetworkReply*)));

    network_manager->postHTTP(url_dologin, parameters);
}

/**
 * @brief Login::process_login
 *      Calls the correct functions based on the response from the user authentication.
 * @remarks
 *      The transfer, unlike all the other POST and GET methods, is built only with the parameters
 *      that the server returns, here we emulate the same behavior as the login.js in the steam page.
 * @param reply
 *      The result from the user authentication request.
 * @date
 *      Created:  Filipe, 9 Jan 2014
 *      Modified: Filipe, 15 Apr 2014
 */
void Login::process_login(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray buffer = reply->read(reply->bytesAvailable());
        QJsonObject json_object = QJsonDocument::fromJson(buffer).object();

        if(json_object.value("success").toBool() && json_object.value("login_complete").toBool())
        {
            //Unscape URL
            QUrl transfer_url = json_object.value("transfer_url").toString().replace("\\", "");

            //Build parameters
            QHash<QString, QString> transfer_parameters;
            QJsonObject transfer_parameters_object = json_object.value("transfer_parameters").toObject();
            QStringList transfer_parameters_keys = transfer_parameters_object.keys();

            for(int i = 0; i < transfer_parameters_keys.size(); i++)
            {
                if(transfer_parameters_object.value(transfer_parameters_keys.at(i)).type() == QJsonValue::Bool)
                {
                    transfer_parameters.insert(transfer_parameters_keys.at(i), transfer_parameters_object.value(transfer_parameters_keys.at(i)).toBool() ? "true" : "false");
                }
                else
                {
                    transfer_parameters.insert(transfer_parameters_keys.at(i), transfer_parameters_object.value(transfer_parameters_keys.at(i)).toString());
                }
            }

            request_transfer(transfer_url, transfer_parameters);
        }
        else
        {
            output(json_object.value("message").toString(), 1);

            if(json_object.value("captcha_needed").toBool())
            {
                captcha_id = json_object.value("captcha_gid").toString();
                request_captcha();
            }
            else if(json_object.value("emailauth_needed").toBool())
            {
                guard_email = json_object.value("emailsteamid").toString();
                output("Type the code sent to your email at " + json_object.value("emaildomain").toString(), 1);
                emit steamguard_mode();
                emit unlock_login();
            }
            else
            {
                emit unlock_login();
            }
        }
    }
    else
    {
        output("Network Error: " + reply->errorString(), 1);
        emit unlock_login();
    }

    reply->deleteLater();
}

/**
 * @brief Login::request_transfer
 *      Query the server to transfer the login to the steamcommunity server.
 *      If the login was made from the steamcommunity server, the transfer would be to the store.
 * @param transfer_url
 *      The URL to transfer the login cookies. If the login is made from the store the transfer is
 *      for the community. But it can be the other way around.
 * @param transfer_parameters
 *      The parameters are variables according to the account status and the chosen parameters.
 *      For example, if SteamGuard is disabled on the account, the 'remember_login' parameter will be always false regardless of what was set.
 *      So, even if we choose to remmember the login, the transfer_parameters returns false, and in this case, the parameter steamRememberLogin
 *      is not set. Therfore, this needs to be complety dynamic.
 * @remarks
 *      There is no need to heap alocate this data because it is just used to pull the cookies from the server.
 * @date
 *      Created:  Filipe, 16 Feb 2014
 *      Modified: Filipe, 15 Apr 2014
 */
void Login::request_transfer(const QUrl &transfer_url, const QHash<QString, QString> &transfer_parameters)
{
    output("Requesting login transfer...", 2);

    QUrlQuery parameters;
    QHash<QString, QString>::const_iterator i;
    for(i = transfer_parameters.constBegin(); i != transfer_parameters.constEnd(); i++)
    {
        output("Key:" + i.key() + " Value:" + i.value(), 3);
        parameters.addQueryItem(i.key(), i.value());
    }

    disconnect(network_manager, SIGNAL(finished(QNetworkReply*)), this, NULL);
    connect(network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(process_transfer(QNetworkReply*)));

    network_manager->postHTTP(transfer_url, parameters);
}

/**
 * @brief Login::process_transfer
 *      Checks if the login was successfully transferred to the community page.
 * @param reply
 *      The result from the login transfer request.
 * @date
 *      Created:  Filipe, 16 Jan 2014
 *      Modified: Filipe, 7 Mar 2014
 */
void Login::process_transfer(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        state = cookies;
        process_state();
    }
    else
    {
        output("Network Error: " + reply->errorString(), 1);
        emit unlock_login();
    }

    reply->deleteLater();
}

/**
 * @brief Login::request_captcha
 *      Query the server for the captcha image with GET. Then triggers the process_captcha slot.
 * @date
 *      Created:  Filipe, 5 Feb 2014
 *      Modified: Filipe, 12 Feb 2014
 */
void Login::request_captcha()
{
    output("Requesting captcha...", 2);

    QUrlQuery parameters;
    parameters.addQueryItem("gid", captcha_id);

    disconnect(network_manager, SIGNAL(finished(QNetworkReply*)), this, NULL);
    connect(network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(process_captcha(QNetworkReply*)));

    network_manager->getHTTP(url_captcha, parameters);
}

/**
 * @brief Login::process_captcha
 *      Processes the image data and sets the captcha mode with the image to the UI.
 * @param reply
 *      The result from the captcha request.
 * @date
 *      Created:  Filipe, 20 Jan 2014
 *      Modified: Filipe, 5 Feb 2014
 */
void Login::process_captcha(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray buffer = reply->read(reply->bytesAvailable());

        QPixmap captcha;
        captcha.loadFromData(buffer);

        emit captcha_mode(captcha);
        emit unlock_login();
    }
    else
    {
        output("Network Error: " + reply->errorString(), 1);
        emit unlock_login();
    }

    reply->deleteLater();
}

/**
 * @brief Login::request_cookies
 *      Gets cookies set by accessing the pages.
 *      The returned page is also used to parse some account information.
 * @date
 *      Created:  Filipe, 20 Feb 2014
 *      Modified: Filipe, 15 Apr 2014
 */
void Login::request_cookies()
{
    disconnect(network_manager, SIGNAL(finished(QNetworkReply*)), this, NULL);
    connect(network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(process_cookies(QNetworkReply*)));

    replys.clear();
    for(int i = 0; i < 4; i++)
    {
        replys.append(NULL);
    }

    output("Requesting account information...", 2);
    replys[0] = network_manager->getHTTP(url_account);

    output("Requesting steampowered cookies...", 2);
    replys[1] = network_manager->getHTTP(url_store);

    output("Requesting steamcommunity cookies...", 2);
    replys[2] = network_manager->getHTTP(url_community);

    output("Requesting eligibilitycheck cookies...", 2);
    replys[3] = network_manager->getHTTP(url_eligibility);
}

/**
 * @brief Login::process_cookies
 *      This is a multi-request, it is accessed the number of times that request_cookies requests.
 *      It verifys if the pages were loaded correctly, and uses the information to fillin the account data.
 *      If no errors ocurred, the last call advances the state and deletes all replys.
 * @param reply
 *      The result from the cookie request.
 * @date
 *      Created:  Filipe, 20 Feb 2014
 *      Modified: Filipe, 15 Apr 2014
 */
void Login::process_cookies(QNetworkReply *reply)
{
    bool clear = false;

    int counter = 0;
    int counter_error = 0;
    int replys_size = replys.size();

    //Find out if all replys already finished
    for(int i = 0; i < replys_size; i++)
    {
        if(replys.at(i)->isFinished())
        {
            if(replys.at(i)->error() == QNetworkReply::NoError)
            {
                counter++;
            }
            else
            {
                counter_error++;
            }
        }
    }

    //Process replys
    if(reply->error() == QNetworkReply::NoError)
    {
        output("Got reply from: " + reply->url().toDisplayString(), 2);

        if(reply == replys[0])
        {
            account_page = reply->read(reply->bytesAvailable());
        }
    }
    else
    {
        output("Network Error: " + reply->errorString(), 1);
    }

    //If all replys arrived
    if(counter == replys_size)
    {
        state = profile;
        process_state();
        clear = true;
    }
    else if((counter + counter_error) == replys_size)
    {
        emit unlock_login();
        clear = true;
    }

    //Clear all replys
    if(clear)
    {
        for(int i = 0; i < replys_size; i++)
        {
            replys.at(i)->deleteLater();
        }
        replys.clear();
    }
}

/**
 * @brief Login::request_profile
 *      Gets the Steamcommunity ID and requests the page.
 *      The account_page is set by the last state.
 * @date
 *      Created:  Filipe, 26 Apr 2014
 *      Modified: Filipe, 26 Apr 2014
 */
void Login::request_profile()
{
    output("Requesting profile information...", 2);

    QString url_profile = "";

    if(account_page.indexOf(url_profile_id) != -1)
    {
        url_profile = url_profile_id;
    }
    else if(account_page.indexOf(url_profile_number) != -1)
    {
        url_profile = url_profile_number;
    }

    if(!url_profile.isEmpty())
    {
        QString steamcommunity_id = Helper::substring(account_page, url_profile, "/");
        url_profile.append(steamcommunity_id + "/");

        QUrlQuery parameters;
        parameters.addQueryItem("xml", "1");

        disconnect(network_manager, SIGNAL(finished(QNetworkReply*)), this, NULL);
        connect(network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(process_profile(QNetworkReply*)));

        network_manager->getHTTP(QUrl(url_profile), parameters);
    }
    else
    {
        output("There was an error parsing the account profile.", 1);
        emit unlock_login();
    }
}

/**
 * @brief Login::process_profile
 *      Gets some information about the account and profile.
 *      Saves the values in the AccountData namespace.
 * @remarks
 *      If an error occurs while parsing, atEnd() and hasError() return true,
 *      so xml.error() == QXmlStreamReader::NoError is not needed.
 * @param reply
 *      The profile page in xml.
 * @date
 *      Created:  Filipe, 27 Apr 2014
 *      Modified: Filipe, 27 Apr 2014
 */
void Login::process_profile(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString url_profile = "";

        if(account_page.indexOf(url_profile_id) != -1)
        {
            url_profile = url_profile_id;
        }
        else if(account_page.indexOf(url_profile_number) != -1)
        {
            url_profile = url_profile_number;
        }

        QString steamcommunity_id = Helper::substring(account_page, url_profile, "/");
        output("Community ID: " + steamcommunity_id, 1);

        url_profile.append(steamcommunity_id + "/");
        output("URL profile: " + url_profile, 1);

        QString wallet_display = Helper::substring(account_page, "<div class=\"accountData price\">", "</div>").replace("-", "0");
        output("Wallet ballance: " + Helper::currency_converter(wallet_display, Helper::currency::name), 1);

        int wallet_balance = Helper::price_converter(Helper::currency_converter(wallet_display, Helper::currency::remove).replace(",", ".").replace(" ","").toDouble());
        output("Wallet ballance converted: " + QString::number(wallet_balance), 3);

        QString email = Helper::substring(account_page, "<div class=\"\">", "</div>");
        output("Email: " + email, 1);

        //Process XML data
        QString steamID64 = "";
        QString steamID = "";
        QString avatar = "";

        QXmlStreamReader xml(reply->read(reply->bytesAvailable()));

        while (!xml.atEnd()) //Include error handling
        {
            if(xml.readNext() == QXmlStreamReader::StartElement)
            {
                QStringRef element = xml.name();
                if(element.contains("steamID64"))
                {
                    steamID64 = xml.readElementText();
                    output("Steam ID64: " + steamID64, 3);
                }
                else if(element.contains("steamID"))
                {
                    steamID = xml.readElementText();
                    if(!steamID.isEmpty())
                    {
                        output("Steam ID: " + steamID, 3);
                    }
                }
                else if(element.contains("avatarMedium"))
                {
                    avatar = xml.readElementText();
                    output("Avatar URL: " + avatar, 3);
                }
            }
        }
        if (!xml.hasError() &&
            !steamcommunity_id.isEmpty() &&
            !url_profile.isEmpty() &&
            !email.isEmpty() &&
            !steamID64.isEmpty() &&
            wallet_balance >= 0)
        {
            //Create account static data
            AccountData::add_account(username,
                                     steamcommunity_id,
                                     url_profile,
                                     email,
                                     steamID64,
                                     steamID,
                                     avatar,
                                     wallet_balance);

            state = complete;
            process_state();
        }
        else
        {
            output("An error ocurred while reading the profile data.", 1);
            emit unlock_login();
        }
    }
    else
    {
        output("Network Error: " + reply->errorString(), 1);
        emit unlock_login();
    }

    reply->deleteLater();
}

/**
 * @brief Login::login_complete
 *      This fuction completes the login process by storing/clearing the cookies.
 * @date
 *      Created:  Filipe, 7 Mar 2014
 *      Modified: Filipe, 7 Jun 2014
 */
void Login::login_complete()
{
    if(remember_login)
    {
        network_manager->cookiejar()->save();
    }
    else
    {
        network_manager->cookiejar()->clear(QStringList("steamMachineAuth"));
        network_manager->cookiejar()->save();
    }

    //Save network settings.
    network_manager->save_settings(username);

    //Add account to UI.
    emit add_account(username);

    output("Cookies pulled:" + network_manager->cookiejar()->print(), 3);
    output("Logged in with " + username + "!", 1, true);
}

/**
 * @brief Login::generate_rsa_publickey
 *      Generates a RSA public key with the parameters passed.
 * @param modulus
 *      Received from the server.
 * @param exponent
 *      Received from the server.
 * @return
 *      The public key to be used in encryption.
 * @date
 *      Created:  Filipe, 3 Jan 2014
 *      Modified: Filipe, 8 Jan 2014
 */
RSA* Login::generate_rsa_publickey(const QString &modulus, const QString &exponent)
{
    RSA *publickey = NULL;

    try
    {
        publickey = RSA_new();
        BIGNUM *publickey_mod = BN_new();
        BIGNUM *publickey_exp = BN_new();

        //QString -> String -> Char* -> BIGNUM
        BN_hex2bn(&publickey_mod, modulus.toStdString().c_str());
        BN_hex2bn(&publickey_exp, exponent.toStdString().c_str());

        //Adds data to RSA publickey
        publickey->n = publickey_mod;
        publickey->e = publickey_exp;
    }
    catch(...)
    {
        output("An error ocurred while creating the RSA publickey.", 1, true);
        RSA_free(publickey);
        publickey = NULL;
    }

    return publickey;
}

/**
 * @brief Login::encrypt_rsa_pkcs1v15
 *      Encrypts the 'input' with the RSA public key from the function 'generate_rsa_publickey'.
 * @param input
 *      Plain text input to be encrypted.
 * @param publickey
 *      The public key generated from the function generate_rsa_publickey.
 * @return
 *      input encrypted.
 * @date
 *      Created:  Filipe, 3 Jan 2014
 *      Modified: Filipe, 8 Jan 2014
 */
unsigned char* Login::encrypt_rsa_pkcs1v15(const QString &input, RSA *publickey)
{
    unsigned char *encrypted = NULL;

    try
    {
        encrypted = new unsigned char[RSA_size(publickey)];

        if(RSA_public_encrypt(input.length(), reinterpret_cast<const unsigned char *>(input.toStdString().c_str()), encrypted, publickey, RSA_PKCS1_PADDING) == -1)
        {
            delete[] encrypted;
            encrypted = NULL;
        }
    }
    catch(...)
    {
        output("An error ocurred while encrypting your password with RSA PKCS#1 v1.5 padding.", 1, true);
        delete[] encrypted;
        encrypted = NULL;
    }

    return encrypted;
}

/**
 * @brief Login::base64_encode
 *      Function to encode 'input' in base64.
 * @param input
 *      Plain text value to be encoded.
 * @param length
 *      The size of input.
 * @return
 *      input encoded.
 * @date
 *      Created:  Filipe, 3 Jan 2014
 *      Modified: Filipe, 8 Jan 2014
 */
char* Login::base64_encode(const unsigned char *input, const int &length)
{
    BIO *base64 = NULL;
    BIO *base64_memory = NULL;
    BUF_MEM *base64_ptr = NULL;
    char *encoded = NULL;

    try
    {
        //Creates new base64 BIO and new memory BIO
        base64 = BIO_new(BIO_f_base64());
        base64_memory = BIO_new(BIO_s_mem());

        //Appends the BIO memory to the base64 BIO and sets flag to encode a single line
        base64 = BIO_push(base64, base64_memory);
        BIO_set_flags(base64, BIO_FLAGS_BASE64_NO_NL);

        //Writes 'length' bytes from 'input' to 'base64' BIO
        BIO_write(base64, input, length);

        //Flushes out internally buffered data and places the underlying base64 structure in pointer
        (void) BIO_flush(base64);
        BIO_get_mem_ptr(base64, &base64_ptr);

        //Data to return
        encoded = new char[base64_ptr->length+1];
        memcpy(encoded, base64_ptr->data, base64_ptr->length);
        encoded[base64_ptr->length] = 0;

        //Frees the BIO chain (base64_memory included)
        BIO_free_all(base64);
    }
    catch(...)
    {
        output("An error ocurred while encoding the encrypted password in Base64.", 1, true);
        delete[] encoded;
        encoded = NULL;
        BIO_free_all(base64);
    }

    return encoded;
}

/**
 * @brief Login::output
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
void Login::output(const QString &message, const int &verbose, const bool &log)
{
    if(Output::get_verbose() >= verbose)
    {
        QVariantHash data;
        data.insert("message", message);
        data.insert("verbose", verbose);
        data.insert("log", log);
        data.insert("ID1", thread_id);

        QString print = Output::builder(data);

        if(!print.isEmpty())
        {
            emit console(print);
        }
    }
}
