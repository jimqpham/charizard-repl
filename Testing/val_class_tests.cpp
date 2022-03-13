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
#include "../shared_ptr.h"

TEST_CASE("Test NumVal methods") {

    PTR(NumVal)ten = NEW(NumVal)(10);
    PTR(NumVal)otherTen = NEW(NumVal)(10);
    PTR(NumVal)two = NEW(NumVal)(2);
    PTR(NumVal)negTwo = NEW(NumVal)(-2);
    PTR(NumVal)five = NEW(NumVal)(5);
    PTR(NumVal)eight = NEW(NumVal)(8);
    PTR(NumVal)twelve = NEW(NumVal)(12);
    PTR(NumVal)nullVal = nullptr;

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
        CHECK(ten->to_expr()->equals(NEW(NumExpr)(10)));
        CHECK(negTwo->to_expr()->equals(NEW(NumExpr)(-2)));
    }

    SECTION("The call method") {
        CHECK_THROWS_WITH(ten->call(NEW(NumVal)(-2)), "call of non-function val");
    }
}

TEST_CASE("Test BoolVal methods") {

    PTR(BoolVal)tr = NEW(BoolVal)(true);
    PTR(BoolVal)otherTr = NEW(BoolVal)(true);
    PTR(BoolVal)fls = NEW(BoolVal)(false);
    PTR(NumVal)two = NEW(NumVal)(2);

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
        CHECK(tr->to_expr()->equals(NEW(BoolExpr)(true)));
        CHECK(fls->to_expr()->equals(NEW(BoolExpr)(false)));
    }

    SECTION("The call method") {
        CHECK_THROWS_WITH(tr->call(NEW(BoolVal)(false)), "call of non-function val");
    }
}

TEST_CASE("Test FunVal methods") {

    PTR(FunVal)f1 = NEW(FunVal)("x", parse_str("x + 1"));

    SECTION("The value_equals method") {
        PTR(Val)otherF1 = parse_str("_fun (x) x + 1")->interp();
        CHECK(f1->value_equals(otherF1));
        CHECK(!f1->value_equals(NEW(FunVal)("y", parse_str("x + 1"))));
        CHECK(!f1->value_equals(NEW(FunVal)("x", parse_str("x + 2"))));
        CHECK(!f1->value_equals(NEW(NumVal)(10)));
    }

    SECTION("The add_to method") {
        CHECK_THROWS_WITH(f1->add_to(NEW(NumVal)(2)), "add of non-number");
    }

    SECTION("The mult_by method") {
        CHECK_THROWS_WITH(f1->mult_by(NEW(NumVal)(2)), "mult of non-number");
    }

    SECTION("The to_string method") {
        CHECK(f1->to_string() == "(_fun (x) (x+1))");
    }

    SECTION("The to_expr method") {
        CHECK(f1->to_expr()->equals(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
    }

    SECTION("The call method") {
        CHECK(f1->call(NEW(NumVal)(10))->value_equals(NEW(NumVal)(11)));
    }
}