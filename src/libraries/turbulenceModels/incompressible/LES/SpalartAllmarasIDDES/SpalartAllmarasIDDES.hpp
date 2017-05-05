/*---------------------------------------------------------------------------*\
    SpalartAllmarasIDDES LES turbulence model for incompressible flows

    Reference:
    "A hybrid RANS-LES approach with delayed-DES and wall-modelled LES
    Capabilities", M. L. Shur, P. R. Spalart, K. Kh. Strelets, A. K. Travin,
    International Journal of Heat Transfer and Fluid Flow, 29 (2008), 
    pp. 1638-1649

Author: A. Jemcov
Copyright Applied CCM (2014), ESI

\*---------------------------------------------------------------------------*/

#ifndef SpalartAllmarasIDDES_HPP
#define SpalartAllmarasIDDES_HPP

#include "SpalartAllmarasDES.hpp"

namespace CML
{
namespace incompressible
{
namespace LESModels
{

class SpalartAllmarasIDDES
:
    public SpalartAllmarasDES
{
    // Private data

        // Model constants

            autoPtr<CML::LESdelta> hmax_;
            autoPtr<CML::LESdelta> IDDESDelta_;
            dimensionedScalar fwStar_;
            dimensionedScalar cl_;
            dimensionedScalar ct_;


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
        tmp<volScalarField> fd(const volScalarField& S) const;

        // Disallow default bitwise copy construct and assignment
        SpalartAllmarasIDDES(const SpalartAllmarasIDDES&);
        SpalartAllmarasIDDES& operator=(const SpalartAllmarasIDDES&);


protected:

    // Protected Member Functions

        //- Length scale
        virtual tmp<volScalarField> dTilda(const volScalarField& S) const;

        virtual tmp<volScalarField> S(const volTensorField& gradU) const;


public:

    //- Runtime type information
    TypeName("SpalartAllmarasIDDES");


    // Constructors

        //- Construct from components
        SpalartAllmarasIDDES
        (
            const volVectorField& U,
            const surfaceScalarField& phi,
            transportModel& transport,
            const word& turbulenceModelName = turbulenceModel::typeName,
            const word& modelName = typeName
        );


    //- Destructor
    virtual ~SpalartAllmarasIDDES()
    {}


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

