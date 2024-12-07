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

template <typename attribute_type> class SafeAttribute
{
public:
   SafeAttribute(std::string name)
   {
      has_been_set = false;
      this->name = name;

   }
   SafeAttribute(std::string name, attribute_type value) : value(value)
   {
      has_been_set = true;
      this->name = name;

   }

   attribute_type operator()() const
   {
      return get();
   }

   attribute_type operator()()
   {
      return get();
   }


   void operator=(attribute_type value)
   {
      set(value);
   }

   void operator()(attribute_type value)
   {
      set(value);
   }

   bool HasValue() const 
   {
      return has_been_set;
   }

   bool operator==(const SafeAttribute<attribute_type>& other) const 
   {
      return get() == other();
   }

   bool operator==(const attribute_type& other) const 
   {
      return get() == other;
   }

   bool operator!=(const SafeAttribute<attribute_type>& other) const 
   {
      return get() != other();
   }

   attribute_type operator+(const SafeAttribute<attribute_type>& other) const 
   {
      return get() + other.get();
   }

   attribute_type operator+(const attribute_type& other) const 
   {
      return get() + other;
   }

   void set_name(std::string name) 
   {
      this->name = name;
   }

   inline void set(attribute_type val) 
   {
      this->value = val;
      has_been_set = true;
   }

private:

   inline attribute_type get() const
   {
      if (!has_been_set)
      {
         throw std::runtime_error("Value (" + name + ") has not been set!");
      }
       return value;
   }

   
   
   attribute_type value;
   std::string name;
   bool has_been_set;
};