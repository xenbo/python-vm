
#include "python_vm.h"
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#ifdef _WIN32
#include <Windows.h>

typedef HINSTANCE HandleType;

#elif linux
#include <dlfcn.h>

typedef void* HandleType;
#define GetProcAddress dlsym

#endif

struct PythonVM {
	HandleType handle;
};

static void call_cpython_api_ret_void_param0(struct PythonVM* p, const char *pyFunctionName);
static void call_cpython_api_ret_void_param1_ptr(struct PythonVM* p, const char *pyFunctionName, void* param1);
static void call_cpython_api_ret_void_param2_ptr_ptr(struct PythonVM* p, const char *pyFunctionName, void* param1, void* param2);
static PyObject* call_cpython_api_ret_ptr_param0(struct PythonVM* p, const char *pyFunctionName);
static PyObject* call_cpython_api_ret_ptr_param1_ptr(struct PythonVM* p, const char *pyFunctionName, void* param1);
static PyObject* call_cpython_api_ret_ptr_param2_ptr_ptr(struct PythonVM* p, const char *pyFunctionName, void* param1, void* param2);


struct PythonVM*
pythonvm_create(const char* sLibName) {
	PythonVM* p = new struct PythonVM;
	if (p == NULL) {
		assert(0);
	}
	p->handle = NULL;

#ifdef _WIN32
	HINSTANCE handle = LoadLibrary(sLibName);
#elif linux
	//dlmopen segmentation fault
	//void *handle = dlmopen(LM_ID_NEWLM, sLibName, RTLD_NOW);
	void *handle = dlopen(sLibName, RTLD_NOW);
#endif
	if (handle == NULL) {
		assert(0);
	}

	p->handle = handle;
	return p;
}

void
pythonvm_destory(struct PythonVM* p) {
#ifdef _WIN32
	//If the function succeeds, the return value is nonzero.
	//If the function fails, the return value is zero.To get extended error information, call the GetLastError function.
	if (FreeLibrary(p->handle) == 0) {
		assert(0);
	}
#elif linux
	//The function dlclose() returns 0 on success, and nonzero on error.
	if (dlclose(p->handle) != 0) {
		assert(0);
	}
#endif
	delete p;
}

static void
call_cpython_api_ret_void_param0(struct PythonVM* p, const char *pyFunctionName) {
	typedef void* (*PyFunctionType)();
	PyFunctionType function = (PyFunctionType)GetProcAddress(p->handle, pyFunctionName);
	if (function == NULL) {
		assert(0);
	}
	function();
}

static void
call_cpython_api_ret_void_param1_ptr(struct PythonVM* p, const char *pyFunctionName, void* param1) {
	typedef void* (*PyFunctionType)(void*);
	PyFunctionType function = (PyFunctionType)GetProcAddress(p->handle, pyFunctionName);
	if (function == NULL) {
		assert(0);
	}
	function(param1);
}

static void
call_cpython_api_ret_void_param2_ptr_ptr(struct PythonVM* p, const char *pyFunctionName, void* param1, void* param2) {
	typedef void* (*PyFunctionType)(void*, void*);
	PyFunctionType function = (PyFunctionType)GetProcAddress(p->handle, pyFunctionName);
	if (function == NULL) {
		assert(0);
	}
	function(param1, param2);
}

static PyObject*
call_cpython_api_ret_ptr_param0(struct PythonVM* p, const char *pyFunctionName) {
	typedef PyObject* (*PyFunctionType)();
	PyFunctionType function = (PyFunctionType)GetProcAddress(p->handle, pyFunctionName);
	if (function == NULL) {
		assert(0);
	}
	return function();
}

static PyObject*
call_cpython_api_ret_ptr_param1_ptr(struct PythonVM* p, const char *pyFunctionName, void* param1) {
	typedef PyObject* (*PyFunctionType)(void*);
	PyFunctionType function = (PyFunctionType)GetProcAddress(p->handle, pyFunctionName);
	if (function == NULL) {
		assert(0);
	}
	return function(param1);
}

static PyObject*
call_cpython_api_ret_ptr_param2_ptr_ptr(struct PythonVM* p, const char *pyFunctionName, void* param1, void* param2) {
	typedef PyObject* (*PyFunctionType)(void*, void*);
	PyFunctionType function = (PyFunctionType)GetProcAddress(p->handle, pyFunctionName);
	if (function == NULL) {
		assert(0);
	}
	return function(param1, param2);
}

void
pythonvm__Py_Initialize(struct PythonVM* p) {
	call_cpython_api_ret_void_param0(p, "Py_Initialize");
	call_cpython_api_ret_void_param0(p, "PyEval_InitThreads");
}

void
pythonvm__Py_FinalizeEx(struct PythonVM* p) {
	call_cpython_api_ret_void_param0(p, "Py_FinalizeEx");
}

void 
pythonvm__PyRun_SimpleString(struct PythonVM* p, const char *command) {
	//int PyRun_SimpleFile(FILE *fp, const char *filename)
	//PyRun_SimpleString: https://docs.python.org/3.6/c-api/veryhigh.html#c.PyRun_SimpleFile
	call_cpython_api_ret_void_param1_ptr(p, "PyRun_SimpleString", (void*)command);
}

PyObject*
pythonvm__PyImport_ImportModule(struct PythonVM* p, const char* sModuleName) {
	//PyObject* PyImport_ImportModule(const char *name)
	//PyImport_ImportModule: https://docs.python.org/3.6/c-api/import.html?highlight=pyimport_importmodule#c.PyImport_ImportModule
	return call_cpython_api_ret_ptr_param1_ptr(p, "PyImport_ImportModule", (void*)sModuleName);
}

PyObject*
pythonvm__PyObject_GetAttrString(struct PythonVM* p, const char* sFunctionName) {
	//PyObject* PyObject_GetAttrString(PyObject *o, const char *attr_name)
	//PyObject_GetAttrString: https://docs.python.org/3.6/c-api/object.html?highlight=pyobject_getattrstring#c.PyObject_GetAttrString
	return call_cpython_api_ret_ptr_param1_ptr(p, "PyObject_GetAttrString", (void*)sFunctionName);
}

PyObject* 
pythonvm__PyObject_CallObject(struct PythonVM* p, PyObject *callable_object, PyObject *args) {
	//PyObject* PyObject_CallObject(PyObject *callable_object, PyObject *args)
	//PyObject_CallObject: https://docs.python.org/3.6/c-api/object.html?highlight=pyobject_callobject#c.PyObject_CallObject
	return call_cpython_api_ret_ptr_param2_ptr_ptr(p, "PyObject_CallObject", (void*)callable_object, (void*)args);
}