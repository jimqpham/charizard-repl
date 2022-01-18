#include "catch.h"
#include "../Expressions/num.h"
#include "../Expressions/add.h"
#include "../Expressions/mult.h"
#include "../Expressions/var.h"

TEST_CASE("Num should be equal to Num of same value") {
    Num num = Num(10);
    Num anotherNum = Num(10);
    CHECK(num.equals(&anotherNum));
}

TEST_CASE("Num should not be equal to Num of different value") {
    Num num = Num(10);
    Num anotherNum = Num(20);
    CHECK(!num.equals(&anotherNum));
}

TEST_CASE("Add should be equal to Add of same components") {
    Num firstNum = Num (10);
    Num secondNum = Num (-10);
    Num thirdNum = Num (10);
    Num fourthNum = Num (-10);
    Add sum = Add(&firstNum, &secondNum);
    Add anotherSum = Add(&thirdNum, &fourthNum);
    CHECK(sum.equals(&anotherSum));
}

TEST_CASE("Add should not be equal to Add of different components") {
    Num firstNum = Num (10);
    Num secondNum =  Num (-10);
    Add firstSum = Add(&firstNum, &secondNum); 
    Num thirdNum = Num (10);
    Num fourthNum = Num (-10);
    Add sum = Add(&firstNum, &firstSum);
    Add anotherSum = Add(&thirdNum, &fourthNum);
    CHECK(!sum.equals(&anotherSum));
}

TEST_CASE("Mult should be equal to Mult of same components") {
    Num firstNum = Num(0);
    Num secondNum = Num(20);
    Add firstSum = Add(&firstNum, &secondNum);
    Num thirdNum = Num(0);
    Add secondSum = Add(&thirdNum, &secondNum);

    Mult product = Mult(&firstSum, &secondNum);
    Mult anotherProduct = Mult(&secondSum, &secondNum);
    CHECK(product.equals(&anotherProduct));
}

TEST_CASE("Mult should not be equal to Mult of different components") {
    Num firstNum = Num(0);
    Num secondNum = Num(20);
    Add firstSum = Add(&firstNum, &secondNum);
    Num thirdNum = Num(-5);
    Add secondSum = Add(&thirdNum, &secondNum);

    Mult product = Mult(&firstSum, &secondNum);
    Mult anotherProduct = Mult(&secondSum, &secondNum);
    CHECK(!product.equals(&anotherProduct));
}

TEST_CASE("Var should be equal to Var of same name") {
    Var var = Var("charizard");
    Var anotherVar = Var("charizard");
    CHECK(var.equals(&anotherVar));
}

TEST_CASE("Var should not be equal to Var of different name") {
    Var var = Var("charizard");
    Var anotherVar = Var("bulbasaur");
    CHECK(!var.equals(&anotherVar));
}

TEST_CASE("Num should not equal Add") {
    Num num = Num(0);
    Add sum = Add(&num, &num);
    CHECK(!num.equals(&sum));
}

TEST_CASE("Num should not equal Mult") {
    Num num = Num(1);
    Mult product = Mult(&num, &num);
    CHECK(!num.equals(&product));
}

TEST_CASE("Add should not equal Mult") {
    Num num = Num(4);
    Add sum = Add(&num, &num);
    Mult product = Mult(&num, &num);
    CHECK(!sum.equals(&product));
}

TEST_CASE("Var should not equal Num") {
    Var var = Var("charizard");
    Num num = Num(070);
    CHECK(!var.equals(&num));
}

TEST_CASE("Var should not equal Add") {
    Num num = Num(20);
    Var var = Var("charizard");
    Add sum = Add(&num, &num);
    CHECK(!var.equals(&sum));
}

TEST_CASE("Var should not equal Mult") {
    Num num = Num(20);
    Var var = Var("charizard");
    Mult product = Mult(&num, &num);
    CHECK(!var.equals(&product));
}





