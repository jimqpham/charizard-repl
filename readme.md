# MSDScript Documentation
# 1. Code Example Notation
Code examples in this document follow some conventions listed below:
Command line instructions (i.e. commands that are executed directly in the terminal) starts with the `$` symbol.
Instructions that are executed in the MSDScript interactive shell starts with the `>` symbol.
Example:
```
$ ./msdscript --help		/* Should be executed directly from the Terminal */
> (1 * 2) 					/* Should be executed from MSDScript interactive shell */
```
When a code example has two lines that both start with `>`, the first line should be the user input in the MSDScript interactive shell and the second line is the result returned by the program. For example:
```
> 1 == 2			/* User input */
> _false			/* Result returned */
```

# 2. Build Instruction for Command Line Program
To build the MSDScript command-line program, follow the step-by-step instructions below:

 1. Download the compressed file `msd-script-charizard.zip` and extract the file using an unarchiver of your choice.
 2. Navigate into the newly-created folder from the compressed file and run this command from the command line:
```
$ make
```
3. The build process should take from 10 - 20 seconds.
4. To make sure the program is build successfully, run this command from the command line: 
```
$./msdcript --test
```
You should receive a log message saying that all tests have passed.

# 3. Command Line Application
The MSDScript command line application is fundamentally an interactive shell that takes in an input, evaluate, and print out the result depending on the operation mode.
You can select the operation mode of the shell by providing the corresponding parameter when launching the program. The three operation modes are as follows:

**3.1. Evaluation mode**

To launch the program in interpretation mode, run this command from the command line:
```
$ ./msdscript --step
```
Note that you can achieve the same goal with using the `--interp` parameter in place of `--step`, but this is not encouraged since doing so will probably throw a segfault when evaluating a complicated expression. The `--step` is always the preferred choice.
In this mode, MSDScript opens up an interactive shell that takes in an expression from the user input. The user has to press `Ctrl + D` to signify the end of the expression. At that point, MSDScript starts parsing the user input, evaluates, and prints out that value. See the Specifications section to learn more about the expression syntax that msdscript supports.
Example:
```
> 1 + 2
> 3
```
```
> _let x = 3 _in (x + 2)
> 5
```

**3.2. Print mode**

To launch the program in print mode, run this command:
```
$ ./msdscript --print
```
In this mode, msdscript opens up an interactive shell that takes in an expression, evaluates, and prints out the expression with minimal whitespaces and explicit parentheses around each sub-expression.
Example:
```
> 1 + 3
> (1+3)
```
```
> _let f = _fun(x)(x+1) _in f(1)
> (_let f=(_fun (x) (x+1)) _in f(1))
```

**3.3. Pretty print mode**

To launch the program in pretty print mode, run this command:
```
$ ./msdscript --pretty-print
```
This mode is fundamentally similar to the print mode. The only difference is that it prints out a more neatly-spaced version of the expression (sometimes spanning across multiple lines), and eliminates unnecessary parentheses.
Example:
```
> (( 1 +    3 ))
> 1 + 3
```
```
> _let f = _fun(x)(x+1) _in f(1)
> _let f = _fun (x)
             x + 1
  _in  f(1)
```

**3.4. Help mode**
You can get a brief instruction for those modes of operation mentioned above by using the `--help` parameter.
```
$ ./msdscript --help
```

# 4. MSDSript Language Specifications
The scripting language that MSDScript uses is simple. It supports the usage of variables, several data types, and operations listed below.

## 4.1. Variables
MSDScript supports the use of variables to store values. See the **Data Types** section below to learn more about which types of values are permitted.
Variable names in MSDScript should only be consisted of alphabetic characters – no digits or special symbols are allowed. Variable names are case-sensitive. Examples of acceptable variable names: `x`, `arrayLength`, `Remainder`.
To assign a value to a variable, see the **Operations > Variable assignment** section below. When evaluating, MSDScript will throw an error if the variable has not been assigned a value.

## 4.2. Data Types

