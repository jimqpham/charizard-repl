#include "catch.h"
#include "../Expressions/num.h"
#include "../Expressions/add.h"
#include "../Expressions/mult.h"
#include "../Expressions/var.h"
#include "../Expressions/parse.h"
#include "../Expressions/let.h"

TEST_CASE("Parse numbers") {
    CHECK(parse_str("(1)")->equals(new Num(1)));
    CHECK(parse_str("(((1)))")->equals(new Num(1)));
    CHECK_THROWS_WITH(parse_str("(1"), "Missing close parenthesis");
    CHECK(parse_str("1")->equals(new Num(1)));
    CHECK(parse_str("10")->equals(new Num(10)));
    CHECK(parse_str("-3")->equals(new Num(-3)));
    CHECK(parse_str("  \n 5  ")->equals(new Num(5)));
    CHECK_THROWS_WITH(parse_str("-"), "Invalid minus sign placement");

    CHECK_THROWS_WITH(parse_str("()"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str(" -   5  "), "Invalid minus sign placement");
}

TEST_CASE("Parse variables") {
    CHECK(parse_str("x")->equals(new Var("x")));
    CHECK(parse_str("xyz")->equals(new Var("xyz")));
    CHECK(parse_str("xYz")->equals(new Var("xYz")));
    CHECK_THROWS_WITH(parse_str("x_z"),
                      "Unexpected input after expression!"); // It considers "x" as a complete expression
    CHECK_THROWS_WITH(parse_str("x2z"),
                      "Unexpected input after expression!");
}

TEST_CASE("Parse mult and add expressions") {
    CHECK(parse_str("x + y")->equals(new Add(new Var("x"), new Var("y"))));
    CHECK(parse_str("x * \n y")->equals(new Mult(new Var("x"), new Var("y"))));
    CHECK(parse_str("z * x + y")
                  ->equals(new Add(new Mult(new Var("z"), new Var("x")),
                                   new Var("y"))));
    CHECK(parse_str("z * (1 + y)")
                  ->equals(new Mult(new Var("z"),
                                    new Add(new Num(1), new Var("y")))));
    CHECK_THROWS_WITH(parse_str("x ** x"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str("xY *+ u"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str("xY *"), "Invalid input!");
    CHECK_THROWS_WITH(parse_str("+x"), "Invalid input!");
}

TEST_CASE("Parse Let expressions") {
    CHECK(parse_str("_let x=1 _in x + 1\n")
                  ->equals(new Let(new Var("x"), new Num(1),
                                   new Add(new Var("x"), new Num(1)))));

    CHECK(parse_str("(_let x=1 _in x) + 1\n")
                  ->equals(new Add(new Let(new Var("x"), new Num(1),
                                           new Var("x")), new Num(1))));

    CHECK(parse_str("2 + _let x=1 _in x + 1")
                  ->equals(new Add(new Num(2), new Let(new Var("x"), new Num(1),
                                                       new Add(new Var("x"), new Num(1))))));

    CHECK(parse_str(" _let x=(_let y = 0 _in y * 2) _in x + 1")
                  ->equals(new Let(new Var("x"),
                                   new Let(new Var("y"), new Num(0),
                                           new Mult(new Var("y"), new Num(2))),
                                   new Add(new Var("x"), new Num(1)))));

    CHECK(parse_str("_let x=2 _in (_let y = 0 _in y * 2)")
                  ->equals(new Let(new Var("x"),
                                   new Num(2),
                                   new Let(new Var("y"),
                                           new Num(0),
                                           new Mult(new Var("y"), new Num(2))))));

    CHECK_THROWS_WITH(parse_str("_let x _in y + 1"), "Consume error");
    CHECK_THROWS_WITH(parse_str("x=3 _in y + 1"), "Unexpected input after expression!");
    CHECK_THROWS_WITH(parse_str("_let"), "Whitespace error after _let");
    CHECK_THROWS_WITH(parse_str("_let x = 3"), "Consume error");
    CHECK_THROWS_WITH(parse_str("_letx = 3 _in x+2"), "Whitespace error after _let");
    CHECK_THROWS_WITH(parse_str("_let x = 2 _inx + 3"), "Whitespace error after _in");
}