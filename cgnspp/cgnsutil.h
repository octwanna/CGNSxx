/***************************************************************************
                          cgnsutil.h  -  description
                             -------------------
    begin                : Fri Mar 16 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSUTIL_H
#define CGNSUTIL_H

#include "cgns++.h"

#include "cgnsbroker.h"

#include "adfutil.h"

#include <memory>
#include <set>

#ifdef HAVE_STD_C_HEADER
#include <cstring>
#define STRNCPY std::strncpy
#define STRNCMP std::strncmp
#define MEMMOVE std::memmove
#else
#include <string.h>
#define STRNCPY strncpy
#define STRNCMP strncmp
#define MEMMOVE memmove
#endif

#if 0
#define GENERATE_D(n)                                    \
static inline CGNS::Private::n * real_D(                 \
	CGNS::Private::Node * iImpl) {                       \
	return static_cast<CGNS::Private::n *>(iImpl);       \
}                                                        \
static inline CGNS::Private::n const * real_D(           \
	CGNS::Private::Node const * iImpl) {                 \
	return static_cast<CGNS::Private::n const *>(iImpl); \
}

#undef D
#define D() real_D(mImpl)
#else
#define GENERATE_D(n)                                    \
static inline CGNS::Private::n * real_D(                 \
	CGNS::Private::Node * iImpl, CGNS::n *) {                       \
	return static_cast<CGNS::Private::n *>(iImpl);       \
}                                                        \
static inline CGNS::Private::n const * real_D(           \
	CGNS::Private::Node const * iImpl, CGNS::n const *) {                 \
	return static_cast<CGNS::Private::n const *>(iImpl); \
}

#undef D
#define D() real_D(mImpl, this)
#endif

// help with the macros below
#define NEEDSM(child) D()->Needs(D(), child)
//CGNS::Private::needsChild(D(), child)
// and with the typical D()->Needs(D(), D()->mXXX)
#define NEEDS(child) NEEDSM(D()->child)
// and of course self
#define NEEDS_THIS() D()->Needs(D())

// later we use __file__ and __line__
#define EMIT_INTERNAL_ERROR() CGNS::Private::Emit(CGNS::Exception::INTERNAL_ERROR)

namespace CGNS {

//DEFINE_UNIQUE_STRUCTS(DonorRange, IndexRange);

using ADF::child_container;

namespace Private {

struct DiffusionModel_t {
	int mActive[6];
	DiffusionModel_t(DiffusionIndex iIndex) {
		for (int i=0; i<6; ++i) mActive[i]=0; mActive[iIndex]=1; }
	DiffusionModel_t() {
		for (int i=0; i<6; ++i) mActive[i]=0; }
};

extern CGNS::PreReadStrategy SStrategy;

inline void clear(CGNS::_Range3D & cDest) {
	cDest.mMinI=cDest.mMaxI=0;
	cDest.mMinJ=cDest.mMaxJ=0;
	cDest.mMinK=cDest.mMaxK=0;
}

/** Declare standard conversions between our basic types and a textual representation.
  */
#define DECLARE_CONVERT(type)                             \
bool convertN(std::string const & iString, type * oData); \
void convert(std::string const & iString, type * oData);  \
void convert(type iData, std::string * oString);          \
void writeADF(ADF::Node & iNode, type iData);             \
void readADF(ADF::Node const & iNode, type & iData)

DECLARE_CONVERT(GridLocation);
DECLARE_CONVERT(CGNS::Elements::ElementType);
DECLARE_CONVERT(CGNS::Base::SimulationType);
DECLARE_CONVERT(DataClass);
DECLARE_CONVERT(MassUnit);
DECLARE_CONVERT(LengthUnit);
DECLARE_CONVERT(TimeUnit);
DECLARE_CONVERT(TemperatureUnit);
DECLARE_CONVERT(AngleUnit);
DECLARE_CONVERT(CGNS::DataArray::Quantity);
DECLARE_CONVERT(CGNS::GridCoordinates::CoordinateType);
DECLARE_CONVERT(CGNS::Discreted::FieldType);
DECLARE_CONVERT(CGNS::GridConnectivity::GridConnectivityType);
DECLARE_CONVERT(CGNS::BC::BCType);
DECLARE_CONVERT(CGNS::GoverningEquations::GoverningEquationsType);
DECLARE_CONVERT(CGNS::GasModel::GasModelType);
DECLARE_CONVERT(CGNS::ViscosityModel::ViscosityModelType);
DECLARE_CONVERT(CGNS::ThermalConductivityModel::ThermalConductivityModelType);
DECLARE_CONVERT(CGNS::TurbulenceClosure::TurbulenceClosureType);
DECLARE_CONVERT(CGNS::TurbulenceModel::TurbulenceModelType);
DECLARE_CONVERT(CGNS::GeometryReference::GeometryFormat);
DECLARE_CONVERT(CGNS::RigidGridMotion::RigidGridMotionType);
DECLARE_CONVERT(CGNS::ArbitraryGridMotion::ArbitraryGridMotionType);
DECLARE_CONVERT(CGNS::ThermalRelaxationModel::ThermalRelaxationModelType);
DECLARE_CONVERT(CGNS::ChemicalKineticsModel::ChemicalKineticsModelType);
#undef DECLARE_CONVERT

/** Checks whether an error occured and brings it to attention.
  */
void Check(int iErr);

/** Emits the specified error.
  */
