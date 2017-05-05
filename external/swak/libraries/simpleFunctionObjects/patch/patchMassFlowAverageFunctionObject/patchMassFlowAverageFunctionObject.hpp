/*---------------------------------------------------------------------------*\
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
    patchMassFlowAverageFunctionObject

Description
    Mass-Flow-Weighted Averaging the fields on the given patches

SourceFiles
    patchMassFlowAverageFunctionObject.cpp

Contributors/Copyright:
    2010 Oliver Borm (oli.borm@web.de)
    2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef patchMassFlowAverageFunctionObject_H
#define patchMassFlowAverageFunctionObject_H

#include "patchFieldFunctionObject.hpp"
#include "SolverInfo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class patchMassFlowAverageFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class patchMassFlowAverageFunctionObject
:
    public patchFieldFunctionObject
{

    // Private Member Functions

    //- Disallow default bitwise copy construct
    patchMassFlowAverageFunctionObject(const patchMassFlowAverageFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const patchMassFlowAverageFunctionObject&);

    //- integrate single field on all patches
    template <class T>
    Field<T> average(const word& fieldName,T preset) const;

    SolverInfo solver_;

protected:

    word dirName();

    virtual scalarField process(const word& fieldName,scalar preset);
    virtual Field<vector> process(const word& fieldName,vector preset);
    virtual Field<sphericalTensor> process(const word& fieldName,sphericalTensor preset);
    virtual Field<symmTensor> process(const word& fieldName,symmTensor preset);
    virtual Field<tensor> process(const word& fieldName,tensor preset);

public:

    //- Runtime type information
    TypeName("patchMassFlowAverage");


    // Constructors

    //- Construct from components
    patchMassFlowAverageFunctionObject
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

template <class T>
Field<T> patchMassFlowAverageFunctionObject::average(const word& fieldName,T unsetVal) const
{
    const GeometricField<T, fvPatchField, volMesh>& fld =
        obr_.lookupObject<GeometricField<T, fvPatchField, volMesh> >
        (
            fieldName
        );

    Field<T> vals(patchNames_.size(), unsetVal);

    const surfaceScalarField &phi=obr_.lookupObject<surfaceScalarField>(solver_.phi());

    forAll(patchNames_, patchI)
    {
        if (patchIndizes_[patchI] >= 0)
        {
            label index=patchIndizes_[patchI];
            scalar flux=sum(phi.boundaryField()[index]);
            reduce(flux,sumOp<scalar>());
            vals[patchI] = sum
                        (
                            phi.boundaryField()[index]
                            *fld.boundaryField()[index]
                        );
            reduce(vals[patchI],sumOp<T>());
            vals[patchI]/=flux;
        }
    }

    if(verbose()) {
        Info<< regionString()
            << " Mass-Flow-Weighted Averages of " << fieldName << " :";

        forAll(patchNames_, patchI)
        {
            Info << "  " << patchNames_[patchI] << " = "
                << vals[patchI];
        }

        Info << endl;
    }

    //    Pstream::listCombineGather(vals, isNotEqOp<T>());
    //    Pstream::listCombineScatter(vals);

    return vals;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
