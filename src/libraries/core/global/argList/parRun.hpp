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

Class
    CML::ParRunControl

Description
    Helper class for initializing parallel jobs from the command arguments.

\*---------------------------------------------------------------------------*/

#ifndef parRun_H
#define parRun_H

#include "Pstream.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class ParRunControl Declaration
\*---------------------------------------------------------------------------*/

class ParRunControl
{
    bool RunPar;

public:

    ParRunControl()
    :
        RunPar(false)
    {}

    ~ParRunControl()
    {
        if (RunPar)
        {
            Info<< "Finalising parallel run" << endl;
            Pstream::exit(0);
        }
    }

    void runPar(int& argc, char**& argv)
    {
        RunPar = true;

        if (!Pstream::init(argc, argv))
        {
            Info<< "Failed to start parallel run" << endl;
            Pstream::exit(1);
        }
    }

    bool parRun() const
    {
        return RunPar;
    }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
