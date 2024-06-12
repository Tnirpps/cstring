#ifndef CSTRING_LIB
#define CSTRING_LIB

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum EErrorCode {
    ERR_NO_ERROR,
    ERR_ALLOCATE_SPACE,
    ERR_BUFFER_OVERFLOW,
    ERR_EMPTY_STRING_POP,
    ERR_EOF_NOT_EXPECTED,
    ERR_INVALID_STATE,
    ERR_NULL_POINTER,
    ERR_NUMBER_OVERFLOW,
    ERR_INVALID_NUMBER_REPR,
} EErrorCode;

#define MAX_ERROR_MSG_LEN 300
static EErrorCode ERROR_CODE = ERR_NO_ERROR;
static char ERROR_BUF[MAX_ERROR_MSG_LEN] = {0};

int isError();
const char *getErrorMsg();
void setError(EErrorCode e);
void clearError();

typedef struct TString {
    char *data;
    size_t size;
    size_t capacity;
} TString;

typedef struct TStrVec {
    TString *data;
    size_t size;
    size_t capacity;
} TStrVec;


bool stringCharIsDigit(char c);
bool stringCharIsAlpha(char c);
bool stringCharIsAlphanum(char c);
char stringCharToLower(char c);
char stringCharToUpper(char c);

int stringCharToInt(char c);

bool stringStartWith(TString s, TString pref);
bool stringStartWithCharArr(TString s, const char *pref);
bool stringEndWith(TString s, TString pref);
bool stringEndWithCharArr(TString s, const char *pref);
bool stringIsEqual(TString s1, TString s2);
bool stringIsEqualIgnoreCase(TString s1, TString s2);
bool stringIsEmpty(TString s);
bool stringIsDigits(TString s);
bool stringIsAlphas(TString s);
bool stringContains(TString s, TString pattern);
bool stringContainsCharArr(TString s, const char *pattern);
bool stringIsPalindrome(TString s);

size_t stringLen(TString s);
size_t stringCount(TString s, char c);

int stringCompare(TString s1, TString s2);

int64_t stringFindFirst(TString s, TString pattern);
int64_t stringFindFirstCharArr(TString s, const char *pattern);
int64_t stringToInt(TString s);

TString stringRand(size_t size);
TString stringInit(size_t capacity);
TString stringInitWithCharArr(const char *s);
TString stringInitWithInt(int64_t n);
TString stringCopy(TString s);
TString stringDeepCopy(TString s);
TString stringSubstring(TString s, size_t pos, size_t len);
TString stringConcat(TString s1, TString s2);
TString stringArrConcat(const TString *s, size_t count);
TString stringJoin(TString s1, TString s2, TString delim);
TString stringJoinCharArr(TString s1, TString s2, const char *delim);
TString stringArrJoin(const TString *s, size_t count, TString delim);
TString stringArrJoinCharArr(const TString *s, size_t count, const char *delim);

char* stringConvertToCharArr(TString s);
void stringScan(TString *s);
void stringPrint(TString s);
void stringDebug(TString s);

void stringRemoveChar(TString *s, char c);
void stringSwap(TString *s1, TString *s2);
void stringPushBack(TString *s, char c);
void stringPushFront(TString *s, char c);
void stringPopBack(TString *s);
void stringPopFront(TString *s);
void stringTrimLeft(TString *s);
void stringTrimRight(TString *s);
void stringTrim(TString *s);
void stringPadRight(TString *s, size_t newLen, char padChar);
void stringPadLeft(TString *s, size_t newLen, char padChar);
void stringToUpper(TString *s);
void stringToLower(TString *s);
void stringReplaceAll(TString *s, const char *oldSub, const char *newSub);
void stringReverse(TString *s);
void stringFilter(TString *s, bool (*predicate)(char));
void stringMap(TString *s, char (*func)(char));
void stringMapIndex(TString *s, char (*func)(size_t, char));
void stringRemove(TString *s, size_t pos, size_t len);
void stringDestroy(TString *s);
void stringCapitalize(TString *s);

double stringToDouble(TString s);

#endif

// for testing:
#define CSTRING_IMPLEMENTATION

#ifdef CSTRING_IMPLEMENTATION

int isError() {
    return (ERROR_CODE != ERR_NO_ERROR);
}

