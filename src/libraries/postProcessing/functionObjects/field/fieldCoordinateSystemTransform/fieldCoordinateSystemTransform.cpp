/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

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

#include "fieldCoordinateSystemTransform.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(CML::fieldCoordinateSystemTransform, 0);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fieldCoordinateSystemTransform::fieldCoordinateSystemTransform
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    name_(name),
    obr_(obr),
    active_(true),
    fieldSet_(),
    coordSys_(dict, obr)
{
    // Check if the available mesh is an fvMesh otherise deactivate
    if (!isA<fvMesh>(obr_))
    {
        active_ = false;
        WarningIn
        (
            "fieldCoordinateSystemTransform::fieldCoordinateSystemTransform"
            "("
                "const word&, "
                "const objectRegistry&, "
                "const dictionary&, "
                "const bool"
            ")"
        )   << "No fvMesh available, deactivating."
            << endl;
    }

    read(dict);

    Info<< type() << ":" << nl
        << "   Applying transformation from global Cartesian to local "
        << coordSys_ << nl << endl;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::fieldCoordinateSystemTransform::~fieldCoordinateSystemTransform()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::fieldCoordinateSystemTransform::read(const dictionary& dict)
{
    if (active_)
    {
        dict.lookup("fields") >> fieldSet_;
    }
}


void CML::fieldCoordinateSystemTransform::execute()
{
    // Do nothing
}


void CML::fieldCoordinateSystemTransform::end()
{
    // Do nothing
}


void CML::fieldCoordinateSystemTransform::write()
{
    forAll(fieldSet_, fieldI)
    {
        // If necessary load field
        transform<scalar>(fieldSet_[fieldI]);
        transform<vector>(fieldSet_[fieldI]);
        transform<sphericalTensor>(fieldSet_[fieldI]);
        transform<symmTensor>(fieldSet_[fieldI]);
        transform<tensor>(fieldSet_[fieldI]);
    }
}


// ************************************************************************* //
