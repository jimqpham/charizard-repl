#include "catch.h"
#include "../Expressions/num_expr.h"
#include "../Expressions/add_expr.h"
#include "../Expressions/mult_expr.h"
#include "../Expressions/var_expr.h"
#include "../Expressions/let_expr.h"

#include <stdexcept>

TEST_CASE("Interp Tests on NumExpr Objects") {

    // SET UP
    NumExpr num1 = NumExpr(3);
    NumExpr num2 = NumExpr(-2);
    VarExpr var1 = VarExpr("x");
    VarExpr var2 = VarExpr("y");

    AddExpr add1 = AddExpr(&num1, &num2); //"3 + -2" = 1
    MultExpr mult1 = MultExpr(&num1, &num2); //"3 * -2" = -6
    LetExpr let1 = LetExpr(&var1, &num1, new AddExpr(&var1, &num2)); //"_let x=3 _in (x+-2)" = 1

    SECTION("Should evaluate Number") {
        CHECK(num1.interp() == 3);
        CHECK(num2.interp() == -2);
    }

    SECTION("Should evaluate VarExpr") {
        CHECK_THROWS_WITH(var1.interp(), "No value for variable");
        CHECK_THROWS_WITH(var2.interp(), "No value for variable");
    }

    SECTION("Should evaluate AddExpr") {
        AddExpr add2 = AddExpr(&num1, &var1);
        AddExpr add3 = AddExpr(&num1, &add1);
        AddExpr add4 = AddExpr(&num1, &mult1);

        AddExpr add5 = AddExpr(&var1, &num1);
        AddExpr add6 = AddExpr(&var1, &var2);
        AddExpr add7 = AddExpr(&var1, &add1);
        AddExpr add8 = AddExpr(&var1, &mult1);

        AddExpr add9 = AddExpr(&add1, &num1);
        AddExpr add10 = AddExpr(&add1, &var1);
        AddExpr add11 = AddExpr(&add1, &add1);
        AddExpr add12 = AddExpr(&add1, &mult1);

        AddExpr add13 = AddExpr(&mult1, &num1);
        AddExpr add14 = AddExpr(&mult1, &var1);
        AddExpr add15 = AddExpr(&mult1, &add1);
        AddExpr add16 = AddExpr(&mult1, &mult1);

        CHECK(add1.interp() == 1);
        CHECK_THROWS_WITH(add2.interp(), "No value for variable");
        CHECK(add3.interp() == 4);
        CHECK(add4.interp() == -3);

        CHECK_THROWS_WITH(add5.interp(), "No value for variable");
        CHECK_THROWS_WITH(add6.interp(), "No value for variable");
        CHECK_THROWS_WITH(add7.interp(), "No value for variable");
        CHECK_THROWS_WITH(add8.interp(), "No value for variable");

        CHECK(add9.interp() == 4);
        CHECK_THROWS_WITH(add10.interp(), "No value for variable");
        CHECK(add11.interp() == 2);
        CHECK(add12.interp() == -5);

        CHECK(add13.interp() == -3);
        CHECK_THROWS_WITH(add14.interp(), "No value for variable");
        CHECK(add15.interp() == -5);
        CHECK(add16.interp() == -12);
    }


    SECTION("Should evaluate MultExpr") {
        MultExpr mult2 = MultExpr(&num1, &var1);
        MultExpr mult3 = MultExpr(&num1, &add1);
        MultExpr mult4 = MultExpr(&num1, &mult1);

        MultExpr mult5 = MultExpr(&var1, &num1);
        MultExpr mult6 = MultExpr(&var1, &var2);
        MultExpr mult7 = MultExpr(&var1, &add1);
        MultExpr mult8 = MultExpr(&var1, &mult1);

        MultExpr mult9 = MultExpr(&add1, &num1);
        MultExpr mult10 = MultExpr(&add1, &var1);
        MultExpr mult11 = MultExpr(&add1, &add1);
        MultExpr mult12 = MultExpr(&add1, &mult1);

        MultExpr mult13 = MultExpr(&mult1, &num1);
        MultExpr mult14 = MultExpr(&mult1, &var1);
        MultExpr mult15 = MultExpr(&mult1, &add1);
        MultExpr mult16 = MultExpr(&mult1, &mult1);

        CHECK(mult1.interp() == -6);
        CHECK_THROWS_WITH(mult2.interp(), "No value for variable");
        CHECK(mult3.interp() == 3);
        CHECK(mult4.interp() == -18);

        CHECK_THROWS_WITH(mult5.interp(), "No value for variable");
        CHECK_THROWS_WITH(mult6.interp(), "No value for variable");
        CHECK_THROWS_WITH(mult7.interp(), "No value for variable");
        CHECK_THROWS_WITH(mult8.interp(), "No value for variable");

        CHECK(mult9.interp() == 3);
        CHECK_THROWS_WITH(mult10.interp(), "No value for variable");
        CHECK(mult11.interp() == 1);
        CHECK(mult12.interp() == -6);

        CHECK(mult13.interp() == -18);
        CHECK_THROWS_WITH(mult14.interp(), "No value for variable");
        CHECK(mult15.interp() == -6);
        CHECK(mult16.interp() == 36);
    }

    SECTION("Should evaluate LetExpr") {

        LetExpr let2 = LetExpr(&var2, &num1, new AddExpr(&var1, &num2)); //"_let y=3 _in (x+-2)" = 1
        LetExpr let3 = LetExpr(&var1, &num2, &let1); // "_let x=-2 _in (_let x=3 _in (x+-2))"
        LetExpr let4 = LetExpr(&var1, &num2, &let2); // "_let x=-2 _in (_let x=3 _in (x+-2))"
        LetExpr let5 = LetExpr(&var1, &let1, new AddExpr(&var1, &num2)); // "_let x=(_let x=3 _in (x+-2)) _in (x+-2)"
        LetExpr let6 = LetExpr(&var2, &num2, &let2); // "_let y=-2 _in (_let y=3 _in (x+-2))"

        CHECK(let1.interp() == 1); // All vars are bound with a concrete value
        CHECK_THROWS_WITH(let2.interp(), "No value for variable"); // Some vars are not bound with a concrete value
        CHECK(let3.interp() == 1); // Nested lets - binding of x is both in inner & outer lets - should go with inner
        CHECK(let4.interp() == -4); // Nested lets - binding of x is in outer let
        CHECK(let5.interp() == -1); // Nested lets - rhs is a LetExpr expr
        CHECK_THROWS_WITH(let6.interp(), "No value for variable"); // Nested lets - no bindings of x in any lets
    }
}