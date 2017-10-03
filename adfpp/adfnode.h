 /***************************************************************************
                          adfnode.h  -  description
                             -------------------
    begin                : Tue Sep 18 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef ADFNODE_H
#define ADFNODE_H

#include "adf++.h"

#include "adfutil.h"

#include <map>

namespace ADF {

/** Implementation of ADF::Node, containing the real data
  */
struct Node::Private {
	Private(double iId);
	~Private();
	
	enum Initialized { INIT_NONE=0, INIT_NAME=1, INIT_LABEL=2,
		INIT_TYPE=4, INIT_DIM=8, INIT_CHILD=16, INIT_ALL=31, DELETED=~0 };
	/** Operator for handling Initialization requests */
	static Initialized Or(Initialized lhs, Initialized rhs);
	static Initialized const & OrEq(Initialized & lhs, Initialized rhs);
	static Initialized And(Initialized lhs, Initialized rhs);
	static Initialized const & AndEq(Initialized & lhs, Initialized rhs);
	static Initialized Inv(Initialized rhs);
	static Initialized const & InvEq(Initialized & rhs);
	
	/** Takes care the requested info is here. */
	void checkInitialized(Initialized iInitialized) const;
	/** Rereads the requested info. */
	void update(Initialized iInitialized);
	/** Marks the node and all child nodes as deleted.
	  * Returns true if it got deleted, false if already gone. */
	bool markDeleted();
    
	/** Compares the names */
	bool hasName(std::string const & iName);

	/** Compares the labels */
	bool hasLabel(std::string const & iLabel);
	
	/** Sets the name */
	void setName(std::string const & iName);

	/** Sets the label */
	void setLabel(std::string const & iLabel);

	int mReferenceCount;
	// The real data
	double mId;
	Initialized mInitialized;
	char mName[NAME_LENGTH+1];
	char mLabel[LABEL_LENGTH+1];
	DataType mDataType;
	int mNumDimensions;
	int mDimensions[MAX_DIMENSIONS];
	// Cache child table for faster retrieval
	typedef ADF::child_container<Node> children_t;
	children_t mChildren;
private:
	Private(Private const &);
	Private const & operator=(Private const &);	
};

} // namespace ADF

#endif
