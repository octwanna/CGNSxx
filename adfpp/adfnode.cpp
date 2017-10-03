/***************************************************************************
                          adfnode.cpp  -  description
                             -------------------
    begin                : Mon Mar 12 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "adfnode.h"

#include "ADF.h"

#include <iostream>
#include <map>
#include <assert.h>

using ADF::Node;

template struct ADF::iterator<ADF::Node>;
template struct ADF::refcnt_ptr<ADF::Node::Private>;

typedef std::map<double, Node::Private *> LookupMap_t;
typedef LookupMap_t::iterator LookupMap_i;
static LookupMap_t SLookupMap;

/** Enters the id and impl pointer into the map. */
static void enterEntry(double iId, Node::Private * iNode) {
	// double entries shouldn't happen
	if (SLookupMap.find(iId)!=SLookupMap.end()) {
		std::cerr << "enterEntry:: doubly enter\n";		
		ADF::Emit(ADF::Exception::INTERNAL_ERROR);
	}
	SLookupMap[iId]=iNode;
}
	
/** Deletes the id and impl pointer from the map. */
static void deleteEntry(double iId, Node::Private * /*iNode*/) {
	SLookupMap.erase(iId);
}
	
/** Asks if the id is already in the map. */
/*
static bool hasEntry(double iId) {
	return SLookupMap.find(iId)!=SLookupMap.end();
}
*/
/** Looks the id up in the map and returns the (possibly new created) impl pointer. */
static Node::Private * getEntry(double iId) {
	LookupMap_i iter=SLookupMap.find(iId);
	if (iter==SLookupMap.end())
		return new Node::Private(iId);
	assert(iter->second);
	return iter->second;
}

/** Looks the id up in the map and returns the corresponding impl pointer. */
static Node::Private * lookupEntry(double iId) {
	LookupMap_i iter=SLookupMap.find(iId);
	// should be there always
	if (iter==SLookupMap.end()) {
		std::cerr << "lookupEntry:: not found\n";		
		ADF::Emit(ADF::Exception::INTERNAL_ERROR);
	}
	assert(iter->second);
	return iter->second;
}

Node::Private::Initialized Node::Private::Or(Node::Private::Initialized lhs,
	Node::Private::Initialized rhs) {
	return static_cast<Node::Private::Initialized>(
		static_cast<int>(lhs)|static_cast<int>(rhs));
}

Node::Private::Initialized const & Node::Private::OrEq(Node::Private::Initialized & lhs,
	Node::Private::Initialized rhs) {
	return lhs=Or(lhs,rhs);
}

Node::Private::Initialized Node::Private::And(Node::Private::Initialized lhs,
	Node::Private::Initialized rhs) {
	return static_cast<Node::Private::Initialized>(static_cast<int>(lhs)&static_cast<int>(rhs));
}

Node::Private::Initialized const & Node::Private::AndEq(Node::Private::Initialized & lhs,
	Node::Private::Initialized rhs) {
	return lhs=And(lhs,rhs);
}

Node::Private::Initialized Node::Private::Inv(Node::Private::Initialized rhs) {
	return static_cast<Node::Private::Initialized>(
		static_cast<int>(Node::Private::INIT_ALL)&~static_cast<int>(rhs));
}

Node::Private::Initialized const & Node::Private::InvEq(Node::Private::Initialized & lhs) {
	return lhs=Inv(lhs);
}


// check with header file!
static char const * dataTypeString[] = { "MT", "I4", "I8", "U4", "U8", "R4", "R8", "X4", "X8",
	"C1", "B1", "LK", "STRUCT" };
CT_ASSERT(ADF::MT==0 && ADF::I4==1 && ADF::I8==2 && ADF::U4==3 && ADF::U8==4
	&& ADF::R4==5 && ADF::R8==6 && ADF::X4==7 && ADF::X8==8
	&& ADF::C1==9 && ADF::B1==10 && ADF::LK==11 && ADF::STRUCT==12);
	
std::string ADF::asString(DataType iType) {
	if (iType<MT || iType>STRUCT) return "<unknown data type>";
	else return dataTypeString[iType];
}

static ADF::DataType stringToType(std::string const & iType) {
	for (int t=ADF::MT; t<=ADF::STRUCT; ++t)
		if (iType==dataTypeString[t])
			return static_cast<ADF::DataType>(t);
	ADF::Emit(ADF::Exception::INVALID_DATA_TYPE);
	return ADF::MT;
}
		
static void checkIndex(int iIndex, int iMax, int iMin=0) {
	if (iIndex<iMin) ADF::Emit(ADF::Exception::NUMBER_LESS_THAN_MINIMUM);
	if (iIndex>=iMax) ADF::Emit(ADF::Exception::NUMBER_GREATER_THAN_MAXIMUM);
}

