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
    CML::thermalBaffle1DFvPatchScalarField

Description
    This BC solves a steady 1D thermal baffle.

    The solid properties are specify as dictionary. Optionally radiative heat
    flux (qr) can be incorporated into the balance. Some under-relaxation might
    be needed on qr.  Baffle and solid properties need to be specified on the
    master side of the baffle.

Usage
    Example of the boundary condition specification using constant
    solid thermo :

    \verbatim
    <masterPatchName>
    {
        type   compressible::thermalBaffle1D<hConstSolidThermoPhysics>;
        samplePatch     <slavePatchName>;

        thickness       uniform 0.005;  // thickness [m]
        Qs              uniform 100;    // heat flux [W/m2]

        qr              none;
        qrRelaxation    1;

        // Solid thermo
        specie
        {
            molWeight       20;
        }
        transport
        {
            kappa           1;
        }
        thermodynamics
        {
            Hf              0;
            Cp              10;
        }
        equationOfState
        {
            rho             10;
        }

        value               uniform 300;
    }

    <slavePatchName>
    {
        type   compressible::thermalBaffle1D<hConstSolidThermoPhysics>;
        samplePatch     <masterPatchName>;

        qr              none;
        relaxation      1;
    }
    \endverbatim

SourceFiles
    thermalBaffle1DFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef thermalBaffle1DFvPatchScalarField_H
#define thermalBaffle1DFvPatchScalarField_H

#include "mixedFvPatchFields.hpp"
#include "autoPtr.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "mappedPatchBase.hpp"
#include "compressibleTurbulenceModel.hpp"
#include "mapDistribute.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{

