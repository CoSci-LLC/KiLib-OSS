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
   int skiprows_;
   bool token_compress_on_;
   bool remove_first_sep_;
};
