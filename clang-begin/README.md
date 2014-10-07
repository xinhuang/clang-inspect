# clang-begin

Use `std::begin` `std::end` instead of member function `begin` `end`.

This transformation will only be applied to classes that has both member `begin` `end` defined.

## Usage

```
clang-begin <files> [-pl] [-i] [-p=compilation_database.json] --
  -pl: Print locations.
  -i: In-place.
```
