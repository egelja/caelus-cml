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

#include "gnuplotGraph.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(CML::gnuplotGraph, 0);
const CML::word CML::gnuplotGraph::ext_("gplt");

namespace CML
{
    typedef graph::writer graphWriter;
    addToRunTimeSelectionTable(graphWriter, gnuplotGraph, word);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::gnuplotGraph::write(const graph& g, Ostream& os) const
{
    os  << "#set term postscript color" << endl
        << "set output \"" << word(g.title()) << ".ps\"" << endl
        << "set title " << g.title() << " 0,0" << endl << "show title" << endl
        << "set xlabel " << g.xName() << " 0,0" << endl << "show xlabel" << endl
        << "set ylabel " << g.yName() << " 0,0" << endl << "show ylabel" << endl
        << "plot";

    bool firstField = true;

    forAllConstIter(graph, g, iter)
    {
        if (!firstField)
        {
            os << ',';
        }
        firstField = false;

        os  << "'-' title " << iter()->name() << " with lines";
    }
    os << "; pause -1" << endl;


    forAllConstIter(graph, g, iter)
    {
        os  << endl;
        writeXY(g.x(), *iter(), os);
    }
}


// ************************************************************************* //