#ifdef __GNUC__
void Emit(int iErr) __attribute__((noreturn));
#else
void Emit(int iErr);
#endif

void checkIndex(int iIndex, int iMax, int iMin=0);

template<typename T>
inline T clampIndex(T iIndex, T iMin, T iMax) {
	if (iIndex<iMin) return iMin;
	if (iIndex>iMax) return iMax;
	return iIndex;
}

template<typename T>
inline T validateIndex(T iIndex, T iNull, T iUser) {
	if (iIndex<iNull | iIndex>iUser) return iNull;
	return iIndex;
}

template<typename T>
inline T validateIndex(T iIndex, T iNull) {
	if (iIndex<iNull) return iNull;
	return iIndex;
}

template<typename T1, typename T2>
inline void assertIndex(T1 iIndex, T2 iMin, T2 iMax, int iException) {
	if (iIndex<iMin | iIndex>iMax) Emit(iException);
}

template<typename T1, typename T2>
inline void assertIndex(T1 iIndex, T2 iMin, int iException) {
	if (iIndex<iMin) Emit(iException);
}

void checkString(char const * iString, int iMaxLength=0);

void warnRequired(char const * iElementName, char const * iParentName);

void checkRequired(ADF::Node const & iNode,
	char const * iElementName, char const * iParentName);

template<typename T>
inline void checkPointer(T * iPointer) {
	if (!iPointer) Emit(Exception::NULL_POINTER);
}

void checkPointer(char const * iPointer);

static inline ADF::DataType adfType(char) { return ADF::C1; }
static inline ADF::DataType adfType(int) { return ADF::I4; }
static inline ADF::DataType adfType(float) { return ADF::R4; }
static inline ADF::DataType adfType(double) { return ADF::R8; }

void checkNode(ADF::Node const & iNode, ADF::DataType iType, int iDim1);
void checkNode(ADF::Node const & iNode, ADF::DataType iType, int iDim1, int iDim2);
void checkNode(ADF::Node const & iNode, ADF::DataType iType, int iDim1, int iDim2, int iDim3);
void checkNodeReal(ADF::Node const & iNode, int iDim1);
void checkNodeReal(ADF::Node const & iNode, int iDim1, int iDim2);
void checkNodeReal(ADF::Node const & iNode, int iDim1, int iDim2, int iDim3);

void checkNodeAll(ADF::Node const & iNode, ADF::DataType iType, int iAllDim, int iMaxNumDims=0);

void deleteLabeledChildren(ADF::Node & iNode, std::string const & iLabel);

void readADF(ADF::Node const & iNode, int & cDest);
void readADF(ADF::Node const & iNode, std::string & cDest);
void readADF(ADF::Node const & iNode, _Range1D & cDest);
void readADF(ADF::Node const & iNode, _Range2D & cDest);
void readADF(ADF::Node const & iNode, _Range3D & cDest);
void readADF(ADF::Node const & iNode, Point & cDest);

template<typename T>
void readRangeADF(ADF::Node & iNode, T & cDest) {
	int numDim=iNode.getNumDimensions();
	if (numDim!=1 && numDim!=2) Emit(Exception::WRONG_NUM_DIMENSIONS);
	int dims=iNode.getDimension(0);
	if (numDim==2) dims*=iNode.getDimension(1);
	switch (dims) {
		case 2: readADF(iNode, cDest.m1D); return;
		case 4: readADF(iNode, cDest.m2D); return;
		case 6: readADF(iNode, cDest.m3D); return;
	}
	Emit(Exception::WRONG_DIMENSION);
}

void writeADF(ADF::Node & iNode, int iValue);
void writeADF(ADF::Node & iNode, std::string const & iValue);
void writeADF(ADF::Node & iNode, _Range1D const & iRange);
void writeADF(ADF::Node & iNode, _Range2D const & iRange);
void writeADF(ADF::Node & iNode, _Range3D const & iRange);


template<typename T>
inline T findName(std::string const & iString, char const * iNames[], T iNull, T iLast) {
	for (int t=iNull; t<=iLast; ++t)
		if (iString==iNames[t])
			return static_cast<T>(t);
	return iNull;
}

template<typename T>
inline bool findName(std::string const & iString, const char * const iNames[], int iLast, T * oT) {
	for (int t=0; t<=iLast; ++t)
		if (iString==iNames[t]) {
			*oT=static_cast<T>(t);
			return true;
		}
	return false;
}


template<typename T1, typename T2>
void readLabeledChildren(ADF::Node const & iNode, std::string const & iLabel,
	child_container<T1> & oContainer, T2 * iParent) {
	typedef typename implementation_traits<T1>::Impl Impl;
	for (ADF::Node::Child_iterator_t i=iNode.beginChild(); i!=iNode.endChild(); ++i) {
		if (i->getLabel()==iLabel) {
			Impl * impl=new Impl(*i, iParent);
			impl->Init();
			oContainer.insert(impl);
		}
	}
}

template<typename T>
void readLabeledChildren(ADF::Node const & iNode, std::string const & iLabel,
	child_container<CGNS::IndexRange> & oContainer, T * iParent) {
	for (ADF::Node::Child_iterator_t i=iNode.beginChild(); i!=iNode.endChild(); ++i) {
		if (i->getLabel()==iLabel) {
			IndexRange index;
			readRangeADF(*i, index);
			oContainer.insert(index, i->getName());
		}
	}
}

