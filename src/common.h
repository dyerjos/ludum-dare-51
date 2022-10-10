#include <stdio.h>
#include <stdlib.h>

#ifndef ASSERT_ENABLED
#define ASSERT_ENABLED 1
#endif

#if ASSERT_ENABLED
#define ASSERT(c, m) \
do { \
    if (!(c)) { \
        fprintf(stderr, __FILE__ ":%d: assertion %s failed: %s\n", \
                        __LINE__, #c, m); \
exit(1);  }\
} while(0)
#else
#define ASSERT(c, m)  // Empty macro if not enabled
#endif

// FREE(&x)
#define FREE(ptrptr) do { \
	__typeof__(ptrptr) const __x = (ptrptr); \
	free(*__x); *__x = NULL; \
} while(0)

#define HERE() fprintf(stderr, "%s:%d\n", __FILE__, __LINE__)

#define UNUSED(x) ((void)(x))

#define numberof(x) (sizeof(x) / sizeof(*(x)))

#define STR_LEN(str) (str), (sizeof(str)-1)

#ifndef NULL
#define NULL (0)
#endif
