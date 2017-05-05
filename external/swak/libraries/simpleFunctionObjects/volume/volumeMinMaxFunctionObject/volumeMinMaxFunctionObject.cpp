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

#include "volumeMinMaxFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "volFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(volumeMinMaxFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        volumeMinMaxFunctionObject,
        dictionary
    );



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

volumeMinMaxFunctionObject::volumeMinMaxFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    volumeFieldFunctionObject(name,t,dict)
{
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

word volumeMinMaxFunctionObject::dirName()
{
    return typeName;
}

scalarField volumeMinMaxFunctionObject::process(const word& fieldName,scalar preset)
{
    return minMax(fieldName,preset);
}

Field<vector> volumeMinMaxFunctionObject::process(const word& fieldName,vector preset)
{
    return minMax(fieldName,preset);
}

Field<sphericalTensor> volumeMinMaxFunctionObject::process(const word& fieldName,sphericalTensor preset)
{
    return minMax(fieldName,preset);
}

Field<symmTensor> volumeMinMaxFunctionObject::process(const word& fieldName,symmTensor preset)
{
    return minMax(fieldName,preset);
}

Field<tensor> volumeMinMaxFunctionObject::process(const word& fieldName,tensor preset)
{
    return minMax(fieldName,preset);
}

stringList volumeMinMaxFunctionObject::columnNames()
{
    stringList result(2);
    result[0]="minimum";
    result[1]="maximum";
    return result;
}

} // namespace CML

// ************************************************************************* //
