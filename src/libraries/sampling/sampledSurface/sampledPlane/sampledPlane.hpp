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

Class
    CML::sampledPlane

Description
    A sampledSurface defined by a cuttingPlane. Always triangulated.

Note
    Does not actually cut until update() called.


\*---------------------------------------------------------------------------*/

#ifndef sampledPlane_H
#define sampledPlane_H

#include "sampledSurface.hpp"
#include "cuttingPlane.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class sampledPlane Declaration
\*---------------------------------------------------------------------------*/

class sampledPlane
:
    public sampledSurface,
    public cuttingPlane
{
    // Private data

        //- If restricted to zones, name of this zone or a regular expression
        keyType zoneKey_;

        //- Track if the surface needs an update
        mutable bool needsUpdate_;

    // Private Member Functions

        //- sample field on faces
        template <class Type>
        tmp<Field<Type> > sampleField
        (
            const GeometricField<Type, fvPatchField, volMesh>& vField
        ) const;


        template <class Type>
        tmp<Field<Type> >
        interpolateField(const interpolation<Type>&) const;


public:

    //- Runtime type information
    TypeName("sampledPlane");


    // Constructors

        //- Construct from components
        sampledPlane
        (
            const word& name,
            const polyMesh& mesh,
            const plane& planeDesc,
            const keyType& zoneKey = word::null
        );

        //- Construct from dictionary
        sampledPlane
        (
            const word& name,
            const polyMesh& mesh,
            const dictionary& dict
        );


    //- Destructor
    virtual ~sampledPlane();


    // Member Functions

        //- Does the surface need an update?
        virtual bool needsUpdate() const;

        //- Mark the surface as needing an update.
        //  May also free up unneeded data.
        //  Return false if surface was already marked as expired.
        virtual bool expire();

        //- Update the surface as required.
        //  Do nothing (and return false) if no update was needed
        virtual bool update();


        //- Points of surface
        virtual const pointField& points() const
        {
            return cuttingPlane::points();
        }

        //- Faces of surface
        virtual const faceList& faces() const
        {
            return cuttingPlane::faces();
        }

        //- For every face original cell in mesh
        const labelList& meshCells() const
        {
            return cuttingPlane::cutCells();
        }

        //- sample field on surface
        virtual tmp<scalarField> sample
        (
            const volScalarField&
        ) const;


        //- sample field on surface
        virtual tmp<vectorField> sample
        (
            const volVectorField&
        ) const;

        //- sample field on surface
        virtual tmp<sphericalTensorField> sample
        (
            const volSphericalTensorField&
        ) const;

        //- sample field on surface
        virtual tmp<symmTensorField> sample
        (
            const volSymmTensorField&
        ) const;

        //- sample field on surface
        virtual tmp<tensorField> sample
        (
            const volTensorField&
        ) const;


        //- interpolate field on surface
        virtual tmp<scalarField> interpolate
        (
            const interpolation<scalar>&
        ) const;


        //- interpolate field on surface
        virtual tmp<vectorField> interpolate
        (
            const interpolation<vector>&
        ) const;

        //- interpolate field on surface
        virtual tmp<sphericalTensorField> interpolate
        (
            const interpolation<sphericalTensor>&
        ) const;

        //- interpolate field on surface
        virtual tmp<symmTensorField> interpolate
        (
            const interpolation<symmTensor>&
        ) const;

        //- interpolate field on surface
        virtual tmp<tensorField> interpolate
        (
            const interpolation<tensor>&
        ) const;

        //- Write
        virtual void print(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template <class Type>
CML::tmp<CML::Field<Type> >
CML::sampledPlane::sampleField
(
    const GeometricField<Type, fvPatchField, volMesh>& vField
) const
{
    return tmp<Field<Type> >(new Field<Type>(vField, meshCells()));
}


template <class Type>
CML::tmp<CML::Field<Type> >
CML::sampledPlane::interpolateField
(
    const interpolation<Type>& interpolator
) const
{
    // One value per point
    tmp<Field<Type> > tvalues(new Field<Type>(points().size()));
    Field<Type>& values = tvalues();

    boolList pointDone(points().size(), false);

    forAll(faces(), cutFaceI)
    {
        const face& f = faces()[cutFaceI];

        forAll(f, faceVertI)
        {
            label pointI = f[faceVertI];

            if (!pointDone[pointI])
            {
                values[pointI] = interpolator.interpolate
                (
                    points()[pointI],
                    meshCells()[cutFaceI]
                );
                pointDone[pointI] = true;
            }
        }
    }

    return tvalues;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
