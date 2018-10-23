/*---------------------------------------------------------------------------*\
Copyright (C) 2018 Applied CCM
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
    
Description
    kOmegaSST IDDES LES turbulence model for incompressible flows

    References:
        Gritskevich, M.S., Garbaruk, A.V., Schuetze, J., Menter, F.R. (2011)
        Development of DDES and IDDES Formulations for the k-omega
        Shear Stress Transport Model, Flow, Turbulence and Combustion,
        pp. 1-19

        M. L. Shur, P. R. Spalart, M. K. Strelets, and A. K. Travin. (2008)
        A Hybrid RANS-LES Approach With Delayed-DES and Wall-Modelled LES 
        Capabilities. International Journal of Heat and Fluid Flow. 29:6.
        December 2008. pp. 1638-1649

Author: C. Sideroff

\*---------------------------------------------------------------------------*/

#ifndef kOmegaSSTDDES_HPP
#define kOmegaSSTDDES_HPP

#include "kOmegaSSTDES.hpp"

namespace CML
{
namespace incompressible
{
namespace LESModels
{

class kOmegaSSTIDDES : public kOmegaSSTDES
{
    // Private Data

        // Model coefficients

        autoPtr<CML::LESdelta> hmax_;
        autoPtr<CML::LESdelta> IDDESDelta_;

        dimensionedScalar Cdt1_;
        dimensionedScalar Cdt2_;
        dimensionedScalar Cl_;
        dimensionedScalar Ct_;

        Switch simplified_;

    // Private Member Functions

        tmp<volScalarField> alpha() const;
        tmp<volScalarField> ft(const volScalarField& S) const;
        tmp<volScalarField> fl(const volScalarField& S) const;

        tmp<volScalarField> rd
        (
            const volScalarField& visc,
            const volScalarField& S
        ) const;

        //- Delay function
        tmp<volScalarField> fdt(const volScalarField& S) const;

        tmp<volScalarField> LIDDES() const;

    // Disallow default bitwise copy construct and assignment
    kOmegaSSTIDDES(kOmegaSSTIDDES const&);
    kOmegaSSTIDDES& operator=(kOmegaSSTIDDES const&);

protected:

    // Protected Member Functions

    //- Length scale
    virtual tmp<volScalarField> FDES() const;

public:

    //- Runtime type information
    TypeName("kOmegaSSTIDDES");


    // Constructors

    //- Construct from components
    kOmegaSSTIDDES
    (
        volVectorField const& U,
        surfaceScalarField const& phi,
        transportModel& transport,
        word const& turbulenceModelName = turbulenceModel::typeName,
        word const& modelName = typeName
    );

    //- Destructor
    virtual ~kOmegaSSTIDDES() {}

    // Member Functions

        //- Access function to filter width
        virtual const volScalarField& delta() const
        {
            return IDDESDelta_();
        }

        //- Read LESProperties dictionary
        virtual bool read();

};

} 
} 
}

#endif


