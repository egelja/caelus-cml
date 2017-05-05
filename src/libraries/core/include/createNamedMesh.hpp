//
// createNamedMesh.H
// ~~~~~~~~~~~~~~~~~

    CML::word regionName;

    if (args.optionReadIfPresent("region", regionName))
    {
        CML::Info
            << "Create mesh " << regionName << " for time = "
            << runTime.timeName() << CML::nl << CML::endl;
    }
    else
    {
        regionName = CML::fvMesh::defaultRegion;
        CML::Info
            << "Create mesh for time = "
            << runTime.timeName() << CML::nl << CML::endl;
    }

    CML::fvMesh mesh
    (
        CML::IOobject
        (
            regionName,
            runTime.timeName(),
            runTime,
            CML::IOobject::MUST_READ
        )
    );
