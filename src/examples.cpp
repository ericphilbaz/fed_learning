
#include "environment.h"

using namespace seal;
using namespace std;
//using namespace seal;
int init_numpy() {//��ʼ�� numpy ִ�л�������Ҫ�ǵ������python2.7��void�������ͣ�python3.0������int��������

	import_array();
}
void main2() {
}
void main1() {
	int count = 5;
	cout << "creating the Homomorphic encryption instance" << endl;
	//����̬ͬ������Կ
	EncryptionParameters parms = EncryptionParameters(scheme_type::CKKS);
	parms.set_poly_modulus_degree(4096);
	parms.set_coeff_modulus(DefaultParams::coeff_modulus_128(4096));
	auto context = SEALContext::Create(parms);
	KeyGenerator keygen(context);
	auto public_key = keygen.public_key();
	auto secret_key = keygen.secret_key();
	auto relin_keys = keygen.relin_keys(DefaultParams::dbc_max());
	//��������ʵ��
	Encryptor encryptor(context, public_key);
	Evaluator evaluator(context);
	Decryptor decryptor(context, secret_key);
	CKKSEncoder encoder(context);
	size_t slot_count = encoder.slot_count();
	//���ű���
	double scale = pow(2.0, 60);
	//����
	Ciphertext weights_cipher;
	Ciphertext biases_cipher;
	//����ȫ�����ʽ
	//vector <double> weight_vector{ 13,13,13,13,13 };
	vector <double> weight_vector(2048);//�����ڴ�������61470��Ԫ��
	vector<Ciphertext> weight_vector_2D(31);

	vector <double> biases_vector(236);//�����ڴ�������128��Ԫ��
	//��ʱplaintext
	Plaintext temp_plantext;
	//����Ȩ��
	encoder.encode(weight_vector, scale, temp_plantext);
	//����Ȩ��
	encryptor.encrypt(temp_plantext, weights_cipher);
	//����λ��
	encoder.encode(biases_vector, scale, temp_plantext);
	//����λ��
	encryptor.encrypt(temp_plantext, biases_cipher);
	for (int i = 0;i < 31;i++) {
		weight_vector_2D[i] = weights_cipher;
	}

	PyObject * pModule = NULL;
	PyObject * pFunc = NULL;
	PyObject * pArg = NULL;

	// ��ʼ��python����
	Py_Initialize();
	init_numpy();//��ʼ��numpy����
	// ����python�ű�
	pModule = PyImport_ImportModule("federated_mnistcnn");


	// ���TensorFlow����ָ��
	pFunc = PyObject_GetAttrString(pModule, "run");
	//pFunc = PyObject_GetAttrString(pModule, "test");
	// ����TensorFlow����
	/*pArg = Py_BuildValue("(s)", "this is a call from c++");
	if (pModule != NULL) {
		PyEval_CallObject(pFunc, pArg);
	}*/
	PyObject * list1 = PyEval_CallObject(pFunc, NULL);
	cout << "��ȡ�����飬���ڽ�������ֵ" << endl;
	if (PyList_Check(list1)) {//����python�ķ���ֵ
		Plaintext plain;
		vector<double> vector_data;
		//vector<double> weights;
		//vector<double> biases;
		int Index_1 = 0, Index_2 = 0, Index_3 = 0;//��һ�㣨Ȩ�أ�ƫ���������ڶ��㣨ÿ���ͻ��˵����ݣ��������㣨�������磩
		int size_of_list = PyList_Size(list1);//��ȡlist�ĳߴ�
		for (Index_1 = 0;Index_1 < size_of_list;Index_1++) {
			if (Index_1 == 0) {
				//������һ��list
				//��ȡList�е�PyArrayObject����������Ҫ����ǿ��ת����
				//PyArrayObject *ListItem = (PyArrayObject *)PyList_GetItem(run_learning, Index_1);
				PyObject *List2 = (PyObject *)PyList_GetItem(list1, Index_1);//�����ڶ���list
				for (Index_2 = 0;Index_2 < PyList_Size(List2);Index_2++)
				{
					vector_data.clear();//�����ʱ��������
					PyObject *List3 = (PyObject *)PyList_GetItem(List2, Index_2);//����������list.numpy�����ά��=����㣨5��5��1��6����һ�㣨5��5��6��16���ڶ��㣨5��5��16��120�������㣨120��84�����Ĳ㣨84��10��
					for (Index_3 = 0;Index_3 < PyList_Size(List3);Index_3++) {
						PyArrayObject *data = (PyArrayObject *)PyList_GetItem(List3, Index_3);
						switch (Index_3) {
						case 0:
							for (int i = 0;i < 5;i++) {
								for (int j = 0;j < 5;j++) {
									for (int x = 0;x < 1;x++) {
										for (int y = 0;y < 6;y++) {
											vector_data.push_back(*(float *)(data->data + i * data->strides[0] + j * data->strides[1]+ x * data->strides[2]+ y * data->strides[3]));
										}
									}
								}
							}
							break;
						case 1:
							for (int i = 0;i < 5;i++) {
								for (int j = 0;j < 5;j++) {
									for (int x = 0;x < 6;x++) {
										for (int y = 0;y < 16;y++) {
											vector_data.push_back(*(float *)(data->data + i * data->strides[0] + j * data->strides[1] + x * data->strides[2] + y * data->strides[3]));
										}
									}
								}
							}
							break;
						case 2:
							for (int i = 0;i < 5;i++) {
								for (int j = 0;j < 5;j++) {
									for (int x = 0;x < 16;x++) {
										for (int y = 0;y < 120;y++) {
											vector_data.push_back(*(float *)(data->data + i * data->strides[0] + j * data->strides[1] + x * data->strides[2] + y * data->strides[3]));
										}
									}
								}
							}
							break;
						case 3:
							for (int i = 0;i < 120;i++) {
								for (int j = 0;j < 84;j++) {
									
									vector_data.push_back(*(float *)(data->data + i * data->strides[0] + j * data->strides[1]));
										
								}
							}
							break;
						case 4:
							for (int i = 0;i < 84;i++) {
								for (int j = 0;j < 10;j++) {

									vector_data.push_back(*(float *)(data->data + i * data->strides[0] + j * data->strides[1]));

								}
							}
							break;
						}
					}
					for (int i = 0;i < 30;i++) {
						//weight_vector = vector<double>(vector_data.begin(), vector_data.begin() + 2048);
						encoder.encode(vector<double>(vector_data.begin() + i * 2048, vector_data.begin() + i * 2048 + 2048), weights_cipher.parms_id(), weights_cipher.scale(), temp_plantext);
						evaluator.add_plain_inplace(weight_vector_2D[i], temp_plantext);
					}
					encoder.encode(vector<double>(vector_data.end() - 70, vector_data.end()), weights_cipher.parms_id(), weights_cipher.scale(), temp_plantext);
					evaluator.add_plain_inplace(weight_vector_2D[30], temp_plantext);


				}
			}
			if (Index_1 == 1) {

				PyObject *List2 = (PyObject *)PyList_GetItem(list1, Index_1);//λ�Ƶ�ά�ȵ�6,16,120,84,10
				for (Index_2 = 0;Index_2 < PyList_Size(List2);Index_2++) {
					PyObject *List3 = (PyObject *)PyList_GetItem(List2, Index_2);
					vector_data.clear();//�����ʱ��������
					for (Index_3 = 0;Index_3 < PyList_Size(List3);Index_3++) {
						PyArrayObject *data = (PyArrayObject *)PyList_GetItem(List3, Index_3);
						switch (Index_3) {
						case 0:
							for (int i = 0;i < 6;i++) {
								vector_data.push_back(*(float *)(data->data + i * data->strides[0]));
							}
							break;
						case 1:
							for (int i = 0;i < 16;i++) {
								vector_data.push_back(*(float *)(data->data + i * data->strides[0]));
							}
							break;
						case 2:
							for (int i = 0;i < 120;i++) {
								vector_data.push_back(*(float *)(data->data + i * data->strides[0]));
							}
							break;
						case 3:
							for (int i = 0;i < 84;i++) {
								vector_data.push_back(*(float *)(data->data + i * data->strides[0]));
							}
							break;
						case 4:
							for (int i = 0;i < 10;i++) {
								vector_data.push_back(*(float *)(data->data + i * data->strides[0]));
							}
							break;
						}
					}
					encoder.encode(vector_data, biases_cipher.parms_id(), biases_cipher.scale(), temp_plantext);
					evaluator.add_plain_inplace(biases_cipher, temp_plantext);
				}
			}
		}

	}
	weight_vector.clear();//�����ʱ��������
	vector<double>temp_vector(2048);
	for (int i = 0;i < 31;i++) {
		decryptor.decrypt(weight_vector_2D[i], temp_plantext);
		encoder.decode(temp_plantext, temp_vector);
		weight_vector.insert(weight_vector.end(), temp_vector.begin(), temp_vector.end());

	}
	decryptor.decrypt(biases_cipher, temp_plantext);
	encoder.decode(temp_plantext, biases_vector);
	cout << "�������" << endl;
	PyObject *ArgList = PyTuple_New(2);//����python������ֵ
	PyObject *PyList_weight = PyList_New(61470);//����Ȩ��list
	PyObject *PyList_biase = PyList_New(236);//����ƫ��list
	for (int Index_i = 0; Index_i < PyList_Size(PyList_weight); Index_i++) {

		PyList_SetItem(PyList_weight, Index_i, PyFloat_FromDouble(weight_vector[Index_i]));//Ȩ�ز㸳ֵ
	}
	for (int Index_i = 0; Index_i < PyList_Size(PyList_biase); Index_i++) {

		PyList_SetItem(PyList_biase, Index_i, PyFloat_FromDouble(biases_vector[Index_i]));//ƫ�ò㸳ֵ
	}
	PyTuple_SetItem(ArgList, 0, PyList_weight);
	PyTuple_SetItem(ArgList, 1, PyList_biase);
	pFunc = PyObject_GetAttrString(pModule, "test");
	PyObject_CallObject(pFunc, ArgList);
	while (count > 1) {
		count -= 1;
		pFunc = PyObject_GetAttrString(pModule, "next");
		list1 = PyObject_CallObject(pFunc, ArgList);
		cout << "��ȡ�����飬���ڽ�������ֵ" << endl;
		if (PyList_Check(list1)) {//����python�ķ���ֵ
			Plaintext plain;
			vector<double> vector_data;
			//vector<double> weights;
			//vector<double> biases;
			int Index_1 = 0, Index_2 = 0, Index_3 = 0;//��һ�㣨Ȩ�أ�ƫ���������ڶ��㣨ÿ���ͻ��˵����ݣ��������㣨�������磩
			int size_of_list = PyList_Size(list1);//��ȡlist�ĳߴ�
			for (Index_1 = 0;Index_1 < size_of_list;Index_1++) {
				if (Index_1 == 0) {
					//������һ��list
					//��ȡList�е�PyArrayObject����������Ҫ����ǿ��ת����
					//PyArrayObject *ListItem = (PyArrayObject *)PyList_GetItem(run_learning, Index_1);
					PyObject *List2 = (PyObject *)PyList_GetItem(list1, Index_1);//�����ڶ���list
					for (Index_2 = 0;Index_2 < PyList_Size(List2);Index_2++)
					{
						vector_data.clear();//�����ʱ��������
						PyObject *List3 = (PyObject *)PyList_GetItem(List2, Index_2);//����������list.numpy�����ά��=����㣨5��5��1��6����һ�㣨5��5��6��16���ڶ��㣨5��5��16��120�������㣨120��84�����Ĳ㣨84��10��
						for (Index_3 = 0;Index_3 < PyList_Size(List3);Index_3++) {
							PyArrayObject *data = (PyArrayObject *)PyList_GetItem(List3, Index_3);
							switch (Index_3) {
							case 0:
								for (int i = 0;i < 5;i++) {
									for (int j = 0;j < 5;j++) {
										for (int x = 0;x < 1;x++) {
											for (int y = 0;y < 6;y++) {
												vector_data.push_back(*(float *)(data->data + i * data->strides[0] + j * data->strides[1] + x * data->strides[2] + y * data->strides[3]));
											}
										}
									}
								}
								break;
							case 1:
								for (int i = 0;i < 5;i++) {
									for (int j = 0;j < 5;j++) {
										for (int x = 0;x < 6;x++) {
											for (int y = 0;y < 16;y++) {
												vector_data.push_back(*(float *)(data->data + i * data->strides[0] + j * data->strides[1] + x * data->strides[2] + y * data->strides[3]));
											}
										}
									}
								}
								break;
							case 2:
								for (int i = 0;i < 5;i++) {
									for (int j = 0;j < 5;j++) {
										for (int x = 0;x < 16;x++) {
											for (int y = 0;y < 120;y++) {
												vector_data.push_back(*(float *)(data->data + i * data->strides[0] + j * data->strides[1] + x * data->strides[2] + y * data->strides[3]));
											}
										}
									}
								}
								break;
							case 3:
								for (int i = 0;i < 120;i++) {
									for (int j = 0;j < 84;j++) {

										vector_data.push_back(*(float *)(data->data + i * data->strides[0] + j * data->strides[1]));

									}
								}
								break;
							case 4:
								for (int i = 0;i < 84;i++) {
									for (int j = 0;j < 10;j++) {

										vector_data.push_back(*(float *)(data->data + i * data->strides[0] + j * data->strides[1]));

									}
								}
								break;
							}
						}
						for (int i = 0;i < 30;i++) {
							//weight_vector = vector<double>(vector_data.begin(), vector_data.begin() + 2048);
							encoder.encode(vector<double>(vector_data.begin() + i * 2048, vector_data.begin() + i * 2048 + 2048), weights_cipher.parms_id(), weights_cipher.scale(), temp_plantext);
							evaluator.add_plain_inplace(weight_vector_2D[i], temp_plantext);
						}
						encoder.encode(vector<double>(vector_data.end() - 70, vector_data.end()), weights_cipher.parms_id(), weights_cipher.scale(), temp_plantext);
						evaluator.add_plain_inplace(weight_vector_2D[30], temp_plantext);


					}
				}
				if (Index_1 == 1) {

					PyObject *List2 = (PyObject *)PyList_GetItem(list1, Index_1);//λ�Ƶ�ά�ȵ�6,16,120,84,10
					for (Index_2 = 0;Index_2 < PyList_Size(List2);Index_2++) {
						PyObject *List3 = (PyObject *)PyList_GetItem(List2, Index_2);
						vector_data.clear();//�����ʱ��������
						for (Index_3 = 0;Index_3 < PyList_Size(List3);Index_3++) {
							PyArrayObject *data = (PyArrayObject *)PyList_GetItem(List3, Index_3);
							switch (Index_3) {
							case 0:
								for (int i = 0;i < 6;i++) {
									vector_data.push_back(*(float *)(data->data + i * data->strides[0]));
								}
								break;
							case 1:
								for (int i = 0;i < 16;i++) {
									vector_data.push_back(*(float *)(data->data + i * data->strides[0]));
								}
								break;
							case 2:
								for (int i = 0;i < 120;i++) {
									vector_data.push_back(*(float *)(data->data + i * data->strides[0]));
								}
								break;
							case 3:
								for (int i = 0;i < 84;i++) {
									vector_data.push_back(*(float *)(data->data + i * data->strides[0]));
								}
								break;
							case 4:
								for (int i = 0;i < 10;i++) {
									vector_data.push_back(*(float *)(data->data + i * data->strides[0]));
								}
								break;
							}
						}
						encoder.encode(vector_data, biases_cipher.parms_id(), biases_cipher.scale(), temp_plantext);
						evaluator.add_plain_inplace(biases_cipher, temp_plantext);
					}
				}
			}

		}
		weight_vector.clear();//�����ʱ��������
		vector<double>temp_vector(2048);
		for (int i = 0;i < 31;i++) {
			decryptor.decrypt(weight_vector_2D[i], temp_plantext);
			encoder.decode(temp_plantext, temp_vector);
			weight_vector.insert(weight_vector.end(), temp_vector.begin(), temp_vector.end());

		}
		decryptor.decrypt(biases_cipher, temp_plantext);
		encoder.decode(temp_plantext, biases_vector);
		cout << "�������" << endl;
		PyObject *ArgList = PyTuple_New(2);//����python������ֵ
		PyObject *PyList_weight = PyList_New(61470);//����Ȩ��list
		PyObject *PyList_biase = PyList_New(236);//����ƫ��list
		for (int Index_i = 0; Index_i < PyList_Size(PyList_weight); Index_i++) {

			PyList_SetItem(PyList_weight, Index_i, PyFloat_FromDouble(weight_vector[Index_i]));//Ȩ�ز㸳ֵ
		}
		for (int Index_i = 0; Index_i < PyList_Size(PyList_biase); Index_i++) {

			PyList_SetItem(PyList_biase, Index_i, PyFloat_FromDouble(biases_vector[Index_i]));//ƫ�ò㸳ֵ
		}
		PyTuple_SetItem(ArgList, 0, PyList_weight);
		PyTuple_SetItem(ArgList, 1, PyList_biase);
		pFunc = PyObject_GetAttrString(pModule, "test");
		PyObject_CallObject(pFunc, ArgList);

	}
	cout << "aaaa" << endl;


	Py_Finalize();



}
