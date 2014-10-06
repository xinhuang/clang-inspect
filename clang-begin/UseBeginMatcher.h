#pragma once

#include "clang/ASTMatchers/ASTMatchers.h"

namespace begin {
extern const char *ClassId;
clang::ast_matchers::StatementMatcher createMemberBeginEndMatcher();
}
