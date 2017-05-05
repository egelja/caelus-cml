/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#ifndef MRFZone_HPP
#define MRFZone_HPP

#include "dictionary.hpp"
#include "wordList.hpp"
#include "labelList.hpp"
#include "dimensionedScalar.hpp"
#include "dimensionedVector.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "fvMatricesFwd.hpp"
#include "mapPolyMesh.hpp"
#include "DataEntry.hpp"
#include "autoPtr.hpp"

namespace CML
{

// Forward declaration of classes
class fvMesh;

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

        const wordList excludedPatchNames_;
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
        void relativeRhoFlux
        (
            const RhoFieldType& rho,
            surfaceScalarField& phi
        ) const;

        //- Make the given relative mass/vol flux absolute within the MRF region
        template<class RhoFieldType>
        void absoluteRhoFlux
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
            notImplemented("autoPtr<MRFZone> clone() const");
            return autoPtr<MRFZone>(NULL);
        }


    // Member Functions

        // Access

            //- Return const access to the MRF region name
            inline const word& name() const;

            //- Return const access to the MRF active flag
            inline bool active() const;

            //- Return the current Omega vector
            vector Omega() const;


        // Evaluation

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
            void relativeVelocity(volVectorField& U) const;

            //- Make the given relative velocity absolute within the MRF region
            void absoluteVelocity(volVectorField& U) const;

            //- Make the given absolute flux relative within the MRF region
            void relativeFlux(surfaceScalarField& phi) const;

            //- Make the given absolute mass-flux relative within the MRF region
            void relativeFlux
            (
                const surfaceScalarField& rho,
                surfaceScalarField& phi
            ) const;

            //- Make the given relative flux absolute within the MRF region
            void absoluteFlux(surfaceScalarField& phi) const;

            //- Make the given relative mass-flux absolute within the MRF region
            void absoluteFlux
            (
                const surfaceScalarField& rho,
                surfaceScalarField& phi
            ) const;

            //- Correct the boundary velocity for the roation of the MRF region
            void correctBoundaryVelocity(volVectorField& U) const;


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
void CML::MRFZone::relativeRhoFlux
(
    const RhoFieldType& rho,
    surfaceScalarField& phi
) const
{
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

    // Included patches
    forAll(includedFaces_, patchi)
    {
        forAll(includedFaces_[patchi], i)
        {
            label patchFacei = includedFaces_[patchi][i];

            phi.boundaryField()[patchi][patchFacei] = 0.0;
        }
    }

    // Excluded patches
    forAll(excludedFaces_, patchi)
    {
        forAll(excludedFaces_[patchi], i)
        {
            label patchFacei = excludedFaces_[patchi][i];

            phi.boundaryField()[patchi][patchFacei] -=
                rho.boundaryField()[patchi][patchFacei]
              * (Omega ^ (Cf.boundaryField()[patchi][patchFacei] - origin_))
              & Sf.boundaryField()[patchi][patchFacei];
        }
    }
}


template<class RhoFieldType>
void CML::MRFZone::absoluteRhoFlux
(
    const RhoFieldType& rho,
    surfaceScalarField& phi
) const
{
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

    // Included patches
    forAll(includedFaces_, patchi)
    {
        forAll(includedFaces_[patchi], i)
        {
            label patchFacei = includedFaces_[patchi][i];

            phi.boundaryField()[patchi][patchFacei] +=
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

            phi.boundaryField()[patchi][patchFacei] +=
                rho.boundaryField()[patchi][patchFacei]
              * (Omega ^ (Cf.boundaryField()[patchi][patchFacei] - origin_))
              & Sf.boundaryField()[patchi][patchFacei];
        }
    }
}


#include "MRFZoneI.hpp"

#endif


