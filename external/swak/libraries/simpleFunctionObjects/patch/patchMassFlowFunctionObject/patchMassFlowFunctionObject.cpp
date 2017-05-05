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

Contributors/Copyright:
    2008-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "patchMassFlowFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "IOmanip.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(patchMassFlowFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        patchMassFlowFunctionObject,
        dictionary
    );



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

patchMassFlowFunctionObject::patchMassFlowFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    patchFunctionObject(name,t,dict),
    solver_(obr_,dict)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void patchMassFlowFunctionObject::write()
{
    scalarField vals(patchNames_.size(), -GREAT);
   
    const surfaceScalarField &phi=obr_.lookupObject<surfaceScalarField>(solver_.phi());

    forAll(patchIndizes_,i) {
        label patchI=patchIndizes_[i];

        if(patchI>=0) {
            vals[i]=sum(
                phi.boundaryField()[patchI]
            );
            reduce(vals[i],sumOp<scalar>());
        }
    }    

    forAll(vals,i) {
        vals[i]*=factor();
    }

    if (Pstream::master())
    {
        writeTime("massFlow",phi.time().value());
        writeData("massFlow",vals);
        endData("massFlow");
    }

    if(verbose()) {
        Info<< regionString() 
            << " MassFlows: ";

        forAll(patchNames_, patchI)
        {
            Info << "  " << patchNames_[patchI] << " = " 
                << vals[patchI];
        }

        Info << endl;
    }
    
}

    //- Names of the files
wordList patchMassFlowFunctionObject::fileNames()
{
    return wordList(1,"massFlow");
}

word patchMassFlowFunctionObject::dirName()
{
    return word("patchMassFlows");
}


} // namespace CML

// ************************************************************************* //
