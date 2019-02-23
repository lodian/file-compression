#pragma once 
#include <queue>
#include <vector>

template<class W>
struct HTNode
{
	HTNode(const W& weight) 
		:_pLeft(nullptr)
		,_pRight(nullptr)
		, _pParent(nullptr)
		,_weight(weight)
	{}

	HTNode<W>* _pLeft;
	HTNode<W>* _pRight;
	HTNode<W>* _pParent;
	W _weight;
};

//仿函数
template<class W>
struct Compare
{
	bool operator()(HTNode<W>* pLeft, HTNode<W>* pRight)
	{
		if (pLeft->_weight > pRight->_weight)//由>=改为>，因为文本内容为“ABBBCCCCCCDDDDDDD你好”的时候，后面有186,195,196...等CharInfo的出现，而这些出现次数都为1，在插入优先级队列的时候会崩溃
			return true;

		return false;
	}
};

template<class W>
class HuffmanTree
{
	typedef HTNode<W> Node;
	typedef HTNode<W>* PNode;
public:
	HuffmanTree() 
		:_pRoot(nullptr)
	{}

	~HuffmanTree()
	{
		Destroy(_pRoot);
	}

	void CreateHuffmanTree(const std::vector<W>& v, const W& invalid)
	{
		if (v.empty())
			return;

		//小堆--创建二叉树森林
		std::priority_queue<PNode, std::vector<PNode>, Compare<W>> hp;
		for (size_t i = 0; i < v.size(); ++i)
		{
			if (v[i]!=invalid)	//出现0次的字符不必放到二叉树中
				hp.push(new Node(v[i]));
		}
		while (hp.size()>1)
		{
			//从堆中取权值最小的两棵二叉树
			PNode pLeft = hp.top();
			hp.pop();

			PNode pRight = hp.top();
			hp.pop();

			//以pLeft与pRight的权值之和创建新节点
			PNode pParent = new Node(pLeft->_weight + pRight->_weight);
			pParent->_pLeft = pLeft;
			pLeft->_pParent = pParent;

			pParent->_pRight = pRight;
			pRight->_pParent = pParent;

			hp.push(pParent);
		}
		_pRoot = hp.top();
	}

	PNode GetRoot()
	{
		return _pRoot;
	}

private:
	void Destroy(PNode& pRoot)
	{
		if (pRoot)
		{
			Destroy(pRoot->_pLeft);
			Destroy(pRoot->_pRight);
			delete pRoot;
			pRoot = nullptr;
		}
	}
private:
	PNode _pRoot;
};