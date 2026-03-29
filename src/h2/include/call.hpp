#ifndef _0CALL0_
#define _0CALL0_

/* comments: 
* the biggest disadvantage here is the primitive itself: exit;
* it does not allow any dynamic memory clean-up
* for a more control there is "exception.hpp"
*/

#define print_function_name(x) printf("\"%s\"", #x);

#define call(x) if(x < 0) { \
    printf("line %d error: ", __LINE__); print_function_name(x); \
    printf(" failed - %s.\n", strerror(errno)); \
    exit(EXIT_FAILURE); \
}

#define var_call(x) if(x < 0) { \
    printf("line %d error - %s\n", __LINE__, strerror(errno)); \
    exit(EXIT_FAILURE); \
}

#define call_noexit(x) if(x < 0) { \
    printf("line %d error: ", __LINE__); print_function_name(x); \
    printf(" failed - %s.\n", strerror(errno)); \
}

#endif
