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

class IncludeFinder : public clang::PPCallbacks {
  const std::string TargetName = "iterator";
  clang::SourceManager& sourceMgr;

public:
  IncludeFinder(clang::SourceManager &sourceMgr) : sourceMgr(sourceMgr) {}

  void FileChanged(clang::SourceLocation loc, FileChangeReason reason,
                   clang::SrcMgr::CharacteristicKind fileType,
                   clang::FileID prevFid) override;
};

class UseBeginAction : public clang::PluginASTAction {
  BeginMatchCallback matchCallback;

public:
  UseBeginAction(clang::tooling::Replacements &replacement, bool printLocations)
      : matchCallback(replacement, printLocations) {}

  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &ci, llvm::StringRef);

  bool ParseArgs(const clang::CompilerInstance &ci,
                 const std::vector<std::string> &args);

  bool BeginSourceFileAction(clang::CompilerInstance &ci, llvm::StringRef);
};

}
