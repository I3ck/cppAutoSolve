#define CATCH_CONFIG_MAIN
#include "../dependencies/Catch.h" //https://github.com/philsquared/Catch

using namespace std;

#include "../cppAutoSolve.h"

TEST_CASE("first case") {

    SECTION("first section") {
        REQUIRE(true);
    }
}
