#include "catch.hpp"
#include "../include/program.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Go here for more information on using catch for unit testing:
// https://github.com/philsquared/Catch

TEST_CASE("simple test") {
    SECTION("sub test") {
        REQUIRE(strcmp("Hi there", sayHi()) == 0);
    }
}
