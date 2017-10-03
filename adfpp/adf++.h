/***************************************************************************
                          adf++.h  -  description
                             -------------------
    begin                : Wed Mar 14 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/


#ifndef ADFPP_H
#define ADFPP_H

#include <complex>
#include <exception>
#include <string>
#include <vector>

namespace ADF {

template<typename T>
class refcnt_ptr {
public:	
	refcnt_ptr() : mPtr(0) {}
	template<typename U>
	refcnt_ptr(U iPtr) : mPtr(iPtr) { inc(); }
	refcnt_ptr(refcnt_ptr const & rhs) : mPtr(rhs.mPtr) { inc(); }
	template<typename U>
	refcnt_ptr(refcnt_ptr<U> const & rhs) : mPtr(rhs.mPtr) { inc(); }
	~refcnt_ptr();
	operator T*() { return mPtr; }
	operator T const *() const { return mPtr; }
	bool operator !() const { return !mPtr; }
	bool valid() const { return mPtr; }
	T * get() { return mPtr; }
	void release() { dec(); }
	T const * get() const { return mPtr; }
	T* operator ->() { return mPtr; }
	T const * operator ->() const { return mPtr; }
	template<typename U>
	void reset(U rhs) { dec(); mPtr=rhs; inc(); }
	template<typename U>
	refcnt_ptr const & operator=(U rhs) {
		dec(); mPtr=rhs; inc(); return *this; }
	template<typename U>
	refcnt_ptr const & operator=(refcnt_ptr<U> const & rhs) {
		dec(); mPtr=rhs.mPtr; inc(); return *this; }
	refcnt_ptr const & operator=(refcnt_ptr const & rhs) {
		dec(); mPtr=rhs.mPtr; inc(); return *this; }
private:
	void inc() { if (mPtr) ++mPtr->mReferenceCount; }
	void dec() { if (mPtr) {
		if (--mPtr->mReferenceCount==0) { delete mPtr; mPtr=0; } mPtr=0; } }
	T * mPtr;
};

/** The (opaque) iterator class template for all multiple child accesses.
  */
template<typename T>
struct iterator /*: public std::iterator<std::forward_iterator_tag, T> */{
	typedef std::forward_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef T * pointer;
	typedef T & reference;
	explicit iterator(void * iData=0);
	std::string const & getName() const;
	reference operator*() const;
	pointer operator->() const;
	iterator const & operator++(); //prefix
	iterator operator++(int);      //postfix
	bool operator!=(iterator const & rhs) const;
	bool operator==(iterator const & rhs) const;
private:	
	void * mData;
};

// forward declarations
class Exception;
class File;
class Node;

/** Constants taken from the CGNS distribution, file adf/ADF.h */
enum {
	DATA_TYPE_LENGTH     = 32,
	DATE_LENGTH          = 32,
	FILENAME_LENGTH      = 1024,
	FORMAT_LENGTH        = 20,
	LABEL_LENGTH         = 32,
	MAXIMUM_LINK_DEPTH   = 100,
	MAX_DIMENSIONS       = 12,
	MAX_ERROR_STR_LENGTH = 80,
	MAX_LINK_DATA_SIZE   = 4096,
	NAME_LENGTH          = 32,
	STATUS_LENGTH        = 32,
	VERSION_LENGTH       = 32
};

//typedef char NameT[NAME_LENGTH+1];

/** Possible data types of the node */
enum DataType {
	MT, ///< No data
	I4, ///< Signed integer, 4 bytes
	I8, ///< Signed integer, 8 bytes
	U4, ///< Unsigned integer, 4 bytes
	U8, ///< Unsigned integer, 8 bytes
	R4, ///< Real, 4 bytes
	R8, ///< Real, 8 bytes
	X4, ///< Complex, 2*4 bytes
	X8, ///< Complex, 2*8 bytes
	C1, ///< Character
	B1, ///< Generic byte
	LK, ///< Link node
	STRUCT ///< Arbitrary structured data (currently unsupported)
};
std::string asString(DataType iType);

