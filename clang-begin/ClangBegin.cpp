#include "UseBeginActionFactory.h"

using namespace begin;

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Signals.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

static llvm::cl::OptionCategory ClangBeginCategory("clang-begin options");

static cl::opt<bool> Inplace("i", cl::desc("Overwrite edited <file>s."),
                             cl::cat(ClangBeginCategory));
static cl::opt<bool> PrintLocations(
    "pl", cl::desc("Print the locations affected to stderr."),
    cl::cat(ClangBeginCategory));

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

static cl::extrahelp MoreHelp(
    "\nclang-begin will examine all `begin' and `end' member function calls,"
    "\nreplace them with non-member `begin' and `end' if applicable."
    "\nBy applicable it means the class should have member function `begin'"
    "\n`end' defined.");

int main(int argc, const char **argv) {
  llvm::sys::PrintStackTraceOnErrorSignal();
  CommonOptionsParser options(argc, argv, ClangBeginCategory);
  RefactoringTool tool(options.getCompilations(), options.getSourcePathList());

  UseBeginActionFactory ubFactory(tool.getReplacements(), PrintLocations);

  if (Inplace){
    return tool.runAndSave(&ubFactory);
  } else {
    auto result = tool.run(&ubFactory);
    
    LangOptions defaultLangOptions;
    IntrusiveRefCntPtr<DiagnosticOptions> diagOpts = new DiagnosticOptions();
    TextDiagnosticPrinter diagPrinter(errs(), &*diagOpts);
    DiagnosticsEngine diagnostics(
        IntrusiveRefCntPtr<DiagnosticIDs>(new DiagnosticIDs), &*diagOpts,
        &diagPrinter, false);
    auto& fileMgr = tool.getFiles();
    SourceManager sourceMgr(diagnostics, fileMgr);
    Rewriter rewriter(sourceMgr, defaultLangOptions);

    tool.applyAllReplacements(rewriter);
    for (const auto &file : options.getSourcePathList()) {
      const auto *entry = fileMgr.getFile(file);
      auto id = sourceMgr.translateFile(entry);
      rewriter.getEditBuffer(id).write(outs());
    }

    return result;
  }
}
