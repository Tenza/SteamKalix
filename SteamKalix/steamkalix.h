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

#ifndef STEAMKALIX_H
#define STEAMKALIX_H

#include <QMainWindow>
#include <QDesktopWidget>

#include "defines.h"
#include "output.h"
#include "login.h"
#include "threadmanager.h"
#include "settingsmanager.h"

namespace Ui
{
    class SteamKalix;
}

/**
 * @brief The SteamKalix class
 *      This class implements all the user interface interactions.
 *      It also saves and sets the information from the UI.
 * @date
 *      Created:  Filipe, 29 Dez 2013
 *      Modified: Filipe, 24 Mar 2014
 */
class SteamKalix : public QMainWindow
{
    Q_OBJECT

public_construct:
    explicit SteamKalix(QWidget *parent = 0);
    ~SteamKalix();

private_methods:
    void load_setup();
    void load_connections();
    void load_settings();
    void save_settings();

    void validate_all();
    void validate_login();
    void validate_append();
    void validate_connectivity();

    void validate_empty();
    bool validate_new_account(const QString &username);
    bool validate_main_account(const QString &username);
    bool validate_secondary_account(const QString &username, const int &mode);

    void lock_login();

    void set_proxys(const QString &user);
    QStringList get_proxy(const QString &user);
    QVariantHash listings_options();
    void output(const QString &message, const int &verbose);

private_data_members:
    Ui::SteamKalix *ui;
    Login *login;
    ThreadManager *threads;

private_members:
    QString thread_id;

public slots:
    void console(const QString &message);

    void unlock_login();
    void captcha_mode(const QPixmap &captcha);
    void steamguard_mode();
    void listing_mode();

    void lock_listings();

    void add_account(QString username);
    void remove_account(QString username);

private slots:
    void on_btn_login_clicked();
    void on_btn_logout_clicked();
    void on_btn_accounts_remove_clicked();
    void on_cb_accounts_currentIndexChanged(int index);
    void on_cb_selected_account_currentIndexChanged(int index);

    void on_btn_add_clicked();
    void on_btn_remove_clicked();
    void on_btn_resume_clicked();
    void on_btn_pause_clicked();
    void on_table_listings_clicked(const QModelIndex &index);

    void on_btn_add_proxy_clicked();
    void on_btn_remove_proxy_clicked();
    void on_table_proxys_clicked(const QModelIndex &index);

    void on_cb_logging_clicked(bool checked);
    void on_rb_log_normal_clicked();
    void on_rb_log_essencial_clicked();
    void on_rb_log_debug_clicked();

    void on_tab_container_tabBarClicked(int index);
    void on_cb_disable_proxys_clicked();
    void on_cb_disable_direct_clicked();
    void on_cb_append_account_clicked();
};

#endif // STEAMKALIX_H
