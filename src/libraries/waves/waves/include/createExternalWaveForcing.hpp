autoPtr<CML::waveTheories::externalWaveForcing> externalWave;

if (waves)
{
    Info << "\nCreating externalWaveForcing" << endl;

    externalWave =
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
}
