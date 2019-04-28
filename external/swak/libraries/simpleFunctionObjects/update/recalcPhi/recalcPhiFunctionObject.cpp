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

#include "recalcPhiFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

#include "fvCFD.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

    defineTypeNameAndDebug(recalcPhiFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        recalcPhiFunctionObject,
        dictionary
    );

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

recalcPhiFunctionObject::recalcPhiFunctionObject
(
    const word &name,
    const Time& t,
    const dictionary& dict
)
:
    updateSimpleFunctionObject(name,t,dict)
{
}

bool recalcPhiFunctionObject::start()
{
    UName_=word(dict_.lookup("UName"));
    phiName_=word(dict_.lookup("phiName"));
    pName_=word(dict_.lookup("pName"));
    rhoName_=dict_.lookupOrDefault<word>("rhoName","none");
    writeOldFields_=readBool(dict_.lookup("writeOldFields"));
    writeFields_=readBool(dict_.lookup("writeFields"));

    return updateSimpleFunctionObject::start();
}

void recalcPhiFunctionObject::recalc()
{
    Info << "Calculating flux field " << phiName_
        << " for velocity " << UName_ << endl;

    const fvMesh &mesh=dynamicCast<const fvMesh&>(obr_);

    const volVectorField &U=mesh.lookupObject<volVectorField>(UName_);
    volScalarField &p=const_cast<volScalarField&>(
        mesh.lookupObject<volScalarField>(pName_)
    );
    surfaceScalarField &phi=const_cast<surfaceScalarField&>(
        mesh.lookupObject<surfaceScalarField>(phiName_)
    );

    if(writeOldFields_) {
        Info << "Writing copy of old " << phiName_ << endl;
        surfaceScalarField oldPhi(phiName_+".old",phi);
        oldPhi.write();
    }

    if(phi.dimensions()==dimensionSet(0,3,-1,0,0,0,0)) {
        phi = fvc::interpolate(U) & mesh.Sf();
    } else if(phi.dimensions()==dimensionSet(1,0,-1,0,0,0,0)) {
        if(rhoName_=="none") {
            // force read
            rhoName_=word(dict_.lookup("rhoName"));
        }
        const volScalarField &rho=mesh.lookupObject<volScalarField>(rhoName_);
        phi = fvc::interpolate(rho)*(fvc::interpolate(U) & mesh.Sf());
     } else {
        FatalErrorInFunction
            << "Can't deal with a flux field " << phiName_
                << " with dimensions " << phi.dimensions()
                << endl
                << exit(FatalError);

    }
    adjustPhi(phi, U, p);

    if(writeFields_) {
        Info << "Writing new value of " << phiName_ << endl;
        phi.write();
    }
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

} // namespace CML

// ************************************************************************* //