// the appropriate typedefs
// I8 and U8 are properly supported only on 64 bit systems, where (unsigned) long is 8 bytes
typedef int I4_t;					///< Signed integer, 4 bytes
typedef long I8_t;					///< Signed integer, 8 bytes
typedef unsigned U4_t;				///< Unsigned integer, 4 bytes
typedef unsigned long U8_t;			///< Unsigned integer, 8 bytes
typedef float R4_t;					///< Real, 4 bytes
typedef double R8_t;				///< Real, 8 bytes
typedef std::complex<float> X4_t;	///< Complex, 2*4 bytes
typedef std::complex<double> X8_t;	///< Complex, 2*8 bytes
typedef char C1_t;					///< Character
typedef unsigned char B1_t;			///< Generic byte

typedef int Index_t;
/** STL style range, meaning [min,max[, that is including min, but excluding max.
  * counting starts from zero, i.e. C style, not FORTRAN style. */
struct Range {
	Index_t mMin, mMax;
	Range() : mMin(0), mMax(0) {}
	Range(Index_t iMin, Index_t iMax) : mMin(iMin), mMax(iMax) {}
};


/** This exception is thrown when an error occurs calling an ADF function.
  */
class Exception : public std::exception {
public:
	enum { NO_ERROR=-1, IMPOSSIBLE=0,
		NUMBER_LESS_THAN_MINIMUM=1, NUMBER_GREATER_THAN_MAXIMUM,
		STRING_LENGTH_ZERO, STRING_LENGTH_TOO_BIG, STRING_NOT_A_HEX_STRING,
		TOO_MANY_ADF_FILES_OPENED, ADF_FILE_STATUS_NOT_RECOGNIZED,
		FILE_OPEN_ERROR, ADF_FILE_NOT_OPENED, FILE_INDEX_OUT_OF_RANGE=10,
		BLOCK_OFFSET_OUT_OF_RANGE, NULL_STRING_POINTER,
		FSEEK_ERROR, FWRITE_ERROR, FREAD_ERROR,
		ADF_MEMORY_TAG_ERROR, ADF_DISK_TAG_ERROR,
		REQUESTED_NEW_FILE_EXISTS, ADF_FILE_FORMAT_NOT_RECOGNIZED,
		FREE_OF_ROOT_NODE=20, FREE_OF_FREE_CHUNK_TABLE, REQUESTED_OLD_FILE_NOT_FOUND,
		UNIMPLEMENTED_CODE, SUB_NODE_TABLE_ENTRIES_BAD, MEMORY_ALLOCATION_FAILED,
		DUPLICATE_CHILD_NAME, ZERO_DIMENSIONS, BAD_NUMBER_OF_DIMENSIONS,
		CHILD_NOT_OF_GIVEN_PARENT, DATA_TYPE_TOO_LONG=30, INVALID_DATA_TYPE,
		NULL_POINTER, NO_DATA, ERROR_ZEROING_OUT_MEMORY, REQUESTED_DATA_TOO_LONG,
		END_OUT_OF_DEFINED_RANGE, BAD_STRIDE_VALUE, MINIMUM_GT_MAXIMUM,
		MACHINE_FORMAT_NOT_RECOGNIZED, CANNOT_CONVERT_NATIVE_FORMAT=40,
		CONVERSION_FORMATS_EQUAL, DATA_TYPE_NOT_SUPPORTED, FILE_CLOSE_ERROR,
		NUMERIC_OVERFLOW, START_OUT_OF_DEFINED_RANGE, ZERO_LENGTH_VALUE,
		BAD_DIMENSION_VALUE, BAD_ERROR_STATE, UNEQUAL_MEMORY_AND_DISK_DIMS,
		LINKS_TOO_DEEP=50, NODE_IS_NOT_A_LINK, LINK_TARGET_NOT_THERE,
		LINKED_TO_FILE_NOT_THERE, NODE_ID_ZERO, INCOMPLETE_DATA, INVALID_NODE_NAME,
		INVALID_VERSION, NODES_NOT_IN_SAME_FILE, PRISTK_NOT_FOUND,
		MACHINE_FILE_INCOMPATABLE=60, FFLUSH_ERROR=61,
		ADFPP_FIRST_ERROR=100, DIFFERENT_DATA_TYPE, DELETED_NODE, INTERNAL_ERROR=127
		};
	