static void checkString(std::string const & iString, int iMaxLength=0) {
	if (iString.size()==0) ADF::Emit(ADF::Exception::STRING_LENGTH_ZERO);
	if (iMaxLength && static_cast<int>(iString.size())>iMaxLength)
		ADF::Emit(ADF::Exception::STRING_LENGTH_TOO_BIG);
}

template<typename T>
void checkPointer(T * iPointer) {
	if (!iPointer) ADF::Emit(ADF::Exception::NULL_POINTER);
}

static int implCount=0;

Node::Private::Private(double iId) : mReferenceCount(0), mId(iId), mInitialized(INIT_NONE) {
	enterEntry(mId, this);
}

Node::Private::~Private() {
if (mReferenceCount !=0) std::cerr << "oops, reference count is still " << mReferenceCount << std::endl;	
	deleteEntry(mId, this);
}

bool Node::Private::markDeleted() {
	if (mInitialized==DELETED) return false;
	// get rid of the child node objects
	mChildren.clear();
	mInitialized=DELETED;	
	return true;
}

void Node::Private::checkInitialized(Initialized iInitialized) const {
	if (mInitialized==DELETED) Emit(Exception::DELETED_NODE);
	Initialized todo=And(iInitialized, Inv(mInitialized));
	if (todo) const_cast<Private *>(this)->update(todo);
}

void Node::Private::update(Initialized iUpdate) {
	if (mInitialized==DELETED) Emit(Exception::DELETED_NODE);
	if (iUpdate&INIT_NAME) {
		mName[0]=0;
		ADF_Get_Name(mId, mName, Checker());
		OrEq(mInitialized, INIT_NAME);
	}
	if (iUpdate&INIT_LABEL) {
		mLabel[0]=0;
		ADF_Get_Label(mId, mLabel, Checker());
		OrEq(mInitialized, INIT_LABEL);
	}
	if (iUpdate&INIT_TYPE) {
		char type[DATA_TYPE_LENGTH+1];
		mDataType=MT;
		ADF_Get_Data_Type(mId, type, Checker());
		mDataType=stringToType(type);
		OrEq(mInitialized, INIT_TYPE);
	}
	if (iUpdate&INIT_DIM) {
		mNumDimensions=0;
		for (int i=0; i<MAX_DIMENSIONS; ++i) mDimensions[i]=0;
		ADF_Get_Number_of_Dimensions(mId, &mNumDimensions, Checker());
		ADF_Get_Dimension_Values(mId, mDimensions, Checker());
		OrEq(mInitialized, INIT_DIM);
	}
	if (iUpdate&INIT_CHILD) {
		int numChildren=0;
		ADF_Number_of_Children(mId, &numChildren, Checker());
		children_t children;
		for (int i=0; i<numChildren; ++i) {
			int numRet=0;
			char childName[NAME_LENGTH+1];
			ADF_Children_Names(mId, i+1, 1, NAME_LENGTH+1,
				&numRet, childName, Checker());
			if (numRet!=1) {
				std::cerr << "update(INIT_CHILD): didn't get one\n";		
				Emit(Exception::INTERNAL_ERROR);
			}
			double childId=0;
			ADF_Get_Node_ID(mId, childName, &childId, Checker());
			children.insert(getEntry(childId));
		}
		// make the new children list current
		mChildren.swap(children);
		// get rid of possibly lost children
		// done by the destructor of children_t
		OrEq(mInitialized,INIT_CHILD);
	}
}

void Node::Private::setName(std::string const & iName) {
	int len=std::min(iName.length(), static_cast<size_t>(NAME_LENGTH));
	iName.copy(mName, len);
	mName[len]=0;
}

void Node::Private::setLabel(std::string const & iLabel) {
	int len=std::min(iLabel.length(), static_cast<size_t>(LABEL_LENGTH));
	iLabel.copy(mLabel, len);
	mLabel[len]=0;
}

bool Node::Private::hasName(std::string const & iName) {
	return iName==mName;
}

bool Node::Private::hasLabel(std::string const & iLabel) {
	return iLabel==mLabel;
}

static int nodeCount=0;

Node::Node(Node const & rhs) : mImpl(rhs.mImpl) {
	++nodeCount;
}
	
Node::Node(double iId) : mImpl(getEntry(iId)) {
	++nodeCount;
}

Node::Node(Private * iPrivate) : mImpl(iPrivate) {
	++nodeCount;
}

Node::~Node() {
	--nodeCount;
}

void Node::flush() {
	ADF_Flush_to_Disk(mImpl->mId, Checker());
}

double Node::getId() const {
	return mImpl->mId;
}

double Node::createBareChild(double iParent,
	std::string const & iName, std::string const & iLabel) {
	checkString(iName, NAME_LENGTH);
	double childId=0;
	ADF_Create(iParent, iName.c_str(), &childId, Checker());
	if (iLabel.size()!=0) {
		checkString(iLabel, LABEL_LENGTH);
		ADF_Set_Label(childId, iLabel.c_str(), Checker());
	}
	return childId;
}

