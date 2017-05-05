/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
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
    CML::randomExponentialPluginFunction

Description

SourceFiles
    randomExponentialPluginFunction.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef randomExponentialPluginFunction_H
#define randomExponentialPluginFunction_H

#include "CommonPluginFunction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class randomExponentialPluginFunction Declaration
\*---------------------------------------------------------------------------*/

template <typename FType,typename DType>
class randomExponentialPluginFunction
:
    public FType
{
    //- Disallow default bitwise assignment
    void operator=(const randomExponentialPluginFunction &);

    randomExponentialPluginFunction(const randomExponentialPluginFunction &);

    //- the halfLife of the exponential distribution
    scalar halfLife_;

    //- the seed for the random number generator
    label seed_;

    //- do the actual compuation
    void doEvaluationInternal(scalarField &f);

public:
    randomExponentialPluginFunction(
        const DType &parentDriver,
        const word &name
    );

    virtual ~randomExponentialPluginFunction() {}

    TypeName("randomExponentialPluginFunction");

    void doEvaluation();

    void setArgument(label index,const scalar &);

    void setArgument(label index,const label &);
};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
