/**
 * Singleton.hpp
 * Provides a way to create singleton objects using templates
 * @author Owen T. Parkins
 */

#pragma once

namespace KiLib
{
   namespace Utils
   {
      template <typename T> class Singleton
      {
      private:
         static T *m_instance;

         Singleton()
         {
         }

      public:
         static T *GetInstance()
         {
            if (m_instance == nullptr) {
               m_instance = new T();
            }
            return m_instance;
         }
      };
      template <typename T> T *Singleton<T>::m_instance = nullptr; // only change here
   }                                                               // namespace Utils
} // namespace KiLib
