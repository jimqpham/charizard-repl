#include "catch.h"
#include "../Expressions/num.h"
#include "../Expressions/add.h"
#include "../Expressions/mult.h"
#include "../Expressions/var.h"
#include "../Expressions/let.h"

#include <stdexcept>

TEST_CASE("Test Num has_variable") {
    Num numOne = Num(5);
    CHECK(!numOne.has_variable());
}

TEST_CASE("Test Var has_variable") {
    Var varOne = Var("x");
    CHECK(varOne.has_variable());
}

TEST_CASE("Test Add has_variable") {
    Num numOne = Num(5);
    Num numTwo = Num(7);
    Var varOne = Var("x");
    
    Add addOne = Add(&numOne, &numTwo);
    Add addTwo = Add(&varOne, &numTwo);
    Add addThree = Add(&addOne, &varOne);
    Add addFour = Add(&varOne, &varOne);
    
    CHECK(!addOne.has_variable());
    CHECK(addTwo.has_variable());
    CHECK(addThree.has_variable());
    CHECK(addFour.has_variable());
}

TEST_CASE("Test Mult has_variable") {
    Num numOne = Num(5);
    Num numTwo = Num(7);
    Var varOne = Var("x");

    Mult multOne = Mult(&numOne, &numTwo);
    Mult multTwo = Mult(&varOne, &numTwo);
    Mult multThree = Mult(&multOne, &varOne);
    Mult multFour = Mult(&varOne, &varOne);

    CHECK(!multOne.has_variable());
    CHECK(multTwo.has_variable());
    CHECK(multThree.has_variable());
    CHECK(multFour.has_variable());
}

TEST_CASE("Test Let has_variable") {
    Num numOne = Num(5);
    Var varOne = Var("x");
    Add addOne = Add(&numOne, &varOne); // Expr with var
    Mult multOne = Mult(&numOne, &numOne); // Expr without var

    CHECK(!Let(&varOne, &numOne, &multOne).has_variable()); // rhs no var, body no var
    CHECK(Let(&varOne, &varOne, &multOne).has_variable());  // rhs has var, body no var
    CHECK(Let(&varOne, &numOne, &addOne).has_variable());   // rhs no var, body has var
    CHECK(Let(&varOne, &varOne, &addOne).has_variable());   // rhs & body have var
}