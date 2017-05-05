//
// createNamedPolyMesh.H
// ~~~~~~~~~~~~~~~~~~~~~

    CML::word regionName;

    if (args.optionReadIfPresent("region", regionName))
    {
        CML::Info
            << "Create polyMesh " << regionName << " for time = "
            << runTime.timeName() << CML::nl << CML::endl;
    }
    else
    {
        regionName = CML::polyMesh::defaultRegion;
        CML::Info
            << "Create polyMesh for time = "
            << runTime.timeName() << CML::nl << CML::endl;
    }

    CML::polyMesh mesh
    (
        CML::IOobject
        (
            regionName,
            runTime.timeName(),
            runTime,
            CML::IOobject::MUST_READ
        )
    );
