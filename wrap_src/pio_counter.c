#include <Python.h>

#include "counter_lib.h"

static char module_docstring[] =
    "blah";
static char vector_sub_docstring[] =
    "blash";


static PyObject *_pio_counter_claim_statemachine(PyObject *self, PyObject *args)
{
    // printf("claiming state machine\r\n");
    int sm = claim_statemachine();
    // printf("claimed state machine %d\r\n", sm);
    return Py_BuildValue("i", sm);
}

static PyObject *_pio_counter_release_statemachine(PyObject *self, PyObject *args)
{
    int sm;
    if(!PyArg_ParseTuple(args, "i", &sm))
    {
        printf("failed to parse args\r\n");
        return Py_BuildValue("");
    }
    release_statemachine(sm);

    return Py_BuildValue("");
}


static PyObject *_pio_counter_initialise(PyObject *self, PyObject *args)
{
    // int gpio_pin, clock_rate;

    // if(!PyArg_ParseTuple(args, "ii", &gpio_pin, &clock_rate))
    // {
    //     printf("failed to parse args\r\n");
    //     return Py_BuildValue("");
    // }

    // initialise(gpio_pin, clock_rate);

    return Py_BuildValue("");
}

static PyObject *_pio_counter_capture(PyObject *self, PyObject *args)
{
    Py_buffer buffer;
    int sm;
    int ret;

    if(!PyArg_ParseTuple(args, "is*", &sm, &buffer)) 
    {
        return Py_BuildValue("");
    }
    // printf("capture buffer 0x%08x %d\r\n", buffer.buf, buffer.len);
    // Py_INCREF(&buffer);
    
    Py_BEGIN_ALLOW_THREADS
    ret = capture(sm, buffer.buf, buffer.len);
    Py_END_ALLOW_THREADS
    
    // Py_DECREF(&buffer);

    PyBuffer_Release(&buffer);
    return Py_BuildValue("i", ret);
}

static PyObject *_pio_counter_load_program(PyObject *self, PyObject *args)
{
    uint offset;
    int sm;
    int gpio_pin;
    int clock_rate;
    
    if(!PyArg_ParseTuple(args, "iii", &sm, &gpio_pin, &clock_rate)) 
    {
        return Py_BuildValue("");
    }
    offset = load_program(sm, gpio_pin, clock_rate);
    
    return Py_BuildValue("i", offset);
}

static PyObject *_pio_counter_start_program(PyObject *self, PyObject *args)
{
    int sm;
    if(!PyArg_ParseTuple(args, "i", &sm)) 
    {
        return Py_BuildValue("");
    }
    start_program(sm);

    return Py_BuildValue("");
}

static PyObject *_pio_counter_stop_program(PyObject *self, PyObject *args)
{
    int sm;
    uint offset;
    if(!PyArg_ParseTuple(args, "ii", &sm, &offset)) 
    {
        return Py_BuildValue("");
    }
    stop_program(sm, offset);

    return Py_BuildValue("");
}

static PyObject *_pio_counter_rx_fifo_level(PyObject *self, PyObject *args)
{
    int sm;
    if(!PyArg_ParseTuple(args, "i", &sm)) 
    {
        return Py_BuildValue("");
    }
     

    return Py_BuildValue("i", rx_fifo_level(sm));
}

static PyMethodDef module_methods[] = {
    {"_claim_statemachine", _pio_counter_claim_statemachine, METH_VARARGS, vector_sub_docstring},
    {"_release_statemachine", _pio_counter_release_statemachine, METH_VARARGS, vector_sub_docstring},
    {"_initialise", _pio_counter_initialise, METH_VARARGS, vector_sub_docstring},
    {"_capture", _pio_counter_capture, METH_VARARGS, vector_sub_docstring},
    {"_load_program", _pio_counter_load_program, METH_VARARGS, vector_sub_docstring},
    {"_start_program", _pio_counter_start_program, METH_VARARGS, vector_sub_docstring},
    {"_stop_program", _pio_counter_stop_program, METH_VARARGS, vector_sub_docstring},
    {"_rx_fifo_level", _pio_counter_rx_fifo_level, METH_VARARGS, vector_sub_docstring},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef pio_countermodule = {
    PyModuleDef_HEAD_INIT,
    "_pio_counter",   /* name of module */
    module_docstring, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    module_methods,
};

PyMODINIT_FUNC PyInit__pio_counter(void)
{
    return PyModule_Create(&pio_countermodule);
}