template<typename T>
void readLabeledChildren(ADF::Node const & iNode, std::string const & iLabel,
	child_container<CGNS::GeometryReference::GeometryEntity> & oContainer, T * iParent) {
	for (ADF::Node::Child_iterator_t i=iNode.beginChild(); i!=iNode.endChild(); ++i) {
		if (i->getLabel()==iLabel) {
			oContainer.insert(CGNS::GeometryReference::GeometryEntity(), i->getName());
		}
	}
}

template<typename T1, typename T2>
void readChild(ADF::Node & iNode, std::string const & iName,
	std::string const & iLabel, refcnt_ptr<T1> & iDest, T2 * iParent) {
	if (iNode.hasChild(iName)) {
		ADF::Node child=iNode.getChild(iName);
		if (child.getLabel()==iLabel) {
			iDest.reset(new T1(child, iParent));
			iDest->Init();
		}
	}
}

template<typename T1, typename T2>
inline void readChild(ADF::Node & iNode, std::string const & iName,
	refcnt_ptr<T1> & iDest, T2 * iParent) {
	readChild(iNode, iName, implementation_traits<T1>::Label(), iDest, iParent);
}

template<typename T>
void readChild(ADF::Node & iNode, std::string const & iName,
	std::string const & iLabel, std::auto_ptr<T> & iDest) {
	if (iNode.hasChild(iName)) {
		ADF::Node child=iNode.getChild(iName);
		if (child.getLabel()==iLabel) {
			if (iDest.get()==0) iDest.reset(new T);
			readADF(child, *iDest);
		}
	}
}

template<typename T>
bool readStringChild(ADF::Node & iNode, std::string const & iName,
	std::string const & iLabel, T & cDest) {
	if (iNode.hasChild(iName)) {
		ADF::Node child=iNode.getChild(iName);
		if (child.getLabel()==iLabel) {
			std::string text;
			child.readData(&text);
			convert(text, &cDest);
			return true;
		}
	}
	return false;
}

template<typename T>
bool readStringChild(ADF::Node & iNode, std::string const & iName, T & cDest) {
	return readStringChild(iNode, iName,
		implementation_traits<T>::Label(), cDest);
}

template<typename T>
bool readStringChild(ADF::Node & iNode, T & cDest) {
	return readStringChild(iNode, implementation_traits<T>::Name(),
		implementation_traits<T>::Label(), cDest);
}

template<typename T>
bool readStringChild(ADF::Node & iNode, std::string const & iName,
	std::string const & iLabel, std::auto_ptr<T> & cDest) {
	if (iNode.hasChild(iName)) {
		ADF::Node child=iNode.getChild(iName);
		if (child.getLabel()==iLabel) {
			std::string text;
			child.readData(&text);
			cDest.reset(new T);
			convert(text, cDest.get());
			return true;
		}
	}
    cDest.reset(0);
	return false;
}

// needed extra due to the implementation_traits<T> access
template<typename T>
inline bool readStringChild(ADF::Node & iNode, std::string const & iName,
	std::auto_ptr<T> & iDest) {
	return readStringChild(iNode, iName, implementation_traits<T>::Label(), iDest);
}

inline void wipe(IndexRange & cContainer) {}

template<typename T>
inline void wipe(T & cContainer) {
	cContainer.clear();
}

template<typename T>
inline void wipe(refcnt_ptr<T> & cPtr) {
	cPtr.release();
}

template<typename T>
inline void wipe(std::auto_ptr<T> & iDest) {
	delete iDest.release();
}

template<typename T>	
inline T & getChild(child_container<T> & iContainer, std::string const & iName) {	
	if (iContainer.has(iName)) return iContainer.find(iName);
	Emit(Exception::NO_NAMED_CHILD);
	// not sure why gcc 3.0.2 needs this const_cast, MIPSpro, gcc 2.95.3 and 3.1 do without
	return const_cast<T &>(iContainer.begin()->second);
}


template<typename T>	
inline T const & getChild(child_container<T> const & iContainer, std::string const & iName) {	
	if (iContainer.has(iName)) return iContainer.find(iName);
	Emit(Exception::NO_NAMED_CHILD);
	return iContainer.begin()->second;
}

template<typename T>	
inline bool hasChild(T const & iContainer, std::string const & iName) {	
	return iContainer.has(iName);
}

template<typename T1, typename T2>
typename implementation_traits<T1>::Impl *
assureChild(child_container<T1> & iContainer, std::string const & iName,
	std::string const & iLabel, T2 * iParent) {
	typedef typename implementation_traits<T1>::Impl Impl;
	if (iContainer.has(iName)) {
		T1 & n=iContainer.find(iName);
		Impl * impl=static_cast<Impl *>(Broker::GetImplementation(n));
		impl->Wipe();
		return impl;
	}
	ADF::Node child=iParent->mNode.createChild(iName, iLabel);
	std::auto_ptr<Impl> impl(new Impl(child, iParent));
	iContainer.insert(impl.get());
	return impl.release();
}

template<typename T1, typename T2>
inline typename implementation_traits<T1>::Impl *
assureChild(child_container<T1> & iContainer, std::string const & iName,
	T2 * iParent) {
	return assureChild(iContainer, iName, implementation_traits<T1>::Label(), iParent);
}

