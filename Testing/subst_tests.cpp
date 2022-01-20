#include "catch.h"
#include "../Expressions/num.h"
#include "../Expressions/add.h"
#include "../Expressions/mult.h"
#include "../Expressions/var.h"

TEST_CASE("Default test: Subst after replacement - should be equal") {
    CHECK ((new Mult(new Var("x"), new Num(7)))
    ->subst("x", new Var("y"))
    ->equals(new Mult(new Var("y"), new Num(7))));
}

TEST_CASE("No-variable expressions should stay the same after subst") {
    Num numOne = Num(10);
    Mult multOne = Mult(&numOne, &numOne);
    Add addOne = Add(&multOne, &numOne);

    Mult randomExpr = Mult(&multOne, &addOne);

    Expr* substNumOne = numOne.subst("x", &randomExpr);
    Expr* substMultOne = multOne.subst("x", &randomExpr);
    Expr* substAddOne = addOne.subst("x", &randomExpr);

    CHECK(numOne.equals(substNumOne));
    CHECK(multOne.equals((substMultOne)));
    CHECK(addOne.equals(substAddOne));
}

TEST_CASE("Variable-containing expressions should change after subst") {
    // Set up
    Num numOne = Num(10);
    Var varOne = Var("thisShouldChange");
    Var varTwo = Var("thisShouldNotChange");

    // Test expressions
    Mult multOne = Mult(&numOne, &varOne);
    Add addOne = Add(&multOne, &varTwo);

    // Replacement
    Var varThree = Var("changed");


    //Expr* substMultOne = multOne.subst("thisShouldChange", &varThree);
    //Expr* substAddOne = addOne.subst("thisShouldChange", &varThree);
    Mult substMultOne = Mult(&numOne, &varThree);
    Add substAddOne = Add(&substMultOne, &varTwo);

    CHECK(substMultOne.equals(multOne.subst("thisShouldChange", &varThree)));
    CHECK(substAddOne.equals(addOne.subst("thisShouldChange", &varThree)));
}