#pragma once

#include <KiLib/Utils/enum.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <spdlog/spdlog.h>
#include <typeinfo>
#include <algorithm>

template <typename Enum, class Base> class AbsFac
{
public:
   const std::map<Enum, std::function<Base()>> map;

   AbsFac(std::map<Enum, std::function<Base()>> map) : map(map)
   {
   }

   Base get(Enum val) const
   {
      return this->map.at(val)();
   }

   Base get(std::string val) const
   {
      try
      {
         // Erase spaces because of tree species readability
         std::string clean = this->removeSpaces(val);
         return this->map.at(Enum::_from_string_nocase(clean.c_str()))();
      }
      catch (std::exception &e)
      {
         spdlog::error("Unknown {} '{}'! Available options: {}", Enum::_name(), val, fmt::join(Enum::_names(), ", "));
         spdlog::error("Note: These values are case insensitive and ignore spaces!");
         exit(EXIT_FAILURE);
      }
   }

   std::string IDToString(Enum val) const
   {
      return Enum::_to_string(val);
   }
   
   Enum StringToID(std::string val) const
   {
      try
      {
         // Erase spaces because of tree species readability
         std::string clean = this->removeSpaces(val);
         return Enum::_from_string_nocase(clean.c_str());
      }
      catch (std::exception &e)
      {
         spdlog::error("Unknown {} '{}'! Available options: {}", Enum::_name(), val, fmt::join(Enum::_names(), ", "));
         spdlog::error("Note: These values are case insensitive and ignore spaces!");
         exit(EXIT_FAILURE);
      }
   }

private:
   std::string removeSpaces(std::string input) const
   {
     input.erase(std::remove(input.begin(),input.end(),' '),input.end());
     return input;
   }
};
