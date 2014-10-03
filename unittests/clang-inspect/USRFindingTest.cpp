#include "gtest/gtest.h"
#include "clang/Tooling/Tooling.h"
#include <string>

#include "USRFindingAction.h"
#include "Util.h"

namespace inspect {
namespace unittest {

TEST(USRFindingTest, FindFunctionDecl) {
  const char CODE[] = "\n\
int foo(char c) {\n\
  return c + 1;\n\
}\n\
";
  auto location = "2:5";
  auto expectName = "foo";
  auto expectType = "int (char)";
  std::string expectLocation = "2:5";
  USRFindingAction usrAction(location);
  auto factory = clang::tooling::newFrontendActionFactory(&usrAction);
  EXPECT_TRUE(clang::tooling::runToolOnCode(factory->create(), CODE));

  EXPECT_EQ(expectName, usrAction.getUSRSpelling());
  EXPECT_EQ(expectType, usrAction.getUSRType());

  auto locationString = usrAction.getDefinitionLocation();
  std::string actualLocation(locationString.begin() + locationString.length() -
                                 expectLocation.length(),
                             locationString.end());
  EXPECT_EQ(expectLocation, actualLocation);
}

}
}
