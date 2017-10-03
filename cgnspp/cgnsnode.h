/***************************************************************************
                          cgnsnode.h  -  description
                             -------------------
    begin                : Tue Sep 18 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSNODE_H
#define CGNSNODE_H

#include "cgns++.h"

#include "cgnsutil.h"

#include <memory>
#include <iostream>

enum {
	MAX_PHYSICAL_DIMENSION=3,	///< 3D space
	MAX_CELL_DIMENSION=3,		///< Cubes
	MAX_INDEX_DIMENSION=3		///< IJK ordered
};

#define COMMON_SIMPLE_NODE_INTERFACE(className, parentName, baseName)  \
	typedef parentName ParentClass;                                    \
	typedef baseName BaseClass;                                        \
	className(ADF::Node const & iNode, ParentClass * iParent);         \
	virtual ~className();                                              \
	void Wipe();                                                       \
	void Init()

	
/** Implementation class for CGNS::Node data */
struct CGNS::Private::Node {
	struct NonexistentNodeParent { enum { CHILDREN_MASK=0, CHILDREN_SUM=0 }; };
#if _MSC_VER > 1300
	// stupid MSC hack to circumvent dynamic_cast problem in DataArray constructor
	virtual bool MSC_amIParented() const { return false; }
#endif
COMMON_SIMPLE_NODE_INTERFACE(Node, Node, NonexistentNodeParent);
#ifdef __GNUC__	
	virtual void Emit(int iErr) const __attribute__((noreturn));
#else	
	virtual void Emit(int iErr) const;
#endif	
	int mReferenceCount;
	unsigned mInitialized;
	ADF::Node mNode;
	typedef child_container<CGNS::Descriptor> descriptors_t;
	descriptors_t mDescriptors;
	std::auto_ptr<int> mOrdinal;
	typedef child_container<CGNS::UserDefinedData> userarrays_t;
	userarrays_t mUserArrays;
	
    // hook for necessary initialisations
    void Modify() {}
    
	template<typename M>
	static void Read(M * me);
	template<typename M, typename T>
	static void Read(M * me, child_container<T> & cDest);
	template<typename M, typename T>
	static void Read(M * me, refcnt_ptr<T> & cDest);
	template<typename M, typename T>
	static void Read(M * me, std::auto_ptr<T> & cDest);
	
	template<typename M, typename T>
	static void Needs(M const * me, T const & cDest) {
		me->Needs(const_cast<M *>(me), const_cast<T &>(cDest)); }
	template<typename M>
	static void Needs(M const * me) { Needs(const_cast<M *>(me)); }
	template<typename M> static void Needs(M * me);
	template<typename M, typename T>
	static void Needs(M * me, T & cDest);
#ifdef HAVE_TEMPLATE_TEMPLATES	
	template<typename M, typename T, template<typename> class C>
	static void Needs(M * me, C<T>  & cDest);
#else
	template<typename M, typename T>
	static void Needs(M * me, child_container<T> & cDest);
	template<typename M, typename T>
	static void Needs(M * me, refcnt_ptr<T> & cDest);
	template<typename M, typename T>
	static void Needs(M * me, std::auto_ptr<T> & cDest);
#endif	

#ifndef HAVE_MIPSPRO	
	// these two cases are for debugging only
	template<typename M, typename T> static void Needs(M *, T const &);
	template<typename M, typename T> static void Needs(M const *, T &);
#endif	
	// specialisations		
	static void Read(Node * me, descriptors_t & cDest);
	static void Read(Node * me, std::auto_ptr<int> & cDest);

private:
DISTINCT_CHILDREN3(Descriptor, int, UserDefinedData);
};

