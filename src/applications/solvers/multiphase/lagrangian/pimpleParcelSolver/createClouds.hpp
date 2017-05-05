volScalarField mu
(
    IOobject
    (
        "mu",
	    runTime.timeName(),
	    mesh,
	    IOobject::NO_READ,
	    IOobject::AUTO_WRITE
    ),
    rhoInf*turbulence->nu()
);

IOdictionary cloudSettings
(
    IOobject
    (
        "cloudSettings",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
);

label nClouds = 1;

if (cloudSettings.headerOk())
{
    Info<< "Reading the number of clouds from cloudSettings dictionary"<<endl;
    nClouds = readScalar(cloudSettings.lookup("nClouds"));       
}

const word kinematicCloudName
(
    args.optionLookupOrDefault<word>("cloudName", "kinematicCloud")
);

PtrList<basicKinematicCloud> kinematicClouds(nClouds);

for (label i = 0; i < nClouds; ++i)
{
    Info<< "Constructing kinematicCloud " << kinematicCloudName + name(i) << endl;

    kinematicClouds.set
    (
        i,
        new basicKinematicCloud
        (
            kinematicCloudName + name(i),
            rhoInf,
            U,
            mu,
            g
        )
    );
}
