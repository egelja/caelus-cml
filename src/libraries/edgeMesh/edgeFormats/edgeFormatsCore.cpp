/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "edgeFormatsCore.hpp"

#include "Time.hpp"
#include "IFstream.hpp"
#include "OFstream.hpp"
#include "edgeMesh.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

CML::word CML::fileFormats::edgeFormatsCore::nativeExt("eMesh");


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

CML::string CML::fileFormats::edgeFormatsCore::getLineNoComment
(
    IFstream& is
)
{
    string line;
    do
    {
        is.getLine(line);
    }
    while ((line.empty() || line[0] == '#') && is.good());

    return line;
}


#if 0
CML::fileName CML::fileFormats::edgeFormatsCore::localMeshFileName
(
    const word& meshName
)
{
    const word name(meshName.size() ? meshName : surfaceRegistry::defaultName);

    return fileName
    (
        surfaceRegistry::prefix/name/surfMesh::meshSubDir
      / name + "." + nativeExt
    );
}


CML::fileName CML::fileFormats::edgeFormatsCore::findMeshInstance
(
    const Time& t,
    const word& meshName
)
{
    fileName localName = localMeshFileName(meshName);

    // Search back through the time directories list to find the time
    // closest to and lower than current time

    instantList ts = t.times();
    label instanceI;

    for (instanceI = ts.size()-1; instanceI >= 0; --instanceI)
    {
        if (ts[instanceI].value() <= t.timeOutputValue())
        {
            break;
        }
    }

    // Noting that the current directory has already been searched
    // for mesh data, start searching from the previously stored time directory

    if (instanceI >= 0)
    {
        for (label i = instanceI; i >= 0; --i)
        {
            if (isFile(t.path()/ts[i].name()/localName))
            {
                return ts[i].name();
            }
        }
    }

    return "constant";
}


CML::fileName CML::fileFormats::edgeFormatsCore::findMeshFile
(
    const Time& t,
    const word& meshName
)
{
    fileName localName = localMeshFileName(meshName);

    // Search back through the time directories list to find the time
    // closest to and lower than current time

    instantList ts = t.times();
    label instanceI;

    for (instanceI = ts.size()-1; instanceI >= 0; --instanceI)
    {
        if (ts[instanceI].value() <= t.timeOutputValue())
        {
            break;
        }
    }

    // Noting that the current directory has already been searched
    // for mesh data, start searching from the previously stored time directory

    if (instanceI >= 0)
    {
        for (label i = instanceI; i >= 0; --i)
        {
            fileName testName(t.path()/ts[i].name()/localName);

            if (isFile(testName))
            {
                return testName;
            }
        }
    }

    // fallback to "constant"
    return t.path()/"constant"/localName;
}
#endif


bool CML::fileFormats::edgeFormatsCore::checkSupport
(
    const wordHashSet& available,
    const word& ext,
    const bool verbose,
    const word& functionName
)
{
    if (available.found(ext))
    {
        return true;
    }
    else if (verbose)
    {
        wordList known = available.sortedToc();

        Info<<"Unknown file extension for " << functionName
            << " : " << ext << nl
            <<"Valid types: (";
        // compact output:
        forAll(known, i)
        {
            Info<<" " << known[i];
        }
        Info<<" )" << endl;
    }

    return false;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fileFormats::edgeFormatsCore::edgeFormatsCore()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::fileFormats::edgeFormatsCore::~edgeFormatsCore()
{}


// ************************************************************************* //
