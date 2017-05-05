/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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
    SolverInfo

Description
    Determines the type of the Solver (whether it is 
    compressible/incompressible, turbulent etc) 

SourceFiles
    SolverInfo.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef SolverInfo_H
#define SolverInfo_H

#include "dictionary.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

    class objectRegistry;

/*---------------------------------------------------------------------------*\
                           Class SolverInfo Declaration
\*---------------------------------------------------------------------------*/

class SolverInfo
{
    // Private data

    word phi_;
    
    bool compressible_;

    bool turbulent_;

    bool les_;

    // Private Member Functions

    //- Disallow default bitwise copy construct
    SolverInfo(const SolverInfo&);

    //- Disallow default bitwise assignment
    void operator=(const SolverInfo&);

public:

    //- Construct from basic information and try to determine properties 
    SolverInfo(const objectRegistry &obr,const dictionary& dict);

    word phi() const { return phi_; }

    bool compressible() const { return compressible_; }

    bool turbulent() const { return turbulent_; }

    bool les() const { return les_; }

    void printInfo();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
