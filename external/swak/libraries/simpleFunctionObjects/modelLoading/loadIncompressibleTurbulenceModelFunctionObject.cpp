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
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "loadIncompressibleTurbulenceModelFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "fvMesh.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

    defineTypeNameAndDebug(loadIncompressibleTurbulenceModelFunctionObject, 0);

    addNamedToRunTimeSelectionTable
    (
        functionObject,
        loadIncompressibleTurbulenceModelFunctionObject,
        dictionary,
        loadIncompressibleTurbulenceModel
    );


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

loadIncompressibleTurbulenceModelFunctionObject::loadIncompressibleTurbulenceModelFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    modelLoadingFunctionObject<incompressible::turbulenceModel>(name,t,dict)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    autoPtr<incompressible::turbulenceModel> loadIncompressibleTurbulenceModelFunctionObject::initModel()
{
    return incompressible::turbulenceModel::New(
        obr().lookupObject<volVectorField>(
            dict_.lookup("UName")
        ),
        obr().lookupObject<surfaceScalarField>(
            dict_.lookup("phiName")
        ),
        const_cast<transportModel &>(
            obr().lookupObject<transportModel>(
                dict_.lookup("transportModel")
            )
        )
    );
}


} // namespace CML

// ************************************************************************* //
