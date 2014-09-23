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

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

/**
 * @brief The SettingsManager namespace
 *      This namespace is used implement static functions to intetact with the configuration file directly.
 * @date
 *      Created:  Filipe, 9 Mar 2014
 *      Modified: Filipe, 9 Mar 2014
 */
namespace SettingsManager
{
    QString get_filepath();

    void write(const QString &key, const QVariant &value);
    QVariant read(const QString &key, const QVariant &default_value = QVariant());
    void remove(const QString &key);

    void log(QString message);
}

#endif // SETTINGSMANAGER_H
