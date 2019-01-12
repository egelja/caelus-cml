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
    mixture.mu()
);


basicKinematicCloud bubbleCloud
(
    "bubbleCloud",
    rho,
	U,
	mu,
	g
);
