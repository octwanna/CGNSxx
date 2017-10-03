/***************************************************************************
                          cgnsfamily.cpp  -  description
                             -------------------
    begin                : Fri Sep 28 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsfamily.h"

#include "cgnsbase.h"

#include "cgnsgeometryreference.h"

using CGNS::Family;

GENERATE_D(Family);

COMMON_SIMPLE_NODE_IMPLEMENTATION(Family);

INIT_IMPLEMENTATION2(Family, mBCType, mGeometries);

void CGNS::Private::Family::Read(CGNS::Private::Family * me, std::auto_ptr<CGNS::BC::BCType> & cDest) {
	readStringChild(me->mNode, "FamilyBC", "FamilyBC_t", cDest);
	me->mInitialized|=implementation_traits<CGNS::BC::BCType>::INIT_MASK;
}

OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION_WRITE(FamilyBC, CGNS::BC::BCType, Family, D()->mBCType);

N_CHILDS_IMPLEMENTATION_WRITE2(GeometryReference, GeometryReference, Family, D()->mGeometries,
	std::string const &, CGNS::GeometryReference::GeometryFormat);

