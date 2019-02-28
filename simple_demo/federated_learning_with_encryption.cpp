#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <limits>
#include <site-packages/numpy/core/include/numpy/arrayobject.h>
#include "seal/seal.h"



using namespace std;
using namespace seal;
int init_numpy() {//��ʼ�� numpy ִ�л�������Ҫ�ǵ������python2.7��void�������ͣ�python3.0������int��������

	import_array();
}
void main() {
	char msg[256] = "11111 ";

	PyObject * pModule = NULL;
	PyObject * pFunc = NULL;
	PyObject * pArg = NULL;

	// ��ʼ��python����
	Py_Initialize();
	init_numpy();//��ʼ��numpy����
	// ����python�ű�
	pModule = PyImport_ImportModule("federated_mnistcnn");


	// ���TensorFlow����ָ��
	pFunc = PyObject_GetAttrString(pModule, "main");

	// ����TensorFlow����
	/*pArg = Py_BuildValue("(s)", "this is a call from c++");
	if (pModule != NULL) {
		PyEval_CallObject(pFunc, pArg);
	}*/
	PyObject * run_learning=PyEval_CallObject(pFunc,NULL);
	cout << "��ȡ�����飬���ڽ�������ֵ";
	if (PyList_Check(run_learning)) {//����python�ķ���ֵ
		vector<double> get_data;
		int Index_i = 0, Index_k = 0, Index_m = 0, Index_n = 0;
		int size_of_list = PyList_Size(run_learning);//��ȡlist�ĳߴ�
		for (Index_i = 0;Index_i < size_of_list;Index_i++) {
			//��ȡList�е�PyArrayObject����������Ҫ����ǿ��ת����
			PyArrayObject *ListItem = (PyArrayObject *)PyList_GetItem(run_learning, Index_i);
			int Rows = ListItem->dimensions[0], columns = ListItem->dimensions[1];
			for (Index_m = 0; Index_m < Rows; Index_m++) {

				for (Index_n = 0; Index_n < columns; Index_n++) {

					//get_data.push_back( *(double *)(ListItem->data + Index_m * ListItem->strides[0] + Index_n * ListItem->strides[1]));//�������ݣ�Index_m �� Index_n �ֱ�������Ԫ�ص����꣬������Ӧά�ȵĲ����������Է�������Ԫ��
				}
				cout << endl;
			}
			
		}
		cout << "�������";


	}
	Py_Finalize();



}

void pre_do() {

}