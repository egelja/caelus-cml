//
// setRootCase.H
// ~~~~~~~~~~~~~

    CML::argList args(argc, argv);
    if (!args.checkRootCase())
    {
        CML::FatalError.exit();
    }
