#include "catch.h"
#include "../Expressions/num_expr.h"
#include "../Expressions/add_expr.h"
#include "../Expressions/mult_expr.h"
#include "../Expressions/var_expr.h"
#include "../CmdLines/parse.h"
#include "../Expressions/let_expr.h"

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

    CHECK_THROWS_WITH(parse_str("_let x _in y + 1"), "Missing character: Expect =");
    CHECK_THROWS_WITH(parse_str("x=3 _in y + 1"), "Unexpected input after expression!");
    CHECK_THROWS_WITH(parse_str("_let"), "Whitespace error after _let");
    CHECK_THROWS_WITH(parse_str("_let x = 3"), "Missing character: Expect _");
    CHECK_THROWS_WITH(parse_str("_letx = 3 _in x+2"), "Whitespace error after _let");
    CHECK_THROWS_WITH(parse_str("_let x = 2 _inx + 3"), "Whitespace error after _in");
}