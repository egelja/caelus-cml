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

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

#include "loadSLGThermoModelFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "fvMesh.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

#include "swakThermoTypes.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

    template <>
    void modelLoadingFunctionObject<SLGThermo>::write()
    {
        if(correctModel_) {
            if(model_.valid()) {
                FatalErrorInFunction
                    << "SLGThermo has no correct method"
                        << endl
                        << exit(FatalError);
            } else {
                FatalErrorInFunction
                    << "Model has never been intialized"
                        << endl
                        << exit(FatalError);
            }
        }
    }

    defineTypeNameAndDebug(loadSLGThermoModelFunctionObject, 0);

    addNamedToRunTimeSelectionTable
    (
        functionObject,
        loadSLGThermoModelFunctionObject,
        dictionary,
        loadSLGThermoModel
    );


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

loadSLGThermoModelFunctionObject::loadSLGThermoModelFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    modelLoadingFunctionObject<SLGThermo>(name,t,dict)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    autoPtr<SLGThermo> loadSLGThermoModelFunctionObject::initModel()
{
    return autoPtr<SLGThermo>(
        new SLGThermo(
            dynamicCast<const fvMesh &>(
                obr()
            ),
            const_cast<swakFluidThermoType &>(
                obr().lookupObject<swakFluidThermoType>(
                    dict_.lookup("thermoName")
                )
            )
        )
    );
}


} // namespace CML

// ************************************************************************* //