/*
// TODO: describe all errors
const char * getErrorMsg() {
    switch (ERROR_CODE) {
        case ERR_NO_ERROR:
            *ERROR_BUF = '\0';
            break;
        case ERR_ALLOCATE_SPACE:
            memcpy(ERROR_BUF, "Unable to allocate memory", MAX_ERROR_MSG_LEN * sizeof(char));
            break;
        case ERR_NULL_POINTER:
            memcpy(ERROR_BUF, "String or its data is NULL", MAX_ERROR_MSG_LEN * sizeof(char));
            break;
        case ERR_INVALID_STATE:
            break;
        case ERR_BUFFER_OVERFLOW:
            break;
        case ERR_NUMBER_OVERFLOW:
            break;
        default:
            break;
    }
    return ERROR_BUF;
}
*/

void setError(EErrorCode e) {
    ERROR_CODE = e;
}

void clearError() {
    setError(ERR_NO_ERROR);
    *ERROR_BUF = '\0';
}

// private

void swapPtr(void **a, void **b) {
    void *c = *b;
    *b = *a;
    *a = c;
}

bool stringCharOneOf(char c, const char *variants) {
    while (*variants) {
        if (*variants == c) return true;
        ++variants;
    }
    return false;
}

void stringCopyCharArr(TString *dest, const char *s, size_t n) {
    assert(dest != NULL && dest->data != NULL && dest->capacity >= n);
    for (size_t i = 0; i < n && s[i] != '\0'; ++i) {
        dest->data[i] = s[i];
    }
}

int stringCompSubstr(
    const char *s1, size_t pos1, size_t len1,
    const char *s2, size_t pos2, size_t len2,
    bool caseSensative) {
    if (len1 > len2) return -1 * stringCompSubstr(s2, pos2, len2, s1, pos1, len1, caseSensative);
    assert(s1 != NULL && s2 != NULL);
    for (size_t i = 0; i < len1; ++i) {
        if (caseSensative) {
            if (s1[pos1 + i] != s2[pos2 + i]) {
                if (s1[pos1 + 1] < s2[pos2 + i]) return 1;
                return -1;
            }
        } else {
            if (stringCharToLower(s1[pos1 + i]) != stringCharToLower(s2[pos2 + i])) {
                if (stringCharToLower(s1[pos1 + 1]) < stringCharToLower(s2[pos2 + i])) return 1;
                return -1;
            }
        }
    }
    if (len1 != len2) return -1;
    return 0;
}

size_t stringLenCharArr(const char *s) {
    if (s == NULL) return 0;
    const char *begin = s;
    while (*s != '\0') ++s;
    return (size_t)(s - begin);
}

void stringIncreaseCap(TString *s) {
    assert(s != NULL);
    size_t newCap = s->capacity * 2;
    if (newCap == 0) newCap = 1;
    char *newData = (char *)malloc(sizeof(char) * newCap);
    if (newData == NULL) {
        setError(ERR_ALLOCATE_SPACE);
        return;
    }

    swapPtr((void **)&s->data, (void **)&newData);
    stringCopyCharArr(s, newData, s->size);
    free(newData);
    s->capacity = newCap;
}

// import 

bool stringCharIsDigit(char c) {
    return ('0' <= c && c <= '9');
}

bool stringCharIsAlpha(char c) {
    return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
}

bool stringCharIsAlphanum(char c) {
    return stringCharIsDigit(c) || stringCharIsAlpha(c);
}

char stringCharToLower(char c) {
    if ('A' <= c && c <= 'Z') return 'a' + (c - 'A');
    return c;
}
char stringCharToUpper(char c) {
    if ('a' <= c && c <= 'z') return 'A' + (c - 'a');
    return c;
}

int stringCharToInt(char c) {
    clearError();
    if ('0' <= c && c <= '9') 
        return c - '0';
    setError(ERR_INVALID_NUMBER_REPR);
    return -1;
}

bool stringStartWith(TString s, TString pref) {
    if (s.size < pref.size) return false;
    return stringCompSubstr(s.data, 0, pref.size, pref.data,
                            0, pref.size, true /* caseSensative */) == 0;
}

