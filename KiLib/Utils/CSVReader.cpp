#include <KiLib/Utils/CSVReader.hpp>


/**
 * A strtok (thread safe) that doesn't ignore consecuative delimiters
 * @param string The input string
 * @param delimiter The delimiter(s) to use
 * @param saveptr The pointer to make the function thread safe
 */
char *strtok_new_r(char *string, char const *delimiter, char **saveptr)
{

   char *ptr, *riturn = 0;

   if (string != NULL)
   {
      *saveptr = string;
   }

   if (*saveptr == NULL)
   {
      return NULL;
   }

   if ((ptr = strpbrk(*saveptr, delimiter)) != NULL)
   {
      *ptr     = 0;
      riturn   = *saveptr;
      *saveptr = ++ptr;
   }

   if (!ptr)
   {
      if (*saveptr)
      {
         riturn   = *saveptr;
         *saveptr = NULL;
      }
   }

   return riturn;
}


std::vector<std::vector<std::string>> CSVReader::GetData()
{
   std::ifstream file(fileName_);

   std::string line;

   // Skip rows
   for (auto i = 0; i < skiprows_; ++i)
   {
      getline(file, line);
   }

   // Read data
   char *                                saveptr = nullptr;
   std::vector<std::vector<std::string>> data;
   while (getline(file, line))
   {
      char *                   inputLine    = strdup(line.c_str());
      char *                   refInputLine = inputLine;
      std::vector<std::string> vec;

      // Create the vector of tokens
      // Use the for loop to set the second iteration to null.
      char *token = nullptr;
      for (int count = 0;; count++, inputLine = nullptr)
      {
         token = strtok_new_r(inputLine, sep_.c_str(), &saveptr);
         if (token == nullptr)
         {
            // Finished parsing the entire line; break out
            break;
         }

         // If we are compressing useless tokens....
         if (token_compress_on_)
         {
            // Check if the token has nothing in it, if so, just go to the next
            // iteration in the loop. But only if it is in the middle of the
            // string.
            if (strncmp("", token, 1) == 0 && count != 0)
            {
               continue;
            }
         }
         // Push back onto the return vector.
         vec.push_back(std::string(token));
      }

      if (remove_first_sep_ && vec[0] == "")
      {
         vec.erase(vec.begin());
      }

      data.push_back(vec);
      free(refInputLine);
   }
   file.close();

   return data;
}
