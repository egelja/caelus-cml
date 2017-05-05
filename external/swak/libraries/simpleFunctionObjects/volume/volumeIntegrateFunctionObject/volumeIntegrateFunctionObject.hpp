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
    volumeIntegrateFunctionObject

Description
    Integrates the fields on the volumes

SourceFiles
    volumeIntegrateFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef volumeIntegrateFunctionObject_H
#define volumeIntegrateFunctionObject_H

#include "volumeFieldFunctionObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class volumeIntegrateFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class volumeIntegrateFunctionObject
:
    public volumeFieldFunctionObject
{

    // Private Member Functions

    //- Disallow default bitwise copy construct
    volumeIntegrateFunctionObject(const volumeIntegrateFunctionObject&);
    
    //- Disallow default bitwise assignment
    void operator=(const volumeIntegrateFunctionObject&);

    //- averages single field
    template <class T>
    Field<T> integrate(const word& fieldName,T preset) const;
    
protected:

    word dirName();

    stringList columnNames();
    
    virtual scalarField process(const word& fieldName,scalar preset);
    virtual Field<vector> process(const word& fieldName,vector preset);
    virtual Field<sphericalTensor> process(const word& fieldName,sphericalTensor preset);
    virtual Field<symmTensor> process(const word& fieldName,symmTensor preset);
    virtual Field<tensor> process(const word& fieldName,tensor preset);

public:

    //- Runtime type information
    TypeName("volumeIntegrate");


    // Constructors

    //- Construct from components
    volumeIntegrateFunctionObject
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
Field<T> volumeIntegrateFunctionObject::integrate(const word& fieldName,T unsetVal) const
{
    const GeometricField<T, fvPatchField, volMesh>& fld =
        obr_.lookupObject<GeometricField<T, fvPatchField, volMesh> >
        (
            fieldName
        );

    Field<T> vals(1, unsetVal);

    const fvMesh &mesh=refCast<const fvMesh>(obr_);
    
    vals[0] = (
        sum
        (
            mesh.V()*fld
        )
    ).value();

    forAll(vals,i) {
        vals[i]*=factor();
    }

    if(verbose()) {
        Info<< regionString() 
            << " Integral of " << fieldName << " = "
            << vals[0] << "  " 
            << fld.dimensions()*dimensionSet(0,3,0,0,0,0,0)
            << endl;
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
