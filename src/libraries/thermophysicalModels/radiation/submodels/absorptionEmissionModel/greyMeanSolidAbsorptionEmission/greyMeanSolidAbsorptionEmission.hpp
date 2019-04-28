/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::radiation::greyMeanSolidAbsorptionEmission

Description
    greyMeanSolidAbsorptionEmission radiation absorption and emission
    coefficients for continuous phase

    The coefficients for the species in the Look up table have to be specified
    for use in moles x P [atm], i.e. (k[i] = species[i]*p*9.869231e-6).

    The coefficients for CO and soot or any other added are multiplied by the
    respective mass fraction being solved

    All the species in the dictionary need either to be in the look-up table or
    being solved. Conversely, all the species solved do not need to be included
    in the calculation of the absorption coefficient

    The names of the species in the absorption dictionary must match exactly the
    name in the look-up table or the name of the field being solved

SourceFiles
    greyMeanSolidAbsorptionEmission.cpp

\*---------------------------------------------------------------------------*/

#ifndef greyMeanSolidAbsorptionEmission_HPP
#define greyMeanSolidAbsorptionEmission_HPP

#include "absorptionEmissionModel.hpp"
#include "solidThermo.hpp"
#include "basicSpecieMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace radiation
{

/*---------------------------------------------------------------------------*\
                 Class greyMeanSolidAbsorptionEmission Declaration
\*---------------------------------------------------------------------------*/

class greyMeanSolidAbsorptionEmission
:
    public absorptionEmissionModel
{

private:

    //- Enumering of radiative properties
    enum radiativeProperties
    {
        absorptivity,
        emissivity
    };

    //- Absorption model dictionary
    dictionary coeffsDict_;

    //- SLG thermo package
    const solidThermo& thermo_;

    //- Hash table of species names
    HashTable<label> speciesNames_;

    //- Basic multicomponent mixture
    const basicSpecieMixture& mixture_;

    //- List of solid species data
    List<FixedList<scalar, 2> > solidData_;


    //- Calculate the volumetric fraction of Yj
    tmp<scalarField> X(const word specie) const;

    //- Calculate the property mixing
    tmp<volScalarField> calc(const label) const;


public:

    //- Runtime type information
    TypeName("greyMeanSolidAbsorptionEmission");


    //- Construct from components
    greyMeanSolidAbsorptionEmission
    (
        const dictionary& dict,
        const fvMesh& mesh
    );


    //- Destructor
    virtual ~greyMeanSolidAbsorptionEmission()
    {}


    // Member Functions

    // Access

    //- Absorption coefficient for continuous phase
    tmp<volScalarField> aCont(const label bandI = 0) const;

    //- Emission coefficient for continuous phase
    tmp<volScalarField> eCont(const label bandI = 0) const;

    inline bool isGrey() const
    {
        return true;
    }

};


} // End namespace radiation
} // End namespace CML


#endif