### 4.2.1. Numeric Values
MSDScript can work with round numbers, both negative and positive. Numeric values should only be written using digits, and in case of negative numbers, a minus sign `-` right before the first digit with no whitespace in between. It does not support floating point numbers, numbers written using scientific notation, thousand separators, etc.
Example: `12`, `2022`, `-1000000`

### 4.2.2. Boolean Values
MSDScript has support for boolean values. The notation for boolean values is either `_true` or `_false`.

### 4.2.3. Functions
Functions are considered values in MSDScript (similar to how functions are treated in JavaScript). This means you can assign a function value to a variable and pass it around just as you do a numeric/boolean value.
A function value starts with the keyword `_fun` and have this general syntax:
```
_fun (<formal_argument>) (<function_body>)
```
For example, a MSDScript function written like this `_fun (x) (x + 1)` can be re-written using mathematical notation: 

$f(x) = x + 1$ or $f:x \mapsto (x + 1)$

At the moment MSDScript functions can only take a single argument as the input (the formal argument), hence it is more limited in scope than a mathematical function. In other words, it is not possible to translate a function with a complex input (for example, $f(x + y) = 1$) into a MSDScript function.
If you try to interpret a stand-alone function value, MSDScript can only return a generic `[function]` message. Function values should make more sense when used in combination with other data types. See the **Operations > Function Call** section to learn more about how to call a function value in MSDScript.
Example in interpretation mode:
```
> _fun (length) (length * 2)
> [function]
```

## 4.3. Operations

### 4.3.1. Basic Arithmetic
MSDScript supports addition, multiplication, and the use of parentheses. The notations for those are similar to those of the common scripting languages: addition `+`, multiplication `*`, and parentheses `()`. The order of operations is parentheses > multiplication > addition (a reduced version of PEMDAS).
Note that you can only perform arithmetic operations on numeric values. Doing so on boolean values/functions will throw a runtime error.
Example in interpretation mode:
- Addition
```
> 1 + 1
> 2
```
- Multiplication
```
> 2 * 3
> 6
```
- Parentheses
```
> (1 + 5)
> 6
```
- Combination of all of the above
```
> (1 + 2) * 3
> 9
```
### 4.3.2. Equality Checking
Equality check (notation: `==`) is possible in MSDScript. Inequality check (e.g. greater than, less than, not equal) is not supported.
Equality check can be performed on every data type. Two numeric values are equal if they represent the same amount. Two boolean values are equal if they are both `_true` or `_false` and not equal if otherwise. Two function values are equal if they have the same formal argument and the function body.
Equality check can also be performed in a cross data type manner. This type of check always returns `_false`.
Example:
- Numeric values
```
> -5 == -5
> _true
```
```
> 1 == 1000
> _false
```
- Boolean values
```
> _false == _false
> _true
```
```
> _true == _false
> _false
```
- Functions
```
> _fun (y) (y * y) == _fun (y) (y * y)
> _true
```
```
> _fun (y) (y * y) == _fun (z) (z * z)
> _false
```
```
> _fun (y) (y * y) == _fun (y) (y + 2)
> _false
```
### 4.3.3. Conditional statement
The general syntax for a conditional statement in MSDScript is:
```
_if (<condition>) _then (<expression to interp if condition is true>)
				  _else (<expression to interp if condition is false>)
```
The condition should be an expression that can be evaluated to a boolean value, which means it can be a `_true`, `_false` value or an equality check `5 == 5`. If the condition evaluates to `true`, the program continues to interpret the expression in the `_then` branch; otherwise, it goes into the `_else` branch.
Example:
```
> _if (_true) _then (3 + 4) _else (3 * 4)
> 7
```
```
> _if (3 * 4 == 13) _then 3 + 4 _else 3 * 4
> 12
```
### 4.3.4. Variable assignment
The general syntax for a variable assignment statement is:
```
_let <variable_name> = <expression> _in <body expression>
```
Looking at the syntax, you can see that variable assignment always has to be accompanied by a body expression. That is, when you bind a value to a variable, you also have to provide the expression which is using that variable. You cannot separately assign a value to a variable and save that variable somewhere for future use.
A variable can be assigned a numeric value, a boolean value or a function value.
The `_let` statement can also be nested as well to assign values to multiple variables.
Example in interpretation mode:
```
> _let x = 3 _in x * x
> 9
```
```
> _let x = 1 + 2 _in (_let y = 5 _in x + y)
> 8
```
An important note is that variable values go with the most recent binding. If the same variable has been assigned multiple values, it forgets about all previous values and only retains the most recent value it is bound to. For example:
```
> _let x = 1 _in (_let x = 5 _in x + 1)
> 6
```

