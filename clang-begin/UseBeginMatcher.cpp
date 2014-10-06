#include "UseBeginMatcher.h"

using namespace clang::ast_matchers;

namespace begin {
const char *ClassID = "class";
clang::ast_matchers::StatementMatcher createMemberBeginEndMatcher() {
  return memberCallExpr(
             hasDeclaration(methodDecl(ofClass(
                 recordDecl(allOf(hasMethod(hasName("begin")),
                                  hasMethod(hasName("end")))).bind("class")))),
             callee(methodDecl(anyOf(hasName("begin"), hasName("end")))))
      .bind("callExpr");
}
}

