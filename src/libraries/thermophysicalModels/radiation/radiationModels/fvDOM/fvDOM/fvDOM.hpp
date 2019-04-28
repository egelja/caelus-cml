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
    CML::radiation::fvDOM

Description

    Finite Volume Discrete Ordinates Method. Solves the RTE equation for n
    directions in a participating media, not including scatter.

    Available absorption models:
        constantAbsorptionEmission
        greyMeanAbsoprtionEmission
        wideBandAbsorptionEmission

    i.e. dictionary
    \verbatim
        fvDOMCoeffs
        {
            nPhi        4;          // azimuthal angles in PI/2 on X-Y.
                                    //(from Y to X)
            nTheta      0;          // polar angles in PI (from Z to X-Y plane)
            convergence 1e-3;       // convergence criteria for radiation
                                    // iteration
            maxIter     4;          // maximum number of iterations
        }

        solverFreq   1; // Number of flow iterations per radiation iteration
    \endverbatim

    In 1-D the ray directions are bound to one of the X, Y or Z directions. The
    total number of solid angles is 2. nPhi and nTheta are ignored.

    In 2-D the ray directions are within one of the X-Y, X-Z or Y-Z planes. The
    total number of solid angles is 4*nPhi. nTheta is ignored.

    In 3D the rays span all directions. The total number of solid angles is
    4*nPhi*nTheta.

SourceFiles
    fvDOM.cpp

\*---------------------------------------------------------------------------*/

#ifndef radiationModelfvDOM_HPP
#define radiationModelfvDOM_HPP

#include "radiativeIntensityRay.hpp"
#include "radiationModel.hpp"
#include "fvMatrices.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace radiation
{

/*---------------------------------------------------------------------------*\
                           Class fvDOM Declaration
\*---------------------------------------------------------------------------*/

class fvDOM
:
    public radiationModel
{

    //- Incident radiation  [W/m2]
    volScalarField G_;

    //- Total radiative heat flux [W/m2]
    volScalarField qr_;

    //- Emitted radiative heat flux [W/m2]
    volScalarField qem_;

    //- Incidet radiative heat flux [W/m2]
    volScalarField qin_;

    //- Total absorption coefficient [1/m]
    volScalarField a_;

    //- Number of solid angles in theta
    label nTheta_;

    //- Number of solid angles in phi
    label nPhi_ ;

    //- Total number of rays (1 per direction)
    label nRay_;

    //- Number of wavelength bands
    label nLambda_;

    //- Wavelength total absorption coefficient [1/m]
    PtrList<volScalarField> aLambda_;

    //- Black body
    blackBodyEmission blackBody_;

    //- List of pointers to radiative intensity rays
    PtrList<radiativeIntensityRay> IRay_;

    //- Convergence tolerance
    scalar tolerance_;

    //- Maximum number of iterations
    label maxIter_;

    //- Maximum omega weight
    scalar omegaMax_;


    //- Initialise
    void initialise();

    //- Disallow default bitwise copy construct
    fvDOM(const fvDOM&);

    //- Disallow default bitwise assignment
    void operator=(const fvDOM&);

    //- Update nlack body emission
    void updateBlackBodyEmission();


public:

    //- Runtime type information
    TypeName("fvDOM");


    //- Construct from components
    fvDOM(const volScalarField& T);

    //- Construct from components
    fvDOM(const dictionary& dict, const volScalarField& T);


    //- Destructor
    virtual ~fvDOM()
    {}


    // Member functions

    // Edit

    //- Solve radiation equation(s)
    void calculate();

    //- Read radiation properties dictionary
    bool read();

    //- Update G and calculate total heat flux on boundary
    void updateG();

    //- Set the rayId and lambdaId from by decomposing an intensity
    //  field name
    void setRayIdLambdaId
    (
        const word& name,
        label& rayId,
        label& lambdaId
    ) const;

    //- Source term component (for power of T^4)
    virtual tmp<volScalarField> Rp() const;

    //- Source term component (constant)
    virtual tmp<DimensionedField<scalar, volMesh> > Ru() const;


    // Access

    //- Ray intensity for rayI
    inline const radiativeIntensityRay& IRay(const label rayI) const
    {
        return  IRay_[rayI];
    }

    //- Ray intensity for rayI and lambda bandwidth
    inline const volScalarField& IRayLambda
    (
         const label rayI,
         const label lambdaI
    ) const
    {
        return IRay_[rayI].ILambda(lambdaI);
    }

    //- Number of angles in theta
    inline label nTheta() const
    {
        return nTheta_;
    }

    //- Number of angles in phi
    inline label nPhi() const
    {
        return nPhi_;
    }

    //- Number of rays
    inline label nRay() const
    {
        return nRay_;
    }

    //- Number of wavelengths
    inline label nLambda() const
    {
        return nLambda_;
    }

    //- Const access to total absorption coefficient
    inline const volScalarField& a() const
    {
        return a_;
    }

    //- Const access to wavelength total absorption coefficient
    inline const volScalarField& aLambda(const label lambdaI) const
    {
        return aLambda_[lambdaI];
    }

    //- Const access to incident radiation field
    inline const volScalarField& G() const
    {
        return G_;
    }

    //- Const access to total radiative heat flux field
    inline const volScalarField& qr() const
    {
        return qr_;
    }

    //- Const access to incident radiative heat flux field
    inline const volScalarField& qin() const
    {
        return qin_;
    }

    //- Const access to emitted radiative heat flux field
    inline const volScalarField& qem() const
    {
        return qem_;
    }

    //- Const access to black body
    inline const blackBodyEmission& blackBody() const
    {
        return blackBody_;
    }

    //- Return omegaMax
    inline scalar omegaMax() const
    {
        return omegaMax_;
    }

};


} // End namespace radiation
} // End namespace CML


#endif
