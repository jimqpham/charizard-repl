#include "catch.h"
#include "../Expressions/num_expr.h"
#include "../Expressions/add_expr.h"
#include "../Expressions/mult_expr.h"
#include "../Expressions/var_expr.h"
#include "../Expressions/let_expr.h"
#include "../Expressions/bool_expr.h"
#include "../Expressions/equal_expr.h"
#include "../Expressions/if_expr.h"

TEST_CASE("Test NumExpr value_equals") {
    SECTION("NumExpr should be equal to NumExpr of same value") {
        NumExpr num = NumExpr(10);
        NumExpr anotherNum = NumExpr(10);
        CHECK(num.equals(&anotherNum));
    }

    SECTION("NumExpr should not be equal to NumExpr of different value") {
        NumExpr num = NumExpr(10);
        NumExpr anotherNum = NumExpr(20);
        CHECK(!num.equals(&anotherNum));
    }

    SECTION("NumExpr should not equal objects of other classes") {
        NumExpr num = NumExpr(0);
        AddExpr sum = AddExpr(&num, &num);
        CHECK(!num.equals(&sum));
        CHECK(!sum.equals(&num));
    }
}

TEST_CASE("Test BoolExpr value_equals") {
    BoolExpr tr = BoolExpr(true);
    BoolExpr otherTr = BoolExpr(10);
    BoolExpr fls = BoolExpr(false);
    BoolExpr otherFls = BoolExpr(fls);
    NumExpr num = NumExpr(0);

    SECTION("BoolExpr should be equal to BoolExpr of same value") {
        CHECK(tr.equals(&otherTr));
        CHECK(fls.equals(&otherFls));
    }

    SECTION("BoolExpr should not be equal to BoolExpr of different value") {
        CHECK(!tr.equals(&fls));
    }

    SECTION("BoolExpr should not equal objects of other classes") {
        CHECK(!fls.equals(&num));
        CHECK(!tr.equals(&num));
    }
}

TEST_CASE("Test AddExpr value_equals") {

    SECTION("AddExpr should be equal to AddExpr of same components") {
        NumExpr firstNum = NumExpr(10);
        NumExpr secondNum = NumExpr(-10);
        NumExpr thirdNum = NumExpr(10);
        NumExpr fourthNum = NumExpr(-10);
        AddExpr sum = AddExpr(&firstNum, &secondNum);
        AddExpr anotherSum = AddExpr(&thirdNum, &fourthNum);
        CHECK(sum.equals(&anotherSum));
    }

    SECTION("AddExpr should not be equal to AddExpr of different components") {
        NumExpr firstNum = NumExpr(10);
        NumExpr secondNum = NumExpr(-10);
        AddExpr firstSum = AddExpr(&firstNum, &secondNum);
        NumExpr thirdNum = NumExpr(10);
        NumExpr fourthNum = NumExpr(-10);
        AddExpr sum = AddExpr(&firstNum, &firstSum);
        AddExpr anotherSum = AddExpr(&thirdNum, &fourthNum);
        CHECK(!sum.equals(&anotherSum));
    }

    SECTION("AddExpr should not equal objects of other classes") {
        NumExpr num = NumExpr(4);
        AddExpr sum = AddExpr(&num, &num);
        MultExpr product = MultExpr(&num, &num);
        CHECK(!sum.equals(&product));
        CHECK(!product.equals(&sum));
    }
}

TEST_CASE("Test MultExpr value_equals") {

    SECTION("MultExpr should be equal to MultExpr of same components") {
        NumExpr firstNum = NumExpr(0);
        NumExpr secondNum = NumExpr(20);
        AddExpr firstSum = AddExpr(&firstNum, &secondNum);
        NumExpr thirdNum = NumExpr(0);
        AddExpr secondSum = AddExpr(&thirdNum, &secondNum);

        MultExpr product = MultExpr(&firstSum, &secondNum);
        MultExpr anotherProduct = MultExpr(&secondSum, &secondNum);
        CHECK(product.equals(&anotherProduct));
    }

    SECTION("MultExpr should not be equal to MultExpr of different components") {
        NumExpr firstNum = NumExpr(0);
        NumExpr secondNum = NumExpr(20);
        AddExpr firstSum = AddExpr(&firstNum, &secondNum);
        NumExpr thirdNum = NumExpr(-5);
        AddExpr secondSum = AddExpr(&thirdNum, &secondNum);

        MultExpr product = MultExpr(&firstSum, &secondNum);
        MultExpr anotherProduct = MultExpr(&secondSum, &secondNum);
        MultExpr anotherProduct2 = MultExpr(&firstSum, &firstSum);
        CHECK(!product.equals(&anotherProduct));
        CHECK(!product.equals(&anotherProduct2));
    }

    SECTION("MultExpr should not equal objects of other classes") {
        NumExpr firstNum = NumExpr(0);
        NumExpr secondNum = NumExpr(20);
        MultExpr product = MultExpr(&firstNum, &secondNum);
        CHECK(!product.equals(&firstNum));
    }
}

