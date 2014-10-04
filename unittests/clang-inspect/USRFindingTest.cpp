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
namespace foobar {\n\
class Bar {\n\
public:\n\
  int x;\n\
};\n\
}\n\
void bar() {\n\
  foobar::Bar b;\n\
  b.x = 0;\n\
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
  testInspection(CODE, "2:5", "foo", "int (char)", "2:5");
}

TEST(USRFindingTest, FindParamDecl) {
  testInspection(CODE, "2:14", "c", "char", "2:14");
}

TEST(USRFindingTest, FindVarDecl) {
  testInspection(CODE, "3:7", "a", "int", "3:7");
}

TEST(USRFindingTest, FindVarExpr) {
  testInspection(CODE, "4:10", "a", "int", "3:7");
}

TEST(USRFindingTest, FindClassDecl) {
  testInspection(CODE, "7:8", "foobar::Bar", "foobar::Bar", "7:7");
}

TEST(USRFindingTest, FindClassMemberDecl) {
  testInspection(CODE, "9:7", "foobar::Bar::x", "int", "9:7");
}

TEST(USRFindingTest, FindClassMemberExpr) {
  testInspection(CODE, "14:5", "foobar::Bar::x", "int", "9:7");
}

/* FIXME: namspace inspection
TEST(USRFindingTest, FindNamespaceDecl) {
  testInspection(CODE, "9:13", "foobar", "namespace", "9:11");
}
*/

}
}
