#include "FileCompress.h"
#include <iostream>
#include <assert.h>
using namespace std;

FileCompress::FileCompress()
{
	_fileInfo.resize(256);
	for (size_t i = 0; i < 256; ++i)
	{
		_fileInfo[i]._ch = i;
		_fileInfo[i]._count = 0;
	}
}

void FileCompress::CompressFile(const std::string &strFilePath)
{
	//	1.统计源文件中每个字符出现的次数
	FILE* fIn = fopen(strFilePath.c_str(), "rb");
	if (nullptr == fIn)
	{
		cout << "打开文件失败" << endl;
		return;
	}
	UCH* pReadBuff = new UCH[1024];
	while (true)
	{
		size_t rdSize = fread(pReadBuff, 1, 1024, fIn);
		if (0 == rdSize)
			break;

		for (size_t i = 0; i < rdSize; ++i)
		{
			_fileInfo[pReadBuff[i]]._count++;
		}
	}
		
	//2.以每个字符出现的次数为权值创建Huffman树
	HuffmanTree<CharInfo> ht;
	ht.CreateHuffmanTree(_fileInfo, CharInfo(0));

	//3.通过Huffman树获取每个字符的编码
	GetHuffmanCode(ht.GetRoot());

	//4.用获取到的Huffman编码来重新改写文件
	char ch = 0;
	char bitCount = 0;
	FILE* fOut = fopen("1.hzp", "wb");
	assert(fOut);

	WriteHeaderInfo(fOut, strFilePath);
	fseek(fIn, 0, SEEK_SET);
	while (true)
	{
		size_t rdSize = fread(pReadBuff, 1, 1024, fIn);
		if (0 == rdSize)
			break;

		for (size_t i = 0; i < rdSize; ++i)
		{
			string& strCode = _fileInfo[pReadBuff[i]]._strCode;
			//将每个字符的编码放到ch的比特位中
			for (size_t j = 0; j < strCode.size(); ++j)
			{
				ch <<= 1;//最后一个比特位放好之后，下次进来应该向左移上一位
				if ('1' == strCode[j])
					ch |= 1;//按位或，相异为1，运算完最后一个比特位为1
				//else为0，最后一个比特位本来就是0
				bitCount++;
				if (8 == bitCount)
				{
					//ch的8个比特位都放好之后就写入到文件中
					fputc(ch, fOut);
					ch = 0;
					bitCount = 0;
				}
			}
		}
	}
	//最后一次的ch的8个比特位没有填充满
	if (bitCount > 0 && bitCount < 8)
	{
		ch <<= (8 - bitCount);
		fputc(ch, fOut);
	}

	delete[] pReadBuff;
	fclose(fIn);
	fclose(fOut);
}

void FileCompress::UnCompressFile(const std::string &strFilePath)
{
	string suffix = strFilePath.substr(strFilePath.rfind('.'));
	if (suffix != ".hzp")
	{
		cout << "压缩文件格式不匹配！" << endl;
		return;
	}
	FILE* fIn = fopen(strFilePath.c_str(), "rb");
	if (nullptr == fIn)
	{
		cout << "压缩文件打开文件失败" << endl;
		return;
	}
	//从压缩文件中获取源文件的后缀
	suffix = "";
	GetLine(fIn, suffix);

	//从源文件中获取字符编码信息
	string strContent;
	GetLine(fIn, strContent);
	size_t lineCount = atoi(strContent.c_str());

	size_t charCount = 0;
	for (size_t i = 0; i < lineCount; ++i)
	{
		strContent = "";
		GetLine(fIn, strContent);
		if (strContent == "")//在GetLine里面碰到换行符是直接return，所以strContent=""，而可能源文件中可能就有一个换行符
		{
			strContent += '\n';
			GetLine(fIn, strContent);
		}
		charCount = atoi(strContent.c_str() + 2);
		_fileInfo[(UCH)strContent[0]]._count = charCount;//虽然已经将_ch设置成无符号char类型，但是这里的strContent是string类，拿到的也可能是负数，所以要进行强转
	}
	//还原Huffman树
	HuffmanTree<CharInfo> ht;
	ht.CreateHuffmanTree(_fileInfo, CharInfo(0));

	//解压缩
	string strUCFileName("2");
	strUCFileName += suffix;
	FILE* fOut = fopen(strUCFileName.c_str(), "wb");
	char* pReadBuff = new char[1024];
	int pos = 7;
	HTNode<CharInfo>* pCur = ht.GetRoot();
	long long fileSize = pCur->_weight._count;
	while (true)
	{
		size_t rdSize = fread(pReadBuff, 1, 1024, fIn);
		if (0 == rdSize)
			break;

		for (size_t i = 0; i < rdSize; ++i)
		{
			//解压缩当前字节的压缩数据
			pos = 7;
			for (size_t j = 0; j < 8; ++j)
			{
				if (pReadBuff[i] & (1 << pos))//依次检验当前字节的8个比特位是否为1
					pCur = pCur->_pRight;
				else
					pCur = pCur->_pLeft;

				if (nullptr == pCur->_pLeft && nullptr == pCur->_pRight)
				{
					fputc(pCur->_weight._ch, fOut);
					pCur = ht.GetRoot();
					--fileSize;
					if (0 == fileSize)
						break;
				}
				pos--;
				if (pos < 0)
				{
					break;
				}
			}
		}
	}
	delete[] pReadBuff;
	fclose(fIn);
	fclose(fOut);
}

void FileCompress::GetHuffmanCode(HTNode<CharInfo>* pRoot)
{
	if (nullptr == pRoot)
		return;

	GetHuffmanCode(pRoot->_pLeft);
	GetHuffmanCode(pRoot->_pRight);

	if (nullptr == pRoot->_pLeft && nullptr == pRoot->_pRight)
	{
		HTNode<CharInfo>* pCur = pRoot;
		HTNode<CharInfo>* pParent = pCur->_pParent;
		string& strCode = _fileInfo[pRoot->_weight._ch]._strCode;
		while (pParent)
		{
			if (pCur == pParent->_pLeft)
				strCode += '0';
			else
				strCode += '1';

			pCur = pParent;
			pParent = pCur->_pParent;
		}
		reverse(strCode.begin(), strCode.end());
	}
}

void FileCompress::WriteHeaderInfo(FILE* pf, const string& strFileName)
{
	//记录源文件后缀
	string suffix = strFileName.substr(strFileName.rfind('.'));

	//记录有效编码的行数

	//记录有效字符以及其出现的次数
	string strCharCount;
	size_t lineCount = 0;
	char szCount[32] = { 0 };
	for (size_t i = 0; i < 256; ++i)
	{
		if (0 != _fileInfo[i]._count)
		{
			strCharCount += _fileInfo[i]._ch;
			strCharCount += ',';
			memset(szCount, 0, 32);
			_itoa(_fileInfo[i]._count, szCount, 10);
			strCharCount += szCount;
			strCharCount += '\n';
			lineCount++;
		}
	}
	string strHeaderInfo;
	strHeaderInfo += suffix;
	strHeaderInfo += '\n';

	_itoa(lineCount, szCount, 10);
	strHeaderInfo += szCount;
	strHeaderInfo += '\n';

	strHeaderInfo += strCharCount;

	fwrite(strHeaderInfo.c_str(), 1, strHeaderInfo.size(), pf);
}

void FileCompress::GetLine(FILE* pf, string& strContent)
{
	while (!feof(pf))
	{
		char ch = fgetc(pf);
		if ('\n' == ch)
			return;

		strContent += ch;
	}
}

