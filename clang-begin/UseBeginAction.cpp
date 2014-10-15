#include "UseBeginAction.h"

#include "UseBeginMatcher.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <string>

void BackSlashToSlash(std::string& s) {
  for (size_t i = 0; i < s.length(); ++i) {
    if (s[i] == '\\')
      s[i] = '/';
  }
}

bool HasSuffix(const std::string &s, const std::string &suffix) {
  if (s.length() < suffix.length())
    return false;
  std::string guess(s.begin() + s.length() - suffix.length(), s.end());
  return guess == suffix;
}

namespace begin {

void IncludeFinder::FileChanged(clang::SourceLocation loc,
                                FileChangeReason reason,
                                clang::SrcMgr::CharacteristicKind /*fileType*/,
                                clang::FileID /*prevFid*/) {
  if (reason != EnterFile) 
    return;

  if (const auto* entry = sourceMgr.getFileEntryForID(sourceMgr.getFileID(loc))) {
    std::string path = entry->getName();
    BackSlashToSlash(path);
    if (HasSuffix(path, TargetSuffix)) {
      llvm::outs() << ">>> Found " << TargetSuffix << " at " << path << "\n";
    }
  }
}

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
      std::unique_ptr<IncludeFinder>(new IncludeFinder(ci.getSourceManager())));
  return true;
}

}