TEST_CASE("Test EqualExpr value_equals") {
    NumExpr zero = NumExpr(0);
    NumExpr twenty = NumExpr(20);
    NumExpr otherZero = NumExpr(0);
    NumExpr otherTwenty = NumExpr(20);

    SECTION("EqualExpr should be equal to EqualExpr of same components") {
        EqualExpr eq1 = EqualExpr(&zero, &twenty);
        EqualExpr eq2 = EqualExpr(&otherZero, &otherTwenty);
        CHECK(eq1.equals(&eq2));
    }

    SECTION("MultExpr should not be equal to MultExpr of different components") {
        EqualExpr eq1 = EqualExpr(&zero, &twenty);
        EqualExpr eq2 = EqualExpr(&otherTwenty, &otherZero);
        EqualExpr eq3 = EqualExpr(&twenty, &twenty);
        EqualExpr eq4 = EqualExpr(&zero, &zero);

        CHECK(!eq1.equals(&eq2));
        CHECK(!eq1.equals(&eq3));
        CHECK(!eq1.equals(&eq4));
    }

    SECTION("MultExpr should not equal objects of other classes") {
        EqualExpr eq1 = EqualExpr(&zero, &twenty);
        CHECK(!eq1.equals(&zero));
    }
}

TEST_CASE("Test VarExpr value_equals") {
    SECTION("VarExpr should be equal to VarExpr of same name") {
        VarExpr var = VarExpr("charizard");
        VarExpr anotherVar = VarExpr("charizard");
        CHECK(var.equals(&anotherVar));
    }

    SECTION("VarExpr should not be equal to VarExpr of different name") {
        VarExpr var = VarExpr("charizard");
        VarExpr anotherVar = VarExpr("bulbasaur");
        CHECK(!var.equals(&anotherVar));
    }

    SECTION("VarExpr should not equal objects of other classes") {
        VarExpr var = VarExpr("charizard");
        NumExpr num = NumExpr(070);
        CHECK(!var.equals(&num));
        CHECK(!num.equals(&var));
    }
}

TEST_CASE("Test LetExpr value_equals") {
    SECTION("LetExpr should equal LetExpr of same components") {
        NumExpr num1 = NumExpr(4);
        NumExpr num2 = NumExpr(5);
        VarExpr var1 = VarExpr("x");
        AddExpr add1 = AddExpr(&var1, &num1);
        CHECK((new LetExpr(&var1, &num2, &add1))
                      ->equals(new LetExpr(&var1, &num2, &add1)));
    }

    SECTION("LetExpr should not equal LetExpr of different components") {
        NumExpr num1 = NumExpr(4);
        NumExpr num2 = NumExpr(5);
        VarExpr var1 = VarExpr("x");
        VarExpr var2 = VarExpr("y");
        AddExpr add1 = AddExpr(&var1, &num1);
        CHECK(!(new LetExpr(&var1, &num2, &add1))
                ->equals(new LetExpr(&var2, &num2, &add1))); // difference in var (lhs)
        CHECK(!(new LetExpr(&var1, &num1, &add1))
                ->equals(new LetExpr(&var1, &num2, &add1))); //difference in rhs
        CHECK(!(new LetExpr(&var1, &num1, &add1))
                ->equals(new LetExpr(&var1, &num1, &num2))); //difference in body
    }

    SECTION("LetExpr should not equal objects of other classes") {
        NumExpr num1 = NumExpr(4);
        VarExpr var1 = VarExpr("x");
        AddExpr add1 = AddExpr(&var1, &num1);
        CHECK(!(new LetExpr(&var1, &num1, &add1))->equals(&add1));
    }
}

TEST_CASE("Test IfExpr value_equals") {
    SECTION("IfExpr should equal IfExpr of same components") {
        NumExpr four = NumExpr(4);
        NumExpr five = NumExpr(5);
        BoolExpr tr = BoolExpr(true);
        AddExpr add1 = AddExpr(&four, &five);
        CHECK((new IfExpr(&tr, &add1, &four))
                      ->equals(new IfExpr(new BoolExpr(true), &add1, new NumExpr(4))));
    }

    SECTION("LetExpr should not equal LetExpr of different components") {
        NumExpr num1 = NumExpr(4);
        NumExpr num2 = NumExpr(5);
        BoolExpr tr = BoolExpr(true);
        BoolExpr fls = BoolExpr(false);
        AddExpr add1 = AddExpr(&tr, &num1);
        CHECK(!(new IfExpr(&tr, &num2, &add1))
                ->equals(new IfExpr(&fls, &num2, &add1))); // difference in var (lhs)
        CHECK(!(new IfExpr(&tr, &num1, &add1))
                ->equals(new IfExpr(&tr, &num2, &add1))); //difference in rhs
        CHECK(!(new IfExpr(&tr, &num1, &add1))
                ->equals(new IfExpr(&tr, &num1, &num2))); //difference in body
    }

    SECTION("LetExpr should not equal objects of other classes") {
        NumExpr num1 = NumExpr(4);
        VarExpr var1 = VarExpr("x");
        AddExpr add1 = AddExpr(&var1, &num1);
        CHECK(!(new IfExpr(&var1, &num1, &add1))->equals(&add1));
    }
}



