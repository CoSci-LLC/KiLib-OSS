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