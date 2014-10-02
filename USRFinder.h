#pragma once

namespace clang {
  class NamedDecl;
  class ASTContext;
  class SourceLocation;
}

namespace inspect {

const clang::NamedDecl *getNamedDeclAt(clang::ASTContext &context,
                                 const clang::SourceLocation &point);
}

