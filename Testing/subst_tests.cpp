#include "catch.h"
#include "../Expressions/num_expr.h"
#include "../Expressions/add_expr.h"
#include "../Expressions/mult_expr.h"
#include "../Expressions/var_expr.h"
#include "../Expressions/let_expr.h"
#include "../Expressions/bool_expr.h"

TEST_CASE("Default test: Subst after replacement - should be equal") {
    CHECK ((new MultExpr(new VarExpr("x"), new NumExpr(7)))
                   ->subst("x", new VarExpr("y"))
                   ->equals(new MultExpr(new VarExpr("y"), new NumExpr(7))));
}

TEST_CASE("Expressions containing no matching variables should stay the same after subst") {
    NumExpr numOne = NumExpr(10);
    MultExpr multOne = MultExpr(&numOne, &numOne);
    AddExpr addOne = AddExpr(&multOne, &numOne);
    VarExpr varOne = VarExpr("y");
    AddExpr addTwo = AddExpr(&numOne, &varOne);
    LetExpr letOne = LetExpr(&varOne, &addTwo, &addTwo); // var "y" is in both rhs and body
    BoolExpr tr = BoolExpr(true);
    BoolExpr fls = BoolExpr(false);

    // Random expression to put in place of variable "x"
    // Just a placeholder: No variable "x" in the expressions, so this random replacement expr should not be used
    MultExpr randomExpr = MultExpr(&multOne, &addOne);

    Expr *substNumOne = numOne.subst("x", &randomExpr);
    Expr *substMultOne = multOne.subst("x", &randomExpr);
    Expr *substAddOne = addOne.subst("x", &randomExpr);
    Expr *substVarOne = varOne.subst("x", &randomExpr);
    Expr *substLetOne = letOne.subst("x", &randomExpr);
    Expr *substTr = tr.subst("x", &randomExpr);
    Expr *substFls = fls.subst("x", &randomExpr);

    CHECK(numOne.equals(substNumOne));
    CHECK(multOne.equals((substMultOne)));
    CHECK(addOne.equals(substAddOne));
    CHECK(varOne.equals(substVarOne));
    CHECK(letOne.equals(substLetOne));
    CHECK(tr.equals(substTr));
    CHECK(fls.equals(substFls));
}

TEST_CASE("Expressions containing matching variables should change after subst") {
    // Set up
    NumExpr numOne = NumExpr(10);
    VarExpr varOne = VarExpr("thisShouldChange");
    VarExpr varTwo = VarExpr("thisShouldNotChange");
    LetExpr letOne = LetExpr(new VarExpr("x"),
                             new AddExpr(&numOne, new VarExpr("x")),
                             new AddExpr(&numOne, new VarExpr("x")));
    /*  LetExpr Case 1: VarExpr to replace and var bound by _let are the same. Should NOT go into the body
     *  letOne pretty printed:
     *  _let x = 10 + x
     *  _in  10 + x
     *  Replacement: x -> changed
     *  Expected expression after subst: see variable substLetOne
     */
    LetExpr letTwo = LetExpr(new VarExpr("y"),
                             new AddExpr(&numOne, new VarExpr("x")),
                             new AddExpr(&numOne, new VarExpr("x")));
    /*  LetExpr Case 2: VarExpr to replace and var bound by _let are different. Subst SHOULD go into the body
     *  letTwo pretty printed:
     *  _let y = 10 + x
     *  _in  10 + x
     *  Replacement: x -> changed
     *  Expected expression after subst: see variable substLetTwo
     */

    // Expressions before subst
    MultExpr multOne = MultExpr(&numOne, &varOne);
    AddExpr addOne = AddExpr(&multOne, &varTwo);

    // Replacement variable
    VarExpr varThree = VarExpr("changed");

    // Expected expressions after subst
    MultExpr substMultOne = MultExpr(&numOne, &varThree);
    AddExpr substAddOne = AddExpr(&substMultOne, &varTwo);
    LetExpr substLetOne = LetExpr(new VarExpr("x"), // lhs variable should NOT be substituted
                                  new AddExpr(&numOne, &varThree), // rhs var should be subst-ed
                                  new AddExpr(&numOne, new VarExpr("x"))); // body var should NOT be subst-ed
    /*  LetExpr Case 1: VarExpr to replace and var bound by _let are the same. Should NOT go into the body
     *  Expected subst result substLetOne:
     *  _let x = 10 + changed
     *  _in  10 + x
     *  Replacement: x -> changed
     */
    LetExpr substLetTwo = LetExpr(new VarExpr("y"),
                                  new AddExpr(&numOne, &varThree),
                                  new AddExpr(&numOne, &varThree));
    /*  LetExpr Case 2: VarExpr to replace and var bound by _let are different. Subst SHOULD go into the body
     *  Expected subst result substLetTwo:
     *  _let y = 10 + changed
     *  _in  10 + changed
     *  Replacement: x -> changed
     */

    CHECK(varThree.equals(varOne.subst("thisShouldChange", &varThree)));
    CHECK(substMultOne.equals(multOne.subst("thisShouldChange", &varThree)));
    CHECK(substAddOne.equals(addOne.subst("thisShouldChange", &varThree)));
    CHECK(substLetOne.equals(letOne.subst("x", &varThree)));
    CHECK(substLetTwo.equals(letTwo.subst("x", &varThree)));
}