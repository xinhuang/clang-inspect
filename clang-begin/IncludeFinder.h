#pragma once

#include "clang/Lex/PPCallbacks.h"

#include <string>

namespace begin {

class IncludeFinder : public clang::PPCallbacks {
  const std::string Target = "iterator";
  clang::SourceLocation Loc;

public:
  void InclusionDirective(clang::SourceLocation HashLoc,
                          const clang::Token &IncludeTok,
                          clang::StringRef FileName, bool IsAngled,
                          clang::CharSourceRange FileNameRange,
                          const clang::FileEntry *File,
                          clang::StringRef SearchPath,
                          clang::StringRef RelativePath,
                          const clang::Module *Imported) final;

  const clang::SourceLocation &getLocation() const { return Loc; }
};

}
