#include "env.h"
#include "empty_env.h"


PTR(Env) Env::empty = NEW(EmptyEnv)();
