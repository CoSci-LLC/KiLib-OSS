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


// ============================================================================
// Copyright (C) CoSci LLC. All rights reserved.
//
// Authors: Denis Cohen-Corticchiato
// Email:   denis.cohen@gmail.com
//
// This file is part of the KiRoot library
// ============================================================================

#pragma once

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

class CSVReader
{
public:
   CSVReader(
      std::string filename, std::string sep = ",", int skiprows = 0, bool token_compress_on = false,
      bool remove_first_sep = false)
      : fileName_(filename), sep_(sep), skiprows_(skiprows), token_compress_on_(token_compress_on),
        remove_first_sep_(remove_first_sep){};

   std::vector<std::vector<std::string>> GetData();

   std::string fileName_;
   std::string sep_;
   int         skiprows_;
   bool        token_compress_on_;
   bool        remove_first_sep_;
};