template<typename T1, typename T2>
T1 * assureChild(refcnt_ptr<T1> & iDest, std::string const & iName,
	std::string const & iLabel, T2 * iParent, bool /* dummy to disambiguate from the former*/) {
	if (!iDest) {
		ADF::Node child=iParent->mNode.createChild(iName, iLabel);
 		iDest.reset(new T1(child, iParent));
	}
	else iDest->Wipe();
	return iDest.get();
}

template<typename T1, typename T2>
inline T1 * assureChild(refcnt_ptr<T1> & iDest, std::string const & iName,
	T2 * iParent) {
	return assureChild(iDest, iName, implementation_traits<T1>::Label(),
		iParent, true);
}

template<typename T1, typename T2>
inline T1 * assureChild(refcnt_ptr<T1> & iDest, T2 * iParent) {
	return assureChild(iDest, implementation_traits<T1>::Name(), implementation_traits<T1>::Label(),
		iParent, true);
}

template<typename T1, typename T2>
ADF::Node assureChild(std::auto_ptr<T1> & iChild, T2 * iParent,
	T1 const & iData, std::string const & iName, std::string const & iLabel) {
	if (!iParent->mNode.hasChild(iName))
		 iParent->mNode.createChild(iName, iLabel);
	ADF::Node n(iParent->mNode.getChild(iName));
	if (!iChild.get()) iChild.reset(new T1(iData));
	else *iChild=iData;
	return n;
}

template<typename T1, typename T2>
inline ADF::Node assureChild(std::auto_ptr<T1> & iChild, T2 * iParent,
	T1 const & iData, std::string const & iName) {
	return assureChild(iChild, iParent, iData, iName, implementation_traits<T1>::Label());
}

template<typename T1, typename T2>
inline ADF::Node assureChild(std::auto_ptr<T1> & iChild, T2 * iParent,
	T1 const & iData) {
	return assureChild(iChild, iParent, iData,
		implementation_traits<T1>::Name(), implementation_traits<T1>::Label());
}

template<typename T>
inline ADF::Node assureChild(T * iParent, std::string const & iName, std::string const & iLabel) {
	if (!iParent->mNode.hasChild(iName))
		 iParent->mNode.createChild(iName, iLabel);
	return iParent->mNode.getChild(iName);
}

template<typename T1, typename T2>
inline ADF::Node assureChild(T1 * iParent, T2 const & /* dummy for implementation_traits */) {
	char const * name=implementation_traits<T2>::Name();
	if (!iParent->mNode.hasChild(name))
		 iParent->mNode.createChild(name, implementation_traits<T2>::Label());
	return iParent->mNode.getChild(name);
}

// special case for simple types
template<typename T1, typename T2>
inline ADF::Node assureChild(child_container<T1> & iContainer, T2 * iParent,
	T1 const & iData, std::string const & iName) {
	if (iContainer.has(iName)) iContainer.find(iName)=iData;
	else iContainer.insert(iData, iName);
	return assureChild(iParent, iName, implementation_traits<T2>::Label());
}

// another special case for simple required types
template<typename T1, typename T2>
inline ADF::Node assureChild(T1 & iContainer, T2 * iParent,
	T1 const & iData, std::string const & iName, std::string const & iLabel) {
	iContainer=iData;
	return assureChild(iParent, iName, iLabel);
}

} // Private

} // CGNS



#define N_EMPTY_CHILDS_IMPLEMENTATION(name, type, class, container)  \
int class::getNum##name() const { NEEDSM(container);           \
	return container.size(); }                                 \
CGNS::class::name##_iterator_t class::begin##name() const {    \
	NEEDSM(container);                                         \
	child_container<type>::const_iterator                      \
		iter(container.begin());                               \
	return name##_iterator_t(*reinterpret_cast<void **>(&iter)); } \
CGNS::class::name##_iterator_t class::end##name() const {      \
	NEEDSM(container);                                         \
	child_container<type>::const_iterator                      \
		iter(container.end());                                 \
	return name##_iterator_t(*reinterpret_cast<void **>(&iter)); }
	
#define N_CHILDS_IMPLEMENTATION_NOGET(name, type, class, container)  \
N_EMPTY_CHILDS_IMPLEMENTATION(name, type, class, container)    \
bool class::has##name(std::string const & iName) const {       \
	NEEDSM(container); return container.has(iName); }          \
	
#define N_CHILDS_IMPLEMENTATION_NOITER(name, type, class, container) \
N_CHILDS_IMPLEMENTATION_NOGET(name, type, class, container)    \
CGNS::type class::get##name(std::string const & iName) const { \
	NEEDSM(container);                                         \
	return CGNS::Private::getChild(container, iName); }

#define N_CHILDS_IMPLEMENTATION(name, type, class, container)  \
N_CHILDS_IMPLEMENTATION_NOITER(name, type, class, container)   \
template struct CGNS::iterator<CGNS::type>;                    

#define CHILDS_IMPLEMENTATION_WRITE(name, type, class, container) \
CGNS::type class::write##name(std::string const & iName) {        \
	NEEDSM(container);                                            \
	return CGNS::Private::assureChild(container, iName, #name "_t", D());\
}

#define CHILDS_IMPLEMENTATION_WRITE1(name, type, class, container, p1) \
CGNS::type class::write##name(std::string const & iName, p1 iP1) {     \
	NEEDSM(container);                                                 \
	Private::implementation_traits<type>::Impl * child=                \
		Private::assureChild(container, iName, #name "_t", D());       \
	child->Modify(iP1);                                                \
	return child;                                                      \
}

