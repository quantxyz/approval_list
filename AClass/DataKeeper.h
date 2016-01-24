#ifndef _DATA_KEEPER_H
#define _DATA_KEEPER_H

namespace _Datakeeper {

template <class Type> class Ref;

template <class Type> class Data {
private:
	typedef unsigned long ULONG;
	Data(Type *p=NULL) { if(!p) p=new Type; pData=p; dwRefs=1;};
	ULONG AddRef() { dwRefs++; return dwRefs;};
 	ULONG Release() { 
		dwRefs--; if(dwRefs==0) {delete this;return 0;} return dwRefs;};

// accessors
	Type *operator->() { return pData;};
	operator Type&() { return *pData;};
	Type &get_Data() { return *pData;};
	~Data() { if(pData) delete pData;};
	// op new
	//
	Type *pData;
	ULONG dwRefs;
	friend class Ref<Type>;
};

template <class Type> class Ref {
	typedef Data<Type> DataT;
	
public:
	typedef unsigned long ULONG;
	typedef unsigned long KEY;
	// ctors
	Ref(const Type &t) { pData=new DataT(new Type(t));};
	Ref(Type *p) { if(p) pData=new DataT(p); else pData=NULL;};
	Ref() { pData=new DataT();};
	//Ref(Data<Type> *p) : pData(p) { if(p) p->AddRef();};
	Ref(const Ref<Type> &rhs) { pData=(rhs.pData); if(pData) pData->AddRef();};
	//dtor
	virtual ~Ref() {if(pData) pData->Release();};

	//operators =
	Ref &operator=(const Ref<Type> &rhs) 
	{ Release(); pData=rhs.pData; if(pData) pData->AddRef(); return *this;};
	Ref &operator=(const Type &t) 
	{ Release(); pData=new DataT(new Type(t)); return *this;};
	Ref &operator=(Type *p) 
	{ Release(); if(p) pData=new DataT(p); else pData=NULL; return *this;};
	// operator ==
	bool operator==(const Ref<Type> &rhs) 
	{ return pData==rhs.pData;};
	
	Ref Clone() { if(!pData) return Ref(); return Ref(pData->get_Data());};
	void Release() { if(pData) pData->Release(); pData=NULL;};
	bool IsNull() { return pData==NULL;};

// accessors	
	Type *operator->() {return pData->operator->();};
	Type &get_Data() { return pData->get_Data();};
	operator Type&() { return pData->get_Data();};
	KEY Key() { return ((KEY)pData)+1;};
protected: 
	ULONG GetRefCount() { if(!pData) return 0; return pData->dwRefs;};
	Ref(KEY key) { try{pData=(DataT*)(key-1);if(pData) pData->AddRef();}catch(...){pData=NULL;}};
private:
	Data<Type> *pData;
	friend class Ref<Type>;
// niama new	
};

}// namespace _Datakeeper

using _Datakeeper::Ref;

#endif