template<typename M>
inline void CGNS::Private::Node::Read(M * me) {
//std::cerr << "Generic Node::Read(this) in " << __PRETTY_FUNCTION__ << ": " << me->mNode.getName() << std::endl;
//	me->mInitialized|=INIT_MASK_SELF;
}
template<typename M, typename T>
inline void CGNS::Private::Node::Read(M * me, child_container<T> & cDest) {
//std::cerr << "Generic Node::Read(child_container<T>) in " << __PRETTY_FUNCTION__ << ": " << me->mNode.getName() << std::endl;
	readLabeledChildren(me->mNode, implementation_traits<T>::Label(), cDest, me);
	me->mInitialized|=implementation_traits<T>::INIT_MASK;
}
template<typename M, typename T>
inline void CGNS::Private::Node::Read(M * me, std::auto_ptr<T> & cDest) {
//std::cerr << "Generic Node::Read(std::auto_ptr<T>) in " << __PRETTY_FUNCTION__ << ": " << me->mNode.getName() << std::endl;
	readStringChild(me->mNode, implementation_traits<T>::Name(), cDest);
	me->mInitialized|=implementation_traits<T>::INIT_MASK;
}
template<typename M, typename T>
inline void CGNS::Private::Node::Read(M * me, refcnt_ptr<T> & cDest) {
//std::cerr << "Generic Node::Read(refcnt_ptr<T>) in " << __PRETTY_FUNCTION__ << ": " << me->mNode.getName() << std::endl;
	readChild(me->mNode, implementation_traits<T>::Name(), cDest, me);
	me->mInitialized|=implementation_traits<T>::INIT_MASK;
}

template<typename M>
inline void CGNS::Private::Node::Needs(M * me) {
//std::cerr << "Generic Needs(this) : " << me->mNode.getName() << ": " << me->mInitialized << std::endl;
	if (me->mInitialized&INIT_MASK_SELF
		/*&& SStrategy<=ON_DEMAND_PREREAD*/) return;
	me->Read(me);
}

#if 1
template<typename M, typename T>
inline void CGNS::Private::Node::Needs(M * me, T  & cDest) {
//std::cerr << "Generic Node::Needs(T) in " << __PRETTY_FUNCTION__ << "\n" << me->mNode.getName() << std::endl;
	if (me->mInitialized&implementation_traits<T>::INIT_MASK
		/*&& SStrategy<=ON_DEMAND_PREREAD*/) return;
	me->Read(me, cDest);
}
#endif
#ifdef HAVE_TEMPLATE_TEMPLATES
template<typename M, typename T, template<typename> class C>
inline void CGNS::Private::Node::Needs(M * me, C<T>  & cDest) {
//std::cerr << "Generic Node::Needs(C<T>) in " << __PRETTY_FUNCTION__ << "\n" << me->mNode.getName() << std::endl;
	if (me->mInitialized&implementation_traits<T>::INIT_MASK
		/*&& SStrategy<=ON_DEMAND_PREREAD*/) return;
	me->Read(me, cDest);
}
#else
template<typename M, typename T>
inline void CGNS::Private::Node::Needs(M * me, child_container<T>  & cDest) {
	if (me->mInitialized&implementation_traits<T>::INIT_MASK
		/*&& SStrategy<=ON_DEMAND_PREREAD*/) return;
	me->Read(me, cDest);
}
template<typename M, typename T>
inline void CGNS::Private::Node::Needs(M * me, refcnt_ptr<T>  & cDest) {
	if (me->mInitialized&implementation_traits<T>::INIT_MASK
		/*&& SStrategy<=ON_DEMAND_PREREAD*/) return;
	me->Read(me, cDest);
}
template<typename M, typename T>
inline void CGNS::Private::Node::Needs(M * me, std::auto_ptr<T>  & cDest) {
	if (me->mInitialized&implementation_traits<T>::INIT_MASK
		/*&& SStrategy<=ON_DEMAND_PREREAD*/) return;
	me->Read(me, cDest);
}
#endif

#define COMMON_SIMPLE_NODE_IMPLEMENTATION(className)           \
CGNS::Private::className::className(ADF::Node const & iNode,   \
	ParentClass * iParent) : BaseClass(iNode, iParent) {}      \
CGNS::Private::className::~className() {}                      \
CGNS::className::className(CGNS::Private::className * iImpl)   \
	: CGNS::Private::implementation_traits<CGNS::Private::className::BaseClass>::Node(iImpl) {} \
template struct CGNS::refcnt_ptr<CGNS::Private::className>

#ifdef HAVE_USING_BASE_MEMBERS
#define USING_READ(base) using base::Read
#define USING_NEEDS(base) using base::Needs
#else
#define USING_READ(base) \
template<typename Me, typename T> \
static void Read(Me * me, T & cDest) { base::Read(me, cDest); } \
template<typename Me> \
static void Read(Me * me) { base::Read(me); }
#define USING_NEEDS(base) \
template<typename Me, typename T> \
static void Needs(Me * me, T & cDest) { base::Needs(me, cDest); } \
template<typename Me> \
static void Needs(Me * me) { base::Needs(me); }
#endif

#endif
