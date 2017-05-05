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
    patchIntegrateFunctionObject

Description
    Integrates fields on the patches

SourceFiles
    patchIntegrateFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef patchIntegrateFunctionObject_H
#define patchIntegrateFunctionObject_H

#include "patchFieldFunctionObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class patchIntegrateFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class patchIntegrateFunctionObject
:
    public patchFieldFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    patchIntegrateFunctionObject(const patchIntegrateFunctionObject&);
    
    //- Disallow default bitwise assignment
    void operator=(const patchIntegrateFunctionObject&);

    //- integrate single field on all patches
    template <class T>
    Field<T> integrate(const word& fieldName,T preset) const;
    
protected:

    word dirName();

    virtual scalarField process(const word& fieldName,scalar preset);
    virtual Field<vector> process(const word& fieldName,vector preset);
    virtual Field<sphericalTensor> process(const word& fieldName,sphericalTensor preset);
    virtual Field<symmTensor> process(const word& fieldName,symmTensor preset);
    virtual Field<tensor> process(const word& fieldName,tensor preset);

public:

    //- Runtime type information
    TypeName("patchIntegrate");


    // Constructors

    //- Construct from components
    patchIntegrateFunctionObject
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

template <class T>
Field<T> patchIntegrateFunctionObject::integrate(const word& fieldName,T unsetVal) const
{
    const GeometricField<T, fvPatchField, volMesh>& fld =
        obr_.lookupObject<GeometricField<T, fvPatchField, volMesh> >
        (
            fieldName
        );

    Field<T> vals(patchNames_.size(), unsetVal);

    const fvMesh &mesh=refCast<const fvMesh>(obr_);
    
    forAll(patchNames_, patchI)
    {
        if (patchIndizes_[patchI] >= 0)
        {
            label index=patchIndizes_[patchI];
            vals[patchI] = sum
                        (
                            mesh.magSf().boundaryField()[index]
                            *fld.boundaryField()[index]
                        );
        }
    }

    forAll(vals,i) {
        reduce(vals[i],sumOp<T>());
        vals[i]*=factor();
    }

    if(verbose()) {
        Info<< regionString() 
            << " Integrals of " << fieldName << " :";

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
