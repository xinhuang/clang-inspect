#include "UseBeginMatcher.h"
#include "UseBeginAction.h"

using namespace begin;

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Signals.h"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

static llvm::cl::OptionCategory ClangBeginCategory("clang-begin options");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

static cl::extrahelp MoreHelp(
    "\nclang-begin will examine all `begin' and `end' member function calls,"
    "\nreplace them with non-member `begin' and `end' if applicable."
    "\nBy applicable it means the class should have member function `begin'"
    "\n`end' defined.");

int main(int argc, const char **argv) {
  llvm::sys::PrintStackTraceOnErrorSignal();
  CommonOptionsParser OptionsParser(argc, argv, ClangBeginCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());
  UseBeginAction ubAction;
  MatchFinder finder;
  finder.addMatcher(createMemberBeginEndMatcher(), &ubAction);
  return Tool.run(newFrontendActionFactory(&finder).get());
}
