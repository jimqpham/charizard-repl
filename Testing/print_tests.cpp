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
    NumExpr num1 = NumExpr(3);
    NumExpr num2 = NumExpr(-2);
    VarExpr var1 = VarExpr("x");
    VarExpr var2 = VarExpr("y");
    BoolExpr tr = BoolExpr(true);
    BoolExpr fls = BoolExpr(false);

    AddExpr add1 = AddExpr(&num1, &num2); //"3 + -2"
    MultExpr mult1 = MultExpr(&num1, &num2); //"3 * -2"
    LetExpr let1 = LetExpr(&var1, &num1, new AddExpr(&var1, &num2)); //"_let x=3 _in (x+-2)" = 1
    IfExpr if1 = IfExpr(&tr, &num1, &num2);

    SECTION("Should print NumExpr") {
        CHECK(num1.to_string() == "3");
        CHECK(num2.to_string() == "-2");
    }

    SECTION("Should print BoolExpr") {
        CHECK(tr.to_string() == "_true");
        CHECK(fls.to_string() == "_false");
    }

    SECTION("Should print VarExpr") {
        CHECK(var1.to_string() == "x");
        CHECK(var2.to_string() == "y");
    }

    SECTION("Should print AddExpr") {
        CHECK(add1.to_string() == "(3+-2)");
        CHECK(AddExpr(&num1, &var1).to_string() == "(3+x)");
        CHECK(AddExpr(&num1, &add1).to_string() == "(3+(3+-2))");
        CHECK(AddExpr(&num1, &mult1).to_string() == "(3+(3*-2))");

        CHECK(AddExpr(&var1, &num1).to_string() == "(x+3)");
        CHECK(AddExpr(&var1, &var2).to_string() == "(x+y)");
        CHECK(AddExpr(&var1, &add1).to_string() == "(x+(3+-2))");
        CHECK(AddExpr(&var1, &mult1).to_string() == "(x+(3*-2))");

        CHECK(AddExpr(&add1, &num1).to_string() == "((3+-2)+3)");
        CHECK(AddExpr(&add1, &var1).to_string() == "((3+-2)+x)");
        CHECK(AddExpr(&add1, &add1).to_string() == "((3+-2)+(3+-2))");
        CHECK(AddExpr(&add1, &mult1).to_string() == "((3+-2)+(3*-2))");

        CHECK(AddExpr(&mult1, &num1).to_string() == "((3*-2)+3)");
        CHECK(AddExpr(&mult1, &var1).to_string() == "((3*-2)+x)");
        CHECK(AddExpr(&mult1, &add1).to_string() == "((3*-2)+(3+-2))");
        CHECK(AddExpr(&mult1, &mult1).to_string() == "((3*-2)+(3*-2))");
    }

    SECTION("Should print MultExpr") {
        CHECK(mult1.to_string() == "(3*-2)");
        CHECK(MultExpr(&num1, &var1).to_string() == "(3*x)");
        CHECK(MultExpr(&num1, &add1).to_string() == "(3*(3+-2))");
        CHECK(MultExpr(&num1, &mult1).to_string() == "(3*(3*-2))");

        CHECK(MultExpr(&var1, &num1).to_string() == "(x*3)");
        CHECK(MultExpr(&var1, &var2).to_string() == "(x*y)");
        CHECK(MultExpr(&var1, &add1).to_string() == "(x*(3+-2))");
        CHECK(MultExpr(&var1, &mult1).to_string() == "(x*(3*-2))");

        CHECK(MultExpr(&add1, &num1).to_string() == "((3+-2)*3)");
        CHECK(MultExpr(&add1, &var1).to_string() == "((3+-2)*x)");
        CHECK(MultExpr(&add1, &add1).to_string() == "((3+-2)*(3+-2))");
        CHECK(MultExpr(&add1, &mult1).to_string() == "((3+-2)*(3*-2))");

        CHECK(MultExpr(&mult1, &num1).to_string() == "((3*-2)*3)");
        CHECK(MultExpr(&mult1, &var1).to_string() == "((3*-2)*x)");
        CHECK(MultExpr(&mult1, &add1).to_string() == "((3*-2)*(3+-2))");
        CHECK(MultExpr(&mult1, &mult1).to_string() == "((3*-2)*(3*-2))");
    }

    SECTION("Should print EqualExpr") {
        CHECK(EqualExpr(&num1, &num2).to_string() == "(3==-2)");
        CHECK(EqualExpr(&add1, &num1).to_string() == "((3+-2)==3)");
        CHECK(EqualExpr(&num1, &mult1).to_string() == "(3==(3*-2))");
        CHECK(EqualExpr(&num1, &tr).to_string() == "(3==_true)");
        CHECK(EqualExpr(&let1, &num1).to_string() == "((_let x=3 _in (x+-2))==3)");
    }

    SECTION("Should print LetExpr") {
        CHECK(let1.to_string() == "(_let x=3 _in (x+-2))");

        // Nested lets - body is a LetExpr body
        CHECK(LetExpr(&var1, &num2, &let1).to_string() == "(_let x=-2 _in (_let x=3 _in (x+-2)))");

        // Nested lets - rhs is a LetExpr body
        CHECK(LetExpr(&var1, &let1, new AddExpr(&var1, &num2)).to_string() ==
              "(_let x=(_let x=3 _in (x+-2)) _in (x+-2))");

        // Nested lets - both body and rhs are LetExpr body's
        CHECK(LetExpr(&var1, &let1, &let1).to_string() == "(_let x=(_let x=3 _in (x+-2)) _in (_let x=3 _in (x+-2)))");

        // LetExpr is lhs of a +/* body
        CHECK(AddExpr(&let1, &num1).to_string() == "((_let x=3 _in (x+-2))+3)");

        // LetExpr is rhs of a parenthesized +/* body
        CHECK(MultExpr(&num1, new AddExpr(&num1, &let1)).to_string() == "(3*(3+(_let x=3 _in (x+-2))))");

        // LetExpr is rhs of an unparenthesized +/* and would NOT have needed parentheses in the surrounding context
        CHECK(MultExpr(&num1, &let1).to_string() == "(3*(_let x=3 _in (x+-2)))");

        // LetExpr is rhs of an unparenthesized +/* and would have needed parentheses in the surrounding context
        CHECK(AddExpr(new MultExpr(&num1, &let1), &num1).to_string() == "((3*(_let x=3 _in (x+-2)))+3)");
    }

    SECTION("Should print IfExpr") {
        CHECK(if1.to_string() == "(_if _true _then 3 _else -2)");

        // Nested ifs - thenBranch is a LetExpr body
        CHECK(IfExpr(&fls, &if1, &num1).to_string() == "(_if _false _then (_if _true _then 3 _else -2) _else 3)");

        // Nested ifs - elseBranch is a LetExpr body
        CHECK(IfExpr(&tr, &num1, &if1).to_string() == "(_if _true _then 3 _else (_if _true _then 3 _else -2))");

        // Nested lets - both thenBranch and elseBranch are LetExpr body's
        CHECK(IfExpr(new EqualExpr(new NumExpr(1), new NumExpr(1)), &if1, &if1).to_string() ==
              "(_if (1==1) _then (_if _true _then 3 _else -2) _else (_if _true _then 3 _else -2))");

        // IfExpr is lhs of a +/* body
        CHECK(AddExpr(&if1, &num1).to_string() == "((_if _true _then 3 _else -2)+3)");

        // IfExpr is rhs of a parenthesized +/* body
        CHECK(MultExpr(&num1, new AddExpr(&num1, &if1)).to_string() == "(3*(3+(_if _true _then 3 _else -2)))");

        // IfExpr is rhs of an unparenthesized +/* and would NOT have needed parentheses in the surrounding context when pretty print
        CHECK(MultExpr(&num1, &if1).to_string() == "(3*(_if _true _then 3 _else -2))");

        // IfExpr is rhs of an unparenthesized +/* and would have needed parentheses in the surrounding context when pretty print
        CHECK(AddExpr(new MultExpr(&num1, &if1), &num1).to_string() == "((3*(_if _true _then 3 _else -2))+3)");
    }

    SECTION("Should print FunExpr") {
        CHECK(FunExpr("x", &add1).to_string() == "(_fun (x) (3+-2))");
        CHECK(FunExpr("x", &if1).to_string() == "(_fun (x) (_if _true _then 3 _else -2))");
    }

    SECTION("Should print CallExpr") {
        CHECK(CallExpr(new VarExpr("f"), new VarExpr("x")).to_string() == "f(x)");
    }

    SECTION("Should pretty print Number") {
        CHECK(num1.to_string(true) == "3");
        CHECK(num2.to_string(true) == "-2");
    }

    SECTION("Should pretty print VarExpr") {
        CHECK(var1.to_string(true) == "x");
        CHECK(var2.to_string(true) == "y");
    }

    SECTION("Should pretty print AddExpr") {
        CHECK(add1.to_string(true) == "3 + -2");
        CHECK(AddExpr(&num1, &var1).to_string(true) == "3 + x");
        CHECK(AddExpr(&num1, &add1).to_string(true) == "3 + 3 + -2");
        CHECK(AddExpr(&num1, &mult1).to_string(true) == "3 + 3 * -2");

        CHECK(AddExpr(&var1, &num1).to_string(true) == "x + 3");
        CHECK(AddExpr(&var1, &var2).to_string(true) == "x + y");
        CHECK(AddExpr(&var1, &add1).to_string(true) == "x + 3 + -2");
        CHECK(AddExpr(&var1, &mult1).to_string(true) == "x + 3 * -2");

        CHECK(AddExpr(&add1, &num1).to_string(true) == "(3 + -2) + 3");
        CHECK(AddExpr(&add1, &var1).to_string(true) == "(3 + -2) + x");
        CHECK(AddExpr(&add1, &add1).to_string(true) == "(3 + -2) + 3 + -2");
        CHECK(AddExpr(&add1, &mult1).to_string(true) == "(3 + -2) + 3 * -2");

        CHECK(AddExpr(&mult1, &num1).to_string(true) == "3 * -2 + 3");
        CHECK(AddExpr(&mult1, &var1).to_string(true) == "3 * -2 + x");
        CHECK(AddExpr(&mult1, &add1).to_string(true) == "3 * -2 + 3 + -2");
        CHECK(AddExpr(&mult1, &mult1).to_string(true) == "3 * -2 + 3 * -2");
    }

    SECTION("Should pretty print MultExpr") {
        CHECK(mult1.to_string(true) == "3 * -2");
        CHECK(MultExpr(&num1, &var1).to_string(true) == "3 * x");
        CHECK(MultExpr(&num1, &add1).to_string(true) == "3 * (3 + -2)");
        CHECK(MultExpr(&num1, &mult1).to_string(true) == "3 * 3 * -2");

        CHECK(MultExpr(&var1, &num1).to_string(true) == "x * 3");
        CHECK(MultExpr(&var1, &var2).to_string(true) == "x * y");
        CHECK(MultExpr(&var1, &add1).to_string(true) == "x * (3 + -2)");
        CHECK(MultExpr(&var1, &mult1).to_string(true) == "x * 3 * -2");

        CHECK(MultExpr(&add1, &num1).to_string(true) == "(3 + -2) * 3");
        CHECK(MultExpr(&add1, &var1).to_string(true) == "(3 + -2) * x");
        CHECK(MultExpr(&add1, &add1).to_string(true) == "(3 + -2) * (3 + -2)");
        CHECK(MultExpr(&add1, &mult1).to_string(true) == "(3 + -2) * 3 * -2");

        CHECK(MultExpr(&mult1, &num1).to_string(true) == "(3 * -2) * 3");
        CHECK(MultExpr(&mult1, &var1).to_string(true) == "(3 * -2) * x");
        CHECK(MultExpr(&mult1, &add1).to_string(true) == "(3 * -2) * (3 + -2)");
        CHECK(MultExpr(&mult1, &mult1).to_string(true) == "(3 * -2) * 3 * -2");
    }

    SECTION("Should pretty print EqualExpr") {
        CHECK(EqualExpr(&num1, &num2).to_string(true) == "3 == -2");
        CHECK(EqualExpr(&add1, &num1).to_string(true) == "3 + -2 == 3");
        CHECK(EqualExpr(&num1, &mult1).to_string(true) == "3 == 3 * -2");
        CHECK(EqualExpr(&num1, &tr).to_string(true) == "3 == _true");
        CHECK(EqualExpr(&let1, &num1).to_string(true) == "(_let x = 3\n"
                                                         " _in  x + -2) == 3");
        CHECK(EqualExpr(new EqualExpr(&num1, &num2), &num1).to_string(true) == "(3 == -2) == 3");
        CHECK(EqualExpr(&num1, new EqualExpr(&num1, &num2)).to_string(true) == "3 == 3 == -2");
        CHECK(AddExpr(new EqualExpr(&num1, &num2),
                      new EqualExpr(&num1, &tr)).to_string(true) == "(3 == -2) + (3 == _true)");
    }

    SECTION("Should pretty print LetExpr") {

        CHECK(let1.to_string(true) == "_let x = 3\n"
                                      "_in  x + -2");

        // Nested lets - body is a LetExpr body
        CHECK(LetExpr(&var1, &num2, &let1)
                      .to_string(true) == "_let x = -2\n"
                                          "_in  _let x = 3\n"
                                          "     _in  x + -2");

        // Nested lets - rhs is a LetExpr body
        CHECK(LetExpr(&var1, &let1, new AddExpr(&var1, &num2))
                      .to_string(true) == "_let x = _let x = 3\n"
                                          "         _in  x + -2\n"
                                          "_in  x + -2");

        // Nested lets - both body and rhs are LetExpr body's
        CHECK(LetExpr(&var1, &let1, &let1)
                      .to_string(true) == "_let x = _let x = 3\n"
                                          "         _in  x + -2\n"
                                          "_in  _let x = 3\n"
                                          "     _in  x + -2");

        // LetExpr is lhs of a +/* body
        CHECK(AddExpr(&let1, &num1)
                      .to_string(true) == "(_let x = 3\n"
                                          " _in  x + -2) + 3");

        // LetExpr is rhs of a parenthesized +/* body
        CHECK(MultExpr(&num1, new AddExpr(&num1, &let1)).to_string(true) == "3 * (3 + _let x = 3\n"
                                                                            "         _in  x + -2)");

        // LetExpr is rhs of an unparenthesized +/* and would NOT have needed parentheses in the surrounding context
        CHECK(MultExpr(&num1, &let1).to_string(true) == "3 * _let x = 3\n"
                                                        "    _in  x + -2");

        // LetExpr is rhs of an unparenthesized +/* and would have needed parentheses in the surrounding context
        CHECK(AddExpr(new MultExpr(&num1, &let1), &num1).to_string(true) == "3 * (_let x = 3\n"
                                                                            "     _in  x + -2) + 3");

    }

    SECTION("Should pretty print IfExpr") {

        CHECK(if1.to_string(true) == "_if _true\n"
                                     "_then 3\n"
                                     "_else -2");

        // Nested ifs - thenBranch is a LetExpr body
        CHECK(IfExpr(&fls, &if1, &num1).to_string(true) == "_if _false\n"
                                                           "_then _if _true\n"
                                                           "      _then 3\n"
                                                           "      _else -2\n"
                                                           "_else 3");
        // Nested ifs - elseBranch is a LetExpr body
        CHECK(IfExpr(&tr, &num1, &if1).to_string(true) == "_if _true\n"
                                                          "_then 3\n"
                                                          "_else _if _true\n"
                                                          "      _then 3\n"
                                                          "      _else -2");

        // Nested lets - both thenBranch and elseBranch are LetExpr body's
        CHECK(IfExpr(new EqualExpr(new NumExpr(1), new NumExpr(1)),
                     &if1,
                     &if1).to_string(true) == "_if 1 == 1\n"
                                              "_then _if _true\n"
                                              "      _then 3\n"
                                              "      _else -2\n"
                                              "_else _if _true\n"
                                              "      _then 3\n"
                                              "      _else -2");
        // IfExpr is lhs of a +/* body
        CHECK(AddExpr(&if1, &num1).to_string(true) == "(_if _true\n"
                                                      " _then 3\n"
                                                      " _else -2) + 3");

        // IfExpr is rhs of a parenthesized +/* body
        CHECK(MultExpr(&num1, new AddExpr(&num1, &if1)).to_string(true) == "3 * (3 + _if _true\n"
                                                                           "         _then 3\n"
                                                                           "         _else -2)");
        // IfExpr is rhs of an unparenthesized +/* and would NOT have needed parentheses in the surrounding context
        CHECK(MultExpr(&num1, &if1).to_string(true) == "3 * _if _true\n"
                                                       "    _then 3\n"
                                                       "    _else -2");


        CHECK(MultExpr(&if1, new LetExpr(new VarExpr("x"),
                                         new NumExpr(2),
                                         new AddExpr(new VarExpr("x"), new NumExpr(2))))
                      .to_string(true) == "(_if _true\n"
                                          " _then 3\n"
                                          " _else -2) * _let x = 2\n"
                                          "             _in  x + 2");

        // IfExpr is rhs of an unparenthesized +/* and would have needed parentheses in the surrounding context
        CHECK(AddExpr(new MultExpr(new NumExpr(3),
                                   new IfExpr(new BoolExpr(true),
                                              new NumExpr(3),
                                              new NumExpr(-2))), &num1)
                      .to_string(true) == "3 * (_if _true\n"
                                          "     _then 3\n"
                                          "     _else -2) + 3");

        // Class example 1
        CHECK(LetExpr(new VarExpr("same"),
                      new EqualExpr(new NumExpr(1), new NumExpr(2)),
                      new IfExpr(new EqualExpr(new NumExpr(1), new NumExpr(2)),
                                 new AddExpr(new BoolExpr(false), new NumExpr(5)),
                                 new NumExpr(88)))
                      .to_string(true) == "_let same = 1 == 2\n"
                                          "_in  _if 1 == 2\n"
                                          "     _then _false + 5\n"
                                          "     _else 88");
        // Class example 2
        CHECK(IfExpr(new AddExpr(new NumExpr(4), new NumExpr(1)),
                     new NumExpr(2),
                     new NumExpr(3))
                      .to_string(true) == "_if 4 + 1\n"
                                          "_then 2\n"
                                          "_else 3");
    }

    SECTION("Should pretty print FunExpr") {
        // Nested funs - body is a FunExpr body
        CHECK(FunExpr("x", new FunExpr("x",
                                       new EqualExpr(new VarExpr("x"), new NumExpr(1))))
                      .to_string(true) == "_fun (x)\n"
                                          "  _fun (x)\n"
                                          "    x == 1");

        // Nested funs - fun is rhs of a let
        CHECK(LetExpr(&var1,
                      new FunExpr("x", new EqualExpr(new VarExpr("x"), new NumExpr(1))),
                      new AddExpr(&var1, &num2))
                      .to_string(true) == "_let x = _fun (x)\n"
                                          "           x == 1\n"
                                          "_in  x + -2");

        // LetExpr is lhs of a +/* body
        CHECK(AddExpr(new FunExpr("x", new EqualExpr(new VarExpr("x"), new NumExpr(1))), &num1)
                      .to_string(true) == "(_fun (x)\n"
                                          "   x == 1) + 3");

        CHECK(MultExpr(&num1,
                       new FunExpr("x", new EqualExpr(new VarExpr("x"), new NumExpr(1))))
                      .to_string(true) == "3 * _fun (x)\n"
                                          "      x == 1");

        // LetExpr is rhs of an unparenthesized +/* and would NOT have needed parentheses in the surrounding context
        CHECK(MultExpr(&num1, new FunExpr("x", new EqualExpr(new VarExpr("x"),
                                                             new AddExpr(new VarExpr("x"),
                                                                         new NumExpr(1)))))
                      .to_string(true) == "3 * _fun (x)\n"
                                          "      x == x + 1");
    }

    SECTION("Should pretty print CallExpr") {

        CHECK(CallExpr(new VarExpr("f"), new VarExpr("x"))
                      .to_string(true) == "f(x)");

        // Nested funs - body is a FunExpr body
        CHECK(CallExpr(new FunExpr("x", new EqualExpr(new VarExpr("x"), new NumExpr(1))),
                       new AddExpr(new VarExpr("x"), new NumExpr(1)))
                      .to_string(true) == "(_fun (x)\n"
                                          "   x == 1)(x + 1)");

        // Recursive
        CHECK(CallExpr(new CallExpr(new VarExpr("f"), new VarExpr("f")), new NumExpr(10))
                      .to_string(true) == "f(f)(10)");
    }
}

