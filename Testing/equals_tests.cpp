#include "catch.h"
#include "../CmdLines/parse.h"
#include "../Expressions/num_expr.h"
#include "../Expressions/add_expr.h"
#include "../Expressions/mult_expr.h"
#include "../Expressions/var_expr.h"
#include "../Expressions/let_expr.h"
#include "../Expressions/bool_expr.h"
#include "../Expressions/equal_expr.h"
#include "../Expressions/if_expr.h"
#include "../Expressions/call_expr.h"
#include "../Expressions/fun_expr.h"
#include "../shared_ptr.h"

bool compareParsedExpressions(const std::string &s1, const std::string &s2) {
    Expr *e1 = parse_str(s1);
    Expr *e2 = parse_str(s2);
    return e1->equals(e2);
}

bool compareParsedExpressions(const std::string &s, Expr *e) {
    Expr *e1 = parse_str(s);
    return e1->equals(e);
}

TEST_CASE("Test NumExpr value_equals") {
    SECTION("NumExpr should be equal to NumExpr of same value") {
        CHECK(compareParsedExpressions("10", "10"));
    }

    SECTION("NumExpr should not be equal to NumExpr of different value") {
        CHECK(!compareParsedExpressions("10", "20"));
    }

    SECTION("NumExpr should not equal objects of other classes") {
        CHECK(!compareParsedExpressions("10", "5+5"));
    }
}

TEST_CASE("Test BoolExpr value_equals") {

    SECTION("BoolExpr should be equal to BoolExpr of same value") {
        CHECK(compareParsedExpressions("_true", "_true"));
        CHECK(compareParsedExpressions("_false", "_false"));
    }

    SECTION("BoolExpr should not be equal to BoolExpr of different value") {
        CHECK(!compareParsedExpressions("_true", "_false"));
    }

    SECTION("BoolExpr should not equal objects of other classes") {
        CHECK(!compareParsedExpressions("_false", "10"));
        CHECK(!compareParsedExpressions("_true", "10"));
    }
}

TEST_CASE("Test AddExpr value_equals") {

    SECTION("AddExpr should be equal to AddExpr of same components") {
        CHECK(compareParsedExpressions("10 + -10", new AddExpr(new NumExpr(10), new NumExpr(-10))));
    }

    SECTION("AddExpr should not be equal to AddExpr of different components") {
        CHECK(!compareParsedExpressions("10 + -10", "-10   + 10 "));
    }

    SECTION("AddExpr should not equal objects of other classes") {
        CHECK(!compareParsedExpressions("10 + -10", "1 "));
    }
}

TEST_CASE("Test MultExpr value_equals") {

    SECTION("MultExpr should be equal to MultExpr of same components") {
        CHECK(compareParsedExpressions("10 * -10", "10   * -10 "));
    }

    SECTION("MultExpr should not be equal to MultExpr of different components") {
        CHECK(!compareParsedExpressions("10 * -10", "-10   * -10 "));
    }

    SECTION("MultExpr should not equal objects of other classes") {
        CHECK(!compareParsedExpressions("10 * -10", "10   + -10 "));
    }
}

TEST_CASE("Test EqualExpr equals") {
    SECTION("EqualExpr should be equal to EqualExpr of same components") {
        CHECK(compareParsedExpressions("10 == -2",
                                       new EqualExpr(new NumExpr(10), new NumExpr(-2))));
    }

    SECTION("EqualExpr should not be equal to MultExpr of different components") {
        CHECK(!compareParsedExpressions("0 == 20", "20 == 0"));
        CHECK(!compareParsedExpressions("0 == 20", "20 == 20"));
        CHECK(!compareParsedExpressions("0 == 20", "0 == 0"));
    }

    SECTION("EqualExpr should not equal objects of other classes") {
        CHECK(!compareParsedExpressions("20 == 0", "20"));
    }
}

TEST_CASE("Test VarExpr value_equals") {
    SECTION("VarExpr should be equal to VarExpr of same name") {
        CHECK(compareParsedExpressions("ch", new VarExpr("ch")));
    }

    SECTION("VarExpr should not be equal to VarExpr of different name") {
        CHECK(!compareParsedExpressions("ch", new VarExpr("cv")));
    }

    SECTION("VarExpr should not equal objects of other classes") {
        CHECK(!compareParsedExpressions("ch", new NumExpr(10)));
    }
}

