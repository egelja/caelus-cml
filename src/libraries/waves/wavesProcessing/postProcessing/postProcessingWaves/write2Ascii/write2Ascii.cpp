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

#include "write2Ascii.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(write2Ascii, 0);
addToRunTimeSelectionTable
(
    postProcessingWaves,
    write2Ascii,
    postProcessingWaves
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


write2Ascii::write2Ascii
(
    const Time& rT,
    const dictionary& actionProp,
    const word& action
)
:
    postProcessingWaves(rT, actionProp, action)
{
}


write2Ascii::~write2Ascii()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void write2Ascii::evaluate()
{
    // Writing the time instances to ascii file
    IOField<scalar> time
    (
        IOobject
        (
            callName_ + "_time",
            rT_.constant(),
            addDir_,
            rT_,
            IOobject::MUST_READ
        )
    );

    autoPtr<OFstream> asciiPtr_;
    asciiPtr_.reset
    (
        new OFstream(directDir_ + "/" + callName_ + "_time.txt")
    );

    forAll (time, timei)
    {
        asciiPtr_() << time[timei] << endl;
    }

    // Writing the data fields
    label count(0);

    while (true)
    {
        std::stringstream ss;
        ss << callName_ << "_" << count;

        IOobject fileHeader
        (
            ss.str(),
            rT_.constant(),
            addDir_,
            rT_,
            IOobject::MUST_READ
        );

        if (!fileHeader.headerOk())
        {
            break;
        }

        asciiPtr_.reset(new OFstream(directDir_ + "/" + ss.str() + ".txt"));

        if (fileHeader.headerClassName() == "scalarField")
        {
            IOField<scalar> field(fileHeader);

            forAll (field, datai)
            {
                asciiPtr_() << field[datai] << endl;
            }
        }
        else if (fileHeader.headerClassName() == "vectorField")
        {
            IOField<vector> field(fileHeader);

            forAll (field, datai)
            {
                asciiPtr_() << field[datai].x() << tab << field[datai].y()
                            << tab << field[datai].z() << endl;
            }
        }
        else
        {
            FatalErrorInFunction
                    << "The field type" << fileHeader.headerClassName() << endl
                    << "is not supported." << endl << exit(FatalError);
        }

        count++;
    }

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

    // Writing time step
    scalar dt = readScalar(dict.lookup("deltaT"));

    asciiPtr_.reset
    (
        new OFstream(directDir_ + "/" + callName_ + "_deltaT.txt")
    );

    asciiPtr_() << dt << endl;

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
