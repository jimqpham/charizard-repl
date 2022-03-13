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

bool compareParsedExpressions(const std::string s1, const std::string s2) {
    PTR(Expr) e1 = parse_str(s1);
    PTR(Expr) e2 = parse_str(s2);
    return e1->equals(e2);
}

bool compareParsedExpressions(const std::string s, PTR(Expr) e) {
    PTR(Expr) e1 = parse_str(s);
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
        CHECK(compareParsedExpressions("10 + -10", NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(-10))));
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
                                       NEW(EqualExpr)(NEW(NumExpr)(10), NEW(NumExpr)(-2))));
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
        CHECK(compareParsedExpressions("ch", NEW(VarExpr)("ch")));
    }

    SECTION("VarExpr should not be equal to VarExpr of different name") {
        CHECK(!compareParsedExpressions("ch", NEW(VarExpr)("cv")));
    }

    SECTION("VarExpr should not equal objects of other classes") {
        CHECK(!compareParsedExpressions("ch", NEW(NumExpr)(10)));
    }
}

TEST_CASE("Test LetExpr value_equals") {
    SECTION("LetExpr should equal LetExpr of same components") {
        CHECK(compareParsedExpressions("(_let x = 5 _in (x + 4))",
                                       NEW(LetExpr)(NEW(VarExpr)("x"),
                                                    NEW(NumExpr)(5),
                                                    NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(4)))));
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
                                       NEW(IfExpr)(NEW(BoolExpr)(true),
                                                   NEW(NumExpr)(4),
                                                   NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
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
        PTR(NumExpr) four = NEW(NumExpr)(4);
        PTR(BoolExpr) tr = NEW(BoolExpr)(true);
        CHECK((NEW(CallExpr)(tr, four))
                      ->equals(NEW(CallExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(4))));
    }

    SECTION("CallExpr should not equal CallExpr of different components") {
        PTR(NumExpr) num1 = NEW(NumExpr)(4);
        PTR(NumExpr) num2 = NEW(NumExpr)(5);
        PTR(BoolExpr) tr = NEW(BoolExpr)(true);
        PTR(BoolExpr) fls = NEW(BoolExpr)(false);
        CHECK(!(NEW(CallExpr)(tr, num2))
                ->equals(NEW(CallExpr)(fls, num2)));
        CHECK(!(NEW(CallExpr)(tr, num1))
                ->equals(NEW(CallExpr)(tr, num2)));
    }

    SECTION("LetExpr should not equal objects of other classes") {
        PTR(NumExpr) num1 = NEW(NumExpr)(4);
        PTR(VarExpr) var1 = NEW(VarExpr)("x");
        PTR(AddExpr) add1 = NEW(AddExpr)(var1, num1);
        CHECK(!(NEW(CallExpr)(var1, num1))->equals(add1));
    }
}

TEST_CASE("Test FunExpr value_equals") {
    SECTION("FunExpr should equal FunExpr of same components") {
        PTR(NumExpr) four = NEW(NumExpr)(4);
        CHECK((NEW(FunExpr)("arg", four))
                      ->equals(NEW(FunExpr)("arg", NEW(NumExpr)(4))));
    }

    SECTION("FunExpr should not equal FunExpr of different components") {
        PTR(NumExpr) num1 = NEW(NumExpr)(4);
        PTR(NumExpr) num2 = NEW(NumExpr)(5);
        CHECK(!(NEW(FunExpr)("arg", num2))
                ->equals(NEW(FunExpr)("not arg", num2)));
        CHECK(!(NEW(FunExpr)("arg", num1))
                ->equals(NEW(FunExpr)("arg", num2)));
    }

    SECTION("LetExpr should not equal objects of other classes") {
        PTR(NumExpr) num1 = NEW(NumExpr)(4);
        PTR(AddExpr) add1 = NEW(AddExpr)(num1, num1);
        CHECK(!(NEW(FunExpr)("x", num1))->equals(add1));
    }
}


