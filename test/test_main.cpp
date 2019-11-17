/***
    This file is part of popl (program options parser lib)
    Copyright (C) 2015-2019 Johannes Pohl

    This software may be modified and distributed under the terms
    of the MIT license.  See the LICENSE file for details.
***/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "popl.hpp"

using namespace popl;
using namespace std;

TEST_CASE("Main test")
{
    OptionParser op("Allowed options");
    std::vector<const char*> args = {"popl", "-h"};

    auto help_option = op.add<Switch>("h", "help", "produce help message");
    auto int_option = op.add<Value<int>>("i", "int", "test for int value", 42);

    try
    {
        op.parse(args.size(), args.data());
        REQUIRE(help_option->count() == 1);
        REQUIRE(int_option->is_set() == false);
        REQUIRE(int_option->count() == 0);
        REQUIRE(int_option->value() == 42);
    }
    catch (const std::exception& e)
    {
        cerr << "Exception: " << e.what() << "\n";
    }
}
