#include <Python.h>

#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>
#include <alloca.h>

#include <hangul.h>

static PyObject *_pyhangul_error;

/* User defined Object */
typedef struct {
    PyObject_HEAD
} PY_HANGUL;

typedef struct {
    PyObject_HEAD

    HangulInputContext *hic;
} PY_HANGULIC;

extern PyTypeObject PY_HANGULIC_Type;

static int ucscharlen(const ucschar *str)
{
    const ucschar *end = str;
    while (*end != 0)
	end++;
    return end - str;
} 

static PyObject *_create_ic(PY_HANGUL *self, PyObject *args)
{ 
    PY_HANGULIC *imObject;
    const char* keyboard = NULL;

    if(!PyArg_ParseTuple(args,"s",&keyboard)) {
	PyErr_SetString(_pyhangul_error,
			"Usage: create_ic(keyboard)\n"
			"\tkeyboard: hangul2, hangul3{2,90,f,s}");
	return NULL;
    }

    imObject = PyObject_NEW(PY_HANGULIC, &PY_HANGULIC_Type);
    if(imObject == NULL) {
	PyErr_SetString(_pyhangul_error,"Fail to create PY_HANGULIC Object");
	return NULL;
    }

    imObject->hic = hangul_ic_new(keyboard);

    return (PyObject *)imObject;
}

static PyMethodDef _pyhangul_methods[] = {
    { "create_ic", (PyCFunction) _create_ic, METH_VARARGS, NULL },
    { NULL,      NULL, 0, NULL } 
};

#if PY_MAJOR_VERSION >= 3
static int _pyhangul_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(_pyhangul_error);
    return 0;
}

static int _pyhangul_clear(PyObject *m) {
    Py_CLEAR(_pyhangul_error);
    return 0;
}

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "hangul",
    NULL,
    sizeof(_pyhangul_error),
    _pyhangul_methods,
    NULL,
    _pyhangul_traverse,
    _pyhangul_clear,
    NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_hangul(void)

#else

#define INITERROR return

PyMODINIT_FUNC
inithangul(void)
#endif /* PY_MAJOR_VERSION */
{
    PyObject *m, *d;

#if PY_MAJOR_VERSION >= 3
    m = PyModule_Create(&moduledef);
#else
    m = Py_InitModule("hangul", _pyhangul_methods);
#endif /* PY_MAJOR_VERSION */
    if (m == NULL)
        INITERROR;

    _pyhangul_error = PyErr_NewException("hangul.error", NULL, NULL);
    if (_pyhangul_error == NULL) {
        Py_DECREF(m);
        INITERROR;
    }
    d = PyModule_GetDict(m);

    PyDict_SetItemString(d, "error", _pyhangul_error);

#if PY_MAJOR_VERSION >= 3
    return m;
#endif /* PY_MAJOR_VERSION */
} 

/* im's member function */
static PyObject *_pyhangulic_process(PY_HANGULIC *self, PyObject *args)
{
    int ret;
    int ascii; 

    if(!PyArg_ParseTuple(args,"i", &ascii)) {
	PyErr_SetString(_pyhangul_error,"Usage: process(ascii)");
	return NULL;
    }

    ret = hangul_ic_process(self->hic, ascii);

    return Py_BuildValue("i", ret);
}

static PyObject *_pyhangulic_reset(PY_HANGULIC *self, PyObject *args)
{
    hangul_ic_reset(self->hic);

    return Py_None;
}

static PyObject *_pyhangulic_flush(PY_HANGULIC *self, PyObject *args)
{
#ifndef Py_UNICODE_WIDE
    int i;
    Py_UNICODE *buf;
#endif /* !Py_UNICODE_WIDE */
    int len;
    const ucschar *str;

    str = hangul_ic_flush(self->hic);
    len = ucscharlen(str);

#ifdef Py_UNICODE_WIDE
    return PyUnicode_FromUnicode((Py_UNICODE*)str, len);
#else  /* Py_UNICODE_WIDE */
    buf = alloca(sizeof(Py_UNICODE) * len);
    for (i = 0; i < len; i++)
	buf[i] = str[i];
    return PyUnicode_FromUnicode(buf, len);
#endif /* Py_UNICODE_WIDE */
}

static PyObject *_pyhangulic_backspace(PY_HANGULIC *self, PyObject *args)
{
    int ret;

    ret = hangul_ic_backspace(self->hic);

    return Py_BuildValue("i", ret);
}

