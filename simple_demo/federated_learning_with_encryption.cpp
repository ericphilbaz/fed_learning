#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <thread>
#include <mutex>
#include <memory>
#include <limits>
#include<boost/python.hpp>
#include<boost/python/numpy.hpp>
#include "seal/seal.h"
//#ifdef _DEBUG
//#undef _DEBUG
//#include <python.h>
//#define _DEBUG
//#else
//#include <python.h>
//#endif
using namespace std;
using namespace seal;
void main() {
	char msg[256] = "11111 ";

	PyObject * pModule = NULL;
	PyObject * pFunc = NULL;
	PyObject * pArg = NULL;
	PyObject * run_learning = NULL;

	// ��ʼ��python����
	Py_Initialize();
	//PyRun_SimpleString("import sys"); //PyRun_SimpleString("import tensorflow as tf");
	// ����python�ű�
	pModule = PyImport_ImportModule("federated_mnistcnn");


	// ���TensorFlow����ָ��
	pFunc = PyObject_GetAttrString(pModule, "main");

	// ����TensorFlow����
	/*pArg = Py_BuildValue("(s)", "this is a call from c++");
	if (pModule != NULL) {
		PyEval_CallObject(pFunc, pArg);
	}*/
	run_learning=PyEval_CallObject(pFunc,NULL);
	Py_Finalize();



}

void pre_do() {

}