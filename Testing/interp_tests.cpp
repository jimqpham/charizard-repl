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
    PTR(NumExpr) num1 = NEW(NumExpr)(3);
    PTR(NumExpr) num2 = NEW(NumExpr)(-2);
    PTR(VarExpr) var1 = NEW(VarExpr)("x");
    PTR(VarExpr) var2 = NEW(VarExpr)("y");
    PTR(BoolExpr) tr = NEW(BoolExpr)(true);
    PTR(BoolExpr) fls = NEW(BoolExpr)(false);
    PTR(FunExpr) fun1 = NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)));
    PTR(CallExpr) call1 = NEW(CallExpr)(fun1, NEW(NumExpr)(1));

    PTR(AddExpr) add1 = NEW(AddExpr)(num1, num2); //"3 + -2" = 1
    PTR(MultExpr) mult1 = NEW(MultExpr)(num1, num2); //"3 * -2" = -6
    PTR(LetExpr) let1 = NEW(LetExpr)(var1, num1, NEW(AddExpr)(var1, num2)); //"_let x=3 _in (x+-2)" = 1

    SECTION("Should evaluate NumExpr") {
        CHECK(num1->interp()->value_equals(NEW(NumVal)(3)));
        CHECK(num2->interp()->value_equals(NEW(NumVal)(-2)));
    }

    SECTION("Should evaluate BoolExpr") {
        CHECK(tr->interp()->value_equals(NEW(BoolVal)(true)));
        CHECK(fls->interp()->value_equals(NEW(BoolVal)(false)));
    }

    SECTION("Should evaluate VarExpr") {
        CHECK_THROWS_WITH(var1->interp(), "free variable: x");
    }

    SECTION("Should evaluate AddExpr") {
        CHECK(add1->interp()->value_equals(NEW(NumVal)(1)));
        CHECK_THROWS_WITH(NEW(AddExpr)(num1, var1)->interp(), "free variable: x");
        CHECK(NEW(AddExpr)(num1, add1)->interp()->value_equals(NEW(NumVal)(4)));
        CHECK(NEW(AddExpr)(num1, mult1)->interp()->value_equals(NEW(NumVal)(-3)));

        CHECK_THROWS_WITH(NEW(AddExpr)(var1, num1)->interp(), "free variable: x");
        CHECK_THROWS_WITH(NEW(AddExpr)(var1, var2)->interp(), "free variable: x");
        CHECK_THROWS_WITH(NEW(AddExpr)(var1, add1)->interp(), "free variable: x");
        CHECK_THROWS_WITH(NEW(AddExpr)(var1, mult1)->interp(), "free variable: x");

        CHECK(NEW(AddExpr)(add1, num1)->interp()->value_equals(NEW(NumVal)(4)));
        CHECK_THROWS_WITH(NEW(AddExpr)(add1, var1)->interp(), "free variable: x");
        CHECK(NEW(AddExpr)(add1, add1)->interp()->value_equals(NEW(NumVal)(2)));
        CHECK(NEW(AddExpr)(add1, mult1)->interp()->value_equals(NEW(NumVal)(-5)));

        CHECK(NEW(AddExpr)(mult1, num1)->interp()->value_equals(NEW(NumVal)(-3)));
        CHECK_THROWS_WITH(NEW(AddExpr)(mult1, var1)->interp(), "free variable: x");
        CHECK(NEW(AddExpr)(mult1, add1)->interp()->value_equals(NEW(NumVal)(-5)));
        CHECK(NEW(AddExpr)(mult1, mult1)->interp()->value_equals(NEW(NumVal)(-12)));

        CHECK_THROWS_WITH(NEW(AddExpr)(NEW(BoolExpr)(true), num1)->interp(), "add of non-number");
        CHECK_THROWS_WITH(NEW(AddExpr)(num1, NEW(EqualExpr)(NEW(NumExpr)(1),
                                                            NEW(NumExpr)(2)))->interp(), "add of non-number");
        CHECK(NEW(AddExpr)(num1, NEW(IfExpr)(NEW(BoolExpr)(true),
                                             num1,
                                             num2))->interp()->value_equals(NEW(NumVal)(6)));
    }


    SECTION("Should evaluate MultExpr") {
        CHECK(mult1->interp()->value_equals(NEW(NumVal)(-6)));
        CHECK_THROWS_WITH(NEW(MultExpr)(num1, var1)->interp(), "free variable: x");
        CHECK(NEW(MultExpr)(num1, add1)->interp()->value_equals(NEW(NumVal)(3)));
        CHECK(NEW(MultExpr)(num1, mult1)->interp()->value_equals(NEW(NumVal)(-18)));

        CHECK_THROWS_WITH(NEW(MultExpr)(var1, num1)->interp(), "free variable: x");
        CHECK_THROWS_WITH(NEW(MultExpr)(var1, var2)->interp(), "free variable: x");
        CHECK_THROWS_WITH(NEW(MultExpr)(var1, add1)->interp(), "free variable: x");
        CHECK_THROWS_WITH(NEW(MultExpr)(var1, mult1)->interp(), "free variable: x");

        CHECK(NEW(MultExpr)(add1, num1)->interp()->value_equals(NEW(NumVal)(3)));
        CHECK_THROWS_WITH(NEW(MultExpr)(add1, var1)->interp(), "free variable: x");
        CHECK(NEW(MultExpr)(add1, add1)->interp()->value_equals(NEW(NumVal)(1)));
        CHECK(NEW(MultExpr)(add1, mult1)->interp()->value_equals(NEW(NumVal)(-6)));

        CHECK(NEW(MultExpr)(mult1, num1)->interp()->value_equals(NEW(NumVal)(-18)));
        CHECK_THROWS_WITH(NEW(MultExpr)(mult1, var1)->interp(), "free variable: x");
        CHECK(NEW(MultExpr)(mult1, add1)->interp()->value_equals(NEW(NumVal)(-6)));
        CHECK(NEW(MultExpr)(mult1, mult1)->interp()->value_equals(NEW(NumVal)(36)));

        CHECK_THROWS_WITH(NEW(MultExpr)(NEW(EqualExpr)(num1, add1), num1)->interp(), "mult of non-number");
    }

    SECTION("Should evaluate EqualExpr") {
        // Interp EqualExpr with NumExpr
        CHECK(NEW(EqualExpr)(num1, num2)->interp()->value_equals(NEW(BoolVal)(false)));
        CHECK(NEW(EqualExpr)(num1, NEW(NumExpr)(3))->interp()
                      ->value_equals(NEW(BoolVal)(true)));

        // Interp EqualExpr with Add
        CHECK(NEW(EqualExpr)(add1, NEW(NumExpr)(1))->interp()
                      ->value_equals(NEW(BoolVal)(true)));
        CHECK(NEW(EqualExpr)(add1, NEW(NumExpr)(3))->interp()
                      ->value_equals(NEW(BoolVal)(false)));

        // Interp EqualExpr with Mult
        CHECK(NEW(EqualExpr)(mult1, NEW(NumExpr)(-6))->interp()
                      ->value_equals(NEW(BoolVal)(true)));
        CHECK(NEW(EqualExpr)(mult1, NEW(NumExpr)(0))->interp()
                      ->value_equals(NEW(BoolVal)(false)));

        // Interp EqualExpr with Let
        CHECK(NEW(EqualExpr)(let1, add1)->interp()
                      ->value_equals(NEW(BoolVal)(true)));
        CHECK(NEW(EqualExpr)(let1, mult1)->interp()
                      ->value_equals(NEW(BoolVal)(false)));

        // Interp EqualExpr with Var
        CHECK_THROWS_WITH(NEW(EqualExpr)(var1, add1)->interp(), "free variable: x");
        CHECK_THROWS_WITH(NEW(EqualExpr)(NEW(LetExpr)(NEW(VarExpr)("x"),
                                                      NEW(NumExpr)(2),
                                                      NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))),
                                         add1)->interp(),
                          "free variable: y");

        // Interp EqualExpr with BoolExpr
        CHECK(NEW(EqualExpr)(tr, fls)->interp()->value_equals(NEW(BoolVal)(false)));
        CHECK(NEW(EqualExpr)(tr, tr)->interp()->value_equals(NEW(BoolVal)(true)));
        CHECK(NEW(EqualExpr)(tr, num1)->interp()->value_equals(NEW(BoolVal)(false)));

        // Interp EqualExpr with IfExpr
        CHECK(NEW(EqualExpr)(NEW(NumExpr)(2),
                             NEW(IfExpr)(NEW(BoolExpr)(true),
                                         NEW(NumExpr)(2),
                                         NEW(VarExpr)("x")))->interp()->value_equals(NEW(BoolVal)(true)));
    }

    SECTION("Should evaluate LetExpr") {
        CHECK(let1->interp()->value_equals(NEW(NumVal)(1))); // All vars are bound with a concrete value

        CHECK_THROWS_WITH(parse_str("_let y=3 _in (x+-2)")->interp(),
                          "free variable: x"); // Some vars are not bound with a concrete value

        CHECK(parse_str("_let x=-2 _in (_let x=3 _in (x+-2))")->interp()->value_equals(
                NEW(NumVal)(1))); // Nested lets - binding of x is both in inner  outer lets - should go with inner

        CHECK(parse_str("_let x=-2 _in (_let x=3 _in (x+-2))")->interp()->value_equals(
                NEW(NumVal)(1))); // Nested lets - binding of x is in outer let

        CHECK(parse_str("_let x=(_let x=3 _in (x+-2)) _in (x+-2)")->interp()->value_equals(
                NEW(NumVal)(-1))); // Nested lets - rhs is a LetExpr body

        CHECK_THROWS_WITH(parse_str("_let y=-2 _in (_let y=3 _in (x+-2))")->interp(),
                          "free variable: x"); // Nested lets - no bindings of x in any lets
    }

    SECTION("Should evaluate IfExpr") {

        CHECK(parse_str("_if _true _then 10 _else -10")->interp()->value_equals(NEW(NumVal)(10)));

        CHECK(parse_str("_if _false _then 10 _else -10")->interp()->value_equals(NEW(NumVal)(-10)));

        CHECK(parse_str("_if (1+1) == 2 _then 10 _else -10")->interp()->value_equals(NEW(NumVal)(10)));

        CHECK(parse_str("_if _true == 1 _then 10 _else -10")->interp()->value_equals(NEW(NumVal)(-10)));

        CHECK_THROWS_WITH((NEW(IfExpr)(NEW(NumExpr)(1), NEW(NumExpr)(10), NEW(NumExpr)(-10)))
                                  ->interp(), "non-boolean condition in if body");

        CHECK(parse_str("_let same = (1 == 2) _in _if 1 == 2 "
                        "                            _then _false + 5 "
                        "                            _else 88")->interp()->value_equals(NEW(NumVal)(88)));

        CHECK_THROWS_WITH(parse_str("_if (4 + 1) _then 2 _else 3")->interp(), "non-boolean condition in if body");
    }

    SECTION("Should evaluate FunExpr") {
        PTR(AddExpr) body = NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1));
        CHECK(fun1->interp()->value_equals(NEW(FunVal)("x", body)));
        CHECK(!fun1->interp()->value_equals(NEW(FunVal)("x1", body)));
        CHECK(!fun1->interp()->value_equals(NEW(FunVal)("x1", add1)));

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
        CHECK(call1->interp()->value_equals(NEW(NumVal)(2)));
        CHECK(parse_str("_fun (x) x + 1")->interp()->to_string() == "(_fun (x) (x+1))");
        CHECK(parse_str("_let f = _fun (x) x + 1 _in f")->interp()->to_string() == "(_fun (x) (x+1))");
    }
}