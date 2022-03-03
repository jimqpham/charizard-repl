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

TEST_CASE("Default test: Subst after replacement - should be equal") {
    CHECK ((new MultExpr(new VarExpr("x"), new NumExpr(7)))
                   ->subst("x", new VarExpr("y"))
                   ->equals(new MultExpr(new VarExpr("y"), new NumExpr(7))));
}

TEST_CASE("Expressions containing no matching variables should stay the same after subst") {
    CHECK(parse_str("10")->subst("x", new VarExpr("new"))->to_string() == "10");
    CHECK(parse_str("10*10")->subst("x", new VarExpr("new"))->to_string() == "(10*10)");
    CHECK(parse_str("10+10")->subst("x", new VarExpr("new"))->to_string() == "(10+10)");
    CHECK(parse_str("y")->subst("x", new VarExpr("new"))->to_string() == "y");
    CHECK(parse_str("_let y = y + 1 _in y")->subst("x", new VarExpr("new"))
                  ->to_string() == "(_let y=(y+1) _in y)");
    CHECK(parse_str("_true")->subst("x", new VarExpr("new"))->to_string() == "_true");
    CHECK(parse_str("y == 10")->subst("x", new VarExpr("new"))->to_string() == "(y==10)");
    CHECK(parse_str("_if _true _then y _else y + 1")->subst("x", new VarExpr("new"))
                  ->to_string() == "(_if _true _then y _else (y+1))");
    CHECK(parse_str(" f (y*2 + z)")->subst("x", new VarExpr("new"))
                  ->to_string() == "f(((y*2)+z))");
}

TEST_CASE("Expressions containing matching variables should change after subst") {

    // Basic expr
    // Expressions before subst
    CHECK(parse_str("10 * x")->subst("x", new VarExpr("changed"))
                  ->to_string() == "(10*changed)");
    CHECK(parse_str("10 + x")->subst("x", new VarExpr("changed"))
                  ->to_string() == "(10+changed)");
    CHECK(parse_str("10 == x")->subst("x", new VarExpr("changed"))
                  ->to_string() == "(10==changed)");
    CHECK(parse_str("x == 10")->subst("x", new VarExpr("changed"))
                  ->to_string() == "(changed==10)");

    // LetExpr
    // Case 1: subst does not go into the body because it tries to substitute the var in lhs
    CHECK(parse_str("_let x = 10 + x _in 10 + x")->subst("x", new VarExpr("changed"))
                  ->to_string() == "(_let x=(10+changed) _in (10+x))");
    // Case 2: subst goes into the body
    CHECK(parse_str("_let y = 10 + x _in 10 + x")->subst("x", new VarExpr("changed"))
                  ->to_string() == "(_let y=(10+changed) _in (10+changed))");

    // IfExpr
    CHECK(parse_str("_if x==y _then x _else y")->subst("x", new VarExpr("changed"))
                  ->to_string() == "(_if (changed==y) _then changed _else y)");


    // CallExpr
    CHECK(parse_str("f(x)")->subst("x", new VarExpr("y"))->to_string() == "f(y)");
    CHECK(parse_str("f(x)")->subst("f", new VarExpr("y"))->to_string() == "y(x)");

    // FunExpr
    // Not substitute x (subst not go into the body) because x is the formal arg
    CHECK(parse_str("_fun (x) (x + y)")->subst("x", new VarExpr("changed"))
                  ->to_string() == "(_fun (x) (x+y))");
    // Substitute x because x is not the formal arg
    CHECK(parse_str("_fun (y) (x + y)")->subst("x", new VarExpr("changed"))
                  ->to_string() == "(_fun (y) (changed+y))");

}
