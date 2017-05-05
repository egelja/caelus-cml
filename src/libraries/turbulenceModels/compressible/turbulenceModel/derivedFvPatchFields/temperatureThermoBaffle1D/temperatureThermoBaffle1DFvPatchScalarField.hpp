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
    CML::temperatureThermoBaffle1DFvPatchScalarField

Description
    Boundary which solves the 1D steady state heat transfer equation
    through a baffle.


\*---------------------------------------------------------------------------*/

#ifndef temperatureThermoBaffle1DFvPatchScalarField_H
#define temperatureThermoBaffle1DFvPatchScalarField_H

#include "mixedFvPatchFields.hpp"
#include "autoPtr.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "mappedPatchBase.hpp"
#include "turbulenceModel.hpp"
#include "mapDistribute.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{

/*---------------------------------------------------------------------------*\
        Class temperatureThermoBaffle1DFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

template<class solidType>
class temperatureThermoBaffle1DFvPatchScalarField
:
    public mixedFvPatchScalarField
{
    // Private data

    class solidThermoData
    {
            // Solid thermo
            autoPtr<solidType> solidPtr_;

            // Solid dictionaries
            dictionary  transportDict_;
            dictionary  radiationDict_;
            dictionary  thermoDict_;
            dictionary  densityDict_;


    public:

        // Constructor from components
        solidThermoData(const dictionary& dict)
        :
            solidPtr_(new solidType(dict)),
            transportDict_(dict.subDict("transport")),
            radiationDict_(dict.subDict("radiation")),
            thermoDict_(dict.subDict("thermodynamics")),
            densityDict_(dict.subDict("density"))
        {}


        // Null constructor
        solidThermoData()
        :
            solidPtr_(),
            transportDict_(),
            radiationDict_(),
            thermoDict_(),
            densityDict_()
        {}


        // Destructor
        virtual ~solidThermoData()
        {}


        // Member Functions

            void write(Ostream& os) const
            {
                os.writeKeyword("transport");
                os << transportDict_  << nl;
                os.writeKeyword("radiation");
                os << radiationDict_ <<  nl;
                os.writeKeyword("thermodynamics");
                os << thermoDict_ << nl;
                os.writeKeyword("density");
                os << densityDict_ << nl;
            }


            scalar K(const scalar T) const
            {
                return solidPtr_().K(T);
            }
    };


        //- Name of the temperature field
        word TName_;

        //- Baffle is activated
        bool baffleActivated_;

        //- Baffle thickness [m]
        scalarField thickness_;

        //- Superficial heat source [W/m2]
        scalarField Qs_;

        //- Solid thermo
        autoPtr<solidThermoData> solid_;


public:

    //- Runtime type information
    TypeName("compressible::temperatureThermoBaffle1D");


    // Constructors

        //- Construct from patch and internal field
        temperatureThermoBaffle1DFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        temperatureThermoBaffle1DFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  temperatureThermoBaffle1DFvPatchScalarField onto a new patch
        temperatureThermoBaffle1DFvPatchScalarField
        (
            const temperatureThermoBaffle1DFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        temperatureThermoBaffle1DFvPatchScalarField
        (
            const temperatureThermoBaffle1DFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new temperatureThermoBaffle1DFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        temperatureThermoBaffle1DFvPatchScalarField
        (
            const temperatureThermoBaffle1DFvPatchScalarField&,
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
                new temperatureThermoBaffle1DFvPatchScalarField(*this, iF)
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
temperatureThermoBaffle1DFvPatchScalarField<solidType>::
temperatureThermoBaffle1DFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(p, iF),
    TName_("T"),
    baffleActivated_(true),
    thickness_(p.size()),
    Qs_(p.size()),
    solid_(NULL)
{}


template<class solidType>
temperatureThermoBaffle1DFvPatchScalarField<solidType>::
temperatureThermoBaffle1DFvPatchScalarField
(
    const temperatureThermoBaffle1DFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchScalarField(ptf, p, iF, mapper),
    TName_(ptf.TName_),
    baffleActivated_(ptf.baffleActivated_),
    thickness_(ptf.thickness_),
    Qs_(ptf.Qs_),
    solid_(ptf.solid_)
{}


template<class solidType>
temperatureThermoBaffle1DFvPatchScalarField<solidType>::
temperatureThermoBaffle1DFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchScalarField(p, iF),
    TName_("T"),
    baffleActivated_(readBool(dict.lookup("baffleActivated"))),
    thickness_(scalarField("thickness", dict, p.size())),
    Qs_(scalarField("Qs", dict, p.size())),
    solid_(new solidThermoData(dict))
{
    if (!isA<mappedPatchBase>(this->patch().patch()))
    {
        FatalErrorIn
        (
            "temperatureThermoBaffle1DFvPatchScalarField::"
            "temperatureThermoBaffle1DFvPatchScalarField\n"
            "(\n"
            "    const fvPatch& p,\n"
            "    const DimensionedField<scalar, volMesh>& iF,\n"
            "    const dictionary& dict\n"
            ")\n"
        )   << "\n    patch type '" << patch().type()
            << "' not type '" << mappedPatchBase::typeName << "'"
            << "\n    for patch " << patch().name()
            << " of field " << dimensionedInternalField().name()
            << " in file " << dimensionedInternalField().objectPath()
            << exit(FatalError);
    }

    fvPatchScalarField::operator=(scalarField("value", dict, p.size()));

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
temperatureThermoBaffle1DFvPatchScalarField<solidType>::
temperatureThermoBaffle1DFvPatchScalarField
(
    const temperatureThermoBaffle1DFvPatchScalarField& ptf
)
:
    mixedFvPatchScalarField(ptf),
    TName_(ptf.TName_),
    baffleActivated_(ptf.baffleActivated_),
    thickness_(ptf.thickness_),
    Qs_(ptf.Qs_),
    solid_(ptf.solid_)
{}


template<class solidType>
temperatureThermoBaffle1DFvPatchScalarField<solidType>::
temperatureThermoBaffle1DFvPatchScalarField
(
    const temperatureThermoBaffle1DFvPatchScalarField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(ptf, iF),
    TName_(ptf.TName_),
    baffleActivated_(ptf.baffleActivated_),
    thickness_(ptf.thickness_),
    Qs_(ptf.Qs_),
    solid_(ptf.solid_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class solidType>
void temperatureThermoBaffle1DFvPatchScalarField<solidType>::autoMap
(
    const fvPatchFieldMapper& m
)
{
    mixedFvPatchScalarField::autoMap(m);
}

template<class solidType>
void temperatureThermoBaffle1DFvPatchScalarField<solidType>::rmap
(
    const fvPatchScalarField& ptf,
    const labelList& addr
)
{
    mixedFvPatchScalarField::rmap(ptf, addr);
}


template<class solidType>
void temperatureThermoBaffle1DFvPatchScalarField<solidType>::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    // Since we're inside initEvaluate/evaluate there might be processor
    // comms underway. Change the tag we use.
    int oldTag = UPstream::msgType();
    UPstream::msgType() = oldTag+1;

    const mappedPatchBase& mpp = refCast<const mappedPatchBase>
    (
        patch().patch()
    );

    const label patchI = patch().index();

    const label nbrPatchI = mpp.samplePolyPatch().index();

    if (baffleActivated_)
    {

        const fvPatch& nbrPatch =
            patch().boundaryMesh()[mpp.samplePolyPatch().index()];

        const compressible::turbulenceModel& model =
            db().template lookupObject<compressible::turbulenceModel>
            (
                "turbulenceModel"
            );


        // local properties
        const scalarField alphaw = model.alphaEff()().boundaryField()[patchI];

        const fvPatchScalarField& hw =
            model.thermo().h().boundaryField()[patchI];

        const scalarField qDot(alphaw*hw.snGrad());

        const scalarField& Tp =
            patch().template lookupPatchField<volScalarField, scalar>(TName_);

        tmp<scalarField> Ti = patchInternalField();

        const scalarField Cpw(model.thermo().Cp(Ti, patchI));

        scalarField myh(patch().deltaCoeffs()*alphaw*Cpw);

        scalarField alphawCp(alphaw*Cpw);


        // nbr properties
        scalarField nbrAlphaw =
            model.alphaEff()().boundaryField()[nbrPatchI];
        mpp.map().distribute(nbrAlphaw);

        const fvPatchScalarField& nbrHw =
            model.thermo().h().boundaryField()[nbrPatchI];

        scalarField nbrQDot
        (
            model.alphaEff()().boundaryField()[nbrPatchI]*nbrHw.snGrad()
        );
        mpp.map().distribute(nbrQDot);

        scalarField nbrTp =
            nbrPatch.template lookupPatchField<volScalarField, scalar>(TName_);
        mpp.map().distribute(nbrTp);

        const temperatureThermoBaffle1DFvPatchScalarField& nbrField =
        refCast<const temperatureThermoBaffle1DFvPatchScalarField>
        (
            nbrPatch.template lookupPatchField<volScalarField, scalar>
            (
                TName_
            )
        );

        scalarField nbrTi(nbrField.patchInternalField());
        mpp.map().distribute(nbrTi);

        const scalarField nbrCpw
        (
            model.thermo().Cp(nbrField.patchInternalField(), nbrPatchI)
        );

        scalarField nbrh
        (
            nbrPatch.deltaCoeffs()*nbrCpw
           *model.alphaEff()().boundaryField()[nbrPatchI]
        );

        mpp.map().distribute(nbrh);


        // heat source
        const scalarField Q(Qs_/thickness_);

        tmp<scalarField> tKDeltaw(new scalarField(patch().size()));
        scalarField KDeltaw = tKDeltaw();

        // Create fields for solid properties
        forAll(KDeltaw, i)
        {
            KDeltaw[i] = solid_().K((Tp[i] + nbrTp[i])/2.0)/thickness_[i];
        }

        const scalarField q
        (
            (Ti() - nbrTi)/(1.0/KDeltaw + 1.0/nbrh + 1.0/myh)
        );


        forAll(qDot, i)
        {
            if (Qs_[i] == 0)
            {
                this->refValue()[i] = Ti()[i] - q[i]/myh[i];
                this->refGrad()[i] = 0.0;
                this->valueFraction()[i] = 1.0;
            }
            else
            {
                if (q[i] > 0)
                {
                    this->refValue()[i] =
                        nbrTp[i]
                      - Q[i]*thickness_[i]/(2*KDeltaw[i]);

                    this->refGrad()[i] = 0.0;
                    this->valueFraction()[i] =
                        1.0
                        /
                        (
                            1.0
                          + patch().deltaCoeffs()[i]*alphawCp[i]/KDeltaw[i]
                        );
                }
                else if (q[i] < 0)
                {
                    this->refValue()[i] = 0.0;
                    this->refGrad()[i] =
                        (-nbrQDot[i] + Q[i]*thickness_[i])/alphawCp[i];
                    this->valueFraction()[i] = 0.0;
                }
                else
                {
                    scalar Qt = Q[i]*thickness_[i];
                    this->refValue()[i] = 0.0;
                    this->refGrad()[i] = Qt/2/alphawCp[i];
                    this->valueFraction()[i] = 0.0;
                }
            }
        }

        if (debug)
        {
            scalar Q = gSum(patch().magSf()*qDot);
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

template <class solidType>
void temperatureThermoBaffle1DFvPatchScalarField<solidType>::
write(Ostream& os) const
{
    mixedFvPatchScalarField::write(os);
    os.writeKeyword("TName")
        << TName_ << token::END_STATEMENT << nl;
    thickness_.writeEntry("thickness", os);
    os.writeKeyword("baffleActivated")
        << baffleActivated_ << token::END_STATEMENT << nl;
    Qs_.writeEntry("Qs", os);
    solid_().write(os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace compressible
} // End namespace CML


#endif

// ************************************************************************* //
