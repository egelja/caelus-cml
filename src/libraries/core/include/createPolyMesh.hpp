//
// createPolyMesh.H
// ~~~~~~~~~~~~~~~~

    CML::Info
        << "Create polyMesh for time = "
        << runTime.timeName() << CML::nl << CML::endl;

    CML::polyMesh mesh
    (
        CML::IOobject
        (
            CML::polyMesh::defaultRegion,
            runTime.timeName(),
            runTime,
            CML::IOobject::MUST_READ
        )
    );