	explicit Exception(int iErrorCode);
	virtual ~Exception() throw();
	
	int getErrorCode() const throw();
	virtual char const * getErrorMessage() const throw();
	virtual char const * what() const throw();
	
protected:
	int mErrorCode;
}; // ADF::Exception


/** Represents the bare node in an ADF file
  */
class Node {
public:
	typedef iterator<Node> Child_iterator_t;
	/** Copy constructor */
	Node(Node const & rhs);
	/** Deletes this node handle. */
	~Node();
	
	/** Create a child node with the given name (and possibly label). */
	Node createChild(std::string const & iChildName, std::string const & iChildLabel="");
	/** Delete the given child node. */
	void deleteChild(std::string const & );
	void deleteChild(Node & iChildNode);
	/** Delete all child nodes. */
	void deleteChildren();
	
	/** Get the number of children of this node. */
	int getNumChildren() const;
	/** Get the number of children of this node which have the given label. */
	int getNumLabeledChildren(std::string const & iLabel) const;
	/** Get the named child node. */
	Node getChild(std::string const & iChildName) const;
	/** Gets an iterator to the first child. */
	Child_iterator_t beginChild() const;
	/** Gets an iterator one past the last child. */
	Child_iterator_t endChild() const;
    /** Ask if there is a named child. */
    bool hasChild(std::string const & iChildName) const;
    /** Ask if this is a child node. */
    bool hasChild(Node const & iChildNode) const;
    /** Ask if the given id is a child node. */
    bool hasChild(double iChildId) const;

	/** Get the first child node labeled as indicated. */
	Node getLabeledChild(std::string const & iChildLabel) const;

	/** Get the intrinsic name of this node. */
	std::string getName() const;
	/** Rename child node. */
	void renameChild(std::string const & iOldName, std::string const & iName);
	/** Rename child node. */
	void renameChild(Node & iChildNode, std::string const & iName);
	
	/** Move child to another parent node */
	void reparentChild(std::string const & iChildName, Node & iNewParent);
	void reparentChild(Node const & iChildNode, Node & iNewParent);
	
	/** Create a child which is a link in this file */
	Node createLink(std::string const & iChildName, std::string const & iLinkedToName);
	/** Create a child which is a link to another node */
	Node createLink(std::string const & iChildName, Node const & iLinkedToNode);
	/** Create a child which is a link in another file */
	Node createLink(std::string const & iChildName,
		std::string const & iLinkedToName, std::string const & iLinkedToFile);

	/** Check whether this node is in fact a link .*/
	bool isLink() const;
	
	/** Get the file name of a link. */
	std::string getLinkFile() const;
	/** Get the path to the linked to node.	*/
	std::string getLinkPath() const;
	
	/** Get the root node of the file this node is in. */
	Node getRootNode() const;

	/** Get the label of this node. */
	std::string getLabel() const;
	/** Relabel this node. */
	void setLabel(std::string const & iLabel);
	
	/** Get the data type of this node.*/
	DataType getDataType() const;
//	/** Get the string representation of the data type of this node. */
//	std::string getDataTypeString() const;
	/** Get the total amount of data in this node.*/
	int getDataSize() const;
    /** Set the data type of this node. Data is cleared. */
	void setDataType(DataType iType);
    /** Set the data type of this node with string representation. Data is cleared. */
	void setDataType(std::string const & iType);
		
	/** Get the number of dimensions. */
	int getNumDimensions() const;
	/** Get the size of all dimensions. Returns the numer of dimensions. */
	int getDimensions(int *oDimensions) const;
	/** Get the size of the nth dimension. */
	int getDimension(int iNth) const;
	/** Set the number of dimensions. All dimensions values are set to 1, data is cleared. */
	void setNumDimensions(int iNumDim);
	/** Set the size of the nth dimension. Data is cleared. */
	void setDimension(int iNth, int iSize);
	/** Set all dimensions at once. */
	void setDimensions(int iNumDimensions, int const * iDimensions);
	