bool stringStartWithCharArr(TString s, const char *pref) {
    if (pref == NULL) return true;
    size_t len = stringLenCharArr(pref);
    if (s.size < len) return false;
    return stringCompSubstr(s.data, 0, len, pref, 0, len, true /* caseSensative */) == 0;
}

bool stringEndWith(TString s, TString pref) {
    if (s.size < pref.size) return false;
    return stringCompSubstr(
               s.data, s.size - pref.size, pref.size,
               pref.data, 0, pref.size, true /* caseSensative */) == 0;
}

bool stringEndWithCharArr(TString s, const char *pref) {
    if (pref == NULL) return true;
    size_t len = stringLenCharArr(pref);
    if (s.size < len) return false;
    return stringCompSubstr(s.data, s.size - len, len, pref, 0, len, true /* caseSensative */) == 0;
}

bool stringIsEqual(TString s1, TString s2) {
    return stringCompSubstr(s1.data, 0, s1.size, s2.data, 0, s2.size, true /* caseSensative */) == 0;
}

bool stringIsEqualIgnoreCase(TString s1, TString s2) {
    return stringCompSubstr(s1.data, 0, s1.size, s2.data, 0, s2.size, false /* caseSensative */) == 0;
}

bool stringIsEmpty(TString s) {
    return s.size == 0;
}

bool stringIsDigits(TString s) {
    if (s.size == 0) return false;
    for (size_t i = 0; i < s.size; ++i) {
        if (!stringCharIsDigit(s.data[i])) return false;
    }
    return true;
}

bool stringIsAlphas(TString s) {
    if (s.size == 0) return false;
    for (size_t i = 0; i < s.size; ++i) {
        if (!stringCharIsAlpha(s.data[i])) return false;
    }
    return true;
}

bool stringContains(TString s, TString pattern) {
    return stringFindFirst(s, pattern) > 0;
}

bool stringContainsCharArr(TString s, const char *pattern) {
    return stringFindFirstCharArr(s, pattern) > 0;
}

size_t stringLen(TString s) {
    return s.size;
}

bool stringIsPalindrome(TString s) {
    for (size_t i = 0; i < (s.size + 1) / 2; ++i) {
        if (s.data[i] != s.data[s.size - i - 1]) {
            return false;
        }
    }
    return true;
}

size_t stringCount(TString s, char c) {
    if (s.size == 0) return 0;
    size_t res = 0;
    for (size_t i = 0; i < s.size; ++i) {
        res += (s.data[i] == c);
    }
    return res;
}

int stringCompare(TString s1, TString s2) {
    return stringCompSubstr(s1.data, 0, s1.size, s2.data, 0, s2.size, false /* caseSensative */);
}

int64_t stringFindFirst(TString s, TString pattern) {
    for (size_t i = 0; i < s.size; ++i) {
        size_t match = 0;
        for (size_t j = 0; i + j < s.size && j < pattern.size; ++j) {
            if (s.data[i + j] != pattern.data[j]) break;
            ++match;
        }
        if (match == pattern.size) return i;
    }
    return -1;
}

int64_t stringFindFirstCharArr(TString s, const char *pattern) {
    if (pattern == NULL) {
        setError(ERR_NULL_POINTER);
        return -1;
    }
    size_t patternLen = stringLenCharArr(pattern);
    for (size_t i = 0; i < s.size; ++i) {
        size_t match = 0;
        for (size_t j = 0; i + j < s.size && j < patternLen; ++j) {
            if (s.data[i + j] != pattern[j]) break;
            ++match;
        }
        if (match == patternLen) return i;
    }
    return -1;
}

int64_t stringToInt(TString s) {
    clearError();

    int64_t sign = 1;
    size_t i = 0;

    if (s.data[0] == '-') {
        if (s.size == 20 && stringCompSubstr(s.data, 0, 20, "-9223372036854775808", 0, 20, false) == 0) {
            return INT64_MIN;
        }
        if (s.size == 1) {
            setError(ERR_INVALID_NUMBER_REPR);
            return 0;
        }
        sign = -1;
        ++i;
    }

    int64_t val = 0;
    for (; i < s.size; ++i) {
        int64_t digit = stringCharToInt(s.data[i]);
        if (val > (INT64_MAX - digit) / 10) {
            setError(ERR_NUMBER_OVERFLOW);
            return val;
        }
        else {
            val = val * 10 + digit;
        }
    }
    val = val * sign;

    return val;
}

