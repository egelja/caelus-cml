autoPtr<fluidThermo> thermo
(
    fluidThermo::New(mesh)
);

const volScalarField& h = thermo->he();

volScalarField rho
(
    IOobject
    (
        "rho",
        runTime.timeName(),
        mesh
    ),
    thermo->rho()
);

volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

#include "compressibleCreatePhi.hpp"

autoPtr<compressible::RASModel> RASModel
(
    compressible::RASModel::New
    (
        rho,
        U,
        phi,
        thermo()
    )
);