#define CHILDS_IMPLEMENTATION_WRITE2(name, type, class, container, p1, p2) \
CGNS::type class::write##name(std::string const & iName, p1 iP1, p2 iP2) { \
	NEEDSM(container);                                                     \
	Private::type * child=Private::assureChild(container,                  \
		iName, #name "_t", D());                                           \
	child->Modify(iP1, iP2);                                               \
	return child;                                                          \
}

#define N_CHILDS_IMPLEMENTATION_WRITE(name, type, class, container) \
N_CHILDS_IMPLEMENTATION(name, type, class, container)               \
CHILDS_IMPLEMENTATION_WRITE(name, type, class, container)

#define N_CHILDS_IMPLEMENTATION_WRITE1(name, type, class, container, p1) \
N_CHILDS_IMPLEMENTATION(name, type, class, container)                    \
CHILDS_IMPLEMENTATION_WRITE1(name, type, class, container, p1)

#define N_CHILDS_IMPLEMENTATION_WRITE2(name, type, class, container, p1, p2) \
N_CHILDS_IMPLEMENTATION(name, type, class, container)                        \
CHILDS_IMPLEMENTATION_WRITE2(name, type, class, container, p1, p2)

// needed since we have the Model<T> cases, which behave as Valued
#define CHILD_IMPLEMENTATION(name, type, class, container) \
CGNS::type class::get##name() const {                      \
	NEEDSM(container);                                     \
	return const_cast<Private::implementation_traits<type>::Impl *>( \
		static_cast<Private::implementation_traits<type>::Impl const *>(container.get())); }
//	return const_cast<Private::type *>(container.get()); }
//	return Private::implementation_traits<type>::Ret(container.get()); }

#define CHILD_IMPLEMENTATION_WRITE(name, type, class, container) \
CGNS::type class::write##name() {                                \
	NEEDSM(container);                                           \
	Private::implementation_traits<type>::Impl * child=          \
		Private::assureChild(container, #name, D());             \
	child->Modify();                                             \
	return child;                                                \
}

#define CHILD_IMPLEMENTATION_WRITE1(name, type, class, container, p1)  \
CGNS::type class::write##name(p1 iP1) {                                \
	NEEDSM(container);                                                 \
	Private::implementation_traits<type>::Impl * child=                \
		Private::assureChild(container, D());                          \
	child->Modify(iP1);                                                \
	return child;                                                      \
}

#define CHILD_IMPLEMENTATION_WRITE_INDEX(name, type, class, container)  \
CGNS::type class::write##name() {                                       \
	NEEDSM(container);                                                  \
	Private::implementation_traits<type>::Impl * child=                 \
		Private::assureChild(container, #name, D());                    \
	int index=D()->getZone() ? D()->getZone()->getIndexDimension() : 3; \
	child->Modify(index);                                               \
	return child;                                                       \
}

#define OPTIONAL_CHILD_IMPLEMENTATION(name, type, class, container) \
CHILD_IMPLEMENTATION(name, type, class, container)                  \
bool class::has##name() const {	NEEDSM(container);                  \
	return container.get()!=0; }                                    \
void class::delete##name() { if (!container.get()) return;          \
	NEEDSM(container);                                              \
	container->Wipe(); D()->mNode.deleteChild(container->mNode);    \
	container.release(); }

#define OPTIONAL_CHILD_IMPLEMENTATION_WRITE(name, type, class, container) \
OPTIONAL_CHILD_IMPLEMENTATION(name, type, class, container)               \
CHILD_IMPLEMENTATION_WRITE(name, type, class, container)

#define OPTIONAL_CHILD_IMPLEMENTATION_WRITE1(name, type, class, container, p1) \
OPTIONAL_CHILD_IMPLEMENTATION(name, type, class, container)                    \
CHILD_IMPLEMENTATION_WRITE1(name, type, class, container, p1)

#define OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION(name, type, class, container) \
bool class::has##name() const { NEEDSM(container);                         \
	return container.get()!=0; }                                           \
type class::get##name() const { NEEDSM(container);                         \
	return *container; }                                                   \
void class::delete##name() { if (!container.get()) return;                 \
	NEEDSM(container);                                                     \
	D()->mNode.deleteChild(#name);                                         \
	container.release(); }

#define OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION_WRITE(name, type, class, container) \
OPTIONAL_SIMPLE_CHILD_IMPLEMENTATION(name, type, class, container)               \
type class::write##name(type iP1) {                                              \
	NEEDSM(container);                                                           \
	ADF::Node node=Private::assureChild(container, D(), iP1,                     \
		#name, #name "_t");                                                      \
	Private::writeADF(node, iP1);                                                \
	return iP1;                                                                  \
}

#define SIMPLE_CHILD_IMPLEMENTATION(name, type, class, container) \
type class::get##name() const { \
	NEEDS_THIS(); \
	return container; \
}

#define SIMPLE_CHILD_IMPLEMENTATION_WRITE(name, type, class, container) \
SIMPLE_CHILD_IMPLEMENTATION(name, type, class, container)               \
type class::write##name(type iP1) {                                     \
	NEEDS_THIS();                                                         \
	ADF::Node node=Private::assureChild(D(), iP1);                      \
	Private::writeADF(node, iP1);                                       \
	return container=iP1;                                               \
}


