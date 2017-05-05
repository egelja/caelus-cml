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
    volumeMinMaxFunctionObject

Description
    Calculates max and min of the fields on a volume

SourceFiles
    volumeMinMaxFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef volumeMinMaxFunctionObject_H
#define volumeMinMaxFunctionObject_H

#include "volumeFieldFunctionObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class volumeMinMaxFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class volumeMinMaxFunctionObject
:
    public volumeFieldFunctionObject
{

    // Private Member Functions

    //- Disallow default bitwise copy construct
    volumeMinMaxFunctionObject(const volumeMinMaxFunctionObject&);
    
    //- Disallow default bitwise assignment
    void operator=(const volumeMinMaxFunctionObject&);

    //- calculates minimum and maximum of a field
    template <class T>
    Field<T> minMax(const word& fieldName,T preset) const;
    
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
    TypeName("volumeMinMax");


    // Constructors

    //- Construct from components
    volumeMinMaxFunctionObject
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
Field<T> volumeMinMaxFunctionObject::minMax(const word& fieldName,T unsetVal) const
{
    const GeometricField<T, fvPatchField, volMesh>& fld =
        obr_.lookupObject<GeometricField<T, fvPatchField, volMesh> >
        (
            fieldName
        );

    Field<T> vals(2, unsetVal);

    vals[0] = 
        min
        (
            fld
        ).value();

    vals[1] = 
        max
        (
            fld
        ).value();

    if(verbose()) {
        Info<< regionString() 
            << " Range of " << fieldName << " [ "
            << vals[0] << " , " << vals[1] << " ] "
            << fld.dimensions() << endl;
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
