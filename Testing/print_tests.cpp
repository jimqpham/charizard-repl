#include <sstream>
#include "catch.h"
#include "../Expressions/num.h"
#include "../Expressions/add.h"
#include "../Expressions/mult.h"
#include "../Expressions/var.h"
#include "../Expressions/let.h"

TEST_CASE("Should print or pretty print expressions") {
    Num num1 = Num(30);
    Num num2 = Num(-12);
    Var var1 = Var("x");
    Var var2 = Var("y");
    
    Add add1 = Add(&num1, &num2); //"30 + -12"
    Mult mult1 = Mult(&num1, &num2); //"30 * -12"
    
    Add add2 = Add(&num1, &var1);
    Add add3 = Add(&num1, &add1);
    Add add4 = Add(&num1, &mult1);

    Add add5 = Add(&var1, &num1);
    Add add6 = Add(&var1, &var2);
    Add add7 = Add(&var1, &add1);
    Add add8 = Add(&var1, &mult1);

    Add add9 = Add(&add1, &num1);
    Add add10 = Add(&add1, &var1);
    Add add11 = Add(&add1, &add1);
    Add add12 = Add(&add1, &mult1);

    Add add13 = Add(&mult1, &num1);
    Add add14 = Add(&mult1, &var1);
    Add add15 = Add(&mult1, &add1);
    Add add16 = Add(&mult1, &mult1);

    Mult mult2 = Mult(&num1, &var1);
    Mult mult3 = Mult(&num1, &add1);
    Mult mult4 = Mult(&num1, &mult1);

    Mult mult5 = Mult(&var1, &num1);
    Mult mult6 = Mult(&var1, &var2);
    Mult mult7 = Mult(&var1, &add1);
    Mult mult8 = Mult(&var1, &mult1);

    Mult mult9 = Mult(&add1, &num1);
    Mult mult10 = Mult(&add1, &var1);
    Mult mult11 = Mult(&add1, &add1);
    Mult mult12 = Mult(&add1, &mult1);

    Mult mult13 = Mult(&mult1, &num1);
    Mult mult14 = Mult(&mult1, &var1);
    Mult mult15 = Mult(&mult1, &add1);
    Mult mult16 = Mult(&mult1, &mult1);

    SECTION("Should print Number") {
        CHECK(strcmp(num1.to_string().c_str(), "30") == 0);
        CHECK(strcmp(num2.to_string().c_str(), "-12") == 0);
    }

    SECTION("Should print Var") {
        CHECK(strcmp(var1.to_string().c_str(), "x") == 0);
        CHECK(strcmp(var2.to_string().c_str(), "y") == 0);
    }

    SECTION("Should print Add") {
        CHECK(strcmp(add1.to_string().c_str(), "(30+-12)") == 0);
        CHECK(strcmp(add2.to_string().c_str(), "(30+x)") == 0);
        CHECK(strcmp(add3.to_string().c_str(), "(30+(30+-12))") == 0);
        CHECK(strcmp(add4.to_string().c_str(), "(30+(30*-12))") == 0);

        CHECK(strcmp(add5.to_string().c_str(), "(x+30)") == 0);
        CHECK(strcmp(add6.to_string().c_str(), "(x+y)") == 0);
        CHECK(strcmp(add7.to_string().c_str(), "(x+(30+-12))") == 0);
        CHECK(strcmp(add8.to_string().c_str(), "(x+(30*-12))") == 0);

        CHECK(strcmp(add9.to_string().c_str(), "((30+-12)+30)") == 0);
        CHECK(strcmp(add10.to_string().c_str(), "((30+-12)+x)") == 0);
        CHECK(strcmp(add11.to_string().c_str(), "((30+-12)+(30+-12))") == 0);
        CHECK(strcmp(add12.to_string().c_str(), "((30+-12)+(30*-12))") == 0);

        CHECK(strcmp(add13.to_string().c_str(), "((30*-12)+30)") == 0);
        CHECK(strcmp(add14.to_string().c_str(), "((30*-12)+x)") == 0);
        CHECK(strcmp(add15.to_string().c_str(), "((30*-12)+(30+-12))") == 0);
        CHECK(strcmp(add16.to_string().c_str(), "((30*-12)+(30*-12))") == 0);
    }

    SECTION("Should print Mult") {
        CHECK(strcmp(mult1.to_string().c_str(), "(30*-12)") == 0);
        CHECK(strcmp(mult2.to_string().c_str(), "(30*x)") == 0);
        CHECK(strcmp(mult3.to_string().c_str(), "(30*(30+-12))") == 0);
        CHECK(strcmp(mult4.to_string().c_str(), "(30*(30*-12))") == 0);

        CHECK(strcmp(mult5.to_string().c_str(), "(x*30)") == 0);
        CHECK(strcmp(mult6.to_string().c_str(), "(x*y)") == 0);
        CHECK(strcmp(mult7.to_string().c_str(), "(x*(30+-12))") == 0);
        CHECK(strcmp(mult8.to_string().c_str(), "(x*(30*-12))") == 0);

        CHECK(strcmp(mult9.to_string().c_str(), "((30+-12)*30)") == 0);
        CHECK(strcmp(mult10.to_string().c_str(), "((30+-12)*x)") == 0);
        CHECK(strcmp(mult11.to_string().c_str(), "((30+-12)*(30+-12))") == 0);
        CHECK(strcmp(mult12.to_string().c_str(), "((30+-12)*(30*-12))") == 0);

        CHECK(strcmp(mult13.to_string().c_str(), "((30*-12)*30)") == 0);
        CHECK(strcmp(mult14.to_string().c_str(), "((30*-12)*x)") == 0);
        CHECK(strcmp(mult15.to_string().c_str(), "((30*-12)*(30+-12))") == 0);
        CHECK(strcmp(mult16.to_string().c_str(), "((30*-12)*(30*-12))") == 0);
    }

    SECTION("Should pretty print Number") {
        CHECK(strcmp(num1.to_string(true).c_str(), "30") == 0);
        CHECK(strcmp(num2.to_string(true).c_str(), "-12") == 0);
    }

    SECTION("Should pretty print Var") {
        CHECK(strcmp(var1.to_string(true).c_str(), "x") == 0);
        CHECK(strcmp(var2.to_string(true).c_str(), "y") == 0);
    }
    
    SECTION("Should pretty print Add") {
        CHECK(strcmp(add1.to_string(true).c_str(), "30 + -12") == 0);
        CHECK(strcmp(add2.to_string(true).c_str(), "30 + x") == 0);
        CHECK(strcmp(add3.to_string(true).c_str(), "30 + 30 + -12") == 0);
        CHECK(strcmp(add4.to_string(true).c_str(), "30 + 30 * -12") == 0);

        CHECK(strcmp(add5.to_string(true).c_str(), "x + 30") == 0);
        CHECK(strcmp(add6.to_string(true).c_str(), "x + y") == 0);
        CHECK(strcmp(add7.to_string(true).c_str(), "x + 30 + -12") == 0);
        CHECK(strcmp(add8.to_string(true).c_str(), "x + 30 * -12") == 0);

        CHECK(strcmp(add9.to_string(true).c_str(), "(30 + -12) + 30") == 0);
        CHECK(strcmp(add10.to_string(true).c_str(), "(30 + -12) + x") == 0);
        CHECK(strcmp(add11.to_string(true).c_str(), "(30 + -12) + 30 + -12") == 0);
        CHECK(strcmp(add12.to_string(true).c_str(), "(30 + -12) + 30 * -12") == 0);

        CHECK(strcmp(add13.to_string(true).c_str(), "30 * -12 + 30") == 0);
        CHECK(strcmp(add14.to_string(true).c_str(), "30 * -12 + x") == 0);
        CHECK(strcmp(add15.to_string(true).c_str(), "30 * -12 + 30 + -12") == 0);
        CHECK(strcmp(add16.to_string(true).c_str(), "30 * -12 + 30 * -12") == 0);
    }

    SECTION("Should pretty print Mult") {
        CHECK(strcmp(mult1.to_string(true).c_str(), "30 * -12") == 0);
        CHECK(strcmp(mult2.to_string(true).c_str(), "30 * x") == 0);
        CHECK(strcmp(mult3.to_string(true).c_str(), "30 * (30 + -12)") == 0);
        CHECK(strcmp(mult4.to_string(true).c_str(), "30 * 30 * -12") == 0);

        CHECK(strcmp(mult5.to_string(true).c_str(), "x * 30") == 0);
        CHECK(strcmp(mult6.to_string(true).c_str(), "x * y") == 0);
        CHECK(strcmp(mult7.to_string(true).c_str(), "x * (30 + -12)") == 0);
        CHECK(strcmp(mult8.to_string(true).c_str(), "x * 30 * -12") == 0);

        CHECK(strcmp(mult9.to_string(true).c_str(), "(30 + -12) * 30") == 0);
        CHECK(strcmp(mult10.to_string(true).c_str(), "(30 + -12) * x") == 0);
        CHECK(strcmp(mult11.to_string(true).c_str(), "(30 + -12) * (30 + -12)") == 0);
        CHECK(strcmp(mult12.to_string(true).c_str(), "(30 + -12) * 30 * -12") == 0);

        CHECK(strcmp(mult13.to_string(true).c_str(), "(30 * -12) * 30") == 0);
        CHECK(strcmp(mult14.to_string(true).c_str(), "(30 * -12) * x") == 0);
        CHECK(strcmp(mult15.to_string(true).c_str(), "(30 * -12) * (30 + -12)") == 0);
        CHECK(strcmp(mult16.to_string(true).c_str(), "(30 * -12) * 30 * -12") == 0);
    }

    SECTION("Test print Let") {
        Expr* subLet = new Let(
                        new Var("x"),
                        new Num(7),
                        new Add(
                                new Var("x"),
                                new Num(10)
                                    )
                                );
        Let* letOne = new Let(
                new Var("var"),
                new Num(12),
                subLet
                );
        Expr* letTwo = new Let(
                new Var("y"),
                subLet,
                letOne
                );

        std::stringstream out ("");
        letTwo->pretty_print(out);
        std::cout << out.str();
    }
}