#define INIT_IMPLEMENTATION_HEAD(class) \
void CGNS::Private::class::Init() { \
	CT_ASSERT(CHILDREN_MASK==CHILDREN_SUM); \
	BaseClass::Init(); \
	if (implementation_traits<class>::INIT_MASK) \
		if (SStrategy<=INTERNAL_PREREAD) Needs(this);
	
#define WIPE_IMPLEMENTATION_HEAD(class) \
void CGNS::Private::class::Wipe() {

#define WIPE_IMPLEMENTATION_TAIL() \
	BaseClass::Wipe(); }
	
#define INIT_IMPLEMENTATION_CHILD(child) \
	Needs(this, child);
	
#define WIPE_IMPLEMENTATION_CHILD(child) \
	wipe(child);
	
#define INIT_IMPLEMENTATION_CHILD1(c1) \
	if (SStrategy<=FULL_PREREAD) { \
INIT_IMPLEMENTATION_CHILD(c1) \
	}
#define INIT_IMPLEMENTATION_CHILD2(c1,c2) \
	if (SStrategy<=FULL_PREREAD) { \
INIT_IMPLEMENTATION_CHILD(c1) \
INIT_IMPLEMENTATION_CHILD(c2) \
	}
#define INIT_IMPLEMENTATION_CHILD3(c1,c2,c3) \
	if (SStrategy<=FULL_PREREAD) { \
INIT_IMPLEMENTATION_CHILD(c1) \
INIT_IMPLEMENTATION_CHILD(c2) \
INIT_IMPLEMENTATION_CHILD(c3) \
	}
#define INIT_IMPLEMENTATION_CHILD4(c1,c2,c3,c4)	\
	if (SStrategy<=FULL_PREREAD) { \
INIT_IMPLEMENTATION_CHILD(c1) \
INIT_IMPLEMENTATION_CHILD(c2) \
INIT_IMPLEMENTATION_CHILD(c3) \
INIT_IMPLEMENTATION_CHILD(c4) \
	}
#define INIT_IMPLEMENTATION_CHILD5(c1,c2,c3,c4,c5) \
	if (SStrategy<=FULL_PREREAD) { \
INIT_IMPLEMENTATION_CHILD(c1) \
INIT_IMPLEMENTATION_CHILD(c2) \
INIT_IMPLEMENTATION_CHILD(c3) \
INIT_IMPLEMENTATION_CHILD(c4) \
INIT_IMPLEMENTATION_CHILD(c5) \
	}

#define INIT_IMPLEMENTATION0(class) \
INIT_IMPLEMENTATION_HEAD(class) } \
WIPE_IMPLEMENTATION_HEAD(class) \
WIPE_IMPLEMENTATION_TAIL()
#define INIT_IMPLEMENTATION1(class, c1) \
INIT_IMPLEMENTATION_HEAD(class) \
INIT_IMPLEMENTATION_CHILD1(c1) } \
WIPE_IMPLEMENTATION_HEAD(class) \
WIPE_IMPLEMENTATION_CHILD(c1) \
WIPE_IMPLEMENTATION_TAIL()
#define INIT_IMPLEMENTATION2(class, c1, c2) \
INIT_IMPLEMENTATION_HEAD(class) \
INIT_IMPLEMENTATION_CHILD2(c1,c2) } \
WIPE_IMPLEMENTATION_HEAD(class) \
WIPE_IMPLEMENTATION_CHILD(c2) \
WIPE_IMPLEMENTATION_CHILD(c1) \
WIPE_IMPLEMENTATION_TAIL()
#define INIT_IMPLEMENTATION3(class, c1, c2, c3) \
INIT_IMPLEMENTATION_HEAD(class) \
INIT_IMPLEMENTATION_CHILD3(c1,c2,c3) } \
WIPE_IMPLEMENTATION_HEAD(class) \
WIPE_IMPLEMENTATION_CHILD(c3) \
WIPE_IMPLEMENTATION_CHILD(c2) \
WIPE_IMPLEMENTATION_CHILD(c1) \
WIPE_IMPLEMENTATION_TAIL()
#define INIT_IMPLEMENTATION4(class, c1, c2, c3, c4) \
INIT_IMPLEMENTATION_HEAD(class) \
INIT_IMPLEMENTATION_CHILD4(c1,c2,c3,c4) } \
WIPE_IMPLEMENTATION_HEAD(class) \
WIPE_IMPLEMENTATION_CHILD(c4) \
WIPE_IMPLEMENTATION_CHILD(c3) \
WIPE_IMPLEMENTATION_CHILD(c2) \
WIPE_IMPLEMENTATION_CHILD(c1) \
WIPE_IMPLEMENTATION_TAIL()
#define INIT_IMPLEMENTATION5(class, c1, c2, c3, c4, c5) \
INIT_IMPLEMENTATION_HEAD(class) \
INIT_IMPLEMENTATION_CHILD5(c1,c2,c3,c4,c5) } \
WIPE_IMPLEMENTATION_HEAD(class) \
WIPE_IMPLEMENTATION_CHILD(c5) \
WIPE_IMPLEMENTATION_CHILD(c4) \
WIPE_IMPLEMENTATION_CHILD(c3) \
WIPE_IMPLEMENTATION_CHILD(c2) \
WIPE_IMPLEMENTATION_CHILD(c1) \
WIPE_IMPLEMENTATION_TAIL()
#define INIT_IMPLEMENTATION6(class, c1, c2, c3, c4, c5,c6) \
INIT_IMPLEMENTATION_HEAD(class) \
INIT_IMPLEMENTATION_CHILD3(c1,c2,c3) \
INIT_IMPLEMENTATION_CHILD3(c4,c5,c6) } \
WIPE_IMPLEMENTATION_HEAD(class) \
WIPE_IMPLEMENTATION_CHILD(c6) \
WIPE_IMPLEMENTATION_CHILD(c5) \
WIPE_IMPLEMENTATION_CHILD(c4) \
WIPE_IMPLEMENTATION_CHILD(c3) \
WIPE_IMPLEMENTATION_CHILD(c2) \
WIPE_IMPLEMENTATION_CHILD(c1) \
WIPE_IMPLEMENTATION_TAIL()
#define INIT_IMPLEMENTATION7(class, c1, c2, c3, c4, c5,c6,c7) \
INIT_IMPLEMENTATION_HEAD(class) \
INIT_IMPLEMENTATION_CHILD5(c1,c2,c3,c4,c5) \
INIT_IMPLEMENTATION_CHILD2(c6,c7) } \
WIPE_IMPLEMENTATION_HEAD(class) \
WIPE_IMPLEMENTATION_CHILD(c7) \
WIPE_IMPLEMENTATION_CHILD(c6) \
WIPE_IMPLEMENTATION_CHILD(c5) \
WIPE_IMPLEMENTATION_CHILD(c4) \
WIPE_IMPLEMENTATION_CHILD(c3) \
WIPE_IMPLEMENTATION_CHILD(c2) \
WIPE_IMPLEMENTATION_CHILD(c1) \
WIPE_IMPLEMENTATION_TAIL()
#define INIT_IMPLEMENTATION10(class, c1, c2, c3, c4, c5,c6,c7,c8,c9,c10) \
INIT_IMPLEMENTATION_HEAD(class) \
INIT_IMPLEMENTATION_CHILD5(c1,c2,c3,c4,c5) \
INIT_IMPLEMENTATION_CHILD5(c6,c7,c8,c9,c10) } \
WIPE_IMPLEMENTATION_HEAD(class) \
WIPE_IMPLEMENTATION_CHILD(c10) \
WIPE_IMPLEMENTATION_CHILD(c9) \
WIPE_IMPLEMENTATION_CHILD(c8) \
WIPE_IMPLEMENTATION_CHILD(c7) \
WIPE_IMPLEMENTATION_CHILD(c6) \
WIPE_IMPLEMENTATION_CHILD(c5) \
WIPE_IMPLEMENTATION_CHILD(c4) \
WIPE_IMPLEMENTATION_CHILD(c3) \
WIPE_IMPLEMENTATION_CHILD(c2) \
WIPE_IMPLEMENTATION_CHILD(c1) \
WIPE_IMPLEMENTATION_TAIL()
#define INIT_IMPLEMENTATION11(class, c1, c2, c3, c4, c5,c6,c7,c8,c9,c10,c11) \
INIT_IMPLEMENTATION_HEAD(class) \
INIT_IMPLEMENTATION_CHILD5(c1,c2,c3,c4,c5) \
INIT_IMPLEMENTATION_CHILD5(c6,c7,c8,c9,c10) \
INIT_IMPLEMENTATION_CHILD1(c11) } \
WIPE_IMPLEMENTATION_HEAD(class) \
WIPE_IMPLEMENTATION_CHILD(c11) \
WIPE_IMPLEMENTATION_CHILD(c10) \
WIPE_IMPLEMENTATION_CHILD(c9) \
WIPE_IMPLEMENTATION_CHILD(c8) \
WIPE_IMPLEMENTATION_CHILD(c7) \
WIPE_IMPLEMENTATION_CHILD(c6) \
WIPE_IMPLEMENTATION_CHILD(c5) \
WIPE_IMPLEMENTATION_CHILD(c4) \
WIPE_IMPLEMENTATION_CHILD(c3) \
WIPE_IMPLEMENTATION_CHILD(c2) \
WIPE_IMPLEMENTATION_CHILD(c1) \
WIPE_IMPLEMENTATION_TAIL()

