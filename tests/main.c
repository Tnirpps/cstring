#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#define CSTRING_IMPLEMENTATION
#include "../cstring.h"

#define assertEq(X, Y)                                    \
    while ((X) != (Y)) {                                  \
        printf("%zu vs %zu\n", (size_t)(X), (size_t)(Y)); \
        assert((X) == (Y));                               \
    }

#define assertNotEq(X, Y)                                 \
    while ((X) == (Y)) {                                  \
        printf("%zu vs %zu\n", (size_t)(X), (size_t)(Y)); \
        assert((X) != (Y));                               \
    }

#define UNUSED(x) (void)(x)

void printGreen(const char *text) {
    printf("\033[32m[ok]\033[0m %s", text);
}

void printRed(const char *text) {
    printf("\033[31m[wa] %s\033[0m", text);
}

void test_stringStartWith() {
    TString str = stringInitWithCharArr("Hello, World!");

    TString prefix = stringInitWithCharArr("Hello");
    assertEq(stringStartWith(str, prefix), true);

    TString wrongPrefix = stringInitWithCharArr("World");
    assertEq(stringStartWith(str, wrongPrefix), false);

    stringDestroy(&str);
    stringDestroy(&prefix);
    stringDestroy(&wrongPrefix);

    printGreen("test_stringStartWith\n");
}

void test_stringStartWithCharArr() {
    TString str = stringInitWithCharArr("Hello, World!");

    assertEq(stringStartWithCharArr(str, "Hello"), true);
    assertEq(stringStartWithCharArr(str, "World"), false);

    stringDestroy(&str);

    printGreen("test_stringStartWithCharArr\n");
}

void test_stringEndWith() {
    TString str = stringInitWithCharArr("Hello, World!");

    TString suffix = stringInitWithCharArr("World!");
    assertEq(stringEndWith(str, suffix), true);

    TString wrongSuffix = stringInitWithCharArr("Hello");
    assertEq(stringEndWith(str, wrongSuffix), false);

    stringDestroy(&str);
    stringDestroy(&suffix);
    stringDestroy(&wrongSuffix);

    printGreen("test_stringEndWith\n");
}

void test_stringEndWithCharArr() {
    TString str = stringInitWithCharArr("Hello, World!");

    assertEq(stringEndWithCharArr(str, "World!"), true);
    assertEq(stringEndWithCharArr(str, "Hello"), false);

    stringDestroy(&str);

    printGreen("test_stringEndWithCharArr\n");
}

void test_stringLen() {
    TString str = stringInitWithCharArr("Hello");
    assertEq(stringLen(str), 5);

    TString emptyString = stringInitWithCharArr("");
    assertEq(stringLen(emptyString), 0);

    stringDestroy(&str);
    stringDestroy(&emptyString);

    printGreen("test_stringLen\n");
}

void test_stringFindFirst() {
    TString str = stringInitWithCharArr("Hello, World!");
    TString pattern = stringInitWithCharArr("World");
    assertEq(stringFindFirst(str, pattern), 7);

    TString nonExistingPattern = stringInitWithCharArr("XYZ");
    assertEq(stringFindFirst(str, nonExistingPattern), -1);

    stringDestroy(&str);
    stringDestroy(&pattern);
    stringDestroy(&nonExistingPattern);

    printGreen("test_stringFindFirst\n");
}

void test_stringFindFirstCharArr() {
    TString str = stringInitWithCharArr("Hello, World!");

    assertEq(stringFindFirstCharArr(str, "World"), 7);
    assertEq(stringFindFirstCharArr(str, "XYZ"), -1);

    stringDestroy(&str);

    printGreen("test_stringFindFirstCharArr\n");
}

void test_stringInit() {
    TString str = stringInit(10);

    assertEq(str.capacity, 10);
    assertEq(str.size, 0);

    stringDestroy(&str);

    printGreen("test_stringInit\n");
}

void test_stringInitWithCharArr() {
    const char *testStr = "Test string";
    TString str = stringInitWithCharArr(testStr);

    assertEq(stringLen(str), strlen(testStr));
    assertEq(strncmp(str.data, testStr, str.size), 0);

    stringDestroy(&str);

    printGreen("test_stringInitWithCharArr\n");
}

void test_stringInitWithInt() {
    int64_t number = 12345678;
    TString str = stringInitWithInt(number);

    TString expectedString = stringInitWithCharArr("12345678");
    assertEq(stringLen(str), stringLen(expectedString));
    assertEq(strncmp(str.data, expectedString.data, str.size), 0);

    stringDestroy(&str);
    stringDestroy(&expectedString);

    printGreen("test_stringInitWithInt\n");
}

