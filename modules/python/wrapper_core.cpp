
/* ************************************************************************ */

// Declaration
#include "wrapper_core.hpp"

// Python
#include <Python.h>

// Core
#include "core/Vector.hpp"

// Module
#include "wrapper.hpp"

/* ************************************************************************ */

using namespace module::python;

/* ************************************************************************ */

static PyObject* vectorFloat_getX(ObjectWrapper<core::Vector<float>>* self, void* closure)
{
    assert(self);
    const float x = self->value.getX();

    return Py_BuildValue("f", x);
}

/* ************************************************************************ */

static int vectorFloat_setX(ObjectWrapper<core::Vector<float>>* self, PyObject* value, void* closure)
{
    if (value == nullptr)
    {
        PyErr_SetString(PyExc_TypeError, "X value cannot be NULL");
        return -1;
    }

    if (!PyFloat_Check(value))
    {
        PyErr_SetString(PyExc_TypeError, "X value must be float");
        return -1;
    }

    assert(self);
    self->value.setX(PyFloat_AsDouble(value));

    return 0;
}

/* ************************************************************************ */

static PyObject* vectorFloat_getY(ObjectWrapper<core::Vector<float>>* self, void* closure)
{
    assert(self);
    const float y = self->value.getY();

    return Py_BuildValue("f", y);
}

/* ************************************************************************ */

static int vectorFloat_setY(ObjectWrapper<core::Vector<float>>* self, PyObject* value, void* closure)
{
    if (value == nullptr)
    {
        PyErr_SetString(PyExc_TypeError, "X value cannot be NULL");
        return -1;
    }

    if (!PyFloat_Check(value))
    {
        PyErr_SetString(PyExc_TypeError, "X value must be float");
        return -1;
    }

    assert(self);
    self->value.setY(PyFloat_AsDouble(value));

    return 0;
}

/* ************************************************************************ */

static PyGetSetDef g_vectorFloatGettersSeters[] = {
    {"x", (getter)vectorFloat_getX, (setter)vectorFloat_setX, nullptr, nullptr },
    {"y", (getter)vectorFloat_getY, (setter)vectorFloat_setY, nullptr, nullptr },
    {"width", (getter)vectorFloat_getX, (setter)vectorFloat_setX, nullptr, nullptr },
    {"height", (getter)vectorFloat_getY, (setter)vectorFloat_setY, nullptr, nullptr },
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

static void python_wrapper_core_VectorFloat(PyObject* module)
{
    using type_def = TypeDefinition<core::Vector<float>>;

    type_def::init("core.VectorFloat");
    type_def::definition.tp_getset = g_vectorFloatGettersSeters;
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "VectorFloat", reinterpret_cast<PyObject*>(&type_def::definition));
}

/* ************************************************************************ */

static PyObject* vectorInt_getX(ObjectWrapper<core::Vector<int>>* self, void* closure)
{
    assert(self);
    const int x = self->value.getX();

    return Py_BuildValue("i", x);
}

/* ************************************************************************ */

static int vectorInt_setX(ObjectWrapper<core::Vector<int>>* self, PyObject* value, void* closure)
{
    if (value == nullptr)
    {
        PyErr_SetString(PyExc_TypeError, "X value cannot be NULL");
        return -1;
    }

    if (!PyLong_Check(value))
    {
        PyErr_SetString(PyExc_TypeError, "X value must be int");
        return -1;
    }

    assert(self);
    self->value.setX(PyLong_AsLong(value));

    return 0;
}

/* ************************************************************************ */

static PyObject* vectorInt_getY(ObjectWrapper<core::Vector<int>>* self, void* closure)
{
    assert(self);
    const int y = self->value.getY();

    return Py_BuildValue("i", y);
}

/* ************************************************************************ */

static int vectorInt_setY(ObjectWrapper<core::Vector<int>>* self, PyObject* value, void* closure)
{
    if (value == nullptr)
    {
        PyErr_SetString(PyExc_TypeError, "X value cannot be NULL");
        return -1;
    }

    if (!PyLong_Check(value))
    {
        PyErr_SetString(PyExc_TypeError, "X value must be float");
        return -1;
    }

    assert(self);
    self->value.setY(PyLong_AsLong(value));

    return 0;
}

/* ************************************************************************ */

static PyGetSetDef g_vectorIntGettersSeters[] = {
    {"x", (getter)vectorInt_getX, (setter)vectorInt_setX, nullptr, nullptr },
    {"y", (getter)vectorInt_getY, (setter)vectorInt_setY, nullptr, nullptr },
    {NULL}  /* Sentinel */
};

/* ************************************************************************ */

static void python_wrapper_core_VectorInt(PyObject* module)
{
    using type_def = TypeDefinition<core::Vector<int>>;

    type_def::init("core.VectorInt");
    type_def::definition.tp_getset = g_vectorIntGettersSeters;
    type_def::ready();

    Py_INCREF(&type_def::definition);
    PyModule_AddObject(module, "VectorInt", reinterpret_cast<PyObject*>(&type_def::definition));
}

/* ************************************************************************ */

void python_wrapper_core()
{
    PyObject* module = Py_InitModule3("core", nullptr, nullptr);

    python_wrapper_core_VectorFloat(module);
    python_wrapper_core_VectorInt(module);
}

/* ************************************************************************ */
