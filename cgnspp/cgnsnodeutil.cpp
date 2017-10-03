/***************************************************************************
                          cgnsnodeutil.cpp  -  description
                             -------------------
    begin                : Thu May 23 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsnodeutil.h"

#include "cgnsutil.h"

#include "cgnszone.h"

#include <assert.h>

void CGNS::Private::readIndexRange(std::string const & iName, Parented * me, CGNS::_Range3D & cDest) {
	assert(&cDest);
	clear(cDest);
	if (me->mNode.hasChild(iName)) {
		ADF::Node range=me->mNode.getChild(iName);
		if (range.getLabel()==implementation_traits<CGNS::IndexRange>::Label()) {
			if (me->P())
				if (Zone const * zone=dynamic_cast<Zone const *>(me->P()->P()))
					checkNode(range, ADF::I4, zone->getIndexDimension(), 2);
			readRangeADF(range, reinterpret_cast<CGNS::IndexRange &>(cDest));
		}
	}
}

void CGNS::Private::readIndex(std::string const & iName, Node * me, Zone const * zone, CGNS::_Index3D & cDest) {
	char const * name=implementation_traits<Transform>::Name();
	if (me->mNode.hasChild(name)) {
		ADF::Node index=me->mNode.getChild(name);
		if (zone) {
			checkNode(index, ADF::I4, zone->getIndexDimension());
		}
		else {
			checkNode(index, ADF::I4, 0);
			checkIndex(index.getDimension(0), MAX_INDEX_DIMENSION+1, 1);
		}
		index.readData(&cDest.mI);
	}
}

void CGNS::Private::readFamilyName(Node * me, std::auto_ptr<std::string> & cDest) {
	if (me->mNode.getNumLabeledChildren("FamilyName_t")<1) { cDest.reset(0); return; }
	// in version <=1.20 FamiliyName_t has the family name as the node name
	// later on it is the C1 data in the node
	ADF::Node familyName=me->mNode.getLabeledChild("FamilyName_t");
	if (familyName.getName()!="FamilyName") {
		if (cDest.get()==0) cDest.reset(new std::string);
		*cDest=familyName.getName();
	}
	else readChild(me->mNode, "FamilyName", "FamilyName_t", cDest);
}

void CGNS::Private::readDiffusionModel(Node * me, Zone const * zone, std::auto_ptr<DiffusionModel_t> & cDest) {
	if (me->mNode.hasChild("DiffusionModel")) {
		int index=0;
		if (zone) {
			index=zone->getIndexDimension();
			index=index*(index+1)/2;
		}
		ADF::Node node=me->mNode.getChild("DiffusionModel");
		checkNode(node, ADF::I4, index);
		int dim=node.getDimension(0);
		if (dim!=1 && dim!=3 && dim!=6) Emit(Exception::WRONG_DIMENSION);
		if (!cDest.get()) cDest.reset(new DiffusionModel_t);
		node.readData(&cDest->mActive[0]);
		// handle the 2D case, where the IJ component is in the third place
		if (dim==3) {
			cDest->mActive[IJ_INDEX]=cDest->mActive[KK_INDEX];
			cDest->mActive[KK_INDEX]=0;
		}
	}
}

