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
#include "../Expressions/fun_expr.h"
#include "../Vals/fun_val.h"
#include "../Expressions/call_expr.h"
#include "../CmdLines/parse.h"
#include "../shared_ptr.h"

#include <stdexcept>

TEST_CASE("Interp Tests on NumExpr Objects") {

    // SET UP
    NumExpr num1 = NumExpr(3);
    NumExpr num2 = NumExpr(-2);
    VarExpr var1 = VarExpr("x");
    VarExpr var2 = VarExpr("y");
    BoolExpr tr = BoolExpr(true);
    BoolExpr fls = BoolExpr(false);
    FunExpr fun1 = FunExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(1)));
    CallExpr call1 = CallExpr(&fun1, new NumExpr(1));

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
        CHECK(add1.interp()->value_equals(new NumVal(1)));
        CHECK_THROWS_WITH(AddExpr(&num1, &var1).interp(), "No value for variable");
        CHECK(AddExpr(&num1, &add1).interp()->value_equals(new NumVal(4)));
        CHECK(AddExpr(&num1, &mult1).interp()->value_equals(new NumVal(-3)));

        CHECK_THROWS_WITH(AddExpr(&var1, &num1).interp(), "No value for variable");
        CHECK_THROWS_WITH(AddExpr(&var1, &var2).interp(), "No value for variable");
        CHECK_THROWS_WITH(AddExpr(&var1, &add1).interp(), "No value for variable");
        CHECK_THROWS_WITH(AddExpr(&var1, &mult1).interp(), "No value for variable");

        CHECK(AddExpr(&add1, &num1).interp()->value_equals(new NumVal(4)));
        CHECK_THROWS_WITH(AddExpr(&add1, &var1).interp(), "No value for variable");
        CHECK(AddExpr(&add1, &add1).interp()->value_equals(new NumVal(2)));
        CHECK(AddExpr(&add1, &mult1).interp()->value_equals(new NumVal(-5)));

        CHECK(AddExpr(&mult1, &num1).interp()->value_equals(new NumVal(-3)));
        CHECK_THROWS_WITH(AddExpr(&mult1, &var1).interp(), "No value for variable");
        CHECK(AddExpr(&mult1, &add1).interp()->value_equals(new NumVal(-5)));
        CHECK(AddExpr(&mult1, &mult1).interp()->value_equals(new NumVal(-12)));

        CHECK_THROWS_WITH(AddExpr(new BoolExpr(true), &num1).interp(), "add of non-number");
        CHECK_THROWS_WITH(AddExpr(&num1, new EqualExpr(new NumExpr(1),
                                                       new NumExpr(2))).interp(), "add of non-number");
        CHECK(AddExpr(&num1, new IfExpr(new BoolExpr(true),
                                        &num1,
                                        &num2)).interp()->value_equals(new NumVal(6)));
    }


    SECTION("Should evaluate MultExpr") {
        CHECK(mult1.interp()->value_equals(new NumVal(-6)));
        CHECK_THROWS_WITH(MultExpr(&num1, &var1).interp(), "No value for variable");
        CHECK(MultExpr(&num1, &add1).interp()->value_equals(new NumVal(3)));
        CHECK(MultExpr(&num1, &mult1).interp()->value_equals(new NumVal(-18)));

        CHECK_THROWS_WITH(MultExpr(&var1, &num1).interp(), "No value for variable");
        CHECK_THROWS_WITH(MultExpr(&var1, &var2).interp(), "No value for variable");
        CHECK_THROWS_WITH(MultExpr(&var1, &add1).interp(), "No value for variable");
        CHECK_THROWS_WITH(MultExpr(&var1, &mult1).interp(), "No value for variable");

        CHECK(MultExpr(&add1, &num1).interp()->value_equals(new NumVal(3)));
        CHECK_THROWS_WITH(MultExpr(&add1, &var1).interp(), "No value for variable");
        CHECK(MultExpr(&add1, &add1).interp()->value_equals(new NumVal(1)));
        CHECK(MultExpr(&add1, &mult1).interp()->value_equals(new NumVal(-6)));

        CHECK(MultExpr(&mult1, &num1).interp()->value_equals(new NumVal(-18)));
        CHECK_THROWS_WITH(MultExpr(&mult1, &var1).interp(), "No value for variable");
        CHECK(MultExpr(&mult1, &add1).interp()->value_equals(new NumVal(-6)));
        CHECK(MultExpr(&mult1, &mult1).interp()->value_equals(new NumVal(36)));

        CHECK_THROWS_WITH(MultExpr(new EqualExpr(&num1, &add1), &num1).interp(), "mult of non-number");
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
        CHECK(let1.interp()->value_equals(new NumVal(1))); // All vars are bound with a concrete value

        CHECK_THROWS_WITH(parse_str("_let y=3 _in (x+-2)")->interp(),
                          "No value for variable"); // Some vars are not bound with a concrete value

        CHECK(parse_str("_let x=-2 _in (_let x=3 _in (x+-2))")->interp()->value_equals(
                new NumVal(1))); // Nested lets - binding of x is both in inner & outer lets - should go with inner

        CHECK(parse_str("_let x=-2 _in (_let x=3 _in (x+-2))")->interp()->value_equals(
                new NumVal(1))); // Nested lets - binding of x is in outer let

        CHECK(parse_str("_let x=(_let x=3 _in (x+-2)) _in (x+-2)")->interp()->value_equals(
                new NumVal(-1))); // Nested lets - rhs is a LetExpr body

        CHECK_THROWS_WITH(parse_str("_let y=-2 _in (_let y=3 _in (x+-2))")->interp(),
                          "No value for variable"); // Nested lets - no bindings of x in any lets
    }

    SECTION("Should evaluate IfExpr") {

        CHECK(parse_str("_if _true _then 10 _else -10")->interp()->value_equals(new NumVal(10)));

        CHECK(parse_str("_if _false _then 10 _else -10")->interp()->value_equals(new NumVal(-10)));

        CHECK(parse_str("_if (1+1) == 2 _then 10 _else -10")->interp()->value_equals(new NumVal(10)));

        CHECK(parse_str("_if _true == 1 _then 10 _else -10")->interp()->value_equals(new NumVal(-10)));

        CHECK_THROWS_WITH((new IfExpr(new NumExpr(1), new NumExpr(10), new NumExpr(-10)))
                                  ->interp(), "non-boolean condition in if body");

        CHECK(parse_str("_let same = (1 == 2) _in _if 1 == 2 "
                        "                            _then _false + 5 "
                        "                            _else 88")->interp()->value_equals(new NumVal(88)));

        CHECK_THROWS_WITH(parse_str("_if (4 + 1) _then 2 _else 3")->interp(), "non-boolean condition in if body");
    }

    SECTION("Should evaluate FunExpr") {
        AddExpr body = AddExpr(new VarExpr("x"), new NumExpr(1));
        CHECK(fun1.interp()->value_equals(new FunVal("x", &body)));
        CHECK(!fun1.interp()->value_equals(new FunVal("x1", &body)));
        CHECK(!fun1.interp()->value_equals(new FunVal("x1", &add1)));

        CHECK(parse_str("_let f = _fun (x)\n"
                        "x + 1\n"
                        "_in f(5)")->interp()->to_string() == "6");

        CHECK(parse_str("_let f = _fun (x)\n"
                        "7\n"
                        "_in f(5)")->interp()->to_string() == "7");

        CHECK(parse_str("_let f = _fun (x)\n"
                        "_true\n"
                        "_in f(5)")->interp()->to_string() == "_true");

        CHECK_THROWS_WITH(parse_str("_let f = _fun (x)\n"
                                    "x + _true\n"
                                    "_in f(5)")->interp(), "add of non-number");

        CHECK(parse_str("_let f = _fun (x)\n"
                        "x + _true\n"
                        "_in 5 + 1")->interp()->to_string() == "6");

        CHECK_THROWS_WITH(parse_str("_let f = _fun (x)\n"
                                    "7\n"
                                    "_in f(5 + _true)")->interp(), "add of non-number");

        CHECK_THROWS_WITH(parse_str("_let f = _fun (x)\n"
                                    "x + 1\n"
                                    "_in f + 5")->interp(), "add of non-number");

        CHECK(parse_str("_let f = _fun (x)\n"
                        "x + 1\n"
                        "_in _if _false\n"
                        "    _then f(5)\n"
                        "    _else f(6)")->interp()->to_string() == "7");

        CHECK(parse_str("_let f = _fun (x)\n"
                        "              x + 1\n"
                        "_in _let g = _fun (y)\n"
                        "               y + 2\n"
                        "_in _if _true\n"
                        "    _then f(5)\n"
                        "    _else g(5)")->interp()->to_string() == "6");

        CHECK(parse_str("_let f = _fun (x)\n"
                        "              x + 1\n"
                        "_in _let g = _fun (y)\n"
                        "               y + 2\n"
                        "    _in f(g(5))")->interp()->to_string() == "8");

        CHECK(parse_str("_let f = _fun (x)\n"
                        "              x + 1\n"
                        "_in _let g = _fun (y)\n"
                        "               f(y + 2)\n"
                        "    _in g(5)")->interp()->to_string() == "8");

        CHECK(parse_str("_let f = _fun (x)\n"
                        "              x + 1\n"
                        "_in _let g = _fun (x)\n"
                        "               f(2) + x\n"
                        "    _in g(5)")->interp()->to_string() == "8");;

        CHECK(parse_str("_let f = _if _false\n"
                        "_then _fun (x)\n"
                        "x + 1\n"
                        "_else _fun (x)\n"
                        "x + 2\n"
                        "_in f(5)")->interp()->to_string() == "7");;

        CHECK(parse_str("(_if _false\n"
                        "_then _fun (x)\n"
                        "x + 1\n"
                        "_else _fun (x)\n"
                        "x + 2)(5)")->interp()->to_string() == "7");

        CHECK(parse_str("_let f = _fun (g)\n"
                        "g(5)\n"
                        "_in _let g = _fun (y)\n"
                        "y + 2\n"
                        "_in f(g)")->interp()->to_string() == "7");

        CHECK(parse_str("_let f = _fun (g)\n"
                        "g(5)\n"
                        "_in f(_fun (y)\n"
                        "y + 2)")->interp()->to_string() == "7");;

        CHECK(parse_str("_let f = _fun (x)\n"
                        "_fun (y)\n"
                        "x + y\n"
                        "_in (f(5))(1)")->interp()->to_string() == "6");

        CHECK(parse_str("_let f = _fun (x)\n"
                        "_fun (y)\n"
                        "x + y\n"
                        "_in f(5)(1)")->interp()->to_string() == "6");;

        CHECK(parse_str("_let f = _fun (x)\n"
                        "_fun (g)\n"
                        "g(x + 1)\n"
                        "_in _let g = _fun (y)\n"
                        "y + 2\n"
                        "_in (f(5))(g)")->interp()->to_string() == "8");

        CHECK_THROWS_WITH(parse_str("_let f = _fun (x)\n"
                                    "_fun (g)\n"
                                    "g(x + 1)\n"
                                    "_in _let g = _fun (y)\n"
                                    "y + 2\n"
                                    "_in f(5(g))")->interp(), "call of non-function val");

        CHECK(parse_str("_let f = _fun (x)\n"
                        "_fun (g)\n"
                        "g(x + 1)\n"
                        "_in _let g = _fun (y)\n"
                        "y + 2\n"
                        "_in f(5)(g)")->interp()->to_string() == "8");

        CHECK(parse_str("_let f = _fun (f)\n"
                        "_fun (x)\n"
                        "_if x == 0\n"
                        "_then 0\n"
                        "_else x + f(f)(x + -1)\n"
                        "_in f(f)(3)")->interp()->to_string() == "6");
    }

    SECTION("Should evaluate CallExpr") {
        CHECK(call1.interp()->value_equals(new NumVal(2)));
        CHECK(parse_str("_fun (x) x + 1")->interp()->to_string() == "(_fun (x) (x+1))");
        CHECK(parse_str("_let f = _fun (x) x + 1 _in f")->interp()->to_string() == "(_fun (x) (x+1))");
    }
}