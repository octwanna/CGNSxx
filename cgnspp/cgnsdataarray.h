/***************************************************************************
                          cgnsdataarray.h  -  description
                             -------------------
    begin                : Wed Sep 19 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSDATAARRAY_H
#define CGNSDATAARRAY_H

#include "cgnsdimensioned.h"

#include "cgnsarrayed.h"

/** Implementation data for nodes containing real data
  */
struct CGNS::Private::DataArray : CGNS::Private::Dimensioned  {
COMMON_NODE_INTERFACE(DataArray, Arrayed, Dimensioned);
	// HACK for GridConnectivity and others
	DataArray(ADF::Node const & mNode, Node * iParent)
#if _MSC_VER > 1300
		: Dimensioned(mNode, iParent->MSC_amIParented() ? static_cast<Dimensioned *>(iParent) : 0) {}
#else
		: Dimensioned(mNode, dynamic_cast<Dimensioned *>(iParent)) {}	
#endif 
	std::auto_ptr<CGNS::DataArray::DataConversion> mConversion;
	std::auto_ptr<CGNS::DataArray::DimensionalExponents> mExponents;
	CGNS::DataArray::Quantity mQuantity;
	void Modify(CGNS::DataArray::Quantity iQuantity);
    // necessary due to name hiding
	void Modify() {}
	
	USING_READ(Dimensioned);
	static void Read(DataArray * me);
	static void Read(DataArray * me,
		std::auto_ptr<CGNS::DataArray::DataConversion> & cDest);
	static void Read(DataArray * me,
		std::auto_ptr<CGNS::DataArray::DimensionalExponents> & cDest);
DISTINCT_CHILDREN2(CGNS::DataArray::DataConversion, CGNS::DataArray::DimensionalExponents);
};

#endif
