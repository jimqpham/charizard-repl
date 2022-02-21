#include "catch.h"
#include "../Vals/num_val.h"
#include "../Expressions/num_expr.h"

TEST_CASE("Test NumVal methods") {

    NumVal *ten = new NumVal(10);
    NumVal *otherTen = new NumVal(10);
    NumVal *two = new NumVal(2);
    NumVal *negTwo = new NumVal(-2);
    NumVal *five = new NumVal(5);
    NumVal *eight = new NumVal(8);
    NumVal *twelve = new NumVal(12);
    NumVal *nullVal = nullptr;

    SECTION("The equals method") {
        CHECK(ten->equals(otherTen));
        CHECK(!ten->equals(eight));
        CHECK(!ten->equals(nullVal));
    }

    SECTION("The add_to method") {
        CHECK(ten->equals(two->add_to(eight)));
        CHECK(ten->equals(negTwo->add_to(twelve)));
        CHECK_THROWS_WITH(two->add_to(nullVal), "add of non-number");
    }

    SECTION("The mult_by method") {
        CHECK((two->mult_by(five))->equals(ten));
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
}