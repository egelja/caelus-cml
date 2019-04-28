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
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "volumeFieldFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "volFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(volumeFieldFunctionObject, 0);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

volumeFieldFunctionObject::volumeFieldFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    timelineFunctionObject(name,t,dict),
    fieldNames_(0),
    scalarFields_(0),
    vectorFields_(0),
    sphericalTensorFields_(0),
    symmTensorFields_(0),
    tensorFields_(0)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void volumeFieldFunctionObject::write()
{
    processAndWrite<scalar>(scalarFields_);
    processAndWrite<vector>(vectorFields_);
    processAndWrite<sphericalTensor>(sphericalTensorFields_);
    processAndWrite<symmTensor>(symmTensorFields_);
    processAndWrite<tensor>(tensorFields_);
}

bool volumeFieldFunctionObject::start()
{
    fieldNames_ = wordList(dict_.lookup("fields"));

    boolList foundFields(fieldNames_.size(), false);
    findFields<volScalarField>(scalarFields_, foundFields);
    findFields<volVectorField>(vectorFields_, foundFields);
    findFields<volSphericalTensorField>(sphericalTensorFields_, foundFields);
    findFields<volSymmTensorField>(symmTensorFields_, foundFields);
    findFields<volTensorField>(tensorFields_, foundFields);

    label validFieldi=0;
    forAll(fieldNames_, fieldi)
    {
        if (foundFields[fieldi])
        {
            fieldNames_[validFieldi++] = fieldNames_[fieldi];
        }
        else
        {
            WarningInFunction
                << "Unknown field " << fieldNames_[fieldi]
                << " when reading dictionary " << dict_.name() << endl
                << "    Can only probe registered volScalar, volVector, "
                << "volSphericalTensor, volSymmTensor and volTensor fields"
                << nl << endl;
        }
    }

    fieldNames_.setSize(validFieldi);


    if (Pstream::master())
    {
        if (debug)
        {
            Pout<< "Probing fields:" << fieldNames_ << nl;
        }

        // Check if any fieldNames have been removed. If so close
        // the file.
    }

    timelineFunctionObject::start();

    return true;
}

wordList volumeFieldFunctionObject::fileNames()
{
    return fieldNames_;
}

} // namespace CML

// ************************************************************************* //
