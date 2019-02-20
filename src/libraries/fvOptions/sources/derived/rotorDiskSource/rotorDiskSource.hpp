/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2016 Applied CCM
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
    Cell based momentum source

    \verbatim
        "Development of Virtual Blade Model for Modelling Helicopter 
        Rotor Downwas in OpenFOAM"
        Stefano Wahono
        Defence Science and Technology Organisation, DSTO-TR-2931, 2013.

    \endverbatim

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

#ifndef rotorDiskSource_HPP
#define rotorDiskSource_HPP

#include "fvOption.hpp"
#include "cylindricalCS.hpp"
#include "NamedEnum.hpp"
#include "bladeModel.hpp"
#include "profileModelList.hpp"
#include "volFieldsFwd.hpp"
#include "dimensionSet.hpp"
#include "DataEntry.hpp"

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
        scalar beta1s;  // longitudinal flapping coeff (sin coeff)
    };


    // Protected data

    //- Reference density for incompressible case
    scalar rhoRef_;

    //- Rotor Debug Mode
    bool rotorDebug_;

    //- Rotor Under Relaxation Factor
    scalar rotorURF_;

    //- Rotor RPM as a DataEntry class 
    //  Positive anti-clockwise when looking along -ve lift direction
    autoPtr<DataEntry<scalar> > rpm_;

    //- Number of blades
    label nBlades_;

    //- Inlet flow type
    inletFlowType inletFlow_;

    //- Inlet velocity for specified inflow
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

	//- Blade flapping velocity
	List<scalar> rBetaDOT_;

    //- Area [m2]
    List<scalar> area_;

    //- Rotor co-ordinate system (r, theta, z)
    cylindricalCS coordSys_;

    //- Maximum radius
    scalar rMax_;

    //- A list of psi angle in the rotor zone for IO 
    List<scalar> psiList_;

    //- Trim model
    autoPtr<trimModel> trim_;

    //- Blade data
    bladeModel blade_;

    //- Profile data
    profileModelList profiles_;

    //- Rotor bank angle
    scalar bankAng_;

    //- Rotor pitch angle
    scalar pitchAng_;

    //- Transofrmation from Carteisan to Pitch/Bank plane
    tensor PB_;

    //- Transformation from Pitch Bank plane to Cartesian
    tensor invPB_;

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

        //- Return the cell centre positions in local rotor frame
        // (Cylindrical r, theta, z)
        inline const List<point>& x() const;

        //- Return the rotor co-ordinate system (r, theta, z)
        inline const cylindricalCS& coordSys() const;

        //- Return the rotational speed [rad/s]
        //  Positive anti-clockwise when looking along -ve lift direction
        inline scalar omega() const;

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

#include "mathematicalConstants.hpp"

using namespace CML::constant;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::scalar CML::fv::rotorDiskSource::rhoRef() const
{
    return rhoRef_;
}


const CML::List<CML::point>& CML::fv::rotorDiskSource::x() const
{
    return x_;
}


const CML::cylindricalCS& CML::fv::rotorDiskSource::coordSys() const
{
    return coordSys_;
}