TString stringInit(size_t capacity) {
    TString s = {0};
    s.data = (char *)malloc(sizeof(char) * capacity);
    if (s.data == NULL) {
        setError(ERR_ALLOCATE_SPACE);
        return s;
    }
    s.capacity = capacity;
    s.size = 0;
    return s;
}

TString stringRand(size_t size) {
    static const char randChars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t len = sizeof(randChars) / sizeof(char) - 1;
    clearError();
    TString res = stringInit(size);
    if (isError()) {
        return res;
    }
    for (size_t i = 0; i < size; ++i) {
        res.data[i] = randChars[rand() % len];
    }
    res.size = size;
    return res;
}

TString stringInitWithCharArr(const char *s) {
    if (s == NULL) return (TString){0};
    clearError();
    size_t len = stringLenCharArr(s);
    TString res = stringInit(len);
    if (isError()) {
        return res;
    }
    for (size_t i = 0; i < len; ++i) {
        res.data[i] = s[i];
    }
    res.size = len;
    return res;
}

TString stringInitWithInt(int64_t n) {
    clearError();
    TString res = {0};
    int sign = 1;
    if (n == 0) return stringInitWithCharArr("0");
    if (n < 0) {
        sign = -1;
        n *= -1;
    }
    while (n > 0) {
        stringPushBack(&res, '0' + n % 10);
        if (isError()) {
            return (TString){0};
        }
        n /= 10;
    }
    if (sign == -1) {
        stringPushBack(&res, '-');
        if (isError()) {
            return (TString){0};
        }
    }
    stringReverse(&res);
    return res;
}

TString stringCopy(TString s) {
    TString res = s;
    return res;
}

TString stringDeepCopy(TString s) {
    clearError();
    TString res = stringInit(s.capacity);
    if (isError()) return (TString){0};

    for (size_t i = 0; i < s.size; ++i) {
        res.data[i] = s.data[i];
    }
    res.size = s.size;
    return res;
}

TString stringConcat(TString s1, TString s2) {
    clearError();
    TString res = stringInit(s1.size + s2.size);
    if (isError()) return (TString){0};

    for (size_t i = 0; i < s1.size; ++i) {
        res.data[i] = s1.data[i];
    }

    for (size_t i = 0; i < s2.size; ++i) {
        res.data[s1.size + i] = s2.data[i];
    }
    res.size = s1.size + s2.size;

    return res;
}

TString stringArrConcat(const TString *s, size_t count) {
    if (s == NULL) {
        setError(ERR_NULL_POINTER);
        return (TString){0};
    }
    clearError();
    size_t totalSize = 0;
    for (size_t i = 0; i < count; ++i) {
        totalSize += s[i].size;
    }

    TString res = stringInit(totalSize);
    if (isError()) return (TString){0};

    size_t ind = 0;
    for (size_t i = 0; i < count; ++i) {
        for (size_t j = 0; j < s[i].size; ++j) {
            res.data[ind++] = s[i].data[j];
        }
    }
    res.size = totalSize;
    return res;
}

TString stringJoin(TString s1, TString s2, TString delim) {
    clearError();
    TString res = stringInit(s1.size + delim.size + s2.size);
    if (isError()) return (TString){0};

    for (size_t i = 0; i < s1.size; ++i) {
        res.data[i] = s1.data[i];
    }

    for (size_t i = 0; i < delim.size; ++i) {
        res.data[s1.size + i] = delim.data[i];
    }

    for (size_t i = 0; i < s2.size; ++i) {
        res.data[s1.size + delim.size + i] = s2.data[i];
    }
    res.size = s1.size + delim.size + s2.size;

    return res;
}

TString stringJoinCharArr(TString s1, TString s2, const char *delim) {
    clearError();
    size_t delimSize = stringLenCharArr(delim);
    TString res = stringInit(s1.size + delimSize + s2.size);
    if (isError()) return (TString){0};

    for (size_t i = 0; i < s1.size; ++i) {
        res.data[i] = s1.data[i];
    }

    for (size_t i = 0; i < delimSize; ++i) {
        res.data[s1.size + i] = delim[i];
    }

    for (size_t i = 0; i < s2.size; ++i) {
        res.data[s1.size + delimSize + i] = s2.data[i];
    }
    res.size = s1.size + delimSize + s2.size;

    return res;
}

