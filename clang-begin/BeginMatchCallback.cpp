#include "BeginMatchCallback.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Tooling/Refactoring.h"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

#include <string>

namespace begin {

std::string getStringInRange(const SourceManager& SM, const Stmt& ST) {
  const auto& LocStart = ST.getLocStart();
  const auto& LocEnd = ST.getLocEnd();
  const char* CharStart = SM.getCharacterData(LocStart);
  const char* CharEnd = SM.getCharacterData(LocEnd);
  return std::string(CharStart, CharEnd + 1);
}

void BeginMatchCallback::run(const MatchFinder::MatchResult &result) {
  const auto *memberCall =
      result.Nodes.getNodeAs<CXXMemberCallExpr>("callExpr");

  const auto& sourceMgr = *result.SourceManager;
  std::string refName;
  if (const auto* memberExpr = dyn_cast<MemberExpr>(memberCall->getCallee())) {
    refName = getStringInRange(sourceMgr, *memberExpr->getBase());
  }
  // FIXME: use the original class name, qualified name might has namespace
  const auto& methodName = memberCall->getMethodDecl()->getNameAsString();
  const auto startLoc = sourceMgr.getSpellingLoc(memberCall->getLocStart());
  const auto endLoc = sourceMgr.getSpellingLoc(memberCall->getLocEnd());

  if (printLocations) {
    outs() << sourceMgr.getFilename(startLoc) << ":"
           << sourceMgr.getSpellingLineNumber(startLoc) << ":"
           << sourceMgr.getSpellingColumnNumber(startLoc) << "\n";
  }

  // FIXME: add #include <iterator>
  std::string replacememnt("std::" + methodName + "(" + refName);
  auto length = sourceMgr.getFileOffset(endLoc) - sourceMgr.getFileOffset(startLoc);
  replaces.insert(tooling::Replacement(sourceMgr, startLoc, length, replacememnt));
}
}
