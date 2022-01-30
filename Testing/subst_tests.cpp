#include "catch.h"
#include "../Expressions/num.h"
#include "../Expressions/add.h"
#include "../Expressions/mult.h"
#include "../Expressions/var.h"
#include "../Expressions/let.h"

TEST_CASE("Default test: Subst after replacement - should be equal") {
    CHECK ((new Mult(new Var("x"), new Num(7)))
    ->subst("x", new Var("y"))
    ->equals(new Mult(new Var("y"), new Num(7))));
}

TEST_CASE("Expressions containing no matching variables should stay the same after subst") {
    Num numOne = Num(10);
    Mult multOne = Mult(&numOne, &numOne);
    Add addOne = Add(&multOne, &numOne);
    Var varOne = Var("y");
    Add addTwo = Add(&numOne, &varOne);
    Let letOne = Let(&varOne, &addTwo, &addTwo); // var "y" is in both rhs and body

    // Random expression to put in place of variable "x"
    // Just a placeholder: No variable "x" in the expressions, so this random replacement expr should not be used
    Mult randomExpr = Mult(&multOne, &addOne);

    Expr* substNumOne = numOne.subst("x", &randomExpr);
    Expr* substMultOne = multOne.subst("x", &randomExpr);
    Expr* substAddOne = addOne.subst("x", &randomExpr);
    Expr* substVarOne = varOne.subst("x", &randomExpr);
    Expr* substLetOne = letOne.subst("x", &randomExpr);

    CHECK(numOne.equals(substNumOne));
    CHECK(multOne.equals((substMultOne)));
    CHECK(addOne.equals(substAddOne));
    CHECK(varOne.equals(substVarOne));
    CHECK(letOne.equals(substLetOne));
}

TEST_CASE("Expressions containing matching variables should change after subst") {
    // Set up
    Num numOne = Num(10);
    Var varOne = Var("thisShouldChange");
    Var varTwo = Var("thisShouldNotChange");
    Add addTwo = Add(&numOne, &varOne);
    Let letOne = Let(&varOne, &addTwo, &addTwo); // var "thisShouldChange" is in both rhs and body

    // Expressions before subst
    Mult multOne = Mult(&numOne, &varOne);
    Add addOne = Add(&multOne, &varTwo);

    // Replacement variable
    Var varThree = Var("changed");

    // Expected expressions after subst
    Mult substMultOne = Mult(&numOne, &varThree);
    Add substAddOne = Add(&substMultOne, &varTwo);
    Let substLetOne = Let(&varOne, // lhs variable should NOT be substituted
                          new Add(&numOne, &varThree),
                          new Add(&numOne, &varThree));
    Expr* actualSubstLetOne = letOne.subst("thisShouldChange", &varThree);

    CHECK(varThree.equals(varOne.subst("thisShouldChange", &varThree)));
    CHECK(substMultOne.equals(multOne.subst("thisShouldChange", &varThree)));
    CHECK(substAddOne.equals(addOne.subst("thisShouldChange", &varThree)));
    CHECK(substLetOne.equals(letOne.subst("thisShouldChange", &varThree)));
}