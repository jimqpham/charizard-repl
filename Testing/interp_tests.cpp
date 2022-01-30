#include "catch.h"
#include "../Expressions/num.h"
#include "../Expressions/add.h"
#include "../Expressions/mult.h"
#include "../Expressions/var.h"
#include "../Expressions/let.h"

#include <stdexcept>

TEST_CASE("Interp Tests on Num Objects") {

    // SET UP
    Num num1 = Num(3);
    Num num2 = Num(-2);
    Var var1 = Var("x");
    Var var2 = Var("y");

    Add add1 = Add(&num1, &num2); //"3 + -2" = 1
    Mult mult1 = Mult(&num1, &num2); //"3 * -2" = -6
    Let let1 = Let(&var1, &num1, new Add(&var1, &num2)); //"_let x=3 _in (x+-2)" = 1

    SECTION("Should evaluate Number") {
        CHECK(num1.interp() == 3);
        CHECK(num2.interp() == -2);
    }

    SECTION("Should evaluate Var") {
        CHECK_THROWS_WITH( var1.interp(), "No value for variable" );
        CHECK_THROWS_WITH( var2.interp(), "No value for variable" );
    }

    SECTION("Should evaluate Add") {
        Add add2 = Add(&num1, &var1);
        Add add3 = Add(&num1, &add1);
        Add add4 = Add(&num1, &mult1);

        Add add5 = Add(&var1, &num1);
        Add add6 = Add(&var1, &var2);
        Add add7 = Add(&var1, &add1);
        Add add8 = Add(&var1, &mult1);

        Add add9 = Add(&add1, &num1);
        Add add10 = Add(&add1, &var1);
        Add add11 = Add(&add1, &add1);
        Add add12 = Add(&add1, &mult1);

        Add add13 = Add(&mult1, &num1);
        Add add14 = Add(&mult1, &var1);
        Add add15 = Add(&mult1, &add1);
        Add add16 = Add(&mult1, &mult1);

        CHECK(add1.interp() == 1);
        CHECK_THROWS_WITH( add2.interp(), "No value for variable" );
        CHECK(add3.interp() == 4);
        CHECK(add4.interp() == -3);

        CHECK_THROWS_WITH( add5.interp(), "No value for variable" );
        CHECK_THROWS_WITH( add6.interp(), "No value for variable" );
        CHECK_THROWS_WITH( add7.interp(), "No value for variable" );
        CHECK_THROWS_WITH( add8.interp(), "No value for variable" );

        CHECK(add9.interp() == 4);
        CHECK_THROWS_WITH( add10.interp(), "No value for variable" );
        CHECK(add11.interp() == 2);
        CHECK(add12.interp() == -5);

        CHECK(add13.interp() == -3);
        CHECK_THROWS_WITH( add14.interp(), "No value for variable" );
        CHECK(add15.interp() == -5);
        CHECK(add16.interp() == -12);
    }


    SECTION("Should evaluate Mult") {
        Mult mult2 = Mult(&num1, &var1);
        Mult mult3 = Mult(&num1, &add1);
        Mult mult4 = Mult(&num1, &mult1);

        Mult mult5 = Mult(&var1, &num1);
        Mult mult6 = Mult(&var1, &var2);
        Mult mult7 = Mult(&var1, &add1);
        Mult mult8 = Mult(&var1, &mult1);

        Mult mult9 = Mult(&add1, &num1);
        Mult mult10 = Mult(&add1, &var1);
        Mult mult11 = Mult(&add1, &add1);
        Mult mult12 = Mult(&add1, &mult1);

        Mult mult13 = Mult(&mult1, &num1);
        Mult mult14 = Mult(&mult1, &var1);
        Mult mult15 = Mult(&mult1, &add1);
        Mult mult16 = Mult(&mult1, &mult1);

        CHECK(mult1.interp() == -6);
        CHECK_THROWS_WITH( mult2.interp(), "No value for variable" );
        CHECK(mult3.interp() == 3);
        CHECK(mult4.interp() == -18);

        CHECK_THROWS_WITH( mult5.interp(), "No value for variable" );
        CHECK_THROWS_WITH( mult6.interp(), "No value for variable" );
        CHECK_THROWS_WITH( mult7.interp(), "No value for variable" );
        CHECK_THROWS_WITH( mult8.interp(), "No value for variable" );

        CHECK(mult9.interp() == 3);
        CHECK_THROWS_WITH( mult10.interp(), "No value for variable" );
        CHECK(mult11.interp() == 1);
        CHECK(mult12.interp() == -6);

        CHECK(mult13.interp() == -18);
        CHECK_THROWS_WITH( mult14.interp(), "No value for variable" );
        CHECK(mult15.interp() == -6);
        CHECK(mult16.interp() == 36);
    }

    SECTION("Should evaluate Let") {

        Let let2 = Let(&var2, &num1, new Add(&var1, &num2)); //"_let y=3 _in (x+-2)" = 1
        Let let3 = Let(&var1, &num2, &let1); // "_let x=-2 _in (_let x=3 _in (x+-2))"
        Let let4 = Let(&var1, &num2, &let2); // "_let x=-2 _in (_let x=3 _in (x+-2))"
        Let let5 = Let(&var1, &let1, new Add(&var1, &num2)); // "_let x=(_let x=3 _in (x+-2)) _in (x+-2)"
        Let let6 = Let(&var2, &num2, &let2); // "_let y=-2 _in (_let y=3 _in (x+-2))"

        CHECK(let1.interp() == 1); // All vars are bound with a concrete value
        CHECK_THROWS_WITH(let2.interp(), "No value for variable"); // Some vars are not bound with a concrete value
        CHECK(let3.interp() == 1); // Nested lets - binding of x is both in inner & outer lets - should go with inner
        CHECK(let4.interp() == -4); // Nested lets - binding of x is in outer let
        CHECK(let5.interp() == -1); // Nested lets - rhs is a Let expr
        CHECK_THROWS_WITH(let6.interp(), "No value for variable"); // Nested lets - no bindings of x in any lets
    }
}