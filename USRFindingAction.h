#pragma once

#include "clang/Frontend/FrontendAction.h"

namespace clang {
class ASTConsumer;
}

namespace inspect {

class USRFindingAction {
public:
  USRFindingAction(const std::string& offsetString);

  std::unique_ptr<clang::ASTConsumer> newASTConsumer();
  const std::string& getUSRSpelling() const { return spellingName; }
  const std::string& getUSRType() const { return typeInfo; }
  const std::string& getDefinitionLocation() const { return definitionLocation; }

private:
  std::tuple<int, int> symbolOffset;
  std::string spellingName;
  std::string typeInfo;
  std::string definitionLocation;
};
}
