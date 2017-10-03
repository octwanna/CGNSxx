/***************************************************************************
                          cgnsdimensioned.h  -  description
                             -------------------
    begin                : Tue Sep 18 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSDIMENSIONED_H
#define CGNSDIMENSIONED_H

#include "cgnsparented.h"


/** Implementation data for nodes with dimensional data
  */
struct CGNS::Private::Dimensioned : CGNS::Private::Parented  {
COMMON_NODE_INTERFACE(Dimensioned, Dimensioned, Parented);
	std::auto_ptr<DataClass> mClass;
	std::auto_ptr<DimensionalUnits> mUnits;
	
	bool hasDataClass() const;
	bool hasDimensionalUnits() const;
	DataClass getDataClass() const;
	DimensionalUnits const & getDimensionalUnits() const;
	
	// specialisations
	USING_READ(Parented);
	static void Read(Dimensioned * me, std::auto_ptr<DataClass> & cDest);
	static void Read(Dimensioned * me, std::auto_ptr<DimensionalUnits> & cDest);
	USING_NEEDS(Parented);
	static void Needs(Dimensioned * me,
		std::auto_ptr<DataClass> & cDest);
	static void Needs(Dimensioned * me,
		std::auto_ptr<DimensionalUnits> & cDest);
DISTINCT_CHILDREN2(DataClass, DimensionalUnits);
};

#endif
