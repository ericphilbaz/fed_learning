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
	//pFunc = PyObject_GetAttrString(pModule, "test");
	// ����TensorFlow����
	/*pArg = Py_BuildValue("(s)", "this is a call from c++");
	if (pModule != NULL) {
		PyEval_CallObject(pFunc, pArg);
	}*/
	PyObject * list1=PyEval_CallObject(pFunc,NULL);
	cout << "��ȡ�����飬���ڽ�������ֵ";
	if (PyList_Check(list1)) {//����python�ķ���ֵ
		vector<double> get_data;
		int Index_1 = 0, Index_2 = 0, Index_3 = 0;//��һ�㣨Ȩ�أ�ƫ���������ڶ��㣨ÿ���ͻ��˵����ݣ��������㣨�ĸ����磩
		int size_of_list = PyList_Size(list1);//��ȡlist�ĳߴ�
		for (Index_1 = 0;Index_1 < size_of_list;Index_1++) {
			if(Index_1==0){
			//������һ��list
			//��ȡList�е�PyArrayObject����������Ҫ����ǿ��ת����
			//PyArrayObject *ListItem = (PyArrayObject *)PyList_GetItem(run_learning, Index_1);
			PyObject *List2 = (PyObject *)PyList_GetItem(list1, Index_1);//�����ڶ���list
			for (Index_2 = 0;Index_2 < PyList_Size(List2);Index_2++) {
				PyObject *List3 = (PyObject *)PyList_GetItem(List2, Index_2);//����������list.numpy�����ά��=�����5��5��1��32��һ��5��5��32��64�ڶ���3136, 128�����㣨128��10��
				for (Index_3 = 0;Index_3 < PyList_Size(List3);Index_3++) {
					PyArrayObject *data = (PyArrayObject *)PyList_GetItem(List3, Index_3);
					switch (Index_3) {
					case 0:
						for (int i = 0;i < 5;i++) {
							for (int j = 0;j < 5;j++) {
								for (int m = 0;m < 1;m++) {
									for (int n = 0;n < 32;n++) {
										cout << *(float *)(data->data + i * data->strides[0] + j * data->strides[1] + m * data->strides[2] + n * data->strides[3]) << endl;
									}
								}
							}
						}
						break;
					case 1:
						for (int i = 0;i < 5;i++) {
							for (int j = 0;j < 5;j++) {
								for (int m = 0;m < 32;m++) {
									for (int n = 0;n < 64;n++) {
										cout << *(float *)(data->data + i * data->strides[0] + j * data->strides[1] + m * data->strides[2] + n * data->strides[3]);
									}
								}
							}
						}
						break;
					case 2:
						for (int i = 0;i < 3136;i++) {
							for (int j = 0;j < 128;j++) {
								cout << *(float *)(data->data + i * data->strides[0] + j * data->strides[1]);
							}
						}

						break;
					case 3:
						for (int i = 0;i < 128;i++) {
							for (int j = 0;j < 10;j++) {
								cout << *(float *)(data->data + i * data->strides[0] + j * data->strides[1]);
							}
						}
						break;


					}
				}
			}
			if (Index_1 == 1) {
				PyObject *List2 = (PyObject *)PyList_GetItem(list1, Index_1);//�����ڶ���list#�ڶ�������[100,4,32]
				for (Index_2 = 0;Index_2 < PyList_Size(List2);Index_2++) {
					PyObject *List3 = (PyObject *)PyList_GetItem(List2, Index_2);
					for (Index_3 = 0;Index_3 < PyList_Size(List3);Index_3++) {
						PyArrayObject *data = (PyArrayObject *)PyList_GetItem(List3, Index_3);
						for (int i = 0;i < 32;i++) {
							cout << *(float *)(data->data + i * data->strides[0] );
						}
					}
				}
			}
			//	int Rows = ListItem->dimensions[0], columns = ListItem->dimensions[1];
			//	for (int Index_m = 0; Index_m < Rows; Index_m++) {

			//		for (int Index_n = 0; Index_n < columns; Index_n++) {

			//			//get_data.push_back( *(double *)(ListItem->data + Index_m * ListItem->strides[0] + Index_n * ListItem->strides[1]));//�������ݣ�Index_m �� Index_n �ֱ�������Ԫ�ص����꣬������Ӧά�ȵĲ����������Է�������Ԫ��
			//		}
			//		cout << endl;
			//	}
			}

		}
		cout << "�������";


	}
	Py_Finalize();



}

void pre_do() {

}