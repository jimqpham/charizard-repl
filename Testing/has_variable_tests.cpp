#include "catch.h"
#include "../Expressions/num_expr.h"
#include "../Expressions/add_expr.h"
#include "../Expressions/mult_expr.h"
#include "../Expressions/var_expr.h"
#include "../Expressions/let_expr.h"
#include "../Expressions/bool_expr.h"
#include "../Expressions/equal_expr.h"
#include "../Expressions/if_expr.h"

#include <stdexcept>

TEST_CASE("Test NumExpr has_variable") {
    NumExpr numOne = NumExpr(5);
    CHECK(!numOne.has_variable());
}

TEST_CASE("Test BoolExpr has_variable") {
    BoolExpr tr = BoolExpr(true);
    CHECK(!tr.has_variable());
}

TEST_CASE("Test VarExpr has_variable") {
    VarExpr varOne = VarExpr("x");
    CHECK(varOne.has_variable());
}

TEST_CASE("Test AddExpr has_variable") {
    NumExpr numOne = NumExpr(5);
    NumExpr numTwo = NumExpr(7);
    VarExpr varOne = VarExpr("x");

    AddExpr addOne = AddExpr(&numOne, &numTwo);
    AddExpr addTwo = AddExpr(&varOne, &numTwo);
    AddExpr addThree = AddExpr(&addOne, &varOne);
    AddExpr addFour = AddExpr(&varOne, &varOne);

    CHECK(!addOne.has_variable());
    CHECK(addTwo.has_variable());
    CHECK(addThree.has_variable());
    CHECK(addFour.has_variable());
}

TEST_CASE("Test MultExpr has_variable") {
    NumExpr numOne = NumExpr(5);
    NumExpr numTwo = NumExpr(7);
    VarExpr varOne = VarExpr("x");

    MultExpr multOne = MultExpr(&numOne, &numTwo);
    MultExpr multTwo = MultExpr(&varOne, &numTwo);
    MultExpr multThree = MultExpr(&multOne, &varOne);
    MultExpr multFour = MultExpr(&varOne, &varOne);

    CHECK(!multOne.has_variable());
    CHECK(multTwo.has_variable());
    CHECK(multThree.has_variable());
    CHECK(multFour.has_variable());
}

TEST_CASE("Test EqualExpr has_variable") {
    NumExpr five = NumExpr(5);
    VarExpr x = VarExpr("x");

    EqualExpr eq1 = EqualExpr(&five, &five);
    EqualExpr eq2 = EqualExpr(&five, &x);
    EqualExpr eq3 = EqualExpr(&x, &five);
    EqualExpr eq4 = EqualExpr(&x, &x);

    CHECK(!eq1.has_variable());
    CHECK(eq2.has_variable());
    CHECK(eq3.has_variable());
    CHECK(eq4.has_variable());
}

TEST_CASE("Test LetExpr has_variable") {
    NumExpr numOne = NumExpr(5);
    VarExpr varOne = VarExpr("x");
    AddExpr addOne = AddExpr(&numOne, &varOne); // Expr with var
    MultExpr multOne = MultExpr(&numOne, &numOne); // Expr without var

    CHECK(!LetExpr(&varOne, &numOne, &multOne).has_variable()); // rhs no var, body no var
    CHECK(LetExpr(&varOne, &varOne, &multOne).has_variable());  // rhs has var, body no var
    CHECK(LetExpr(&varOne, &numOne, &addOne).has_variable());   // rhs no var, body has var
    CHECK(LetExpr(&varOne, &varOne, &addOne).has_variable());   // rhs & body have var
}

TEST_CASE("Test IfExpr has_variable") {
    BoolExpr tr = BoolExpr(true);
    NumExpr one = NumExpr(1);
    VarExpr x = VarExpr("x");
    AddExpr addOne = AddExpr(&one, &x); // Expr with var
    MultExpr multOne = MultExpr(&one, &one); // Expr without var

    CHECK(!IfExpr(&tr, &one, &multOne).has_variable());  // no var
    CHECK(IfExpr(&x, &one, &multOne).has_variable());  // condition has var
    CHECK(IfExpr(&tr, &addOne, &multOne).has_variable());   // then branch has var
    CHECK(IfExpr(&tr, &multOne, &addOne).has_variable());   // else branch has var
}