TEST_CASE("Test LetExpr value_equals") {
    SECTION("LetExpr should equal LetExpr of same components") {
        CHECK(compareParsedExpressions("(_let x = 5 _in (x + 4))",
                                       new LetExpr(new VarExpr("x"),
                                                   new NumExpr(5),
                                                   new AddExpr(new VarExpr("x"), new NumExpr(4)))));
    }

    SECTION("LetExpr should not equal LetExpr of different components") {
        CHECK(!compareParsedExpressions("(_let x = 5 _in (x + 4))", "(  _let y = 5 _in (x + 4))"));
        CHECK(!compareParsedExpressions("(_let x = 5 _in (x + 4))", "(_let x = 6 _in ( x + 4))"));
        CHECK(!compareParsedExpressions("(_let x = 5 _in (x + 4))", "(_let x = 5  _in (y + 4))"));
    }

    SECTION("LetExpr should not equal objects of other classes") {
        CHECK(!compareParsedExpressions("(_let x = 5 _in (x + 4))", "yy"));
    }
}

TEST_CASE("Test IfExpr value_equals") {
    SECTION("IfExpr should equal IfExpr of same components") {
        CHECK(compareParsedExpressions("_if _true _then 4 _else (x + 1)",
                                       new IfExpr(new BoolExpr(true),
                                                  new NumExpr(4),
                                                  new AddExpr(new VarExpr("x"), new NumExpr(1)))));
    }

    SECTION("IfExpr should not equal CallExpr of different components") {
        CHECK(!compareParsedExpressions("_if _true _then 4 _else (x + 1)", " _if 1 _then 4 _else (x + 1)"));
        CHECK(!compareParsedExpressions("_if _true _then 4 _else (x + 1)", " _if _true _then _false _else (x + 1)"));
        CHECK(!compareParsedExpressions("_if _true _then 4 _else (x + 1)", " _if _true _then 4 _else (x + 2)"));
    }

    SECTION("IfExpr should not equal objects of other classes") {
        CHECK(!compareParsedExpressions("_if _true _then 4 _else (x + 1)", " _let y = 1 _in y * 2"));
    }
}

TEST_CASE("Test CallExpr value_equals") {
    SECTION("CallExpr should equal CallExpr of same components") {
        NumExpr four = NumExpr(4);
        BoolExpr tr = BoolExpr(true);
        CHECK((new CallExpr(&tr, &four))
                      ->equals(new CallExpr(new BoolExpr(true), new NumExpr(4))));
    }

    SECTION("CallExpr should not equal CallExpr of different components") {
        NumExpr num1 = NumExpr(4);
        NumExpr num2 = NumExpr(5);
        BoolExpr tr = BoolExpr(true);
        BoolExpr fls = BoolExpr(false);
        CHECK(!(new CallExpr(&tr, &num2))
                ->equals(new CallExpr(&fls, &num2)));
        CHECK(!(new CallExpr(&tr, &num1))
                ->equals(new CallExpr(&tr, &num2)));
    }

    SECTION("LetExpr should not equal objects of other classes") {
        NumExpr num1 = NumExpr(4);
        VarExpr var1 = VarExpr("x");
        AddExpr add1 = AddExpr(&var1, &num1);
        CHECK(!(new CallExpr(&var1, &num1))->equals(&add1));
    }
}

TEST_CASE("Test FunExpr value_equals") {
    SECTION("FunExpr should equal FunExpr of same components") {
        NumExpr four = NumExpr(4);
        CHECK((new FunExpr("arg", &four))
                      ->equals(new FunExpr("arg", new NumExpr(4))));
    }

    SECTION("FunExpr should not equal FunExpr of different components") {
        NumExpr num1 = NumExpr(4);
        NumExpr num2 = NumExpr(5);
        CHECK(!(new FunExpr("arg", &num2))
                ->equals(new FunExpr("not arg", &num2)));
        CHECK(!(new FunExpr("arg", &num1))
                ->equals(new FunExpr("arg", &num2)));
    }

    SECTION("LetExpr should not equal objects of other classes") {
        NumExpr num1 = NumExpr(4);
        AddExpr add1 = AddExpr(&num1, &num1);
        CHECK(!(new FunExpr("x", &num1))->equals(&add1));
    }
}


