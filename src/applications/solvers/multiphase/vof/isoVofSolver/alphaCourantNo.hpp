
scalar maxAlphaCo
(
    readScalar(runTime.controlDict().lookup("maxAlphaCo"))
);

scalar alphaCoNum = 0.0;
scalar meanAlphaCoNum = 0.0;

if (mesh.nInternalFaces())
{
    scalarField sumPhi
    (
        interface.nearInterface()().internalField()
       *fvc::surfaceSum(mag(phi))().internalField()
    );

    alphaCoNum = 0.5*gMax(sumPhi/mesh.V().field())*runTime.deltaTValue();

    meanAlphaCoNum =
        0.5*(gSum(sumPhi)/gSum(mesh.V().field()))*runTime.deltaTValue();
}

Info<< "Interface Courant Number mean: " << meanAlphaCoNum
    << " max: " << alphaCoNum << endl;

// ************************************************************************* //
