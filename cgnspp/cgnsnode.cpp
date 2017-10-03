/***************************************************************************
                          cgnsnode.cpp  -  description
                             -------------------
    begin                : Mon Mar 12 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsnode.h"

#include "cgnsuserdefineddata.h"

#include <iostream>
#include <assert.h>

GENERATE_D(Node);

void CGNS::Private::Node::Emit(int iErr) const {
	// throw only if no exception is active
	std::cerr << "Node::Emit in " << mNode.getName();
//	if (P()) std::cerr << ", parent is " << P()->mNode.getName();
	std::cerr << std::endl;
	if (!std::uncaught_exception()) throw Exception(iErr);
	exit(-1);
}

std::string const & CGNS::Descriptor::getName() const { return mName; }

std::string const & CGNS::Descriptor::getContent() const { return mContent; }

void CGNS::Descriptor::readData(std::string * oData) const { *oData=mContent; }

void CGNS::Descriptor::readData(char * oData) const {
	mContent.copy(oData, std::string::npos);
	oData[mContent.length()]=0;
}

void CGNS::Descriptor::readData(signed char * oData) const {
	mContent.copy(reinterpret_cast<char *>(oData), std::string::npos);
	oData[mContent.length()]=0;
}

void CGNS::Descriptor::readData(unsigned char * oData) const {
	mContent.copy(reinterpret_cast<char *>(oData), std::string::npos);
	oData[mContent.length()]=0;
}

void CGNS::Descriptor::readData(void * oData) const {
	char * data=static_cast<char *>(oData);
	mContent.copy(data, std::string::npos);
	data[mContent.length()]=0;
}

using CGNS::Node;

CGNS::Private::Node::Node(ADF::Node const &iNode, Node * /*iParent*/)
	: mReferenceCount(0), mInitialized(0), mNode(iNode) {}

CGNS::Private::Node::~Node() {}

void CGNS::Private::Node::Read(Node * me, descriptors_t & cDest) {
	assert(&cDest==&me->mDescriptors);
	Descriptor descriptor;
	for (ADF::Node::Child_iterator_t i=me->mNode.beginChild(); i!=me->mNode.endChild(); ++i)
		if (i->getLabel()==implementation_traits<Descriptor>::Label())
		{
			const_cast<std::string &>(descriptor.mName)=i->getName();
			i->readData(const_cast<std::string *>(&descriptor.mContent));
			cDest.insert(descriptor);
		}
	me->mInitialized|=implementation_traits<Descriptor>::INIT_MASK;
}

void CGNS::Private::Node::Read(Node * me, std::auto_ptr<int> & cDest) {
	assert(&cDest==&me->mOrdinal);
	readChild(me->mNode, "Ordinal", "Ordinal_t", cDest);	
	me->mInitialized|=implementation_traits<int>::INIT_MASK;
}

void CGNS::Private::Node::Init() {
	Needs(this, mDescriptors);
	Needs(this, mOrdinal);	
	Needs(this, mUserArrays);
}

void CGNS::Private::Node::Wipe() {
	wipe(mUserArrays);
	wipe(mOrdinal);
	wipe(mDescriptors);
	mNode.deleteChildren();
}

template struct CGNS::iterator<std::string>;
template struct CGNS::refcnt_ptr<CGNS::Private::Node>;

Node::Node(CGNS::Private::Node * iImpl) : mImpl(iImpl) {}

Node::Node(Node const & iRhs) : mImpl(iRhs.mImpl) {}

Node::~Node() {}

void Node::flush() {
	// delegate to ADF::Node
	D()->mNode.flush();
}

std::string Node::getName() const {
	return D()->mNode.getName();
}

std::string Node::getLabel() const {
	return D()->mNode.getLabel();
}

int Node::getNumDescriptors() const {
	NEEDS(mDescriptors);
	return D()->mDescriptors.size();
}

std::string Node::getDescriptorText(std::string const & iDescriptorName) const {
	NEEDS(mDescriptors);
	if (!D()->mDescriptors.has(iDescriptorName)) Private::Emit(Exception::NO_NAMED_CHILD);
	std::string text;
	D()->mDescriptors.find(iDescriptorName).readData(&text);
	return text;
}

bool Node::hasDescriptor(std::string const & iDescriptorName) const {
	NEEDS(mDescriptors);
	return D()->mDescriptors.has(iDescriptorName);
}

std::string const & Node::writeDescriptor(std::string const & iDescriptorName,
	std::string const & iDescriptorText) {
	NEEDS(mDescriptors);
	if (!D()->mDescriptors.has(iDescriptorName)) {
		D()->mNode.createChild(iDescriptorName,
			Private::implementation_traits<Descriptor>::Label());
		D()->mDescriptors.insert(Descriptor(iDescriptorName, iDescriptorText));
	}	
	D()->mNode.getChild(iDescriptorName).writeData(iDescriptorText);
	return D()->mDescriptors.find(iDescriptorName).getContent();
}

void Node::deleteDescriptor(std::string const & iDescriptorName) {
	NEEDS(mDescriptors);
	if (D()->mDescriptors.has(iDescriptorName))
		D()->mDescriptors.erase(iDescriptorName);
	if (D()->mNode.hasChild(iDescriptorName))
		D()->mNode.deleteChild(iDescriptorName);
}

template struct CGNS::iterator<CGNS::Descriptor>;

Node::Descriptor_iterator_t Node::beginDescriptor() const {
	NEEDS(mDescriptors);
	Private::Node::descriptors_t::const_iterator i(D()->mDescriptors.begin());
	return Descriptor_iterator_t(*reinterpret_cast<void **>(&i));
}

Node::Descriptor_iterator_t Node::endDescriptor() const {
	NEEDS(mDescriptors);
	Private::Node::descriptors_t::const_iterator i(D()->mDescriptors.end());
	return Descriptor_iterator_t(*reinterpret_cast<void **>(&i));
}

OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION(Ordinal, int, Node, D()->mOrdinal);

int Node::writeOrdinal(int iOrdinal) {
	NEEDS(mOrdinal);
	ADF::Node n(Private::assureChild(D()->mOrdinal, D(), iOrdinal));
	n.setDataTypeDimensions(ADF::I4, 1);
	n.writeData(&iOrdinal);
	return iOrdinal;
}

N_CHILDS_IMPLEMENTATION_WRITE(UserDefinedData, UserDefinedData, Node, D()->mUserArrays);

double Node::getDatabaseId() const { return D()->mNode.getId(); }