	/** Set to 1D and the size of the only dimension. Data is cleared. */
	void setDimensions(int iSize);
	/** Set to 2D and the size of two dimension. Data is cleared. */
	void setDimensions(int iSize1, int iSize2);
	/** Set to 3D and the size of three dimensions. Data is cleared. */
	void setDimensions(int iSize1, int iSize2, int iSize3);
	
	/** Set data type and all dimensions at once. */
	void setDataTypeDimensions(DataType iType, int iNumDimensions, int const * iDimensions);
	/** Set data type (as string) and all dimensions at once. */
	void setDataTypeDimensions(std::string const & iType, int iNumDimensions, int const * iDimensions);
	
	/** Set data type and the single one dimension. */
	void setDataTypeDimensions(std::string const & iType, int iSize);
	/** Set data type, 1D and the single one dimension. */
	void setDataTypeDimensions(DataType iType, int iSize);
	/** Set data type, 2D and the two dimensions. */
	void setDataTypeDimensions(DataType iType, int iSize1, int iSize2);
	/** Set data type, 3D and the three dimensions. */
	void setDataTypeDimensions(DataType iType, int iSize1, int iSize2, int iSize3);
	
	
	/** Read the raw data from disk. */
	void readData(void * oData) const;
	/** Read the data from disk, possibly with conversion. */
	void readData(int * oData) const;
#if _MSC_VER > 1300
	void readData(__int64 * oData) const;
#else
	void readData(long long * oData) const;
#endif
	void readData(unsigned * oData) const;
#if _MSC_VER > 1300
	void readData(unsigned __int64 * oData) const;
#else
	void readData(unsigned long long * oData) const;
#endif
	void readData(float * oData) const;
	void readData(double * oData) const;
	void readData(std::complex<float> * oData) const;
	void readData(std::complex<double> * oData) const;
	void readData(char * oData) const;
	void readData(unsigned char * oData) const;
	
	/** Convenience function for reading a string */
	void readData(std::string * oData) const;
	
    /** Read the raw data partially. */
    void readData(Range iRange, void * oData) const;
    /** Read the data partially from disk, possibly with conversion. */
    void readData(Range iRange, int * oData) const;
#if _MSC_VER > 1300
    void readData(Range iRange, __int64 * oData) const;
#else
	void readData(Range iRange, long long * oData) const;
#endif
    void readData(Range iRange, unsigned * oData) const;
#if _MSC_VER > 1300    
	void readData(Range iRange, unsigned __int64 * oData) const;
#else
	void readData(Range iRange, unsigned long long * oData) const;
#endif
    void readData(Range iRange, float * oData) const;
    void readData(Range iRange, double * oData) const;
    void readData(Range iRange, std::complex<float> * oData) const;
    void readData(Range iRange, std::complex<double> * oData) const;
    void readData(Range iRange, char * oData) const;
    void readData(Range iRange, unsigned char * oData) const;
	
	/** Write the raw data to disk. */
	void writeData(void const * iData);
	/** Write the data to disk, possibly with conversion. */
	void writeData(int const * iData);
#if _MSC_VER > 1300
	void writeData(__int64 const * iData);
#else
	void writeData(long long const * iData);
#endif
	void writeData(unsigned const * iData);
#if _MSC_VER > 1300
	void writeData(unsigned __int64 const * iData);
#else
	void writeData(unsigned long long const * iData);
#endif
	void writeData(float const * iData);
	void writeData(double const * iData);
	void writeData(std::complex<float> const * iData);
	void writeData(std::complex<double> const * iData);
	void writeData(char const * iData);
	void writeData(unsigned char const * iData);
	
	/** Convenience function for reading a string */
	void writeData(std::string const & iData);
	
    /** Write the raw data partially. iData is the start of the data to be written. */
    void writeData(Range iRange, void const * iData);
    /** Write the data partially to disk, possibly with conversion.
      * iData is the start of the data to be written. */
    void writeData(Range iRange, int const * iData);
#if _MSC_VER > 1300
	void writeData(Range iRange, __int64 const * iData);
#else
	void writeData(Range iRange, long long const * iData);
#endif
	void writeData(Range iRange, unsigned const * iData);
#if _MSC_VER > 1300
    void writeData(Range iRange, unsigned __int64 const * iData);
#else
	void writeData(Range iRange, unsigned long long const * iData);
#endif
    void writeData(Range iRange, float const * iData);
    void writeData(Range iRange, double const * iData);
    void writeData(Range iRange, std::complex<float> const * iData);
    void writeData(Range iRange, std::complex<double> const * iData);
    void writeData(Range iRange, char const * iData);
    void writeData(Range iRange, unsigned char const * iData);

