/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::MRFZone

Description
    MRF zone definition based on cell zone and parameters
    obtained from a control dictionary constructed from the given stream.

    The rotation of the MRF region is defined by an origin and axis of
    rotation and an angular speed.

SourceFiles
    MRFZone.cpp

\*---------------------------------------------------------------------------*/

#ifndef MRFZone_HPP
#define MRFZone_HPP

#include "dictionary.hpp"
#include "wordReList.hpp"
#include "labelList.hpp"
#include "dimensionedScalar.hpp"
#include "dimensionedVector.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFields.hpp"
#include "fvMatricesFwd.hpp"
#include "mapPolyMesh.hpp"
#include "DataEntry.hpp"
#include "autoPtr.hpp"

namespace CML
{

// Forward declaration of classes
class fvMesh;

/*---------------------------------------------------------------------------*\
                           Class MRFZone Declaration
\*---------------------------------------------------------------------------*/

class MRFZone
{
    // Private data

        //- Reference to the mesh database
        const fvMesh& mesh_;

        //- Name of the MRF region
        const word name_;

        //- Coefficients dictionary
        dictionary coeffs_;

        //- MRF region active flag
        bool active_;

        //- Name of cell zone
        word cellZoneName_;

        //- Cell zone ID
        label cellZoneID_;

        const wordReList excludedPatchNames_;

        labelList excludedPatchLabels_;

        //- Internal faces that are part of MRF
        labelList internalFaces_;

        //- Outside faces (per patch) that move with the MRF
        labelListList includedFaces_;

        //- Excluded faces (per patch) that do not move with the MRF
        labelListList excludedFaces_;

        //- Origin of the axis
        const vector origin_;

        //- Axis vector
        vector axis_;

        //- Angular velocty (rad/sec)
        autoPtr<DataEntry<scalar> > omega_;


    // Private Member Functions

        //- Divide faces in frame according to patch
        void setMRFFaces();

        //- Make the given absolute mass/vol flux relative within the MRF region
        template<class RhoFieldType>
        void makeRelativeRhoFlux
        (
            const RhoFieldType& rho,
            surfaceScalarField& phi
        ) const;

        //- Make the given absolute mass/vol flux relative within the MRF region
        template<class RhoFieldType>
        void makeRelativeRhoFlux
        (
            const RhoFieldType& rho,
            FieldField<fvsPatchField, scalar>& phi
        ) const;

        //- Make the given absolute mass/vol flux relative within the MRF region
        template<class RhoFieldType>
        void makeRelativeRhoFlux
        (
            const RhoFieldType& rho,
            Field<scalar>& phi,
            const label patchi
        ) const;

        //- Make the given relative mass/vol flux absolute within the MRF region
        template<class RhoFieldType>
        void makeAbsoluteRhoFlux
        (
            const RhoFieldType& rho,
            surfaceScalarField& phi
        ) const;

        //- Disallow default bitwise copy construct
        MRFZone(const MRFZone&);

        //- Disallow default bitwise assignment
        void operator=(const MRFZone&);


public:

    // Declare name of the class and its debug switch
    ClassName("MRFZone");


    // Constructors

        //- Construct from fvMesh
        MRFZone
        (
            const word& name,
            const fvMesh& mesh,
            const dictionary& dict,
            const word& cellZoneName = word::null
        );

        //- Return clone
        autoPtr<MRFZone> clone() const
        {
            NotImplemented;
            return autoPtr<MRFZone>(nullptr);
        }


    // Member Functions

        //- Return const access to the MRF region name
        inline const word& name() const;

        //- Return const access to the MRF active flag
        inline bool active() const;

        //- Return the current Omega vector
        vector Omega() const;

        //- Update the mesh corresponding to given map
        void updateMesh(const mapPolyMesh& mpm)
        {
            // Only updates face addressing
            setMRFFaces();
        }

        //- Add the Coriolis force contribution to the acceleration field
        void addCoriolis
        (
            const volVectorField& U,
            volVectorField& ddtU
        ) const;

