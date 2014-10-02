#include "USRFindingAction.h"

#include <cassert>
#include <utility>
#include <sstream>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "llvm/Support/raw_ostream.h"

#include "Util.h"
#include "USRFinder.h"

using namespace inspect;

// FIXME: add test
clang::SourceLocation gotoLineAndColumn(const clang::SourceManager &sourceMgr,
                                        const std::tuple<int, int> &offset) {
  const unsigned int line = std::get<0>(offset);
  const unsigned int column = std::get<1>(offset);
  auto point = sourceMgr.getLocForStartOfFile(sourceMgr.getMainFileID());

  clang::FullSourceLoc fullLoc(point, sourceMgr);
  do {
    llvm::errs() << "goto " << fullLoc.getSpellingLineNumber() << ":"
           << fullLoc.getSpellingColumnNumber() << "\n";
    if (fullLoc.getSpellingLineNumber() == line)
      break;
    // FIXME: the step can be improved
    point = point.getLocWithOffset(1);
    fullLoc = clang::FullSourceLoc{point, sourceMgr};
  } while (true);

  assert(fullLoc.getSpellingLineNumber() == line);
  return point.getLocWithOffset(column - fullLoc.getSpellingColumnNumber());
}

class NamedDeclFindingConsumer : public clang::ASTConsumer {
  const std::tuple<int, int> offset;
  std::string* spellingName = nullptr;
  std::string* typeInfo = nullptr;

public:
  NamedDeclFindingConsumer(const std::tuple<int, int> &offset,
                           std::string *spellingName, std::string *typeInfo)
      : offset(offset), spellingName(spellingName), typeInfo(typeInfo) {}

  void HandleTranslationUnit(clang::ASTContext &context) final {
    llvm::errs() << "HandleTranslationUnit>\n";
    const auto& sourceMgr = context.getSourceManager();
    const auto& point = gotoLineAndColumn(sourceMgr, offset);
    if (!point.isValid()) {
      return;
    }
    const auto* foundDecl = getNamedDeclAt(context, point);
    if (foundDecl == nullptr) {
      clang::FullSourceLoc fullLoc(point, sourceMgr);
      llvm::errs() << "clang-inspect: could not find symbol at "
                   << sourceMgr.getFilename(point) << ":"
                   << fullLoc.getSpellingLineNumber() << ":"
                   << fullLoc.getSpellingColumnNumber() << ":"
                   << " (offset " << to_string(offset) << ").\n";
    }

    *spellingName = foundDecl->getNameAsString();

    // get definition location: file:<line>:<column>
    
    // get type info
  }

private:
  static std::string to_string(const std::tuple<int, int>& t) {
    std::ostringstream iss;
    iss << std::get<0>(t) << ":" << std::get<1>(t);
    return iss.str();
  }
};

namespace inspect {

USRFindingAction::USRFindingAction(const std::string &offsetString) {
  assert(contains(offsetString, ':'));

  auto parts = split(offsetString, ':');
  assert(parts.size() == 2);
  symbolOffset =
      std::make_tuple<int, int>(std::stoi(parts[0]), std::stoi(parts[1]));
}

std::unique_ptr<clang::ASTConsumer> USRFindingAction::newASTConsumer() {
  auto consumer = std::unique_ptr<clang::ASTConsumer>(
      new NamedDeclFindingConsumer(symbolOffset, &spellingName, &typeInfo));
  return consumer;
}

}
