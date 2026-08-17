#include "ConfigLoader.hpp"

namespace Infrastructure::Config {
Domain::Rules::GameRules load_rules()
{
    // TODO: open file "config.txt" and read rules
    return Domain::Rules::DEFAULT_RULES;
}
} // namespace Infrastructure::Config