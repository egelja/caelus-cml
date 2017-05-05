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
    CML::gnuplotGraph

Description
    Output in gnuplot (http://www.gnuplot.info) format

SourceFiles
    gnuplotGraph.cpp

\*---------------------------------------------------------------------------*/

#ifndef gnuplotGraph_H
#define gnuplotGraph_H

#include "graph.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class gnuplotGraph Declaration
\*---------------------------------------------------------------------------*/

class gnuplotGraph
:
    public graph::writer
{

public:

    //- Runtime type information
    TypeName("gnuplot");

    //- FileName extension  for this graph format
    static const word ext_;


    // Constructors

        //- Construct null
        gnuplotGraph()
        {}


    //- Destructor
    ~gnuplotGraph()
    {}


    // Member Functions

        // Access

            //- Return the appropriate fileName extension
            //  for this graph format
            const word& ext() const
            {
                return ext_;
            }


        // Write

            void write(const graph&, Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
