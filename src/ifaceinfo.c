#include "list.h"
#include <Python.h>
#include <string.h>

static PyObject *ifaceinfo_getinfo(PyObject *self, PyObject *args) {
    char *name;

    if (!PyArg_ParseTuple(args, "s", &name))
        return NULL;
    ifface_t *iface = get_iface_byname(name);
    return Py_BuildValue("(sssss)", iface->ifname, iface->addr, iface->netmask,
                         iface->mac, iface->type);
}

static PyObject *ifaceinfo_getlocalip(PyObject *self, PyObject *args) {
    char *fallbackifname;
    char *ip = "";

    if (!PyArg_ParseTuple(args, "s", &fallbackifname))
        return NULL;
    ip = get_local_ip();
    if (!ip) {
        ifface_t *iface = get_iface_byname(fallbackifname);
        ip = iface->addr;
    }
    return Py_BuildValue("s", ip);
}

static PyObject *ifaceinfo_listifa(PyObject *self, PyObject *args) {
    ifacelist_t *iflist = list_ifa();
    int const ifa_cnt = len_ifacelist(iflist);
    int i = 0;
    PyObject *python_val = PyList_New(ifa_cnt);
    while (iflist->next) {

        PyObject *py_tuple = Py_BuildValue(
            "(sssss)", iflist->next->iface->ifname, iflist->next->iface->addr,
            iflist->next->iface->netmask, iflist->next->iface->mac,
            iflist->next->iface->type);
        iflist = iflist->next;
        PyList_SetItem(python_val, i, py_tuple);
        i++;
    }

    return python_val;
}

static PyMethodDef IfaceMethods[] = {
    {"getinfo", ifaceinfo_getinfo, METH_VARARGS, "Get Interface Information."},
    {"getip", ifaceinfo_getlocalip, METH_VARARGS, "Get Local IP Address."},
    {"listall", ifaceinfo_listifa, METH_VARARGS, "List All Interfaces."},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef ifacemodule = {
    PyModuleDef_HEAD_INIT, "ifaceinfo", "Get Address, Netmask, Mac Information",
    -1, IfaceMethods};

PyMODINIT_FUNC PyInit_ifaceinfo(void) { return PyModule_Create(&ifacemodule); }
