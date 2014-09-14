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

#ifndef DEFINES_H
#define DEFINES_H

/**
 * @brief Defines header
 *      This header defines more explicit names for access modifiers in order to increase readability.
 * @remarks Details
 *      modifier_construct              Used for constructors and destructors.
 *      modifier_methods                Used for class methods.
 *      modifier_members                Used for non data class members. E.g. general control variables.
 *      modifier_data_members           Used for data class members. E.g. arrays, pointers to data, persistent data (static).
 *      modifier_enums                  Used for enumerators (structs should go to data_members).
 *      modifier_operators              Used for operator overloading.
 * @remarks
 *      We could further subdivide with static, const and other identifiers, but that would became confusing.
 *      Instead, logicaly divide then within the above defined blocks.
 * @date
 *      Created:  Filipe, 22 May 2014
 *      Modified: Filipe, 22 May 2014
 */

//Public
#define public_construct public
#define public_methods public
#define public_members public
#define public_data_members public
#define public_enums public
#define public_operators public

//Protected
#define protected_construct protected
#define protected_methods protected
#define protected_members protected
#define protected_data_members protected
#define protected_enums protected
#define protected_operators protected

//Private
#define private_construct private
#define private_methods private
#define private_members private
#define private_data_members private
#define private_enums private
#define private_operators private

#endif // DEFINES_H
