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

#include "steamkalix.h"
#include "ui_steamkalix.h"

/**
 * @brief SteamKalix::SteamKalix
 *      Instanciates and connects objects to the UI.
 *      Loads UI settings.
 * @param parent
 * @date
 *      Created:  Filipe, 29 Dez 2013
 *      Modified: Filipe, 29 Mar 2014
 */
SteamKalix::SteamKalix(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SteamKalix),
    thread_id(Helper::get_thread_id())
{
    ui->setupUi(this);

    login = new Login(this);
    threads = new ThreadManager(this);

    load_setup();
    load_connections();
    load_settings();

    validate_all();
}

/**
 * @brief SteamKalix::~SteamKalix
 *      Saves settings and deletes the UI.
 *      Other resorses will be cleared by the parent object.
 * @date
 *      Created:  Filipe, 29 Dez 2013
 *      Modified: Filipe, 29 Mar 2014
 */
SteamKalix::~SteamKalix()
{
    save_settings();

    delete ui;
    ui = NULL;
}

/**
 * @brief SteamKalix::load_setup
 *      Setup the UI state.
 * @date
 *      Created:  Filipe, 29 Mar 2014
 *      Modified: Filipe, 29 Mar 2014
 */
void SteamKalix::load_setup()
{
    //Optimize log viewer
    ui->console->setMaximumBlockCount(5000);
    ui->console->setCenterOnScroll(true);

    //This sections will be unlocked by the login class.
    ui->group_captcha->setEnabled(false);
    ui->group_steamguard->setEnabled(false);
    ui->cb_remember_login->setChecked(true);

    //Create listing table headers
    QStringList table_headers;
    table_headers.append("ID");
    table_headers.append("Link");
    table_headers.append("Buy");
    table_headers.append("Price buy");
    table_headers.append("Sell");
    table_headers.append("Price sell");
    table_headers.append("Connections");
    table_headers.append("Refresh rate");
    table_headers.append("Timeout");
    table_headers.append("Delay");
    table_headers.append("Random delay");
    table_headers.append("Email");
    table_headers.append("Buy and pause");
    table_headers.append("Buy and pause number");
    table_headers.append("Buy and sell");

    //Configure listing table headers
    ui->table_listings->horizontalHeader()->setStretchLastSection(true);
    ui->table_listings->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table_listings->setColumnCount(table_headers.count());
    ui->table_listings->setHorizontalHeaderLabels(table_headers);
    ui->table_listings->setColumnHidden(2, true);
    ui->table_listings->setColumnHidden(3, true);
    ui->table_listings->setColumnHidden(4, true);
    ui->table_listings->setColumnHidden(5, true);
    ui->table_listings->setColumnHidden(6, true);
    ui->table_listings->setColumnHidden(7, true);
    ui->table_listings->setColumnHidden(8, true);
    ui->table_listings->setColumnHidden(9, true);
    ui->table_listings->setColumnHidden(10, true);
    ui->table_listings->setColumnHidden(11, true);
    ui->table_listings->setColumnHidden(12, true);
    ui->table_listings->setColumnHidden(13, true);
    ui->table_listings->setColumnHidden(14, true);

    //Create proxy table headers
    table_headers.clear();
    table_headers.append("IP");
    table_headers.append("Port");
    table_headers.append("Username");
    table_headers.append("Password");
    table_headers.append("Type");
    table_headers.append("Account");

    //Configure proxy table headers
    ui->table_proxys->horizontalHeader()->setStretchLastSection(false);
    ui->table_proxys->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_proxys->setColumnCount(table_headers.count());
    ui->table_proxys->setHorizontalHeaderLabels(table_headers);
    ui->table_proxys->setColumnHidden(2, true); //User
    ui->table_proxys->setColumnHidden(3, true); //Pass
    ui->table_proxys->setColumnHidden(4, true); //Type
}

/**
 * @brief SteamKalix::load_connections
 *      Connects the UI objects with internal classes.
 * @date
 *      Created:  Filipe, 29 Mar 2014
 *      Modified: Filipe, 7 Jun 2014
 */
void SteamKalix::load_connections()
{
    //Connect signal/slot with the Login class.
    connect(login, SIGNAL(console(const QString&)), this, SLOT(console(const QString&)));
    connect(login, SIGNAL(unlock_login()), this, SLOT(unlock_login()));
    connect(login, SIGNAL(captcha_mode(const QPixmap&)), this, SLOT(captcha_mode(const QPixmap&)));
    connect(login, SIGNAL(steamguard_mode()), this, SLOT(steamguard_mode()));
    connect(login, SIGNAL(listing_mode()), this, SLOT(listing_mode()));
    connect(login, SIGNAL(add_account(QString)), this, SLOT(add_account(QString)));
    connect(login, SIGNAL(remove_account(QString)), this, SLOT(remove_account(QString)));

    //Connect signal/slot with the ThreadManager class.
    connect(threads, SIGNAL(console(const QString&)), this, SLOT(console(const QString&)));
    connect(threads, SIGNAL(lock_listings()), this, SLOT(lock_listings()));   

    //Handle enter key on Login textboxs.
    connect(ui->txt_username, SIGNAL(returnPressed()), this, SLOT(on_btn_login_clicked()));
    connect(ui->txt_password, SIGNAL(returnPressed()), this, SLOT(on_btn_login_clicked()));
    connect(ui->txt_captcha, SIGNAL(returnPressed()), this, SLOT(on_btn_login_clicked()));
    connect(ui->txt_guard_code, SIGNAL(returnPressed()), this, SLOT(on_btn_login_clicked()));
    connect(ui->txt_guard_name, SIGNAL(returnPressed()), this, SLOT(on_btn_login_clicked()));
}

