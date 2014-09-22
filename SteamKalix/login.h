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

#ifndef LOGIN_H
#define LOGIN_H

#include <QObject>
#include <QPixmap>
#include <QScopedArrayPointer>
#include <QXmlStreamReader>

#include "defines.h"
#include "output.h"
#include "helper.h"
#include "accountdata.h"
#include "networkmanager.h"

/**
 * @class The Login class
 *      This class implements all the procedures required to login on the steam servers.
 * @remarks Path of execution
 *      IF cookies exist for current user, try login.
 *      Request RSA data from server.
 *      Generate a usable RSA public key.
 *      Encrypt password with RSA key.
 *      Encode password in base64.
 *      Send authentication to the server.
 *      IF captcha verification, resend authentication.
 *      IF steamGuard verification, resend authentication.
 *      Request pages for cookies and profile information.
 *      Completed login, save cookies if remember password is enabled.
 * @date
 *      Created:  Filipe, 2 Jan 2014
 *      Modified: Filipe, 7 Mar 2014
 */
class Login : public QObject
{
    Q_OBJECT

public_construct:
    explicit Login(QObject *parent = 0);

public_methods:
    void do_login(const QVariantHash &options);
    void do_logout(const QString &username_logout);

private_methods:
    //Request data functions 
    void request_persistent();
    void request_cookies();
    void request_rsa();
    void request_login();
    void request_captcha();
    void request_transfer(const QUrl &transfer_url, const QHash<QString, QString> &transfer_parameters);
    void request_profile();

    void login_complete();
    void process_state();

    //Helper password functions
    RSA* generate_rsa_publickey(const QString &modulus, const QString &exponent);
    unsigned char* encrypt_rsa_pkcs1v15(const QString &input, RSA *publickey);
    char* base64_encode(const unsigned char *input, const int &length);

    void output(const QString &message, const int &verbose, const bool &log = false);

private_enums:
    enum states
    {
        persistent = 1,
        rsa = 2,
        login = 3,
        cookies = 4,
        profile = 5,
        complete = 6
    };

private_members:
    //Identifiers
    QString thread_id;
    states state;

    //User set variables
    QString username;
    QString password;
    QString encrypted_password;
    QString captcha_id;
    QString captcha_text;
    QString guard_code;
    QString guard_name;
    QString guard_email;
    QString timestamp;
    bool remember_login;

    //Steam URLs
    QUrl url_getrsakey;
    QUrl url_dologin;
    QUrl url_captcha;
    QUrl url_logout;
    QUrl url_store;
    QUrl url_community;
    QUrl url_eligibility;
    QUrl url_account;
    QString url_profile_id;
    QString url_profile_number;

private_data_members:
    QByteArray account_page;
    QList<QNetworkReply*> replys;
    NetworkManager *network_manager;

private slots:
    void process_persistent(QNetworkReply *reply);
    void process_cookies(QNetworkReply *reply);
    void process_rsa(QNetworkReply *reply);
    void process_login(QNetworkReply *reply);
    void process_captcha(QNetworkReply *reply);
    void process_transfer(QNetworkReply *reply);
    void process_profile(QNetworkReply *reply);
    void process_logout(QNetworkReply *reply);

signals:
    void console(const QString &message);
    void unlock_login();
    void captcha_mode(const QPixmap &captcha);
    void steamguard_mode();
    void listing_mode();
    void add_account(QString username);
    void remove_account(QString username);

};

#endif // LOGIN_H
