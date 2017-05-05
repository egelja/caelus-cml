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
    fieldDistributionFunctionObject

Description
    Distribution of a field on the interior

SourceFiles
    fieldDistributionFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef fieldDistributionFunctionObject_H
#define fieldDistributionFunctionObject_H

#include "distributionFunctionObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class fieldDistributionFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class fieldDistributionFunctionObject
:
    public distributionFunctionObject
{

    // Private Member Functions

    //- Disallow default bitwise copy construct
    fieldDistributionFunctionObject(const fieldDistributionFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const fieldDistributionFunctionObject&);

    word fieldName_;

    template <typename T>
    void getDistributionInternal(autoPtr<SimpleDistribution<T> > &dist);
protected:

    word dirName();

    virtual word baseName();

    virtual void getDistribution();

    const word &fieldName() const { return fieldName_; }

public:

    //- Runtime type information
    TypeName("fieldDistribution");


    // Constructors

    //- Construct from components
    fieldDistributionFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "volFields.hpp"
#include "IOmanip.hpp"
#include "fvMesh.hpp"
#include "fvCFD.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <typename T>
void fieldDistributionFunctionObject::getDistributionInternal(
    autoPtr<SimpleDistribution<T> > &dist
) {
    const fvMesh &mesh=refCast<const fvMesh>(obr_);

    typedef GeometricField<T,fvPatchField,volMesh> volTField;
    typedef GeometricField<T,fvsPatchField,surfaceMesh> surfaceTField;
    typedef GeometricField<T,pointPatchField,pointMesh> pointTField;

    if(mesh.foundObject<volTField>(fieldName_)) {
        dist=setDataScalar(
            mesh.lookupObject<volTField>(
                fieldName_
            ).internalField(),
            mesh.V()
        );
        return;
    }
    if(mesh.foundObject<surfaceTField>(fieldName_)) {
        dist=setDataScalar(
            mesh.lookupObject<surfaceTField>(
                fieldName_
            ).internalField(),
            mesh.magSf()
        );
        return;
    }
    if(mesh.foundObject<pointTField>(fieldName_)) {
        dist=setDataScalar(
            mesh.lookupObject<pointTField>(
                fieldName_
            ).internalField(),
            scalarField(mesh.nPoints(),1)
        );
        return;
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