void test_stringCopy() {
    TString original = stringInitWithCharArr("Test string for copy");
    TString copied = stringCopy(original);

    assertEq(stringLen(copied), stringLen(original));
    assertEq(strncmp(copied.data, original.data, copied.size), 0);
    assertEq(copied.data, original.data);  // Ensure it's a shallow copy

    stringDestroy(&original);

    printGreen("test_stringCopy\n");
}

void test_stringDeepCopy() {
    TString original = stringInitWithCharArr("Test string for deep copy");
    TString deepCopy = stringDeepCopy(original);

    assertEq(stringLen(deepCopy), stringLen(original));
    assertEq(strncmp(deepCopy.data, original.data, deepCopy.size), 0);
    assertNotEq(deepCopy.data, original.data);  // Ensure it's a deep copy

    stringDestroy(&original);
    stringDestroy(&deepCopy);

    printGreen("test_stringDeepCopy\n");
}

void test_stringSubstring() {
    TString str = stringInitWithCharArr("Hello, World!");
    size_t startPos = 7;
    size_t length = 5;
    TString sub = stringSubstring(str, startPos, length);
    TString ans = stringInitWithCharArr("World");

    assertEq(stringLen(sub), length);
    assertEq(stringIsEqual(sub, ans), true);
    assertEq(strncmp(sub.data, &str.data[startPos], length), 0);

    stringDestroy(&str);
    stringDestroy(&sub);
    stringDestroy(&ans);

    printGreen("test_stringSubstring\n");
}

void test_stringConcat() {
    TString str1 = stringInitWithCharArr("Hello");
    TString str2 = stringInitWithCharArr(", World!");
    TString result = stringConcat(str1, str2);

    assertEq(stringLen(result), stringLen(str1) + stringLen(str2));
    assertEq(strncmp(result.data, "Hello, World!", stringLen(result)), 0);

    stringDestroy(&str1);
    stringDestroy(&str2);
    stringDestroy(&result);

    printGreen("test_stringConcat\n");
}

void test_stringArrConcat() {
    TString strs[3];
    strs[0] = stringInitWithCharArr("One");
    strs[1] = stringInitWithCharArr("Two");
    strs[2] = stringInitWithCharArr("Three");
    size_t count = sizeof(strs) / sizeof(strs[0]);

    TString result = stringArrConcat(strs, count);

    size_t expectedLength = 0;
    for (size_t i = 0; i < count; ++i) {
        expectedLength += stringLen(strs[i]);
    }

    assertEq(stringLen(result), expectedLength);
    assertEq(strncmp(result.data, "OneTwoThree", stringLen(result)), 0);

    for (size_t i = 0; i < count; ++i) {
        stringDestroy(&strs[i]);
    }
    stringDestroy(&result);

    printGreen("test_stringArrConcat\n");
}

void test_stringPushBack() {
    TString str = stringInitWithCharArr("hello");
    char c = '!';
    stringPushBack(&str, c);

    assertEq(stringLen(str), 6);
    assertEq(str.data[stringLen(str) - 1], c);

    stringDestroy(&str);
    printGreen("test_stringPushBack\n");
}

void test_stringTrim() {
    TString str = stringInitWithCharArr("   hello   ");
    stringTrim(&str);

    assertEq(stringLen(str), 5);
    assertEq(strncmp(str.data, "hello", stringLen(str)), 0);

    stringPushBack(&str, '\n');
    for (size_t i = 0; i < 100; ++i) {
        stringPushBack(&str, ' ');
    }
    stringTrim(&str);

    assertEq(stringLen(str), 5);
    assertEq(strncmp(str.data, "hello", stringLen(str)), 0);

    stringDestroy(&str);
    printGreen("test_stringTrim\n");
}

void test_stringReplaceAll() {
    TString str = stringInitWithCharArr("hello world, world!");
    const char *oldSub = "world";
    const char *newSub = "C";
    stringReplaceAll(&str, oldSub, newSub);

    TString expected = stringInitWithCharArr("hello C, C!");
    assertEq(stringLen(str), stringLen(expected));
    assertEq(strncmp(str.data, expected.data, stringLen(expected)), 0);

    stringDestroy(&str);
    stringDestroy(&expected);
    printGreen("test_stringReplaceAll\n");
}

void test_stringReverse() {
    TString str = stringInitWithCharArr("hello");
    stringReverse(&str);

    assertEq(stringLen(str), 5);
    assertEq(strncmp(str.data, "olleh", stringLen(str)), 0);

    stringDestroy(&str);
    printGreen("test_stringReverse\n");
}

