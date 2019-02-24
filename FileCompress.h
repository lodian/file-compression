#pragma once
#include "Huffman.hpp"
#include <string>
#include <vector>


typedef unsigned char UCH;
struct CharInfo
{
	UCH _ch;
	unsigned long long _count;
	std::string _strCode;

	CharInfo(unsigned long long count=0) 
		:_count(count)
	{}

	CharInfo operator+(const CharInfo& info)
	{
		return CharInfo(_count + info._count);
	}

	bool operator>(const CharInfo& info)
	{
		return _count > info._count;
	}

	bool operator!=(const CharInfo& info)const
	{
		return _count != info._count;
	}
};

class FileCompress
{
public:
	FileCompress();
	void CompressFile(const std::string &strFilePath);
	void UnCompressFile(const std::string &strFilePath);
private:
	void GetHuffmanCode(HTNode<CharInfo>* pRoot);
	void WriteHeaderInfo(FILE* pf, const std::string& strFileName);
	void GetLine(FILE* pf, std::string& strContent);
private:
	std::vector<CharInfo> _fileInfo;
};
