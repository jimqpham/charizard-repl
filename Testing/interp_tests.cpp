#include "catch.h"
#include "../Expressions/num.h"
#include "../Expressions/add.h"
#include "../Expressions/mult.h"
#include "../Expressions/var.h"

#include <stdexcept>

TEST_CASE("Interp Tests on Num Objects") {

    Num num1 = Num(30);
    Num num2 = Num(-12);
    Var var1 = Var("x");
    Var var2 = Var("y");

    Add add1 = Add(&num1, &num2); //"30 + -12"
    Mult mult1 = Mult(&num1, &num2); //"30 * -12"

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

    SECTION("Should evaluate Number") {
        CHECK(num1.interp() == 30);
        CHECK(num2.interp() == -12);
    }

    SECTION("Should evaluate Var") {
        CHECK_THROWS_WITH( var1.interp(), "No value for variable" );
        CHECK_THROWS_WITH( var2.interp(), "No value for variable" );
    }

    SECTION("Should evaluate Add") {
        CHECK(add1.interp() == 18);
        CHECK_THROWS_WITH( add2.interp(), "No value for variable" );
        CHECK(add3.interp() == 48);
        CHECK(add4.interp() == -330);

        CHECK_THROWS_WITH( add5.interp(), "No value for variable" );
        CHECK_THROWS_WITH( add6.interp(), "No value for variable" );
        CHECK_THROWS_WITH( add7.interp(), "No value for variable" );
        CHECK_THROWS_WITH( add8.interp(), "No value for variable" );

        CHECK(add9.interp() == 48);
        CHECK_THROWS_WITH( add10.interp(), "No value for variable" );
        CHECK(add11.interp() == 36);
        CHECK(add12.interp() == -342);

        CHECK(add13.interp() == -330);
        CHECK_THROWS_WITH( add14.interp(), "No value for variable" );
        CHECK(add15.interp() == -342);
        CHECK(add16.interp() == -720);
    }


    SECTION("Should evaluate Mult") {
        CHECK(mult1.interp() == -360);
        CHECK_THROWS_WITH( mult2.interp(), "No value for variable" );
        CHECK(mult3.interp() == 540);
        CHECK(mult4.interp() == -10800);

        CHECK_THROWS_WITH( mult5.interp(), "No value for variable" );
        CHECK_THROWS_WITH( mult6.interp(), "No value for variable" );
        CHECK_THROWS_WITH( mult7.interp(), "No value for variable" );
        CHECK_THROWS_WITH( mult8.interp(), "No value for variable" );

        CHECK(mult9.interp() == 540);
        CHECK_THROWS_WITH( mult10.interp(), "No value for variable" );
        CHECK(mult11.interp() == 324);
        CHECK(mult12.interp() == -6480);

        CHECK(mult13.interp() == -10800);
        CHECK_THROWS_WITH( mult14.interp(), "No value for variable" );
        CHECK(mult15.interp() == -6480);
        CHECK(mult16.interp() == 129600);
    }
}