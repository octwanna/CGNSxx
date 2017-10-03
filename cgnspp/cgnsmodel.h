/***************************************************************************
                          cgnsmodel.h  -  description
                             -------------------
    begin                : Mon May 27 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSMODEL_H
#define CGNSMODEL_H

#include "cgnsvalued.h"

#include "cgnsequationset.h"

/** Implementation data for GoverningEquations_t subnode
  */
struct CGNS::Private::GoverningEquations : CGNS::Private::Parented  {
COMMON_NODE_INTERFACE(GoverningEquations, FlowEquationSet, Parented);
	CGNS::GoverningEquations::GoverningEquationsType mType;
	std::auto_ptr<DiffusionModel_t> mDiffusion;

	Zone const * getZone() const { return P() ? P()->getZone() : 0; }
		
	USING_READ(Node);
	static void Read(GoverningEquations * me);
	static void Read(GoverningEquations * me, std::auto_ptr<DiffusionModel_t> & cDest);

	void Modify(CGNS::GoverningEquations::GoverningEquationsType iType);
DISTINCT_CHILDREN0();
};

/** Special subnode containing gas model
  */
struct CGNS::Private::GasModel : CGNS::Private::Valued {
COMMON_NODE_INTERFACE(GasModel, FlowEquationSet, Valued);
	CGNS::GasModel::GasModelType mType;
	USING_READ(Valued);
	static void Read(GasModel * me);
	
	void Modify(CGNS::GasModel::GasModelType iType);
DISTINCT_CHILDREN0();
};

/** Special subnode containing viscosity model
  */
struct CGNS::Private::ViscosityModel : CGNS::Private::Valued {
COMMON_NODE_INTERFACE(ViscosityModel, FlowEquationSet, Valued);
	CGNS::ViscosityModel::ViscosityModelType mType;
	USING_READ(Valued);
	static void Read(ViscosityModel * me);
	
	void Modify(CGNS::ViscosityModel::ViscosityModelType iType);
DISTINCT_CHILDREN0();
};

/** Special subnode containing thermal conductivity model
  */
struct CGNS::Private::ThermalConductivityModel : CGNS::Private::Valued {
COMMON_NODE_INTERFACE(ThermalConductivityModel, FlowEquationSet, Valued);
	CGNS::ThermalConductivityModel::ThermalConductivityModelType mType;
	USING_READ(Valued);
	static void Read(ThermalConductivityModel * me);
	
	void Modify(CGNS::ThermalConductivityModel::ThermalConductivityModelType iType);
DISTINCT_CHILDREN0();
};

/** Special subnode containing turbulence closure
  */
struct CGNS::Private::TurbulenceClosure : CGNS::Private::Valued {
COMMON_NODE_INTERFACE(TurbulenceClosure, FlowEquationSet, Valued);
	CGNS::TurbulenceClosure::TurbulenceClosureType mType;
	USING_READ(Valued);
	static void Read(TurbulenceClosure * me);
	
	void Modify(CGNS::TurbulenceClosure::TurbulenceClosureType iType);
DISTINCT_CHILDREN0();
};

/** Special subnode containing turbulence model
  */
struct CGNS::Private::TurbulenceModel : CGNS::Private::Valued {
COMMON_NODE_INTERFACE(TurbulenceModel, FlowEquationSet, Valued);
	CGNS::TurbulenceModel::TurbulenceModelType mType;
	std::auto_ptr<DiffusionModel_t> mDiffusion;

	Zone const * getZone() const { return P() ? P()->getZone() : 0; }
	
	USING_READ(Valued);
	static void Read(TurbulenceModel * me);
	static void Read(TurbulenceModel * me, std::auto_ptr<DiffusionModel_t> & cDest);
	
	void Modify(CGNS::TurbulenceModel::TurbulenceModelType iType);
DISTINCT_CHILDREN0();
};

/** Special subnode containing thermal relaxation model
  */
struct CGNS::Private::ThermalRelaxationModel : CGNS::Private::Valued {
COMMON_NODE_INTERFACE(ThermalRelaxationModel, FlowEquationSet, Valued);
	CGNS::ThermalRelaxationModel::ThermalRelaxationModelType mType;
	USING_READ(Valued);
	static void Read(ThermalRelaxationModel * me);
	
	void Modify(CGNS::ThermalRelaxationModel::ThermalRelaxationModelType iType);
DISTINCT_CHILDREN0();
};

/** Special subnode containing chemical kinetics model
  */
struct CGNS::Private::ChemicalKineticsModel : CGNS::Private::Valued {
COMMON_NODE_INTERFACE(ChemicalKineticsModel, FlowEquationSet, Valued);
	CGNS::ChemicalKineticsModel::ChemicalKineticsModelType mType;
	USING_READ(Valued);
	static void Read(ChemicalKineticsModel * me);
	
	void Modify(CGNS::ChemicalKineticsModel::ChemicalKineticsModelType iType);
DISTINCT_CHILDREN0();
};



#endif
