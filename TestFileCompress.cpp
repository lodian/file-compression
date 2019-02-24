#include "FileCompress.h"
#include <iostream>

void about()
{
	system("cls");
	std::cout << "\t╔═══════════════════════════════╗" << std::endl;
	std::cout << "\t║                      欢迎使用解压缩程序                      ║" << std::endl;
	std::cout << "\t║  关于程序：                                                  ║" << std::endl;
	std::cout << "\t║      压缩：                                                  ║" << std::endl;
	std::cout << "\t║          压缩文件请按照绝对路径的方式输入文件所在路径和带后  ║" << std::endl;
	std::cout << "\t║      缀文件名，否则默认压缩程序所在目录下对应名称的文件。    ║" << std::endl;
	std::cout << "\t║                                                              ║" << std::endl;
	std::cout << "\t║      解压：                                                  ║" << std::endl;
	std::cout << "\t║          解压文件请按照绝对路径的方式输入文件所在路径和带后  ║" << std::endl;
	std::cout << "\t║      缀文件名，否则默认解压程序所在目录下对应名称的文件。    ║" << std::endl;
	std::cout << "\t║                                                              ║" << std::endl;
	std::cout << "\t║  程序支持无损压缩各类中英文文本文件、图片、音频和视频文件，  ║" << std::endl;
	std::cout << "\t║      暂不支持目录压缩功能。谢谢支持！                        ║" << std::endl;
	std::cout << "\t║                                                              ║" << std::endl;
	std::cout << "\t║                                                lodian制作    ║" << std::endl;
	std::cout << "\t║                                https://blog.csdn.net/lodianm ║" << std::endl;
	std::cout << "\t╚═══════════════════════════════╝" << std::endl;
}

//选择菜单
void menu()
{
	std::cout << "\t╔═══════════════════════════════╗" << std::endl;
	std::cout << "\t║                                                              ║" << std::endl;
	std::cout << "\t║                   欢迎使用解压缩程序                         ║" << std::endl;
	std::cout << "\t║                                                              ║" << std::endl;
	std::cout << "\t║                     1. 压缩文件                              ║" << std::endl;
	std::cout << "\t║                     2. 解压文件                              ║" << std::endl;
	std::cout << "\t║                     3. 关于程序                              ║" << std::endl;
	std::cout << "\t║                     4. 退出程序                              ║" << std::endl;
	std::cout << "\t║                                                              ║" << std::endl;
	std::cout << "\t╚═══════════════════════════════╝" << std::endl;
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
		std::cout << "请输入所选择的序号:";
		std::cin >> choose;
		switch (choose)
		{
		case 1:
			std::cout << "请输入您要压缩的文件名:";
			std::cin >> strFilePath;
			fc.CompressFile(strFilePath);
			system("pause");
			break;
		case 2:
			std::cout << "请输入您要解压的文件名:";
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
			std::cout << "输入错误！请重新输入";
			system("pause");
			break;
		}
	}
	return 0;
}