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


#include <KiLib/Utils/CSVReader.hpp>
#include <gtest/gtest.h>

namespace KiLib
{

   void checkVectors(std::vector<std::vector<std::string>> results, std::vector<std::vector<std::string>> goldValues)
   {
      ASSERT_EQ(results.size(), goldValues.size());
      for (size_t i = 0; i < results.size(); i++)
      {
         ASSERT_EQ(results[i].size(), goldValues[i].size());
         for (size_t j = 0; j < results[i].size(); j++)
         {
            ASSERT_STRCASEEQ(goldValues[i][j].c_str(), results[i][j].c_str());
         }
      }
   }

   TEST(CSVReader, NormalCSV)
   {

      std::vector<std::vector<std::string>> goldValues{
         {
            "John",
            "Doe",
            "120 jefferson st.",
            "Riverside",
            " NJ",
            " 08075",
         },
         {
            "Jack",
            "McGinnis",
            "220 hobo Av.",
            "Phila",
            " PA",
            "09119",
         },
         {
            "\"John \"\"Da Man\"\"\"",
            "Repici",
            "120 Jefferson St.",
            "Riverside",
            " NJ",
            "08075",
         },
         {
            "Stephen",
            "Tyler",
            "\"7452 Terrace \"\"At the Plaza\"\" road\"",
            "SomeTown",
            "SD",
            " 91234",
         },
         {
            "",
            "Blankman",
            "",
            "SomeTown",
            " SD",
            " 00298",
         },
         {
            "\"Joan \"\"the bone\"\"",
            " Anne\"",
            "Jet",
            "\"9th",
            " at Terrace plc\"",
            "Desert City",
            "CO",
            "00123",
         }};

      std::vector<std::vector<std::string>> results =
         CSVReader(std::string(TEST_DIRECTORY) + "/CSV/test1.csv").GetData();
      checkVectors(results, goldValues);
   }

   TEST(CSVReader, CompressCSV)
   {
      std::vector<std::vector<std::string>> goldValues{
         {
            "John",
            "Doe",
            "120 jefferson st.",
            "Riverside",
            " NJ",
            " 08075",
         },
         {
            "Jack",
            "McGinnis",
            "220 hobo Av.",
            "Phila",
            " PA",
            "09119",
         },
         {
            "\"John \"\"Da Man\"\"\"",
            "Repici",
            "120 Jefferson St.",
            "Riverside",
            " NJ",
            "08075",
         },
         {
            "Stephen",
            "Tyler",
            "\"7452 Terrace \"\"At the Plaza\"\" road\"",
            "SomeTown",
            "SD",
            " 91234",
         },
         {
            "",
            "Blankman",
            "SomeTown",
            " SD",
            " 00298",
         },
         {
            "\"Joan \"\"the bone\"\"",
            " Anne\"",
            "Jet",
            "\"9th",
            " at Terrace plc\"",
            "Desert City",
            "CO",
            "00123",
         }};
      std::vector<std::vector<std::string>> results =
         CSVReader(std::string(TEST_DIRECTORY) + "/CSV/test1.csv", ",", 0, true, false).GetData();
      checkVectors(results, goldValues);
   }

   TEST(CSVReader, IgnoreFirstCSV)
   {
      std::vector<std::vector<std::string>> goldValues{
         {
            "John",
            "Doe",
            "120 jefferson st.",
            "Riverside",
            " NJ",
            " 08075",
         },
         {
            "Jack",
            "McGinnis",
            "220 hobo Av.",
            "Phila",
            " PA",
            "09119",
         },
         {
            "\"John \"\"Da Man\"\"\"",
            "Repici",
            "120 Jefferson St.",
            "Riverside",
            " NJ",
            "08075",
         },
         {
            "Stephen",
            "Tyler",
            "\"7452 Terrace \"\"At the Plaza\"\" road\"",
            "SomeTown",
            "SD",
            " 91234",
         },
         {
            "Blankman",
            "SomeTown",
            " SD",
            " 00298",
         },
         {
            "\"Joan \"\"the bone\"\"",
            " Anne\"",
            "Jet",
            "\"9th",
            " at Terrace plc\"",
            "Desert City",
            "CO",
            "00123",
         }};
      std::vector<std::vector<std::string>> results =
         CSVReader(std::string(TEST_DIRECTORY) + "/CSV/test1.csv", ",", 0, true, true).GetData();
      checkVectors(results, goldValues);
   }

} // namespace KiLib
