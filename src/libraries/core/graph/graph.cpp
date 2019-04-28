/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "graph.hpp"
#include "OFstream.hpp"
#include "IOmanip.hpp"
#include "Pair.hpp"
#include "OSspecific.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    typedef graph::writer graphWriter;
    defineTypeNameAndDebug(graphWriter, 0);
    defineRunTimeSelectionTable(graphWriter, word);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::graph::readCurves(Istream& is)
{
    List<xy> xyData(is);

    x_.setSize(xyData.size());
    scalarField y(xyData.size());

    forAll(xyData, i)
    {
        x_[i] = xyData[i].x_;
        y[i] = xyData[i].y_;
    }

    insert(yName_, new curve(yName_, curve::curveStyle::CONTINUOUS, y));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::graph::graph
(
    const string& title,
    const string& xName,
    const string& yName,
    const scalarField& x
)
:
    title_(title),
    xName_(xName),
    yName_(yName),
    x_(x)
{}


CML::graph::graph
(
    const string& title,
    const string& xName,
    const string& yName,
    const scalarField& x,
    const scalarField& y
)
:
    title_(title),
    xName_(xName),
    yName_(yName),
    x_(x)
{
    insert(yName, new curve(yName, curve::curveStyle::CONTINUOUS, y));
}


CML::graph::graph
(
    const string& title,
    const string& xName,
    const string& yName,
    Istream& is
)
:
    title_(title),
    xName_(xName),
    yName_(yName)
{
    readCurves(is);
}


CML::graph::graph(Istream& is)
:
    title_(is),
    xName_(is),
    yName_(is)
{
    readCurves(is);
}


const CML::scalarField& CML::graph::y() const
{
    if (size() != 1)
    {
        FatalErrorInFunction
            << "y field requested for graph containing " << size()
            << "ys" << exit(FatalError);
    }

    return *begin()();
}


CML::scalarField& CML::graph::y()
{
    if (size() != 1)
    {
        FatalErrorInFunction
            << "y field requested for graph containing " << size()
            << "ys" << exit(FatalError);
    }

    return *begin()();
}


CML::autoPtr<CML::graph::writer> CML::graph::writer::New
(
    const word& graphFormat
)
{
    if (!wordConstructorTablePtr_)
    {
        FatalErrorInFunction
            << "Graph writer table is empty"
            << exit(FatalError);
    }

    wordConstructorTable::iterator cstrIter =
        wordConstructorTablePtr_->find(graphFormat);

    if (cstrIter == wordConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown graph format " << graphFormat
            << endl << endl
            << "Valid graph formats are : " << endl
            << wordConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<graph::writer>(cstrIter()());
}


void CML::graph::writer::writeXY
(
    const scalarField& x,
    const scalarField& y,
    Ostream& os
) const
{
    forAll(x, xi)
    {
        os << setw(10) << x[xi] << token::SPACE << setw(10) << y[xi]<< endl;
    }
}


void CML::graph::writeTable(Ostream& os) const
{
    forAll(x_, xi)
    {
        os  << setw(10) << x_[xi];

        forAllConstIter(graph, *this, iter)
        {
            os  << token::SPACE << setw(10) << (*iter())[xi];
        }
        os  << endl;
    }
}


void CML::graph::write(Ostream& os, const word& format) const
{
    writer::New(format)().write(*this, os);
}


void CML::graph::write(const fileName& pName, const word& format) const
{
    autoPtr<writer> graphWriter(writer::New(format));

    OFstream graphFile(pName + '.' + graphWriter().ext());

    if (graphFile.good())
    {
        write(graphFile, format);
    }
    else
    {
        WarningInFunction
            << "Could not open graph file " << graphFile.name()
            << endl;
    }
}


void CML::graph::write
(
    const fileName& path,
    const word& name,
    const word& format
) const
{
    mkDir(path);
    write(path/name, format);
}


CML::Ostream& CML::operator<<(Ostream& os, const graph& g)
{
    g.writeTable(os);
    os.check("Ostream& operator<<(Ostream&, const graph&)");
    return os;
}


// ************************************************************************* //