double Node::createBareChild(std::string const & iName, std::string const & iLabel) {
	return createBareChild(mImpl->mId, iName, iLabel);
}

Node Node::createChild(std::string const & iChildName, std::string const & iLabel) {
	createBareChild(iChildName, iLabel);
	/* Update child list myself or let Update do the work???
	   Better use Update to keep a consistent (with ADF) child numbering. */
	mImpl->update(Private::INIT_CHILD);
	return getChild(iChildName);	
}

void Node::deleteChild(std::string const & iChildName) {
	Node child(getChild(iChildName));
	deleteChild(child);
}

void Node::deleteChild(Node & iChildNode) {
	if (!hasChild(iChildNode)) Emit(Exception::CHILD_NOT_OF_GIVEN_PARENT);
	// delete the ADF node only if not yet gone
	if (iChildNode.mImpl->markDeleted())
		ADF_Delete(mImpl->mId, iChildNode.getId(), Checker());
	// the child node object gets deleted by Update!
	mImpl->update(Private::INIT_CHILD);
}

void Node::deleteChildren() {
	for (Child_iterator_t i=beginChild(); i!=endChild(); ++i)
		if (i->mImpl->markDeleted())
			ADF_Delete(mImpl->mId, i->getId(), Checker());
	mImpl->mChildren.clear();
	// the child node object gets deleted by Update!
	mImpl->update(Private::INIT_CHILD);
}

int Node::getNumChildren() const {
	mImpl->checkInitialized(Private::INIT_CHILD);
	return mImpl->mChildren.size();
}

Node Node::getChild(std::string const & iChildName) const {
	checkString(iChildName, NAME_LENGTH);
	mImpl->checkInitialized(Private::INIT_CHILD);
	if (!mImpl->mChildren.has(iChildName)) {
		std::cerr << "No child " << iChildName << " available only:\n";
		for (Child_iterator_t i=beginChild(); i!=endChild(); ++i)
			std::cerr << "\t" << i->getName() << std::endl;
		Emit(Exception::CHILD_NOT_OF_GIVEN_PARENT);
	}
	return mImpl->mChildren.find(iChildName);
}

Node::Child_iterator_t Node::beginChild() const {
	mImpl->checkInitialized(Private::INIT_CHILD);
	Private::children_t::const_iterator i(mImpl->mChildren.begin());
	return Node::Child_iterator_t(*reinterpret_cast<void **>(&i));
}

Node::Child_iterator_t Node::endChild() const {
	mImpl->checkInitialized(Private::INIT_CHILD);
	Private::children_t::const_iterator i(mImpl->mChildren.end());
	return Node::Child_iterator_t(*reinterpret_cast<void **>(&i));
}

int Node::getNumLabeledChildren(std::string const & iLabel) const{
	mImpl->checkInitialized(Private::INIT_CHILD);
	int count=0;
	for (Child_iterator_t i=beginChild(); i!=endChild(); ++i)
		if( i->getLabel()==iLabel) ++count;
	return count;
}

Node Node::getLabeledChild(std::string const & iChildLabel) const {
	mImpl->checkInitialized(Private::INIT_CHILD);
	for (Child_iterator_t i=beginChild(); i!=endChild(); ++i)
		if( i->getLabel()==iChildLabel) return *i;
	Emit(Exception::CHILD_NOT_OF_GIVEN_PARENT);
	return *beginChild();
}

bool Node::hasChild(std::string const & iChildName) const {
	checkString(iChildName, NAME_LENGTH);
	mImpl->checkInitialized(Private::INIT_CHILD);
	return mImpl->mChildren.has(iChildName);	
}

bool Node::hasChild(Node const & iChildNode) const {
	mImpl->checkInitialized(Private::INIT_CHILD);
	if (!mImpl->mChildren.has(iChildNode.getName())) return false;
	return mImpl->mChildren.find(iChildNode.getName()).mImpl==iChildNode.mImpl;
}

bool Node::hasChild(double iChildId) const {
	mImpl->checkInitialized(Private::INIT_CHILD);
	for (Private::children_t::const_iterator i=mImpl->mChildren.begin();
		i!=mImpl->mChildren.end(); ++i)
		if (iChildId==i->second.getId())
			return true;
	return false;
}

std::string Node::getName() const {
 	mImpl->checkInitialized(Private::INIT_NAME);
	return mImpl->mName;
}

void Node::renameChild(std::string const & iOldName, std::string const & iName) {
	Node child(getChild(iOldName));
	renameChild(child, iName);
}

