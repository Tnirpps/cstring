#define CSTRING_IMPLEMENTATION
#include <stdio.h>
#include "cstring.h"
#include <string.h>

int64_t LevenstheinRecursive(TString s1, TString s2, int64_t len1, int64_t len2) {
    int64_t a, b, c;
    if (len1 == 0) {
        return len2;
    }
    if (len2 == 0) {
        return len1;
    }
    if (s1.data[len1 - 1] == s2.data[len2 - 1]) {
        return LevenstheinRecursive(s1, s2, len1 - 1, len2 - 1);
    }
    a = LevenstheinRecursive(s1, s2, len1 - 1, len2 - 1);
    b = LevenstheinRecursive(s1, s2, len1, len2 - 1);
    c = LevenstheinRecursive(s1, s2, len1 - 1, len2);

    if (a > b) a = b;
    if (a > c) a = c;
    return a + 1;
}

int64_t stringLevenstheinDistance(TString s1, TString s2) {
    int64_t len1 = (int64_t)stringLen(s1);
    int64_t len2 = (int64_t)stringLen(s2);
    int64_t res = LevenstheinRecursive(s1, s2, len1, len2);
    return res;
}

int main(void) {
    TString myString1 = stringInitWithCharArr("hello world");
    TString myString2 = stringInitWithCharArr("hello 112312ld");
    int64_t res = stringLevenstheinDistance(myString1, myString2);
    printf("%d", res);
}
