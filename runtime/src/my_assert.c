
/**
 * @file   my_assert.cc
 *
 * @brief  .
 *
 * @author Jean-Francois FABRE.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <assert.h>
#endif

#include "my_assert.h"

// get him officer, he was trying to violate a segmentation

void assert_failed(const char *e,const char *f,const int l)
{
    // -----> WELCOME TO THE "ASSERTION FAILED" TRAP <-----
    
    // you have reached this point because the coder (or more likely
    // yourself) failed to code some feature and was catched just
    // before the segmentation violation or worse.
    //
    // Now, in order to get out from the pit, use "stack up one frame"
    // as many times as necessary to reach a context you know of

    char buf[1000];
    
    sprintf(buf,"%s:%d: failed assertion `%s`",f,l,e);
    fprintf(stderr,"%s\n",buf);
#ifdef WIN32
    MessageBox(NULL,buf,"assertion failed",MB_OK);
    exit(1);
#else
    assert(0);
#endif
}
