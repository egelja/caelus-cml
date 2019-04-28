/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

#include "waveGauges.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(waveGauges, 0);

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


void waveGauges::writeVTKFormat
(
    const word& name,
    const pointField& pp,
    const point& addPoint
)
{
    autoPtr<OFstream> vtk;

    // Writing the lines as VTK format
    vtk.reset(new OFstream("waveGaugesNProbes/" + name + ".vtk"));

    // Writing header
    vtk() << "# vtk DataFile Version 3.0" << nl << "vtk output" << nl
          << "ASCII" << nl << "DATASET POLYDATA" << endl;

    // Writing points
    vtk() << "POINTS " << 2*pp.size() << " float" << endl;

    forAll(pp, pointi)
    {
        point p(pp[pointi]);
        vtk() << p.x() << " " << p.y() << " " << p.z() << endl;
    }

    forAll(pp, pointi)
    {
        point p(pp[pointi] + addPoint);
        vtk() << p.x() << " " << p.y() << " " << p.z() << endl;
    }

    // Writing lines
    vtk() << "LINES " << pp.size() << " " << 3*pp.size() << endl;
    forAll (pp, pointi)
    {
        vtk() << "2 " << pointi << " " << pointi + pp.size() << endl;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


waveGauges::waveGauges
(
    const fvMesh& mesh,
    const dictionary& dict
)
:
    mesh_(mesh),

    gaugeDict_(dict)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void waveGauges::evaluate(const word& name)
{
    point addPoint(gaugeDict_.lookup("add"));
    word  vertAxis(word(gaugeDict_.lookup("axis")));

    autoPtr<CML::pointDistributions> pd
    (
        CML::pointDistributions::New(mesh_, gaugeDict_)
    );

    pointField pp(pd->evaluate());

    autoPtr<OFstream> gauges;

    // Writing the sets file
    gauges.reset(new OFstream("waveGaugesNProbes/" + name + "_sets"));

    gauges() << "sets" << nl << token::BEGIN_LIST << nl << incrIndent;

    forAll (pp, pointi)
    {
        gauges() << indent << "gauge_" << pointi << nl << indent
                 << token::BEGIN_BLOCK << incrIndent << nl;
        gauges() << indent << "type         face"
                 << token::END_STATEMENT << nl;
        gauges() << indent << "axis         " << vertAxis
                 << token::END_STATEMENT << nl;
        gauges() << indent << "start        " << pp[pointi]
                 << token::END_STATEMENT << nl;
        gauges() << indent << "end          " << pp[pointi] + addPoint
                 << token::END_STATEMENT << nl;
        gauges() << indent << "nPoints      100" << token::END_STATEMENT << nl;
        gauges() << decrIndent << indent << token::END_BLOCK << nl << nl;
    }

    gauges() << decrIndent << token::END_LIST << token::END_STATEMENT << nl;

    // Writing the file to be included in controlDict
    gauges.reset(new OFstream("waveGaugesNProbes/" + name + "_controlDict"));

    gauges() << incrIndent << indent << name << nl << indent
             << token::BEGIN_BLOCK << nl << incrIndent;
    gauges() << indent << "type               surfaceElevation;" << nl;
    gauges() << indent << "functionObjectLibs ( \"libwavesSampling.so\" );" << nl;
    gauges() << nl;
    word wc(gaugeDict_.lookup("writeControl"));
    scalar wi = readScalar(gaugeDict_.lookup("writeInterval"));
    gauges() << indent << "writeControl      " << wc << ";" << nl;
    gauges() << indent << "writeInterval      " << wi << ";" << nl;
    gauges() << nl;
    gauges() << indent << "setFormat          raw;" << nl;
    gauges() << indent << "interpolationScheme cellPointFace;" << nl;
    gauges() << indent << "fields (" << "alpha1" << ");"
             << nl << nl;
    gauges() << indent << "#includeIfPresent \"../waveGaugesNProbes/" << name
             << "_sets\";" << nl << nl;
    gauges() << decrIndent << indent << token::END_BLOCK << decrIndent << nl;

    // Writing the surfaceElevationDict
    gauges.reset
    (
        new OFstream("waveGaugesNProbes/" + name + "surfaceElevationDict")
    );

    mesh_.objectRegistry::writeBanner(gauges());

    // Write the file information. Class name is not correct when
    // using wOut.writeHeader(os); hence manual entries
    gauges() << "FoamFile" << nl;
    gauges() << token::BEGIN_BLOCK << incrIndent << nl;
    gauges() << indent << "version" << tab << IOstream::currentVersion
             << token::END_STATEMENT << nl;
    gauges() << indent << "format" << tab << "ascii;" << nl;
    gauges() << indent << "class" << tab << "dictionary;" << nl;
    gauges() << indent << "object" << tab << "surfaceElevationDict;" << nl;
    gauges() << decrIndent << indent << token::END_BLOCK << nl;

    // Write the divider
    mesh_.objectRegistry::writeDivider(gauges());
    gauges() << nl << nl;

    gauges() << "setFormat           raw;" << nl;
    gauges() << "interpolationScheme cellPointFace;" << nl;
    gauges() << indent << "fields (" << "alpha1" << ");"
             << nl << nl;
    gauges() << "#includeIfPresent  \"../waveGaugesNProbes/" << name
             << "_sets\";" << nl;

    mesh_.objectRegistry::writeEndDivider(gauges());

    if (gaugeDict_.lookupOrDefault<Switch>("writeVTK", true))
    {
        writeVTKFormat(name, pp, addPoint);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
