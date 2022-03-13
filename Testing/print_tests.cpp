#include <sstream>
#include "catch.h"
#include "../Expressions/num_expr.h"
#include "../Expressions/add_expr.h"
#include "../Expressions/mult_expr.h"
#include "../Expressions/var_expr.h"
#include "../Expressions/let_expr.h"
#include "../Expressions/bool_expr.h"
#include "../Expressions/equal_expr.h"
#include "../Expressions/if_expr.h"
#include "../Expressions/fun_expr.h"
#include "../Expressions/call_expr.h"
#include "../shared_ptr.h"

TEST_CASE("Should print or pretty print expressions") {
    PTR(NumExpr) num1 = NEW(NumExpr)(3);
    PTR(NumExpr) num2 = NEW(NumExpr)(-2);
    PTR(VarExpr) var1 = NEW(VarExpr)("x");
    PTR(VarExpr) var2 = NEW(VarExpr)("y");
    PTR(BoolExpr) tr = NEW(BoolExpr)(true);
    PTR(BoolExpr) fls = NEW(BoolExpr)(false);

    PTR(AddExpr) add1 = NEW(AddExpr)(num1, num2); //"3 + -2"
    PTR(MultExpr) mult1 = NEW(MultExpr)(num1, num2); //"3 * -2"
    PTR(LetExpr) let1 = NEW(LetExpr)(var1, num1, NEW(AddExpr)(var1, num2)); //"_let x=3 _in (x+-2)" = 1
    PTR(IfExpr) if1 = NEW(IfExpr)(tr, num1, num2);

    SECTION("Should print NumExpr") {
        CHECK(num1->to_string() == "3");
        CHECK(num2->to_string() == "-2");
    }

    SECTION("Should print BoolExpr") {
        CHECK(tr->to_string() == "_true");
        CHECK(fls->to_string() == "_false");
    }

    SECTION("Should print VarExpr") {
        CHECK(var1->to_string() == "x");
        CHECK(var2->to_string() == "y");
    }

    SECTION("Should print AddExpr") {
        CHECK(add1->to_string() == "(3+-2)");
        CHECK(NEW(AddExpr)(num1, var1)->to_string() == "(3+x)");
        CHECK(NEW(AddExpr)(num1, add1)->to_string() == "(3+(3+-2))");
        CHECK(NEW(AddExpr)(num1, mult1)->to_string() == "(3+(3*-2))");

        CHECK(NEW(AddExpr)(var1, num1)->to_string() == "(x+3)");
        CHECK(NEW(AddExpr)(var1, var2)->to_string() == "(x+y)");
        CHECK(NEW(AddExpr)(var1, add1)->to_string() == "(x+(3+-2))");
        CHECK(NEW(AddExpr)(var1, mult1)->to_string() == "(x+(3*-2))");

        CHECK(NEW(AddExpr)(add1, num1)->to_string() == "((3+-2)+3)");
        CHECK(NEW(AddExpr)(add1, var1)->to_string() == "((3+-2)+x)");
        CHECK(NEW(AddExpr)(add1, add1)->to_string() == "((3+-2)+(3+-2))");
        CHECK(NEW(AddExpr)(add1, mult1)->to_string() == "((3+-2)+(3*-2))");

        CHECK(NEW(AddExpr)(mult1, num1)->to_string() == "((3*-2)+3)");
        CHECK(NEW(AddExpr)(mult1, var1)->to_string() == "((3*-2)+x)");
        CHECK(NEW(AddExpr)(mult1, add1)->to_string() == "((3*-2)+(3+-2))");
        CHECK(NEW(AddExpr)(mult1, mult1)->to_string() == "((3*-2)+(3*-2))");
    }

    SECTION("Should print MultExpr") {
        CHECK(mult1->to_string() == "(3*-2)");
        CHECK(NEW(MultExpr)(num1, var1)->to_string() == "(3*x)");
        CHECK(NEW(MultExpr)(num1, add1)->to_string() == "(3*(3+-2))");
        CHECK(NEW(MultExpr)(num1, mult1)->to_string() == "(3*(3*-2))");

        CHECK(NEW(MultExpr)(var1, num1)->to_string() == "(x*3)");
        CHECK(NEW(MultExpr)(var1, var2)->to_string() == "(x*y)");
        CHECK(NEW(MultExpr)(var1, add1)->to_string() == "(x*(3+-2))");
        CHECK(NEW(MultExpr)(var1, mult1)->to_string() == "(x*(3*-2))");

        CHECK(NEW(MultExpr)(add1, num1)->to_string() == "((3+-2)*3)");
        CHECK(NEW(MultExpr)(add1, var1)->to_string() == "((3+-2)*x)");
        CHECK(NEW(MultExpr)(add1, add1)->to_string() == "((3+-2)*(3+-2))");
        CHECK(NEW(MultExpr)(add1, mult1)->to_string() == "((3+-2)*(3*-2))");

        CHECK(NEW(MultExpr)(mult1, num1)->to_string() == "((3*-2)*3)");
        CHECK(NEW(MultExpr)(mult1, var1)->to_string() == "((3*-2)*x)");
        CHECK(NEW(MultExpr)(mult1, add1)->to_string() == "((3*-2)*(3+-2))");
        CHECK(NEW(MultExpr)(mult1, mult1)->to_string() == "((3*-2)*(3*-2))");
    }

    SECTION("Should print EqualExpr") {
        CHECK(NEW(EqualExpr)(num1, num2)->to_string() == "(3==-2)");
        CHECK(NEW(EqualExpr)(add1, num1)->to_string() == "((3+-2)==3)");
        CHECK(NEW(EqualExpr)(num1, mult1)->to_string() == "(3==(3*-2))");
        CHECK(NEW(EqualExpr)(num1, tr)->to_string() == "(3==_true)");
        CHECK(NEW(EqualExpr)(let1, num1)->to_string() == "((_let x=3 _in (x+-2))==3)");
    }

    SECTION("Should print LetExpr") {
        CHECK(let1->to_string() == "(_let x=3 _in (x+-2))");

        // Nested lets - body is a LetExpr body
        CHECK(NEW(LetExpr)(var1, num2, let1)->to_string() == "(_let x=-2 _in (_let x=3 _in (x+-2)))");

        // Nested lets - rhs is a LetExpr body
        CHECK(NEW(LetExpr)(var1, let1, NEW(AddExpr)(var1, num2))->to_string() ==
              "(_let x=(_let x=3 _in (x+-2)) _in (x+-2))");

        // Nested lets - both body and rhs are LetExpr body's
        CHECK(NEW(LetExpr)(var1, let1, let1)->to_string() ==
              "(_let x=(_let x=3 _in (x+-2)) _in (_let x=3 _in (x+-2)))");

        // LetExpr is lhs of a +/* body
        CHECK(NEW(AddExpr)(let1, num1)->to_string() == "((_let x=3 _in (x+-2))+3)");

        // LetExpr is rhs of a parenthesized +/* body
        CHECK(NEW(MultExpr)(num1, NEW(AddExpr)(num1, let1))->to_string() == "(3*(3+(_let x=3 _in (x+-2))))");

        // LetExpr is rhs of an unparenthesized +/* and would NOT have needed parentheses in the surrounding context
        CHECK(NEW(MultExpr)(num1, let1)->to_string() == "(3*(_let x=3 _in (x+-2)))");

        // LetExpr is rhs of an unparenthesized +/* and would have needed parentheses in the surrounding context
        CHECK(NEW(AddExpr)(NEW(MultExpr)(num1, let1), num1)->to_string() == "((3*(_let x=3 _in (x+-2)))+3)");
    }

    SECTION("Should print IfExpr") {
        CHECK(if1->to_string() == "(_if _true _then 3 _else -2)");

        // Nested ifs - thenBranch is a LetExpr body
        CHECK(NEW(IfExpr)(fls, if1, num1)->to_string() == "(_if _false _then (_if _true _then 3 _else -2) _else 3)");

        // Nested ifs - elseBranch is a LetExpr body
        CHECK(NEW(IfExpr)(tr, num1, if1)->to_string() == "(_if _true _then 3 _else (_if _true _then 3 _else -2))");

        // Nested lets - both thenBranch and elseBranch are LetExpr body's
        CHECK(NEW(IfExpr)(NEW(EqualExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)), if1, if1)->to_string() ==
              "(_if (1==1) _then (_if _true _then 3 _else -2) _else (_if _true _then 3 _else -2))");

        // IfExpr is lhs of a +/* body
        CHECK(NEW(AddExpr)(if1, num1)->to_string() == "((_if _true _then 3 _else -2)+3)");

        // IfExpr is rhs of a parenthesized +/* body
        CHECK(NEW(MultExpr)(num1, NEW(AddExpr)(num1, if1))->to_string() == "(3*(3+(_if _true _then 3 _else -2)))");

        // IfExpr is rhs of an unparenthesized +/* and would NOT have needed parentheses in the surrounding context when pretty print
        CHECK(NEW(MultExpr)(num1, if1)->to_string() == "(3*(_if _true _then 3 _else -2))");

        // IfExpr is rhs of an unparenthesized +/* and would have needed parentheses in the surrounding context when pretty print
        CHECK(NEW(AddExpr)(NEW(MultExpr)(num1, if1), num1)->to_string() == "((3*(_if _true _then 3 _else -2))+3)");
    }

    SECTION("Should print FunExpr") {
        CHECK(NEW(FunExpr)("x", add1)->to_string() == "(_fun (x) (3+-2))");
        CHECK(NEW(FunExpr)("x", if1)->to_string() == "(_fun (x) (_if _true _then 3 _else -2))");
    }

    SECTION("Should print CallExpr") {
        CHECK(NEW(CallExpr)(NEW(VarExpr)("f"), NEW(VarExpr)("x"))->to_string() == "f(x)");
    }

    SECTION("Should pretty print Number") {
        CHECK(num1->to_string(true) == "3");
        CHECK(num2->to_string(true) == "-2");
    }

    SECTION("Should pretty print VarExpr") {
        CHECK(var1->to_string(true) == "x");
        CHECK(var2->to_string(true) == "y");
    }

    SECTION("Should pretty print AddExpr") {
        CHECK(add1->to_string(true) == "3 + -2");
        CHECK(NEW(AddExpr)(num1, var1)->to_string(true) == "3 + x");
        CHECK(NEW(AddExpr)(num1, add1)->to_string(true) == "3 + 3 + -2");
        CHECK(NEW(AddExpr)(num1, mult1)->to_string(true) == "3 + 3 * -2");

        CHECK(NEW(AddExpr)(var1, num1)->to_string(true) == "x + 3");
        CHECK(NEW(AddExpr)(var1, var2)->to_string(true) == "x + y");
        CHECK(NEW(AddExpr)(var1, add1)->to_string(true) == "x + 3 + -2");
        CHECK(NEW(AddExpr)(var1, mult1)->to_string(true) == "x + 3 * -2");

        CHECK(NEW(AddExpr)(add1, num1)->to_string(true) == "(3 + -2) + 3");
        CHECK(NEW(AddExpr)(add1, var1)->to_string(true) == "(3 + -2) + x");
        CHECK(NEW(AddExpr)(add1, add1)->to_string(true) == "(3 + -2) + 3 + -2");
        CHECK(NEW(AddExpr)(add1, mult1)->to_string(true) == "(3 + -2) + 3 * -2");

        CHECK(NEW(AddExpr)(mult1, num1)->to_string(true) == "3 * -2 + 3");
        CHECK(NEW(AddExpr)(mult1, var1)->to_string(true) == "3 * -2 + x");
        CHECK(NEW(AddExpr)(mult1, add1)->to_string(true) == "3 * -2 + 3 + -2");
        CHECK(NEW(AddExpr)(mult1, mult1)->to_string(true) == "3 * -2 + 3 * -2");
    }

    SECTION("Should pretty print MultExpr") {
        CHECK(mult1->to_string(true) == "3 * -2");
        CHECK(NEW(MultExpr)(num1, var1)->to_string(true) == "3 * x");
        CHECK(NEW(MultExpr)(num1, add1)->to_string(true) == "3 * (3 + -2)");
        CHECK(NEW(MultExpr)(num1, mult1)->to_string(true) == "3 * 3 * -2");

        CHECK(NEW(MultExpr)(var1, num1)->to_string(true) == "x * 3");
        CHECK(NEW(MultExpr)(var1, var2)->to_string(true) == "x * y");
        CHECK(NEW(MultExpr)(var1, add1)->to_string(true) == "x * (3 + -2)");
        CHECK(NEW(MultExpr)(var1, mult1)->to_string(true) == "x * 3 * -2");

        CHECK(NEW(MultExpr)(add1, num1)->to_string(true) == "(3 + -2) * 3");
        CHECK(NEW(MultExpr)(add1, var1)->to_string(true) == "(3 + -2) * x");
        CHECK(NEW(MultExpr)(add1, add1)->to_string(true) == "(3 + -2) * (3 + -2)");
        CHECK(NEW(MultExpr)(add1, mult1)->to_string(true) == "(3 + -2) * 3 * -2");

        CHECK(NEW(MultExpr)(mult1, num1)->to_string(true) == "(3 * -2) * 3");
        CHECK(NEW(MultExpr)(mult1, var1)->to_string(true) == "(3 * -2) * x");
        CHECK(NEW(MultExpr)(mult1, add1)->to_string(true) == "(3 * -2) * (3 + -2)");
        CHECK(NEW(MultExpr)(mult1, mult1)->to_string(true) == "(3 * -2) * 3 * -2");
    }

    SECTION("Should pretty print EqualExpr") {
        CHECK(NEW(EqualExpr)(num1, num2)->to_string(true) == "3 == -2");
        CHECK(NEW(EqualExpr)(add1, num1)->to_string(true) == "3 + -2 == 3");
        CHECK(NEW(EqualExpr)(num1, mult1)->to_string(true) == "3 == 3 * -2");
        CHECK(NEW(EqualExpr)(num1, tr)->to_string(true) == "3 == _true");
        CHECK(NEW(EqualExpr)(let1, num1)->to_string(true) == "(_let x = 3\n"
                                                             " _in  x + -2) == 3");
        CHECK(NEW(EqualExpr)(NEW(EqualExpr)(num1, num2), num1)->to_string(true) == "(3 == -2) == 3");
        CHECK(NEW(EqualExpr)(num1, NEW(EqualExpr)(num1, num2))->to_string(true) == "3 == 3 == -2");
        CHECK(NEW(AddExpr)(NEW(EqualExpr)(num1, num2),
                           NEW(EqualExpr)(num1, tr))->to_string(true) == "(3 == -2) + (3 == _true)");
    }

    SECTION("Should pretty print LetExpr") {

        CHECK(let1->to_string(true) == "_let x = 3\n"
                                       "_in  x + -2");

        // Nested lets - body is a LetExpr body
        CHECK(NEW(LetExpr)(var1, num2, let1)
                      ->to_string(true) == "_let x = -2\n"
                                           "_in  _let x = 3\n"
                                           "     _in  x + -2");

        // Nested lets - rhs is a LetExpr body
        CHECK(NEW(LetExpr)(var1, let1, NEW(AddExpr)(var1, num2))
                      ->to_string(true) == "_let x = _let x = 3\n"
                                           "         _in  x + -2\n"
                                           "_in  x + -2");

        // Nested lets - both body and rhs are LetExpr body's
        CHECK(NEW(LetExpr)(var1, let1, let1)
                      ->to_string(true) == "_let x = _let x = 3\n"
                                           "         _in  x + -2\n"
                                           "_in  _let x = 3\n"
                                           "     _in  x + -2");

        // LetExpr is lhs of a +/* body
        CHECK(NEW(AddExpr)(let1, num1)
                      ->to_string(true) == "(_let x = 3\n"
                                           " _in  x + -2) + 3");

        // LetExpr is rhs of a parenthesized +/* body
        CHECK(NEW(MultExpr)(num1, NEW(AddExpr)(num1, let1))->to_string(true) == "3 * (3 + _let x = 3\n"
                                                                                "         _in  x + -2)");

        // LetExpr is rhs of an unparenthesized +/* and would NOT have needed parentheses in the surrounding context
        CHECK(NEW(MultExpr)(num1, let1)->to_string(true) == "3 * _let x = 3\n"
                                                            "    _in  x + -2");

        // LetExpr is rhs of an unparenthesized +/* and would have needed parentheses in the surrounding context
        CHECK(NEW(AddExpr)(NEW(MultExpr)(num1, let1), num1)->to_string(true) == "3 * (_let x = 3\n"
                                                                                "     _in  x + -2) + 3");

    }

    SECTION("Should pretty print IfExpr") {

        CHECK(if1->to_string(true) == "_if _true\n"
                                      "_then 3\n"
                                      "_else -2");

        // Nested ifs - thenBranch is a LetExpr body
        CHECK(NEW(IfExpr)(fls, if1, num1)->to_string(true) == "_if _false\n"
                                                              "_then _if _true\n"
                                                              "      _then 3\n"
                                                              "      _else -2\n"
                                                              "_else 3");
        // Nested ifs - elseBranch is a LetExpr body
        CHECK(NEW(IfExpr)(tr, num1, if1)->to_string(true) == "_if _true\n"
                                                             "_then 3\n"
                                                             "_else _if _true\n"
                                                             "      _then 3\n"
                                                             "      _else -2");

        // Nested lets - both thenBranch and elseBranch are LetExpr body's
        CHECK(NEW(IfExpr)(NEW(EqualExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)),
                          if1,
                          if1)->to_string(true) == "_if 1 == 1\n"
                                                   "_then _if _true\n"
                                                   "      _then 3\n"
                                                   "      _else -2\n"
                                                   "_else _if _true\n"
                                                   "      _then 3\n"
                                                   "      _else -2");
        // IfExpr is lhs of a +/* body
        CHECK(NEW(AddExpr)(if1, num1)->to_string(true) == "(_if _true\n"
                                                          " _then 3\n"
                                                          " _else -2) + 3");

        // IfExpr is rhs of a parenthesized +/* body
        CHECK(NEW(MultExpr)(num1, NEW(AddExpr)(num1, if1))->to_string(true) == "3 * (3 + _if _true\n"
                                                                               "         _then 3\n"
                                                                               "         _else -2)");
        // IfExpr is rhs of an unparenthesized +/* and would NOT have needed parentheses in the surrounding context
        CHECK(NEW(MultExpr)(num1, if1)->to_string(true) == "3 * _if _true\n"
                                                           "    _then 3\n"
                                                           "    _else -2");


        CHECK(NEW(MultExpr)(if1, NEW(LetExpr)(NEW(VarExpr)("x"),
                                              NEW(NumExpr)(2),
                                              NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2))))
                      ->to_string(true) == "(_if _true\n"
                                           " _then 3\n"
                                           " _else -2) * _let x = 2\n"
                                           "             _in  x + 2");

        // IfExpr is rhs of an unparenthesized +/* and would have needed parentheses in the surrounding context
        CHECK(NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(3),
                                         NEW(IfExpr)(NEW(BoolExpr)(true),
                                                     NEW(NumExpr)(3),
                                                     NEW(NumExpr)(-2))), num1)
                      ->to_string(true) == "3 * (_if _true\n"
                                           "     _then 3\n"
                                           "     _else -2) + 3");

        // Class example 1
        CHECK(NEW(LetExpr)(NEW(VarExpr)("same"),
                           NEW(EqualExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                           NEW(IfExpr)(NEW(EqualExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                                       NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)),
                                       NEW(NumExpr)(88)))
                      ->to_string(true) == "_let same = 1 == 2\n"
                                           "_in  _if 1 == 2\n"
                                           "     _then _false + 5\n"
                                           "     _else 88");
        // Class example 2
        CHECK(NEW(IfExpr)(NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(1)),
                          NEW(NumExpr)(2),
                          NEW(NumExpr)(3))
                      ->to_string(true) == "_if 4 + 1\n"
                                           "_then 2\n"
                                           "_else 3");
    }

    SECTION("Should pretty print FunExpr") {
        // Nested funs - body is a FunExpr body
        CHECK(NEW(FunExpr)("x", NEW(FunExpr)("x",
                                             NEW(EqualExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                      ->to_string(true) == "_fun (x)\n"
                                           "  _fun (x)\n"
                                           "    x == 1");

        // Nested funs - fun is rhs of a let
        CHECK(NEW(LetExpr)(var1,
                           NEW(FunExpr)("x", NEW(EqualExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))),
                           NEW(AddExpr)(var1, num2))
                      ->to_string(true) == "_let x = _fun (x)\n"
                                           "           x == 1\n"
                                           "_in  x + -2");

        // LetExpr is lhs of a +/* body
        CHECK(NEW(AddExpr)(NEW(FunExpr)("x", NEW(EqualExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), num1)
                      ->to_string(true) == "(_fun (x)\n"
                                           "   x == 1) + 3");

        CHECK(NEW(MultExpr)(num1,
                            NEW(FunExpr)("x", NEW(EqualExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                      ->to_string(true) == "3 * _fun (x)\n"
                                           "      x == 1");

        // LetExpr is rhs of an unparenthesized +/* and would NOT have needed parentheses in the surrounding context
        CHECK(NEW(MultExpr)(num1, NEW(FunExpr)("x", NEW(EqualExpr)(NEW(VarExpr)("x"),
                                                                   NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                NEW(NumExpr)(1)))))
                      ->to_string(true) == "3 * _fun (x)\n"
                                           "      x == x + 1");
    }

    SECTION("Should pretty print CallExpr") {

        CHECK(NEW(CallExpr)(NEW(VarExpr)("f"), NEW(VarExpr)("x"))
                      ->to_string(true) == "f(x)");

        // Nested funs - body is a FunExpr body
        CHECK(NEW(CallExpr)(NEW(FunExpr)("x", NEW(EqualExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))),
                            NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))
                      ->to_string(true) == "(_fun (x)\n"
                                           "   x == 1)(x + 1)");

        // Recursive
        CHECK(NEW(CallExpr)(NEW(CallExpr)(NEW(VarExpr)("f"), NEW(VarExpr)("f")), NEW(NumExpr)(10))
                      ->to_string(true) == "f(f)(10)");
    }
}

