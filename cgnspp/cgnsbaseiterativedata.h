/***************************************************************************
                        cgnsbaseiterativedata.h  -  description
                             -------------------
    begin                : Fri Sep 28 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSBASEITERATIVEDATA_H
#define CGNSBASEITERATIVEDATA_H

#include "cgnsarrayed.h"

#include "cgnsbase.h"

/** Implementation data for BaseIterativeData subnode
  */
struct CGNS::Private::BaseIterativeData : CGNS::Private::Arrayed {
COMMON_NODE_INTERFACE(BaseIterativeData, Base, Arrayed);
	int mNumberOfSteps;
	
	refcnt_ptr<DataArray> mTimeValues;
	refcnt_ptr<DataArray> mIterationValues;
	refcnt_ptr<DataArray> mNumberOfZones;
	refcnt_ptr<DataArray> mNumberOfFamilies;
	refcnt_ptr<DataArray> mZonePointers;
	refcnt_ptr<DataArray> mFamilyPointers;
	
	USING_READ(BaseClass);
	static void Read(BaseIterativeData * me);
	static void Read(BaseIterativeData * me, refcnt_ptr<DataArray> &cDest);
	
	void Modify(int iNumberOfSteps);
	
private:
	template<typename T>
	void getValue(refcnt_ptr<DataArray> const & iArray, int iStep, T * oValue) const;
	template<typename T>
	int appendValue(refcnt_ptr<DataArray> & cArray, T iValue, std::string const & iName);
	template<typename T>
	T writeValue(refcnt_ptr<DataArray> & cArray, int iStep, T iValue, std::string const & iName);

	int getNumPointer(refcnt_ptr<DataArray> const & iArray, int iStep) const;
	void getPointer(refcnt_ptr<DataArray> const & iArray,
		int iStep, int iNum, std::string * oPointer) const;
	int appendPointer(refcnt_ptr<DataArray> & cArray, refcnt_ptr<DataArray> & cNumArray,
		int iNum, std::string const & iPointer,
		std::string const & iName, std::string const & iNumName);
	void writePointer(refcnt_ptr<DataArray> & cArray,
		int iStep, int iNum, std::string const & iPointer);
		
	int appendStep();	
	int appendStep(DataArray * iArray);	
DISTINCT_CHILDREN0();
friend class CGNS::BaseIterativeData;
};

#endif
