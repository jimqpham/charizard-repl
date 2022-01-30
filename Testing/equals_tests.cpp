#include "catch.h"
#include "../Expressions/num.h"
#include "../Expressions/add.h"
#include "../Expressions/mult.h"
#include "../Expressions/var.h"
#include "../Expressions/let.h"

TEST_CASE("Test Num equals") {
    SECTION("Num should be equal to Num of same value") {
        Num num = Num(10);
        Num anotherNum = Num(10);
        CHECK(num.equals(&anotherNum));
    }

    SECTION("Num should not be equal to Num of different value") {
        Num num = Num(10);
        Num anotherNum = Num(20);
        CHECK(!num.equals(&anotherNum));
    }

    SECTION("Num should not equal objects of other classes") {
        Num num = Num(0);
        Add sum = Add(&num, &num);
        CHECK(!num.equals(&sum));
        CHECK(!sum.equals(&num));
    }
}

TEST_CASE("Test Add equals") {

    SECTION("Add should be equal to Add of same components") {
        Num firstNum = Num(10);
        Num secondNum = Num(-10);
        Num thirdNum = Num(10);
        Num fourthNum = Num(-10);
        Add sum = Add(&firstNum, &secondNum);
        Add anotherSum = Add(&thirdNum, &fourthNum);
        CHECK(sum.equals(&anotherSum));
    }

    SECTION("Add should not be equal to Add of different components") {
        Num firstNum = Num(10);
        Num secondNum = Num(-10);
        Add firstSum = Add(&firstNum, &secondNum);
        Num thirdNum = Num(10);
        Num fourthNum = Num(-10);
        Add sum = Add(&firstNum, &firstSum);
        Add anotherSum = Add(&thirdNum, &fourthNum);
        CHECK(!sum.equals(&anotherSum));
    }

    SECTION("Add should not equal objects of other classes") {
        Num num = Num(4);
        Add sum = Add(&num, &num);
        Mult product = Mult(&num, &num);
        CHECK(!sum.equals(&product));
        CHECK(!product.equals(&sum));
    }
}

TEST_CASE("Test Mult equals") {

    SECTION("Mult should be equal to Mult of same components") {
        Num firstNum = Num(0);
        Num secondNum = Num(20);
        Add firstSum = Add(&firstNum, &secondNum);
        Num thirdNum = Num(0);
        Add secondSum = Add(&thirdNum, &secondNum);

        Mult product = Mult(&firstSum, &secondNum);
        Mult anotherProduct = Mult(&secondSum, &secondNum);
        CHECK(product.equals(&anotherProduct));
    }

    SECTION("Mult should not be equal to Mult of different components") {
        Num firstNum = Num(0);
        Num secondNum = Num(20);
        Add firstSum = Add(&firstNum, &secondNum);
        Num thirdNum = Num(-5);
        Add secondSum = Add(&thirdNum, &secondNum);

        Mult product = Mult(&firstSum, &secondNum);
        Mult anotherProduct = Mult(&secondSum, &secondNum);
        Mult anotherProduct2 = Mult(&firstSum, &firstSum);
        CHECK(!product.equals(&anotherProduct));
        CHECK(!product.equals(&anotherProduct2));
    }

    SECTION("Mult should not equal objects of other classes") {
        Num firstNum = Num(0);
        Num secondNum = Num(20);
        Mult product = Mult(&firstNum, &secondNum);
        CHECK(!product.equals(&firstNum));
    }
}

TEST_CASE("Test Var equals") {
    SECTION("Var should be equal to Var of same name") {
        Var var = Var("charizard");
        Var anotherVar = Var("charizard");
        CHECK(var.equals(&anotherVar));
    }

    SECTION("Var should not be equal to Var of different name") {
        Var var = Var("charizard");
        Var anotherVar = Var("bulbasaur");
        CHECK(!var.equals(&anotherVar));
    }

    SECTION("Var should not equal objects of other classes") {
        Var var = Var("charizard");
        Num num = Num(070);
        CHECK(!var.equals(&num));
        CHECK(!num.equals(&var));
    }
}

TEST_CASE("Test Let equals") {
    SECTION("Let should equal Let of same components") {
        Num num1 = Num(4);
        Num num2 = Num(5);
        Var var1 = Var("x");
        Add add1 = Add(&var1, &num1);
        CHECK((new Let(&var1, &num2, &add1))
                ->equals(new Let(&var1, &num2, &add1)));
    }

    SECTION("Let should not equal Let of different components") {
        Num num1 = Num(4);
        Num num2 = Num(5);
        Var var1 = Var("x");
        Var var2 = Var("y");
        Add add1 = Add(&var1, &num1);
        CHECK(!(new Let(&var1, &num2, &add1))
                      ->equals(new Let(&var2, &num2, &add1))); // difference in var (lhs)
        CHECK(!(new Let(&var1, &num1, &add1))
                ->equals(new Let(&var1, &num2, &add1))); //difference in rhs
        CHECK(!(new Let(&var1, &num1, &add1))
                ->equals(new Let(&var1, &num1, &num2))); //difference in body
    }

    SECTION("Let should not equal objects of other classes") {
        Num num1 = Num(4);
        Var var1 = Var("x");
        Add add1 = Add(&var1, &num1);
        CHECK(!(new Let(&var1, &num1, &add1))->equals( &add1));
    }
}



