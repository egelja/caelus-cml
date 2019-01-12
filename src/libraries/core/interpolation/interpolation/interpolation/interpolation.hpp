/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

Class
    CML::interpolation

Description
    Abstract base class for interpolation

\*---------------------------------------------------------------------------*/

#ifndef interpolation_H
#define interpolation_H

#include "faceList.hpp"
#include "volFieldsFwd.hpp"
#include "pointFields.hpp"
#include "typeInfo.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "tetIndices.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class polyMesh;

/*---------------------------------------------------------------------------*\
                           Class interpolation Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class interpolation
{

protected:

    // Protected data

        const GeometricField<Type, fvPatchField, volMesh>& psi_;

        const polyMesh& pMesh_;
        const vectorField& pMeshPoints_;
        const faceList& pMeshFaces_;
        const vectorField& pMeshFaceCentres_;
        const vectorField& pMeshFaceAreas_;


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            interpolation,
            dictionary,
            (
                const GeometricField<Type, fvPatchField, volMesh>& psi
            ),
            (psi)
        );


    // Selectors

        //- Return a reference to the specified interpolation scheme
        static autoPtr<interpolation<Type> > New
        (
            const word& interpolationType,
            const GeometricField<Type, fvPatchField, volMesh>& psi
        );

        //- Return a reference to the selected interpolation scheme
        static autoPtr<interpolation<Type> > New
        (
            const dictionary& interpolationSchemes,
            const GeometricField<Type, fvPatchField, volMesh>& psi
        );


    // Constructors

        //- Construct from components
        interpolation
        (
            const GeometricField<Type, fvPatchField, volMesh>& psi
        );


    //- Destructor
    virtual ~interpolation()
    {}


    // Member Functions

        //- Return the field to be interpolated
        const GeometricField<Type, fvPatchField, volMesh>& psi() const
        {
            return psi_;
        }

        //- Interpolate field to the given point in the given cell
        virtual Type interpolate
        (
            const vector& position,
            const label celli,
            const label facei = -1
        ) const = 0;

        //- Interpolate field to the given coordinates in the tetrahedron
        //  defined by the given indices.  Calls interpolate function
        //  above here execpt where overridden by derived
        //  interpolation types.
        virtual Type interpolate
        (
            const barycentric& coordinates,
            const tetIndices& tetIs,
            const label facei = -1
        ) const
        {
            return
                interpolate
                (
                    tetIs.tet(pMesh_).barycentricToPoint(coordinates),
                    tetIs.cell(),
                    facei
                );
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeInterpolationType(SS, Type)                                        \
                                                                               \
defineNamedTemplateTypeNameAndDebug(SS<Type>, 0);                              \
                                                                               \
interpolation<Type>::adddictionaryConstructorToTable<SS<Type> >                \
    add##SS##Type##ConstructorToTable_;


#define makeInterpolation(SS)                                                  \
                                                                               \
makeInterpolationType(SS, scalar)                                              \
makeInterpolationType(SS, vector)                                              \
makeInterpolationType(SS, sphericalTensor)                                     \
makeInterpolationType(SS, symmTensor)                                          \
makeInterpolationType(SS, tensor)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
#include "polyMesh.hpp"
#include "calculatedPointPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * //

template<class Type>
interpolation<Type>::interpolation
(
    const GeometricField<Type, fvPatchField, volMesh>& psi
)
:
    psi_(psi),
    pMesh_(psi.mesh()),
    pMeshPoints_(pMesh_.points()),
    pMeshFaces_(pMesh_.faces()),
    pMeshFaceCentres_(pMesh_.faceCentres()),
    pMeshFaceAreas_(pMesh_.faceAreas())
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
CML::autoPtr<CML::interpolation<Type> > CML::interpolation<Type>::New
(
    const word& interpolationType,
    const GeometricField<Type, fvPatchField, volMesh>& psi
)
{
    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(interpolationType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown interpolation type " << interpolationType
            << " for field " << psi.name() << nl << nl
            << "Valid interpolation types : " << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<interpolation<Type> >(cstrIter()(psi));
}


template<class Type>
CML::autoPtr<CML::interpolation<Type> > CML::interpolation<Type>::New
(
    const dictionary& interpolationSchemes,
    const GeometricField<Type, fvPatchField, volMesh>& psi
)
{
    return New(word(interpolationSchemes.lookup(psi.name())), psi);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
