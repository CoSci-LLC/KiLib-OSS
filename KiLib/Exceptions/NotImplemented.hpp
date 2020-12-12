#pragma once
#include <exception>


class NotImplementedException : public std::exception
{
   const char *message;

public:
   NotImplementedException(const char *message)
   {
      this->message = message;
   }

   virtual const char *what() const throw()
   {
      return message;
   }
};