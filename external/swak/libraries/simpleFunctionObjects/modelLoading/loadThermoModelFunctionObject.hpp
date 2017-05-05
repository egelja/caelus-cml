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
    loadThermoModelFunctionObject

Description
    Loads a thermodynamic model into memory

SourceFiles
    loadThermoModelFunctionObject.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef loadThermoModelFunctionObject_H
#define loadThermoModelFunctionObject_H

#include "modelLoadingFunctionObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class loadThermoModelFunctionObject Declaration
\*---------------------------------------------------------------------------*/

template <class ModelType>
class loadThermoModelFunctionObject
:
    public modelLoadingFunctionObject<ModelType>
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    loadThermoModelFunctionObject(const loadThermoModelFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const loadThermoModelFunctionObject&);

protected:
    virtual autoPtr<ModelType> initModel();

public:

    //- Runtime type information
    TypeName("loadThermoModelFunctionObject");


    // Constructors

    //- Construct from components
    loadThermoModelFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "addToRunTimeSelectionTable.hpp"

#include "fvMesh.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template <class ModelType>
loadThermoModelFunctionObject<ModelType>::loadThermoModelFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    modelLoadingFunctionObject<ModelType>(name,t,dict)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <class ModelType>
autoPtr<ModelType> loadThermoModelFunctionObject<ModelType>::initModel()
{
    return ModelType::New(
        //        dynamicCast<const fvMesh &>(
        dynamic_cast<const fvMesh &>(
            this->obr()
        )
    );
}


} // namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
