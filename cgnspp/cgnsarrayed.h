/***************************************************************************
                          cgnsarrayed.h  -  description
                             -------------------
    begin                : Wed Sep 19 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSARRAYED_H
#define CGNSARRAYED_H

#include "cgnsdimensioned.h"

#include "cgnsutil.h"

/** Implementation data for nodes (possibly) with DataArray_t subnodes
  */
struct CGNS::Private::Arrayed : CGNS::Private::Dimensioned  {
COMMON_NODE_INTERFACE(Arrayed, Dimensioned, Dimensioned);
	/** Asks if there is a given quantity. */
	bool hasArray(std::string const & iName) const;
	bool hasArray(CGNS::DataArray::Quantity iQuantity) const;
	/** Gets the given quantity. Throws if not available. */
	DataArray * getArray(std::string const & iName) const;
	CGNS::DataArray getArray(CGNS::DataArray::Quantity iQuantity) const;
	
	/** Creates or changes the given quantity. */
	DataArray * writeArray(std::string const & iName);
	CGNS::DataArray writeArray(CGNS::DataArray::Quantity iQuantity);
	
	/** Deletes the given quantity. */
	void deleteArray(std::string const & iName);
	void deleteArray(CGNS::DataArray::Quantity iQuantity);
	
//	typedef std::vector<CGNS::DataArray> arrays_t;
	typedef child_container<CGNS::DataArray> arrays_t;
	arrays_t mArrays;
DISTINCT_CHILDREN1(DataArray);
};

#endif
