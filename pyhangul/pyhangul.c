#include <Python.h>

#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>

#include "../hangul/hangul.h"

static PyObject *_pyhangul_error;

// User defined Object
typedef struct {
    PyObject_HEAD
} PY_HANGUL;

typedef struct {
    PyObject_HEAD

    HangulInputContext *hic;
} PY_HANGULIC;

extern PyTypeObject PY_HANGULIC_Type;

static PyObject *_create_ic(PY_HANGUL *self, PyObject *args)
{ 
    PY_HANGULIC *imObject;
    int keyboard;

    if(!PyArg_ParseTuple(args,"i",&keyboard)) {
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

void inithangul(void)
{
    PyObject *m, *d;

    m = Py_InitModule("hangul", _pyhangul_methods);
    PyModule_AddIntConstant(m, "hangul2", HANGUL_KEYBOARD_2);
    PyModule_AddIntConstant(m, "hangul32", HANGUL_KEYBOARD_32);
    PyModule_AddIntConstant(m, "hangul390", HANGUL_KEYBOARD_390);
    PyModule_AddIntConstant(m, "hangul3f", HANGUL_KEYBOARD_3FINAL);
    PyModule_AddIntConstant(m, "hangul3s", HANGUL_KEYBOARD_3NOSHIFT);
    //PyModule_AddIntConstant(m, "Hangul3_Yetgeul", HANGUL_KEYBOARD_3YETGUL);

    d = PyModule_GetDict(m);
    _pyhangul_error = PyErr_NewException("_pyhangul.error", NULL, NULL);
    PyDict_SetItemString(d, "error", _pyhangul_error);
} 

// im's member function
static PyObject *_pyhangulic_filter(PY_HANGULIC *self, PyObject *args)
{
    int ret;
    int ascii, capslock; 

    if(!PyArg_ParseTuple(args,"ii", &ascii, &capslock)) {
	PyErr_SetString(_pyhangul_error,"Usage: filter(ascii, capslock)");
	return NULL;
    }

    ret = hangul_ic_filter(self->hic, ascii, capslock);

    return Py_BuildValue("i", ret);
}

static PyObject *_pyhangulic_reset(PY_HANGULIC *self, PyObject *args)
{
    hangul_ic_reset(self->hic);

    return Py_None;
}

static PyObject *_pyhangulic_backspace(PY_HANGULIC *self, PyObject *args)
{
    int ret;

    ret = hangul_ic_backspace(self->hic);

    return Py_BuildValue("i", ret);
}

static PyObject *_pyhangulic_preedit_string(PY_HANGULIC *self, PyObject *args)
{
    int len;
    const wchar_t *str;

    str = hangul_ic_get_preedit_string(self->hic);
    len = wcslen(str);
    return PyUnicode_FromWideChar(str, len);
}

static PyObject *_pyhangulic_commit_string(PY_HANGULIC *self, PyObject *args)
{
    int len;
    const wchar_t *str;

    str = hangul_ic_get_commit_string(self->hic);
    len = wcslen(str);
    return PyUnicode_FromWideChar(str, len);
}

// PY_HANGULIC methods
static PyMethodDef PY_HANGULIC_methods[] = {
    { "filter",        (PyCFunction)_pyhangulic_filter,         METH_VARARGS, NULL},
    { "reset",         (PyCFunction)_pyhangulic_reset,          METH_VARARGS, NULL},
    { "backspace",     (PyCFunction)_pyhangulic_backspace,      METH_VARARGS, NULL},
    { "preedit_string",(PyCFunction)_pyhangulic_preedit_string, METH_VARARGS, NULL},
    { "commit_string", (PyCFunction)_pyhangulic_commit_string,  METH_VARARGS, NULL},
    { NULL, NULL, 0, NULL }
};

// PY_HANGULIC dealloc
static void PY_HANGULIC_dealloc(PY_HANGULIC *self)
{
    hangul_ic_delete(self->hic);
    self->hic = NULL;
    PyMem_Free((char *) self);
}

// PY_HANGULIC getattr
static PyObject * PY_HANGULIC_getattr(PY_HANGULIC *self, char *name)
{
    PyObject *res;
    res = Py_FindMethod(PY_HANGULIC_methods, (PyObject *)self, name);
    if(res != NULL)
	return res;
    PyErr_Clear();
    PyErr_SetString(_pyhangul_error,"UnKnown method");
    return NULL;
}

// PY_HANGULIC repr
static PyObject * PY_HANGULIC_repr(PY_HANGULIC *self)
{
    char buf[300];
    sprintf(buf,"<Class pyhangul at %lx>",(long)self);
    return PyString_FromString(buf);
}


// PY_HANGUL Type
PyTypeObject PY_HANGULIC_Type = {
#ifndef MS_WIN32
    PyObject_HEAD_INIT(&PyType_Type)
#else
    PyObject_HEAD_INIT(NULL)
#endif
	0,
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
