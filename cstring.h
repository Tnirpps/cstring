#ifndef CSTRING_LIB
#define CSTRING_LIB

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef enum EErrorCode {
    ERR_NO_ERROR,
    ERR_ALLOCATE_SPACE,
    ERR_NULL_POINTER,
    ERR_INVALID_STATE,
    ERR_BUFFER_OVERFLOW,
    ERR_NUMBER_OVERFLOW,
    ERR_EOF_NOT_EXPECTED,
} EErrorCode;

#define MAX_ERROR_MSG_LEN 300
static EErrorCode ERROR_CODE = ERR_NO_ERROR;
static char ERROR_BUF[MAX_ERROR_MSG_LEN] = {0};

int isError();
const char *getErrorMsg();
void clearError();


typedef struct TString {
    char *data;
    size_t size;
    size_t capacity;
} TString;


bool stringStartWith(TString s, TString pref);
bool stringStartWithCharArr(TString s, char *pref);
bool stringEndWith(TString s, TString pref);
bool stringEndWithCharArr(TString s, char *pref);
bool stringIsEqual(TString s1, TString s2);

size_t stringLen(TString s);

int stringCompare(TString s1, TString s2);

int64_t stringFindFirst(TString s, TString pattern);
int64_t stringFindFirstCharArr(TString s, const char *pattern);

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


void stringScan(TString *s);
void stringPrint(TString s);
void stringDebug(TString s);

// TODO:
/*
    void stringReduce(TString *s, int64_t (*f)(char, int64_t), int64_t init);
    TString stringMap(TString s, char (*f)(char));
*/

void stringPushBack(TString *s, char c);
void stringTrim(TString *s);
void stringReplaceAll(TString *s, const char *oldSub, const char *newSub);
void stringReverse(TString *s);
void stringDestroy(TString *s);

#endif

// for testing:
// #define CSTRING_IMPLEMENTATION

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

void clearError() {
    ERROR_CODE = ERR_NO_ERROR;
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
    const char *s2, size_t pos2, size_t len2) {
    if (len1 > len2) return -1 * stringCompSubstr(s2, pos2, len2, s1, pos1, len1);
    for (size_t i = 0; i < len1; ++i) {
        if (s1[pos1 + i] != s2[pos2 + i]) {
            if (s1[pos1 + 1] < s2[pos2 + i]) return 1;
            return -1;
        }
    }
    if (len1 != len2) return -1;
    return 0;
}

size_t stringLenCharArr(const char *s) {
    const char *begin = s;
    while (*s != '\0') ++s;
    return (size_t) (s - begin);
}

void stringIncreaseCap(TString *s) {
    assert(s != NULL);
    size_t newCap = s->capacity * 2;
    if (newCap == 0) newCap = 1;
    char *newData = (char *) malloc(sizeof(char) * newCap);      
    if (newData == NULL) {
        ERROR_CODE = ERR_ALLOCATE_SPACE;
        return;
    }

    swapPtr((void**)&s->data, (void**)&newData);
    stringCopyCharArr(s, newData, s->size);
    free(newData);
    s->capacity  = newCap;
}



// import 

bool stringStartWith(TString s, TString pref) {
    if (s.size < pref.size) return false;
    return stringCompSubstr(s.data, 0, pref.size, pref.data, 0, pref.size) == 0;
}

bool stringStartWithCharArr(TString s, char *pref) {
    if (pref == NULL) return true;
    size_t len = stringLenCharArr(pref);
    if (s.size < len) return false;
    return stringCompSubstr(s.data, 0, len, pref, 0, len) == 0;
}

bool stringEndWith(TString s, TString pref) {
    if (s.size < pref.size) return false;
    return stringCompSubstr(s.data, s.size - pref.size, pref.size, pref.data, 0, pref.size) == 0;
}

bool stringEndWithCharArr(TString s, char *pref) {
    if (pref == NULL) return true;
    size_t len = stringLenCharArr(pref);
    if (s.size < len) return false;
    return stringCompSubstr(s.data, s.size - len, len, pref, 0, len) == 0;
}

bool stringIsEqual(TString s1, TString s2) {
    return stringCompSubstr(s1.data, 0, s1.size, s2.data, 0, s2.size) == 0;
}

size_t stringLen(TString s) {
    return s.size;
}