void Node::renameChild(Node & iChildNode, std::string const & iName) {
	checkString(iName, NAME_LENGTH);
	if (!hasChild(iChildNode)) Emit(Exception::CHILD_NOT_OF_GIVEN_PARENT);
	// --- check new name ---
	if(mImpl->mChildren.has(iName)) Emit(Exception::INVALID_NODE_NAME);

	ADF_Put_Name(mImpl->mId, iChildNode.getId(), iName.c_str(), Checker());
	iChildNode.mImpl->setName(iName);
  
	mImpl->update(Private::INIT_CHILD);
}

void Node::reparentChild(std::string const & iChildName, Node & iNewParent) {
	reparentChild(getChild(iChildName), iNewParent);
}

void Node::reparentChild(Node const & iChildNode, Node & iNewParent) {
	ADF_Move_Child(mImpl->mId, iChildNode.getId(), iNewParent.getId(), Checker());
	// can't use simple update here, since otherwise the iChildNode would
	// be lost (i.e. deleted and recreated at probably another adress)
	// therefore we delete it from the our child table, which is simple
	// since we just clear it and declare it as uninitialized
	// the new parent may now pick the child up
	iNewParent.mImpl->update(Private::INIT_CHILD);
	// finally we can update our own child table
	mImpl->update(Private::INIT_CHILD);
}

Node Node::createLink(std::string const & iChildName, std::string const & iLinkedToName) {
	return createLink(iChildName, iLinkedToName, "");
}

Node Node::createLink(std::string const & iChildName, Node const & iLinkedToNode) {
	return createLink(iChildName, iLinkedToNode.getName(), "");
}

Node Node::createLink(std::string const & iChildName,
	std::string const & iLinkedToName, std::string const & iLinkedToFile) {
	checkString(iChildName, NAME_LENGTH);
	checkString(iLinkedToName, MAX_LINK_DATA_SIZE);
	double childId=0;
	char const *file=iLinkedToFile.c_str();
	// ADF needs zero pointer to indicate same file as target
	if (!*file) file=0;
	ADF_Link(mImpl->mId, iChildName.c_str(), file, iLinkedToName.c_str(),
		&childId, Checker());
	// Use update for the same reasons as for CreateChild
	mImpl->update(Private::INIT_CHILD);
	return getChild(iChildName);
}

bool Node::isLink() const {
	int linkPathLength=0;
	ADF_Is_Link(mImpl->mId, &linkPathLength, Checker());
	return linkPathLength!=0;
}

std::string Node::getLinkFile() const {
	static char file[FILENAME_LENGTH+1];
	static char path[MAX_LINK_DATA_SIZE+1];
	ADF_Get_Link_Path(mImpl->mId, file, path, Checker());
	return file;
}

std::string Node::getLinkPath() const {
	static char file[FILENAME_LENGTH+1];
	static char path[MAX_LINK_DATA_SIZE+1];
	ADF_Get_Link_Path(mImpl->mId, file, path, Checker());
	return path;
}
	
Node Node::getRootNode() const {
	double rootId=0;
	ADF_Get_Root_ID(mImpl->mId, &rootId, Checker());
	return lookupEntry(rootId);
}

std::string Node::getLabel() const {
	mImpl->checkInitialized(Private::INIT_LABEL);
	return mImpl->mLabel;
}

void Node::setLabel(std::string const & iLabel) {
	// an empty label is allowed, therefore can't use checkString
	if (iLabel.size()>LABEL_LENGTH) Emit(Exception::STRING_LENGTH_TOO_BIG);
	ADF_Set_Label(mImpl->mId, iLabel.c_str(), Checker());
	mImpl->setLabel(iLabel);
}

ADF::DataType Node::getDataType() const {
	mImpl->checkInitialized(Private::INIT_TYPE);
	return mImpl->mDataType;
}

int Node::getDataSize() const {
	int numDim=getNumDimensions();
	if (numDim<=0) return 0;
	int size=DataSize(getDataType());
	for (int i=0; i<numDim; ++i)
		size*=mImpl->mDimensions[i];
	return size;
}

/*
std::string Node::getDataTypeString() const {
	return datatypeString[getDataType()];
}
*/

void Node::setDataType(ADF::DataType iType) {
	ADF_Put_Dimension_Information(mImpl->mId, asString(iType).c_str(),
		mImpl->mNumDimensions, mImpl->mDimensions, Checker());
	// let update do its work
	mImpl->update(Private::INIT_TYPE);
}

void Node::setDataType(std::string const & iType) {
	setDataType(stringToType(iType));
}
		
int Node::getNumDimensions() const {
	mImpl->checkInitialized(Private::INIT_DIM);
	return mImpl->mNumDimensions;
}

int Node::getDimensions(int * oDimensions) const {
	mImpl->checkInitialized(Private::INIT_DIM);
	if (oDimensions==0) Emit(Exception::NULL_POINTER);
	for(int i=0; i<mImpl->mNumDimensions; ++i) // <--- only for valid dimensions
		oDimensions[i]=mImpl->mDimensions[i];
	return mImpl->mNumDimensions;
}

