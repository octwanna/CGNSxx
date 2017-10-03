/***************************************************************************
                          cgnsgeometryreference.cpp  -  description
                             -------------------
    begin                : Tue May 28 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsgeometryreference.h"

#include "cgnsfamily.h"

using CGNS::GeometryReference;

GENERATE_D(GeometryReference);

COMMON_SIMPLE_NODE_IMPLEMENTATION(GeometryReference);

INIT_IMPLEMENTATION1(GeometryReference, mEntities);

void CGNS::Private::GeometryReference::Read(GeometryReference * me) {
	char const *name="GeometryFile";
	if (me->mNode.hasChild(name)) readADF(me->mNode.getChild(name), me->mFile);
	else warnRequired(name, "GeometryReference");
	name="GeometryFormat";
	if (me->mNode.hasChild(name))
		readADF(me->mNode.getChild(name), me->mFormat);
	else warnRequired(name, "GeometryReference");
	
	me->mInitialized|=INIT_MASK_SELF;
}

//void CGNS::Private::GeometryReference::Read(GeometryReference * me,
//	child_container<CGNS::GeometryReference::GeometryEntity> & cDest) {}

void CGNS::Private::GeometryReference::Modify(std::string const & iFile,
	CGNS::GeometryReference::GeometryFormat iFormat) {
	ADF::Node file(mNode.createChild("GeometryFile", "GeometryFile_t"));
	file.writeData(mFile=iFile);
	ADF::Node format(mNode.createChild("GeometryFormat", "GeometryFormat_t"));
	format.writeData(asString(mFormat=iFormat));
}

N_EMPTY_CHILDS_IMPLEMENTATION(GeometryEntity, GeometryEntity, GeometryReference, D()->mEntities);
template struct CGNS::iterator<GeometryReference::GeometryEntity>;

GeometryReference::GeometryFormat GeometryReference::getGeometryFormat() const {
	NEEDS_THIS();
	return D()->mFormat;
}
	
std::string GeometryReference::getGeometryFile() const {
	NEEDS_THIS();
	return D()->mFile;
}

std::string GeometryReference::writeGeometryEntity(std::string const & iEntity) {
	NEEDS(mEntities);
	Private::assureChild(D(), iEntity, Private::implementation_traits<GeometryEntity>::Label());
	D()->mEntities.insert(GeometryEntity(), iEntity);
	return iEntity;
}

void GeometryReference::deleteGeometryEntity(std::string const & iEntity) {
	NEEDS(mEntities);
	D()->mNode.deleteChild(iEntity);
	D()->mEntities.erase(iEntity);
}
