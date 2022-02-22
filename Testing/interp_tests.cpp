#include "catch.h"
#include "../Expressions/num_expr.h"
#include "../Expressions/add_expr.h"
#include "../Expressions/mult_expr.h"
#include "../Expressions/var_expr.h"
#include "../Expressions/let_expr.h"
#include "../Vals/num_val.h"
#include "../Expressions/bool_expr.h"
#include "../Vals/bool_val.h"
#include "../Expressions/equal_expr.h"
#include "../Expressions/if_expr.h"

#include <stdexcept>

TEST_CASE("Interp Tests on NumExpr Objects") {

    // SET UP
    NumExpr num1 = NumExpr(3);
    NumExpr num2 = NumExpr(-2);
    VarExpr var1 = VarExpr("x");
    VarExpr var2 = VarExpr("y");
    BoolExpr tr = BoolExpr(true);
    BoolExpr fls = BoolExpr(false);

    AddExpr add1 = AddExpr(&num1, &num2); //"3 + -2" = 1
    MultExpr mult1 = MultExpr(&num1, &num2); //"3 * -2" = -6
    LetExpr let1 = LetExpr(&var1, &num1, new AddExpr(&var1, &num2)); //"_let x=3 _in (x+-2)" = 1

    SECTION("Should evaluate NumExpr") {
        CHECK(num1.interp()->value_equals(new NumVal(3)));
        CHECK(num2.interp()->value_equals(new NumVal(-2)));
    }

    SECTION("Should evaluate BoolExpr") {
        CHECK(tr.interp()->value_equals(new BoolVal(true)));
        CHECK(fls.interp()->value_equals(new BoolVal(false)));
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

        AddExpr add17 = AddExpr(new BoolExpr(true), &num1);
        AddExpr add18 = AddExpr(&num1, new EqualExpr(new NumExpr(1),
                                                     new NumExpr(2)));
        AddExpr add19 = AddExpr(&num1, new IfExpr(new BoolExpr(true),
                                                  &num1,
                                                  &num2));

        CHECK(add1.interp()->value_equals(new NumVal(1)));
        CHECK_THROWS_WITH(add2.interp(), "No value for variable");
        CHECK(add3.interp()->value_equals(new NumVal(4)));
        CHECK(add4.interp()->value_equals(new NumVal(-3)));

        CHECK_THROWS_WITH(add5.interp(), "No value for variable");
        CHECK_THROWS_WITH(add6.interp(), "No value for variable");
        CHECK_THROWS_WITH(add7.interp(), "No value for variable");
        CHECK_THROWS_WITH(add8.interp(), "No value for variable");

        CHECK(add9.interp()->value_equals(new NumVal(4)));
        CHECK_THROWS_WITH(add10.interp(), "No value for variable");
        CHECK(add11.interp()->value_equals(new NumVal(2)));
        CHECK(add12.interp()->value_equals(new NumVal(-5)));

        CHECK(add13.interp()->value_equals(new NumVal(-3)));
        CHECK_THROWS_WITH(add14.interp(), "No value for variable");
        CHECK(add15.interp()->value_equals(new NumVal(-5)));
        CHECK(add16.interp()->value_equals(new NumVal(-12)));

        CHECK_THROWS_WITH(add17.interp(), "add of non-number");
        CHECK_THROWS_WITH(add18.interp(), "add of non-number");
        CHECK(add19.interp()->value_equals(new NumVal(6)));
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

        MultExpr mult17 = MultExpr(new EqualExpr(&num1, &add1), &num1);

        CHECK(mult1.interp()->value_equals(new NumVal(-6)));
        CHECK_THROWS_WITH(mult2.interp(), "No value for variable");
        CHECK(mult3.interp()->value_equals(new NumVal(3)));
        CHECK(mult4.interp()->value_equals(new NumVal(-18)));

        CHECK_THROWS_WITH(mult5.interp(), "No value for variable");
        CHECK_THROWS_WITH(mult6.interp(), "No value for variable");
        CHECK_THROWS_WITH(mult7.interp(), "No value for variable");
        CHECK_THROWS_WITH(mult8.interp(), "No value for variable");

        CHECK(mult9.interp()->value_equals(new NumVal(3)));
        CHECK_THROWS_WITH(mult10.interp(), "No value for variable");
        CHECK(mult11.interp()->value_equals(new NumVal(1)));
        CHECK(mult12.interp()->value_equals(new NumVal(-6)));

        CHECK(mult13.interp()->value_equals(new NumVal(-18)));
        CHECK_THROWS_WITH(mult14.interp(), "No value for variable");
        CHECK(mult15.interp()->value_equals(new NumVal(-6)));
        CHECK(mult16.interp()->value_equals(new NumVal(36)));

        CHECK_THROWS_WITH(mult17.interp(), "mult of non-number");
    }

    SECTION("Should evaluate EqualExpr") {
        // Interp EqualExpr with NumExpr
        CHECK(EqualExpr(&num1, &num2).interp()->value_equals(new BoolVal(false)));
        CHECK(EqualExpr(&num1, new NumExpr(3)).interp()
                      ->value_equals(new BoolVal(true)));

        // Interp EqualExpr with Add
        CHECK(EqualExpr(&add1, new NumExpr(1)).interp()
                      ->value_equals(new BoolVal(true)));
        CHECK(EqualExpr(&add1, new NumExpr(3)).interp()
                      ->value_equals(new BoolVal(false)));

        // Interp EqualExpr with Mult
        CHECK(EqualExpr(&mult1, new NumExpr(-6)).interp()
                      ->value_equals(new BoolVal(true)));
        CHECK(EqualExpr(&mult1, new NumExpr(0)).interp()
                      ->value_equals(new BoolVal(false)));

        // Interp EqualExpr with Let
        CHECK(EqualExpr(&let1, &add1).interp()
                      ->value_equals(new BoolVal(true)));
        CHECK(EqualExpr(&let1, &mult1).interp()
                      ->value_equals(new BoolVal(false)));

        // Interp EqualExpr with Var
        CHECK_THROWS_WITH(EqualExpr(&var1, &add1).interp(), "No value for variable");
        CHECK_THROWS_WITH(EqualExpr(new LetExpr(new VarExpr("x"),
                                                new NumExpr(2),
                                                new AddExpr(new VarExpr("y"), new NumExpr(2))), &add1).interp(),
                          "No value for variable");

        // Interp EqualExpr with BoolExpr
        CHECK(EqualExpr(&tr, &fls).interp()->value_equals(new BoolVal(false)));
        CHECK(EqualExpr(&tr, &tr).interp()->value_equals(new BoolVal(true)));
        CHECK(EqualExpr(&tr, &num1).interp()->value_equals(new BoolVal(false)));

        // Interp EqualExpr with IfExpr
        CHECK(EqualExpr(new NumExpr(2),
                        new IfExpr(new BoolExpr(true),
                                   new NumExpr(2),
                                   new VarExpr("x"))).interp()->value_equals(new BoolVal(true)));
    }

    SECTION("Should evaluate LetExpr") {

        LetExpr let2 = LetExpr(&var2, &num1, new AddExpr(&var1, &num2)); //"_let y=3 _in (x+-2)" = 1
        LetExpr let3 = LetExpr(&var1, &num2, &let1); // "_let x=-2 _in (_let x=3 _in (x+-2))"
        LetExpr let4 = LetExpr(&var1, &num2, &let2); // "_let x=-2 _in (_let x=3 _in (x+-2))"
        LetExpr let5 = LetExpr(&var1, &let1, new AddExpr(&var1, &num2)); // "_let x=(_let x=3 _in (x+-2)) _in (x+-2)"
        LetExpr let6 = LetExpr(&var2, &num2, &let2); // "_let y=-2 _in (_let y=3 _in (x+-2))"

        CHECK(let1.interp()->value_equals(new NumVal(1))); // All vars are bound with a concrete value
        CHECK_THROWS_WITH(let2.interp(), "No value for variable"); // Some vars are not bound with a concrete value
        CHECK(let3.interp()->value_equals(
                new NumVal(1))); // Nested lets - binding of x is both in inner & outer lets - should go with inner
        CHECK(let4.interp()->value_equals(new NumVal(-4))); // Nested lets - binding of x is in outer let
        CHECK(let5.interp()->value_equals(new NumVal(-1))); // Nested lets - rhs is a LetExpr expr
        CHECK_THROWS_WITH(let6.interp(), "No value for variable"); // Nested lets - no bindings of x in any lets
    }

    SECTION("Should evaluate IfExpr") {

        NumExpr ten = NumExpr(10);
        NumExpr minusTen = NumExpr(-10);

        IfExpr if1 = IfExpr(new BoolExpr(true), &ten, &minusTen);
        IfExpr if2 = IfExpr(new BoolExpr(false), &ten, &minusTen);
        IfExpr if3 = IfExpr(new EqualExpr(
                                    new AddExpr(new NumExpr(1), new NumExpr(1)),
                                    new NumExpr(2)),
                            &ten,
                            &minusTen);
        IfExpr if4 = IfExpr(new EqualExpr(new BoolExpr(true), new NumExpr(1)),
                            &ten,
                            &minusTen);
        IfExpr if5 = IfExpr(new NumExpr(1), &ten, &minusTen);
        // Class example 1
        LetExpr if6 = LetExpr(new VarExpr("same"),
                              new EqualExpr(new NumExpr(1), new NumExpr(2)),
                              new IfExpr(new EqualExpr(new NumExpr(1), new NumExpr(2)),
                                         new AddExpr(new BoolExpr(false), new NumExpr(5)),
                                         new NumExpr(88)));
        // Class example 2
        IfExpr if7 = IfExpr(new AddExpr(new NumExpr(4), new NumExpr(1)),
                            new NumExpr(2),
                            new NumExpr(3));

        CHECK(if1.interp()->value_equals(new NumVal(10)));
        CHECK(if2.interp()->value_equals(new NumVal(-10)));
        CHECK(if3.interp()->value_equals(new NumVal(10)));
        CHECK(if4.interp()->value_equals(new NumVal(-10)));
        CHECK_THROWS_WITH(if5.interp(), "non-boolean condition in if expr");
        CHECK(if6.interp()->value_equals(new NumVal(88)));
        CHECK_THROWS_WITH(if7.interp(), "non-boolean condition in if expr");
    }
}