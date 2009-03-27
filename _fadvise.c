/*
 * Python interface to posix_fadvise()
 * Copyright (C) Chris Lamb <chris@chris-lamb.co.uk>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <Python.h>

#define _XOPEN_SOURCE 600
#include <fcntl.h>

static PyObject *
method_posix_fadvise(PyObject *self, PyObject *args)
{
	int fd, advice, ret;
	off_t offset, len;

	if (!PyArg_ParseTuple(args, "iLLi", &fd, &offset, &len, &advice))
		return NULL;

	Py_BEGIN_ALLOW_THREADS;
	ret = posix_fadvise(fd, (off_t)offset, (off_t)len, advice);
	Py_END_ALLOW_THREADS;

	if (ret) {
		PyErr_SetFromErrno(PyExc_OSError);
		return NULL;
	}

	Py_RETURN_NONE;
}

static PyMethodDef PosixFadviseMethods[] = {
	{"posix_fadvise", method_posix_fadvise, METH_VARARGS,
		"posix_fadvise(fd, offset, len, advice)"},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
init_fadvise(void)
{
	PyObject *m = Py_InitModule("_fadvise", PosixFadviseMethods);

	PyObject *d = PyModule_GetDict(m);

	PyModule_AddIntConstant(m, "POSIX_FADV_NORMAL", POSIX_FADV_NORMAL);
	PyModule_AddIntConstant(m, "POSIX_FADV_RANDOM", POSIX_FADV_RANDOM);
	PyModule_AddIntConstant(m, "POSIX_FADV_SEQUENTIAL", POSIX_FADV_SEQUENTIAL);
	PyModule_AddIntConstant(m, "POSIX_FADV_WILLNEED", POSIX_FADV_WILLNEED);
	PyModule_AddIntConstant(m, "POSIX_FADV_DONTNEED", POSIX_FADV_DONTNEED);
	PyModule_AddIntConstant(m, "POSIX_FADV_NOREUSE", POSIX_FADV_NOREUSE);
}
