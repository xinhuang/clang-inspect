#include "gtest/gtest.h"
#include "clang/Tooling/Tooling.h"
#include <string>

#include "USRFindingAction.h"
#include "Util.h"

namespace inspect {
namespace unittest {

static const char CODE[] = "\n\
int foo(char c) {\n\
  int a = 33;\n\
  return a + c + 1;\n\
}\n\
";

void testInspection(const char *code, const std::string &loc,
                    const std::string &expectName,
                    const std::string &expectType, const std::string &expectLoc) {
  USRFindingAction usrAction(loc);
  auto factory = clang::tooling::newFrontendActionFactory(&usrAction);
  EXPECT_TRUE(clang::tooling::runToolOnCode(factory->create(), code));

  EXPECT_EQ(expectName, usrAction.getUSRSpelling());
  EXPECT_EQ(expectType, usrAction.getUSRType());

  auto locationString = usrAction.getDefinitionLocation();
  std::string actualLocation(locationString.begin() + locationString.length() -
                                 expectLoc.length(),
                             locationString.end());
  EXPECT_EQ(expectLoc, actualLocation);
}

TEST(USRFindingTest, FindFunctionDecl) {
  auto location = "2:5";
  auto expectName = "foo";
  auto expectType = "int (char)";
  auto expectLocation = "2:5";

  testInspection(CODE, location, expectName, expectType, expectLocation);
}

TEST(USRFindingTest, FindParamDecl) {
  auto location = "2:14";
  auto expectName = "c";
  auto expectType = "char";
  auto expectLocation = "2:14";

  testInspection(CODE, location, expectName, expectType, expectLocation);
}

TEST(USRFindingTest, FindVarDecl) {
  auto location = "3:7";
  auto expectName = "a";
  auto expectType = "int";
  auto expectLocation = "3:7";

  testInspection(CODE, location, expectName, expectType, expectLocation);
}

}
}
