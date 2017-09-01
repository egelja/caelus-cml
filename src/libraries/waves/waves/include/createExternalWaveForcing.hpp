    Info << "\nReading waveProperties" << endl;

    autoPtr<CML::waveTheories::externalWaveForcing> externalWave =
        CML::waveTheories::externalWaveForcing::New
        (
            IOobject
            (
                "externalWaveForcing",
                runTime.constant(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            runTime,
            mesh
        );