static PyObject *_pyhangulic_preedit_string(PY_HANGULIC *self, PyObject *args)
{
#ifndef Py_UNICODE_WIDE
    int i;
    Py_UNICODE *buf;
#endif /* !Py_UNICODE_WIDE */
    int len;
    const ucschar *str;

    str = hangul_ic_get_preedit_string(self->hic);
    len = ucscharlen(str);

#ifdef Py_UNICODE_WIDE
    return PyUnicode_FromUnicode((Py_UNICODE*)str, len);
#else  /* Py_UNICODE_WIDE */
    buf = alloca(sizeof(Py_UNICODE) * len);
    for (i = 0; i < len; i++)
	buf[i] = str[i];
    return PyUnicode_FromUnicode(buf, len);
#endif /* Py_UNICODE_WIDE */
}

static PyObject *_pyhangulic_commit_string(PY_HANGULIC *self, PyObject *args)
{
#ifndef Py_UNICODE_WIDE
    int i;
    Py_UNICODE *buf;
#endif /* !Py_UNICODE_WIDE */
    int len;
    const ucschar *str;

    str = hangul_ic_get_commit_string(self->hic);
    len = ucscharlen(str);

#ifdef Py_UNICODE_WIDE
    return PyUnicode_FromUnicode((Py_UNICODE*)str, len);
#else  /* Py_UNICODE_WIDE */
    buf = alloca(sizeof(Py_UNICODE) * len);
    for (i = 0; i < len; i++)
	buf[i] = str[i];
    return PyUnicode_FromUnicode(buf, len);
#endif /* Py_UNICODE_WIDE */
}

/* PY_HANGULIC methods */
static PyMethodDef PY_HANGULIC_methods[] = {
    { "process",       (PyCFunction)_pyhangulic_process,        METH_VARARGS, NULL},
    { "reset",         (PyCFunction)_pyhangulic_reset,          METH_VARARGS, NULL},
    { "flush",         (PyCFunction)_pyhangulic_flush,          METH_VARARGS, NULL},
    { "backspace",     (PyCFunction)_pyhangulic_backspace,      METH_VARARGS, NULL},
    { "preedit_string",(PyCFunction)_pyhangulic_preedit_string, METH_VARARGS, NULL},
    { "commit_string", (PyCFunction)_pyhangulic_commit_string,  METH_VARARGS, NULL},
    { NULL, NULL, 0, NULL }
};

/* PY_HANGULIC dealloc */
static void PY_HANGULIC_dealloc(PY_HANGULIC *self)
{
    hangul_ic_delete(self->hic);
    self->hic = NULL;
}

/* PY_HANGULIC getattr */
static PyObject * PY_HANGULIC_getattr(PY_HANGULIC *self, char *name)
{
    PyObject *res;
#if PY_MAJOR_VERSION >= 3
    PyObject *nameobj = PyUnicode_FromString(name);
    // try to find handler manually
    for (int i = 0; PY_HANGULIC_methods[i].ml_name != NULL; ++i) {
        if (strcmp(name, PY_HANGULIC_methods[i].ml_name) == 0) {
            PyObject *result = PyCFunction_New(&PY_HANGULIC_methods[i], (PyObject *)self);
            if (result == NULL)
                result = Py_None;
            Py_INCREF(result);
            return result;
        }
    }

    res = PyObject_GenericGetAttr((PyObject *)self, nameobj);
#else
    res = Py_FindMethod(PY_HANGULIC_methods, (PyObject *)self, name);
#endif /* PY_MAJOR_VERSION */
    if(res != NULL)
	return res;
    PyErr_Clear();
    PyErr_SetString(_pyhangul_error,"UnKnown method");
    return NULL;
}

/* PY_HANGULIC repr */
static PyObject * PY_HANGULIC_repr(PY_HANGULIC *self)
{
    char buf[300];
    sprintf(buf,"<Class pyhangul at %lx>",(long)self);
#if PY_MAJOR_VERSION >= 3
    return PyUnicode_FromString(buf);
#else
    return PyString_FromString(buf);
#endif /* PY_MAJOR_VERSION */
}


/* PY_HANGUL Type */
PyTypeObject PY_HANGULIC_Type = {
#if PY_MAJOR_VERSION >= 3
    PyVarObject_HEAD_INIT(NULL, 0)
#else
#ifndef MS_WIN32
    PyObject_HEAD_INIT(&PyType_Type)
#else
    PyObject_HEAD_INIT(NULL)
#endif
    0,
#endif /* PY_MAJOR_VERSION */
    "hangul.hangulic",
    sizeof(PY_HANGULIC),
    0,
    (destructor)PY_HANGULIC_dealloc,
    0,
    (getattrfunc)PY_HANGULIC_getattr,
    0,
    0,
    (reprfunc)PY_HANGULIC_repr,
};