CML::scalar CML::fv::rotorDiskSource::omega() const
{
    const scalar t = this->mesh_.time().timeOutputValue();
    const scalar omega = rpm_->value(t)/60.0*mathematical::twoPi;
    return omega;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"

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
    const vectorField& C = mesh_.C();
    const scalarField& V = mesh_.V();

    // Logging info
    scalar dragEff = 0.0;
    scalar liftEff = 0.0;
    scalar powerRequired = 0.0;
    scalar AOAmin = GREAT;
    scalar AOAmax = -GREAT;
    scalar epsMin = GREAT;
    scalar epsMax = -GREAT;

    scalar totalThrust = 0.0;
    scalar totalPitchingMoment = 0.0;
    scalar totalRollingMoment = 0.0;
    scalar totalRotorTorque = 0.0;

    // Update omega  
    const scalar t = this->mesh_.time().timeOutputValue();
    scalar omega = rpm_->value(t)/60.0*mathematical::twoPi;

    forAll(cells_, i)
    {
        if (area_[i] > ROOTVSMALL)
        {
            const label celli = cells_[i];

            const scalar radius = x_[i].x();
            const scalar psi = x_[i].y();

            // Velocity in local cylindrical reference frame
            // This assumes that the Uz is the same as the rotorDiskPlane normal axis.

            // Aligning U to the pitch bank angle plane
            vector Upb = PB_ & U[celli];
            vector Uc = vector
                (
                    Upb.x()*cos(psi)+Upb.y()*sin(psi), -Upb.x()*sin(psi)+Upb.y()*cos(psi), Upb.z()
                );

            // Transform from rotor cylindrical into local coning system
            Uc = R_[i] & Uc;

            // Set radial component of velocity to zero
            Uc.x() = 0.0;

            // Set blade normal component of velocity
            Uc.y() = radius*omega - Uc.y();

            // Set blade normal component of velocity
            Uc.z() = Uc.z() + rBetaDOT_[i];

            // Air angle (rad)
            scalar eps = atan2(-Uc.z(), Uc.y());

            if (eps < -mathematical::pi)
            {
                eps = (2.0*mathematical::pi + eps);
            }
            if (eps > mathematical::pi)
            {
                eps = (eps - 2.0*mathematical::pi);
            }

            epsMin = min(epsMin, eps);
            epsMax = max(epsMax, eps);

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
            if (omega < 0)
            {
                alphaGeom = mathematical::pi - alphaGeom;
            }

            // Effective angle of attack
            scalar alphaEff = alphaGeom - eps;

            if (alphaEff < -mathematical::pi)
            {
                alphaEff = (2.0*mathematical::pi + alphaEff);
            }
            if (alphaEff > mathematical::pi)
            {
                alphaEff = (alphaEff - 2.0*mathematical::pi);
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
            scalar pDyn = 0.5*rho[celli]*magSqr(Uc);

            scalar f = pDyn*chord*nBlades_*area_[i]/radius/mathematical::twoPi;

            // Implementation of Kim et al [7th OpenFOAM Workshop]
            scalar ftc = (f*Cd*cos(eps) + tipFactor*f*Cl*sin(eps));
            scalar fnc = (-f*Cd*sin(eps) + tipFactor*f*Cl*cos(eps));

            vector localForce = vector(0.0, -ftc, fnc);

            // Accumulate forces
            dragEff += rhoRef_*localForce.y();
            liftEff += rhoRef_*localForce.z();

            if (rotorDebug_)
            {
                if (i == 0)
                {
                    Info << "Celli    psi    radius    alphaEff    "
                         << "eps    Cl    Cd     f    fn    fth" << nl << endl;
                }

                Info << celli << "    " << psiList_[celli] << "    " << radius << "    "
                     << radToDeg(alphaEff) << "    "
                     << radToDeg(eps) << "    "
                     << Cl << "    " << Cd << "    "
                     << f << "    " << localForce.y() << "    " << localForce.z()
                     << endl;
            }

            // Transform force from local coning system into rotor cylindrical
            localForce = invR_[i] & localForce;

            // Transform force into global Cartesian co-ordinate system
            // the line below assumes that there is zero tilt on the rotorDiskPlane
            localForce = vector
                (
                    localForce.x()*cos(psi) - localForce.y()*sin(psi),
                    localForce.x()*sin(psi) + localForce.y()*cos(psi),
                    localForce.z()
                );

            force[celli] = invPB_ & localForce;
            vector moment = force[celli]^(C[celli] - coordSys_.origin());

            // Calculate global thrust
            totalThrust += force[celli] & coordSys_.R().e3();
            totalPitchingMoment += moment & coordSys_.R().e2();
            totalRollingMoment += moment & coordSys_.R().e1();
            totalRotorTorque += moment & coordSys_.R().e3();

            if (divideVolume)
            {
                // Calculate momentum source
                force[celli] /= V[celli];
            }
        }
    }

    if (output)
    {
        reduce(AOAmin, minOp<scalar>());
        reduce(AOAmax, maxOp<scalar>());
        reduce(epsMin, minOp<scalar>());
        reduce(epsMax, maxOp<scalar>());
        reduce(dragEff, sumOp<scalar>());
        reduce(liftEff, sumOp<scalar>());
        reduce(totalThrust, sumOp<scalar>());
        reduce(totalPitchingMoment, sumOp<scalar>());
        reduce(totalRollingMoment, sumOp<scalar>());
        reduce(totalRotorTorque, sumOp<scalar>());

        scalar rpm = omega*60/mathematical::twoPi;
        powerRequired = totalRotorTorque * omega;

        // Check DataEntry type
        if ((rpm_->type() == "constant") || (rpm_->type() == "CompatibilityConstant"))
        {
            Info << "Constant Rotor Speed: " << this->name() << endl;
            Info << "    RPM                  = " << rpm << endl;
            Info << "    Omega                = " << omega << " rad/s" << endl;
        }
        else
        {
            Info << "Variable Rotor Speed: " << this->name() << endl;
            Info << "    Current RPM          = " << rpm << endl;
            Info << "    Current omega        = " << omega << " rad/s" << endl;
        }

        Info<< type() << " output:" << nl
            << "    Min/Max(AOA)          = " << radToDeg(AOAmin) << ", "
            << radToDeg(AOAmax) << nl
            << "    Min/Max(induced AOA)  = " << radToDeg(epsMin) << ", "
            << radToDeg(epsMax) << nl
            << "    Effective blade drag  = " << dragEff << nl
            << "    Effective blade lift  = " << liftEff << nl
            << "    Total Thrust          = " << totalThrust << nl
            << "    Total Pitching Moment = " << totalPitchingMoment << nl
            << "    Total Rolling Moment  = " << totalRollingMoment << nl
            << "    Total Rotor Torque    = " << totalRotorTorque << nl
            << "    Rotor Power Required  = " << powerRequired << endl;
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
            FatalErrorInFunction << "cells_.size() != values_.size()"
                << abort(FatalError);
        }

        forAll(cells_, i)
        {
            const label celli = cells_[i];
            fld[celli] = values[i];
        }

        tfld().write();
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
