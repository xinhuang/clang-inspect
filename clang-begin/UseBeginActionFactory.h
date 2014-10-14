#include "UseBeginAction.h"

#include "clang/Tooling/Tooling.h"

namespace begin {

class UseBeginActionFactory : public clang::tooling::FrontendActionFactory {
  clang::tooling::Replacements& replaces;
  bool printLocations = false;

public:
  UseBeginActionFactory(clang::tooling::Replacements &replacements,
                        bool printLocations)
      : replaces(replacements), printLocations(printLocations) {}
  virtual clang::FrontendAction* create() final {
    return new UseBeginAction(replaces, printLocations);
  }
};

}
