#include "catch.h"
#include "../Vals/num_val.h"
#include "../Expressions/num_expr.h"
#include "../Vals/bool_val.h"
#include "../Expressions/bool_expr.h"
#include "../Vals/fun_val.h"
#include "../CmdLines/parse.h"
#include "../Expressions/fun_expr.h"
#include "../Expressions/add_expr.h"
#include "../Expressions/var_expr.h"

TEST_CASE("Test NumVal methods") {

    NumVal *ten = new NumVal(10);
    NumVal *otherTen = new NumVal(10);
    NumVal *two = new NumVal(2);
    NumVal *negTwo = new NumVal(-2);
    NumVal *five = new NumVal(5);
    NumVal *eight = new NumVal(8);
    NumVal *twelve = new NumVal(12);
    NumVal *nullVal = nullptr;

    SECTION("The value_equals method") {
        CHECK(ten->value_equals(otherTen));
        CHECK(!ten->value_equals(eight));
        CHECK(!ten->value_equals(nullVal));
    }

    SECTION("The add_to method") {
        CHECK(ten->value_equals(two->add_to(eight)));
        CHECK(ten->value_equals(negTwo->add_to(twelve)));
        CHECK_THROWS_WITH(two->add_to(nullVal), "add of non-number");
    }

    SECTION("The mult_by method") {
        CHECK((two->mult_by(five))->value_equals(ten));
        CHECK_THROWS_WITH(five->mult_by(nullVal), "mult of non-number");
    }

    SECTION("The to_string method") {
        CHECK(strcmp(ten->to_string().c_str(), "10") == 0);
        CHECK(strcmp(negTwo->to_string().c_str(), "-2") == 0);
    }

    SECTION("The to_expr method") {
        CHECK(ten->to_expr()->equals(new NumExpr(10)));
        CHECK(negTwo->to_expr()->equals(new NumExpr(-2)));
    }

    SECTION("The call method") {
        CHECK_THROWS_WITH(ten->call(new NumVal(-2)), "call of non-function val");
    }
}

TEST_CASE("Test BoolVal methods") {

    BoolVal *tr = new BoolVal(true);
    BoolVal *otherTr = new BoolVal(true);
    BoolVal *fls = new BoolVal(false);
    NumVal *two = new NumVal(2);

    SECTION("The value_equals method") {
        CHECK(tr->value_equals(otherTr));
        CHECK(!tr->value_equals(fls));
        CHECK(!tr->value_equals(two));
    }

    SECTION("The add_to method") {
        CHECK_THROWS_WITH(tr->add_to(two), "add of non-number");
        CHECK_THROWS_WITH(tr->add_to(fls), "add of non-number");
    }

    SECTION("The mult_by method") {
        CHECK_THROWS_WITH(tr->mult_by(two), "mult of non-number");
        CHECK_THROWS_WITH(tr->mult_by(fls), "mult of non-number");
    }

    SECTION("The to_string method") {
        CHECK(strcmp(tr->to_string().c_str(), "_true") == 0);
        CHECK(strcmp(fls->to_string().c_str(), "_false") == 0);
    }

    SECTION("The to_expr method") {
        CHECK(tr->to_expr()->equals(new BoolExpr(true)));
        CHECK(fls->to_expr()->equals(new BoolExpr(false)));
    }

    SECTION("The call method") {
        CHECK_THROWS_WITH(tr->call(new BoolVal(false)), "call of non-function val");
    }
}

TEST_CASE("Test FunVal methods") {

    FunVal *f1 = new FunVal("x", parse_str("x + 1"));

    SECTION("The value_equals method") {
        Val *otherF1 = parse_str("_fun (x) x + 1")->interp();
        CHECK(f1->value_equals(otherF1));
        CHECK(!f1->value_equals(new FunVal("y", parse_str("x + 1"))));
        CHECK(!f1->value_equals(new FunVal("x", parse_str("x + 2"))));
        CHECK(!f1->value_equals(new NumVal(10)));
    }

    SECTION("The add_to method") {
        CHECK_THROWS_WITH(f1->add_to(new NumVal(2)), "add of non-number");
    }

    SECTION("The mult_by method") {
        CHECK_THROWS_WITH(f1->mult_by(new NumVal(2)), "mult of non-number");
    }

    SECTION("The to_string method") {
        CHECK(f1->to_string() == "(_fun (x) (x+1))");
    }

    SECTION("The to_expr method") {
        CHECK(f1->to_expr()->equals(new FunExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(1)))));
    }

    SECTION("The call method") {
        CHECK(f1->call(new NumVal(10))->value_equals(new NumVal(11)));
    }
}