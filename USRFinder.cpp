#include "USRFinder.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"

class NamedDeclFindingASTVisitor
    : public clang::RecursiveASTVisitor<NamedDeclFindingASTVisitor> {
  const clang::SourceManager &sourceMgr;
  const clang::SourceLocation point;
  const clang::NamedDecl *result;

public:
  NamedDeclFindingASTVisitor(const clang::SourceManager &sourceMgr,
                             const clang::SourceLocation &point)
      : sourceMgr(sourceMgr), point(point), result(nullptr) {}

  bool VisitNamedDecl(const clang::NamedDecl* decl) {
    return setResult(decl, decl->getLocation(), decl->getNameAsString().length());
  }

  const clang::NamedDecl *getNamedDecl() const { return result; }

private:
  bool setResult(const clang::NamedDecl *decl, clang::SourceLocation begin,
                 unsigned int offset) {
    if (offset == 0 || !isPointWithin(begin, begin.getLocWithOffset(offset)))
      return true;
    result = decl;
    return false;
  }

  bool isPointWithin(const clang::SourceLocation& begin, const clang::SourceLocation& end) const {
    return begin == point || end == point ||
           (sourceMgr.isBeforeInTranslationUnit(begin, point) &&
            sourceMgr.isBeforeInTranslationUnit(point, end));
  }
};

namespace inspect {

const clang::NamedDecl *getNamedDeclAt(clang::ASTContext &context,
                                       const clang::SourceLocation &point) {
  const auto& sourceMgr = context.getSourceManager();
  const auto& fileId = sourceMgr.getFileID(point);

  NamedDeclFindingASTVisitor visitor(sourceMgr, point);

  auto decls = context.getTranslationUnitDecl()->decls();
  for(auto& decl : decls) {
    const auto& declFileLoc = decl->getLocation();
    const auto& declFileId = sourceMgr.getFileID(declFileLoc);
    if (declFileId != fileId) {
      continue;
    }
    visitor.TraverseDecl(decl);
    if (const auto* result = visitor.getNamedDecl()) {
      return result;
    }
  }

  return nullptr;
}
}
