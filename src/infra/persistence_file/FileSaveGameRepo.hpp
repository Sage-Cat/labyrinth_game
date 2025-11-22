#pragma once
#include "app/persistence/ISaveGameRepo.hpp"

namespace Infrastructure::PersistenceFile {
class FileSaveGameRepo final : public Application::Persistence::ISaveGameRepo {
public:
    bool save(const Domain::Core::GameState &, const std::string &path) override;
    bool load(Domain::Core::GameState &, const std::string &path) override;
};
} // namespace Infrastructure::PersistenceFile
