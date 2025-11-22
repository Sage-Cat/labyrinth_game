#include "domain/core/GameState.hpp"
#include "infra/persistence_file/FileSaveGameRepo.hpp"
#include <cassert>
#include <cstdlib>

int main()
{
    Infrastructure::PersistenceFile::FileSaveGameRepo repo{};
    Domain::Core::GameState state{};
    assert(repo.save(state, "tmp.sav") == false);
    assert(repo.load(state, "tmp.sav") == false);

    // TODO: proper check

    return EXIT_SUCCESS;
}
