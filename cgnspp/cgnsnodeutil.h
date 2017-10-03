/***************************************************************************
                          cgnsnodeutil.h  -  description
                             -------------------
    begin                : Thu May 23 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSNODEUTIL_H
#define CGNSNODEUTIL_H

#include "cgns++.h"

#include "cgnsparented.h"

#include "cgnsutil.h"
#include "cgnsparented.h"
	
namespace CGNS {
namespace Private {

void readIndexRange(std::string const & iName, Parented * me, CGNS::_Range3D & cDest);
void readIndex(std::string const & iName, Node * me, Zone const * zone, CGNS::_Index3D & cDest);
void readFamilyName(Node * me, std::auto_ptr<std::string> & cDest);
void readDiffusionModel(Node * me, Zone const * zone, std::auto_ptr<DiffusionModel_t> & cDest);

template<typename T>
void readIndexRange(std::string const & iName, Parented * me, T & cDest) {
	readIndexRange(iName, me, static_cast<CGNS::_Range3D &>(cDest.m3D)); }
template<typename T>
void readIndex(std::string const & iName, Node * me, Zone const * zone, T & cDest) {
	readIndex(iName, me, zone, static_cast<CGNS::_Index3D &>(cDest.m3D)); }
template<typename T>
void readIndexRange(std::string const & iName, Parented * me, std::auto_ptr<T> & cDest) {
	if (me->mNode.hasChild(iName)) {
		if (cDest.get()==0) cDest.reset(new T);
		readIndexRange(iName, me, static_cast<CGNS::_Range3D &>(cDest->m3D));
	} else cDest.reset(0); }
template<typename T>
void readIndex(std::string const & iName, Node * me, Zone const * zone, std::auto_ptr<T> & cDest) {
	if (me->mNode.hasChild(iName)) {
		if (cDest.get()==0) cDest.reset(new T);
		readIndex(iName, me, zone, static_cast<CGNS::_Index3D &>(cDest->m3D));
	} else cDest.reset(0); }

#define DO_COPY(name, itype, otype, dim) \
inline void copy##name(itype const & iP1, otype##dim##D * oP2) { \
	*oP2=iP1.m##dim##D; } \
inline void copy##name(std::auto_ptr<itype> const & iP1, otype##dim##D * oP2) { \
	*oP2=iP1->m##dim##D; } \
inline void copy##name(itype##dim##D const & iP1, itype & oP2) { \
	oP2.m##dim##D=iP1; } \
inline void copy##name(itype##dim##D const & iP1, std::auto_ptr<itype> & oP2) { \
	oP2->m##dim##D=iP1; }
#define DO_COPY_ALL(name, itype, otype) \
DO_COPY(name, itype, otype, 1) \
DO_COPY(name, itype, otype, 2) \
DO_COPY(name, itype, otype, 3)

	DO_COPY_ALL(Range, IndexRange, _Range)
	DO_COPY_ALL(Indices, Transform, _Index)
	DO_COPY_ALL(Indices, Normal, _Index)
	DO_COPY_ALL(Points, Point, _Coor)

}
}

#define GET_RANGE(class, name, dim)                        \
void class::get##name(CGNS::IndexRange##dim##D * oRange) const { \
	NEEDS(m##name);                                        \
	if (Private::Zone const * zone=D()->getZone())         \
		if (zone->getIndexDimension()!=dim)                \
			Private::Emit(Exception::WRONG_DIMENSION);     \
	Private::copyRange(D()->m##name, oRange);              \
}

#define WRITE_RANGE(class, name, dim)                                    \
CGNS::IndexRange##dim##D class::write##name(CGNS::IndexRange##dim##D iRange) { \
	NEEDS(m##name);                                                      \
	if (Private::Zone const * zone=D()->getZone())                       \
		if (zone->getIndexDimension()!=dim)                              \
			Private::Emit(Exception::WRONG_DIMENSION);                   \
	ADF::Node n(Private::assureChild(D(), #name, "IndexRange_t"));       \
	n.setDataTypeDimensions(ADF::I4, dim);                               \
	n.writeData(&iRange.mMinI);                                          \
	return iRange;                                                       \
}

#define DO_RANGE_ALL(class, name) \
GET_RANGE(class, name, 1)         \
WRITE_RANGE(class, name, 1)       \
GET_RANGE(class, name, 2)         \
WRITE_RANGE(class, name, 2)       \
GET_RANGE(class, name, 3)         \
WRITE_RANGE(class, name, 3)

#define GET_INDICES(name, type, class, dim)            \
void class::get##name(type##dim##D * oIndices) const { \
	NEEDS(m##name);                                    \
	if (Private::Zone const * zone=D()->getZone())     \
		if (zone->getIndexDimension()!=dim)            \
			Private::Emit(Exception::WRONG_DIMENSION); \
	Private::copyIndices(D()->m##name, oIndices);      \
}

#define WRITE_INDICES(name, type, class, dim)            \
type##dim##D class::write##name(type##dim##D iIndices) { \
	NEEDS(m##name);                                      \
	if (Private::Zone const * zone=D()->getZone())       \
		if (zone->getIndexDimension()!=dim)              \
			Private::Emit(Exception::WRONG_DIMENSION);   \
	ADF::Node n(Private::assureChild(D()->m##name,       \
		D(), type(iIndices)));                           \
	n.setDataTypeDimensions(ADF::I4, dim);               \
	n.writeData(&iIndices.mI);                           \
	return D()->m##name->m##dim##D=iIndices;             \
}

#define DO_INDICES_ALL(name, type, class) \
GET_INDICES(name, type, class, 1)         \
WRITE_INDICES(name, type, class, 1)       \
GET_INDICES(name, type, class, 2)         \
WRITE_INDICES(name, type, class, 2)       \
GET_INDICES(name, type, class, 3)         \
WRITE_INDICES(name, type, class, 3)       \

#define GET_COORS(name, type, class, dim)              \
void class::get##name(type##dim##D * oCoor) const {    \
	NEEDS(m##name);                                    \
	if (D()->getPhysicalDimension()!=dim)              \
			Private::Emit(Exception::WRONG_DIMENSION); \
	Private::copyPoints(D()->m##name, oCoor);          \
}

#define WRITE_COORS(name, type, class, dim)            \
type##dim##D class::write##name(type##dim##D iCoor) {  \
	NEEDS(m##name);                                    \
	if (D()->getPhysicalDimension()!=dim)              \
			Private::Emit(Exception::WRONG_DIMENSION); \
	ADF::Node n(Private::assureChild(D(), #name, "DataArray_t"));      \
	n.setDataTypeDimensions(ADF::R8, dim);             \
	n.writeData(&iCoor.mX);                            \
	Private::copyPoints(iCoor, D()->m##name);          \
	return iCoor;                                      \
}

#define DO_COORS_ALL(name, type, class) \
GET_COORS(name, type, class, 1)         \
WRITE_COORS(name, type, class, 1)       \
GET_COORS(name, type, class, 2)         \
WRITE_COORS(name, type, class, 2)       \
GET_COORS(name, type, class, 3)         \
WRITE_COORS(name, type, class, 3)       \

#endif
