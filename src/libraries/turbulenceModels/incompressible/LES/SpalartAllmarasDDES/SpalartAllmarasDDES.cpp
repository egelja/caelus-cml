#include "SpalartAllmarasDDES.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace incompressible
{
namespace LESModels
{

defineTypeNameAndDebug(SpalartAllmarasDDES, 0);
addToRunTimeSelectionTable(LESModel, SpalartAllmarasDDES, dictionary);

tmp<volScalarField> SpalartAllmarasDDES::rd
(
    const volScalarField& visc,
    const volScalarField& S
) const
{
    return
    (
        visc
        /(
            max
            (
                S,
                dimensionedScalar("SMALL", S.dimensions(), SMALL)
            )*sqr(kappa_*y_)
         )
    );
}


tmp<volScalarField> SpalartAllmarasDDES::fd(const volScalarField& S) const
{
    return 1 - tanh(pow3(8*rd(nuEff(), S)));
}


tmp<volScalarField> SpalartAllmarasDDES::S(const volTensorField& gradU) const
{
    const volScalarField contraction = (gradU && gradU);
    return sqrt(contraction);
}


tmp<volScalarField> SpalartAllmarasDDES::dTilda(const volScalarField& S) const
{
    return max
    (
        y_
      - fd(S)
       *max(y_ - CDES_*delta(), dimensionedScalar("zero", dimLength, 0)),
        dimensionedScalar("small", dimLength, SMALL)
    );
}


SpalartAllmarasDDES::SpalartAllmarasDDES
(
    const volVectorField& U,
    const surfaceScalarField& phi,
    transportModel& transport,
    const word& turbulenceModelName,
    const word& modelName
)
:
    SpalartAllmarasDES(U, phi, transport, turbulenceModelName, modelName)
{}


}
}
}


