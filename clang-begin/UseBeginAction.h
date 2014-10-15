#pragma once

#include "BeginMatchCallback.h"

#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Refactoring.h"

#include <string>

namespace begin {

class UseBeginAction : public clang::PluginASTAction {
  clang::ast_matchers::MatchFinder finder;
  BeginMatchCallback matchCallback;

public:
  UseBeginAction(clang::tooling::Replacements &replacement,
                 bool printLocations);
  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &ci, llvm::StringRef);

  bool ParseArgs(const clang::CompilerInstance &ci,
                 const std::vector<std::string> &args);

  bool BeginSourceFileAction(clang::CompilerInstance &ci, llvm::StringRef);
};

}
