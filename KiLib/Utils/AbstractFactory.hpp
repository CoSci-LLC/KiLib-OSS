#include <KiLib/Utils/enum.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <typeinfo>


template <typename E, class B> class AbsFac
{
public:
   const std::map<E, std::function<B()>> m;

   AbsFac(std::map<E, std::function<B()>> m) : m(m)
   {
   }

   B get(E o) const
   {
      return this->m.at(o)();
   }

   B get(std::string o) const
   {
      return this->m.at(E::_from_string(o.c_str()))();
   }
};
