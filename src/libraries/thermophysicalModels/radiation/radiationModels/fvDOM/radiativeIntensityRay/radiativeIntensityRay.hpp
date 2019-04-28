/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::radiation::radiativeIntensityRay

Description
    Radiation intensity for a ray in a given direction

SourceFiles
    radiativeIntensityRay.cpp

\*---------------------------------------------------------------------------*/

#ifndef radiativeIntensityRay_HPP
#define radiativeIntensityRay_HPP

#include "absorptionEmissionModel.hpp"
#include "blackBodyEmission.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace radiation
{

// Forward declaration of classes
class fvDOM;

/*---------------------------------------------------------------------------*\
                    Class radiativeIntensityRay Declaration
\*---------------------------------------------------------------------------*/

class radiativeIntensityRay
{
public:

    static const word intensityPrefix;


private:

    //- Reference to the owner fvDOM object
    const fvDOM& dom_;

    //- Reference to the mesh
    const fvMesh& mesh_;

    //- Absorption/emission model
    const absorptionEmissionModel& absorptionEmission_;

    //- Black body
    const blackBodyEmission& blackBody_;

    //- Total radiative intensity / [W/m2]
    volScalarField I_;

    //- Total radiative heat flux on boundary
    volScalarField qr_;

    //- Incident radiative heat flux on boundary
    volScalarField qin_;

    //- Emitted radiative heat flux on boundary
    volScalarField qem_;

    //- Direction
    vector d_;

    //- Average direction vector inside the solid angle
    vector dAve_;

    //- Theta angle
    scalar theta_;

    //- Phi angle
    scalar phi_;

    //- Solid angle
    scalar omega_;

    //- Number of wavelengths/bands
    label nLambda_;

    //- List of pointers to radiative intensity fields for given wavelengths
    PtrList<volScalarField> ILambda_;

    //- Global ray id - incremented in constructor
    static label rayId;

    //- My ray Id
    label myRayId_;


    //- Disallow default bitwise copy construct
    radiativeIntensityRay(const radiativeIntensityRay&);

    //- Disallow default bitwise assignment
    void operator=(const radiativeIntensityRay&);


public:


    //- Construct form components
    radiativeIntensityRay
    (
        const fvDOM& dom,
        const fvMesh& mesh,
        const scalar phi,
        const scalar theta,
        const scalar deltaPhi,
        const scalar deltaTheta,
        const label lambda,
        const absorptionEmissionModel& absEmmModel_,
        const blackBodyEmission& blackBody,
        const label rayId
    );


    //- Destructor
    ~radiativeIntensityRay()
    {}


    // Member functions

    // Edit

    //- Update radiative intensity on i direction
    scalar correct();

    //- Initialise the ray in i direction
    void init
    (
        const scalar phi,
        const scalar theta,
        const scalar deltaPhi,
        const scalar deltaTheta,
        const scalar lambda
    );

    //- Add radiative intensities from all the bands
    void addIntensity();


    // Access

    //- Return intensity
    inline const volScalarField& I() const
    {
        return I_;
    }

    //- Return const access to the boundary heat flux
    inline const volScalarField& qr() const
    {
        return qr_;
    }

    //- Return non-const access to the boundary heat flux
    inline volScalarField& qr()
    {
        return qr_;
    }

    //- Return non-const access to the boundary incident heat flux
    inline volScalarField& qin()
    {
        return qin_;
    }

    //- Return const access to the boundary incident heat flux
    inline const volScalarField& qin() const
    {
        return qin_;
    }

    //- Return non-const access to the boundary emitted heat flux
    inline volScalarField& qem()
    {
        return qem_;
    }

    //- Return const access to the boundary emitted heat flux
    inline const volScalarField& qem() const
    {
        return qem_;
    }

    //- Return direction
    inline const vector& d() const
    {
        return d_;
    }

    //- Return the average vector inside the solid angle
    inline const vector& dAve() const
    {
        return dAve_;
    }

    //- Return the number of bands
    inline scalar nLambda() const
    {
        return nLambda_;
    }

    //- Return the phi angle
    inline scalar phi() const
    {
        return phi_;
    }

    //- Return the theta angle
    inline scalar theta() const
    {
        return theta_;
    }

    //- Return the solid angle
    inline scalar omega() const
    {
        return omega_;
    }

    //- Return the radiative intensity for a given wavelength
    inline const volScalarField& ILambda(const label lambdaI) const
    {
        return ILambda_[lambdaI];
    }

};


} // End namespace radiation
} // End namespace CML


#endif
