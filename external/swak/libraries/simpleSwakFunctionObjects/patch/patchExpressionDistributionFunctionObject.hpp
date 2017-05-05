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
    patchExpressionDistributionFunctionObject

Description
    Distribution of an expression on a patch

SourceFiles
    patchExpressionDistributionFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef patchExpressionDistributionFunctionObject_H
#define patchExpressionDistributionFunctionObject_H

#include "distributionFunctionObject.hpp"
#include "PatchValueExpressionDriver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class patchExpressionDistributionFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class patchExpressionDistributionFunctionObject
:
    public distributionFunctionObject
{

    // Private Member Functions

    //- Disallow default bitwise copy construct
    patchExpressionDistributionFunctionObject(const patchExpressionDistributionFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const patchExpressionDistributionFunctionObject&);

    wordList patchNames_;

    labelList patchIDs_;

    template <typename T>
    void getDistributionInternal(autoPtr<SimpleDistribution<T> > &dist);
protected:

    word dirName();

    virtual word baseName();

    virtual void getDistribution();

    //- the expression that is to be evaluated
    exprString expression_;

    //- the specification
    dictionary data_;

    PtrList<PatchValueExpressionDriver> drivers_;

public:

    //- Runtime type information
    TypeName("patchExpressionDistribution");


    // Constructors

    //- Construct from components
    patchExpressionDistributionFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    bool start();

    void write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "fieldDistributionFunctionObject.hpp"
#include "volFields.hpp"
#include "IOmanip.hpp"
#include "fvMesh.hpp"
#include "fvCFD.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <typename T>
void patchExpressionDistributionFunctionObject::getDistributionInternal(
    autoPtr<SimpleDistribution<T> > &dist
) {
    if(drivers_[0].getResultType()!=pTraits<T>::typeName) {
        return;
    }
    bool firstTime=true;

    forAll(patchIDs_,i) {
        autoPtr<SimpleDistribution<T> > partial;

        partial=setDataScalar(
            drivers_[i].getResult<T>()(),
            drivers_[i].patch().magSf()
        );

        if(partial.valid()) {
            if(firstTime) {
                firstTime=false;
                dist=partial;
            } else {
                SimpleDistribution<T> &d=dist();
                d=d+partial();
            }
        }
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
