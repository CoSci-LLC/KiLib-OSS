/**
 *  Copyright (c) 2020-2021 CoSci LLC, USA <software@cosci-llc.com>
 *
 *  This file is part of KiLib-OSS.
 *
 *  KiLib-OSS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  KiLib-OSS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with KiLib-OSS.  If not, see <https://www.gnu.org/licenses/>.
 */


/**
 * Provides a way to create a getter/setter in a more efficent manner
 */
#pragma once

template <typename attribute_type> class Attribute
{
public:
   Attribute()
   {
   }
   Attribute(attribute_type value) : value(value)
   {
   }

   attribute_type operator()() const
   {
      return value;
   }

   void operator=(attribute_type value)
   {
      this->value = value;
   }

   void operator()(attribute_type value)
   {
      this->value = value;
   }

private:
   attribute_type value;
};