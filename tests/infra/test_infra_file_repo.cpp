#include <cstdlib>
#include <cassert>
#include "infra/persistence_file/FileSaveGameRepo.hpp"
#include "domain/core/GameState.hpp"

int main()
{
    Infrastructure::PersistenceFile::FileSaveGameRepo repo{};
    Domain::Core::GameState state{};
    assert(repo.save(state, "tmp.sav") == false); 
    assert(repo.load(state, "tmp.sav") == false); 

    // TODO: proper check

    return EXIT_SUCCESS;
}