	/** Reread the node info again from the CGNS file. */
	void update();
	/** Assure the data is written to disk. */
	void flush();
	
	/** Get the basic ADF id. */
	double getId() const;
	
	/** Creates a new child node in ADF only. */
	static double createBareChild(double iParent,
		std::string const & iName, std::string const & iLabel="");
		
	
	/** Creates a solitary node for lookup.
	  * This is discouraged and for convenience only.
	  * Dismiss as soon as possible. */
	explicit Node(double iId);
	
	class Private;
	/** Creates a node from implementation data. */
	Node(Private * iImpl);
protected:
	refcnt_ptr<Private> mImpl;
	
	/** Creates a new child node in ADF only. */
	double createBareChild(std::string const & iName, std::string const & iLabel="");
	
private:
friend class File;
}; // ADF::Node

/** Print info about node */
//ostream& operator<<(ostream& outStream, const Node& node);

/** Represents a whole ADF file with a root node.
  */
class File {
public:
	//@{
	/** Get the version of the library which is running.
	  * Format of the string is something like E01001. */
	static std::string getLibraryVersionString();
	static char getLibraryVersionMajor();
	static int getLibraryVersionMinor();
	//@}
		
	/** The possible open policies. */
	enum Status {
		UNKNOWN,	///< OLD if file exists otherwise NEW is used
		READ_ONLY,	///< File must exist, writing is not allowed.
		OLD,		///< File must exist, reading and writing is allowed.
		NEW,		///< File must not exist.
		SCRATCH		///< Temporary new file is created with a system name and deleted on exit or close.
	};
    static std::string asString(Status iStatus);

	/** The possible formats for numeric (i.e. real) data. */
	enum Format {
		UNDEFINED,		///< Format undefined
		NATIVE,			///< The native floating point format of the machine that creates the file.
		IEEE_BIG,		///< IEEE big endian floating point format.
		IEEE_LITTLE,	///< IEEE little endian floating point format.
//		IEEE_BIG_32,	///< IEEE big endian floating point format (32 bit OS).
//		IEEE_LITTLE_32,	///< IEEE little endian floating point format (32 bit OS).
//		IEEE_BIG_64,	///< IEEE big endian floating point format (64 bit OS).
//		IEEE_LITTLE_64,	///< IEEE little endian floating point format (64 bit OS).
		CRAY			///< The native CRAY format.
	};
    static std::string asString(Format iFormat);

	/** Open an ADF database with specified status and format.
	  * Format is ignored for iStatus==OLD and READ_ONLY. */
	File(std::string const & iFileName, Status iStatus=READ_ONLY, Format iFormat=NATIVE);
    /** Open an ADF database with status == SCRATCH and format. */
    File(Format iFormat=NATIVE);
	/** Deleted this ADF database handle and all attached node handles. */
	~File();

    /** Get the status this file has been opened with. */
    Status getStatus() const;

	/** Get the format this file is written in. */
	Format getFormat() const;

	/** Get the root node of this file. */
	Node getRoot() const;
	
	//@{
	/** Get the version of the library this file was written with.
	  * Format of the string is something like E01001. */
	std::string getVersionString() const;
	char getVersionMajor() const;
	int getVersionMinor() const;
	//@}
	
protected:
    Status mStatus;
	Node mRootNode;
	double mRootId;
	/// \todo Encode somehow creation date/time and modification date/time

private:	
	/** Forbidden */
	File(File const &);
	/** Forbidden */
	File const & operator=(File const &);
}; // ADF::File

/** Write information about the given node. Might be useful for debugging. */
std::ostream & operator<<(std::ostream & outStream, Node const & node);

}; // namespace ADF


#endif
