#include "catch.h"
#include "../Expressions/num_expr.h"
#include "../Expressions/add_expr.h"
#include "../Expressions/mult_expr.h"
#include "../Expressions/var_expr.h"
#include "../CmdLines/parse.h"
#include "../Expressions/let_expr.h"
#include "../Expressions/if_expr.h"
#include "../Expressions/bool_expr.h"
#include "../Expressions/equal_expr.h"
#include "../Expressions/fun_expr.h"
#include "../Expressions/call_expr.h"

TEST_CASE("Parse numbers") {
    CHECK(parse_str("(1)")->equals(new NumExpr(1)));
    CHECK(parse_str("(((1)))")->equals(new NumExpr(1)));
    CHECK_THROWS_WITH(parse_str("(1"), "Missing close parenthesis");
    CHECK(parse_str("1")->equals(new NumExpr(1)));
    CHECK(parse_str("10")->equals(new NumExpr(10)));
    CHECK(parse_str("-3")->equals(new NumExpr(-3)));
    CHECK(parse_str("  \n 5  ")->equals(new NumExpr(5)));
    CHECK_THROWS_WITH(parse_str("-"), "Invalid minus sign placement");

    CHECK_THROWS_WITH(parse_str("()"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str(" -   5  "), "Invalid minus sign placement");
}

TEST_CASE("Parse variables") {
    CHECK(parse_str("x")->equals(new VarExpr("x")));
    CHECK(parse_str("xyz")->equals(new VarExpr("xyz")));
    CHECK(parse_str("xYz")->equals(new VarExpr("xYz")));
    CHECK_THROWS_WITH(parse_str("x_z"),
                      "Unexpected input after expression!"); // It considers "x" as a complete expression
    CHECK_THROWS_WITH(parse_str("x2z"),
                      "Unexpected input after expression!");
}

TEST_CASE("Parse _true and _false") {
    CHECK(parse_str("   _true + 1")
                  ->equals(new AddExpr(new BoolExpr(true), new NumExpr(1))));
    CHECK(parse_str("  ( ( ( _false)))")
                  ->equals(new BoolExpr(false)));

    CHECK_THROWS_WITH(parse_str("_tru"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("_f"), "Unexpected token");
}

TEST_CASE("Parse mult and add expressions") {
    CHECK(parse_str("x + y")->equals(new AddExpr(new VarExpr("x"), new VarExpr("y"))));
    CHECK(parse_str("x * \n y")->equals(new MultExpr(new VarExpr("x"), new VarExpr("y"))));
    CHECK(parse_str("z * x + y")
                  ->equals(new AddExpr(new MultExpr(new VarExpr("z"), new VarExpr("x")),
                                       new VarExpr("y"))));
    CHECK(parse_str("z * (1 + y)")
                  ->equals(new MultExpr(new VarExpr("z"),
                                        new AddExpr(new NumExpr(1), new VarExpr("y")))));
    CHECK_THROWS_WITH(parse_str("x ** x"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str("xY *+ u"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str("xY *"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str("+x"), "Invalid input!");
}

TEST_CASE("Parse LetExpr expressions") {
    CHECK(parse_str("_let x=1 _in x + 1\n")
                  ->equals(new LetExpr(new VarExpr("x"), new NumExpr(1),
                                       new AddExpr(new VarExpr("x"), new NumExpr(1)))));

    CHECK(parse_str("(_let x=1 _in x) + 1\n")
                  ->equals(new AddExpr(new LetExpr(new VarExpr("x"), new NumExpr(1),
                                                   new VarExpr("x")), new NumExpr(1))));

    CHECK(parse_str("2 + _let x=1 _in x + 1")
                  ->equals(new AddExpr(new NumExpr(2), new LetExpr(new VarExpr("x"), new NumExpr(1),
                                                                   new AddExpr(new VarExpr("x"), new NumExpr(1))))));

    CHECK(parse_str(" _let x=(_let y = 0 _in y * 2) _in x + 1")
                  ->equals(new LetExpr(new VarExpr("x"),
                                       new LetExpr(new VarExpr("y"), new NumExpr(0),
                                                   new MultExpr(new VarExpr("y"), new NumExpr(2))),
                                       new AddExpr(new VarExpr("x"), new NumExpr(1)))));

    CHECK(parse_str("_let x=2 _in (_let y = 0 _in y * 2)")
                  ->equals(new LetExpr(new VarExpr("x"),
                                       new NumExpr(2),
                                       new LetExpr(new VarExpr("y"),
                                                   new NumExpr(0),
                                                   new MultExpr(new VarExpr("y"), new NumExpr(2))))));

    CHECK_THROWS_WITH(parse_str("_let x _in y + 1"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("x=3 _in y + 1"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("_let"), "Empty variable name");
    CHECK_THROWS_WITH(parse_str("_let x = 3"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("_et x = 2 _inx + 3"), "Unknown underscore syntax");
}

TEST_CASE("Parse IfExpr") {
    CHECK(parse_str("_if x _then x _else 1")
                  ->equals(new IfExpr(new VarExpr("x"),
                                      new VarExpr("x"),
                                      new NumExpr(1))));

    CHECK(parse_str("(_if x== 2 _then x _else 1) + 1")
                  ->equals(new AddExpr(new IfExpr(new EqualExpr(new VarExpr("x"),
                                                                new NumExpr(2)),
                                                  new VarExpr("x"),
                                                  new NumExpr(1)),
                                       new NumExpr(1))));

    CHECK(parse_str("2 + _if x _then x _else 1 + 1")
                  ->equals(new AddExpr(new NumExpr(2),
                                       new IfExpr(new VarExpr("x"),
                                                  new VarExpr("x"),
                                                  new AddExpr(new NumExpr(1),
                                                              new NumExpr(1))))));

    CHECK(parse_str(" _if _true _then (_let y = 0 _in y * 2) _else x + 1")
                  ->equals(new IfExpr(new BoolExpr(true),
                                      new LetExpr(new VarExpr("y"), new NumExpr(0),
                                                  new MultExpr(new VarExpr("y"), new NumExpr(2))),
                                      new AddExpr(new VarExpr("x"), new NumExpr(1)))));

    CHECK(parse_str("_if _false _then _let y = 0 _in y * 2 _else 1")
                  ->equals(new IfExpr(new BoolExpr(false),
                                      new LetExpr(new VarExpr("y"),
                                                  new NumExpr(0),
                                                  new MultExpr(new VarExpr("y"), new NumExpr(2))),
                                      new NumExpr(1))));

    CHECK_THROWS_WITH(parse_str("_if x _then y + 1"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("_if"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str("_if x == 3"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("_ifx == 3 _then x+2"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("_if x == 2 _thenx + 3"), "Unexpected token");
}

TEST_CASE("Parse EqualExpr") {
    CHECK(parse_str("1 + 1 == 2")->equals(new EqualExpr(new AddExpr(new NumExpr(1), new NumExpr(1)), new NumExpr(2))));
    CHECK(parse_str("1 == 1 == 2")->equals(
            new EqualExpr(new NumExpr(1), new EqualExpr(new NumExpr(1), new NumExpr(2)))));
}

TEST_CASE("Parse FunExpr") {
    CHECK(parse_str("   _fun (x) y + 3")
                  ->equals(new FunExpr("x",
                                       new AddExpr(new VarExpr("y"), new NumExpr(3)))));
    CHECK(parse_str("   _fun (var) _let y = 3 _in var + y")
                  ->equals(new FunExpr("var",
                                       new LetExpr(new VarExpr("y"),
                                                   new NumExpr(3),
                                                   new AddExpr(new VarExpr("var"), new VarExpr("y"))))));

    CHECK_THROWS_WITH(parse_str("   _fun (x + 3) x * 2 + 3"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("   _fun () x * 2 + 3"), "Empty variable name");
    CHECK_THROWS_WITH(parse_str("   _fin (x) x * 2 + 3"), "Unexpected token");
}

TEST_CASE("Parse CallExpr") {
    CHECK(parse_str("f ( y + 3 )")
                  ->equals(new CallExpr(new VarExpr("f"),
                                        new AddExpr(new VarExpr("y"), new NumExpr(3)))));
    CHECK(parse_str("x(_let y = 3 _in var + y)")
                  ->equals(new CallExpr(new VarExpr("x"),
                                        new LetExpr(new VarExpr("y"),
                                                    new NumExpr(3),
                                                    new AddExpr(new VarExpr("var"), new VarExpr("y"))))));

    CHECK(parse_str("(f)(x)")->equals(new CallExpr(new VarExpr("f"), new VarExpr("x"))));
    CHECK(parse_str("f(3)(2)")->equals(new CallExpr(new CallExpr(new VarExpr("f"),
                                                                 new NumExpr(3)),
                                                    new NumExpr(2))));

    CHECK_THROWS_WITH(parse_str("f x + 2"), "Unexpected input after expression!");
}