#include "BeginMatchCallback.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Tooling/Refactoring.h"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

#include <string>

namespace begin {

void BeginMatchCallback::run(const MatchFinder::MatchResult &result) {
  const auto *recordDecl = result.Nodes.getNodeAs<RecordDecl>("class");
  const auto *memberCall =
      result.Nodes.getNodeAs<CXXMemberCallExpr>("callExpr");
  // FIXME: use the original class name, qualified name might has namespace
  const auto& className = recordDecl->getQualifiedNameAsString();
  const auto& methodName = memberCall->getMethodDecl()->getNameAsString();
  const auto& sourceMgr = *result.SourceManager;
  const auto startLoc = sourceMgr.getSpellingLoc(memberCall->getLocStart());
  const auto endLoc = sourceMgr.getSpellingLoc(memberCall->getLocEnd());

  if (printLocations) {
    outs() << sourceMgr.getFilename(startLoc) << ":"
           << sourceMgr.getSpellingLineNumber(startLoc) << ":"
           << sourceMgr.getSpellingColumnNumber(startLoc) << "\n";
  }

  // FIXME: add #include <iterator>
  std::string replacememnt("std::" + methodName + "(" + className);
  auto length = sourceMgr.getFileOffset(endLoc) - sourceMgr.getFileOffset(startLoc);
  replaces.insert(tooling::Replacement(sourceMgr, startLoc, length, replacememnt));
}
}
