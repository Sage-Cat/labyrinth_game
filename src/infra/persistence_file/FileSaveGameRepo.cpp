#include "infra/persistence_file/FileSaveGameRepo.hpp"
namespace Infrastructure::PersistenceFile
{
    bool FileSaveGameRepo::save(const Domain::Core::GameState &, const std::string &) {
        // TODO: save game state to file
        return false; 
    } 

    bool FileSaveGameRepo::load(Domain::Core::GameState &, const std::string &) {
        // TODO: load game state from file
        return false; 
    }       
}
