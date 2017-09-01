/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Application
    probesNGauges

Description
    This utility writes the needed input files for wave gauges to ease the
    definition of these.

    In a later stage it is intended to be extended to be used for write the
    location of point probes as well.

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "Time.hpp"
#include "fvMesh.hpp"
#include "uniformDimensionedFields.hpp"
#include "waveGauges.hpp"
#include "probeGauges.hpp"

using namespace CML;

int main(int argc, char *argv[])
{

#   include "addTimeOptions.hpp"
#   include "setRootCase.hpp"

#   include "createTime.hpp"
#   include "createMesh.hpp"

    // Needed by e.g. the reflection analysis
#   include "readGravitationalAcceleration.hpp"
    Info << "\n";

    mkDir("waveGaugesNProbes");

    IOdictionary probeDefs
    (
        IOobject
        (
            "probeDefinitions",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    wordList toc(probeDefs.toc());

    forAll (toc, item)
    {
        word name(toc[item]);

        if (probeDefs.isDict(name))
        {
            const dictionary& dict(probeDefs.subDict(name));

            if (word(dict.lookup("type")) == "waveGauge")
            {
                waveGauges wg(mesh, dict);

                wg.evaluate(name);
            }
            else if (word(dict.lookup("type")) == "probeGauge")
            {
            	probeGauges pg(mesh, dict);

            	pg.evaluate(name);
            }
            else
            {
                Info << "Probe-type: '"
                     << word(dict.lookup("type"))
                     << "' not yet implemented" << endl;
            }
        }
    }

    Info << nl << "End" << endl;

    return 0;
}

