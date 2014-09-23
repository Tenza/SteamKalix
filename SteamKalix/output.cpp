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

#include "output.h"

/**
 *@brief Anonymous namespace
 *      This namespace is used as a "private section".
 *      It is anonymous and therefore can only accessed within file scope.
 *@date
 *      Created:  Filipe, 26 May 2014
 *      Modified: Filipe, 26 May 2014
 */
namespace
{
    QMutex mutex;
    bool logging;
    int verbose;
}

/**
 * @brief Output::get_verbose
 *      Gets the current verbose level.
 *      This is used to filter messages directly in the 'output' functions.
 * @return
 *      The verbose level.
 * @date
 *      Created:  Filipe, 26 May 2014
 *      Modified: Filipe, 26 May 2014
 */
int Output::get_verbose()
{
    return verbose;
}

/**
 * @brief Output::set_verbose
 *      Sets the verbose level.
 * @param new_verbose
 *      The new vervose level.
 * @date
 *      Created:  Filipe, 26 May 2014
 *      Modified: Filipe, 26 May 2014
 */
void Output::set_verbose(int new_verbose)
{
    mutex.lock();
    verbose = new_verbose;
    mutex.unlock();
}

/**
 * @brief Output::get_logging
 *      Gets the current log state.
 *      This is used to enable or disable logged messages.
 * @return
 *      Log state.
 * @date
 *      Created:  Filipe, 24 Jun 2014
 *      Modified: Filipe, 24 Jun 2014
 */
bool Output::get_logging()
{
    return logging;
}

/**
 * @brief Output::set_logging
 *      Sets the log state.
 * @param new_log
 *      New log state.
 * @date
 *      Created:  Filipe, 24 Jun 2014
 *      Modified: Filipe, 24 Jun 2014
 */
void Output::set_logging(bool new_logging)
{
    mutex.lock();
    logging = new_logging;
    mutex.unlock();
}

/**
 * @brief Output::builder
 *      This function is responsible for buinding the displayed messages.
 * @param data
 *      This parameter contains the data with everything to be processed by this function.
 * @return
 *      The string to be displayed.
 * @remarks
 *      This function just processes the data that is passed, the emition of the signal
 *      belongs to the 'output' function present on each class that needs an output method.
 *      This is done because every class has diferent needs. But, if we wanted to eliminate the 'output'
 *      function anyway, we could pass the reference of the signal function and emit directly in here; but this
 *      would just make every output more complicated and we could no longer make optimizations for each class.
 * @date
 *      Created:  Filipe, 25 May 2014
 *      Modified: Filipe, 28 May 2014
 */
QString Output::builder(const QVariantHash &data)
{
    QString result = "";

    //The message and verbose level are obligatory.
    if(data.contains("message") && data.contains("verbose"))
    {
        //Gets all the IDs in numbered order.
        for(int i = 1; i < data.size(); i++)
        {
            QString key = "ID" + QString::number(i);
            if(data.contains(key))
            {
                if(data.value(key).type() == QVariant::String)
                {
                    QString id = data.value(key).toString();
                    if(id != "-1")
                    {
                        result.append("[" + id + "]");
                    }
                }
                else if(data.value(key).type() == QVariant::Int)
                {
                    int id = data.value(key).toInt();
                    if(id != -1)
                    {
                        result.append("[" + QString::number(id) + "]");
                    }
                }
            }
        }

        //If requested loads and prepends the current thread ID.
        if(data.value("load_thread_id", false).toBool())
        {
            result.prepend("[" + Helper::get_thread_id() + "]");
        }

        //Adds the message.
        result.append(" " + data.value("message").toString());

        //Logs this message.
        if(data.value("log", false).toBool() && logging)
        {
            SettingsManager::log(result);
        }

        //Add HTML styles to the message.
        int verbose_level = data.value("verbose").toInt();
        if(verbose_level == 1)
        {
            result.prepend("<b>");
            result.append("</b>");
        }
        else if(verbose_level == 3)
        {
            result.prepend("<font color=\"Gray\">");
            result.append("</font>");
        }
    }

    return result;
}
