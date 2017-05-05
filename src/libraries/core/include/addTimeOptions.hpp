//
// addTimeOptions.H
// ~~~~~~~~~~~~~~~~

    CML::argList::addBoolOption
    (
        "constant",
        "include the 'constant/' dir in the times list"
    );

    CML::argList::addBoolOption
    (
        "latestTime",
        "select the latest time"
    );

    CML::argList::addBoolOption
    (
        "noZero",
        "exclude the '0/' dir from the times list"
    );

    CML::argList::addOption
    (
        "time",
        "time",
        "specify a single time value to select"
    );

