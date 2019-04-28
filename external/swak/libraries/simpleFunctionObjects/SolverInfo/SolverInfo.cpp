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

#include "SolverInfo.hpp"

#include "objectRegistry.hpp"
#include "surfaceFields.hpp"

namespace CML {

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    SolverInfo::SolverInfo(const objectRegistry &obr,const dictionary& dict)
:
    phi_(
        dict.found("phi")
        ? word(dict.lookup("phi"))
        : word("phi")
    ),
    compressible_(false),
    turbulent_(false),
    les_(false)
{
    if(obr.foundObject<surfaceScalarField>(phi_)) {
        const surfaceScalarField &phi=obr.lookupObject<surfaceScalarField>(phi_);

        if(phi.dimensions()==dimensionSet(1,0,-1,0,0,0,0)) {
            compressible_=true;
        } else if(phi.dimensions()!=dimensionSet(0,3,-1,0,0,0,0)) {
        WarningInFunction
            << " Dimensions " << phi.dimensions() 
                << " of the phi-field with name " << phi_ 
                << "don't fit compressible or incompressible " << nl
                << "Assumin incompressible solver" << endl;            
        }
    } else {
        WarningInFunction
            << "Can't find phi-field with name " << phi_ << nl
                << "Assumin incompressible solver" << endl;
    }

    if (isFile(obr.time().constantPath()/"turbulenceProperties")) {
        turbulent_=true;

        bool rasFound=obr.foundObject<IOdictionary>("RASProperties");
        bool lesFound=obr.foundObject<IOdictionary>("LESProperties");

        if(rasFound && lesFound) {
            WarningInFunction
                << "LES and RAS found. Assuming RAS" << endl;
        } else if(lesFound) {
            les_=true;
        } else if(!rasFound && !lesFound) {
            WarningInFunction
                << "Neither LES nor RAS found. Assuming no turbulence" << endl;
            turbulent_=false;
        }
    }
    printInfo();
}

void SolverInfo::printInfo()
{
    Info << "phi: " << phi_ << endl;
    Info << "Compressible: " << compressible_ << endl;
    Info << "Turbulent: " << turbulent_ << endl;
    Info << "LES: " << les_ << endl;
}

}

// ************************************************************************* //
