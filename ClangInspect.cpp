#include "USRFindingAction.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/CommandLineSourceLoc.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Lex/Lexer.h"
#include "clang/Parse/Parser.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
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
  errs() << "RefactoringTool::ctor\n";
  tooling::RefactoringTool tool(option.getCompilations(), files);

  errs() << "inspect::USRFindingAction::ctor\n";
  inspect::USRFindingAction usrAction(SymbolOffset);
  errs() << "tool.run(usrAction)\n";
  tool.run(tooling::newFrontendActionFactory(&usrAction).get());

  errs() << "Found name: " << usrAction.getUSRSpelling() << "\n";

  return 0;
}

