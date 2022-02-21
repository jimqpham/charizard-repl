#include "catch.h"
#include "../Vals/num_val.h"
#include "../Vals/bool_val.h"
#include "../Expressions/num_expr.h"

TEST_CASE("Test BoolVal methods") {

    BoolVal *tr = new BoolVal(true);
    BoolVal *otherTr = new BoolVal(true);
    BoolVal *fls = new BoolVal(false);
    NumVal *two = new NumVal(2);

    SECTION("The equals method") {
        CHECK(tr->equals(otherTr));
        CHECK(!tr->equals(fls));
        CHECK(!tr->equals(two));
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
        CHECK(tr->to_expr() == nullptr);
    }
}