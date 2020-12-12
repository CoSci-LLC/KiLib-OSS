#pragma once
#include <exception>

class VectorSizeException : public std::exception
{
   const char *message;

public:
   VectorSizeException(const char *message)
   {
      this->message = message;
   }

   virtual const char *what() const throw()
   {
      return message;
   }
};