int Node::getDimension(int iNth) const {
	checkIndex(iNth, getNumDimensions());
	return mImpl->mDimensions[iNth];
}

void Node::setNumDimensions(int iNumDim) {
	for (int i=mImpl->mNumDimensions; i<iNumDim; ++i)
		mImpl->mDimensions[i]=0;
	setDataTypeDimensions(getDataType(), iNumDim, mImpl->mDimensions);
}
void Node::setDimension(int iNth, int iSize) {
	checkIndex(iNth, mImpl->mNumDimensions);
	if (iSize<0) Emit(Exception::NUMBER_LESS_THAN_MINIMUM);
	int dimensions[MAX_DIMENSIONS];
	for (int i=0; i<mImpl->mNumDimensions; ++i)
		dimensions[i]=mImpl->mDimensions[i];
	for (int i=mImpl->mNumDimensions; i<MAX_DIMENSIONS; ++i)
		dimensions[i]=0;
	dimensions[iNth]=iSize;
	setDataTypeDimensions(getDataType(), mImpl->mNumDimensions, dimensions);
}
	
void Node::setDimensions(int iNumDimensions, int const * iDimensions) {
	setDataTypeDimensions(getDataType(), iNumDimensions, iDimensions);
}

void Node::setDimensions(int iSize) {
	setDataTypeDimensions(getDataType(), 1, &iSize);
}

void Node::setDimensions(int iSize1, int iSize2) {
	int size[2] = { iSize1, iSize2 };
	setDataTypeDimensions(getDataType(), 2, size);
}

void Node::setDimensions(int iSize1, int iSize2, int iSize3) {
	int size[3] = { iSize1, iSize2, iSize3 };
	setDataTypeDimensions(getDataType(), 3, size);
}

void Node::setDataTypeDimensions(ADF::DataType iType, int iNumDimensions, int const * iDimensions) {
	checkIndex(iNumDimensions, MAX_DIMENSIONS);
	checkPointer(iDimensions);
	int dimensions[MAX_DIMENSIONS];
	for (int i=0; i<iNumDimensions; ++i) {
		if (iDimensions<0) Emit(Exception::NUMBER_LESS_THAN_MINIMUM);
		dimensions[i]=iDimensions[i];
	}
	for (int i=iNumDimensions; i<MAX_DIMENSIONS; ++i)
		dimensions[i]=0;
	ADF_Put_Dimension_Information(mImpl->mId, asString(iType).c_str(),
		iNumDimensions, dimensions, Checker());
	// let update do its work
	mImpl->update(Private::Or(Private::INIT_DIM,Private::INIT_TYPE));
}

void Node::setDataTypeDimensions(std::string const & iType, int iNumDimensions, int const * iDimensions) {
	setDataTypeDimensions(stringToType(iType), iNumDimensions, iDimensions);
}

void Node::setDataTypeDimensions(std::string const & iType, int iSize) {
	setDataTypeDimensions(iType, 1, &iSize);
}

void Node::setDataTypeDimensions(ADF::DataType iType, int iSize) {
	setDataTypeDimensions(iType, 1, &iSize);
}

void Node::setDataTypeDimensions(ADF::DataType iType, int iSize1, int iSize2) {
	int size[2] = { iSize1, iSize2 };
	setDataTypeDimensions(iType, 2, size);
}

void Node::setDataTypeDimensions(ADF::DataType iType, int iSize1, int iSize2, int iSize3) {
	int size[3] = { iSize1, iSize2, iSize3 };
	setDataTypeDimensions(iType, 3, size);
}

void Node::readData(void * oData) const {
	checkPointer(oData);
	ADF_Read_All_Data(mImpl->mId, static_cast<char *>(oData), Checker());	
}

void Node::readData(ADF::Range iRange, void * oData) const {
	checkPointer(oData);
//std::cerr << "readData(" << iRange.mMin << ", " << iRange.mMax << "(\n";
	if (iRange.mMin<iRange.mMax)
		ADF_Read_Block_Data(mImpl->mId, iRange.mMin+1, iRange.mMax,
			static_cast<char *>(oData), Checker());
}

template<typename T_iter, typename T_to>
void doCopyConversion(T_iter iFrom, T_iter iTo, T_to * oData) {
	std::copy(iFrom, iTo, oData);
}

// complex<T> doesn't have conversions to and from complex<U>
// therefore convert real and imaginary part separately
template<typename T_iter, typename T_to>
void doCopyConversion(T_iter iFrom, T_iter iTo, typename std::complex<T_to> * oData) {
	for (; iFrom!=iTo; ++iFrom)
		*oData=std::complex<T_to>((*iFrom).real(), (*iFrom).imag());
}

