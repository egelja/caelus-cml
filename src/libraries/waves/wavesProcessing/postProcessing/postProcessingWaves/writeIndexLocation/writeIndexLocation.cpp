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

#include "writeIndexLocation.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(writeIndexLocation, 0);
addToRunTimeSelectionTable
(
    postProcessingWaves,
    writeIndexLocation,
    postProcessingWaves
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


writeIndexLocation::writeIndexLocation
(
    const Time& rT,
    const dictionary& actionProp,
    const word& action
)
:
    postProcessingWaves(rT, actionProp, action)
{
}


writeIndexLocation::~writeIndexLocation()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void writeIndexLocation::evaluate()
{
    autoPtr<OFstream> asciiPtr_;

    // Writing the content of the dictionary
    IOdictionary dict
    (
        IOobject
        (
            callName_ + "_dict",
            rT_.constant(),
            addDir_,
            rT_,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    // Writing indices and spatial coordinates
    labelList indices(dict.lookup("index"));

    // Either locations or names are stated in the dictionary
    if (dict.found("x"))
    {
        scalarField x(dict.lookup("x"));
        scalarField y(dict.lookup("y"));
        scalarField z(dict.lookup("z"));

        asciiPtr_.reset
        (
            new OFstream(directDir_ + "/" + callName_ + "_indexXYZ.txt")
        );

        forAll (indices, indexi)
        {
            asciiPtr_() << indices[indexi] << tab << x[indexi] << tab
                        << y[indexi] << tab << z[indexi] << endl;
        }
    }
    else
    {
        wordList names(dict.lookup("names"));

        asciiPtr_.reset
        (
            new OFstream(directDir_ + "/" + callName_ + "_indexNames.txt")
        );

        forAll (indices, indexi)
        {
            asciiPtr_() << indices[indexi] << tab << names[indexi] << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
