#include "USRFindingAction.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

using namespace llvm;

#include "Util.h"

cl::OptionCategory ClangInspectCategory("Clang-inspect options");

static cl::opt<std::string> SymbolOffset(
    "offset",
    cl::desc("Locates the symbol by offset as opposed by <line>:<column>"),
    cl::cat(ClangInspectCategory));

#define CLANG_INSPECT_VERSION "0.0.1"

static void PrintVersion() {
  outs() << "clang-inspect version: " << CLANG_INSPECT_VERSION << "\n";
}

static char InspectUsage[] = "";

using namespace clang;

int main(int argc, const char **argv) {
  cl::SetVersionPrinter(PrintVersion);
  tooling::CommonOptionsParser option(argc, argv, ClangInspectCategory, InspectUsage);

  if (!inspect::contains((std::string)SymbolOffset, ':')) {
    errs() << "Symbol offset should be <line>:<column>" << "\n";
    return 1;
  }

  auto files = option.getSourcePathList();
  tooling::RefactoringTool tool(option.getCompilations(), files);

  inspect::USRFindingAction usrAction(SymbolOffset);
  tool.run(tooling::newFrontendActionFactory(&usrAction).get());

  errs() << "Name: " << usrAction.getUSRSpelling() << "\n";
  errs() << "Type: " << usrAction.getUSRType() << "\n";
  errs() << "Location: " << usrAction.getDefinitionLocation() << "\n";

  return 0;
}

