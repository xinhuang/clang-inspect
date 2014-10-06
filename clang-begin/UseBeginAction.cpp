#include "UseBeginAction.h"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

namespace begin {
void UseBeginAction::run(const MatchFinder::MatchResult &result) {
  const auto *recordDecl = result.Nodes.getNodeAs<RecordDecl>("class");
  const auto *memberCall =
      result.Nodes.getNodeAs<CXXMemberCallExpr>("callExpr");
  outs() << "Found potential non-member begin/end transformation: "
         << recordDecl->getQualifiedNameAsString()
         << "::" << memberCall->getMethodDecl()->getNameAsString() << "\n";
}
}