void test_stringCompare() {
    TString s1 = stringInitWithCharArr("hello");
    TString s2 = stringInitWithCharArr("hello");
    TString s3 = stringInitWithCharArr("heaa");

    assertEq(stringCompare(s1, s2), 0);
    assertEq(stringCompare(s1, s3), -1);

    stringDestroy(&s1);
    stringDestroy(&s2);
    stringDestroy(&s3);
    printGreen("test_stringCompare\n");
}

void test_stringToUpper() {
    TString s = stringInitWithCharArr("Hello World! 123\n");
    TString res = stringInitWithCharArr("HELLO WORLD! 123\n");
    stringToUpper(&s);
    assertEq(stringCompare(s, res), 0);

    stringDestroy(&s);
    stringDestroy(&res);
    printGreen("test_stringToUpper\n");
}

void test_stringToLower() {
    TString s = stringInitWithCharArr("Hello World! 123\n");
    TString res = stringInitWithCharArr("hello world! 123\n");
    stringToLower(&s);
    assertEq(stringCompare(s, res), 0);

    stringDestroy(&s);
    stringDestroy(&res);
    printGreen("test_stringToLower\n");
}

char func(char c) {
    if ('0' <= c && c <= '9') {
        return '*';
    }
    return c;
}

void test_stringMap() {
    TString s = stringInitWithCharArr("He22o Wor1d! 123\n");
    TString res = stringInitWithCharArr("He**o Wor*d! ***\n");
    stringMap(&s, func);
    assertEq(stringCompare(s, res), 0);

    stringDestroy(&s);
    stringDestroy(&res);
    printGreen("test_stringMap\n");
}

void test_stringContains() {
    TString s = stringInitWithCharArr("hi good looo shiii po ns skj skdjl hello wgg\n");
    TString p1 = stringInitWithCharArr("hello");
    TString p2 = stringInitWithCharArr("hello1");

    assertEq(stringContains(s, p1), true);
    assertEq(stringContains(s, p2), false);

    stringDestroy(&s);
    stringDestroy(&p1);
    stringDestroy(&p2);

    printGreen("test_stringContains\n");
}

bool acceptAll(char c) {
    UNUSED(c);
    return true;
}

bool skipAll(char c) {
    UNUSED(c);
    return false;
}

bool acceptOnlyDigit(char c) {
    return ('0' <= c) && (c <= '9');
}

void test_stringFilter() {
    TString emptyString = stringInitWithCharArr("");
    stringFilter(&emptyString, acceptAll);
    assertEq(stringIsEmpty(emptyString), true);

    TString s1 = stringInitWithCharArr("banana banana");
    TString s1Expected = stringDeepCopy(s1);
    stringFilter(&s1, acceptAll);
    assertEq(stringCompare(s1, s1Expected), 0);

    TString s2 = stringInitWithCharArr("abc123def");
    TString s2Expected = stringInitWithCharArr("123");
    stringFilter(&s2, acceptOnlyDigit);
    assertEq(stringCompare(s2, s2Expected), 0);

    stringDestroy(&emptyString);
    stringDestroy(&s1);
    stringDestroy(&s1Expected);
    stringDestroy(&s2);
    stringDestroy(&s2Expected);
    printGreen("test_stringFilter\n");
}

void test_stringIsPalindrome() {
    TString emptyString = stringInitWithCharArr("");
    assertEq(stringIsPalindrome(emptyString), true);
    stringDestroy(&emptyString);

    TString notPalindromeEvenSizedString = stringInitWithCharArr("banaba");
    assertEq(stringIsPalindrome(notPalindromeEvenSizedString), false);
    stringDestroy(&notPalindromeEvenSizedString);

    TString palindromeEvenSizedString = stringInitWithCharArr("abba");
    assertEq(stringIsPalindrome(palindromeEvenSizedString), true);
    stringDestroy(&palindromeEvenSizedString);

    TString notPalindromeOddSizedString = stringInitWithCharArr("abc");
    assertEq(stringIsPalindrome(notPalindromeOddSizedString), false);
    stringDestroy(&notPalindromeOddSizedString);

    TString palindromeOddSizedString = stringInitWithCharArr("abcba");
    assertEq(stringIsPalindrome(palindromeOddSizedString), true);
    stringDestroy(&palindromeOddSizedString);

    printGreen("test_stringIsPalindrome\n");
}

