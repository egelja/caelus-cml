/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::fv::rotorDiskSource

Description
    Cell based momemtum source

    Source approximates the mean effects of rotor forces on a cylindrical
    region within the domain

    \heading Source usage

    Example usage:
    \verbatim
    rotorDiskSourceCoeffs
    {
        fieldNames      (U);    // names of fields on which to apply source
        nBlades         3;      // number of blades
        tipEffect       0.96;   // normalised radius above which lift = 0

        inletFlowType   local;  // inlet flow type specification

        geometryMode    auto;   // geometry specification

        refDirection    (-1 0 0); // reference direction
                                  // - used as reference for psi angle

        trimModel       fixed;  // fixed || targetForce

        flapCoeffs
        {
            beta0           0;  // coning angle [deg]
            beta1c          0;  // lateral flapping coeff (cos coeff)
            beta2s          0;  // longitudinal flapping coeff (sin coeff)
        }

        blade
        {
            ...
        }

        profiles
        {
            ...
        }
    }
    \endverbatim

    Where:
    Valid options for the \c geometryMode entry include:
    - auto          : determine rototor co-ord system from cells
    - specified     : specified co-ord system

    Valid options for the \c inletFlowType entry include:
    - fixed         : specified velocity
    - local         : use local flow conditions
    - surfaceNormal : specified normal velocity (positive towards rotor)


SourceFiles
    rotorDiskSource.cpp

\*---------------------------------------------------------------------------*/

#ifndef rotorDiskSource_H
#define rotorDiskSource_H

#include "fvOption.hpp"
#include "cylindricalCS.hpp"
#include "localAxesRotation.hpp"
#include "NamedEnum.hpp"
#include "bladeModel.hpp"
#include "profileModelList.hpp"
#include "volFieldsFwd.hpp"
#include "dimensionSet.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class trimModel;

