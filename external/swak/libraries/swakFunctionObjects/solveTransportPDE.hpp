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
    solveTransportPDE

Description
    Function object that solves the transport equation

SourceFiles
    solveTransportPDE.cpp

Contributors/Copyright:
    2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef solveTransportPDE_H
#define solveTransportPDE_H

#include "solvePDECommonFiniteVolume.hpp"

namespace CML
{

class solveTransportPDE
:
    public solvePDECommonFiniteVolume
{
    //- Disallow default bitwise copy construct
    solveTransportPDE(const solveTransportPDE&);

    //- Disallow default bitwise assignment
    void operator=(const solveTransportPDE&);

protected:

    //- the coefficent for the time-derivative
    exprString rhoExpression_;
    dimensionSet rhoDimension_;

    //- the coefficient for the laplacian term
    exprString diffusionExpression_;
    dimensionSet diffusionDimension_;

    //- the name of the phi-field.
    exprString phiExpression_;
    dimensionSet phiDimension_;

    //- the right hand side
    exprString sourceExpression_;
    dimensionSet sourceDimension_;

    //- the right hand side
    exprString sourceImplicitExpression_;
    dimensionSet sourceImplicitDimension_;

    //- do the actual solving
    void solve();

public:

    //- Runtime type information
    TypeName("solveTransportPDE");


    // Constructors

    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    solveTransportPDE
    (
        const word& name,
        const objectRegistry&,
        const dictionary&,
        const bool loadFromFiles = false
    );


    //- Destructor
    virtual ~solveTransportPDE();


    // Member Functions

    //- Read the forces data
    virtual void read(const dictionary&);

};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif
