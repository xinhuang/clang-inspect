#include "IncludeFinder.h"

namespace begin {
void IncludeFinder::InclusionDirective(
    clang::SourceLocation HashLoc, const clang::Token & /*IncludeTok*/,
    clang::StringRef FileName, bool /*IsAngled*/,
    clang::CharSourceRange /*FileNameRange*/, const clang::FileEntry * /*File*/,
    clang::StringRef /*SearchPath*/, clang::StringRef /*RelativePath*/,
    const clang::Module * /*Imported*/) {
  if (FileName.str() == Target) {
    llvm::outs() << ">>> Include file " << FileName << "\n";
    Loc = HashLoc;
  }
}
}
