#include <KiLib/Utils/enum.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <spdlog/spdlog.h>
#include <typeinfo>


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
         return this->map.at(Enum::_from_string_nocase(val.c_str()))();
      }
      catch (std::exception &e)
      {
         spdlog::error("Unknown {}! Available options: {}", Enum::_name(), fmt::join(Enum::_names(), ", "));
         exit(EXIT_FAILURE);
      }
   }
};
