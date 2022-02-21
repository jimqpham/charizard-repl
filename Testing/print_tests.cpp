#include <sstream>
#include "catch.h"
#include "../Expressions/num_expr.h"
#include "../Expressions/add_expr.h"
#include "../Expressions/mult_expr.h"
#include "../Expressions/var_expr.h"
#include "../Expressions/let_expr.h"
#include "../Expressions/bool_expr.h"

TEST_CASE("Should print or pretty print expressions") {
    NumExpr num1 = NumExpr(3);
    NumExpr num2 = NumExpr(-2);
    VarExpr var1 = VarExpr("x");
    VarExpr var2 = VarExpr("y");
    BoolExpr tr = BoolExpr(true);
    BoolExpr fls = BoolExpr(false);

    AddExpr add1 = AddExpr(&num1, &num2); //"3 + -2"
    MultExpr mult1 = MultExpr(&num1, &num2); //"3 * -2"
    LetExpr let1 = LetExpr(&var1, &num1, new AddExpr(&var1, &num2)); //"_let x=3 _in (x+-2)" = 1

    SECTION("Should print NumExpr") {
        CHECK(strcmp(num1.to_string().c_str(), "3") == 0);
        CHECK(strcmp(num2.to_string().c_str(), "-2") == 0);
    }

    SECTION("Should print BoolExpr") {
        CHECK(strcmp(tr.to_string().c_str(), "_true") == 0);
        CHECK(strcmp(fls.to_string().c_str(), "_false") == 0);
    }

    SECTION("Should print VarExpr") {
        CHECK(strcmp(var1.to_string().c_str(), "x") == 0);
        CHECK(strcmp(var2.to_string().c_str(), "y") == 0);
    }

    SECTION("Should print AddExpr") {
        AddExpr add2 = AddExpr(&num1, &var1);
        AddExpr add3 = AddExpr(&num1, &add1);
        AddExpr add4 = AddExpr(&num1, &mult1);

        AddExpr add5 = AddExpr(&var1, &num1);
        AddExpr add6 = AddExpr(&var1, &var2);
        AddExpr add7 = AddExpr(&var1, &add1);
        AddExpr add8 = AddExpr(&var1, &mult1);

        AddExpr add9 = AddExpr(&add1, &num1);
        AddExpr add10 = AddExpr(&add1, &var1);
        AddExpr add11 = AddExpr(&add1, &add1);
        AddExpr add12 = AddExpr(&add1, &mult1);

        AddExpr add13 = AddExpr(&mult1, &num1);
        AddExpr add14 = AddExpr(&mult1, &var1);
        AddExpr add15 = AddExpr(&mult1, &add1);
        AddExpr add16 = AddExpr(&mult1, &mult1);

        CHECK(strcmp(add1.to_string().c_str(), "(3+-2)") == 0);
        CHECK(strcmp(add2.to_string().c_str(), "(3+x)") == 0);
        CHECK(strcmp(add3.to_string().c_str(), "(3+(3+-2))") == 0);
        CHECK(strcmp(add4.to_string().c_str(), "(3+(3*-2))") == 0);

        CHECK(strcmp(add5.to_string().c_str(), "(x+3)") == 0);
        CHECK(strcmp(add6.to_string().c_str(), "(x+y)") == 0);
        CHECK(strcmp(add7.to_string().c_str(), "(x+(3+-2))") == 0);
        CHECK(strcmp(add8.to_string().c_str(), "(x+(3*-2))") == 0);

        CHECK(strcmp(add9.to_string().c_str(), "((3+-2)+3)") == 0);
        CHECK(strcmp(add10.to_string().c_str(), "((3+-2)+x)") == 0);
        CHECK(strcmp(add11.to_string().c_str(), "((3+-2)+(3+-2))") == 0);
        CHECK(strcmp(add12.to_string().c_str(), "((3+-2)+(3*-2))") == 0);

        CHECK(strcmp(add13.to_string().c_str(), "((3*-2)+3)") == 0);
        CHECK(strcmp(add14.to_string().c_str(), "((3*-2)+x)") == 0);
        CHECK(strcmp(add15.to_string().c_str(), "((3*-2)+(3+-2))") == 0);
        CHECK(strcmp(add16.to_string().c_str(), "((3*-2)+(3*-2))") == 0);
    }

    SECTION("Should print MultExpr") {
        MultExpr mult2 = MultExpr(&num1, &var1);
        MultExpr mult3 = MultExpr(&num1, &add1);
        MultExpr mult4 = MultExpr(&num1, &mult1);

        MultExpr mult5 = MultExpr(&var1, &num1);
        MultExpr mult6 = MultExpr(&var1, &var2);
        MultExpr mult7 = MultExpr(&var1, &add1);
        MultExpr mult8 = MultExpr(&var1, &mult1);

        MultExpr mult9 = MultExpr(&add1, &num1);
        MultExpr mult10 = MultExpr(&add1, &var1);
        MultExpr mult11 = MultExpr(&add1, &add1);
        MultExpr mult12 = MultExpr(&add1, &mult1);

        MultExpr mult13 = MultExpr(&mult1, &num1);
        MultExpr mult14 = MultExpr(&mult1, &var1);
        MultExpr mult15 = MultExpr(&mult1, &add1);
        MultExpr mult16 = MultExpr(&mult1, &mult1);

        CHECK(strcmp(mult1.to_string().c_str(), "(3*-2)") == 0);
        CHECK(strcmp(mult2.to_string().c_str(), "(3*x)") == 0);
        CHECK(strcmp(mult3.to_string().c_str(), "(3*(3+-2))") == 0);
        CHECK(strcmp(mult4.to_string().c_str(), "(3*(3*-2))") == 0);

        CHECK(strcmp(mult5.to_string().c_str(), "(x*3)") == 0);
        CHECK(strcmp(mult6.to_string().c_str(), "(x*y)") == 0);
        CHECK(strcmp(mult7.to_string().c_str(), "(x*(3+-2))") == 0);
        CHECK(strcmp(mult8.to_string().c_str(), "(x*(3*-2))") == 0);

        CHECK(strcmp(mult9.to_string().c_str(), "((3+-2)*3)") == 0);
        CHECK(strcmp(mult10.to_string().c_str(), "((3+-2)*x)") == 0);
        CHECK(strcmp(mult11.to_string().c_str(), "((3+-2)*(3+-2))") == 0);
        CHECK(strcmp(mult12.to_string().c_str(), "((3+-2)*(3*-2))") == 0);

        CHECK(strcmp(mult13.to_string().c_str(), "((3*-2)*3)") == 0);
        CHECK(strcmp(mult14.to_string().c_str(), "((3*-2)*x)") == 0);
        CHECK(strcmp(mult15.to_string().c_str(), "((3*-2)*(3+-2))") == 0);
        CHECK(strcmp(mult16.to_string().c_str(), "((3*-2)*(3*-2))") == 0);
    }

    SECTION("Should print LetExpr") {
        // Nested lets - body is a LetExpr expr
        LetExpr let2 = LetExpr(&var1, &num2, &let1); // "_let x=-2 _in (_let x=3 _in (x+-2))"

        // Nested lets - rhs is a LetExpr expr
        LetExpr let3 = LetExpr(&var1, &let1, new AddExpr(&var1, &num2)); // "_let x=(_let x=3 _in (x+-2)) _in (x+-2)"

        // Nested lets - both body and rhs are LetExpr expr's
        LetExpr let4 = LetExpr(&var1, &let1, &let1);

        // LetExpr is lhs of a +/* expr
        AddExpr add2 = AddExpr(&let1, &num1);

        // LetExpr is rhs of a parenthesized +/* expr
        MultExpr mult2 = MultExpr(&num1, new AddExpr(&num1, &let1));

        // LetExpr is rhs of an unparenthesized +/* and would NOT have needed parentheses in the surrounding context
        MultExpr mult3 = MultExpr(&num1, &let1);

        // LetExpr is rhs of an unparenthesized +/* and would have needed parentheses in the surrounding context
        AddExpr add3 = AddExpr(&mult3, &num1);

        CHECK(strcmp(let1.to_string().c_str(), "(_let x=3 _in (x+-2))") == 0);
        CHECK(strcmp(let2.to_string().c_str(), "(_let x=-2 _in (_let x=3 _in (x+-2)))") == 0);
        CHECK(strcmp(let3.to_string().c_str(), "(_let x=(_let x=3 _in (x+-2)) _in (x+-2))") == 0);
        CHECK(strcmp(let4.to_string().c_str(), "(_let x=(_let x=3 _in (x+-2)) _in (_let x=3 _in (x+-2)))") == 0);
        CHECK(strcmp(add2.to_string().c_str(), "((_let x=3 _in (x+-2))+3)") == 0);
        CHECK(strcmp(mult2.to_string().c_str(), "(3*(3+(_let x=3 _in (x+-2))))") == 0);
        CHECK(strcmp(mult3.to_string().c_str(), "(3*(_let x=3 _in (x+-2)))") == 0);
        CHECK(strcmp(add3.to_string().c_str(), "((3*(_let x=3 _in (x+-2)))+3)") == 0);
    }

    SECTION("Should pretty print Number") {
        CHECK(strcmp(num1.to_string(true).c_str(), "3") == 0);
        CHECK(strcmp(num2.to_string(true).c_str(), "-2") == 0);
    }

    SECTION("Should pretty print VarExpr") {
        CHECK(strcmp(var1.to_string(true).c_str(), "x") == 0);
        CHECK(strcmp(var2.to_string(true).c_str(), "y") == 0);
    }

    SECTION("Should pretty print AddExpr") {
        AddExpr add2 = AddExpr(&num1, &var1);
        AddExpr add3 = AddExpr(&num1, &add1);
        AddExpr add4 = AddExpr(&num1, &mult1);

        AddExpr add5 = AddExpr(&var1, &num1);
        AddExpr add6 = AddExpr(&var1, &var2);
        AddExpr add7 = AddExpr(&var1, &add1);
        AddExpr add8 = AddExpr(&var1, &mult1);

        AddExpr add9 = AddExpr(&add1, &num1);
        AddExpr add10 = AddExpr(&add1, &var1);
        AddExpr add11 = AddExpr(&add1, &add1);
        AddExpr add12 = AddExpr(&add1, &mult1);

        AddExpr add13 = AddExpr(&mult1, &num1);
        AddExpr add14 = AddExpr(&mult1, &var1);
        AddExpr add15 = AddExpr(&mult1, &add1);
        AddExpr add16 = AddExpr(&mult1, &mult1);

        CHECK(strcmp(add1.to_string(true).c_str(), "3 + -2") == 0);
        CHECK(strcmp(add2.to_string(true).c_str(), "3 + x") == 0);
        CHECK(strcmp(add3.to_string(true).c_str(), "3 + 3 + -2") == 0);
        CHECK(strcmp(add4.to_string(true).c_str(), "3 + 3 * -2") == 0);

        CHECK(strcmp(add5.to_string(true).c_str(), "x + 3") == 0);
        CHECK(strcmp(add6.to_string(true).c_str(), "x + y") == 0);
        CHECK(strcmp(add7.to_string(true).c_str(), "x + 3 + -2") == 0);
        CHECK(strcmp(add8.to_string(true).c_str(), "x + 3 * -2") == 0);

        CHECK(strcmp(add9.to_string(true).c_str(), "(3 + -2) + 3") == 0);
        CHECK(strcmp(add10.to_string(true).c_str(), "(3 + -2) + x") == 0);
        CHECK(strcmp(add11.to_string(true).c_str(), "(3 + -2) + 3 + -2") == 0);
        CHECK(strcmp(add12.to_string(true).c_str(), "(3 + -2) + 3 * -2") == 0);

        CHECK(strcmp(add13.to_string(true).c_str(), "3 * -2 + 3") == 0);
        CHECK(strcmp(add14.to_string(true).c_str(), "3 * -2 + x") == 0);
        CHECK(strcmp(add15.to_string(true).c_str(), "3 * -2 + 3 + -2") == 0);
        CHECK(strcmp(add16.to_string(true).c_str(), "3 * -2 + 3 * -2") == 0);
    }

    SECTION("Should pretty print MultExpr") {
        MultExpr mult2 = MultExpr(&num1, &var1);
        MultExpr mult3 = MultExpr(&num1, &add1);
        MultExpr mult4 = MultExpr(&num1, &mult1);

        MultExpr mult5 = MultExpr(&var1, &num1);
        MultExpr mult6 = MultExpr(&var1, &var2);
        MultExpr mult7 = MultExpr(&var1, &add1);
        MultExpr mult8 = MultExpr(&var1, &mult1);

        MultExpr mult9 = MultExpr(&add1, &num1);
        MultExpr mult10 = MultExpr(&add1, &var1);
        MultExpr mult11 = MultExpr(&add1, &add1);
        MultExpr mult12 = MultExpr(&add1, &mult1);

        MultExpr mult13 = MultExpr(&mult1, &num1);
        MultExpr mult14 = MultExpr(&mult1, &var1);
        MultExpr mult15 = MultExpr(&mult1, &add1);
        MultExpr mult16 = MultExpr(&mult1, &mult1);

        CHECK(strcmp(mult1.to_string(true).c_str(), "3 * -2") == 0);
        CHECK(strcmp(mult2.to_string(true).c_str(), "3 * x") == 0);
        CHECK(strcmp(mult3.to_string(true).c_str(), "3 * (3 + -2)") == 0);
        CHECK(strcmp(mult4.to_string(true).c_str(), "3 * 3 * -2") == 0);

        CHECK(strcmp(mult5.to_string(true).c_str(), "x * 3") == 0);
        CHECK(strcmp(mult6.to_string(true).c_str(), "x * y") == 0);
        CHECK(strcmp(mult7.to_string(true).c_str(), "x * (3 + -2)") == 0);
        CHECK(strcmp(mult8.to_string(true).c_str(), "x * 3 * -2") == 0);

        CHECK(strcmp(mult9.to_string(true).c_str(), "(3 + -2) * 3") == 0);
        CHECK(strcmp(mult10.to_string(true).c_str(), "(3 + -2) * x") == 0);
        CHECK(strcmp(mult11.to_string(true).c_str(), "(3 + -2) * (3 + -2)") == 0);
        CHECK(strcmp(mult12.to_string(true).c_str(), "(3 + -2) * 3 * -2") == 0);

        CHECK(strcmp(mult13.to_string(true).c_str(), "(3 * -2) * 3") == 0);
        CHECK(strcmp(mult14.to_string(true).c_str(), "(3 * -2) * x") == 0);
        CHECK(strcmp(mult15.to_string(true).c_str(), "(3 * -2) * (3 + -2)") == 0);
        CHECK(strcmp(mult16.to_string(true).c_str(), "(3 * -2) * 3 * -2") == 0);
    }

    SECTION("Should pretty print LetExpr") {
        // Nested lets - body is a LetExpr expr
        LetExpr let2 = LetExpr(&var1, &num2, &let1); // "_let x=-2 _in (_let x=3 _in (x+-2))"

        // Nested lets - rhs is a LetExpr expr
        LetExpr let3 = LetExpr(&var1, &let1, new AddExpr(&var1, &num2)); // "_let x=(_let x=3 _in (x+-2)) _in (x+-2)"

        // Nested lets - both body and rhs are LetExpr expr's
        LetExpr let4 = LetExpr(&var1, &let1, &let1);

        // LetExpr is lhs of a +/* expr
        AddExpr add2 = AddExpr(&let1, &num1);

        // LetExpr is rhs of a parenthesized +/* expr
        MultExpr mult2 = MultExpr(&num1, new AddExpr(&num1, &let1));

        // LetExpr is rhs of an unparenthesized +/* and would NOT have needed parentheses in the surrounding context
        MultExpr mult3 = MultExpr(&num1, &let1);

        // LetExpr is rhs of an unparenthesized +/* and would have needed parentheses in the surrounding context
        AddExpr add3 = AddExpr(&mult3, &num1);

        CHECK(strcmp(let1.to_string(true).c_str(), "_let x = 3\n"
                                                   "_in  x + -2") == 0);
        CHECK(strcmp(let2.to_string(true).c_str(), "_let x = -2\n"
                                                   "_in  _let x = 3\n"
                                                   "     _in  x + -2") == 0);
        CHECK(strcmp(let3.to_string(true).c_str(), "_let x = _let x = 3\n"
                                                   "         _in  x + -2\n"
                                                   "_in  x + -2") == 0);
        CHECK(strcmp(let4.to_string(true).c_str(), "_let x = _let x = 3\n"
                                                   "         _in  x + -2\n"
                                                   "_in  _let x = 3\n"
                                                   "     _in  x + -2") == 0);
        CHECK(strcmp(add2.to_string(true).c_str(), "(_let x = 3\n"
                                                   " _in  x + -2) + 3") == 0);
        CHECK(strcmp(mult2.to_string(true).c_str(), "3 * (3 + _let x = 3\n"
                                                    "         _in  x + -2)") == 0);
        CHECK(strcmp(mult3.to_string(true).c_str(), "3 * _let x = 3\n"
                                                    "    _in  x + -2") == 0);
        CHECK(strcmp(add3.to_string(true).c_str(), "3 * (_let x = 3\n"
                                                   "     _in  x + -2) + 3") == 0);
    }
}

