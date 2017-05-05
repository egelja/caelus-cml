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

#include "surfaceFilmModel.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    template<>
    const char* NamedEnum
    <
        regionModels::surfaceFilmModels::surfaceFilmModel::thermoModelType,
        2
    >::names[] =
    {
        "constant",
        "singleComponent"
    };
}

const CML::NamedEnum
<
    CML::regionModels::surfaceFilmModels::surfaceFilmModel::thermoModelType,
    2
>
CML::regionModels::surfaceFilmModels::surfaceFilmModel::thermoModelTypeNames_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(surfaceFilmModel, 0);
defineRunTimeSelectionTable(surfaceFilmModel, mesh);

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

bool surfaceFilmModel::read()
{
    if (singleLayerRegion::read())
    {
        thermoModel_ =
            thermoModelTypeNames_.read(coeffs_.lookup("thermoModel"));
        return true;
    }
    else
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

surfaceFilmModel::surfaceFilmModel
(
    const word& modelType,
    const fvMesh& mesh,
    const dimensionedVector& g
)
:
    singleLayerRegion(mesh, "surfaceFilm", modelType),
    g_(g),
    thermoModel_(tmConstant)
{
    if (active_)
    {
        read();
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

surfaceFilmModel::~surfaceFilmModel()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::scalar surfaceFilmModel::CourantNumber() const
{
    return ROOTVSMALL;
}


tmp<DimensionedField<scalar, volMesh> > surfaceFilmModel::Srho() const
{
    notImplemented
    (
        "tmp<DimensionedField<scalar, volMesh> > surfaceFilmModel::Srho() const"
    )

    return tmp<DimensionedField<scalar, volMesh> >(NULL);
}


tmp<DimensionedField<scalar, volMesh> >
surfaceFilmModel::Srho(const label) const
{
    notImplemented
    (
        "tmp<DimensionedField<scalar, volMesh> > surfaceFilmModel::Srho"
        "(const label) const"
    )

    return tmp<DimensionedField<scalar, volMesh> >(NULL);
}


tmp<DimensionedField<scalar, volMesh> > surfaceFilmModel::Sh() const
{
    notImplemented
    (
        "tmp<DimensionedField<scalar, volMesh> > surfaceFilmModel::Sh() const"
    )

    return tmp<DimensionedField<scalar, volMesh> >(NULL);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// ************************************************************************* //
