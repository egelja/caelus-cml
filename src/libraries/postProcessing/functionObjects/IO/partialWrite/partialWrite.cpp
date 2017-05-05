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

#include "partialWrite.hpp"
#include "dictionary.hpp"
#include "Time.hpp"
#include "IOobjectList.hpp"
#include "polyMesh.hpp"
#include "cloud_.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(partialWrite, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::partialWrite::partialWrite
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    name_(name),
    obr_(obr)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::partialWrite::~partialWrite()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::partialWrite::read(const dictionary& dict)
{
    dict.lookup("objectNames") >> objectNames_;
    dict.lookup("writeInterval") >> writeInterval_;
    writeInstance_ = 0;

    Info<< type() << " " << name() << ":" << nl
        << "    dumping every outputTime :";
    forAllConstIter(HashSet<word>, objectNames_, iter)
    {
        Info<< ' ' << iter.key();
    }
    Info<< nl
        << "    dumping all other fields every "
        << writeInterval_ << "th outputTime" << nl
        << endl;

    if (writeInterval_ < 1)
    {
        FatalIOErrorIn("partialWrite::read(const dictionary&)", dict)
            << "Illegal value for writeInterval " << writeInterval_
            << ". It should be >= 1."
            << exit(FatalIOError);
    }
}


void CML::partialWrite::execute()
{
    //Pout<< "execute at time " << obr_.time().timeName()
    //    << " index:" << obr_.time().timeIndex() << endl;
}


void CML::partialWrite::end()
{
    //Pout<< "end at time " << obr_.time().timeName() << endl;
    // Do nothing - only valid on write
}


void CML::partialWrite::write()
{
    //Pout<< "write at time " << obr_.time().timeName() << endl;
    if (obr_.time().outputTime())
    {
        // Above check so it can be used both with
        //  outputControl   timeStep;
        //  outputInterval  1;
        // or with
        //  outputControl   outputTime;

        writeInstance_++;

        if (writeInstance_ == writeInterval_)
        {
            // Normal dump
            writeInstance_ = 0;
        }
        else
        {
            // Delete all but marked objects
            fileName dbDir;
            if (isA<polyMesh>(obr_))
            {
                dbDir = dynamic_cast<const polyMesh&>(obr_).dbDir();
            }

            IOobjectList objects(obr_, obr_.time().timeName());

            if (debug)
            {
                Pout<< "For region:" << obr_.name() << endl;
            }

            forAllConstIter(IOobjectList, objects, iter)
            {
                if (!objectNames_.found(iter()->name()))
                {
                    const fileName f =
                        obr_.time().timePath()
                       /dbDir
                       /iter()->name();
                    if (debug)
                    {
                        Pout<< "   rm " << f << endl;
                    }
                    rm(f);
                }
            }

            // Do the lagrangian files as well.
            fileNameList cloudDirs
            (
                readDir
                (
                    obr_.time().timePath()/dbDir/cloud::prefix,
                    fileName::DIRECTORY
                )
            );
            forAll(cloudDirs, i)
            {
                if (debug)
                {
                    Pout<< "For cloud:" << cloudDirs[i] << endl;
                }

                IOobjectList sprayObjs
                (
                    obr_,
                    obr_.time().timeName(),
                    cloud::prefix/cloudDirs[i]
                );
                forAllConstIter(IOobjectList, sprayObjs, iter)
                {
                    if (!objectNames_.found(iter()->name()))
                    {
                        const fileName f =
                            obr_.time().timePath()
                           /dbDir
                           /cloud::prefix
                           /cloudDirs[i]
                           /iter()->name();
                        if (debug)
                        {
                            Pout<< "   rm " << f << endl;
                        }
                        rm(f);
                    }
                }
            }
        }
    }
}


// ************************************************************************* //
