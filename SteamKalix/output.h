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

#ifndef OUTPUT_H
#define OUTPUT_H

#include <QString>
#include <QVariantHash>
#include <QMutex>

#include "helper.h"
#include "settingsmanager.h"

/**
 * @brief The Output namespace
 *      This namespace is used to generalize the way messages are displayed across the diferent threads and classes.
 *      Any functionality related to the output of messages should be in here.
 * @date
 *      Created:  Filipe, 25 May 2014
 *      Modified: Filipe, 24 Jun 2014
 */
namespace Output
{
    int get_verbose();
    void set_verbose(int new_verbose);

    bool get_logging();
    void set_logging(bool new_logging);

    QString builder(const QVariantHash &data);
}

#endif // OUTPUT_H
