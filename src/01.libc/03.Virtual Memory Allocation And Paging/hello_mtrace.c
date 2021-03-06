#include <mcheck.h>
#include <stdlib.h>

// void mtrace (void)
// void muntrace (void)

// When the [mtrace] function is called it looks for an environment variable named
// [MALLOC_TRACE]. This variable is supported to contain a valid file name. The 
// user must have write access. If the file already exists it is truncated. If 
// the environment variable is not set or it does not name a valid file which 
// can be opened for writing nothing is done. The behavior of malloc etc. is 
// not changed. 

// The muntrace function can be called after mtrace was used to enable tracing 
// the malloc calls. If no (successful) call of mtrace was made muntrace does nothing.

// Otherwise it deinstalls the handlers for malloc, realloc, and free and then 
// closes the protocol file. No calls are protocolled anymore and the program runs 
// again at full speed.

// This is all that is needed if you want to trace the calls during the whole runtime
// of the program. Alternatively you can stop the tracing at any time with a call to 
// muntrace. It is even possible to restart the tracing again with a new call to mtrace. 
// But this can cause unreliable results since there may be calls of the functions which
// are not called. Please note that not only the application uses the traced functions,
// also libraries (including the C library itself) use these functions.

#define SIZE 256

void mtrace_usage(void)
{
	char *buf;

#ifdef DEBUGGING		// gcc -DDEBUGGING -o test test.c
	mtrace();		/* enable tracing */
#endif

	buf = (char *)malloc(SIZE);
	free(buf);

	// This last point is also why it is not a good idea to call [muntrace] before the program
	// terminates. The libraries are informed about the termination of the program only after
	// the program returns from main or calls exit and so cannot free the memory they use 
	// before this time.

#ifdef DEBUGGING
	muntrace();		/* disable tracing */
#endif
}

int main(void)
{
	mtrace_usage();
	return 0;
}

/*

$ MALLOC_TRACE=/tmp/malloc_trace.log ./test 
$ cat malloc_trace.log 
= Start
@ ./test:[0x563418c1a787] + 0x563419f35450 0x100
@ ./test:[0x563418c1a797] - 0x563419f35450
= End


*/

// https://www.gnu.org/software/libc/manual/html_node/Tracing-malloc.html#Tracing-malloc
// https://www.gnu.org/software/libc/manual/html_node/Using-the-Memory-Debugger.html#Using-the-Memory-Debugger
// https://www.gnu.org/software/libc/manual/html_node/Tips-for-the-Memory-Debugger.html#Tips-for-the-Memory-Debugger
// https://www.gnu.org/software/libc/manual/html_node/Interpreting-the-traces.html#Interpreting-the-traces