/**
 * @brief SteamKalix::load_settings
 *      Loads UI settings.
 * @date
 *      Created:  Filipe, 9 Mar 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void SteamKalix::load_settings()
{
    //Window

    this->resize(SettingsManager::read("Window/WindowWidth", 800).toInt(),
                 SettingsManager::read("Window/WindowHeight", 600).toInt());

    this->move(SettingsManager::read("Window/WindowPosition",
               QApplication::desktop()->screen()->rect().center() - this->rect().center()).toPoint());

    if(SettingsManager::read("Window/WindowMaximized", false).toBool())
    {
        this->showMaximized();
    }

    ui->splitter->restoreState(SettingsManager::read("Window/Splitter").toByteArray());

    ui->tab_container->setCurrentIndex(SettingsManager::read("Window/Tab", 1).toInt());

    //Login

    for(int i = 0; i < SettingsManager::read("LoginTab/AccountCount", 0).toInt(); i++)
    {
        QString account_number = QString::number(i + 1);

        QVariantHash account_options;
        account_options.insert("username", SettingsManager::read("LoginTab/Username" + account_number, "").toString());
        account_options.insert("password", SettingsManager::read("LoginTab/Password" + account_number, "").toString());
        account_options.insert("remember_login", SettingsManager::read("LoginTab/RememberLogin" + account_number, true).toBool());
        account_options.insert("append_account", SettingsManager::read("LoginTab/AppendAccount" + account_number, false).toBool());
        account_options.insert("disable_proxys", SettingsManager::read("LoginTab/DisableProxys" + account_number, false).toBool());
        account_options.insert("disable_direct", SettingsManager::read("LoginTab/DisableDirect" + account_number, false).toBool());
        account_options.insert("disable_cookies", SettingsManager::read("LoginTab/DisableCookies" + account_number, false).toBool());

        ui->cb_accounts->addItem(account_options.value("username").toString(), account_options);
        ui->cb_proxy_account->addItem(account_options.value("username").toString());
    }

    //Listings

    ui->txt_link->setText(SettingsManager::read("ListingsTab/Link", "").toString());
    ui->cb_buy->setChecked(SettingsManager::read("ListingsTab/Buy", false).toBool());
    ui->sb_price_buy->setValue(SettingsManager::read("ListingsTab/PriceBuy", 0.00).toDouble());
    ui->cb_sell->setChecked(SettingsManager::read("ListingsTab/Sell", false).toBool());
    ui->sb_price_sell->setValue(SettingsManager::read("ListingsTab/PriceSell", 0.00).toDouble());

    ui->sb_connections->setValue(SettingsManager::read("ListingsTab/Connections", 2).toInt());
    ui->sb_refresh->setValue(SettingsManager::read("ListingsTab/Refresh", 100).toInt());
    ui->sb_timeout->setValue(SettingsManager::read("ListingsTab/Timeout", 5000).toInt());
    ui->sb_delay->setValue(SettingsManager::read("ListingsTab/Delay", 0).toInt());
    ui->cb_delay_random->setChecked(SettingsManager::read("ListingsTab/DelayRandom", false).toBool());

    ui->cb_email->setChecked(SettingsManager::read("ListingsTab/Email", false).toBool());
    ui->cb_pause->setChecked(SettingsManager::read("ListingsTab/Pause", false).toBool());
    ui->sb_pause_number->setValue(SettingsManager::read("ListingsTab/PauseNumber", 1).toInt());
    ui->cb_resume->setChecked(SettingsManager::read("ListingsTab/Resume", false).toBool());

    //Proxys

    for(int i = 0; i < SettingsManager::read("ProxysTab/ProxyCount", 0).toInt(); i++)
    {
        QString proxy_number = QString::number(i + 1);

        ui->table_proxys->insertRow(i);
        ui->table_proxys->setItem(i, 0, new QTableWidgetItem(SettingsManager::read("ProxysTab/ProxyHost" + proxy_number, "").toString()));
        ui->table_proxys->setItem(i, 1, new QTableWidgetItem(SettingsManager::read("ProxysTab/ProxyPort" + proxy_number, "").toString()));
        ui->table_proxys->setItem(i, 2, new QTableWidgetItem(SettingsManager::read("ProxysTab/ProxyUser" + proxy_number, "").toString()));
        ui->table_proxys->setItem(i, 3, new QTableWidgetItem(SettingsManager::read("ProxysTab/ProxyPass" + proxy_number, "").toString()));
        ui->table_proxys->setItem(i, 4, new QTableWidgetItem(SettingsManager::read("ProxysTab/ProxyType" + proxy_number, "").toString()));
        ui->table_proxys->setItem(i, 5, new QTableWidgetItem(SettingsManager::read("ProxysTab/ProxyAccount" + proxy_number, "").toString()));
    }

    //Options

    ui->cb_logging->setChecked(SettingsManager::read("OptionsTab/Log", false).toBool());
    Output::set_logging(SettingsManager::read("OptionsTab/Log", false).toBool());

    int verbose = SettingsManager::read("OptionsTab/Verbose", 2).toInt();
    if(verbose <= 1)
    {
        ui->rb_log_essencial->setChecked(true);
        Output::set_verbose(1);
    }
    else if(verbose == 2)
    {
        ui->rb_log_normal->setChecked(true);
        Output::set_verbose(2);
    }
    else if(verbose >= 3)
    {
        ui->rb_log_debug->setChecked(true);
        Output::set_verbose(3);
    }
}

/**
 * @brief SteamKalix::save_settings
 *      Saves UI settings.
 * @date
 *      Created:  Filipe, 9 Mar 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void SteamKalix::save_settings()
{
    //Window

    //Only save position and size if not maximized, to preserve the last state.
    if(!this->isMaximized())
    {
        SettingsManager::write("Window/WindowWidth", this->width());
        SettingsManager::write("Window/WindowHeight", this->height());
        SettingsManager::write("Window/WindowPosition", this->pos());
    }

    SettingsManager::write("Window/WindowMaximized", this->isMaximized());

    SettingsManager::write("Window/Splitter", ui->splitter->saveState());

    SettingsManager::write("Window/Tab", ui->tab_container->currentIndex());

    //Login

    int top_items = 1; //Change for more top items
    SettingsManager::remove("LoginTab");
    SettingsManager::write("LoginTab/AccountCount", ui->cb_accounts->count() - top_items);

    for(int i = top_items; i < ui->cb_accounts->count(); i++)
    {
        QString account_number = QString::number((i + 1) - top_items);
        QVariantHash account_data = ui->cb_accounts->itemData(i).toHash();

        SettingsManager::write("LoginTab/Username" + account_number, account_data.value("username").toString());
        SettingsManager::write("LoginTab/Password" + account_number, account_data.value("password").toString());
        SettingsManager::write("LoginTab/RememberLogin" + account_number, account_data.value("remember_login").toBool());
        SettingsManager::write("LoginTab/AppendAccount" + account_number, account_data.value("append_account").toBool());
        SettingsManager::write("LoginTab/DisableProxys" + account_number, account_data.value("disable_proxys").toBool());
        SettingsManager::write("LoginTab/DisableDirect" + account_number, account_data.value("disable_direct").toBool());
        SettingsManager::write("LoginTab/DisableCookies" + account_number, account_data.value("disable_cookies").toBool());
    }

    //Listings

    SettingsManager::write("ListingsTab/Link", ui->txt_link->text());
    SettingsManager::write("ListingsTab/Buy", ui->cb_buy->isChecked());
    SettingsManager::write("ListingsTab/PriceBuy", ui->sb_price_buy->value());
    SettingsManager::write("ListingsTab/Sell", ui->cb_sell->isChecked());
    SettingsManager::write("ListingsTab/PriceSell", ui->sb_price_sell->value());

    SettingsManager::write("ListingsTab/Connections", ui->sb_connections->value());
    SettingsManager::write("ListingsTab/Refresh", ui->sb_refresh->value());
    SettingsManager::write("ListingsTab/Timeout", ui->sb_timeout->value());
    SettingsManager::write("ListingsTab/Delay", ui->sb_delay->value());
    SettingsManager::write("ListingsTab/DelayRandom", ui->cb_delay_random->isChecked());

    SettingsManager::write("ListingsTab/Email", ui->cb_email->isChecked());
    SettingsManager::write("ListingsTab/Pause", ui->cb_pause->isChecked());
    SettingsManager::write("ListingsTab/PauseNumber", ui->sb_pause_number->value());
    SettingsManager::write("ListingsTab/Resume", ui->cb_resume->isChecked());

    //Proxys

    SettingsManager::remove("ProxysTab");
    SettingsManager::write("ProxysTab/ProxyCount", ui->table_proxys->rowCount());

    for(int i = 0; i < ui->table_proxys->rowCount(); i++)
    {
        QString proxy_number = QString::number(i + 1);
        SettingsManager::write("ProxysTab/ProxyHost" + proxy_number, ui->table_proxys->item(i, 0)->text());
        SettingsManager::write("ProxysTab/ProxyPort" + proxy_number, ui->table_proxys->item(i, 1)->text());
        SettingsManager::write("ProxysTab/ProxyUser" + proxy_number, ui->table_proxys->item(i, 2)->text());
        SettingsManager::write("ProxysTab/ProxyPass" + proxy_number, ui->table_proxys->item(i, 3)->text());
        SettingsManager::write("ProxysTab/ProxyType" + proxy_number, ui->table_proxys->item(i, 4)->text());
        SettingsManager::write("ProxysTab/ProxyAccount" + proxy_number, ui->table_proxys->item(i, 5)->text());
    }

    //Options

    SettingsManager::write("OptionsTab/Log", ui->cb_logging->isChecked());
    SettingsManager::write("OptionsTab/Verbose", ui->rb_log_essencial->isChecked() ? 1 :
                                                 ui->rb_log_normal->isChecked() ? 2 :
                                                 ui->rb_log_debug->isChecked() ? 3 : 2);
}

/*************************************************************************************/
/*                                      LOGIN                                        */
/*************************************************************************************/
/**
 * @brief SteamKalix::on_btn_login_clicked
 *      Starts the login process.
 * @remarks Proxy
 *      The proxy is for the main account. It only works if the direct option is enabled
 *      and the proxys option is disabled. The other 3 options, will make direct connections.
 * @date
 *      Created:  Filipe, 2 Jan 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void SteamKalix::on_btn_login_clicked()
{
    if(ui->btn_login->isEnabled())
    {
        if(!validate_new_account(ui->txt_username->text()))
        {
            if(!validate_main_account(ui->txt_username->text()))
            {
                if(!validate_secondary_account(ui->txt_username->text(), 1))
                {
                    lock_login();

                    QVariantHash new_account;
                    QStringList new_proxy = get_proxy(ui->txt_username->text());
                    if(ui->cb_disable_direct->isChecked() && !ui->cb_disable_proxys->isChecked() &&  !new_proxy.isEmpty())
                    {
                        new_account.insert("proxy_hostname", new_proxy.at(0));
                        new_account.insert("proxy_hostport", new_proxy.at(1));
                        new_account.insert("proxy_username", new_proxy.at(2));
                        new_account.insert("proxy_password", new_proxy.at(3));
                        new_account.insert("proxy_type", new_proxy.at(4));
                    }

                    new_account.insert("username", ui->txt_username->text());
                    new_account.insert("password", ui->txt_password->text());
                    new_account.insert("captcha", ui->txt_captcha->text());
                    new_account.insert("guard_code", ui->txt_guard_code->text());
                    new_account.insert("guard_name", ui->txt_guard_name->text());
                    new_account.insert("remember_login", ui->cb_remember_login->isChecked());

                    login->do_login(new_account);
                }
                else if(!validate_secondary_account(ui->txt_username->text(), 2)) //The user already exists, but does not exist in the selected account.
                {
                    output("Account already logged, added to the list.", 1);
                    add_account("AlreadyLogged");
                }
                else
                {
                    output("This is already a secondary account.", 1);
                }
            }
            else
            {
                output("This is already a main account.", 1);
            }
        }
        else
        {
            output("This is not a new login, '" + ui->txt_username->text() + "' is already listed.", 1);
        }
    }
}

/**
 * @brief SteamKalix::on_btn_logout_clicked
 *      Starts the logout process.
 * @remarks
 *      This is a multi logout. It works by making multiple calls to the do_logout.
 *      This function is called by the remove_account to make more logout calls.
 *      Also, in order to avoid heap variables, we use the userdata inside the comboboxes.
 * @date
 *      Created:  Filipe, 2 Jan 2014
 *      Modified: Filipe, 15 Jun 2014
 */
