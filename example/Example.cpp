#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace std;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

Rewriter rewriter;
int numFunctions = 0;

class ExampleVisitor : public RecursiveASTVisitor<ExampleVisitor> {
    ASTContext *astContext;

public:
    explicit ExampleVisitor(CompilerInstance *CI) 
        : astContext(&(CI->getASTContext())) {
        rewriter.setSourceMgr(astContext->getSourceManager(), astContext->getLangOpts());
    }

    virtual bool VisitFunctionDecl(FunctionDecl *func) final {
        numFunctions++;
        string funcName = func->getNameInfo().getName().getAsString();
        if (funcName == "do_math") {
            rewriter.ReplaceText(func->getLocation(), funcName.length(), "add5");
            errs() << "** Rewrote function def: " << funcName << "\n";
        }
        return true;
    }

    virtual bool VisitReturnStmt(ReturnStmt *ret) final {
        rewriter.ReplaceText(ret->getRetValue()->getLocStart(), 6, "val");
        errs() << "** Rewrote ReturnStmt\n";
        return true;
    }

    virtual bool VisitCallExpr(CallExpr *call) final {
        rewriter.ReplaceText(call->getLocStart(), 7, "add5");
        errs() << "** Rewrote function call\n";
        return true;
    }
};

class ExampleASTConsumer : public ASTConsumer {
    ExampleVisitor *visitor;

public:
  explicit ExampleASTConsumer(CompilerInstance *CI)
      : visitor(new ExampleVisitor(CI)) {}

  void HandleTranslationUnit(ASTContext &Context) {
    visitor->TraverseDecl(Context.getTranslationUnitDecl());
  }
};

class ExampleFrontendAction : public ASTFrontendAction {
public:
    unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
      return make_unique<ExampleASTConsumer>(&CI);
    }
};

int main(int argc, const char **argv) {
    cl::OptionCategory MyToolCategory("My tool options");
    CommonOptionsParser op(argc, argv, MyToolCategory);
    ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    int result = Tool.run(newFrontendActionFactory<ExampleFrontendAction>().get());

    errs() << "\nFound " << numFunctions << " functions.\n\n";
    rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(errs());
    return result;
}