TString stringArrJoin(const TString *s, size_t count, TString delim) {
    if (s == NULL) {
        setError(ERR_NULL_POINTER);
        return (TString){0};
    }

    clearError();
    size_t totalSize = 0;
    for (size_t i = 0; i < count; ++i) {
        totalSize += s[i].size;
        if (i + 1 < count) totalSize += delim.size;
    }

    TString res = stringInit(totalSize);
    if (isError()) return (TString){0};

    size_t ind = 0;
    for (size_t i = 0; i < count; ++i) {
        for (size_t j = 0; j < s[i].size; ++j) {
            res.data[ind++] = s[i].data[j];
        }
        if (i + 1 < count) {
            for (size_t j = 0; j < delim.size; ++j) {
                res.data[ind++] = delim.data[j];
            }
        }
    }
    res.size = totalSize;
    return res;
}

TString stringArrJoinCharArr(const TString *s, size_t count, const char *delim) {
    if (s == NULL || delim == NULL) {
        setError(ERR_NULL_POINTER);
        return (TString){0};
    }

    clearError();
    size_t totalSize = 0;
    size_t delimSize = stringLenCharArr(delim);
    for (size_t i = 0; i < count; ++i) {
        totalSize += s[i].size;
        if (i + 1 < count) totalSize += delimSize;
    }

    TString res = stringInit(totalSize);
    if (isError()) return (TString){0};

    size_t ind = 0;
    for (size_t i = 0; i < count; ++i) {
        for (size_t j = 0; j < s[i].size; ++j) {
            res.data[ind++] = s[i].data[j];
        }
        if (i + 1 < count) {
            for (size_t j = 0; j < delimSize; ++j) {
                res.data[ind++] = delim[j];
            }
        }
    }
    res.size = totalSize;
    return res;
}

TString stringSubstring(TString s, size_t pos, size_t len) {
    clearError();
    TString res = stringInit(len);
    if (isError()) return (TString){0};
    for (size_t i = 0; i < len && pos + i < s.size; ++i) {
        res.data[i] = s.data[pos + i];
    }
    res.size = len;
    return res;
}

char* stringConvertToCharArr(TString s) {
    if (s.data == NULL)
        return NULL;

    clearError();
    char *res = (char *)malloc(s.size + 1);

    if (res == NULL) {
        setError(ERR_NULL_POINTER);
        return NULL;
    }
    memcpy(res, s.data, s.size * sizeof(char));
    res[s.size] = '\0';
    return res;
}

void stringScan(TString *s) {
    if (s == NULL) {
        setError(ERR_NULL_POINTER);
        return;
    }
    clearError();
    stringDestroy(s);
    char c = getchar();
    while ((c == ' ' || c == '\n' || c == '\t') && c != EOF) {
        c = getchar();
    }
    if (c == EOF) {
        setError(ERR_EOF_NOT_EXPECTED);
        return;
    }

    while (c != EOF &&
           c != '\n' &&
           c != ' ' &&
           c != '\t') {
        stringPushBack(s, c);
        if (isError()) return;
        c = getchar();
    }
}

void stringPrint(TString s) {
    if (s.data == NULL) return;
    for (size_t i = 0; i < s.size; ++i) {
        putchar(s.data[i]);
    }
}

void stringDebug(TString s) {
    if (s.data == NULL) {
        printf("[NULL, size = %zu, cap = %zu]\n", s.size, s.capacity);
        return;
    } else {
        printf("[");
        stringPrint(s);
        printf(", size = %zu, cap = %zu]\n", s.size, s.capacity);
    }
}

void stringRemoveChar(TString *s, char c) {
    size_t newSize = 0;
    for (size_t i = 0; i < s->size; ++i) {
        if (s->data[i] != c) {
            s->data[newSize] = s->data[i];
            ++newSize;
        }
    }
    s->size = newSize;
}

void stringSwap(TString *s1, TString *s2) {
    if (s1 == NULL || s2 == NULL) {
        setError(ERR_NULL_POINTER);
        return;
    }
    TString tmp = *s1;
    *s1 = *s2;
    *s2 = tmp;
}