void SteamKalix::on_btn_logout_clicked()
{
    //Send warning here:
    //"There are listings using this account, if you logout they will be removed. Do you wish to proceed?"

    lock_login();

    //Remove listings of this user here.
    if(ui->table_listings->rowCount() > 0)
    {

    }

    //Set cb_selected_account to the same main account that cb_accounts has and pull its data.
    //Then setCurrentIndex, because we might come here again if there are appended accounts.
    QStringList account = ui->cb_selected_account->currentData().toStringList();
    if(account.value(0) != ui->cb_accounts->currentText())
    {
        for(int i = 0; i < ui->cb_selected_account->count(); i++)
        {
            account = ui->cb_selected_account->itemData(i).toStringList();
            if(account.value(0) == ui->cb_accounts->currentText())
            {
                ui->cb_selected_account->setCurrentIndex(i);
                break;
            }
        }
    }

    if(!account.isEmpty())
    {
        //Discard secondary accounts in use by others currently logged accounts.
        for(int i = 1; i < account.size(); i++)
        {
            if(!validate_secondary_account(account.at(i), 3)) //|| account.at(i) == "Empty")
            {
                account.removeAt(i);
                i--;
            }
        }

        //Set data without already in use accounts, and logout the last one.
        ui->cb_selected_account->setItemData(ui->cb_selected_account->currentIndex(), account);

        login->do_logout(account.last());
    }
}