#define COMBINE_MASKS1(op, c1) \
(implementation_traits<c1>::INIT_MASK)
#define COMBINE_MASKS2(op, c1, c2) \
(COMBINE_MASKS1(op, c1) op implementation_traits<c2>::INIT_MASK)
#define COMBINE_MASKS3(op, c1, c2, c3) \
(COMBINE_MASKS2(op, c1, c2) op implementation_traits<c3>::INIT_MASK)
#define COMBINE_MASKS4(op, c1, c2, c3, c4) \
(COMBINE_MASKS3(op, c1, c2, c3) op implementation_traits<c4>::INIT_MASK)
#define COMBINE_MASKS5(op, c1, c2, c3, c4, c5) \
(COMBINE_MASKS4(op, c1, c2, c3, c4) op implementation_traits<c5>::INIT_MASK)
#define COMBINE_MASKS6(op, c1, c2, c3, c4, c5, c6) \
(COMBINE_MASKS5(op, c1, c2, c3, c4, c5) op implementation_traits<c6>::INIT_MASK)

#define DISTINCT_CHILDREN0() \
public: \
enum { MY_CHILDREN_MASK=0, MY_CHILDREN_SUM=0, \
	CHILDREN_MASK=BaseClass::CHILDREN_MASK, \
	CHILDREN_SUM=BaseClass::CHILDREN_SUM }
