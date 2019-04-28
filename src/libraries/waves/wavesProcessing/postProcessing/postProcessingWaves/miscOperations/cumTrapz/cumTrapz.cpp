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

#include "cumTrapz.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(cumTrapz, 0);
addToRunTimeSelectionTable(postProcessingWaves, cumTrapz, postProcessingWaves);

// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //


void cumTrapz::evaluateScalar()
{
    Info << "        - Cumulative trapezoidal integration" << endl;

    List<scalarField> input = readScalarFields(indices_);
    scalarField time = readIOScalarField(callName_ + "_time");

    List<scalarField> ct(input.size());

    forAll (input, I)
    {
        const scalarField& field(input[I]);
        scalarField& out( ct[ I ] );
        out.setSize(time.size(), 0.0);

        for (int i=1; i < field.size(); i++)
        {
            out[i] = out[i - 1]
                + 0.5*(time[i] - time[i - 1])*(field[i] + field[i - 1]);
        }
    }

    writeScalar(ct);
}


void cumTrapz::writeScalar
(
    const List<scalarField>& ct
)
{
    Info << "        - Writing cumulative trapezoidal integral: "
         << directDir_.c_str() << this->type() << endl;

    mkDir(directDir_ + this->type());

    autoPtr<OFstream> spectrumPtr_;

    forAll (indices_, indexi)
    {
        std::stringstream ss;
        ss << callName_ << "_" << indices_[indexi];

        spectrumPtr_.reset
        (
            new OFstream
            (
                directDir_ + "/" + this->type() + "/"
              + ss.str() + "_cumTrapz.dat"
            )
        );

        const scalarField& data(ct[indexi]);

        forAll (data, ii)
        {
            spectrumPtr_() << data[ii] << endl;
        }
    }
}


void cumTrapz::evaluateVector()
{
    Info << "        - Cumulative trapezoidal integration" << endl;

    List<vectorField> input = readVectorFields(indices_);
    scalarField time = readIOScalarField(callName_ + "_time");

    List<vectorField> ct(input.size());

    forAll (input, I)
    {
        const vectorField& field(input[I]);
        vectorField& out( ct[ I ] );
        out.setSize(time.size(), vector::zero);

        for (int i=1; i < field.size(); i++)
        {
            out[i] = out[i - 1]
                + 0.5*(time[i] - time[i - 1])*(field[i] + field[i - 1]);
        }
    }

    writeVector( ct );
}


void cumTrapz::writeVector
(
    const List<vectorField>& ct
)
{
    Info << "        - Writing cumulative trapezoidal integral: "
         << directDir_.c_str() << this->type() << endl;

    mkDir(directDir_ + this->type());

    autoPtr<OFstream> spectrumPtr_;

    forAll (indices_, indexi)
    {
        std::stringstream ss;
        ss << callName_ << "_" << indices_[indexi];

        spectrumPtr_.reset
        (
            new OFstream
            (
                directDir_ + "/" + this->type() + "/"
              + ss.str() + "_cumTrapz.dat"
            )
        );

        const vectorField& data(ct[indexi]);

        forAll (data, ii)
        {
            spectrumPtr_() << data[ii].x() << tab << data[ii].y() << tab
                           << data[ii].z() << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


cumTrapz::cumTrapz
(
    const Time& rT,
    const dictionary& actionProp,
    const word& action
)
:
    postProcessingWaves(rT, actionProp, action),

#   include "../../dataDict.hpp"
{
    readIndices(dataDict_, indices_);
}


cumTrapz::~cumTrapz()
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void cumTrapz::evaluate()
{
    if (dataType() == "scalar")
    {
        evaluateScalar();
    }
    else if (dataType() == "vector")
    {
        evaluateVector();
    }
    else
    {
        NotImplemented;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