/**
 * @brief SteamKalix::on_cb_accounts_currentIndexChanged
 *      Fills the UI elements with the saved account data.
 * @param username
 *      The username in the combobox.
 * @date
 *      Created:  Filipe, 7 Jun 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void SteamKalix::on_cb_accounts_currentIndexChanged(int index)
{
    if(index == 0)
    {
        //New Login.
        ui->txt_username->clear();
        ui->txt_password->clear();
        ui->txt_username->setEnabled(true);
        ui->txt_password->setEnabled(true);
        ui->cb_remember_login->setChecked(true);
        ui->cb_append_account->setChecked(false);
        ui->cb_disable_proxys->setChecked(false);
        ui->cb_disable_direct->setChecked(false);
        ui->cb_disable_cookies->setChecked(false);
    }
    else
    {
        //Load saved data.
        ui->txt_username->setEnabled(false);
        ui->txt_password->setEnabled(false);
        QVariantHash account_data = ui->cb_accounts->currentData().toHash();
        ui->txt_username->setText(account_data.value("username").toString());
        ui->txt_password->setText(account_data.value("password").toString());
        ui->cb_remember_login->setChecked(account_data.value("remember_login").toBool());
        ui->cb_append_account->setChecked(account_data.value("append_account").toBool());
        ui->cb_disable_proxys->setChecked(account_data.value("disable_proxys").toBool());
        ui->cb_disable_direct->setChecked(account_data.value("disable_direct").toBool());
        ui->cb_disable_cookies->setChecked(account_data.value("disable_cookies").toBool());
        ui->cb_proxy_account->setCurrentText(ui->cb_accounts->currentText());
    }

    validate_all();
}

/**
 * @brief SteamKalix::on_btn_accounts_remove_clicked
 *      Removes the selected account from the settings.
 * @date
 *      Created:  Filipe, 7 Jun 2014
 *      Modified: Filipe, 15 Jun 2014
 */
void SteamKalix::on_btn_accounts_remove_clicked()
{
    ui->cb_proxy_account->removeItem(ui->cb_proxy_account->findText(ui->cb_accounts->currentText()));
    ui->cb_accounts->removeItem(ui->cb_accounts->currentIndex());
    ui->cb_accounts->setCurrentIndex(0);
    on_cb_accounts_currentIndexChanged(0);
}

/**
 * @brief SteamKalix::on_cb_disable_proxys_toggled
 *      If the account secondary, we have to toggle the connectivity buttons.
 *      It is not possible to completly desactivate connectivity on a secondary account.
 * @param checked
 *      State of the box.
 * @date
 *      Created:  Filipe, 3 Sep 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void SteamKalix::on_cb_disable_proxys_clicked()
{
    if(ui->cb_append_account->isChecked())
    {
        if(ui->cb_disable_proxys->isChecked())
        {
            ui->cb_disable_direct->setEnabled(false);
            ui->cb_disable_direct->setChecked(false);
        }
        else
        {
            ui->cb_disable_direct->setEnabled(true);
        }
    }
}

/**
 * @brief SteamKalix::on_cb_disable_direct_toggled
 *      If the account secondary, we have to toggle the connectivity buttons.
 *      It is not possible to completly desactivate connectivity on a secondary account.
 * @param checked
 *      State of the box.
 * @date
 *      Created:  Filipe, 3 Sep 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void SteamKalix::on_cb_disable_direct_clicked()
{
    if(ui->cb_append_account->isChecked())
    {
        if(ui->cb_disable_direct->isChecked())
        {
            ui->cb_disable_proxys->setEnabled(false);
            ui->cb_disable_proxys->setChecked(false);
        }
        else
        {
            ui->cb_disable_proxys->setEnabled(true);
        }
    }
}

/**
 * @brief SteamKalix::on_cb_append_account_clicked
 *      We need to validate the conectivity options again
 *      because it should not be possible for a secondary account to
 *      have all the connections disabled.
 * @date
 *      Created:  Filipe, 3 Sep 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void SteamKalix::on_cb_append_account_clicked()
{
    validate_connectivity();
}

/**
 * @brief SteamKalix::on_cb_selected_account_currentIndexChanged
 *      Revalidate the append option.
 *      The current username, might already be on one main, but perhaps not in another.
 * @param index
 *      Index of the item. Not used.
 * @date
 *      Created:  Filipe, 7 Jun 2014
 *      Modified: Filipe, 7 Jun 2014
 */
void SteamKalix::on_cb_selected_account_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    validate_all();
}

/**
 * @brief SteamKalix::on_tab_container_tabBarClicked
 *      Add a temporary item to the proxys user combobox. This is used for new logins that
 *      want to use a proxy to connect but dont already have an entry on the users table.
 *      Alternatively, I could add a wildcard, but this is cleaner and more perceptable.
 * @param index
 *      Index of the tab.
 * @date
 *      Created:  Filipe, 3 Sep 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void SteamKalix::on_tab_container_tabBarClicked(int index)
{
    bool exists = false;

    if(index == 0 || index == 1)
    {
        if(ui->cb_accounts->currentIndex() == 0 && ui->txt_username->text() != "") //New Login?
        {
            for(int i = 0; i < ui->cb_proxy_account->count(); i++)
            {
                if(ui->cb_proxy_account->itemText(i) == ui->txt_username->text())
                {
                    exists = true;
                    break;
                }
            }

            if(exists && index == 0) //LoginTab, remove item.
            {
                ui->cb_proxy_account->removeItem(ui->cb_proxy_account->findText(ui->txt_username->text()));
            }
            else if(!exists && index == 1) //ProxysTab, add item.
            {
                 ui->cb_proxy_account->addItem(ui->txt_username->text());
            }
        }
    }
}

/*************************************************************************************/
/*                                 Login Validations                                 */
/*************************************************************************************/

/**
 * @brief SteamKalix::validate_all
 *      Performes all the UI validations.
 *      The order of operations matter!
 * @date
 *      Created:  Filipe, 7 Jun 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void SteamKalix::validate_all()
{
    validate_login();

    //If the login is disabled, just load the settings.
    if(ui->btn_login->isEnabled())
    {
        validate_append();
        validate_connectivity();
    }
}

/**
 * @brief SteamKalix::validate_login
 *      Validates the login and remove buttons.
 * @date
 *      Created:  Filipe, 7 Jun 2014
 *      Modified: Filipe, 7 Jun 2014
 */
void SteamKalix::validate_login()
{
    if(validate_main_account(ui->cb_accounts->currentText()))
    {
        ui->btn_login->setEnabled(false);
        ui->btn_logout->setEnabled(true);
    }
    else
    {
        if(validate_secondary_account(ui->cb_accounts->currentText(), 2))
        {
            ui->btn_login->setEnabled(false);
        }
        else
        {
            ui->btn_login->setEnabled(true);
        }

        ui->btn_logout->setEnabled(false);
    }

    if(ui->cb_accounts->currentIndex() > 1)
    {
        ui->btn_accounts_remove->setEnabled(true);
    }
    else
    {
        ui->btn_accounts_remove->setEnabled(false);
    }
}

