#include "FileCompress.h"
#include <iostream>

void about()
{
	system("cls");
	std::cout << "\t�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[" << std::endl;
	std::cout << "\t�U                      ��ӭʹ�ý�ѹ������                      �U" << std::endl;
	std::cout << "\t�U  ���ڳ���                                                  �U" << std::endl;
	std::cout << "\t�U      ѹ����                                                  �U" << std::endl;
	std::cout << "\t�U          ѹ���ļ��밴�վ���·���ķ�ʽ�����ļ�����·���ʹ���  �U" << std::endl;
	std::cout << "\t�U      ׺�ļ���������Ĭ��ѹ����������Ŀ¼�¶�Ӧ���Ƶ��ļ���    �U" << std::endl;
	std::cout << "\t�U                                                              �U" << std::endl;
	std::cout << "\t�U      ��ѹ��                                                  �U" << std::endl;
	std::cout << "\t�U          ��ѹ�ļ��밴�վ���·���ķ�ʽ�����ļ�����·���ʹ���  �U" << std::endl;
	std::cout << "\t�U      ׺�ļ���������Ĭ�Ͻ�ѹ��������Ŀ¼�¶�Ӧ���Ƶ��ļ���    �U" << std::endl;
	std::cout << "\t�U                                                              �U" << std::endl;
	std::cout << "\t�U  ����֧������ѹ��������Ӣ���ı��ļ���ͼƬ����Ƶ����Ƶ�ļ���  �U" << std::endl;
	std::cout << "\t�U      �ݲ�֧��Ŀ¼ѹ�����ܡ�лл֧�֣�                        �U" << std::endl;
	std::cout << "\t�U                                                              �U" << std::endl;
	std::cout << "\t�U                                                lodian����    �U" << std::endl;
	std::cout << "\t�U                                https://blog.csdn.net/lodianm �U" << std::endl;
	std::cout << "\t�^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a" << std::endl;
}

//ѡ��˵�
void menu()
{
	std::cout << "\t�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[" << std::endl;
	std::cout << "\t�U                                                              �U" << std::endl;
	std::cout << "\t�U                   ��ӭʹ�ý�ѹ������                         �U" << std::endl;
	std::cout << "\t�U                                                              �U" << std::endl;
	std::cout << "\t�U                     1. ѹ���ļ�                              �U" << std::endl;
	std::cout << "\t�U                     2. ��ѹ�ļ�                              �U" << std::endl;
	std::cout << "\t�U                     3. ���ڳ���                              �U" << std::endl;
	std::cout << "\t�U                     4. �˳�����                              �U" << std::endl;
	std::cout << "\t�U                                                              �U" << std::endl;
	std::cout << "\t�^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a" << std::endl;
}


int main()
{
	while (1)
	{
		int choose;
		std::string strFilePath;
		FileCompress fc;
		system("cls");
		menu();
		std::cout << "��������ѡ������:";
		std::cin >> choose;
		switch (choose)
		{
		case 1:
			std::cout << "��������Ҫѹ�����ļ���:";
			std::cin >> strFilePath;
			fc.CompressFile(strFilePath);
			system("pause");
			break;
		case 2:
			std::cout << "��������Ҫ��ѹ���ļ���:";
			std::cin >> strFilePath;
			fc.UnCompressFile(strFilePath);
			system("pause");
			break;
		case 3:
			about();
			system("pause");
			break;
		case 4:
			return 0;
		default:
			std::cout << "�����������������";
			system("pause");
			break;
		}
	}
	return 0;
}