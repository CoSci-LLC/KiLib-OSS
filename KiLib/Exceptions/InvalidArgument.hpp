#pragma once
#include <exception>

class InvalidArgumentException : public std::exception
{
   const char *message;

public:
   InvalidArgumentException(const char *message)
   {
      this->message = message;
   }

   virtual const char *what() const throw()
   {
      return message;
   }
};