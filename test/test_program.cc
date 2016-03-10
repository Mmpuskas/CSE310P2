#include "catch.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

// Go here for more information on using catch for unit testing:
// https://github.com/philsquared/Catch

TEST_CASE("simple test") {
    SECTION("sub test") {
        REQUIRE(strcmp("Hi there", "Hi there") == 0);
    }
}
