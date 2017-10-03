/***************************************************************************
                          cgnsparented.h  -  description
                             -------------------
    begin                : Thu May 23 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSPARENTED_H
#define CGNSPARENTED_H

#include "cgnsnode.h"

#define COMMON_NODE_INTERFACE(className, parentName, baseName) \
COMMON_SIMPLE_NODE_INTERFACE(className, parentName, baseName); \
private:                                                       \
	ParentClass * P() {                                        \
		return static_cast<ParentClass *>(mParent); }          \
	ParentClass const * P() const {                            \
		return static_cast<ParentClass const *>(mParent); }    \
public:                                                        \
	static void declare_really_silly_dummy_function()

/** Node implementation with parent backlink
  */
struct CGNS::Private::Parented : CGNS::Private::Node  {
COMMON_SIMPLE_NODE_INTERFACE(Parented, Parented, Node);
public:	
#ifdef __GNUC__	
	virtual void Emit(int iErr) const __attribute__((noreturn));
#else	
	virtual void Emit(int iErr) const;
#endif	
#if _MSC_VER > 1300
	// stupid MSC hack to circumvent dynamic_cast problem in DataArray constructor
	virtual bool MSC_amIParented() const { return true; }
#endif
	// here public, to be accessible later on
	ParentClass * P() {
		return static_cast<ParentClass *>(mParent); }
	ParentClass const * P() const {
		return static_cast<ParentClass const *>(mParent); }
protected:	
	Parented * mParent;
DISTINCT_CHILDREN0();
};

#define COMMON_NODE_IMPLEMENTATION(className)                \
CGNS::Private::className::className(ADF::Node const & iNode, \
	ParentClass * iParent) : BaseClass(iNode, iParent) {}    \
CGNS::Private::className::~className() {}                    \
CGNS::className::className(CGNS::Private::className * iImpl) \
	: CGNS::Private::implementation_traits<CGNS::Private::className::BaseClass>::Node(iImpl) {} \
template struct CGNS::refcnt_ptr<CGNS::Private::className>
	
#endif
