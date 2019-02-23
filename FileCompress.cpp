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
	//	1.ͳ��Դ�ļ���ÿ���ַ����ֵĴ���
	FILE* fIn = fopen(strFilePath.c_str(), "rb");
	if (nullptr == fIn)
	{
		cout << "���ļ�ʧ��" << endl;
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
		
	//2.��ÿ���ַ����ֵĴ���ΪȨֵ����Huffman��
	HuffmanTree<CharInfo> ht;
	ht.CreateHuffmanTree(_fileInfo, CharInfo(0));

	//3.ͨ��Huffman����ȡÿ���ַ��ı���
	GetHuffmanCode(ht.GetRoot());

	//4.�û�ȡ����Huffman���������¸�д�ļ�
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
			//��ÿ���ַ��ı���ŵ�ch�ı���λ��
			for (size_t j = 0; j < strCode.size(); ++j)
			{
				ch <<= 1;//���һ������λ�ź�֮���´ν���Ӧ����������һλ
				if ('1' == strCode[j])
					ch |= 1;//��λ������Ϊ1�����������һ������λΪ1
				//elseΪ0�����һ������λ��������0
				bitCount++;
				if (8 == bitCount)
				{
					//ch��8������λ���ź�֮���д�뵽�ļ���
					fputc(ch, fOut);
					ch = 0;
					bitCount = 0;
				}
			}
		}
	}
	//���һ�ε�ch��8������λû�������
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
		cout << "ѹ���ļ���ʽ��ƥ�䣡" << endl;
		return;
	}
	FILE* fIn = fopen(strFilePath.c_str(), "rb");
	if (nullptr == fIn)
	{
		cout << "ѹ���ļ����ļ�ʧ��" << endl;
		return;
	}
	//��ѹ���ļ��л�ȡԴ�ļ��ĺ�׺
	suffix = "";
	GetLine(fIn, suffix);

	//��Դ�ļ��л�ȡ�ַ�������Ϣ
	string strContent;
	GetLine(fIn, strContent);
	size_t lineCount = atoi(strContent.c_str());

	size_t charCount = 0;
	for (size_t i = 0; i < lineCount; ++i)
	{
		strContent = "";
		GetLine(fIn, strContent);
		if (strContent == "")//��GetLine�����������з���ֱ��return������strContent=""��������Դ�ļ��п��ܾ���һ�����з�
		{
			strContent += '\n';
			GetLine(fIn, strContent);
		}
		charCount = atoi(strContent.c_str() + 2);
		_fileInfo[(UCH)strContent[0]]._count = charCount;//��Ȼ�Ѿ���_ch���ó��޷���char���ͣ����������strContent��string�࣬�õ���Ҳ�����Ǹ���������Ҫ����ǿת
	}
	//��ԭHuffman��
	HuffmanTree<CharInfo> ht;
	ht.CreateHuffmanTree(_fileInfo, CharInfo(0));

	//��ѹ��
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
			//��ѹ����ǰ�ֽڵ�ѹ������
			pos = 7;
			for (size_t j = 0; j < 8; ++j)
			{
				if (pReadBuff[i] & (1 << pos))//���μ��鵱ǰ�ֽڵ�8������λ�Ƿ�Ϊ1
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
	//��¼Դ�ļ���׺
	string suffix = strFileName.substr(strFileName.rfind('.'));

	//��¼��Ч���������

	//��¼��Ч�ַ��Լ�����ֵĴ���
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