// read in and copy convert
template<typename T_from, typename T_to>
void doRead(Node const & iNode, T_from const &, T_to * oData) {
	if (iNode.getNumDimensions()<=0) return;
	int count=1;
	for (int i=0; i<iNode.getNumDimensions(); ++i)
		count*=iNode.getDimension(i);
	std::vector<T_from> data(count);
	iNode.readData(static_cast<void *>(&data[0]));
	doCopyConversion(data.begin(), data.end(), oData);
}

template<typename T_from, typename T_to>
void doRead(Node const & iNode, ADF::Range iRange, T_from const &, T_to * oData) {
	if (iNode.getNumDimensions()<=0) return;
	int count=1;
	for (int i=0; i<iNode.getNumDimensions(); ++i)
		count*=iNode.getDimension(i);
	std::vector<T_from> data(count);
	iNode.readData(iRange, static_cast<void *>(&data[0]));
	doCopyConversion(data.begin(), data.end(), oData);
}

// but read directly if possible (same data type)
template<typename T>
void doRead(Node const & iNode, T const &, T * oData) {
	iNode.readData(static_cast<void *>(oData));
}

template<typename T>
void doRead(Node const & iNode, ADF::Range iRange, T const &, T * oData) {
	iNode.readData(iRange, static_cast<void *>(oData));
}

// read any integer type in
template<typename T>
void doIntegerRead(Node const & iNode, T * oData) {
	switch (iNode.getDataType()) {
		case ADF::I4: doRead(iNode, ADF::I4_t(), oData); break;
		case ADF::U4: doRead(iNode, ADF::U4_t(), oData); break;
		case ADF::I8: doRead(iNode, ADF::I8_t(), oData); break;
		case ADF::U8: doRead(iNode, ADF::U8_t(), oData); break;
		default: ADF::Emit(ADF::Exception::DIFFERENT_DATA_TYPE);
	}
}

template<typename T>
void doIntegerRead(Node const & iNode, ADF::Range iRange, T * oData) {
	switch (iNode.getDataType()) {
		case ADF::I4: doRead(iNode, iRange, ADF::I4_t(), oData); break;
		case ADF::U4: doRead(iNode, iRange, ADF::U4_t(), oData); break;
		case ADF::I8: doRead(iNode, iRange, ADF::I8_t(), oData); break;
		case ADF::U8: doRead(iNode, iRange, ADF::U8_t(), oData); break;
		default: ADF::Emit(ADF::Exception::DIFFERENT_DATA_TYPE);
	}
}

// read any real type in
template<typename T>
void doRealRead(Node const & iNode, T * oData) {
	switch (iNode.getDataType()) {
		case ADF::R4: doRead(iNode, ADF::R4_t(), oData); break;
		case ADF::R8: doRead(iNode, ADF::R8_t(), oData); break;
		default: ADF::Emit(ADF::Exception::DIFFERENT_DATA_TYPE);
	}
}

template<typename T>
void doRealRead(Node const & iNode, ADF::Range iRange, T * oData) {
	switch (iNode.getDataType()) {
		case ADF::R4: doRead(iNode, iRange, ADF::R4_t(), oData); break;
		case ADF::R8: doRead(iNode, iRange, ADF::R8_t(), oData); break;
		default: ADF::Emit(ADF::Exception::DIFFERENT_DATA_TYPE);
	}
}

// read any complex type in
template<typename T>
void doComplexRead(Node const & iNode, typename std::complex<T> * oData) {
	switch (iNode.getDataType()) {
		case ADF::X4: doRead(iNode, ADF::X4_t(), oData); break;
		case ADF::X8: doRead(iNode, ADF::X8_t(), oData); break;
		default: ADF::Emit(ADF::Exception::DIFFERENT_DATA_TYPE);
	}
}

template<typename T>
void doComplexRead(Node const & iNode, ADF::Range iRange, typename std::complex<T> * oData) {
	switch (iNode.getDataType()) {
		case ADF::X4: doRead(iNode, iRange, ADF::X4_t(), oData); break;
		case ADF::X8: doRead(iNode, iRange, ADF::X8_t(), oData); break;
		default: ADF::Emit(ADF::Exception::DIFFERENT_DATA_TYPE);
	}
}

// read any character or byte type in
template<typename T>
void doCharacterRead(Node const & iNode, T * oData) {
	switch (iNode.getDataType()) {
		case ADF::C1: doRead(iNode, ADF::C1_t(), oData); break;
		case ADF::B1: doRead(iNode, ADF::B1_t(), oData); break;
		default: ADF::Emit(ADF::Exception::DIFFERENT_DATA_TYPE);
	}
}

