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


    autoPtr<dynamicFvMesh> meshPtr
    (
        dynamicFvMesh::New
        (
            IOobject
            (
                regionName,
                runTime.timeName(),
                runTime,
                IOobject::MUST_READ
            )
        )
    );

    dynamicFvMesh& mesh = meshPtr();
