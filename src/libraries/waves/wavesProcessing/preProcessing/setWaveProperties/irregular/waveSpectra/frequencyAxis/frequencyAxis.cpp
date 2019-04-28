/*---------------------------------------------------------------------------*\
Copyright  Niels Gjøl Jacobsen, Technical University of Denmark.
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

#include "frequencyAxis.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(frequencyAxis, 0);
defineRunTimeSelectionTable(frequencyAxis, frequencyAxis);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


frequencyAxis::frequencyAxis
(
    const Time& rT,
    dictionary& dict
)
:
    rT_(rT),
    dict_(dict)
{
    if (dict_.subDict("frequencyAxis").found("lowerFrequencyCutoff"))
    {
        fl_ = readScalar
            (
                dict_.subDict("frequencyAxis").lookup("lowerFrequencyCutoff")
            );
    }
    else
    {
        scalar Tp = readScalar(dict_.lookup("Tp"));
        fl_ = 0.3/Tp;
    }

    if (dict_.subDict("frequencyAxis").found("upperFrequencyCutoff"))
    {
        fu_ = readScalar
            (
                dict_.subDict("frequencyAxis").lookup("upperFrequencyCutoff")
            );
    }
    else
    {
        scalar Tp = readScalar(dict_.lookup("Tp"));
        fu_ = 3.0/Tp;
    }
}


frequencyAxis::~frequencyAxis()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


autoPtr<frequencyAxis> frequencyAxis::New
(
    const Time& rT,
    dictionary& dict
)
{
    word discretisation = dict.subDict("frequencyAxis").lookup("discretisation");

    frequencyAxisConstructorTable::iterator cstrIter =
            frequencyAxisConstructorTablePtr_->find(discretisation);

    if (cstrIter == frequencyAxisConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown discretisation method '" << discretisation << "'"
            << endl << endl
            << "Valid discretisation methods are:" << endl
            << frequencyAxisConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<frequencyAxis>(cstrIter()(rT, dict));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
