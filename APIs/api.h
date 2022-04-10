#include "string"

class MSDApi {
public:
    static std::string interp(std::string input);

    static std::string to_string(std::string input);

    static std::string to_pretty_string(std::string input);
};