namespace fv
{

/*---------------------------------------------------------------------------*\
                      Class rotorDiskSource Declaration
\*---------------------------------------------------------------------------*/

class rotorDiskSource
:
    public option
{
public:

    enum geometryModeType
    {
        gmAuto,
        gmSpecified
    };
    static const NamedEnum<geometryModeType, 2> geometryModeTypeNames_;

    enum inletFlowType
    {
        ifFixed,
        ifSurfaceNormal,
        ifLocal
    };
    static const NamedEnum<inletFlowType, 3> inletFlowTypeNames_;


protected:

    // Helper structures to encapsulate flap and trim data
    // Note: all input in degrees (converted to radians internally)

        struct flapData
        {
            scalar beta0;   // coning angle
            scalar beta1c;  // lateral flapping coeff (cos coeff)
            scalar beta2s;  // longitudinal flapping coeff (sin coeff)
        };


    // Protected data

        //- Reference density for incompressible case
        scalar rhoRef_;

        //- Rotational speed [rad/s]
        //  Positive anti-clockwise when looking along -ve lift direction
        scalar omega_;

        //- Number of blades
        label nBlades_;

        //- Inlet flow type
        inletFlowType inletFlow_;

        //- Inlet velocity for specified iinflow
        vector inletVelocity_;

        //- Tip effect [0-1]
        //  Ratio of blade radius beyond which lift=0
        scalar tipEffect_;

        //- Blade flap coefficients [rad/s]
        flapData flap_;

        //- Cell centre positions in local rotor frame
        //  (Cylindrical r, theta, z)
        List<point> x_;

        //- Rotation tensor for flap angle
        List<tensor> R_;

        //- Inverse rotation tensor for flap angle
        List<tensor> invR_;

        //- Area [m2]
        List<scalar> area_;

        //- Rotor local cylindrical co-ordinate system (r, theta, z)
        cylindricalCS coordSys_;

         //- Rotor transformation co-ordinate system
        autoPtr<localAxesRotation> localAxesRotation_;

        //- Maximum radius
        scalar rMax_;

        //- Trim model
        autoPtr<trimModel> trim_;

        //- Blade data
        bladeModel blade_;

        //- Profile data
        profileModelList profiles_;


    // Protected Member Functions

        //- Check data
        void checkData();

        //- Set the face areas per cell, and optionally correct the rotor axis
        void setFaceArea(vector& axis, const bool correct);

        //- Create the co-ordinate system
        void createCoordinateSystem();

        //- Construct geometry
        void constructGeometry();

        //- Return the inlet flow field
        tmp<vectorField> inflowVelocity(const volVectorField& U) const;

        //- Helper function to write rotor values
        template<class Type>
        void writeField
        (
            const word& name,
            const List<Type>& values,
            const bool writeNow = false
        ) const;


public:

    //- Runtime type information
    TypeName("rotorDisk");


    // Constructors


        //- Construct from components
        rotorDiskSource
        (
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );


    //- Destructor
    virtual ~rotorDiskSource();


    // Member Functions

        // Access

            //- Return the reference density for incompressible case
            inline scalar rhoRef() const;

            //- Return the rotational speed [rad/s]
            //  Positive anti-clockwise when looking along -ve lift direction
            inline scalar omega() const;

            //- Return the cell centre positions in local rotor frame
            // (Cylindrical r, theta, z)
            inline const List<point>& x() const;

            //- Return the rotor co-ordinate system (r, theta, z)
            inline const cylindricalCS& coordSys() const;


        // Evaluation

            //- Calculate forces
            template<class RhoFieldType>
            void calculate
            (
                const RhoFieldType& rho,
                const vectorField& U,
                const scalarField& thetag,
                vectorField& force,
                const bool divideVolume = true,
                const bool output = true
            ) const;


        // Source term addition

            //- Source term to momentum equation
            virtual void addSup
            (
                fvMatrix<vector>& eqn,
                const label fieldI
            );

            //- Source term to compressible momentum equation
            virtual void addSup
            (
                const volScalarField& rho,
                fvMatrix<vector>& eqn,
                const label fieldI
            );


        // I-O

            //- Write the source properties
            virtual void writeData(Ostream&) const;

            //- Read source dictionary
            virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "rotorDiskSourceI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
using namespace CML::constant;

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class RhoFieldType>
void CML::fv::rotorDiskSource::calculate
(
    const RhoFieldType& rho,
    const vectorField& U,
    const scalarField& thetag,
    vectorField& force,
    const bool divideVolume,
    const bool output
) const
{
    const scalarField& V = mesh_.V();

    // Logging info
    scalar dragEff = 0.0;
    scalar liftEff = 0.0;
    scalar AOAmin = GREAT;
    scalar AOAmax = -GREAT;

    forAll(cells_, i)
    {
        if (area_[i] > ROOTVSMALL)
        {
            const label cellI = cells_[i];

            const scalar radius = x_[i].x();

            // Transform velocity into local cylindrical reference frame
            vector Uc = localAxesRotation_->invTransform(U[cellI], i);

            // Transform velocity into local coning system
            Uc = R_[i] & Uc;

            // Set radial component of velocity to zero
            Uc.x() = 0.0;

            // Set blade normal component of velocity
            Uc.y() = radius*omega_ - Uc.y();

            // Determine blade data for this radius
            // i2 = index of upper radius bound data point in blade list
            scalar twist = 0.0;
            scalar chord = 0.0;
            label i1 = -1;
            label i2 = -1;
            scalar invDr = 0.0;
            blade_.interpolate(radius, twist, chord, i1, i2, invDr);

            // Flip geometric angle if blade is spinning in reverse (clockwise)
            scalar alphaGeom = thetag[i] + twist;
            if (omega_ < 0)
            {
                alphaGeom = mathematical::pi - alphaGeom;
            }

            // Effective angle of attack
            scalar alphaEff = alphaGeom - atan2(-Uc.z(), Uc.y());
            if (alphaEff > mathematical::pi)
            {
                alphaEff -= mathematical::twoPi;
            }
            if (alphaEff < -mathematical::pi)
            {
                alphaEff += mathematical::twoPi;
            }

            AOAmin = min(AOAmin, alphaEff);
            AOAmax = max(AOAmax, alphaEff);

            // Determine profile data for this radius and angle of attack
            const label profile1 = blade_.profileID()[i1];
            const label profile2 = blade_.profileID()[i2];

            scalar Cd1 = 0.0;
            scalar Cl1 = 0.0;
            profiles_[profile1].Cdl(alphaEff, Cd1, Cl1);

            scalar Cd2 = 0.0;
            scalar Cl2 = 0.0;
            profiles_[profile2].Cdl(alphaEff, Cd2, Cl2);

            scalar Cd = invDr*(Cd2 - Cd1) + Cd1;
            scalar Cl = invDr*(Cl2 - Cl1) + Cl1;

            // Apply tip effect for blade lift
            scalar tipFactor = neg(radius/rMax_ - tipEffect_);

            // Calculate forces perpendicular to blade
            scalar pDyn = 0.5*rho[cellI]*magSqr(Uc);

            scalar f = pDyn*chord*nBlades_*area_[i]/radius/mathematical::twoPi;
            vector localForce = vector(0.0, -f*Cd, tipFactor*f*Cl);

            // Accumulate forces
            dragEff += rhoRef_*localForce.y();
            liftEff += rhoRef_*localForce.z();

            // Transform force from local coning system into rotor cylindrical
            localForce = invR_[i] & localForce;

            // Transform force into global Cartesian co-ordinate system
            force[cellI] = localAxesRotation_->transform(localForce, i);

            if (divideVolume)
            {
                force[cellI] /= V[cellI];
            }
        }
    }

    if (output)
    {
        reduce(AOAmin, minOp<scalar>());
        reduce(AOAmax, maxOp<scalar>());
        reduce(dragEff, sumOp<scalar>());
        reduce(liftEff, sumOp<scalar>());

        Info<< type() << " output:" << nl
            << "    min/max(AOA)   = " << radToDeg(AOAmin) << ", "
            << radToDeg(AOAmax) << nl
            << "    Effective drag = " << dragEff << nl
            << "    Effective lift = " << liftEff << endl;
    }
}


template<class Type>
void CML::fv::rotorDiskSource::writeField
(
    const word& name,
    const List<Type>& values,
    const bool writeNow
) const
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    if (mesh_.time().outputTime() || writeNow)
    {
        tmp<fieldType> tfld
        (
            new fieldType
            (
                IOobject
                (
                    name,
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh_,
                dimensioned<Type>("zero", dimless, pTraits<Type>::zero)
            )
        );

        Field<Type>& fld = tfld().internalField();

        if (cells_.size() != values.size())
        {
            FatalErrorIn("") << "cells_.size() != values_.size()"
                << abort(FatalError);
        }

        forAll(cells_, i)
        {
            const label cellI = cells_[i];
            fld[cellI] = values[i];
        }

        tfld().write();
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
