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

#include "removeData.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(removeData, 0);
addToRunTimeSelectionTable
(
    postProcessingWaves,
    removeData,
    postProcessingWaves
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


removeData::removeData
(
    const Time& rT,
    const dictionary& actionProp,
    const word& action
)
:
    postProcessingWaves(rT, actionProp, action),

    removeOF_(actionProperties_.lookupOrDefault<Switch>("removeOF", true )),

    removeAscii_
    (
        actionProperties_.lookupOrDefault<Switch>("removeAscii", false)
    )
{
}


removeData::~removeData()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void removeData::evaluate()
{
    fileName fn( directDir_ );

    // Remove the files with a OF-data format
    // Removal of OF-data formats are done as default
    if (removeOF_)
    {
        Info << "        - Removing data in OF-data format" << endl;

        // Remove the time instances
        if (exists(fn + callName_ + "_time"))
        {
            CML::rm(fn + callName_ + "_time");
        }

        // Remove the dictionary file
        if (exists(fn + callName_ + "_dict"))
        {
            CML::rm(fn + callName_ + "_dict");
        }

        // Remove all the data sets for each probe/gauge/etc
        label count = 0;
        do
        {
            std::stringstream ss;
            ss << callName_ << "_" << count++;

            if (!exists(fn + ss.str()))
            {
                break;
            }

            CML::rm(fn + ss.str());
        }
        while (true);
    }

    // Removal of ascii formatted data is only done per request.
    // The Switch "removeAscii_" is by default set to "false".
    if (removeAscii_)
    {
        Info << "        - Removing data in ascii format" << endl;

        fileNameList fnl = CML::readDir(fn, CML::fileName::FILE);

        forAll (fnl, fi)
        {
            label N = fnl[fi].size();
            if (fnl[fi].substr(N - 3,N) == "txt")
            {
                CML::rm(fn + fnl[fi]);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