        //- Add the Coriolis force contribution to the momentum equation
        //  Adds to the lhs of the equation; optionally add to rhs
        void addCoriolis
        (
            fvVectorMatrix& UEqn,
            const bool rhs = false
        ) const;

        //- Add the Coriolis force contribution to the momentum equation
        //  Adds to the lhs of the equation; optionally add to rhs
        void addCoriolis
        (
            const volScalarField& rho,
            fvVectorMatrix& UEqn,
            const bool rhs = false
        ) const;

        //- Make the given absolute velocity relative within the MRF region
        void makeRelative(volVectorField& U) const;

        //- Make the given absolute flux relative within the MRF region
        void makeRelative(surfaceScalarField& phi) const;

        //- Make the given absolute boundary flux relative
        //  within the MRF region
        void makeRelative(FieldField<fvsPatchField, scalar>& phi) const;

        //- Make the given absolute patch flux relative
        //  within the MRF region
        void makeRelative(Field<scalar>& phi, const label patchi) const;

        //- Make the given absolute mass-flux relative within the MRF region
        void makeRelative
        (
            const surfaceScalarField& rho,
            surfaceScalarField& phi
        ) const;

        //- Make the given relative velocity absolute within the MRF region
        void makeAbsolute(volVectorField& U) const;

        //- Make the given relative flux absolute within the MRF region
        void makeAbsolute(surfaceScalarField& phi) const;

        //- Make the given relative mass-flux absolute within the MRF region
        void makeAbsolute
        (
            const surfaceScalarField& rho,
            surfaceScalarField& phi
        ) const;

        //- Correct the boundary velocity for the rotation of the MRF region
        void correctBoundaryVelocity(volVectorField& U) const;

        //- Zero the MRF region of the given field
        template<class Type>
        void zero(GeometricField<Type, fvsPatchField, surfaceMesh>& phi) const;

        //- Update MRFZone faces if the mesh topology changes
        void update();


    // I-O

        //- Write
        void writeData(Ostream& os) const;

        //- Read MRF dictionary
        bool read(const dictionary& dict);
};


} // End namespace CML

#include "fvMesh.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "fvMatrices.hpp"

template<class RhoFieldType>
void CML::MRFZone::makeRelativeRhoFlux
(
    const RhoFieldType& rho,
    surfaceScalarField& phi
) const
{
    if (!active_)
    {
        return;
    }

    const surfaceVectorField& Cf = mesh_.Cf();
    const surfaceVectorField& Sf = mesh_.Sf();

    const vector Omega = omega_->value(mesh_.time().timeOutputValue())*axis_;

    const vectorField& Cfi = Cf.internalField();
    const vectorField& Sfi = Sf.internalField();
    scalarField& phii = phi.internalField();

    // Internal faces
    forAll(internalFaces_, i)
    {
        label facei = internalFaces_[i];
        phii[facei] -= rho[facei]*(Omega ^ (Cfi[facei] - origin_)) & Sfi[facei];
    }

    makeRelativeRhoFlux(rho.boundaryField(), phi.boundaryField());
}


template<class RhoFieldType>
void CML::MRFZone::makeRelativeRhoFlux
(
    const RhoFieldType& rho,
    FieldField<fvsPatchField, scalar>& phi
) const
{
    if (!active_)
    {
        return;
    }

    const surfaceVectorField& Cf = mesh_.Cf();
    const surfaceVectorField& Sf = mesh_.Sf();

    const vector Omega = omega_->value(mesh_.time().timeOutputValue())*axis_;

    // Included patches
    forAll(includedFaces_, patchi)
    {
        forAll(includedFaces_[patchi], i)
        {
            label patchFacei = includedFaces_[patchi][i];

            phi[patchi][patchFacei] = 0.0;
        }
    }

    // Excluded patches
    forAll(excludedFaces_, patchi)
    {
        forAll(excludedFaces_[patchi], i)
        {
            label patchFacei = excludedFaces_[patchi][i];

            phi[patchi][patchFacei] -=
                rho[patchi][patchFacei]
              * (Omega ^ (Cf.boundaryField()[patchi][patchFacei] - origin_))
              & Sf.boundaryField()[patchi][patchFacei];
        }
    }
}