### 4.3.5. Function call
The general syntax for a function call is:
```
<variable>(<expression>)
```
This is similar to how we denote a function call mathematically: if a function definition is $f(x) = x^2$ or $f:x \mapsto x^2$ then we can call the function like this: $f(3) = 9$.
Fundamentally, a function call can be understood as substituting all the occurences of the formal argument in the function body with the value in the function call. For example, expanding on our example above with the function `f = _fun (x) (x*x)`, we have that:
- The equivalent mathematical notation: $f(x) = x^2$
- Formal argument: `x`
- Function body: `x*x`
- Function call example: `f(3)`  – this function call looks for every occurence of `x` in the function body and replace them with the numeric value `3`. Hence we get back the value 9 (or `3 * 3`) from this function call.

The function call operation can only be performed on a variable containing a function value. If the call operation is performed on a non-function value, e.g. a numeric or boolean value, a runtime error is thrown. For example, `_let f = _true _in f(1)` should throw an error because the variable `f` is assigned a non-function value.
The call operation is most of the time accompanied by a `_let` statement. The `_let` statement assigns a function value to a variable before we can do the call operation on that variable.
Example:
```
> _let f = _fun (x) (x*x) _in f(3)
> 9
```
# 5. API Information
Aside from using MSDScript as a standalone command line program, you can also use it as a library to supplement your C++ code.
## 5.1. Library Usage Instructions
To use it as a library, you will need two files from the package: the archived library file `msdscript.a` and the header file `msdscript.h` specifying method signatures to use.
In your source code, you need to first import the header file to be able to use the MSDScript methods:
```
#include "msdscript.h"
```
And when compiling, add the archived library file to the command line together with other source code files, for example:
```
c++ msdscript.a main.cpp helper.cpp
```
## 5.2. Library Methods' API Information
The MSDScript as a library provides methods corresponding to its command line operation modes. All methods are defined as static methods of the `MSDApi` class for better code organization. As such, when calling a method, you need to prefix the method name with `MSDApi::`.
All methods take in an input string which follows the MSDScript specifications above, then return the result also in the form of a string.
The description of each method is as follows:
### 5.2.1. The `interp()` method

Method signature:
```
std::string MSDApi::interp(std::string input);
```
This method takes in an input, parses it into an expression, and return the value of that expression.
Example:
```
std::cout << MSDApi::interp("1 + 1"); // Should print out 2
```
### 5.2.2. The `to_string()` method
Method signature:
```
std::string MSDApi::to_string(std::string input);
```
Same as the `interp()` method, but this method does not return the value of the expression. Instead, it returns a string representation of the expression with extra parentheses and minimal whitespaces.
Example:
```
std::cout << MSDApi::to_string("(1 + 3) * (2 + 4)");
// Should print out ((1+3)*(2+4))
```
### 5.2.3. The `to_pretty_string()` method
Method signature:
```
std::string MSDApi::to_pretty_string(std::string input);
```
Same as the `to_string()` method, but this method returns a string representation of the expression with minimal parentheses and extra whitespaces for better readability. 
Example:
```
std::cout << MSDApi::to_pretty_string("(1+3) * (((((2 + 4)))))");
// Should print out (1 + 3) * (2 + 4)
```


