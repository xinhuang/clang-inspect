#pragma once

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/Refactoring.h"

namespace begin {
class UseBeginAction : public clang::ast_matchers::MatchFinder::MatchCallback {
  clang::tooling::Replacements &replaces;
  bool printLocations;

public:
  UseBeginAction(clang::tooling::Replacements &replacement, bool printLocations)
      : replaces(replacement), printLocations(printLocations) {}

  virtual void
  run(const clang::ast_matchers::MatchFinder::MatchResult &result) final;
};
}
