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

#include <KiLib/Raster/Raster.hpp>

namespace KiLib
{
   void Raster::AssertAgreeDim(const std::vector<const KiLib::Raster *> &rasts)
   {
      if (rasts.size() == 0)
      {
         return;
      }

      auto cmp = [&](double a, double b, double eps, std::string val)
      {
         if (std::abs(a - b) > eps)
         {
            throw std::invalid_argument(fmt::format("Raster {} sizes do not agree! Got {} and {}", val, a, b));
         }
      };

      // Cell Size
      for (auto *rast : rasts)
      {
         cmp(rast->cellsize, rasts.at(0)->cellsize, 1e-2, "CellSize");
      }

      // nRows
      for (auto *rast : rasts)
      {
         cmp(rast->nRows, rasts.at(0)->nRows, 0.0, "Num Rows");
      }

      // nCols
      for (auto *rast : rasts)
      {
         cmp(rast->nCols, rasts.at(0)->nCols, 0.0, "Num Cols");
      }
   }

} // namespace KiLib