void test_stringPad() {
    TString str = stringInitWithCharArr("Hello, World!");
    stringPadLeft(&str, 20, ' ');

    assertEq(stringLen(str), 20);
    assertEq(strncmp(str.data, "       Hello, World!", stringLen(str)), 0);

    stringPadLeft(&str, 10, ' ');
    assertEq(stringLen(str), 20);
    assertEq(strncmp(str.data, "       Hello, World!", stringLen(str)), 0);

    stringTrim(&str);

    stringPadRight(&str, 20, ' ');

    assertEq(stringLen(str), 20);
    assertEq(strncmp(str.data, "Hello, World!       ", stringLen(str)), 0);

    stringPadRight(&str, 10, ' ');
    assertEq(stringLen(str), 20);
    assertEq(strncmp(str.data, "Hello, World!       ", stringLen(str)), 0);

    stringDestroy(&str);
    printGreen("test_stringPad\n");
}

void test_stringRemove() {
    TString s = stringInitWithCharArr("Hello, World! testing remove");

    stringRemove(&s, stringFindFirstCharArr(s, "test"), 8);
    assertEq(strncmp(s.data, "Hello, World! remove", stringLen(s)), 0);

    stringRemove(&s, 0, 10000);
    assertEq(stringIsEmpty(s), true);

    stringDestroy(&s);

    printGreen("test_stringRemove\n");
}

void test_stringToInt() {
    TString s1 = stringInitWithCharArr("0");
    TString s2 = stringInitWithCharArr("-2132456");
    TString s3 = stringInitWithCharArr("9223372036854775807");
    TString s4 = stringInitWithCharArr("-9223372036854775808");
    
    assertEq(stringToInt(s1), 0);
    assertEq(stringToInt(s2), -2132456);
    assertEq(stringToInt(s3), 9223372036854775807LL);
    assertEq(stringToInt(s4), (-9223372036854775807LL - 1));
  
    stringDestroy(&s1);
    stringDestroy(&s2);
    stringDestroy(&s3);
    stringDestroy(&s4);
  
    printGreen("test_stringToInt\n");
}

void test_stringToDouble() {
    TString s1 = stringInitWithCharArr("0");
    TString s2 = stringInitWithCharArr("12.03459");
    TString s3 = stringInitWithCharArr("-12.34");

    assertEq(stringToDouble(s1), 0);
    assertEq(stringToDouble(s2), 12.34);
    assertEq(stringToDouble(s3), -12.34);

    stringDestroy(&s1);
    stringDestroy(&s2);
    stringDestroy(&s3);

    printGreen("stringToDouble\n");
}

void test_stringCapitalize(){
    TString num1 = stringInitWithCharArr("Hello, World! that is my test");
    TString answer1 = stringInitWithCharArr("Hello, World! That Is My Test");
    TString num2 = stringInitWithCharArr("1ello, World! that is my test");
    TString answer2 = stringInitWithCharArr("1ello, World! That Is My Test");
    TString num3 = stringInitWithCharArr("Hello, World! tha%t is my test");
    TString answer3= stringInitWithCharArr("Hello, World! Tha%t Is My Test");
    TString num4 = stringInitWithCharArr("Hello, World!.that is my test");
    TString answer4 = stringInitWithCharArr("Hello, World!.That Is My Test");
    
    stringCapitalize(&num1);
    assertEq(stringCompare(num1, answer1), 0);
    stringCapitalize(&num2);
    assertEq(stringCompare(num2, answer2), 0);
    stringCapitalize(&num3);
    assertEq(stringCompare(num3, answer3), 0);
    stringCapitalize(&num4);
    assertEq(stringCompare(num4, answer4), 0);

    stringDestroy(&num1);
    stringDestroy(&num2);
    stringDestroy(&num3);
    stringDestroy(&num4);
    stringDestroy(&answer1);
    stringDestroy(&answer2);
    stringDestroy(&answer3);
    stringDestroy(&answer4);

    printGreen("test_stringCapitalize\n");
}

int main() {
    test_stringStartWith();
    test_stringEndWith();
    test_stringLen();
    test_stringFindFirst();
    test_stringFindFirstCharArr();
    test_stringInit();
    test_stringInitWithInt();
    test_stringInitWithCharArr();
    test_stringCopy();
    test_stringDeepCopy();
    test_stringSubstring();
    test_stringConcat();
    test_stringArrConcat();
    test_stringPushBack();
    test_stringTrim();
    test_stringReplaceAll();
    test_stringReverse();
    test_stringCompare();
    test_stringToLower();
    test_stringToUpper();
    test_stringToDouble();
    test_stringMap();
    test_stringContains();
    test_stringFilter();
    test_stringIsPalindrome();
    test_stringPad();
    test_stringRemove();
    test_stringCapitalize();
    test_stringToInt();
    return 0;
}
