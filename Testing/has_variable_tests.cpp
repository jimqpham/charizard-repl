#include "catch.h"
#include "../Expressions/num.h"
#include "../Expressions/add.h"
#include "../Expressions/mult.h"
#include "../Expressions/var.h"

#include <stdexcept>

TEST_CASE("Should return false if no variable") {
    Num numOne = Num(5);
    Num numTwo = Num(7);
    Add addOne = Add(&numOne, &numTwo);
    Mult multOne = Mult(&addOne, &numOne);

    CHECK(!numOne.has_variable());
    CHECK(!numTwo.has_variable());
    CHECK(!addOne.has_variable());
    CHECK(!multOne.has_variable());
}

TEST_CASE("Should return true if has variable") {
    Num numOne = Num(5);
    Var var = Var("giraffe");
    Add addOne = Add(&numOne, &var);
    Mult multOne = Mult(&addOne, &numOne);

    CHECK(var.has_variable());
    CHECK(addOne.has_variable());
    CHECK(multOne.has_variable());
}