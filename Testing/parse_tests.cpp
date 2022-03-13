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
#include "../shared_ptr.h"

TEST_CASE("Parse numbers") {
    CHECK(parse_str("(1)")->equals(NEW(NumExpr)(1)));
    CHECK(parse_str("(((1)))")->equals(NEW(NumExpr)(1)));
    CHECK_THROWS_WITH(parse_str("(1"), "Missing close parenthesis");
    CHECK(parse_str("1")->equals(NEW(NumExpr)(1)));
    CHECK(parse_str("10")->equals(NEW(NumExpr)(10)));
    CHECK(parse_str("-3")->equals(NEW(NumExpr)(-3)));
    CHECK(parse_str("  \n 5  ")->equals(NEW(NumExpr)(5)));
    CHECK_THROWS_WITH(parse_str("-"), "Invalid minus sign placement");

    CHECK_THROWS_WITH(parse_str("()"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str(" -   5  "), "Invalid minus sign placement");
}

TEST_CASE("Parse variables") {
    CHECK(parse_str("x")->equals(NEW(VarExpr)("x")));
    CHECK(parse_str("xyz")->equals(NEW(VarExpr)("xyz")));
    CHECK(parse_str("xYz")->equals(NEW(VarExpr)("xYz")));
    CHECK_THROWS_WITH(parse_str("x_z"),
                      "Unexpected input after expression!"); // It considers "x" as a complete expression
    CHECK_THROWS_WITH(parse_str("x2z"),
                      "Unexpected input after expression!");
}

TEST_CASE("Parse _true and _false") {
    CHECK(parse_str("   _true + 1")
                  ->equals(NEW(AddExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1))));
    CHECK(parse_str("  ( ( ( _false)))")
                  ->equals(NEW(BoolExpr)(false)));

    CHECK_THROWS_WITH(parse_str("_tru"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("_f"), "Unexpected token");
}

TEST_CASE("Parse mult and add expressions") {
    CHECK(parse_str("x + y")->equals(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))));
    CHECK(parse_str("x * \n y")->equals(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))));
    CHECK(parse_str("z * x + y")
                  ->equals(NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("z"), NEW(VarExpr)("x")),
                                        NEW(VarExpr)("y"))));
    CHECK(parse_str("z * (1 + y)")
                  ->equals(NEW(MultExpr)(NEW(VarExpr)("z"),
                                         NEW(AddExpr)(NEW(NumExpr)(1), NEW(VarExpr)("y")))));
    CHECK_THROWS_WITH(parse_str("x ** x"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str("xY *+ u"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str("xY *"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str("+x"), "Invalid input!");
}

TEST_CASE("Parse LetExpr expressions") {
    CHECK(parse_str("_let x=1 _in x + 1\n")
                  ->equals(NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1),
                                        NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));

    CHECK(parse_str("(_let x=1 _in x) + 1\n")
                  ->equals(NEW(AddExpr)(NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1),
                                                     NEW(VarExpr)("x")), NEW(NumExpr)(1))));

    CHECK(parse_str("2 + _let x=1 _in x + 1")
                  ->equals(NEW(AddExpr)(NEW(NumExpr)(2), NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1),
                                                                      NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                   NEW(NumExpr)(1))))));

    CHECK(parse_str(" _let x=(_let y = 0 _in y * 2) _in x + 1")
                  ->equals(NEW(LetExpr)(NEW(VarExpr)("x"),
                                        NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(0),
                                                     NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))),
                                        NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));

    CHECK(parse_str("_let x=2 _in (_let y = 0 _in y * 2)")
                  ->equals(NEW(LetExpr)(NEW(VarExpr)("x"),
                                        NEW(NumExpr)(2),
                                        NEW(LetExpr)(NEW(VarExpr)("y"),
                                                     NEW(NumExpr)(0),
                                                     NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))))));

    CHECK_THROWS_WITH(parse_str("_let x _in y + 1"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("x=3 _in y + 1"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("_let"), "Empty variable name");
    CHECK_THROWS_WITH(parse_str("_let x = 3"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("_et x = 2 _inx + 3"), "Unknown underscore syntax");
}

TEST_CASE("Parse IfExpr") {
    CHECK(parse_str("_if x _then x _else 1")
                  ->equals(NEW(IfExpr)(NEW(VarExpr)("x"),
                                       NEW(VarExpr)("x"),
                                       NEW(NumExpr)(1))));

    CHECK(parse_str("(_if x== 2 _then x _else 1) + 1")
                  ->equals(NEW(AddExpr)(NEW(IfExpr)(NEW(EqualExpr)(NEW(VarExpr)("x"),
                                                                   NEW(NumExpr)(2)),
                                                    NEW(VarExpr)("x"),
                                                    NEW(NumExpr)(1)),
                                        NEW(NumExpr)(1))));

    CHECK(parse_str("2 + _if x _then x _else 1 + 1")
                  ->equals(NEW(AddExpr)(NEW(NumExpr)(2),
                                        NEW(IfExpr)(NEW(VarExpr)("x"),
                                                    NEW(VarExpr)("x"),
                                                    NEW(AddExpr)(NEW(NumExpr)(1),
                                                                 NEW(NumExpr)(1))))));

    CHECK(parse_str(" _if _true _then (_let y = 0 _in y * 2) _else x + 1")
                  ->equals(NEW(IfExpr)(NEW(BoolExpr)(true),
                                       NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(0),
                                                    NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))),
                                       NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));

    CHECK(parse_str("_if _false _then _let y = 0 _in y * 2 _else 1")
                  ->equals(NEW(IfExpr)(NEW(BoolExpr)(false),
                                       NEW(LetExpr)(NEW(VarExpr)("y"),
                                                    NEW(NumExpr)(0),
                                                    NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))),
                                       NEW(NumExpr)(1))));

    CHECK_THROWS_WITH(parse_str("_if x _then y + 1"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("_if"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str("_if x == 3"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("_ifx == 3 _then x+2"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("_if x == 2 _thenx + 3"), "Unexpected token");
}

TEST_CASE("Parse EqualExpr") {
    CHECK(parse_str("1 + 1 == 2")->equals(
            NEW(EqualExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)), NEW(NumExpr)(2))));
    CHECK(parse_str("1 == 1 == 2")->equals(
            NEW(EqualExpr)(NEW(NumExpr)(1), NEW(EqualExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))));
}

TEST_CASE("Parse FunExpr") {
    CHECK(parse_str("   _fun (x) y + 3")
                  ->equals(NEW(FunExpr)("x",
                                        NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)))));
    CHECK(parse_str("   _fun (var) _let y = 3 _in var + y")
                  ->equals(NEW(FunExpr)("var",
                                        NEW(LetExpr)(NEW(VarExpr)("y"),
                                                     NEW(NumExpr)(3),
                                                     NEW(AddExpr)(NEW(VarExpr)("var"), NEW(VarExpr)("y"))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "x + 1\n"
                    "_in f(5)")->equals((NEW(LetExpr)(NEW(VarExpr)("f"),
                                                      NEW(FunExpr)("x",
                                                                   NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))),
                                                      NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "7\n"
                    "_in f(5)")->equals((NEW(LetExpr)(NEW(VarExpr)("f"),
                                                      NEW(FunExpr)("x", NEW(NumExpr)(7)),
                                                      NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "_true\n"
                    "_in f(5)")->equals((NEW(LetExpr)(NEW(VarExpr)("f"),
                                                      NEW(FunExpr)("x", NEW(BoolExpr)(true)),
                                                      NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "x + _true\n"
                    "_in f(5)")->equals((NEW(LetExpr)(NEW(VarExpr)("f"),
                                                      NEW(FunExpr)("x",
                                                                   NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                NEW(BoolExpr)(true))),
                                                      NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "x + _true\n"
                    "_in 5 + 1")->equals((NEW(LetExpr)(NEW(VarExpr)("f"),
                                                       NEW(FunExpr)("x",
                                                                    NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                 NEW(BoolExpr)(true))),
                                                       NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "7\n"
                    "_in f(5 + _true)")->equals((NEW(LetExpr)(NEW(VarExpr)("f"),
                                                              NEW(FunExpr)("x", NEW(NumExpr)(7)),
                                                              NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                            NEW(AddExpr)(NEW(NumExpr)(5),
                                                                                         NEW(BoolExpr)(
                                                                                                 true)))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "x + 1\n"
                    "_in f + 5")->equals((NEW(LetExpr)(NEW(VarExpr)("f"),
                                                       NEW(FunExpr)("x",
                                                                    NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))),
                                                       NEW(AddExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "x + 1\n"
                    "_in _if _false\n"
                    "    _then f(5)\n"
                    "    _else f(6)")->equals((NEW(LetExpr)(NEW(VarExpr)("f"),
                                                            NEW(FunExpr)("x",
                                                                         NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                      NEW(NumExpr)(1))),
                                                            NEW(IfExpr)(NEW(BoolExpr)(false),
                                                                        NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                                      NEW(NumExpr)(5)),
                                                                        NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                                      NEW(NumExpr)(6)))))));
    CHECK(parse_str("_let f = _fun (x)\n"
                    "              x + 1\n"
                    "_in _let g = _fun (y)\n"
                    "               y + 2\n"
                    "_in _if _true\n"
                    "    _then f(5)\n"
                    "    _else g(5)")->equals(NEW(LetExpr)(NEW(VarExpr)("f"),
                                                           NEW(FunExpr)("x",
                                                                        NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                     NEW(NumExpr)(1))),
                                                           NEW(LetExpr)(NEW(VarExpr)("g"),
                                                                        NEW(FunExpr)("y",
                                                                                     NEW(AddExpr)(NEW(VarExpr)("y"),
                                                                                                  NEW(NumExpr)(2))),
                                                                        NEW(IfExpr)(NEW(BoolExpr)(true),
                                                                                    NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                                                  NEW(NumExpr)(5)),
                                                                                    NEW(CallExpr)(NEW(VarExpr)("g"),
                                                                                                  NEW(NumExpr)(5)))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "              x + 1\n"
                    "_in _let g = _fun (y)\n"
                    "               y + 2\n"
                    "    _in f(g(5))")->equals(NEW(LetExpr)(NEW(VarExpr)("f"),
                                                            NEW(FunExpr)("x",
                                                                         NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                      NEW(NumExpr)(1))),
                                                            NEW(LetExpr)(NEW(VarExpr)("g"),
                                                                         NEW(FunExpr)("y",
                                                                                      NEW(AddExpr)(NEW(VarExpr)("y"),
                                                                                                   NEW(NumExpr)(2))),
                                                                         NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                                       NEW(CallExpr)(NEW(VarExpr)("g"),
                                                                                                     NEW(NumExpr)(
                                                                                                             5)))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "              x + 1\n"
                    "_in _let g = _fun (y)\n"
                    "               f(y + 2)\n"
                    "    _in g(5)")->equals(NEW(LetExpr)(NEW(VarExpr)("f"),
                                                         NEW(FunExpr)("x",
                                                                      NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))),
                                                         NEW(LetExpr)(NEW(VarExpr)("g"),
                                                                      NEW(FunExpr)("y", NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                                                      NEW(AddExpr)(
                                                                                                              NEW(VarExpr)(
                                                                                                                      "y"),
                                                                                                              NEW(NumExpr)(
                                                                                                                      2)))),
                                                                      NEW(CallExpr)(NEW(VarExpr)("g"),
                                                                                    NEW(NumExpr)(5))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "              x + 1\n"
                    "_in _let g = _fun (x)\n"
                    "               f(2) + x\n"
                    "    _in g(5)")->equals(NEW(LetExpr)(NEW(VarExpr)("f"),
                                                         NEW(FunExpr)("x",
                                                                      NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))),
                                                         NEW(LetExpr)(NEW(VarExpr)("g"),
                                                                      NEW(FunExpr)("x", NEW(AddExpr)(
                                                                              NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                                            NEW(NumExpr)(2)),
                                                                              NEW(VarExpr)("x"))),
                                                                      NEW(CallExpr)(NEW(VarExpr)("g"),
                                                                                    NEW(NumExpr)(5))))));

    CHECK(parse_str("_let f = _if _false\n"
                    "_then _fun (x)\n"
                    "x + 1\n"
                    "_else _fun (x)\n"
                    "x + 2\n"
                    "_in f(5)")->equals(NEW(LetExpr)(NEW(VarExpr)("f"),
                                                     NEW(IfExpr)(NEW(BoolExpr)(false),
                                                                 NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                                NEW(NumExpr)(1))),
                                                                 NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                                NEW(NumExpr)(2)))),
                                                     NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5)))));

    CHECK(parse_str("(_if _false\n"
                    "_then _fun (x)\n"
                    "x + 1\n"
                    "_else _fun (x)\n"
                    "x + 2)(5)")->equals(NEW(CallExpr)(NEW(IfExpr)(NEW(BoolExpr)(false),
                                                                   NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                                  NEW(NumExpr)(1))),
                                                                   NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                                  NEW(NumExpr)(2)))),
                                                       NEW(NumExpr)(5))));

    CHECK(parse_str("_let f = _fun (g)\n"
                    "g(5)\n"
                    "_in _let g = _fun (y)\n"
                    "y + 2\n"
                    "_in f(g)")->equals(NEW(LetExpr)(NEW(VarExpr)("f"),
                                                     NEW(FunExpr)("g",
                                                                  NEW(CallExpr)(NEW(VarExpr)("g"), NEW(NumExpr)(5))),
                                                     NEW(LetExpr)(NEW(VarExpr)("g"),
                                                                  NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("y"),
                                                                                                 NEW(NumExpr)(2))),
                                                                  NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                                NEW(VarExpr)("g"))))));

    CHECK(parse_str("_let f = _fun (g)\n"
                    "g(5)\n"
                    "_in f(_fun (y)\n"
                    "y + 2)")->equals(NEW(LetExpr)(NEW(VarExpr)("f"),
                                                   NEW(FunExpr)("g", NEW(CallExpr)(NEW(VarExpr)("g"), NEW(NumExpr)(5))),
                                                   NEW(CallExpr)(NEW(VarExpr)("f"), NEW(FunExpr)("y", NEW(AddExpr)(
                                                           NEW(VarExpr)("y"), NEW(NumExpr)(2)))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "_fun (y)\n"
                    "x + y\n"
                    "_in (f(5))(1)")->equals(NEW(LetExpr)(NEW(VarExpr)("f"),
                                                          NEW(FunExpr)("x",
                                                                       NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                                      NEW(VarExpr)(
                                                                                                              "y")))),
                                                          NEW(CallExpr)(
                                                                  NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5)),
                                                                  NEW(NumExpr)(1)))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "_fun (y)\n"
                    "x + y\n"
                    "_in (f(5))(1)")->equals(NEW(LetExpr)(NEW(VarExpr)("f"),
                                                          NEW(FunExpr)("x",
                                                                       NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                                      NEW(VarExpr)(
                                                                                                              "y")))),
                                                          NEW(CallExpr)(
                                                                  NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5)),
                                                                  NEW(NumExpr)(1)))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "_fun (y)\n"
                    "x + y\n"
                    "_in f(5)(1)")->equals(NEW(LetExpr)(NEW(VarExpr)("f"),
                                                        NEW(FunExpr)("x",
                                                                     NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                                    NEW(VarExpr)(
                                                                                                            "y")))),
                                                        NEW(CallExpr)(NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5)),
                                                                      NEW(NumExpr)(1)))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "_fun (g)\n"
                    "g(x + 1)\n"
                    "_in _let g = _fun (y)\n"
                    "y + 2\n"
                    "_in (f(5))(g)")->equals(NEW(LetExpr)(NEW(VarExpr)("f"), NEW(FunExpr)("x", NEW(FunExpr)("g",
                                                                                                            NEW(CallExpr)(
                                                                                                                    NEW(VarExpr)(
                                                                                                                            "g"),
                                                                                                                    NEW(AddExpr)(
                                                                                                                            NEW(VarExpr)(
                                                                                                                                    "x"),
                                                                                                                            NEW(NumExpr)(
                                                                                                                                    1))))),
                                                          NEW(LetExpr)(NEW(VarExpr)("g"),
                                                                       NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("y"),
                                                                                                      NEW(NumExpr)(2))),
                                                                       NEW(CallExpr)(NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                                                   NEW(NumExpr)(5)),
                                                                                     NEW(VarExpr)("g"))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "_fun (g)\n"
                    "g(x + 1)\n"
                    "_in _let g = _fun (y)\n"
                    "y + 2\n"
                    "_in f(5(g))")->equals(NEW(LetExpr)(NEW(VarExpr)("f"), NEW(FunExpr)("x", NEW(FunExpr)("g",
                                                                                                          NEW(CallExpr)(
                                                                                                                  NEW(VarExpr)(
                                                                                                                          "g"),
                                                                                                                  NEW(AddExpr)(
                                                                                                                          NEW(VarExpr)(
                                                                                                                                  "x"),
                                                                                                                          NEW(NumExpr)(
                                                                                                                                  1))))),
                                                        NEW(LetExpr)(NEW(VarExpr)("g"),
                                                                     NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("y"),
                                                                                                    NEW(NumExpr)(2))),
                                                                     NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                                   NEW(CallExpr)(NEW(NumExpr)(5),
                                                                                                 NEW(VarExpr)(
                                                                                                         "g")))))));

    CHECK(parse_str("_let f = _fun (x)\n"
                    "_fun (g)\n"
                    "g(x + 1)\n"
                    "_in _let g = _fun (y)\n"
                    "y + 2\n"
                    "_in f(5)(g)")->equals(NEW(LetExpr)(NEW(VarExpr)("f"), NEW(FunExpr)("x", NEW(FunExpr)("g",
                                                                                                          NEW(CallExpr)(
                                                                                                                  NEW(VarExpr)(
                                                                                                                          "g"),
                                                                                                                  NEW(AddExpr)(
                                                                                                                          NEW(VarExpr)(
                                                                                                                                  "x"),
                                                                                                                          NEW(NumExpr)(
                                                                                                                                  1))))),
                                                        NEW(LetExpr)(NEW(VarExpr)("g"),
                                                                     NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("y"),
                                                                                                    NEW(NumExpr)(2))),
                                                                     NEW(CallExpr)(NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                                                 NEW(NumExpr)(5)),
                                                                                   NEW(VarExpr)("g"))))));

    CHECK(parse_str("_let f = _fun (f)\n"
                    "_fun (x)\n"
                    "_if x == 0\n"
                    "_then 0\n"
                    "_else x + f(f)(x + -1)\n"
                    "_in f(f)(3)")->equals(NEW(LetExpr)(NEW(VarExpr)("f"),
                                                        NEW(FunExpr)("f", NEW(FunExpr)("x", NEW(IfExpr)(
                                                                NEW(EqualExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(0)),
                                                                NEW(NumExpr)(0),
                                                                NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                             NEW(CallExpr)(
                                                                                     NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                                                   NEW(VarExpr)("f")),
                                                                                     NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                                  NEW(NumExpr)(
                                                                                                          -1))))))),
                                                        NEW(CallExpr)(
                                                                NEW(CallExpr)(NEW(VarExpr)("f"), NEW(VarExpr)("f")),
                                                                NEW(NumExpr)(3)))));


    CHECK_THROWS_WITH(parse_str("   _fun (x + 3) x * 2 + 3"), "Unexpected token");
    CHECK_THROWS_WITH(parse_str("   _fun () x * 2 + 3"), "Empty variable name");
    CHECK_THROWS_WITH(parse_str("   _fin (x) x * 2 + 3"), "Unexpected token");
}

TEST_CASE("Parse CallExpr") {
    CHECK(parse_str("f ( y + 3 )")
                  ->equals(NEW(CallExpr)(NEW(VarExpr)("f"),
                                         NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)))));
    CHECK(parse_str("x(_let y = 3 _in var + y)")
                  ->equals(NEW(CallExpr)(NEW(VarExpr)("x"),
                                         NEW(LetExpr)(NEW(VarExpr)("y"),
                                                      NEW(NumExpr)(3),
                                                      NEW(AddExpr)(NEW(VarExpr)("var"), NEW(VarExpr)("y"))))));

    CHECK(parse_str("(f)(x)")->equals(NEW(CallExpr)(NEW(VarExpr)("f"), NEW(VarExpr)("x"))));
    CHECK(parse_str("f(3)(2)")->equals(NEW(CallExpr)(NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                 NEW(NumExpr)(3)),
                                                    NEW(NumExpr)(2))));
    CHECK(parse_str("(_fun (x) x + 1)(5)")->equals(
            NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))),
                         NEW(NumExpr)(5))));

    CHECK_THROWS_WITH(parse_str("f 5"), "Unexpected input after expression!");
}