template<class RhoFieldType>
void CML::MRFZone::makeRelativeRhoFlux
(
    const RhoFieldType& rho,
    Field<scalar>& phi,
    const label patchi
) const
{
    if (!active_)
    {
        return;
    }

    const surfaceVectorField& Cf = mesh_.Cf();
    const surfaceVectorField& Sf = mesh_.Sf();

    const vector Omega = omega_->value(mesh_.time().timeOutputValue())*axis_;

    // Included patches
    forAll(includedFaces_[patchi], i)
    {
        label patchFacei = includedFaces_[patchi][i];

        phi[patchFacei] = 0.0;
    }

    // Excluded patches
    forAll(excludedFaces_[patchi], i)
    {
        label patchFacei = excludedFaces_[patchi][i];

        phi[patchFacei] -=
            rho[patchFacei]
          * (Omega ^ (Cf.boundaryField()[patchi][patchFacei] - origin_))
          & Sf.boundaryField()[patchi][patchFacei];
    }
}


template<class RhoFieldType>
void CML::MRFZone::makeAbsoluteRhoFlux
(
    const RhoFieldType& rho,
    surfaceScalarField& phi
) const
{
    if (!active_)
    {
        return;
    }

    const surfaceVectorField& Cf = mesh_.Cf();
    const surfaceVectorField& Sf = mesh_.Sf();

    const vector Omega = omega_->value(mesh_.time().timeOutputValue())*axis_;

    const vectorField& Cfi = Cf.internalField();
    const vectorField& Sfi = Sf.internalField();
    scalarField& phii = phi.internalField();

    // Internal faces
    forAll(internalFaces_, i)
    {
        label facei = internalFaces_[i];
        phii[facei] += rho[facei]*(Omega ^ (Cfi[facei] - origin_)) & Sfi[facei];
    }

    surfaceScalarField::GeometricBoundaryField& phibf = phi.boundaryField();


    // Included patches
    forAll(includedFaces_, patchi)
    {
        forAll(includedFaces_[patchi], i)
        {
            label patchFacei = includedFaces_[patchi][i];

            phibf[patchi][patchFacei] +=
                rho.boundaryField()[patchi][patchFacei]
              * (Omega ^ (Cf.boundaryField()[patchi][patchFacei] - origin_))
              & Sf.boundaryField()[patchi][patchFacei];
        }
    }

    // Excluded patches
    forAll(excludedFaces_, patchi)
    {
        forAll(excludedFaces_[patchi], i)
        {
            label patchFacei = excludedFaces_[patchi][i];

            phibf[patchi][patchFacei] +=
                rho.boundaryField()[patchi][patchFacei]
              * (Omega ^ (Cf.boundaryField()[patchi][patchFacei] - origin_))
              & Sf.boundaryField()[patchi][patchFacei];
        }
    }
}


template<class Type>
void CML::MRFZone::zero
(
    GeometricField<Type, fvsPatchField, surfaceMesh>& phi
) const
{
    if (!active_)
    {
        return;
    }

    Field<Type>& phii = phi.internalField();

    forAll(internalFaces_, i)
    {
        phii[internalFaces_[i]] = Zero;
    }

    typename GeometricField<Type, fvsPatchField, surfaceMesh>::
        GeometricBoundaryField& phibf = phi.boundaryField();

    forAll(includedFaces_, patchi)
    {
        forAll(includedFaces_[patchi], i)
        {
            phibf[patchi][includedFaces_[patchi][i]] = Zero;
        }
    }

    forAll(excludedFaces_, patchi)
    {
        forAll(excludedFaces_[patchi], i)
        {
            phibf[patchi][excludedFaces_[patchi][i]] = Zero;
        }
    }
}


#include "MRFZoneI.hpp"

#endif