int stringCompare(TString s1, TString s2) {
    return stringCompSubstr(s1.data, 0, s1.size, s2.data, 0, s2.size);
}

int64_t stringFindFirst(TString s, TString pattern) {
    int64_t res = 0;
    for (size_t i = 0; i < s.size; ++i) {
        int64_t match = 0;
        for (size_t j = 0; i + j < s.size && j < pattern.size; ++j) {
            if (s.data[i + j] != pattern.data[j]) break;
            ++match;
        }
        if (match == pattern.size) return i;
    }
    return -1;
}

int64_t stringFindFirstCharArr(TString s, const char *pattern) {
    int64_t res = 0;
    size_t patternLen = stringLenCharArr(pattern);
    for (size_t i = 0; i < s.size; ++i) {
        int64_t match = 0;
        for (size_t j = 0; i + j < s.size && j < patternLen; ++j) {
            if (s.data[i + j] != pattern[j]) break;
            ++match;
        }
        if (match == patternLen) return i;
    }
    return -1; 
}

TString stringInit(size_t capacity) {
    TString s = {0};
    s.data = (char *) malloc(sizeof(char) * capacity);
    if (s.data == NULL) {
        ERROR_CODE = ERR_ALLOCATE_SPACE;
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

TString stringArrJoinCharArr(const TString *s, size_t count, const char *delim) {
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

void stringScan(TString *s) {
    clearError();
    stringDestroy(s);
    char c = getchar();
    while ((c == ' ' || c == '\n' || c == '\t') && c != EOF) {
        c = getchar();
    }
    if (c == EOF) {
        ERROR_CODE = ERR_EOF_NOT_EXPECTED;
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

void stringPushBack(TString *s, char c) {
    if (s == NULL) {
        ERROR_CODE = ERR_NULL_POINTER;
        return;
    }

    clearError();
    if (s->data == NULL) {
        *s = stringInit(10);
        if (isError()) return;
    }

    assert(s->capacity >= s->size && s->data != NULL);

    if (s->size == s->capacity) {
        stringIncreaseCap(s);
    }

    s->data[s->size] = c;
    s->size++;
}

void stringTrim(TString *s) {
    if (s == NULL || s->data == NULL || s->size == 0) return;

    size_t start = 0;
    size_t end = s->size - 1;
    while (start < s->size && stringCharOneOf(s->data[start], " \t\n")) {
        ++start;
    }
    // if string contains only delimeters
    if (start == s->size) {
        s->size = 0;
        return;
    }

    while (end > 0 && stringCharOneOf(s->data[end], " \t\n")) {
        --end;
    }

    for (size_t i = 0; i + start < s->size; ++i) {
        s->data[i] = s->data[i + start];
    }
    s->size = (end - start + 1);
}

void stringReplaceAll(TString *s, const char *oldS, const char *newS) {
    if (s == NULL || s->data == NULL || stringFindFirstCharArr(*s, oldS) < 0) return;

    TString res = {0};
    size_t pos = 0;
    TString copy = stringCopy(*s);
    size_t newSubstrLen = stringLenCharArr(newS);
    size_t oldSubstrLen = stringLenCharArr(oldS);
    clearError();
    while (pos != s->size) {
        int64_t substrPos = stringFindFirstCharArr(copy, oldS);
        
        if (substrPos < 0)  {
            for (size_t i = pos; i < s->size; ++i) {
                stringPushBack(&res, s->data[i]);
                if (isError()) {
                    stringDestroy(&res);
                    return;
                }
            }
            break;
        }

        for (size_t i = 0; i < substrPos; ++i) {
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
        pos           += oldSubstrLen + substrPos;
        copy.data     += oldSubstrLen + substrPos;
        copy.size     -= oldSubstrLen + substrPos;
        copy.capacity -= oldSubstrLen + substrPos;
    }
    stringDestroy(s);
    *s = res;
}

void stringReverse(TString *s) {
    if (s == NULL || s->data == NULL || s->size == 0) return;
    for (size_t i = 0; i < (s->size) / 2; ++i) {
        char tmp = s->data[i];
        s->data[i] = s->data[s->size - i - 1];
        s->data[s->size - i - 1] = tmp;
    }
}

void stringDestroy(TString *s) {
    if (s == NULL) return;
    free(s->data);
    *s = (TString) {0};
}

#endif

