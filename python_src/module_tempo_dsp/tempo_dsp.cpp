
#include <Python.h>

#include "tempo_dsp/biquad.hpp"

typedef enum {BQ_DOUBLES, BQ_INTS, BQ_BYTES} data_type_t;

static PyObject * biquad_filter(PyObject *self, PyObject *args)
{
    double freq;
    double gain;
    double fs;
    tempo::filt_opt_t opt;
    double optval;
    tempo::filt_t type;
    unsigned int normalize;
    tempo::filt_quirk_t quirk;
    data_type_t data_type;

    if (!PyArg_ParseTuple(args, "dddIdIIII",
                &freq, &gain, &fs,
                &opt, &optval,
                &type,
                &normalize,
                &quirk,
                &data_type)) {
        // PyArg_ParseTuple will raise exception
        return NULL;
    }

    tempo::bqd_t bq = tempo::biquad_filter(freq, gain, fs,
                                            opt, optval,
                                            type,
                                            normalize,
                                            quirk);

    if (data_type == BQ_DOUBLES) {
        // Ownership is passed
        return Py_BuildValue("[dddddd]",
                                bq.a0, bq.a1, bq.a2,
                                bq.b0, bq.b1, bq.b2);
    } else if (data_type == BQ_INTS) {
        tempo::bqi_t bqi = bqd_2_bqi(bq);
        return Py_BuildValue("[IIIIII]",
                                bqi.a0, bqi.a1, bqi.a2,
                                bqi.b0, bqi.b1, bqi.b2);
    } else if (data_type == BQ_BYTES) {
        tempo::bqb_t bqb = bqd_2_bqb(bq);
        return Py_BuildValue("[BBBBBBBBBBBBBBBBBB]",
                                bqb.a0_l, bqb.a0_m, bqb.a0_h,
                                bqb.a1_l, bqb.a1_m, bqb.a1_h,
                                bqb.a2_l, bqb.a2_m, bqb.a2_h,
                                bqb.b0_l, bqb.b0_m, bqb.b0_h,
                                bqb.b1_l, bqb.b1_m, bqb.b1_h,
                                bqb.b2_l, bqb.b2_m, bqb.b2_h);
    } else { 
        PyErr_SetString(PyExc_ValueError,
                "Expected BQ_DOUBLES, BQ_INTS, or BQ_BYTES");
        return NULL;
    }
}

static PyMethodDef tempo_dsp_methods[] = {
    {"biquad_filter", biquad_filter, METH_VARARGS,
        "Create biquad filter coefficients list."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef tempo_dsp_module = {
    PyModuleDef_HEAD_INIT,
    "tempo_dsp",
    NULL, // TODO: Add a doc
    -1,
    tempo_dsp_methods,
};

PyMODINIT_FUNC
PyInit_tempo_dsp(void)
{
    PyObject *module = PyModule_Create(&tempo_dsp_module);
    PyModule_AddIntConstant(module, "BQ_DOUBLES", BQ_DOUBLES);
    PyModule_AddIntConstant(module, "BQ_INTS", BQ_INTS);
    PyModule_AddIntConstant(module, "BQ_BYTES", BQ_BYTES);
    PyModule_AddIntConstant(module, "LPF", tempo::LPF);
    PyModule_AddIntConstant(module, "HPF", tempo::HPF);
    PyModule_AddIntConstant(module, "BPFQ", tempo::BPFQ);
    PyModule_AddIntConstant(module, "BPF", tempo::BPF);
    PyModule_AddIntConstant(module, "NOTCH", tempo::NOTCH);
    PyModule_AddIntConstant(module, "APF", tempo::APF);
    PyModule_AddIntConstant(module, "PEAKING", tempo::PEAKING);
    PyModule_AddIntConstant(module, "LOWSHELF", tempo::LOWSHELF);
    PyModule_AddIntConstant(module, "HIGHSHELF", tempo::HIGHSHELF);
    PyModule_AddIntConstant(module, "Q", tempo::Q);
    PyModule_AddIntConstant(module, "BW", tempo::BW);
    PyModule_AddIntConstant(module, "S", tempo::S);
    PyModule_AddIntConstant(module, "NO_QUIRK", tempo::NO_QUIRK);

    return module;
}
