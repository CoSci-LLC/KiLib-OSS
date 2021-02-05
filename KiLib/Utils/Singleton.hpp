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
            if (m_instance == nullptr)
            {
               m_instance = new T();
            }
            return m_instance;
         }
      };
      template <typename T> T *Singleton<T>::m_instance = nullptr; // only change here
   }                                                               // namespace Utils
} // namespace KiLib
