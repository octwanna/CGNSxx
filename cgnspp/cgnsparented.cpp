/***************************************************************************
                          cgnsparented.cpp  -  description
                             -------------------
    begin                : Thu May 23 2002
    copyright            : (C) 2002 by Dr. Manuel Kessler
    email                : kessler@iag.uni-stuttgart.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "cgnsparented.h"

// this is COMMON_NODE_IMPLEMENTATION
CGNS::Private::Parented::Parented(ADF::Node const & iNode,
	ParentClass * iParent) : BaseClass(iNode, 0), mParent(iParent) {}
CGNS::Private::Parented::~Parented() {}
template struct CGNS::refcnt_ptr<CGNS::Private::Parented>;

INIT_IMPLEMENTATION0(Parented)

void CGNS::Private::Parented::Emit(int iErr) const {
	// throw only if no exception is active
	std::cerr << "Parented::Emit in " << mNode.getName();
	if (P()) std::cerr << ", parent is " << P()->mNode.getName();
	std::cerr << std::endl;
	if (!std::uncaught_exception()) throw Exception(iErr);
	exit(-1);
}

