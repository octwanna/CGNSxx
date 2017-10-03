/***************************************************************************
                          cgnsfile.cpp  -  description
                             -------------------
    begin                : Mon Mar 12 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgns++.h"

#include "cgnsbroker.h"
#include "cgnsbase.h"
#include "cgnsutil.h"

#include <iostream>
#include <assert.h>

// undo macro trickery of cgnsutil.h
#ifdef D
#undef D
#endif
#define D() mImpl.get()


// possible MLL-versions
static const int nVersions = 6;
static const int cVersionList[nVersions] = {2100, 2000, 1270, 1200, 1100, 1050};

struct CGNS::Private::File {
	File(std::string const & iFileName, CGNS::File::OpenMode iMode);
	int mReferenceCount;
	ADF::File mFile;
	child_container<CGNS::Base> mBases;
	float mLibVersion;
	// needed for macro expansion
	template<typename P, typename T>
	static void Needs(P *, T &) {}
};
template struct CGNS::refcnt_ptr<CGNS::Private::File>;

using CGNS::File;


static ADF::File::Status toADFMode(File::OpenMode iMode) {
	switch (iMode) {
	case File::WRITE: return ADF::File::NEW;
	case File::READ: return ADF::File::READ_ONLY;
	case File::MODIFY: return ADF::File::OLD;
	}
	throw CGNS::Exception(CGNS::Exception::ILLEGAL_OPEN_MODE);
}

CGNS::Private::File::File(std::string const & iFileName,
	CGNS::File::OpenMode iMode)
	: mReferenceCount(0), mFile(iFileName, toADFMode(iMode)) {
	ADF::Node root = mFile.getRoot();

	if (iMode==CGNS::File::WRITE) {
		mLibVersion = 2.1; // set compatibility with MLL v2.1
		ADF::Node version=root.createChild("CGNSLibraryVersion", "CGNSLibraryVersion_t");
		version.setDataTypeDimensions(ADF::R4, 1);
		version.writeData(&mLibVersion);
	}
	else if (iMode==CGNS::File::READ || iMode==CGNS::File::MODIFY) {
    	int nNodes=root.getNumLabeledChildren("CGNSLibraryVersion_t");
		if (nNodes==0) mLibVersion = 1.05;
		else if (nNodes > 1)
			throw CGNS::Exception(CGNS::Exception::MULTIPLE_LIBRARY_VERSION_NODES);
		else {
			ADF::Node libVersionNode=root.getLabeledChild("CGNSLibraryVersion_t");
			// --- check ---
/*			
			if (libVersionNode.getDataType()!=ADF::R4) {
				std::cerr << std::endl << "ERROR: wrong data type [="
					<< ADF::DataType2String(libVersionNode.getDataType())
					<< "] for 'CGNSLibraryVersion_t'-node, expected R4" << std::endl;
				throw CGNS::Exception(CGNS::Exception::WRONG_DATATYPE);	
			}
			if (libVersionNode.getNumDimensions()!=1 ||
				libVersionNode.getDimension(0)!=1) {
				cerr << endl << "ERROR: wrong dimensions for 'CGNSLibraryVersion_t'-node, "
					<< "expected nDims = 1 and dims[0] = 1" << endl;
				throw CGNS::Exception(CGNS::Exception::WRONG_DIMENSION);		
			}
*/			checkNode(libVersionNode, ADF::R4, 1);
			/* To prevent round off error in version number */
			float tmpVersion = 0;
			libVersionNode.readData(&tmpVersion);
			tmpVersion*=1000;
			mLibVersion=0;
			for (int vers=0; vers<nVersions; vers++) {
				if (tmpVersion>(cVersionList[vers]-2)
					&& tmpVersion<(cVersionList[vers]+2)) {
					mLibVersion=cVersionList[vers]/1000;
					break;
				}
			}
      
			if (mLibVersion==0) {
				std::cerr << std::endl << "ERROR: could not determine the version number" << std::endl;
				throw CGNS::Exception(CGNS::Exception::UNKNOWN_VERSION);
			}
		}
	}
#ifdef CGNS_Log
	std::cerr << "File::File library version = " << mLibVersion << std::endl;
#endif
}
	

// look for a CGNSBase_t node, but only iLevelsLeft levels deep
template<typename C>
void searchForBase(ADF::Node const & iRoot, int iLevelsLeft, C & oBases) {
	for (ADF::Node::Child_iterator_t i=iRoot.beginChild(); i!=iRoot.endChild(); ++i) {
		ADF::Node & child=*i;
		if (child.getLabel()=="CGNSBase_t") {			
			std::auto_ptr<CGNS::Private::Base> base(new CGNS::Private::Base(child, 0));
			base->Init();
			oBases.insert(base.get());
			base.release();
		}
#if 0
		else if (iLevelsLeft!=0)
			searchForBase(child, iLevelsLeft-1, oBases);
#endif
	}
}


File::File(std::string const & iFileName, OpenMode iMode, SearchBaseDown iSearch)
	: mImpl(new Private::File(iFileName, iMode)) {
	// --- populate the base nodes ---
	searchForBase(mImpl->mFile.getRoot(), iSearch, mImpl->mBases);
}


File::~File() {}

double File::getLibraryVersion() {
	return 0.96;
}

// dummy to keep macro happy
namespace CGNS { namespace Private {
void needsChild(refcnt_ptr<File> const &, child_container<CGNS::Base> const &) {}
} }

N_CHILDS_IMPLEMENTATION(Base, Base, File, mImpl->mBases)


CGNS::Base File::writeBase(std::string const & iBaseName,
	int iCellDimension, int iPhysicalDimension) {
	CGNS::Private::Base * base=0;
	if (hasBase(iBaseName)) {
    base=static_cast<CGNS::Private::Base *>(CGNS::Private::Broker::GetImplementation(CGNS::Private::getChild(mImpl->mBases, iBaseName)));
		base->Wipe();
	}
	else {
		ADF::Node child=mImpl->mFile.getRoot().createChild(iBaseName, "CGNSBase_t");
		base=new CGNS::Private::Base(child, 0);
		mImpl->mBases.insert(base);
	}
	base->Modify(iCellDimension, iPhysicalDimension);
	return base;
}

double File::getFileVersion() const {
	return mImpl->mLibVersion;
}

#undef D
#define D() real_D(mImpl, this)
