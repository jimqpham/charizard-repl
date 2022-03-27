#include <string>
#include "env.h"

class EmptyEnv : public Env {
    PTR(Val) lookup(std::string find_name) override;
};
