#include "api.h"
#include "../Expressions/expr.h"
#include "../CmdLines/parse.h"
#include "../Utils/step.h"
#include "../Vals/val.h"

std::string MSDApi::interp(std::string input) {
    PTR(Expr) parsed_expr = parse_str(input);
    return Step::interp_by_steps(parsed_expr)->to_string();
}

std::string MSDApi::to_string(std::string input) {
    PTR(Expr) parsed_expr = parse_str(input);
    return parsed_expr->to_string();
}

std::string MSDApi::to_pretty_string(std::string input) {
    PTR(Expr) parsed_expr = parse_str(input);
    return parsed_expr->to_string(true);
}
