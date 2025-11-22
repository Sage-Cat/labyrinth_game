#include "infra/log/Logger.hpp"
#include <cstdlib>

int main()
{
    Infrastructure::Log::Logger log{};
    log.info("hello");
    log.error("oops");
    log.debug("dbg");

    // TODO: proper check

    return EXIT_SUCCESS;
}
