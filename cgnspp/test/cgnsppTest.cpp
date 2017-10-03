/***************************************************************************
                          cgnsppTest.cpp  -  description
                             -------------------
    begin                : Mon Mär 12 15:39:20 CET 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "adf++.h"
#include "cgns++.h"

#include "test.h"

#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>

using std::ostream;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;

TempFile::TempFile() : mName(tmpnam(0)) {}
TempFile::~TempFile() { remove(mName.c_str()); }

template<typename T>
inline bool isLast(T iter, T const & end) {
	return ++iter==end;
}

void CreateTest(TempFile & iFile, int iNum) {
	switch (iNum) {
		case 1: CreateTest1(iFile); break;
		case 2: CreateTest2(iFile); break;
		case 3: CreateTest3(iFile); break;
		case 4: CreateTest4(iFile); break;
		case 5: CreateTest5(iFile); break;
/*		case 6: CreateTest6(iFile); break;
*/		default: throw "Test number does not exist";
	}
}

// special case since unsigned char is to be print as integer
template<typename T>
T toPrintable(T t) { return t; }
// therefore convert it
int toPrintable(unsigned char t) { return t; }

template<typename T>
void PrintData(ostream & out, ADF::Node const & iNode, int iMaxData) {
	int dim=iNode.getDimension(0);
	if (dim>1) out << '{';
	T * data=new T[dim];
	iNode.readData(data);
	out << toPrintable(data[0]);
	for (int i=1; i<dim; ++i) {
		if (i>=iMaxData && i!=dim-1) {
			out << ",...";
			break;
		}
		out << ',' << toPrintable(data[i]);
	}
	if (dim>1) out << '}';
	delete[] data;
}

template<>
void PrintData<char>(ostream & out, ADF::Node const & iNode, int iMaxData) {
	int dim=iNode.getDimension(0);
	char * data=new char[dim+1];
	iNode.readData(data);
	data[dim]=0;
	bool print=true;
	for (int i=0; i<dim; ++i) if (!isprint(data[i])) print=false;
	if (print) out << '"' << data << '"';
	else out << "(non-printable)";
	delete[] data;
}

void PrintNode(ostream & out, ADF::Node const & iNode, string const & iFrontString) {
	if (iFrontString.size()>1)
		out << iFrontString.substr(0, iFrontString.size()-2) << "+-";
	out << iNode.getName() << '{' << iNode.getLabel() << '}'
		<< ':' << ADF::asString(iNode.getDataType());
	if (iNode.getDataType()!=ADF::MT) {
		int numDim=iNode.getNumDimensions();
		out << '[';
		if (numDim>0) {
			out << iNode.getDimension(0);
			for (int d=1; d<numDim; ++d)
				out << ',' << iNode.getDimension(d);
		}
		out << ']';
		// try and print some data, if not too much
		if (numDim==1) {
			int dim=iNode.getDimension(0);
			if (dim>0) {
				out << '=';
				// it's a string
				switch (iNode.getDataType()) {
					case ADF::I8:
#ifdef OUTPUT_LONG_LONG
						PrintData<long long>(cout, iNode, 6); break;
#endif					
					case ADF::I4: PrintData<int>(cout, iNode, 6); break;
					case ADF::U8:
#ifdef OUTPUT_LONG_LONG
						PrintData<unsigned long long>(cout, iNode, 6); break;
#endif
					case ADF::U4: PrintData<unsigned>(cout, iNode, 6); break;
					case ADF::R4: PrintData<float>(cout, iNode, 6); break;
					case ADF::R8: PrintData<double>(cout, iNode, 6); break;
					case ADF::X4: PrintData<std::complex<float> >(cout, iNode, 6); break;
					case ADF::X8: PrintData<std::complex<double> >(cout, iNode, 6); break;
					case ADF::C1: PrintData<char>(cout, iNode, 6); break;
					case ADF::B1: PrintData<unsigned char>(cout, iNode, 6); break;
					default: ;// do nothing
				}
			}
		}
	}
	out << endl;
	string front=iFrontString+"| ";
	
//	int numChildren=iNode.GetNumChildren();
	for (ADF::Node::Child_iterator_t i=iNode.beginChild(); i!=iNode.endChild(); ++i)
		PrintNode(out, *i, isLast(i, iNode.endChild()) ? iFrontString+"  " : front);
}