#define DISTINCT_CHILDREN1(c1) \
public: \
enum { MY_CHILDREN_MASK=COMBINE_MASKS1(|,c1), \
	MY_CHILDREN_SUM=COMBINE_MASKS1(+,c1), \
	CHILDREN_MASK=MY_CHILDREN_MASK|BaseClass::CHILDREN_MASK, \
	CHILDREN_SUM=MY_CHILDREN_SUM+BaseClass::CHILDREN_SUM }
#define DISTINCT_CHILDREN2(c1, c2) \
public: \
enum { MY_CHILDREN_MASK=COMBINE_MASKS2(|,c1,c2), \
	MY_CHILDREN_SUM=COMBINE_MASKS2(+,c1,c2), \
	CHILDREN_MASK=MY_CHILDREN_MASK|BaseClass::CHILDREN_MASK, \
	CHILDREN_SUM=MY_CHILDREN_SUM+BaseClass::CHILDREN_SUM }
#define DISTINCT_CHILDREN3(c1, c2, c3) \
public: \
enum { MY_CHILDREN_MASK=COMBINE_MASKS3(|,c1,c2,c3), \
	MY_CHILDREN_SUM=COMBINE_MASKS3(+,c1,c2,c3), \
	CHILDREN_MASK=MY_CHILDREN_MASK|BaseClass::CHILDREN_MASK, \
	CHILDREN_SUM=MY_CHILDREN_SUM+BaseClass::CHILDREN_SUM }
#define DISTINCT_CHILDREN4(c1, c2, c3, c4) \
public: \
enum { MY_CHILDREN_MASK=COMBINE_MASKS4(|,c1,c2,c3,c4), \
	MY_CHILDREN_SUM=COMBINE_MASKS4(+,c1,c2,c3,c4), \
	CHILDREN_MASK=MY_CHILDREN_MASK|BaseClass::CHILDREN_MASK, \
	CHILDREN_SUM=MY_CHILDREN_SUM+BaseClass::CHILDREN_SUM }
#define DISTINCT_CHILDREN5(c1, c2, c3, c4,c5) \
public: \
enum { MY_CHILDREN_MASK=COMBINE_MASKS5(|,c1,c2,c3,c4,c5), \
	MY_CHILDREN_SUM=COMBINE_MASKS5(+,c1,c2,c3,c4,c5), \
	CHILDREN_MASK=MY_CHILDREN_MASK|BaseClass::CHILDREN_MASK, \
	CHILDREN_SUM=MY_CHILDREN_SUM+BaseClass::CHILDREN_SUM }
#define DISTINCT_CHILDREN6(c1, c2, c3, c4,c5,c6) \
public: \
enum { MY_CHILDREN_MASK=COMBINE_MASKS4(|,c1,c2,c3,c4)|COMBINE_MASKS2(|,c5,c6), \
	MY_CHILDREN_SUM=COMBINE_MASKS4(+,c1,c2,c3,c4)+COMBINE_MASKS2(+,c5,c6), \
	CHILDREN_MASK=MY_CHILDREN_MASK|BaseClass::CHILDREN_MASK, \
	CHILDREN_SUM=MY_CHILDREN_SUM+BaseClass::CHILDREN_SUM }
#define DISTINCT_CHILDREN7(c1, c2, c3, c4,c5,c6,c7) \
public: \
enum { MY_CHILDREN_MASK=COMBINE_MASKS5(|,c1,c2,c3,c4,c5)|COMBINE_MASKS2(|,c6,c7), \
	MY_CHILDREN_SUM=COMBINE_MASKS5(+,c1,c2,c3,c4,c5)+COMBINE_MASKS2(+,c6,c7), \
	CHILDREN_MASK=MY_CHILDREN_MASK|BaseClass::CHILDREN_MASK, \
	CHILDREN_SUM=MY_CHILDREN_SUM+BaseClass::CHILDREN_SUM }
#define DISTINCT_CHILDREN8(c1, c2, c3, c4,c5,c6,c7,c8) \
public: \
enum { MY_CHILDREN_MASK=COMBINE_MASKS5(|,c1,c2,c3,c4,c5)|COMBINE_MASKS3(|,c6,c7,c8), \
	MY_CHILDREN_SUM=COMBINE_MASKS5(+,c1,c2,c3,c4,c5)+COMBINE_MASKS3(+,c6,c7,c8), \
	CHILDREN_MASK=MY_CHILDREN_MASK|BaseClass::CHILDREN_MASK, \
	CHILDREN_SUM=MY_CHILDREN_SUM+BaseClass::CHILDREN_SUM }
#define DISTINCT_CHILDREN10(c1, c2, c3, c4,c5,c6,c7,c8,c9,c10) \
public: \
enum { MY_CHILDREN_MASK=COMBINE_MASKS5(|,c1,c2,c3,c4,c5)|COMBINE_MASKS5(|,c6,c7,c8,c9,c10), \
	MY_CHILDREN_SUM=COMBINE_MASKS5(+,c1,c2,c3,c4,c5)+COMBINE_MASKS5(+,c6,c7,c8,c9,c10), \
	CHILDREN_MASK=MY_CHILDREN_MASK|BaseClass::CHILDREN_MASK, \
	CHILDREN_SUM=MY_CHILDREN_SUM+BaseClass::CHILDREN_SUM }

#endif
