/*---------------------------------------------------------------------------*\
Description
    SpalartAllmaras DDES LES turbulence model for incompressible flows

    References:
    1) P.R. Spalart, S. Deck, S., M.L.Shur, K.D. Squires, M.Kh Strelets, and
       A. Travin. `A new version of detached-eddy simulation, resistant to
       ambiguous grid densities'. Theor. Comp. Fluid Dyn., 20:181-195, 2006.

    2) Spalart, P. R. and Allmaras, S. R., "A One-Equation Turbulence Model for 
       Aerodynamic Flows," Recherche Aerospatiale, No. 1, 1994, pp. 5-21.

Author: A. Jemcov
Copyright Applied CCM (2014), ESI
\*---------------------------------------------------------------------------*/

#ifndef compressibleSpalartAllmarasDDES_HPP
#define compressibleSpalartAllmarasDDES_HPP

#include "SpalartAllmarasDES.hpp"

namespace CML
{
namespace compressible
{
namespace LESModels
{

class SpalartAllmarasDDES
:
    public SpalartAllmarasDES
{
    // Private Member Functions

        tmp<volScalarField> fd(const volScalarField& S) const;

        tmp<volScalarField> rd
        (
            const volScalarField& visc,
            const volScalarField& S
        ) const;

        // Disallow default bitwise copy construct and assignment
        SpalartAllmarasDDES(const SpalartAllmarasDDES&);
        SpalartAllmarasDDES& operator=(const SpalartAllmarasDDES&);


protected:

    // Protected Member Functions

        //- Length scale
        virtual tmp<volScalarField> dTilda(const volScalarField& S) const;

        virtual tmp<volScalarField> S(const volTensorField& gradU) const;


public:

    //- Runtime type information
    TypeName("SpalartAllmarasDDES");


    // Constructors

        //- Construct from components
        SpalartAllmarasDDES
        (
            const volScalarField& rho,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const basicThermo& thermophysicalModel,
            const word& turbulenceModelName = turbulenceModel::typeName,
            const word& modelName = typeName
        );


    //- Destructor
    virtual ~SpalartAllmarasDDES()
    {}
};

} 
} 
}

#endif


