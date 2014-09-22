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

#include <QApplication>

#include "steamkalix.h"

void debug_messages_handler(QtMsgType type, const QMessageLogContext &context, const QString &message);

/**
 * @brief main
 *      Application entry point.
 * @param argc
 *      Command line argument count.
 * @param argv
 *      Command line argument vector.
 * @return
 *      0 = Success
 * @date
 *      Created:  Filipe, 29 Dez 2013
 *      Modified: Filipe, 29 Dez 2013
 */
int main(int argc, char *argv[])
{
    qInstallMessageHandler(debug_messages_handler);

    QApplication application(argc, argv);
    SteamKalix steamkalix;
    steamkalix.show();

    return application.exec();
}

/**
 * @brief debug_messages_handler
 *      Handler for custom debugging messages.
 * @param type
 *  	The message type.
 * @param context
 *      Additional information about a log message.
 * @param message
 *      The message.
 * @date
 *      Created:  Filipe, 23 Apr 2014
 *      Modified: Filipe, 23 Apr 2014
 */
void debug_messages_handler(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    QByteArray localMsg = message.toLocal8Bit();

    switch (type)
    {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}

/*
 * Compilation:
 *  Tested on:
 *      Windows 7 64bit
 *      Arch 3.16.1 64Bit
 *      Ubuntu 14.04 64Bit
 *      Ubuntu 14.04 32Bit
 *
 * SteamKalix:
 * +TODO v0.1: Login interface base.
 * +TODO v0.1: Login proxy support.
 * +TODO v0.1: Login captcha support support.
 * +TODO v0.1: Login SteamGuard support.
 * +TODO v0.1: Save/load of window settings.
 * +TODO v0.1: Listings interface base.
 * +TODO v0.1: Listings table.
 * +TODO v0.1: Documentation.
 * +TODO v0.1: Redesign listing tab.
 * +TODO v0.2: MainWindow replaced by SteamKalix, as it should be.
 * +TODO v0.3: Added support for multiple accounts.
 * +TODO v0.3: Login Redesign.
 * +TODO v0.3: Login validations.
 * +TODO v0.3: Seconday accounts.
 * +TODO v0.3: Empty accounts.
 * +TODO v0.3: Added support for multiple proxys.
 * +TODO v0.3: New proxy tab.
 * +TODO v0.3: Proxy list implementation.
 * +TODO v0.3: Account add and remove signals.
 * +TODO v0.3: Verbose and logging options.
 * +TODO v0.3: Logout for multiple users.
 * +TODO v0.4: Added support to disable proxys.
 * +TODO v0.4: Added support to disable direct connections.
 * +TODO v0.4: Disable account option is now infered. (This is needed to have a main ONLY making buy and sell operations.)
 * +TODO v0.4: Completly removed all the functionality of the "Empty" wildcard
 * +TODO v0.4: Added support to disable cookies.
 * +TODO v0.4: Review component tooltips.
 * +TODO v0.4: Disable unused options.
 * +TODO v0.4: Replace QList<QTableWidgetItem *> for QModelIndexList method.
 * -TODO v0.X: Change listing table (ID, Short URL, Status, Main ACC).
 * -TODO v0.X: Currency, language and country options. (They can be made from: Cookies, profile or change the page cookies to /market/.)
 * -TODO v0.X: Proxy organize.
 * -TODO v0.X: Implement custom headers.(Parameters tab)
 * -TODO v0.X: Rename Layouts
 *
 * Login:
 * +TODO v0.1: Create login class base.
 * +TODO v0.1: Use OpenSSL (QCA wasnt ready for QT5).
 * +TODO v0.1: Implement password encryption.
 * +TODO v0.1: Implement output method.
 * +TODO v0.1: Add GUI proxy support.
 * +TODO v0.1: Optimize code.
 * +TODO v0.1: Implement captcha.
 * +TODO v0.1: Change the POST and GETs to use QUrlQuery (new Qt5).
 * +TODO v0.1: Documentation.
 * +TODO v0.1: Steamguard verifications.
 * +TODO v0.1: Splitter Layout.
 * +TODO v0.1: Change to use new NetworkManager.
 * +TODO v0.1: Change all GET and POST to use new implementation.
 * +TODO v0.1: BUG: Clear cookies cannot clear ALL cookies, the settings that identify the browser, should never be cleared.
 * +TODO v0.1: State enum
 * +TODO v0.1: BUG: The login transfer was not being made because there was missing parameters. They are now dynamic.
 * +TODO v0.1: Push the webTradeEligibility cookie.
 * +TODO v0.1: Pull profile information, new state added, read XML data, create AccountData entry.
 * +TODO v0.2: AccountData ballance converted to INT.
 * +TODO v0.3: Passing QHash as parameter.
 * +TODO v0.3: Fixed accountadd order.
 * +TODO v0.3: Fixed "Empty" to pull proxys.
 * +TODO v0.3: Logout for multiple users.
 * +TODO v0.4: Removed "Empty" exception.
 * -TODO v0.X: Login new logic, use single finnish method. (Avoids connects/disconnects).
 * -TODO v0.X: BUG: Logout if queried from diferent IP. Make login checks.
 * -TODO v0.X: BUG: Potencial session problems with multilogins.
 * -TODO v0.X: Emulate the timezoneOffset cookie. This cookie does not show in the trafic analyser because it is set by the JS, function: setTimezoneCookies
 * -TODO v0.X: Implement diferent user-agents.
 *
 * Persistent Storage:
 * +TODO v0.1: Create base inplementation of PersistentCookieJar.
 * +TODO v0.1: Save and load are now abstracted with SettingsManager.
 * +TODO v0.1: Documentation.
 * +TODO v0.1: New 'clear()' function that supports exclusions.
 * +TODO v0.1: Load method optionally cleans cookies.
 * +TODO v0.1: Print method now uses HTML paragraphs instead of '\n'
 * +TODO v0.4: Count number of active cookies.
 * -TODO v0.X: Crypt cookies in file (Only values, so I can use QSettings).
 *
 * SettingsManager:
 * +TODO v0.1: Create SettingsManager class to abstract file interaction.
 * +TODO v0.1: Documentation.
 * +TODO v0.1: Logging function.
 *
 * NetworkManager:
 * +TODO v0.1: Network manager created and reimplemented.
 * +TODO v0.1: Network manager handles persisten storage.
 * +TODO v0.1: Network manager static settings load/save.
 * +TODO v0.1: Organize include files.
 * +TODO v0.1: Documentation.
 * +TODO v0.1: Implement a new GET to support timeout system.
 * +TODO v0.1: Implement a new POST to support timeout system.
 * +TODO v0.1: Move all requests to manager (Auto set headers).
 * +TODO v0.1: Support multiple logins (Array of cookies lists).
 * +TODO v0.1: Add support to temporary headers.
 * +TODO v0.1: Bypass loading the current user.
 * +TODO v0.2: Save_settings is now thread-safe.
 * +TODO v0.3: GET and POST now use QUrlQuery, applied changes to all classes.
 * +TODO v0.3: Added support to multiple proxys for each network user.
 * +TODO v0.3: Added support for multiple network users in one instance.
 * +TODO v0.3: New user/proxy throtling mechanism.
 * +TODO v0.3: Saving proxys as static methods.
 * +TODO v0.3: Print current user.
 * +TODO v0.4: Removed  parameters from save/load.
 * +TODO v0.4: Removed throttle start/stop functions
 * +TODO v0.4: Fixed bug where headers where not being applied to all proxys.
 * +TODO v0.4: No cookies support with new constructor and load parameter.
 * +TODO v0.4: Network print shows "No cookies".
 * +TODO v0.4: Overall organization.
 *
 * ReplyTimeout:
 * +TODO v0.1: Implementation of base functionality.
 * +TODO v0.1: Documentation.
 *
 * ThreadsManager:
 * +TODO v0.1: Create ThreadManager class base.
 * +TODO v0.1: Thread creation.
 * +TODO v0.1: Thread communication.
 * +TODO v0.1: Thread termination.
 * +TODO v0.1: Multithread handling.
 * +TODO v0.1: Problem with stopping (ID introduced).
 * +TODO v0.1: Block application exit until thread termination.
 * +TODO v0.1: Documentation.
 * +TODO v0.1: Optimize code.
 * +TODO v0.1: BUG: Removing listings and imediatly closing, causes improper termination (pending implemented).
 * +TODO v0.1: Change outputs.
 * +TODO v0.1: Buy thread support.
 * +TODO v0.1: Sell thread support.
 *
 * Listings:
 * +TODO v0.1: Create listnings class base.
 * +TODO v0.1: Network testing.
 * +TODO v0.1: Self destruct by finnish signal.
 * +TODO v0.1: Documentation.
 * +TODO v0.1: Create base operation mechanism.
 * +TODO v0.1: Stop and resume
 * +TODO v0.1: Information parsing
 * +TODO v0.1: BUG: rare case of mixed reply, this causes the thread not to terminate.
 * +TODO v0.1: Optimize code.
 * +TODO v0.1: Basic scan operations.
 * +TODO v0.1: QUrl in request
 * +TODO v0.1: Wait pending buy operations.
 * +TODO v0.1: Setup currency conversion to support unlogged settings. (Removed)
 * +TODO v0.1: Change outputs.
 * +TODO v0.1: Feature to block buy operations if X items are set.
 * +TODO v0.1: Dynamic query parameters. The number of loaded items is automaticaly ajusted.
 * +TODO v0.1: Generata buy and sell ID's.
 * +TODO v0.1: Implement listings history.
 * +TODO v0.1: Handle sell operations.
 * +TODO v0.1: Pass username.
 * +TODO v0.1: Price generation.
 * +TODO v0.1: Reload prices on resume
 * +TODO v0.2: Reload prices on resume, and renamings.
 * +TODO v0.2: Using price_converter functions.
 * +TODO v0.3: New fixed parameters.
 * +TODO v0.3: Fixed termination bug on selling only listings.
 * +TODO v0.3: Added cache trick. (Not working)
 * -TODO v0.X: Pass link_counter, link_counter_step, price_counter_max, price_counter_average to the GUI.
 * -TODO v0.X: Statistics
 * -TODO v0.X: Add suport for listing save and loading.
 *
 * Buy:
 * +TODO v0.1: Create buy class base.
 * +TODO v0.1: Send buy request.
 * +TODO v0.1: Pending counter
 * +TODO v0.1: Referer problem
 * +TODO v0.1: Change paradim to queue.
 * +TODO v0.1: Repeat request if not able to contact the game server.
 * +TODO v0.1: Stop repeted requests.
 * +TODO v0.1: BUG: Lock on "Waiting for pending buying request."
 * +TODO v0.1: BUG: IODeviceRead problem. (Every call to reply->readAll(); will be replaced, problem due to close on timeout.)
 * +TODO v0.1: Output function
 * +TODO v0.1: History moved to listing.
 * +TODO v0.1: Pass BuyID and username.
 * +TODO v0.2: Update walletbalance.
 * -TODO v0.X: Pass number of retrys to the GUI.
 * -TODO v0.X: Flush items if counter reacher X itens on hold.
 * -TODO v0.X: Multiuser, probably is possible multirequest.
 * -TODO v0.X: Statistics
 *
 * Sell:
 * +TODO v0.1: Create sell class base.
 * +TODO v0.1: Send sell request.
 * +TODO v0.1: BUG: The ID set by the listing is different from the real ID.
 * +TODO v0.1: Pull information data based on inventory.
 * +TODO v0.1: Parse inventory data.
 * +TODO v0.1: Repeat system system.
 * +TODO v0.1: Pass SellID and username
 * +TODO v0.1: Delay on failure.
 * -TODO v0.X: Pass number of retrys to the GUI.
 * -TODO v0.X: Multiuser, probably is possible multirequest.
 * -TODO v0.X: Statistics
 *
 * Helper:
 * +TODO v0.1: Base implementation.
 * +TODO v0.1: Move helper functions to this namespace.
 * +TODO v0.1: Fix code to use this namespace instead.
 * +TODO v0.1: New currency converter.
 * +TODO v0.1: Documentation.
 * +TODO v0.2: Percentage functions.
 * +TODO v0.2: Price convertion functions.
 * +TODO v0.2: Change floats to double, tested invert functions.
 * +TODO v0.2: Currency_converter is now thread-safe.
 * +TODO v0.3: New currency converter.
 *
 * AccountData:
 * +TODO v0.1: Namespace to provide access to the data of multiple logins.
 * +TODO v0.1: Documentation
 * +TODO v0.2: Walletbalance to INT.
 * +TODO v0.2: Write functions are now thread-safe.
 *
 * ListingsManager:
 * +TODO v0.1: Base implementation.
 * -TODO v0.X: Verify current listings (reprice items), items that failed to put on sale! (Watch out for items that should not be sold like Keys)
 * -TODO v0.X: Send emails with statistics.
 * -TODO v0.X: Crawler for the market, check recent added items?
 * -TODO v0.X:   Add/remove/update (available only for connectivity) items based on statistics.
 * -TODO v0.X:   Define the total number of items based on performance (at start).
 * -TODO v0.X: Define listing types.
 * -TODO v0.X:   Listings that are constantly trading.
 * -TODO v0.X:   Listings that just look for extremly good prices (higher priced items).
 *
 * Defines:
 * +TODO v0.2: Base implementation.
 * +TODO v0.2: Apply modifications to current classes.
 *
 * Output:
 * +TODO v0.2: Create base class.
 * +TODO v0.2: Namespace for verbose, styling and uniformization of outputs.
 * +TODO v0.2: Namespace applied to all output functions.
 * +TODO v0.2: Output functions are now filtering messages, will avoid unnessesary signals.
 * +TODO v0.2: Documentation.
 *
 * Multiple:
 * +TODO v0.1: Review const correctnes, nulls, includes, documentation, replace [] for .at()
 * +TODO v0.1: All floats to doubles.
 * +TODO v0.4: Organized the TODO list.
 * +TODO v0.4: Comments review.
 * +TODO v0.4: Removed reusability and dependencies sections.
 * +TODO v0.4: Removed unused destructors.
 *
 */
