#pragma once

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace begin {
class UseBeginAction : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
  virtual void
  run(const clang::ast_matchers::MatchFinder::MatchResult &result) final;
};

}