/**
 * @brief SteamKalix::validate_append
 *      Only allows to set the append options if there are logged accounts.
 * @date
 *      Created:  Filipe, 7 Jun 2014
 *      Modified: Filipe, 7 Jun 2014
 */
void SteamKalix::validate_append()
{
    if(ui->cb_selected_account->count() > 0)
    {
        if(validate_main_account(ui->cb_accounts->currentText()) ||
           validate_secondary_account(ui->cb_accounts->currentText(), 2))
        {
            ui->cb_append_account->setChecked(false);
            ui->cb_append_account->setEnabled(false);
        }
        else
        {
            if(validate_secondary_account(ui->cb_accounts->currentText(), 1))
            {
                ui->cb_append_account->setChecked(true);
                ui->cb_append_account->setEnabled(false);
            }
            else
            {
                ui->cb_append_account->setEnabled(true);
            }
        }
    }
    else //There is no account to append to.
    {
        ui->cb_append_account->setChecked(false);
        ui->cb_append_account->setEnabled(false);
    }
}

/**
 * @brief SteamKalix::validate_connectivity
 *      Only allows to set the proxy options if proxys exist.
 *      Prevents this secondary account to be connectionless.
 * @date
 *      Created:  Filipe, 7 Jun 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void SteamKalix::validate_connectivity()
{
    ui->cb_disable_direct->setEnabled(true);
    ui->cb_disable_proxys->setEnabled(true);

    if(ui->cb_append_account->isEnabled() && ui->cb_append_account->isChecked())
    {
        if(ui->cb_disable_direct->isChecked() && ui->cb_disable_proxys->isChecked())
        {
            ui->cb_disable_direct->setChecked(false);
            ui->cb_disable_proxys->setChecked(false);
        }
        else if (ui->cb_disable_proxys->isChecked())
        {
            ui->cb_disable_direct->setChecked(false);
            ui->cb_disable_direct->setEnabled(false);
        }
        else if(ui->cb_disable_direct->isChecked())
        {
            ui->cb_disable_proxys->setChecked(false);
            ui->cb_disable_proxys->setEnabled(false);
        }
    }
}

/**
 * @brief SteamKalix::validate_main_account
 *      Is the username passed a main account?
 * @param username
 *      The username to find.
 * @return
 *      The answer to the question.
 * @date
 *      Created:  Filipe, 7 Jun 2014
 *      Modified: Filipe, 7 Jun 2014
 */
bool SteamKalix::validate_main_account(const QString &username)
{
    bool result = false;

    for(int i = 0; i < ui->cb_selected_account->count(); i++)
    {
        QStringList account = ui->cb_selected_account->itemData(i).toStringList();

        if(account.at(0) == username) //Is already a main account
        {
            result = true;
            break;
        }
    }

    return result;
}

/**
 * @brief SteamKalix::validate_secondary_account
 *      Is the username passed a secondary account?
 * @param username
 *      The username to find.
 * @param mode
 *      Mode 1: Find if username exists at all.
 *      Mode 2: Find if username exists in currently selected.
 *      Mode 3: Find if username exists ONLY in currently selected.
 * @return
 *      The answer to the question.
 * @date
 *      Created:  Filipe, 7 Jun 2014
 *      Modified: Filipe, 7 Jun 2014
 */
bool SteamKalix::validate_secondary_account(const QString &username, const int &mode)
{
    bool result = false;

    if(mode == 1)
    {
        for(int i = 0; i < ui->cb_selected_account->count(); i++)
        {
            QStringList account = ui->cb_selected_account->itemData(i).toStringList();

            for(int j = 1; j < account.size(); j++)
            {
                if(account.at(j) == username)
                {
                    result = true;
                    break;
                }
            }

            if(result)
            {
                break;
            }
        }
    }
    else if(mode == 2)
    {
        QStringList account = ui->cb_selected_account->currentData().toStringList();

        for(int j = 1; j < account.size(); j++)
        {
            if(account.at(j) == username)
            {
                result = true;
                break;
            }
        }
    }
    else if(mode == 3)
    {
        if(validate_secondary_account(username, 2))
        {
            result = true;
            int count = 0;
            for(int i = 0; i < ui->cb_selected_account->count(); i++)
            {
                QStringList account = ui->cb_selected_account->itemData(i).toStringList();

                for(int j = 1; j < account.size(); j++)
                {
                    if(account.at(j) == username)
                    {
                        count++;
                        break;
                    }
                }

                if(count == 2)
                {
                    result = false;
                    break;
                }
            }
        }
    }

    return result;
}

/**
 * @brief SteamKalix::validate_new_account
 *      Is this username really new?
 * @param username
 *      The username to find.
 * @return
 *      The answer to the question.
 * @date
 *      Created:  Filipe, 7 Jun 2014
 *      Modified: Filipe, 7 Jun 2014
 */
bool SteamKalix::validate_new_account(const QString &username)
{
    bool result = false;

    if(ui->cb_accounts->currentIndex() == 0)
    {
        for(int i = 0; i < ui->cb_accounts->count(); i++)
        {
            if(ui->cb_accounts->itemText(i) == username)
            {
                result = true;
                break;
            }
        }
    }

    return result;
}

/*************************************************************************************/
/*                                    Proxys                                         */
/*************************************************************************************/

/**
 * @brief SteamKalix::on_btn_add_proxy_clicked
 *      Add a proxy to the list.
 * @date
 *      Created:  Filipe, 5 Jun 2014
 *      Modified: Filipe, 22 Jun 2014
 */
void SteamKalix::on_btn_add_proxy_clicked()
{
    int rowcount = ui->table_proxys->rowCount();
    ui->table_proxys->insertRow(rowcount);
    ui->table_proxys->setItem(rowcount, 0, new QTableWidgetItem(ui->txt_proxy_hostname->text()));
    ui->table_proxys->setItem(rowcount, 1, new QTableWidgetItem(ui->sb_proxy_hostport->text()));
    ui->table_proxys->setItem(rowcount, 2, new QTableWidgetItem(ui->txt_proxy_username->text()));
    ui->table_proxys->setItem(rowcount, 3, new QTableWidgetItem(ui->txt_proxy_password->text()));
    ui->table_proxys->setItem(rowcount, 4, new QTableWidgetItem(ui->cb_proxy_type->currentText()));
    ui->table_proxys->setItem(rowcount, 5, new QTableWidgetItem(ui->cb_proxy_account->currentText()));

    ui->table_proxys->selectRow(ui->table_proxys->rowCount() - 1);

    QNetworkProxy proxy;
    proxy.setHostName(ui->txt_proxy_hostname->text());
    proxy.setPort(ui->sb_proxy_hostport->text().toInt());
    proxy.setUser(ui->txt_proxy_username->text());
    proxy.setPassword(ui->txt_proxy_password->text());
    proxy.setType(Helper::proxy_type(ui->cb_proxy_type->currentText()));

    NetworkManager::add_proxy(ui->cb_proxy_account->currentText(), proxy);
}

