#include "UseBeginAction.h"

#include "UseBeginMatcher.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace begin {

void IncludeFinder::FileChanged(clang::SourceLocation loc,
                                FileChangeReason reason,
                                clang::SrcMgr::CharacteristicKind /*fileType*/,
                                clang::FileID /*prevFid*/) {
  if (reason != EnterFile) 
    return;

  if (const auto* entry = sourceMgr.getFileEntryForID(sourceMgr.getFileID(loc))) {
    llvm::outs() << ">>> Depends on " << entry->getName() << " <<<\n";
  }
}

std::unique_ptr<clang::ASTConsumer>
UseBeginAction::CreateASTConsumer(clang::CompilerInstance &, llvm::StringRef) {
  clang::ast_matchers::MatchFinder finder;
  finder.addMatcher(createMemberBeginEndMatcher(), &matchCallback);

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
  llvm::outs() << "BeginSourceFileAction\n";
  auto &pp = ci.getPreprocessor();
  pp.addPPCallbacks(
      std::unique_ptr<IncludeFinder>(new IncludeFinder(ci.getSourceManager())));
  return true;
}

}
