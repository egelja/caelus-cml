IOdictionary waveProperties
(
    IOobject
    (
        "waveProperties",
        runTime.constant(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::NO_WRITE
    )
);

// Compute a reference point that is placed on the free surface
// A zero offset between original and new reference level. Default offset
dimensionedVector referencePoint("NULL", dimLength, vector::zero);

bool waves = false;

if (waveProperties.headerOk())
{
    waves = true;

    Info << "\nReading waveProperties" << endl;

 	// Get the sea level
    scalar sL = readScalar(waveProperties.lookup("seaLevel"));

    if (SMALL < CML::mag(sL))
  	{
        if (Switch(waveProperties.lookup("seaLevelAsReference")))
        {
      	    // Make a positive unit vector along the direction of gravity
            referencePoint.value() = g.value()/CML::mag(g.value());
            referencePoint.value() = CML::cmptMag(referencePoint.value());

            // Make the sea level the reference level
      	    referencePoint.value() *= sL;
        }
    }
}