/*---------------------------------------------------------------------------*\
            Class thermalBaffle1DFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

template<class solidType>
class thermalBaffle1DFvPatchScalarField
:
    public mappedPatchBase,
    public mixedFvPatchScalarField
{
    // Private data

        //- Name of the temperature field
        word TName_;

        //- Baffle is activated
        bool baffleActivated_;

        //- Baffle thickness [m]
        mutable scalarField thickness_;

        //- Superficial heat source [W/m2]
        mutable scalarField Qs_;

        //- Solid dictionary
        dictionary solidDict_;

        //- Solid thermo
        mutable autoPtr<solidType> solidPtr_;

        //- Cache qr for relaxation
        scalarField qrPrevious_;

        //- Relaxation for qr
        scalar qrRelaxation_;

        //- Name of the radiative heat flux in local region
        const word qrName_;


    // Private members

        //- Return const solid thermo
        const solidType& solid() const;

        //- Return Qs from master
        tmp<scalarField> Qs() const;

        //- Return thickness from master
        tmp<scalarField> baffleThickness() const;

        //- Is Owner
        bool owner() const;


public:

    //- Runtime type information
    TypeName("compressible::thermalBaffle1D");


    // Constructors

        //- Construct from patch and internal field
        thermalBaffle1DFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        thermalBaffle1DFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  thermalBaffle1DFvPatchScalarField onto a new patch
        thermalBaffle1DFvPatchScalarField
        (
            const thermalBaffle1DFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        thermalBaffle1DFvPatchScalarField
        (
            const thermalBaffle1DFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new thermalBaffle1DFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        thermalBaffle1DFvPatchScalarField
        (
            const thermalBaffle1DFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new thermalBaffle1DFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchScalarField&,
                const labelList&
            );


        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class solidType>
thermalBaffle1DFvPatchScalarField<solidType>::
thermalBaffle1DFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mappedPatchBase(p.patch()),
    mixedFvPatchScalarField(p, iF),
    TName_("T"),
    baffleActivated_(true),
    thickness_(p.size()),
    Qs_(p.size()),
    solidDict_(),
    solidPtr_(nullptr),
    qrPrevious_(p.size()),
    qrRelaxation_(1),
    qrName_("undefined-qr")
{}


template<class solidType>
thermalBaffle1DFvPatchScalarField<solidType>::
thermalBaffle1DFvPatchScalarField
(
    const thermalBaffle1DFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mappedPatchBase(p.patch(), ptf),
    mixedFvPatchScalarField(ptf, p, iF, mapper),
    TName_(ptf.TName_),
    baffleActivated_(ptf.baffleActivated_),
    thickness_(ptf.thickness_, mapper),
    Qs_(ptf.Qs_, mapper),
    solidDict_(ptf.solidDict_),
    solidPtr_(ptf.solidPtr_),
    qrPrevious_(ptf.qrPrevious_, mapper),
    qrRelaxation_(ptf.qrRelaxation_),
    qrName_(ptf.qrName_)
{}


template<class solidType>
thermalBaffle1DFvPatchScalarField<solidType>::
thermalBaffle1DFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mappedPatchBase(p.patch(), NEARESTPATCHFACE, dict),
    mixedFvPatchScalarField(p, iF),
    TName_("T"),
    baffleActivated_(dict.lookupOrDefault<bool>("baffleActivated", true)),
    thickness_(),
    Qs_(p.size(), 0),
    solidDict_(dict),
    solidPtr_(),
    qrPrevious_(p.size(), 0.0),
    qrRelaxation_(dict.lookupOrDefault<scalar>("qrRelaxation", 1)),
    qrName_(dict.lookupOrDefault<word>("qr", "none"))
{
    fvPatchScalarField::operator=(scalarField("value", dict, p.size()));

    if (dict.found("thickness"))
    {
        thickness_ = scalarField("thickness", dict, p.size());
    }

    if (dict.found("Qs"))
    {
        Qs_ = scalarField("Qs", dict, p.size());
    }

    if (dict.found("qrPrevious"))
    {
        qrPrevious_ = scalarField("qrPrevious", dict, p.size());
    }

    if (dict.found("refValue") && baffleActivated_)
    {
        // Full restart
        refValue() = scalarField("refValue", dict, p.size());
        refGrad() = scalarField("refGradient", dict, p.size());
        valueFraction() = scalarField("valueFraction", dict, p.size());
    }
    else
    {
        // Start from user entered data. Assume zeroGradient.
        refValue() = *this;
        refGrad() = 0.0;
        valueFraction() = 0.0;
    }

}


template<class solidType>
thermalBaffle1DFvPatchScalarField<solidType>::
thermalBaffle1DFvPatchScalarField
(
    const thermalBaffle1DFvPatchScalarField& ptf
)
:
    mappedPatchBase(ptf.patch().patch(), ptf),
    mixedFvPatchScalarField(ptf),
    TName_(ptf.TName_),
    baffleActivated_(ptf.baffleActivated_),
    thickness_(ptf.thickness_),
    Qs_(ptf.Qs_),
    solidDict_(ptf.solidDict_),
    solidPtr_(ptf.solidPtr_),
    qrPrevious_(ptf.qrPrevious_),
    qrRelaxation_(ptf.qrRelaxation_),
    qrName_(ptf.qrName_)
{}


template<class solidType>
thermalBaffle1DFvPatchScalarField<solidType>::
thermalBaffle1DFvPatchScalarField
(
    const thermalBaffle1DFvPatchScalarField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mappedPatchBase(ptf.patch().patch(), ptf),
    mixedFvPatchScalarField(ptf, iF),
    TName_(ptf.TName_),
    baffleActivated_(ptf.baffleActivated_),
    thickness_(ptf.thickness_),
    Qs_(ptf.Qs_),
    solidDict_(ptf.solidDict_),
    solidPtr_(ptf.solidPtr_),
    qrPrevious_(ptf.qrPrevious_),
    qrRelaxation_(ptf.qrRelaxation_),
    qrName_(ptf.qrName_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class solidType>
bool thermalBaffle1DFvPatchScalarField<solidType>::owner() const
{
    const label patchi = patch().index();

    const label nbrPatchi = samplePolyPatch().index();

    return (patchi < nbrPatchi);
}


template<class solidType>
const solidType& thermalBaffle1DFvPatchScalarField<solidType>::solid() const
{
    if (this->owner())
    {
        if (solidPtr_.empty())
        {
            solidPtr_.reset(new solidType(solidDict_));
        }
        return solidPtr_();
    }
    else
    {
        const fvPatch& nbrPatch =
            patch().boundaryMesh()[samplePolyPatch().index()];

        const thermalBaffle1DFvPatchScalarField& nbrField =
        refCast<const thermalBaffle1DFvPatchScalarField>
        (
            nbrPatch.template lookupPatchField<volScalarField, scalar>(TName_)
        );

        return nbrField.solid();
    }
}


template<class solidType>
tmp<scalarField> thermalBaffle1DFvPatchScalarField<solidType>::
baffleThickness() const
{
    if (this->owner())
    {
        if (thickness_.size() != patch().size())
        {
            FatalIOErrorInFunction
            (
                solidDict_
            )<< " Field thickness has not been specified "
            << " for patch " << this->patch().name()
            << exit(FatalIOError);
        }

        return thickness_;
    }
    else
    {
        const mapDistribute& mapDist = this->mappedPatchBase::map();

        const fvPatch& nbrPatch =
            patch().boundaryMesh()[samplePolyPatch().index()];
        const thermalBaffle1DFvPatchScalarField& nbrField =
        refCast<const thermalBaffle1DFvPatchScalarField>
        (
            nbrPatch.template lookupPatchField<volScalarField, scalar>(TName_)
        );

        tmp<scalarField> tthickness
        (
            new scalarField(nbrField.baffleThickness())
        );
        scalarField& thickness = tthickness();
        mapDist.distribute(thickness);
        return tthickness;
    }
}


template<class solidType>
tmp<scalarField> thermalBaffle1DFvPatchScalarField<solidType>::Qs() const
{
    if (this->owner())
    {
         return Qs_;
    }
    else
    {
        const mapDistribute& mapDist = this->mappedPatchBase::map();

        const fvPatch& nbrPatch =
            patch().boundaryMesh()[samplePolyPatch().index()];

        const thermalBaffle1DFvPatchScalarField& nbrField =
        refCast<const thermalBaffle1DFvPatchScalarField>
        (
            nbrPatch.template lookupPatchField<volScalarField, scalar>(TName_)
        );

        tmp<scalarField> tQs(new scalarField(nbrField.Qs()));
        scalarField& Qs = tQs();
        mapDist.distribute(Qs);
        return tQs;
    }
}


template<class solidType>
void thermalBaffle1DFvPatchScalarField<solidType>::autoMap
(
    const fvPatchFieldMapper& m
)
{
    mappedPatchBase::clearOut();

    mixedFvPatchScalarField::autoMap(m);

    if (this->owner())
    {
        thickness_.autoMap(m);
        Qs_.autoMap(m);
    }
}


template<class solidType>
void thermalBaffle1DFvPatchScalarField<solidType>::rmap
(
    const fvPatchScalarField& ptf,
    const labelList& addr
)
{
    mixedFvPatchScalarField::rmap(ptf, addr);

    const thermalBaffle1DFvPatchScalarField& tiptf =
        refCast<const thermalBaffle1DFvPatchScalarField>(ptf);

    if (this->owner())
    {
        thickness_.rmap(tiptf.thickness_, addr);
        Qs_.rmap(tiptf.Qs_, addr);
    }
}


template<class solidType>
void thermalBaffle1DFvPatchScalarField<solidType>::updateCoeffs()
{
    if (updated())
    {
        return;
    }
    // Since we're inside initEvaluate/evaluate there might be processor
    // comms underway. Change the tag we use.
    int oldTag = UPstream::msgType();
    UPstream::msgType() = oldTag+1;

    const mapDistribute& mapDist = this->mappedPatchBase::map();

    const label patchi = patch().index();

    const label nbrPatchi = samplePolyPatch().index();

    if (baffleActivated_)
    {
        const fvPatch& nbrPatch = patch().boundaryMesh()[nbrPatchi];

        const compressible::turbulenceModel& turbModel =
            db().template lookupObject<compressible::turbulenceModel>
            (
                "turbulenceModel"
            );

        // local properties
        const scalarField kappaw(turbModel.kappaEff(patchi));

        const fvPatchScalarField& Tp =
            patch().template lookupPatchField<volScalarField, scalar>(TName_);


        scalarField qr(Tp.size(), 0.0);

        if (qrName_ != "none")
        {
            qr = patch().template lookupPatchField<volScalarField, scalar>
                (qrName_);

            qr = qrRelaxation_*qr + (1.0 - qrRelaxation_)*qrPrevious_;
            qrPrevious_ = qr;
        }

        tmp<scalarField> Ti = patchInternalField();

        scalarField myKDelta(patch().deltaCoeffs()*kappaw);

        // nrb properties
        scalarField nbrTp =
            turbModel.thermo().T().boundaryField()[nbrPatchi];
        mapDist.distribute(nbrTp);

        // solid properties
        scalarField kappas(patch().size(), 0.0);
        forAll(kappas, i)
        {
            kappas[i] = solid().kappa(0.0, (Tp[i] + nbrTp[i])/2.0);
        }

        scalarField KDeltaSolid(kappas/baffleThickness());

        scalarField alpha(KDeltaSolid - qr/Tp);

        valueFraction() = alpha/(alpha + myKDelta);

        refValue() = (KDeltaSolid*nbrTp + Qs()/2.0)/alpha;

        if (debug)
        {
            scalar Q = gAverage(kappaw*snGrad());
            Info<< patch().boundaryMesh().mesh().name() << ':'
                << patch().name() << ':'
                << this->dimensionedInternalField().name() << " <- "
                << nbrPatch.name() << ':'
                << this->dimensionedInternalField().name() << " :"
                << " heat[W]:" << Q
                << " walltemperature "
                << " min:" << gMin(*this)
                << " max:" << gMax(*this)
                << " avg:" << gAverage(*this)
                << endl;
        }
    }

    // Restore tag
    UPstream::msgType() = oldTag;

    mixedFvPatchScalarField::updateCoeffs();
}

template<class solidType>
void thermalBaffle1DFvPatchScalarField<solidType>::write(Ostream& os) const
{
    mixedFvPatchScalarField::write(os);
    mappedPatchBase::write(os);

    if (this->owner())
    {
        baffleThickness()().writeEntry("thickness", os);
        Qs()().writeEntry("Qs", os);
        solid().write(os);
    }

    qrPrevious_.writeEntry("qrPrevious", os);
    os.writeKeyword("qr")<< qrName_ << token::END_STATEMENT << nl;
    os.writeKeyword("qrRelaxation")<< qrRelaxation_
        << token::END_STATEMENT << nl;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace compressible
} // End namespace CML


#endif

// ************************************************************************* //
