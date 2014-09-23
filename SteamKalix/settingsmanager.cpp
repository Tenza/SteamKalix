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

#include "settingsmanager.h"

/**
 *@brief Anonymous namespace
 *      This namespace is used as a "private section".
 *      It is anonymous and therefore can only accessed within file scope.
 *@remarks Variables
 *      The variable "filepath" is not immediately initialized because the function "applicationDirPath" makes use of the object
 *      "QApplication", and at this point the object does not yet exist.
 *@date
 *      Created:  Filipe, 9 Mar 2014
 *      Modified: Filipe, 28 May 2014
 */
namespace
{
    QString filepath = "";
    QString log_filename = "log.txt";
    QString config_filename = "config.ini";
    QSettings::Format config_fileformat = QSettings::IniFormat;
}

/**
 * @brief SettingsManager::get_filepath
 *      Gets the path to store the settings and log files.
 * @return
 *      The path of the executable.
 * @date
 *      Created:  Filipe, 28 May 2014
 *      Modified: Filipe, 28 May 2014
 */
QString SettingsManager::get_filepath()
{
    if(filepath.isEmpty())
    {
        filepath = QCoreApplication::applicationDirPath() + "/";
    }

    return filepath;
}

/**
 * @brief SettingsManager::write
 *      Writes the key and value to the file.
 * @param key
 *      The key.
 * @param value
 *      The value.
 * @date
 *      Created:  Filipe, 9 Mar 2014
 *      Modified: Filipe, 28 May 2014
 */
void SettingsManager::write(const QString &key, const QVariant &value)
{
    QSettings storage(get_filepath() + config_filename, config_fileformat);
    storage.setValue(key, value);
}

/**
 * @brief SettingsManager::read
 *      Reads a value from a given key.
 * @param key
 *      The key.
 * @param default_value
 *      The default return value.
 * @return
 *      The value.
 * @date
 *      Created:  Filipe, 9 Mar 2014
 *      Modified: Filipe, 28 May 2014
 */
QVariant SettingsManager::read(const QString &key, const QVariant &default_value)
{
    QSettings storage(get_filepath() + config_filename, config_fileformat);
    return storage.value(key, default_value);
}

/**
 * @brief SettingsManager::remove
 *      Removes the setting key and any sub-settings of key.
 * @param key
 *      The key.
 * @date
 *      Created:  Filipe, 5 Jun 2014
 *      Modified: Filipe, 5 Jun 2014
 */
void SettingsManager::remove(const QString &key)
{
    QSettings storage(get_filepath() + config_filename, config_fileformat);
    return storage.remove(key);
}

/**
 * @brief SettingsManager::log
 *      Output message to a file.
 * @param message
 *      Message to be saved to persistent storage.
 *      This is passed as copy in order to prepend the current date.
 * @date
 *      Created:  Filipe, 28 May 2014
 *      Modified: Filipe, 28 May 2014
 */
void SettingsManager::log(QString message)
{
    message.prepend(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss.zzz "));

    QFile file(get_filepath() + log_filename);

    if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << message << endl;
    }
}