#define PRINT_NODE(name) \
	for (CGNS::Node::Descriptor_iterator_t i=name.beginDescriptor(); \
		i!=name.endDescriptor(); ++i) \
		out << iFrontString << "+-" << i->getName() << ": " << i->getContent().substr(0,60) << std::endl; \
	if (name.hasOrdinal()) \
		out << iFrontString << "+-Ordinal: " << name.getOrdinal() << endl;
	
#define PRINT_SELF(name) \
	if (iFrontString.size()>1) \
		out << iFrontString.substr(0, iFrontString.size()-2) << "+-"; \
	out << name.getName() << '{' << name.getLabel() << '}';
#define PRINT_SELF0(name) \
	PRINT_SELF(name) \
	out << std::endl; \
	string front=iFrontString+"| "; \
	PRINT_NODE(name)
#define PRINT_SELF1(name, p) \
	PRINT_SELF(name) \
	out << ':' << p << std::endl; \
	string front=iFrontString+"| "; \
	PRINT_NODE(name)
#define PRINT_SELF2(name, p1, p2) \
	PRINT_SELF(name) \
	out << ':' << p1 << '/' << p2 << std::endl; \
	string front=iFrontString+"| "; \
	PRINT_NODE(name)

#define PRINT_STRING(name, child) \
	if (name.has##child()) \
		out << iFrontString << "+-" << #child ": " << name.get##child() << endl;

#define PRINT_ASSTRING(name, child) \
	if (name.has##child()) \
		out << iFrontString << "+-" << #child ": " << CGNS::asString(name.get##child()) << endl;

#define PRINT_OPTIONAL_MODEL(name, child) \
	if (name.has##child()) { \
		out << iFrontString << "+-" << #child ": " << CGNS::asString(name.get##child().getType()) << endl; \
		PrintValued(out, name.get##child(), front); \
	}

#define PRINT_OPTIONAL(name, child) \
	if (name.has##child()) \
		Print##child(out, name.get##child(), front);

#define PRINT_LOCAL_OPTIONAL(name, child) \
	if (name.hasLocal##child()) \
		Print##child(out, name.get##child(), front);

#define PRINT_MANY(name, child, type) \
	for (CGNS::type::child##_iterator_t i=name.begin##child(); i!=name.end##child(); ++i) \
		Print##child(out, *i, front);
	
#define PRINT_DIMENSIONED(name) \
	if (name.hasLocalDataClass()) \
		out << iFrontString << "+-DataClass: " << CGNS::asString(name.getDataClass()) << endl; \
	PRINT_LOCAL_OPTIONAL(name, DimensionalUnits)
	
#define PRINT_STATED(name) \
	PRINT_LOCAL_OPTIONAL(name, ReferenceState) \
	PRINT_DIMENSIONED(name)
	
#define PRINT_RINDED(name) \
	if (name.hasRind()) \
		out << iFrontString << "+-Rind{Rind_t}\n";

void PrintDimensionalUnits(ostream & out, CGNS::DimensionalUnits const & iUnits,
	string const & iFrontString) {
	out << 	iFrontString.substr(0, iFrontString.size()-2) << "+-"
		<< "DimensionalUnits: " << CGNS::asString(iUnits.mMass) << ", "
		<< CGNS::asString(iUnits.mLength) << ", " << CGNS::asString(iUnits.mTime) << ", "
		<< CGNS::asString(iUnits.mTemperature) << ", " << CGNS::asString(iUnits.mAngle) << endl;
}

void PrintDataConversion(ostream & out, CGNS::DataArray::DataConversion const & iConv,
	string const & iFrontString) {
	out << iFrontString.substr(0, iFrontString.size()-2) << "+-DataConversion{DataConversion_t}: "
		<< iConv.mScale << " * x + " << iConv.mOffset << std::endl;
}

void PrintDimensionalExponents(ostream & out, CGNS::DataArray::DimensionalExponents const & iExp,
	string const & iFrontString) {
	out << iFrontString.substr(0, iFrontString.size()-2)
		<< "+-DimensionalExponents{DimensionalExponents_t}: "
		<< "m: " << iExp.mMass << ", l:" << iExp.mLength << ", t: " << iExp.mTime
		<< ", T: " << iExp.mTemperature << ", °: " << iExp.mAngle << endl;
}

void PrintDataArray(ostream & out, CGNS::DataArray const & iData,
	string const & iFrontString) {
	PRINT_SELF(iData)
	if (iData.getNumDimensions()==1 && iData
		.getDimension(0)==1) {
		out << " ";
		switch (iData.getDataType()) {
		case CGNS::DataArray::FLOAT: {
			float data; iData.readData(&data); out << data; break; }
		case CGNS::DataArray::DOUBLE: {
			double data; iData.readData(&data); out << data; break; }
		case CGNS::DataArray::INT: {
			int data; iData.readData(&data); out << data; break; }
		default: ;
		}
	}
	out << endl;
	string front=iFrontString+"| ";
	PRINT_DIMENSIONED(iData)
	PRINT_OPTIONAL(iData, DataConversion);
	PRINT_OPTIONAL(iData, DimensionalExponents);
}

void PrintCoordinates(ostream & out, CGNS::GridCoordinates const & iCoor,
	string const & iFrontString) {
	PRINT_SELF0(iCoor)
	PRINT_DIMENSIONED(iCoor)
#define PrintCoordinate PrintDataArray
	PRINT_MANY(iCoor, Coordinate, GridCoordinates)
#undef PrintCoordinate
	PRINT_RINDED(iCoor)
}

void PrintArrayed(ostream & out, CGNS::Arrayed const & iArrayed,
	string const & iFrontString) {
	PRINT_SELF0(iArrayed)
	PRINT_DIMENSIONED(iArrayed)
	PRINT_MANY(iArrayed, DataArray, Arrayed)
}

void PrintValues(ostream & out, CGNS::Valued const & iValued,
	string const & iFrontString) {
	for (CGNS::Valued::Value_iterator_t i=iValued.beginValue(); i!=iValued.endValue(); ++i)
		out << iFrontString << "+-" << i.getName() << "{DataArray_t}: " << *i << std::endl;
}

void PrintValued(ostream & out, CGNS::Valued const & iValued,
	string const & iFrontString) {
	PRINT_SELF0(iValued)
	PRINT_DIMENSIONED(iValued)
	PrintValues(out, iValued, iFrontString);
}

void PrintSolution(ostream & out, CGNS::FlowSolution const & iSolution, string const & iFrontString) {
	PRINT_SELF(iSolution)
//		<< ':' << iNode->GetDataTypeString();
	out << "(@" << CGNS::asString(iSolution.getLocation()) << ")\n";
	string front=iFrontString+"| ";
	PRINT_NODE(iSolution)
	
	PRINT_DIMENSIONED(iSolution)
#define PrintField PrintDataArray	
	PRINT_MANY(iSolution, Field, FlowSolution)			
#undef PrintField
	PRINT_RINDED(iSolution)
}

void PrintConvergenceHistory(ostream & out, CGNS::ConvergenceHistory const & iHistory,
	string const & iFrontString) {
	PRINT_SELF1(iHistory, iHistory.getIterations())
}

void PrintIntegralData(ostream & out, CGNS::IntegralData const & iData,
	string const & iFrontString) {
	PRINT_SELF0(iData)
	PRINT_DIMENSIONED(iData)
	PrintValues(out, iData, iFrontString);
}

void PrintReferenceState(ostream & out, CGNS::ReferenceState const & iState,
	string const & iFrontString) {
	PRINT_SELF0(iState)
	PRINT_DIMENSIONED(iState)
	PrintValues(out, iState, iFrontString);
}

void PrintElements(ostream & out, CGNS::Elements const & iElements, string const & iFrontString) {
	PRINT_SELF2(iElements, CGNS::asString(iElements.getElementType()), iElements.getNodesOnBoundary());
}

void PrintRange(ostream & out, CGNS::IndexRange const & iRange, string const & iName,
	string const & iFrontString) {
	out << iFrontString.substr(0, iFrontString.size()-2) << "+-"
		<< iName << "{IndexRange_t}\n";
}

void PrintIndexArray(ostream & out, CGNS::IndexArray const & iIndex,
	string const & iFrontString) {
	PRINT_SELF(iIndex)
	out << ": " << iIndex.getLength() << endl;
}

void PrintBCData(ostream & out, CGNS::BCData const & iBCData, string const & iFrontString) {
	PRINT_SELF0(iBCData)
	PRINT_DIMENSIONED(iBCData)
	PrintValues(out, iBCData, iFrontString);
}

void PrintBCDataSet(ostream & out, CGNS::BCDataSet const & iBCDataSet, string const & iFrontString) {
	PRINT_SELF(iBCDataSet)
	out << ": " << CGNS::asString(iBCDataSet.getBCType()) << endl;
	string front=iFrontString+"| ";
	PRINT_STATED(iBCDataSet)
#define PrintDirichletData PrintBCData	
	PRINT_OPTIONAL(iBCDataSet, DirichletData)
#define PrintNeumannData PrintBCData	
	PRINT_OPTIONAL(iBCDataSet, NeumannData)
}

#define PrintPointRange PrintRange
#define PrintPointList PrintIndexArray	
#define PrintInwardNormalList PrintIndexArray	
void PrintBC(ostream & out, CGNS::BC const & iBC, string const & iFrontString) {
	PRINT_SELF(iBC)
	out << "(@" << CGNS::asString(iBC.getLocation()) << "): " << CGNS::asString(iBC.getBCType()) << endl;
	string front=iFrontString+"| ";
	PRINT_NODE(iBC)
	PRINT_STATED(iBC)
	if (iBC.hasPointRange()) PrintRange(out, iBC.getPointRange(), "PointRange", front);
	PRINT_OPTIONAL(iBC, PointList)
	PRINT_OPTIONAL(iBC, InwardNormalList)
	PRINT_STRING(iBC, FamilyName)
	PRINT_MANY(iBC, BCDataSet, BC)
}

void PrintZoneBC(ostream & out, CGNS::ZoneBC const & iZoneBC, string const & iFrontString) {
	PRINT_SELF0(iZoneBC)
	PRINT_STATED(iZoneBC)
	PRINT_MANY(iZoneBC, BC, ZoneBC)
}

// GoverningEquations isn't really Valued, so skip it, but instead take DiffusionModel
void PrintValued(std::ostream & out, CGNS::GoverningEquations const & iGovEq,
	string const & iFrontString) {
	if (iGovEq.hasDiffusionModel())
		out << iFrontString.substr(0, iFrontString.size()-2) << "+-DiffusionModel{int[]}\n";
}

// TurbulenceModel is really Valued, but take DiffusionModel additionaly
void PrintValued(std::ostream & out, CGNS::TurbulenceModel const & iTurbulence,
	string const & iFrontString) {
	PrintValued(out, static_cast<CGNS::Valued const &>(iTurbulence), iFrontString);
	if (iTurbulence.hasDiffusionModel())
		out << iFrontString.substr(0, iFrontString.size()-2) << "+-DiffusionModel{int[]}\n";
}

void PrintFlowEquationSet(ostream & out, CGNS::FlowEquationSet const & iEqSet,
	string const & iFrontString) {
	PRINT_SELF0(iEqSet)
	PRINT_OPTIONAL_MODEL(iEqSet, GoverningEquations)
	if (iEqSet.hasEquationDimension())
		out << iFrontString.substr(0, iFrontString.size()-2) << "+-EquationDimension{int}: " <<
			iEqSet.getEquationDimension() << std::endl;
	
	PRINT_OPTIONAL_MODEL(iEqSet, GasModel)
	PRINT_OPTIONAL_MODEL(iEqSet, ViscosityModel)
	PRINT_OPTIONAL_MODEL(iEqSet, ThermalConductivityModel)
	PRINT_OPTIONAL_MODEL(iEqSet, TurbulenceClosure)
	PRINT_OPTIONAL_MODEL(iEqSet, TurbulenceModel)
}

void PrintTransform(ostream & out, CGNS::Transform const & iTransform, string const & iFrontString) {
	out << iFrontString.substr(0, iFrontString.size()-2) << "+-Transform{int[]}\n";
}

void PrintGridConnectivity1to1(ostream & out, CGNS::GridConnectivity1to1 const & iGC,
	string const & iFrontString) {
	PRINT_SELF0(iGC)
	out << front << "ZoneDonorName: " << iGC.getZoneDonorName() << endl;
	PrintRange(out, iGC.getPointRange(), "PointRange", front);
	PrintRange(out, iGC.getPointRangeDonor(), "PointRangeDonor", front);
	PRINT_OPTIONAL(iGC, Transform)
}

#define PrintPointListDonor PrintIndexArray	
#define PrintCellListDonor PrintIndexArray
#define PrintInterpolantsDonor PrintDataArray
void PrintGridConnectivity(ostream & out, CGNS::GridConnectivity const & iGC,
	string const & iFrontString) {
	PRINT_SELF(iGC)
	out << "(@" << CGNS::asString(iGC.getLocation()) << ")\n";
	string front=iFrontString+"| ";
	out << front << "ZoneDonorName: " << iGC.getZoneDonorName() << endl;
	PRINT_NODE(iGC)
	if (iGC.hasPointRange()) PrintRange(out, iGC.getPointRange(), "PointRange", front);
	PRINT_OPTIONAL(iGC, PointList)
	PRINT_OPTIONAL(iGC, PointListDonor)
	PRINT_OPTIONAL(iGC, CellListDonor)
	out << front << "GridConnectivityType: " << CGNS::asString(iGC.getGridConnectivityType()) << endl;
	PRINT_OPTIONAL(iGC, InterpolantsDonor);
}

void PrintOversetHoles(ostream & out, CGNS::OversetHoles const & iHole,
	string const & iFrontString) {
	PRINT_SELF(iHole)
	out << "(@" << CGNS::asString(iHole.getLocation()) << ")\n";
	string front=iFrontString+"| ";
	PRINT_NODE(iHole)
	for (CGNS::OversetHoles::PointRange_iterator_t i=iHole.beginPointRange();
		i!=iHole.endPointRange(); ++i)
		PrintRange(out, *i, i.getName(), front);
	PRINT_OPTIONAL(iHole, PointList)
}

void PrintZoneGridConnectivity(ostream & out, CGNS::ZoneGridConnectivity const & iZGC,
	string const & iFrontString) {
	PRINT_SELF0(iZGC)
	PRINT_MANY(iZGC, GridConnectivity1to1, ZoneGridConnectivity)
	PRINT_MANY(iZGC, GridConnectivity, ZoneGridConnectivity)
	PRINT_MANY(iZGC, OversetHoles, ZoneGridConnectivity)
}

void PrintZoneIterativeData(ostream & out, CGNS::ZoneIterativeData const & iData,
	string const & iFrontString) {
	PRINT_SELF0(iData)
	PrintArrayed(out, iData, front);
}

void PrintRigidGridMotion(ostream & out, CGNS::RigidGridMotion const & iMotion,
	string const & iFrontString) {
	PRINT_SELF0(iMotion)
	PrintArrayed(out, iMotion, front);
}

void PrintArbitraryGridMotion(ostream & out, CGNS::ArbitraryGridMotion const & iMotion,
	string const & iFrontString) {
	PRINT_SELF(iMotion)
	out << "(@" << CGNS::asString(iMotion.getLocation()) << ")\n";
	string front=iFrontString+"| ";
	PrintArrayed(out, iMotion, front);
	PRINT_RINDED(iMotion)
}

void PrintZone(ostream & out, CGNS::Zone const & iZone, string const & iFrontString) {
	PRINT_SELF2(iZone, iZone.getIndexDimension(), (iZone.isStructured() ? "structured" : "unstructured") )
	PRINT_STATED(iZone)
	PRINT_OPTIONAL(iZone, ConvergenceHistory)
	PRINT_OPTIONAL(iZone, ZoneIterativeData)
	PRINT_MANY(iZone, IntegralData, Zone)
	PRINT_OPTIONAL(iZone, FlowEquationSet)
	PRINT_MANY(iZone, Elements, Zone)
	PRINT_STRING(iZone, FamilyName)
	PRINT_OPTIONAL(iZone, ZoneBC)
	PRINT_OPTIONAL(iZone, ZoneGridConnectivity)
	PRINT_MANY(iZone, RigidGridMotion, Zone)
	PRINT_MANY(iZone, ArbitraryGridMotion, Zone)
		
	int numSols=iZone.getNumFlowSolution();
	
	if (iZone.hasGridCoordinates())
		PrintCoordinates(out, iZone.getGridCoordinates(),
			numSols>0 ? front : iFrontString+"  ");
	else cerr << "No grid coordinates\n" << flush;
	
	for (CGNS::Zone::FlowSolution_iterator_t i=iZone.beginFlowSolution(); i!=iZone.endFlowSolution(); ++i)
		PrintSolution(out, *i, isLast(i, iZone.endFlowSolution()) ? iFrontString+"  " : front);
}

void PrintGeometryReference(ostream & out, CGNS::GeometryReference const & iGeoRef,
	string const & iFrontString) {
	PRINT_SELF0(iGeoRef)
	out << iFrontString << "+-GeometryFile{GeometryFile_t}: "
		<< iGeoRef.getGeometryFile() << endl;
	out << iFrontString << "+-GeometryFormat{GeometryFormat_t}: "
		<< CGNS::asString(iGeoRef.getGeometryFormat()) << endl;
	for (CGNS::GeometryReference::GeometryEntity_iterator_t i=iGeoRef.beginGeometryEntity();
		i!=iGeoRef.endGeometryEntity(); ++i)
		out << iFrontString << "+-" << i.getName() << endl;
}

void PrintFamily(ostream & out, CGNS::Family const & iFamily,
	string const & iFrontString) {
	PRINT_SELF0(iFamily)
	PRINT_ASSTRING(iFamily, FamilyBC)
	PRINT_MANY(iFamily, GeometryReference, Family);
}

void PrintBaseIterativeData(ostream & out, CGNS::BaseIterativeData const & iData,
	string const & iFrontString) {
	PRINT_SELF1(iData, iData.getNumberOfSteps())
	PrintArrayed(out, iData, front);
	if (iData.hasTimeValues()) out << iFrontString << "+-TimeValues:\n";
	if (iData.hasIterationValues()) out << iFrontString << "+-IterationValues:\n";
	if (iData.hasNumberOfZones()) out << iFrontString << "+-NumberOfZones:\n";
	if (iData.hasZonePointers()) out << iFrontString << "+-ZonePointers:\n";
	if (iData.hasNumberOfFamilies()) out << iFrontString << "+-NumberOfFamilies:\n";
	if (iData.hasFamilyPointers()) out << iFrontString << "+-FamilyPointers:\n";
}

void PrintBase(ostream & out, CGNS::Base const & iBase, string const & iFrontString) {
	PRINT_SELF2(iBase, iBase.getCellDimension(), iBase.getPhysicalDimension())
	PRINT_DIMENSIONED(iBase)
	
	PRINT_LOCAL_OPTIONAL(iBase, ReferenceState)
	PRINT_OPTIONAL(iBase, ConvergenceHistory)
	PRINT_ASSTRING(iBase, SimulationType)
	PRINT_OPTIONAL(iBase, BaseIterativeData)
	PRINT_OPTIONAL(iBase, FlowEquationSet)
	
	PRINT_MANY(iBase, IntegralData, Base)
	PRINT_MANY(iBase, Family, Base)
	
	for (CGNS::Base::Zone_iterator_t i=iBase.beginZone(); i!=iBase.endZone(); ++i)
		PrintZone(out, *i, isLast(i, iBase.endZone()) ? iFrontString+"  " : front);
}

void HandleTest(int iNum) {		
	if (iNum<0) {
		for (int i=1; i<=5; ++i)
			HandleTest(i);
		return;
	}
	TempFile file;
	CreateTest(file, iNum);
	cerr << "Now we reexamine the just written CGNS file as an ADF file.\n";
	ADF::File adfFile(file.mName, ADF::File::READ_ONLY);
	cerr << "ADF Open seems to have been successfull.\n";
	cerr << "Next we try to traverse the ADF tree.\n";
	ADF::Node rootNode=adfFile.getRoot();
	PrintNode(cout, rootNode, "");
	cerr << "Finished. Ok!\n";
	cerr << "Deleting temporary file.\n";
}

int main(int argc, char *argv[]) {
	cerr << "Test program for libCGNS++\n";
	cerr << "This is the CGNS++ library, version "
		<< CGNS::File::getLibraryVersion() << endl;

	cerr << "Running " << ADF::File::getLibraryVersionString() << '('
		<< ADF::File::getLibraryVersionMajor() << ':'
		<< ADF::File::getLibraryVersionMinor() << ")\n";

// just for debugging
if (argc==1) { /*argc=1; argv[1]="test/tut21.cgns";*/ } else
if (argc==2 && isdigit(*argv[1]) && !argv[1][1]) { argc=3; } else
	if (argc!=2) {
  		cerr << "Exactly one parameter should be given, the CGNS file name.\n";
  		return 1;
	}
	
	try {
		if (argc==2) {
		{ // keep the two phases separate
		cerr << "First we try to open the given CGNS file (" << argv[1] << ") as an ADF file.\n";
		ADF::File adfFile(argv[1], ADF::File::READ_ONLY);
		cerr << "ADF Open seems to have been successfull.\n";
		cerr << "Written last by " << adfFile.getVersionString() << '('
			<< adfFile.getVersionMajor() << ':' << adfFile.getVersionMinor() << ")\n";
		cerr << "Next we try to traverse the ADF tree.\n";
		ADF::Node rootNode=adfFile.getRoot();
		PrintNode(cout, rootNode, "");
		cerr << "Finished. Ok!\n";
		}
		{
		cerr << "Next we try to open the given CGNS file (" << argv[1] << ") as a CGNS file proper.\n";
		CGNS::File cgnsFile(argv[1], CGNS::File::READ);
		cerr << "CGNS Open seems to have been successfull.\n";
		int numBases=cgnsFile.getNumBase();
		if (numBases>0) {
			cerr << "Next we try to traverse ";
			if (numBases>1) cerr << "each of the " << numBases << " bases.\n";
			else cerr << "the single base.\n";
			for (CGNS::File::Base_iterator_t i=cgnsFile.beginBase(); i!=cgnsFile.endBase(); ++i)
				PrintBase(cout, *i, "");
		}
		cerr << "Finished. Ok!\n";
		}
		} else {
			if (argc>1) HandleTest(atoi(argv[1]));
			else HandleTest(-1);
		}
	}
	catch (CGNS::Exception const & e) {
		cerr << "OOOPS, CGNS::Exception catched.\n"
			<< "\tError Code:    " << e.getErrorCode() << endl
			<< "\tError Message: " << e.getErrorMessage() << endl;
	}
	catch (ADF::Exception const & e) {
		cerr << "OOOPS, ADF::Exception catched.\n"
			<< "\tError Code:    " << e.getErrorCode() << endl
			<< "\tError Message: " << e.getErrorMessage() << endl;
	}
	catch (std::exception const & e) {
		cerr << "OOOPS, std::exception catched.\n"
			<< "\tWhat: " << e.what() << endl;
	}
	catch (char const * e) {
		cerr << "OOOPS, char * catched.\n"
			<< "\tContent: " << e << endl;
	}
	catch (...) {
		cerr << "OOOPS, unknown exception catched.\n";
	}
	
	return EXIT_SUCCESS;
}