template<typename T>
void doCharacterRead(Node const & iNode, ADF::Range iRange, T * oData) {
	switch (iNode.getDataType()) {
		case ADF::C1: doRead(iNode, iRange, ADF::C1_t(), oData); break;
		case ADF::B1: doRead(iNode, iRange, ADF::B1_t(), oData); break;
		default: ADF::Emit(ADF::Exception::DIFFERENT_DATA_TYPE);
	}
}


//-----------------------------------------------------------------------------
//
//  readData
//

// handle all cases of requested data
void Node::readData(int * oData) const {
	doIntegerRead(*this, oData);
}

#if _MSC_VER > 1300
void Node::readData(__int64 * oData) const {
	doIntegerRead(*this, oData);
}
#else
void Node::readData(long long * oData) const {
	doIntegerRead(*this, oData);
}
#endif

void Node::readData(unsigned * oData) const {
	doIntegerRead(*this, oData);
}

#if _MSC_VER > 1300
void Node::readData(unsigned __int64 * oData) const {
	doIntegerRead(*this, oData);
}
#else
void Node::readData(unsigned long long * oData) const {
	doIntegerRead(*this, oData);
}
#endif

void Node::readData(float * oData) const {
	doRealRead(*this, oData);
}

void Node::readData(double * oData) const {
	doRealRead(*this, oData);
}

void Node::readData(std::complex<float> * oData) const {
	doComplexRead(*this, oData);
}

void Node::readData(std::complex<double> * oData) const {
	doComplexRead(*this, oData);
}

void Node::readData(char * oData) const {
	doCharacterRead(*this, oData);
}

void Node::readData(unsigned char * oData) const {
	doCharacterRead(*this, oData);
}

void Node::readData(std::string * oData) const {
	if (getNumDimensions()!=1) Emit(Exception::BAD_NUMBER_OF_DIMENSIONS);
	int len=getDimension(0);
	std::vector<char> d(len+1);
	readData(&d[0]);
	d[len]=0;
	*oData=&d[0];
}


//-----------------------------------------------------------------------------
//
//  readData
//

/** partial reading **/
void Node::readData(ADF::Range iRange, int * oData) const {
	doIntegerRead(*this, iRange, oData);
}

#if _MSC_VER > 1300
void Node::readData(ADF::Range iRange, __int64 * oData) const {
	doIntegerRead(*this, iRange, oData);
}
#else
void Node::readData(ADF::Range iRange, long long * oData) const {
	doIntegerRead(*this, iRange, oData);
}
#endif

void Node::readData(ADF::Range iRange, unsigned * oData) const {
	doIntegerRead(*this, iRange, oData);
}

#if _MSC_VER > 1300
void Node::readData(ADF::Range iRange, unsigned __int64 * oData) const {
	doIntegerRead(*this, iRange, oData);
}
#else
void Node::readData(ADF::Range iRange, unsigned long long * oData) const {
	doIntegerRead(*this, iRange, oData);
}
#endif

void Node::readData(ADF::Range iRange, float * oData) const {
	doRealRead(*this, iRange, oData);
}

void Node::readData(ADF::Range iRange, double * oData) const {
	doRealRead(*this, iRange, oData);
}

void Node::readData(ADF::Range iRange, std::complex<float> * oData) const {
	doComplexRead(*this, iRange, oData);
}

void Node::readData(ADF::Range iRange, std::complex<double> * oData) const {
	doComplexRead(*this, iRange, oData);
}

void Node::readData(ADF::Range iRange, char * oData) const {
	doCharacterRead(*this, iRange, oData);
}

void Node::readData(ADF::Range iRange, unsigned char * oData) const {
	doCharacterRead(*this, iRange, oData);
}


//-----------------------------------------------------------------------------
//
//  writeData
//

void Node::writeData(void const * iData) {
	checkPointer(iData);
	ADF_Write_All_Data(mImpl->mId, static_cast<char const *>(iData), Checker());	
}

void Node::writeData(int const * iData) {
	if (getDataType()!=I4) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(static_cast<void const *>(iData));
}

#if _MSC_VER > 1300
void Node::writeData(__int64 const * iData) {
	if (getDataType()!=I8) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(static_cast<void const * >(iData));
}
#else
void Node::writeData(long long const * iData) {
	if (getDataType()!=I8) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(static_cast<void const * >(iData));
}
#endif

void Node::writeData(unsigned const * iData) {
	if (getDataType()!=U4) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(static_cast<void const * >(iData));
}

#if _MSC_VER > 1300
void Node::writeData(unsigned __int64 const * iData) {
	if (getDataType()!=U8) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(static_cast<void const * >(iData));
}
#else
void Node::writeData(unsigned long long const * iData) {
	if (getDataType()!=U8) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(static_cast<void const * >(iData));
}
#endif

void Node::writeData(float const * iData) {
	if (getDataType()!=R4) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(static_cast<void const * >(iData));
}

void Node::writeData(double const * iData) {
	if (getDataType()!=R8) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(static_cast<void const * >(iData));
}

