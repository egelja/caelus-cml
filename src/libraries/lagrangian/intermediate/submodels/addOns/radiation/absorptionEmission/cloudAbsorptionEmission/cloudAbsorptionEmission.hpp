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

Class
    CML::radiation::cloudAbsorptionEmission

Description
    Retrieves absorption/emission data from a cloud object

SourceFiles
    cloudAbsorptionEmission.cpp

\*---------------------------------------------------------------------------*/

#ifndef radiationCloudAbsorptionEmission_H
#define radiationCloudAbsorptionEmission_H

#include "absorptionEmissionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace radiation
{

/*---------------------------------------------------------------------------*\
                  Class cloudAbsorptionEmission Declaration
\*---------------------------------------------------------------------------*/

class cloudAbsorptionEmission
:
    public absorptionEmissionModel
{
    // Private data

        //- Coefficients dictionary
        dictionary coeffsDict_;

        //- Cloud name(s)
        const wordList cloudNames_;


public:

    //- Runtime type information
    TypeName("cloudAbsorptionEmission");


    // Constructors

        //- Construct from components
        cloudAbsorptionEmission(const dictionary& dict, const fvMesh& mesh);


    //- Destructor
    virtual ~cloudAbsorptionEmission();


    // Member Operators

        // Access

            // Absorption coefficient

                //- Absorption coefficient for dispersed phase
                virtual tmp<volScalarField> aDisp(const label bandI = 0) const;


            // Emission coefficient

                //- Emission coefficient for dispersed phase
                virtual tmp<volScalarField> eDisp(const label bandI = 0) const;


            // Emission contribution

                //- Return emission contribution for dispersed phase
                virtual tmp<volScalarField> EDisp(const label bandI = 0) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace radiation
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