/**
 * @brief SteamKalix::on_btn_remove_proxy_clicked
 *      Removes a proxy from the list.
 * @date
 *      Created:  Filipe, 5 Jun 2014
 *      Modified: Filipe, 22 Jun 2014
 */
void SteamKalix::on_btn_remove_proxy_clicked()
{
    QModelIndexList selected_rows = ui->table_proxys->selectionModel()->selection().indexes();

    if(selected_rows.count() > 0)
    {
        QNetworkProxy proxy;
        proxy.setHostName(selected_rows.at(0).data().toString());
        proxy.setPort(selected_rows.at(1).data().toString().toInt());
        proxy.setUser(selected_rows.at(2).data().toString());
        proxy.setPassword(selected_rows.at(3).data().toString());
        proxy.setType(Helper::proxy_type(selected_rows.at(4).data().toString()));

        NetworkManager::remove_proxy(selected_rows.at(5).data().toString(), proxy);

        ui->table_proxys->removeRow(selected_rows.at(0).row());
    }
}

/**
 * @brief SteamKalix::on_table_proxys_clicked
 *      Updates the UI with the settings of the current proxy.
 * @param index
 *      Index of the clicked items.
 * @date
 *      Created:  Filipe, 5 Jun 2014
 *      Modified: Filipe, 5 Jun 2014
 */
void SteamKalix::on_table_proxys_clicked(const QModelIndex &index)
{
    int row = index.row();

    ui->txt_proxy_hostname->setText(index.sibling(row, 0).data().toString());
    ui->sb_proxy_hostport->setValue(index.sibling(row, 1).data().toInt());
    ui->txt_proxy_username->setText(index.sibling(row, 2).data().toString());
    ui->txt_proxy_password->setText(index.sibling(row, 3).data().toString());
    ui->cb_proxy_type->setCurrentText(index.sibling(row, 4).data().toString());
    ui->cb_proxy_account->setCurrentText(index.sibling(row, 5).data().toString());
}

/**
 * @brief SteamKalix::set_proxys
 *      Builds the proxy list to be passed to the network user static data.
 * @param user
 *      The user to add the proxys.
 * @date
 *      Created:  Filipe, 22 Jun 2014
 *      Modified: Filipe, 22 Jun 2014
 */
void SteamKalix::set_proxys(const QString &user)
{
    QList<QNetworkProxy> proxys;

    for(int i = 0; i < ui->table_proxys->rowCount(); i++)
    {
        if(ui->table_proxys->item(i, 5)->text() == user)
        {
            QNetworkProxy proxy;
            proxy.setHostName(ui->table_proxys->item(i, 0)->text());
            proxy.setPort(ui->table_proxys->item(i, 1)->text().toInt());
            proxy.setUser(ui->table_proxys->item(i, 2)->text());
            proxy.setPassword(ui->table_proxys->item(i, 3)->text());
            proxy.setType(Helper::proxy_type(ui->table_proxys->item(i, 4)->text()));

            proxys.append(proxy);
        }
    }

    NetworkManager::save_proxys(user, proxys);
}

/**
 * @brief SteamKalix::get_proxy
 *      Gets the main proxy for this user.
 *      If nothing is set, a direct connection will be added by the login class.
 * @param user
 *      The user to add the proxy.
 * @return
 *      The proxy to be used.
 * @date
 *      Created:  Filipe, 22 Jun 2014
 *      Modified: Filipe, 3 Sep 2014
 */
QStringList SteamKalix::get_proxy(const QString &user)
{
    QStringList proxy;

    for(int i = 0; i < ui->table_proxys->rowCount(); i++)
    {
        if(ui->table_proxys->item(i, 5)->text() == user)
        {
            proxy.append(ui->table_proxys->item(i, 0)->text());
            proxy.append(ui->table_proxys->item(i, 1)->text());
            proxy.append(ui->table_proxys->item(i, 2)->text());
            proxy.append(ui->table_proxys->item(i, 3)->text());
            proxy.append(ui->table_proxys->item(i, 4)->text());
            break;
        }
    }

    return proxy;
}

/*************************************************************************************/
/*                                    LISTINGS                                       */
/*************************************************************************************/

/**
 * @brief SteamKalix::on_btn_add_clicked
 *      Adds a new listing.
 * @remarks
 *      Calls a method to create a new listning.
 *      Creates an entry in the table.
 * @date
 *      Created:  Filipe, 24 Mar 2014
 *      Modified: Filipe, 31 Mar 2014
 */
void SteamKalix::on_btn_add_clicked()
{
    QVariantHash options = listings_options();

    if(!options.isEmpty())
    {
//        int id = threads->listing_add(options);
        output("This functionality was removed. You are welcome to fork and use this code base.", 1);

        int rowcount = ui->table_listings->rowCount();
        ui->table_listings->insertRow(rowcount);
//        ui->table_listings->setItem(rowcount, 0, new QTableWidgetItem(QString::number(id)));
        ui->table_listings->setItem(rowcount, 0, new QTableWidgetItem(QString::number(Helper::random_number(0, 1000))));
        ui->table_listings->setItem(rowcount, 1, new QTableWidgetItem(ui->txt_link->text()));
        ui->table_listings->setItem(rowcount, 2, new QTableWidgetItem(ui->cb_buy->isChecked() ? "true" : "false"));
        ui->table_listings->setItem(rowcount, 3, new QTableWidgetItem(ui->sb_price_buy->text()));
        ui->table_listings->setItem(rowcount, 4, new QTableWidgetItem(ui->cb_sell->isChecked() ? "true" : "false"));
        ui->table_listings->setItem(rowcount, 5, new QTableWidgetItem(ui->sb_price_sell->text()));

        ui->table_listings->setItem(rowcount, 6, new QTableWidgetItem(ui->sb_connections->text()));
        ui->table_listings->setItem(rowcount, 7, new QTableWidgetItem(ui->sb_refresh->text()));
        ui->table_listings->setItem(rowcount, 8, new QTableWidgetItem(ui->sb_timeout->text()));
        ui->table_listings->setItem(rowcount, 9, new QTableWidgetItem(ui->sb_delay->text()));
        ui->table_listings->setItem(rowcount, 10, new QTableWidgetItem(ui->cb_delay_random->isChecked() ? "true" : "false"));

        ui->table_listings->setItem(rowcount, 11, new QTableWidgetItem(ui->cb_email->isChecked() ? "true" : "false"));
        ui->table_listings->setItem(rowcount, 12, new QTableWidgetItem(ui->cb_pause->isChecked() ? "true" : "false"));
        ui->table_listings->setItem(rowcount, 13, new QTableWidgetItem(ui->sb_pause_number->text()));
        ui->table_listings->setItem(rowcount, 14, new QTableWidgetItem(ui->cb_resume->isChecked() ? "true" : "false"));

        ui->table_listings->selectRow(ui->table_listings->rowCount() - 1);
    }
    else
    {
        output("There isn’t an account with active connections to start listening items.", 1);
    }
}

