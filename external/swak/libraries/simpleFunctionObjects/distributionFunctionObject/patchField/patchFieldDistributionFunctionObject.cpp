/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "patchFieldDistributionFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "pointFields.hpp"

#include "wordReList.hpp"
#include "HashSet.hpp"
#include "stringListOps.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(patchFieldDistributionFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        patchFieldDistributionFunctionObject,
        dictionary
    );



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

patchFieldDistributionFunctionObject::patchFieldDistributionFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    fieldDistributionFunctionObject(name,t,dict)
{
    const fvMesh &mesh=refCast<const fvMesh>(obr_);

    wordReList newPatches(dict.lookup("patchNames"));
    HashSet<word> patchNamesNew;
    wordList allPatches(mesh.boundaryMesh().names());
    forAll(newPatches,i) {
        labelList IDs=findStrings(newPatches[i],allPatches);
        forAll(IDs,j) {
            patchNamesNew.insert(allPatches[IDs[j]]);
        }
    }

    patchNames_=patchNamesNew.toc();
    patchIDs_.setSize(patchNames_.size());

    forAll(patchNames_,i) {
        const word &name=patchNames_[i];
        patchIDs_[i]=mesh.boundaryMesh().findPatchID(name);
        if(patchIDs_[i]<0) {
            FatalErrorInFunction
                << "Patch name " << name << " is not a valid patch"
                    << endl
                    << exit(FatalError);
        }
    }
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

word patchFieldDistributionFunctionObject::dirName()
{
    word result(typeName+"_"+fieldName());
    forAll(patchNames_,i) {
	result+="_"+patchNames_[i];
    }

    return result;
}

word patchFieldDistributionFunctionObject::baseName()
{
    word result(fieldName());
    forAll(patchNames_,i) {
	result+="_"+patchNames_[i];
    }

    return result;
}

void patchFieldDistributionFunctionObject::getDistribution()
{
    getDistributionInternal(distScalar_);
    getDistributionInternal(distVector_);
    getDistributionInternal(distTensor_);
    getDistributionInternal(distSymmTensor_);
    getDistributionInternal(distSphericalTensor_);
}


} // namespace CML

// ************************************************************************* //
