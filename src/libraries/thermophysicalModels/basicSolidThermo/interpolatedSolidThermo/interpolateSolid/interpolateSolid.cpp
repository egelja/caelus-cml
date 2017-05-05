/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

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

\*---------------------------------------------------------------------------*/

#include "interpolateSolid.hpp"
#include "interpolateXY.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::interpolateSolid::interpolateSolid(const dictionary& dict)
{
    read(dict);

    Info<< "Constructed directionalKSolidThermo with samples" << nl
        << "    T          : " << TValues_ << nl
        << "    rho        : " << rhoValues_ << nl
        << "    cp         : " << cpValues_ << nl
        << "    Hf         : " << HfValues_ << nl
        << "    emissivity : " << emissivityValues_ << nl
        << "    kappa : " << kappaValues_ << nl
        << "    sigmaS : " << sigmaSValues_ << nl
        << endl;


    if
    (
        (TValues_.size() != rhoValues_.size())
     && (TValues_.size() != cpValues_.size())
     && (TValues_.size() != rhoValues_.size())
     && (TValues_.size() != HfValues_.size())
     && (TValues_.size() != emissivityValues_.size())
    )
    {
        FatalIOErrorIn("interpolateSolid::read()", dict)
            << "Size of property tables should be equal to size of Temperature"
            << " values " << TValues_.size()
            << exit(FatalIOError);
    }

    for (label i = 1; i < TValues_.size(); i++)
    {
        if (TValues_[i] <= TValues_[i-1])
        {
            FatalIOErrorIn("interpolateSolid::read()", dict)
                << "Temperature values are not in increasing order "
                << TValues_ << exit(FatalIOError);
        }
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::interpolateSolid::~interpolateSolid()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::interpolateSolid::writeData(Ostream& os) const
{

    os.writeKeyword("TValues") << TValues_ << token::END_STATEMENT << nl;
    os.writeKeyword("rhoValues") << rhoValues_ << token::END_STATEMENT << nl;
    os.writeKeyword("cpValues") << cpValues_ << token::END_STATEMENT << nl;
    os.writeKeyword("HfValues") << HfValues_ << token::END_STATEMENT << nl;
    os.writeKeyword("emissivityValues") << emissivityValues_ <<
         token::END_STATEMENT << nl;
    os.writeKeyword("kappaValues") << kappaValues_
        << token::END_STATEMENT << nl;
    os.writeKeyword("sigmaSValues") << sigmaSValues_
        << token::END_STATEMENT << nl;


    return os.good();
}


bool CML::interpolateSolid::read(const dictionary& dict)
{
    TValues_ = Field<scalar>(dict.lookup("TValues"));
    rhoValues_ = Field<scalar>(dict.lookup("rhoValues"));
    cpValues_ = Field<scalar>(dict.lookup("cpValues"));
    kappaValues_ = Field<scalar>(dict.lookup("kappaValues"));
    sigmaSValues_ = Field<scalar>(dict.lookup("sigmaSValues"));
    HfValues_ = Field<scalar>(dict.lookup("HfValues"));
    emissivityValues_ = Field<scalar>(dict.lookup("emissivityValues"));
    sigmaSValues_ = Field<scalar>(dict.lookup("sigmaSValues"));
    return true;
}


// ************************************************************************* //
