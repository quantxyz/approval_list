#ifndef _T_TREE_H_INCLUDED
#define _T_TREE_H_INCLUDED

#pragma warning(disable:4786)
#include <vector>
using namespace std;
#include "DataKeeper.h"


/* template <class Type> class NodeVector : private vector<Type> {
public:
	Type &operator[](int index) { return at(index);};
	const Type &operator[](int index) const { return at(index);};
	__declspec( property( get=get_Count) ) int Count;
	int get_Count() { return size();};
}; */
template <class Type,class DataType2=NodeData<Type> > class Tree;
template <class Type> class NodeData {
private:	
	typedef Ref<NodeData> NodeRef;
	NodeData(const Type &Data) : tData(Data){};
	NodeData(Type *pData) : tData(pData) {};
	NodeData() : Parent((NodeData*)NULL) {};
	virtual ~NodeData() {};
private:
	Ref<Type> tData;
	std::vector<NodeRef> vChilds;
	Ref<NodeData> Parent;
	friend class Tree<Type>;
	friend class Ref<NodeData>;
	friend _Datakeeper::Data<NodeData>;
};


/*
template <class T> class CScrollImpl;
template <class T, class TBase = CWindow, class TWinTraits = CContro1lWinTraits> class CScrollWindowImpl;
template <class T> class CMapScrollImpl;
template <class T, class TBase = CWindow, class TWinTraits = CControlWinTraits> class CMapScrollWindowImpl;

*/

template <class Type,class DataType2> class Tree : private Ref<DataType2> {
	typedef Ref<DataType2> NodeBase;
public:
	typedef Tree<Type> Node;
//	Node node;
	// vector<Node> &Nodes() {	return vChilds;	};
	Node Parent() { if(!NodeBase::get_Data().Parent.IsNull()) return NodeBase::get_Data().Parent; return *this;};
	operator Type&() { return (NodeBase::get_Data().tData.get_Data()); };
	Type *operator->() {return &(NodeBase::get_Data().tData.get_Data());};
	__declspec( property( get=get_Data) ) Ref<Type> Data;
	Ref<Type> get_Data() { return NodeBase::get_Data().tData;};

	bool IsLeaf() { return NodeBase::get_Data().vChilds.size()==0;};
	bool IsNode() { return NodeBase::get_Data().vChilds.size()>0;};
	bool IsRoot() { return NodeBase::get_Data().Parent.IsNull();};
	bool operator ==(const Node &rhs) { return *(NodeBase*)this==(NodeBase &)rhs;};

	__declspec( property( get=get_Nodes) ) Node Nodes[];
	Node get_Nodes(int nIndex) { return (Node)NodeBase::get_Data().vChilds[nIndex]; };
	__declspec( property( get=get_Count) ) int Count;
	int get_Count() { return NodeBase::get_Data().vChilds.size();};
	__declspec( property( get=get_Key) ) unsigned long Key;
	unsigned long get_Key() { return NodeBase::Key();};

	Node AddNode(const Type &t) { Node n(t); ((NodeBase &)n).get_Data().Parent=(*this); NodeBase::get_Data().vChilds.push_back(n); return n;};

	void DeleteNode(int nIndex) {
		NodeBase::get_Data().vChilds.erase(NodeBase::get_Data().vChilds.begin()+nIndex);
	};
	void DeleteNode(Node &node) {
		vector<NodeBase>::iterator = find(NodeBase::get_Data().vChilds.begin(),NodeBase::get_Data().vChilds.end(),node);
		if(iterator!=NodeBase::get_Data().vChilds.end()) {
			NodeBase::get_Data().vChilds.erase(iterator);
		}
	};
	void Delete() {
		if(!NodeBase::get_Data().Parent.IsNull()) NodeBase::get_Data().Parent.DeleteNode(*this);
		//Release();
	};
	Tree(const Type &Data) : NodeBase(Data) {};
	Tree(Type *Data) : NodeBase(Data) {};
	Tree() {};
	Tree(const Tree<Type> &rhs) : NodeBase(rhs) {};
	Tree(const NodeBase &rhs) : NodeBase(rhs) {};
	Tree(KEY key) : NodeBase(key) {};
	virtual ~Tree() 
	{	// if RefCount==ChildsCount+1, now is the time for release this (i.e. remove all refs to parent from childs
		ReleaseNode();
	};

private:
	void ReleaseNode() {
		if(NodeBase::IsNull()) return;
		ULONG nRefs=NodeBase::GetRefCount();
		int nChilds=NodeBase::get_Data().vChilds.size();
		if(nRefs==nChilds+1) {
			std::vector<NodeBase> &vChilds=NodeBase::get_Data().vChilds;
			for(ULONG n=0;n<vChilds.size();n++) {
				vChilds[n]->Parent.Release();
				((Node &)vChilds[n]).ReleaseNode();
			}
		}
		Release();
	}
};

// typedef Tree<t> Node<t>
//Tree::
// usage:
/*

*/
#endif