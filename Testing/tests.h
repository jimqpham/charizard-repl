#include "catch.h"
#include "../Expressions/num.h"
#include "../Expressions/add.h"
#include "../Expressions/mult.h"
#include "../Expressions/var.h"

TEST_CASE("Num should be equal to Num of same value") {
    Num *num = new Num(10);
    Num *anotherNum = new Num(10);
    CHECK(num->equals(anotherNum));
}

TEST_CASE("Num should not be equal to Num of different value") {
    Num *num = new Num(10);
    Num *anotherNum = new Num(20);
    CHECK(!num->equals(anotherNum));
}

TEST_CASE("Add should be equal to Add of same components") {
    Add *sum = new Add(new Num(10), new Num(-20));
    Add *anotherSum = new Add(new Num(10), new Num(-20));
    CHECK(sum->equals(anotherSum));
}

TEST_CASE("Add should not be equal to Add of different components") {
    Add *sum = new Add(new Num(10), new Num(-20));
    Add *anotherSum = new Add(new Num(-10), new Num(20));
    CHECK(!sum->equals(anotherSum));
}

TEST_CASE("Mult should be equal to Mult of same components") {
    Mult *product = new Mult(new Num(0), new Num(30));
    Mult *anotherProduct = new Mult(new Num(0), new Num(30));
    CHECK(product->equals(anotherProduct));
}

TEST_CASE("Mult should not be equal to Mult of different components") {
    Mult *product = new Mult(new Num(0), new Num(30));
    Mult *anotherProduct = new Mult(new Num(30), new Num(0));
    CHECK(!product->equals(anotherProduct));
}

TEST_CASE("Var should be equal to Var of same name") {
    Var *var = new Var("charizard");
    Var *anotherVar = new Var("charizard");
    CHECK(var->equals(anotherVar));
}

TEST_CASE("Var should not be equal to Var of different name") {
    Var *var = new Var("charizard");
    Var *anotherVar = new Var("bulbasaur");
    CHECK(!var->equals(anotherVar));
}

TEST_CASE("Num should not equal Add") {
    Num *num = new Num(0);
    Add *sum = new Add(new Num(30), new Num(0));
    CHECK(!num->equals(sum));
}

TEST_CASE("Num should not equal Mult") {
    Num *num = new Num(0);
    Mult *product = new Mult(new Num(30), new Num(0));
    CHECK(!num->equals(product));
}

TEST_CASE("Add should not equal Mult") {
    Add *sum = new Add(new Num(30), new Num(0));
    Mult *product = new Mult(new Num(30), new Num(0));
    CHECK(!sum->equals(product));
}

TEST_CASE("Var should not equal Num") {
    Var *var = new Var("charizard");
    Num *num = new Num(070);
    CHECK(!var->equals(num));
}

TEST_CASE("Var should not equal Add") {
    Var *var = new Var("charizard");
    Add *sum = new Add(new Num(070), new Num(404));
    CHECK(!var->equals(sum));
}

TEST_CASE("Var should not equal Mult") {
    Var *var = new Var("charizard");
    Mult *product = new Mult(new Num(070), new Num(404));
    CHECK(!var->equals(product));
}





