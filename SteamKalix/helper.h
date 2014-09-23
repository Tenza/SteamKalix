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

#ifndef HELPER_H
#define HELPER_H

#include <QNetworkProxy>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariantHash>
#include <QThread>
#include <QtMath>
#include <QMutex>

/**
 * @brief The Helper namespace
 *      This namespace is used implement static helper fuctions.
 *      This are general functions, that are reused throughout the code.
 * @date
 *      Created:  Filipe, 26 Apr 2014
 *      Modified: Filipe, 26 Apr 2014
 */
namespace Helper
{
    void parse_json_object(const QJsonObject &object, const QStringList &keys, QVariantHash &listings);
    void parse_json_array(const QJsonArray &array, const QStringList &keys, QVariantHash &listings);

    int price_converter(const double &price);
    double price_converter(const int &price);

    int percentage(const int &number, const int &percentage);
    double percentage(const double &number, const int &percentage);

    QString substring(const QString &text, const QString &start, const QString &end);
    QString random_string(const int &length);
    int random_number(const int &low, const int &high);

    QString get_thread_id();

    double currency_rate(const QString &currency_id);

    namespace currency
    {
        enum type {id, code, ncr, symbol, name, remove};
    }
    QString currency_converter(QString input, const currency::type &to);

    QNetworkProxy::ProxyType proxy_type(const QString &type);

}

#endif // HELPER_H