void stringPushBack(TString *s, char c) {
    if (s == NULL) {
        setError(ERR_NULL_POINTER);
        return;
    }

    clearError();
    if (s->data == NULL) {
        *s = stringInit(10);
        if (isError()) return;
    }

    if (s->size >= s->capacity) {
        stringIncreaseCap(s);
    }

    s->data[s->size] = c;
    s->size++;
}

void stringPushFront(TString *s, char c) {
    stringReverse(s);
    stringPushBack(s, c);
    stringReverse(s);
}

void stringPopBack(TString *s) {
    if (s == NULL) {
        setError(ERR_NULL_POINTER);
        return;
    }
    if (stringIsEmpty(*s)) {
        setError(ERR_EMPTY_STRING_POP);
        return;
    }
    --(s->size);
}

void stringPopFront(TString *s) {
    if (s == NULL) {
        setError(ERR_NULL_POINTER);
        return;
    }
    if (stringIsEmpty(*s)) {
        setError(ERR_EMPTY_STRING_POP);
        return;
    }
    for (size_t i = 1; i < s->size; ++i) {
        s->data[i - 1] = s->data[i];
    }
    --(s->size);
}

void stringTrimLeft(TString *s) {
    if (s == NULL || s->size == 0) return;

    size_t start = 0;
    while (start < s->size && stringCharOneOf(s->data[start], " \t\n")) {
        ++start;
    }

    for (size_t i = 0; i + start < s->size; ++i) {
        s->data[i] = s->data[i + start];
    }
    s->size -= start;
}

void stringTrimRight(TString *s) {
    if (s == NULL || s->size == 0) return;

    int end = s->size - 1;
    while (end >= 0 && stringCharOneOf(s->data[end], " \t\n")) {
        --end;
    }
    s->size = (end + 1);
}

void stringTrim(TString *s) {
    stringTrimRight(s);
    stringTrimLeft(s);
}

void stringPadRight(TString *s, size_t newLen, char padChar) {
    if (s == NULL) {
        setError(ERR_NULL_POINTER);
        return;
    }
    if (s->size >= newLen) return;
    while (s->size < newLen) {
        clearError();
        stringPushBack(s, padChar);
        if (isError()) return;
    }
    s->size = newLen;
}

void stringPadLeft(TString *s, size_t newLen, char padChar) {
    if (s == NULL) {
        setError(ERR_NULL_POINTER);
        return;
    }
    if (s->size >= newLen) return;
    stringReverse(s);
    stringPadRight(s, newLen, padChar);
    stringReverse(s);
}

void stringReplaceAll(TString *s, const char *oldS, const char *newS) {
    if (s == NULL || s->size == 0 || newS == NULL || stringFindFirstCharArr(*s, oldS) < 0) return;

    TString res = {0};
    size_t pos = 0;
    TString copy = stringCopy(*s);
    size_t newSubstrLen = stringLenCharArr(newS);
    size_t oldSubstrLen = stringLenCharArr(oldS);
    clearError();
    while (pos != s->size) {
        int64_t substrPos = stringFindFirstCharArr(copy, oldS);

        if (substrPos < 0) {
            for (size_t i = pos; i < s->size; ++i) {
                stringPushBack(&res, s->data[i]);
                if (isError()) {
                    stringDestroy(&res);
                    return;
                }
            }
            break;
        }

        for (int64_t i = 0; i < substrPos; ++i) {
            stringPushBack(&res, copy.data[i]);
            if (isError()) {
                stringDestroy(&res);
                return;
            }
        }

        for (size_t i = 0; i < newSubstrLen; ++i) {
            stringPushBack(&res, newS[i]);
            if (isError()) {
                stringDestroy(&res);
                return;
            }
        }
        pos += oldSubstrLen + substrPos;
        copy.data += oldSubstrLen + substrPos;
        copy.size -= oldSubstrLen + substrPos;
        copy.capacity -= oldSubstrLen + substrPos;
    }
    stringDestroy(s);
    *s = res;
}

void stringToUpper(TString *s) {
    if (s == NULL || s->size == 0) return;
    for (size_t i = 0; i < s->size; ++i) {
        s->data[i] = stringCharToUpper(s->data[i]);
    }
}

