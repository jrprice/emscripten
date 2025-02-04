// Copyright 2015 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

void *ThreadMain(void *arg)
{
	pthread_attr_t attr;
	int rc;
	void *stbase;
	size_t stsize;
	int dummy, result;

	rc = pthread_attr_init(&attr);
	assert(rc == 0);

	rc = pthread_getattr_np(pthread_self(), &attr);
	assert(rc == 0);

	rc = pthread_attr_getstack(&attr, &stbase, &stsize);
	assert(rc == 0);

	//printf ("%p %p %p\n", stbase, (char*)stbase + stsize, &dummy);

	if (&dummy < stbase)
		result = 1;
	else if ((char*)&dummy > (char*)stbase + stsize)
		result = 2;
	else
		result = 0;

	pthread_exit((void*)result);
}

int main()
{
	pthread_t thread;
	int rc, result;

	rc = pthread_create(&thread, NULL, ThreadMain, NULL);
	assert(rc == 0);

	rc = pthread_join(thread, (void**)&result);
	assert(rc == 0);

	assert(result == 0);
	return 0;
}
