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

Class
    patchFieldDistributionFunctionObject

Description
    Distribution of a field on a patch

SourceFiles
    patchFieldDistributionFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef patchFieldDistributionFunctionObject_H
#define patchFieldDistributionFunctionObject_H

#include "fieldDistributionFunctionObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class patchFieldDistributionFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class patchFieldDistributionFunctionObject
:
    public fieldDistributionFunctionObject
{

    // Private Member Functions

    //- Disallow default bitwise copy construct
    patchFieldDistributionFunctionObject(const patchFieldDistributionFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const patchFieldDistributionFunctionObject&);

    wordList patchNames_;

    labelList patchIDs_;

    template <typename T>
    void getDistributionInternal(autoPtr<SimpleDistribution<T> > &dist);
protected:

    word dirName();

    virtual word baseName();

    virtual void getDistribution();

public:

    //- Runtime type information
    TypeName("patchFieldDistribution");


    // Constructors

    //- Construct from components
    patchFieldDistributionFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "fieldDistributionFunctionObject.hpp"
#include "volFields.hpp"
#include "IOmanip.hpp"
#include "fvMesh.hpp"
#include "fvCFD.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <typename T>
void patchFieldDistributionFunctionObject::getDistributionInternal(
    autoPtr<SimpleDistribution<T> > &dist
) {
    const fvMesh &mesh=refCast<const fvMesh>(obr_);

    typedef GeometricField<T,fvPatchField,volMesh> volTField;
    typedef GeometricField<T,fvsPatchField,surfaceMesh> surfaceTField;
    typedef GeometricField<T,pointPatchField,pointMesh> pointTField;

    bool firstTime=true;

    forAll(patchIDs_,i) {
        label patchID=patchIDs_[i];
        autoPtr<SimpleDistribution<T> > partial;

        if(mesh.foundObject<volTField>(fieldName())) {
            partial=setDataScalar(
                mesh.lookupObject<volTField>(
                    fieldName()
                ).boundaryField()[patchID],
                mesh.boundary()[patchID].magSf()
            );
        }
        if(mesh.foundObject<surfaceTField>(fieldName())) {
            partial=setDataScalar(
                mesh.lookupObject<surfaceTField>(
                    fieldName()
                ).boundaryField()[patchID],
                mesh.boundary()[patchID].magSf()
            );
        }
        if(mesh.foundObject<pointTField>(fieldName())) {
            partial=setDataScalar(
                mesh.lookupObject<pointTField>(
                    fieldName()
                ).boundaryField()[patchID].patchInternalField()(),
                scalarField(
                    mesh.lookupObject<pointTField>(
                        fieldName()
                    ).boundaryField()[patchID].size(),
                    1
                )
            );
        }

        if(partial.valid()) {
            if(firstTime) {
                firstTime=false;
                dist=partial;
            } else {
                SimpleDistribution<T> &d=dist();
                d=d+partial();
            }
        }
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