/**
 * @brief SteamKalix::on_btn_remove_clicked
 *      Removes a listing.
 * @remarks
 *      The values are calculated here instead of on_table_listings_clicked
 *      to enable clicking multiple times on the remove without selection.
 * @date
 *      Created:  Filipe, 24 Mar 2014
 *      Modified: Filipe, 31 Mar 2014
 */
void SteamKalix::on_btn_remove_clicked()
{
    QModelIndexList selected_rows = ui->table_listings->selectionModel()->selection().indexes();

    if(selected_rows.count() > 0)
    {
//        threads->listing_remove(selected_rows.at(0).data().toInt());
        ui->table_listings->removeRow(selected_rows.at(0).row());
    }
}

/**
 * @brief SteamKalix::on_btn_resume_clicked
 *      Resumes the timer by calling the signal from the listings class.
 * @date
 *      Created:  Filipe, 5 Apr 2014
 *      Modified: Filipe, 5 Apr 2014
 */
void SteamKalix::on_btn_resume_clicked()
{
    QModelIndexList selected_rows = ui->table_listings->selectionModel()->selection().indexes();

    if(selected_rows.count() > 0)
    {
        threads->listing_start(selected_rows.at(0).data().toInt());
    }
}

/**
 * @brief SteamKalix::on_btn_resume_clicked
 *      Pauses the timer by calling the signal from the listings class.
 * @date
 *      Created:  Filipe, 5 Apr 2014
 *      Modified: Filipe, 5 Apr 2014
 */
void SteamKalix::on_btn_pause_clicked()
{
    QModelIndexList selected_rows = ui->table_listings->selectionModel()->selection().indexes();

    if(selected_rows.count() > 0)
    {
        threads->listing_stop(selected_rows.at(0).data().toInt());
    }
}

/**
 * @brief SteamKalix::on_table_listings_clicked
 *      Updates the UI with the settings of the current thread.
 * @param index
 *      Index of the clicked items.
 * @date
 *      Created:  Filipe, 5 Apr 2014
 *      Modified: Filipe, 5 Apr 2014
 */
void SteamKalix::on_table_listings_clicked(const QModelIndex &index)
{
    int row = index.row();

    ui->txt_link->setText(index.sibling(row, 1).data().toString());
    ui->cb_buy->setChecked(index.sibling(row, 2).data().toBool());
    ui->sb_price_buy->setValue(index.sibling(row, 3).data().toString().replace(",", ".").toDouble());
    ui->cb_sell->setChecked(index.sibling(row, 4).data().toBool());
    ui->sb_price_sell->setValue(index.sibling(row, 5).data().toString().replace(",", ".").toDouble());

    ui->sb_connections->setValue(index.sibling(row, 6).data().toInt());
    ui->sb_refresh->setValue(index.sibling(row, 7).data().toInt());
    ui->sb_timeout->setValue(index.sibling(row, 8).data().toInt());
    ui->sb_delay->setValue(index.sibling(row, 9).data().toInt());
    ui->cb_delay_random->setChecked(index.sibling(row, 10).data().toBool());

    ui->cb_email->setChecked(index.sibling(row, 11).data().toBool());
    ui->cb_pause->setChecked(index.sibling(row, 12).data().toBool());
    ui->sb_pause_number->setValue(index.sibling(row, 13).data().toInt());
    ui->cb_resume->setChecked(index.sibling(row, 14).data().toBool());
}

/**
 * @brief SteamKalix::listings_options
 *      Creates a QVariantHash with the settings from the UI.
 * @return
 *      The constructed map.
 * @remarks Disable main from making listing requests
 *      If both options are selected, and this is a main account (verifications make sure of that)
 *      we can remove the main account from making any connections on the listing class.
 *      If the user tries to add a listing, we warn him that it is not possible.
 * @date
 *      Created:  Filipe, 5 Apr 2014
 *      Modified: Filipe, 3 Sep 2014
 */
QVariantHash SteamKalix::listings_options()
{
    QVariantHash options;
    QStringList accounts = ui->cb_selected_account->currentData().toStringList();
    QString main_account = ui->cb_selected_account->currentData().toStringList().value(0, "");
    QVariantHash account_options = ui->cb_accounts->itemData(ui->cb_accounts->findText(main_account)).toHash();

    if(account_options.value("disable_proxys").toBool() && account_options.value("disable_direct").toBool() && !accounts.isEmpty())
    {
        accounts.removeFirst();
    }

    if(!accounts.isEmpty()) //There is no account to make the connections.
    {
        if(account_options.value("disable_cookies").toBool()) //Starts this account without setting the cookies.
        {
            options.insert("cookies", false);
        }
        else
        {
            options.insert("cookies", true);
        }

        options.insert("accounts", accounts);
        options.insert("link", ui->txt_link->text());
        options.insert("buy", ui->cb_buy->isChecked());
        options.insert("price_buy", ui->sb_price_buy->value());
        options.insert("sell", ui->cb_sell->isChecked());
        options.insert("price_sell", ui->sb_price_sell->value());
        options.insert("connections", ui->sb_connections->value());
        options.insert("refresh", ui->sb_refresh->value());
        options.insert("timeout", ui->sb_timeout->value());
        options.insert("delay", ui->sb_delay->value());
        options.insert("delay_random", ui->cb_delay_random->isChecked());
        options.insert("email", ui->cb_email->isChecked());
        options.insert("pause", ui->cb_pause->isChecked());
        options.insert("pause_number", ui->sb_pause_number->value());
        options.insert("resume", ui->cb_resume->isChecked());
        options.insert("username", main_account);
    }

    return options;
}

/*************************************************************************************/
/*                                     OPTIONS                                       */
/*************************************************************************************/

/**
 * @brief SteamKalix::Options
 *      The following functions that interact with static elements in order
 *      to control the output of the messages.
 * @date
 *      Created:  Filipe, 24 Jun 2014
 *      Modified: Filipe, 24 Jun 2014
 */
void SteamKalix::on_cb_logging_clicked(bool checked)
{
    if(checked)
    {
        Output::set_logging(true);
        output("Logging Enabled", 1);
    }
    else
    {
        Output::set_logging(false);
        output("Logging Disabled", 1);
    }
}

void SteamKalix::on_rb_log_essencial_clicked()
{
    Output::set_verbose(1);
    output("Verbose level changed to Essencial", 1);
}

