# cstring
Is a header-only library that provides a rich set of string manipulation functions in pure C. It is designed to offer developers a comprehensive toolkit for handling strings in C projects, making common string operations more convenient and safer compared to using standard C string functions directly.

## Features

- Header-only library for easy integration into C projects.
- Wide range of string manipulation functions.
- Emphasis on safety and performance.
- Simple and clear API for ease of use.
- Compatible with standard C compilers.

## Installation

To use cstirng, simply download and include the header file in your project:
```
#define CSTRING_IMPLEMENTATION
#include "cstring.h"


int main() {
    ...
}
```
There is no need to compile or link against any binaries since it is a header-only library.

## Usage

To use a function from cstring, simply call it as you would any standard C function. Here's an example of using stringToUpper:
```
TString myString = stringInitWithCharArr("hello world");
stringToUpper(&myString);
stringPrint(myString);
stringDestroy(&myString);
```

## Contributing

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated.

0. Read [coding conventions](https://github.com/Tnirpps/cstring/blob/main/CONTRIBUTING.md)
1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## TODO List

- [x] bool stringContains(TString s, TString pattern); - Check if string contains a certain substring.
- [x] bool stringContainsCharArr(TString s, const char *pattern); - Check if string contains a certain substring using a char array.
- [x] bool stringIsEmpty(TString s); - Check if the string is empty.
- [x] bool stringIsEqualIgnoreCase(TString s1, TString s2); - Compare strings for equality, case-insensitive.
- [x] bool stringIsPalindrome(TString s); - Check if a string is a palindrome.
- [ ] TStringArray stringSplit(TString s, TString delim); - Split a string into an array of strings based on a delimiter.
- [ ] TStringArray stringSplitCharArr(TString s, const char *delim); - Split string using a char array delimiter.
- [ ] double stringToDouble(TString s); - Convert a string to a double.
- [ ] int64_t stringToInt(TString s); - Convert a string to an integer with error checking.
- [ ] int64_t stringLevenshteinDistance(TString s1, TString s2); - Calculate Levenshtein distance between strings.
- [x] size_t stringCount(TString s, char c); - Count occurrences of a character.
- [ ] size_t stringCountSubstring(TString s, TString pattern); - Count occurrences of a substring.
- [ ] void stringCapitalize(TString *s); - Capitalize the first letter of each word.
- [x] void stringFilter(TString *s, bool (*predicate)(char)); - Remove characters not satisfying a predicate.
- [x] void stringInsert(TString *s, size_t pos, TString toInsert); - Insert a substring at a specified position.
- [ ] void stringInsertCharArr(TString *s, size_t pos, const char *toInsert); - Insert a substring from a char array.
- [x] void stringMap(TString *s, char (*func)(char)); - Apply a function to every character of the string.
- [x] void stringMapIndex(TString *s, char (*func)(size_t, char)); - Apply a function to every character of the string with access to its index.
- [x] void stringPadLeft(TString *s, size_t newLen, char padChar); - Pad the string on the left to a certain length.
- [x] void stringPadRight(TString *s, size_t newLen, char padChar); - Pad the string on the right.
- [x] void stringRemove(TString *s, size_t pos, size_t len); - Remove a range of characters from the string.
- [ ] void stringReplaceFirst(TString *s, const char *oldSub, const char *newSub); - Replace the first occurrence of a substring.
- [x] void stringSwap(TString *s1, TString *s2); - Swap the content of two strings.
- [ ] TString stringFormat(const char *format, ...); - Create a formatted string.
