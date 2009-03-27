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
