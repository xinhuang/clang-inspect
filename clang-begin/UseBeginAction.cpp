#include "UseBeginAction.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Tooling/Refactoring.h"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

#include <string>

namespace begin {
void UseBeginAction::run(const MatchFinder::MatchResult &result) {
  const auto *recordDecl = result.Nodes.getNodeAs<RecordDecl>("class");
  const auto *memberCall =
      result.Nodes.getNodeAs<CXXMemberCallExpr>("callExpr");
  // FIXME: use the original class name, qualified name might has namespace
  const auto& className = recordDecl->getQualifiedNameAsString();
  const auto& methodName = memberCall->getMethodDecl()->getNameAsString();
  outs() << "Found potential non-member begin/end transformation: " << className
         << "::" << methodName << "\n";
  const auto& sourceMgr = *result.SourceManager;
  const auto startLoc = sourceMgr.getSpellingLoc(memberCall->getLocStart());
  const auto endLoc = sourceMgr.getSpellingLoc(memberCall->getLocEnd());

  std::string replacememnt("std::" + methodName + "(" + className + ")");
  auto length = sourceMgr.getFileOffset(endLoc) - sourceMgr.getFileOffset(startLoc);
  tooling::Replacement repl(sourceMgr, startLoc, length, replacememnt);

  // TODO: apply replacement to rewriter.
}
}
