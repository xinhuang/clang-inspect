#include "UseBeginAction.h"

#include "UseBeginMatcher.h"
#include "IncludeFinder.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <string>

namespace begin {

UseBeginAction::UseBeginAction(clang::tooling::Replacements &replacement,
                               bool printLocations)
    : matchCallback(replacement, printLocations) {
    finder.addMatcher(createMemberBeginEndMatcher(), &matchCallback);
  }

std::unique_ptr<clang::ASTConsumer>
UseBeginAction::CreateASTConsumer(clang::CompilerInstance &, llvm::StringRef) {
  return finder.newASTConsumer();
}

bool UseBeginAction::ParseArgs(const clang::CompilerInstance &,
                               const std::vector<std::string> &args) {
  llvm::outs() << "ParseArgs\n";
  for (auto i = 0UL; i < args.size(); ++i)
    llvm::outs() << "Received args: " << args[i] << "\n";
  return true;
}

bool UseBeginAction::BeginSourceFileAction(clang::CompilerInstance &ci,
                                                llvm::StringRef) {
  auto &pp = ci.getPreprocessor();
  pp.addPPCallbacks(
      std::unique_ptr<IncludeFinder>(new IncludeFinder()));
  return true;
}

}