void stringToLower(TString *s) {
    if (s == NULL || s->size == 0) return;
    for (size_t i = 0; i < s->size; ++i) {
        s->data[i] = stringCharToLower(s->data[i]);
    }
}

void stringReverse(TString *s) {
    if (s == NULL || s->size == 0) return;
    for (size_t i = 0; i < (s->size) / 2; ++i) {
        char tmp = s->data[i];
        s->data[i] = s->data[s->size - i - 1];
        s->data[s->size - i - 1] = tmp;
    }
}

void stringFilter(TString *s, bool (*predicate)(char)) {
    if (s == NULL || predicate == NULL) {
        setError(ERR_NULL_POINTER);
        return;
    }
    size_t i = 0;
    size_t j = 0;

    while (j < s->size) {
        if (predicate(s->data[j])) {
            s->data[i] = s->data[j];
            ++i;
        }
        ++j;
    }

    s->size = i;
}

void stringMap(TString *s, char (*func)(char)) {
    if (s == NULL || func == NULL) {
        setError(ERR_NULL_POINTER);
        return;
    }
    for (size_t i = 0; i < s->size; ++i) {
        s->data[i] = func(s->data[i]);
    }
}

void stringMapIndex(TString *s, char (*func)(size_t, char)) {
    if (s == NULL || func == NULL) {
        setError(ERR_NULL_POINTER);
        return;
    }
    for (size_t i = 0; i < s->size; ++i) {
        s->data[i] = func(i, s->data[i]);
    }
}

void stringRemove(TString *s, size_t pos, size_t len) {
    if (s == NULL) {
        setError(ERR_NULL_POINTER);
        return;
    }

    if (pos >= s->size) return;
    if (len > s->size - pos) len = s->size - pos;
    for (size_t i = pos; i + len < s->size; ++i) {
        s->data[i] = s->data[i + len];
    }
    s->size -= len;
}

void stringDestroy(TString *s) {
    if (s == NULL) return;
    free(s->data);
    *s = (TString){0};
}

bool isSeparator(char symbol) {
    return symbol == ' ' || symbol == '\t' || symbol == '\n' || symbol == ',' || symbol == '.';
}

void stringCapitalize(TString *s) {
    if (s == NULL || s->size == 0) {
        return;
    }
    if ('a' <= s->data[0] && s->data[0] <= 'z') {
        s->data[0] = stringCharToUpper(s->data[0]);
    }
    for (size_t i = 1; i < s->size; ++i) {
        if (isSeparator(s->data[i - 1]) && 'a' <= s->data[i] && s->data[i] <= 'z') {
            s->data[i] = stringCharToUpper(s->data[i]);
        }
    }
}

double stringToDouble(TString s) {
    double number = 0;
    double decimal = 0;
    bool negative = false;
    size_t i = 0;
    if (s.data[0] == '-') {
        negative = true;
        i = 1;
    }
    for (; i < s.size && s.data[i] != '.'; ++i) {
        if (stringCharIsDigit(s.data[i])) {
            number *= 10.0;
            number += (double)(s.data[i] - '0');
        } else {
            setError(ERR_INVALID_NUMBER_REPR);
            return 0;
        }
    }
    if (i < s.size && s.data[i] == '.') {
        i = s.size - 1;
        for (; s.data[i] != '.'; --i) {
            if (stringCharIsDigit(s.data[i])) {
                decimal += (double)(s.data[i] - '0');
                decimal /= 10;
            } else {
                setError(ERR_INVALID_NUMBER_REPR);
                return 0;
            }
        }
    }
    if (negative == true) {
        return -(number + decimal);
    }
    return number + decimal;
}

int64_t stringToInt(TString s) {
    int64_t result = 0;
    int sign = 1;
    size_t i = 0;
    
    while (i < s.size && s.data[i] == ' ') {
        i++;
    }
    
    if (i < s.size && s.data[i] == '-') {
        sign = -1;
        i++;
    } else if (i < s.size && s.data[i] == '+') {
        i++;
    }
    
    while (i < s.size && s.data[i] >= '0' && s.data[i] <= '9') {
        result = result * 10 + (s.data[i] - '0');
        i++;
    }

    return result * sign;
}

#endif