void SteamKalix::on_rb_log_normal_clicked()
{
    Output::set_verbose(2);
    output("Verbose level changed to Normal", 1);
}

void SteamKalix::on_rb_log_debug_clicked()
{
    Output::set_verbose(3);
    output("Verbose level changed to Debug", 1);
}

/*************************************************************************************/
/*                                  SIGNAL/SLOTS                                     */
/*************************************************************************************/

/**
 * @brief SteamKalix::SLOTS::General
 *      The following functions are slots of general use.
 *      They are used to interact with UI elements from other objects/threads.
 * @date
 *      Created:  Filipe, 3 Jan 2014
 *      Modified: Filipe, 3 Jan 2014
 */
void SteamKalix::console(const QString &message)
{
    ui->console->appendHtml(message);
}

/**
 * @brief SteamKalix::SLOTS::Login
 *      The following functions are slots for the Login class.
 *      They are used to interact with UI elements from the Login class.
 * @remarks
 *      The unlock functions are used by the login in case of error.
 *      On success the add_account or remove_account are called.
 * @date
 *      Created:  Filipe, 3 Jan 2014
 *      Modified: Filipe, 10 Mar 2014
 */
void SteamKalix::lock_login()
{
    ui->tab_login->setEnabled(false);
    ui->cb_selected_account->setEnabled(false);
}

void SteamKalix::unlock_login()
{
    ui->tab_login->setEnabled(true);
    ui->cb_selected_account->setEnabled(true);
}

void SteamKalix::captcha_mode(const QPixmap &captcha)
{
    ui->group_captcha->setEnabled(true);
    ui->lbl_captcha->setPixmap(captcha);
    ui->txt_captcha->setText("");
    ui->txt_captcha->setFocus();
}

void SteamKalix::steamguard_mode()
{
    ui->group_steamguard->setEnabled(true);
    ui->txt_guard_code->setText("");
    ui->txt_guard_code->setFocus();
}

void SteamKalix::listing_mode()
{
    ui->tab_container->setCurrentWidget(ui->tab_listings);
}

/**
 * @brief SteamKalix::SLOTS::Listning
 *      The following functions are slots for the ThreadManager class.
 *      They are used to interact with UI elements from the ThreadManager class.
 * @date
 *      Created:  Filipe, 24 Mar 2014
 *      Modified: Filipe, 27 Mar 2014
 */
void SteamKalix::lock_listings()
{
    ui->btn_add->setEnabled(false);
    ui->btn_remove->setEnabled(false);
    ui->btn_resume->setEnabled(false);
    ui->btn_pause->setEnabled(false);
}

/**
 * @brief SteamKalix::SLOTS::add_account
 *      This SLOT is activated when a user sucessfully logs in.
 *      It sets the cb_accounts with the user data to save and load.
 *      It sets the cb_selected_account with the logged users information.
 * @param username
 *      The logged username.
 * @date
 *      Created:  Filipe, 10 Mar 2014
 *      Modified: Filipe, 3 Sep 2014
 */
void SteamKalix::add_account(QString username)
{
    //Builds the accounts combobox.
    if(username == "AlreadyLogged")
    {
        username = ui->txt_username->text();
    }
    else
    {
        QVariantHash new_account;
        new_account.insert("username", ui->txt_username->text());
        new_account.insert("password", ui->txt_password->text());
        new_account.insert("remember_login", ui->cb_remember_login->isChecked());
        new_account.insert("append_account", ui->cb_append_account->isChecked());
        new_account.insert("disable_proxys", ui->cb_disable_proxys->isChecked());
        new_account.insert("disable_direct", ui->cb_disable_direct->isChecked());
        new_account.insert("disable_cookies", ui->cb_disable_cookies->isChecked());

        if(username == ui->cb_accounts->currentText())
        {
            //If the user changed the information, or played with the config file, we set this again.
            ui->cb_accounts->setItemData(ui->cb_accounts->currentIndex(), new_account);
        }
        else
        {
            //Add new user.
            ui->cb_accounts->addItem(username, new_account);
            ui->cb_accounts->setCurrentText(username);

            ui->cb_proxy_account->addItem(username);
            ui->cb_proxy_account->setCurrentText(username);
        }
    }

    //Builds the selected combobox.
    QString username_display = "";

    if(!ui->cb_append_account->isChecked())
    {
        username_display.append("Main: " + username);
        ui->cb_selected_account->addItem(username_display, QStringList(username));
    }
    else
    {
        QStringList username_data = ui->cb_selected_account->currentData().toStringList();

        username_data.append(username);
        username_display.append(ui->cb_selected_account->currentText());

        if(username_display.contains("Secondary:"))
        {
            username_display.append(", " + username);
        }
        else
        {
            username_display.append(" Secondary: " + username);
        }

        ui->cb_selected_account->setItemText(ui->cb_selected_account->currentIndex(), username_display);
        ui->cb_selected_account->setItemData(ui->cb_selected_account->currentIndex(), username_data);
    }

    ui->cb_selected_account->setCurrentText(username_display);

    //Sets the proxys for this user. The user has to be already saved, with the first proxy set by the login.
    if(!ui->cb_disable_proxys->isChecked())
    {
        set_proxys(username);
    }

    unlock_login();
    validate_all();
}

/**
 * @brief SteamKalix::SLOTS::remove_account
 *      This SLOT is activated when a user logs out.
 *      This can be used for multiple users, by removing them from the selected cb_selected_account data.
 * @param username
 *      The logged out username.
 * @date
 *      Created:  Filipe, 10 Mar 2014
 *      Modified: Filipe, 22 Jun 2014
 */
void SteamKalix::remove_account(QString username)
{
    QStringList account = ui->cb_selected_account->currentData().toStringList();

    if(account.removeOne(username))
    {
        if(!account.isEmpty())
        {
            //Remove this user and logout the next user on the account.
            ui->cb_selected_account->setItemData(ui->cb_selected_account->currentIndex(), account);
            on_btn_logout_clicked();
        }
        else
        {
            //All accounts logedout and removed, remove the item.
            ui->cb_selected_account->removeItem(ui->cb_selected_account->currentIndex());

            unlock_login();
            validate_all();
        }
    }
    else
    {
        output("An error ocurred while removing one of the usernames from this composed account.", 1);
    }
}

/**
 * @brief SteamKalix::output
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
void SteamKalix::output(const QString &message, const int &verbose)
{
    if(Output::get_verbose() >= verbose)
    {
        QVariantHash data;
        data.insert("message", message);
        data.insert("verbose", verbose);
        data.insert("ID1", thread_id);

        QString print = Output::builder(data);

        if(!print.isEmpty())
        {
            emit console(print);
        }
    }
}