void Node::writeData(std::complex<float> const * iData) {
	if (getDataType()!=X4) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(static_cast<void const * >(iData));
}

void Node::writeData(std::complex<double> const * iData) {
	if (getDataType()!=X8) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(static_cast<void const * >(iData));
}

void Node::writeData(char const * iData) {
	if (getDataType()!=C1) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(static_cast<void const * >(iData));
}

void Node::writeData(unsigned char const * iData) {
	if (getDataType()!=B1) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(static_cast<void const * >(iData));
}

void Node::writeData(std::string const & iData) {
	setDataTypeDimensions(C1, iData.size());	
	writeData(static_cast<void const * >(iData.data()));
}


//-----------------------------------------------------------------------------
//
//  writeData
//

void Node::writeData(ADF::Range iRange, void const * iData) {
	checkPointer(iData);
//std::cerr << "writeData (" << iRange.mMin << ", " << iRange.mMax << ") in " << asString(mImpl->mDataType)
//<< "(" << mImpl->mNumDimensions << "):" << mImpl->mDimensions[0] << "/" << mImpl->mDimensions[1] << "/"
//<< mImpl->mDimensions[2] << "\n";	
	if (iRange.mMin<iRange.mMax)
		ADF_Write_Block_Data(mImpl->mId, iRange.mMin+1, iRange.mMax,
			static_cast<char const *>(iData), Checker());
}

void Node::writeData(ADF::Range iRange, int const * iData) {
	if (getDataType()!=I4) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(iRange, static_cast<void const *>(iData));
}

void Node::writeData(ADF::Range iRange, long long const * iData) {
	if (getDataType()!=I8) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(iRange, static_cast<void const * >(iData));
}

void Node::writeData(ADF::Range iRange, unsigned const * iData) {
	if (getDataType()!=U4) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(iRange, static_cast<void const * >(iData));
}

#if _MSC_VER > 1300
void Node::writeData(ADF::Range iRange, unsigned __int64 const * iData) {
	if (getDataType()!=U8) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(iRange, static_cast<void const * >(iData));
}
#else
void Node::writeData(ADF::Range iRange, unsigned long long const * iData) {
	if (getDataType()!=U8) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(iRange, static_cast<void const * >(iData));
}
#endif

void Node::writeData(ADF::Range iRange, float const * iData) {
	if (getDataType()!=R4) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(iRange, static_cast<void const * >(iData));
}

void Node::writeData(ADF::Range iRange, double const * iData) {
	if (getDataType()!=R8) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(iRange, static_cast<void const * >(iData));
}

void Node::writeData(ADF::Range iRange, std::complex<float> const * iData) {
	if (getDataType()!=X4) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(iRange, static_cast<void const * >(iData));
}

void Node::writeData(ADF::Range iRange, std::complex<double> const * iData) {
	if (getDataType()!=X8) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(iRange, static_cast<void const * >(iData));
}

void Node::writeData(ADF::Range iRange, char const * iData) {
	if (getDataType()!=C1) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(iRange, static_cast<void const * >(iData));
}

void Node::writeData(ADF::Range iRange, unsigned char const * iData) {
	if (getDataType()!=B1) Emit(Exception::DIFFERENT_DATA_TYPE);
	writeData(iRange, static_cast<void const * >(iData));
}


//-----------------------------------------------------------------------------
//
//  writeData
//


std::ostream & ADF::operator<<(std::ostream & outStream, const Node& node) {
	outStream << "  name        = " << node.getName() << std::endl
	    << "  nChildren   = " << node.getNumChildren() << std::endl
	    << "  isLink      = " << (node.isLink() ? "yes" : "no") << std::endl;
	if (node.isLink()) {
    	outStream << "  link-file   = '" << node.getLinkFile() << "'" << std::endl
		      << "  link-path   = '" << node.getLinkPath() << "'" << std::endl;
	}
	outStream << "  label       = " << node.getLabel() << std::endl
	    << "  dataType    = " << asString(node.getDataType()) << std::endl
	    << "  nDimensions = " << node.getNumDimensions() << std::endl
	    << "  dims[]      = (";
	int nDims;
	int dims[MAX_DIMENSIONS];
	nDims=node.getDimensions(dims);
	for (int i=0; i<nDims-1; i++) outStream << dims[i] << ",";
	outStream << dims[nDims-1] << ")" << std::endl;
  
	return outStream;
}


/** The remainder is only for debugging purposes */
struct NodeCountChecker {
	~NodeCountChecker();
};

static NodeCountChecker nodeCountChecker;

NodeCountChecker::~NodeCountChecker() {
	if (implCount!=0) std::cerr << "ADF::Node: final implCount=" << implCount << std::endl;
	if (nodeCount!=0) std::cerr << "ADF::Node: final nodeCount=" << nodeCount << std::endl;
}

