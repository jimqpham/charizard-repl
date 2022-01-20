#include "catch.h"
#include "../Expressions/num.h"
#include "../Expressions/add.h"
#include "../Expressions/mult.h"
#include "../Expressions/var.h"

#include <stdexcept>

TEST_CASE("Interp Tests on Num Objects") {
    Num posNum = Num(10);
    Num negNum = Num(-10);

    SECTION("Should interp the correct val of Num") {
        CHECK(posNum.interp() == 10);
        CHECK(negNum.interp() == -10);
    }

    SECTION("Interps of two equal Num objects should be equal") {
        Num otherPosNum = Num(10);
        CHECK(posNum.interp() == otherPosNum.interp());
    }
}

TEST_CASE("Interp Tests on Add Objects") {
    Num numOne = Num(10);
    Num numTwo = Num(-20);
    Add addOne = Add(&numOne, &numTwo);
    Add addTwo = Add(&addOne, &numTwo);
    Add addThree = Add(&addOne, &addTwo);

    SECTION("Should interp the correct val of Add") {
        CHECK(addOne.interp() == -10);
        CHECK(addTwo.interp() == -30);
        CHECK(addThree.interp() == -40);
    }

    SECTION("Interps of two equal Add objects should be equal") {
        Num otherNumOne = Num(10);
        Num otherNumTwo = Num(-20);
        CHECK(addOne.interp() == Add(&otherNumOne, &otherNumTwo).interp());
    }
}

TEST_CASE("Interp Tests on Mult Objects") {
    Num numOne = Num(10);
    Num numTwo = Num(-20);
    Mult multOne = Mult(&numOne, &numTwo);
    Mult multTwo = Mult(&multOne, &numTwo);
    Mult multThree = Mult(&multOne, &multTwo);

    SECTION("Should interp the correct val of Mult") {
        CHECK(multOne.interp() == -200);
        CHECK(multTwo.interp() == 4000);
        CHECK(multThree.interp() == -800000);
    }

    SECTION("Interps of two equal Mult objects should be equal") {
        Num otherNumOne = Num(10);
        Num otherNumTwo = Num(-20);
        CHECK(multOne.interp() == Mult(&otherNumOne, &otherNumTwo).interp());
    }
}

TEST_CASE("Interp Tests on Var Objects") {
    Var var = Var("elephant");
    Num num = Num(10);
    Add add = Add(&num, &var);

    SECTION("Interp on Var objects should throw an error") {
        CHECK_THROWS_WITH( var.interp(), "No value for variable" );
    }

    SECTION("Interp on Expr objects encapsulating a Var should throw an error") {
        CHECK_THROWS_WITH( add.interp(), "No value for variable" );
    }
}