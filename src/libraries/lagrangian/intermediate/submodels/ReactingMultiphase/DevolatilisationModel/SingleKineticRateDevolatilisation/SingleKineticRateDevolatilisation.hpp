/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::SingleKineticRateDevolatilisation

Description
    Single kinetic rate devolatisation model.
    - acts on a per-specie basis
    - Rate given by Arrhenius eqn

        kappa = A1.exp(- E/R.T)

    Where:
        kappa = rate constant
        A1    = pre-exponential factor (user input)
        E     = activation energy (user input)
        R     = universal gas constant
        T     = temperature

    Usage:

        SingleKineticRateDevolatilisationCoeffs
        {
            volatileData
            (
                (CH4     12     0.5)   // (name A1 E)
                (CO2     12     0.5)   // (name A1 E)
            );

            volatileResidualCoeff 1e-6;
        }

\*---------------------------------------------------------------------------*/

#ifndef SingleKineticRateDevolatilisation_H
#define SingleKineticRateDevolatilisation_H

#include "DevolatilisationModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
              Class SingleKineticRateDevolatilisation Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class SingleKineticRateDevolatilisation
:
    public DevolatilisationModel<CloudType>
{
    // Helper class to store specie-local volatile data
    class volatileData
    {
        // Private data

            //- Specie name
            word name_;

            //- Activation energy
            scalar A1_;

            //- Pre-exponential factor
            scalar E_;


    public:

        // Constructors

            //- Null constructor
            volatileData()
            :
                name_(word::null),
                A1_(0.0),
                E_(0.0)
            {}

            //- Construct from Istream
            volatileData(Istream& is)
            :
                name_(is),
                A1_(readScalar(is)),
                E_(readScalar(is))
            {}

            //- Construct as copy
            volatileData(const volatileData& vd)
            :
                name_(vd.name_),
                A1_(vd.A1_),
                E_(vd.E_)
            {}


        //- Destructor
        ~volatileData()
        {}


        // Public Member Functions

            // Access

                //- Return const access to the name
                const word& name() const
                {
                    return name_;
                }

                //- Return const access to the activation energy
                scalar A1() const
                {
                    return A1_;
                }

                //- Return const access to the pre-exponential factor
                scalar E() const
                {
                    return E_;
                }


        // IOstream Operators

            //- Read from Istream
            friend Istream& operator>>(Istream& is, volatileData& vd)
            {
                is.readBeginList("volatileData");
                is  >>  vd.name_  >> vd.A1_  >> vd.E_;
                is.readEndList("volatileData");

                return is;
            }

            //- Write to Ostream
            friend Ostream& operator<<(Ostream& os, const volatileData& vd)
            {
                os  << token::BEGIN_LIST
                    << vd.name_ << token::SPACE
                    << vd.A1_ << token::SPACE
                    << vd.E_
                    << token::END_LIST;

                return os;
            }
    };


    // Private data

        // Model constants

            //- List of volatile data - (name A1 E)
            List<volatileData> volatileData_;

            //- List of initial volatile mass fractions
            List<scalar> YVolatile0_;

            //- Mapping between local and cloud gaseous species
            List<label> volatileToGasMap_;

            //- Volatile residual coefficient (0-1)
            //  When the fraction of volatiles are depleted below this
            //  threshold, combustion can occur
            const scalar residualCoeff_;


public:

    //- Runtime type information
    TypeName("singleKineticRateDevolatilisation");


    // Constructors

        //- Construct from dictionary
        SingleKineticRateDevolatilisation
        (
            const dictionary& dict,
            CloudType& owner
        );

        //- Construct copy
        SingleKineticRateDevolatilisation
        (
            const SingleKineticRateDevolatilisation<CloudType>& dm
        );

        //- Construct and return a clone
        virtual autoPtr<DevolatilisationModel<CloudType> > clone() const
        {
            return autoPtr<DevolatilisationModel<CloudType> >
            (
                new SingleKineticRateDevolatilisation<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~SingleKineticRateDevolatilisation();


    // Member Functions

        //- Update model
        virtual void calculate
        (
            const scalar dt,
            const scalar age,
            const scalar mass0,
            const scalar mass,
            const scalar T,
            const scalarField& YGasEff,
            const scalarField& YLiquidEff,
            const scalarField& YSolidEff,
            label& canCombust,
            scalarField& dMassDV
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::SingleKineticRateDevolatilisation<CloudType>::
SingleKineticRateDevolatilisation
(
    const dictionary& dict,
    CloudType& owner
)
:
    DevolatilisationModel<CloudType>(dict, owner, typeName),
    volatileData_(this->coeffDict().lookup("volatileData")),
    YVolatile0_(volatileData_.size()),
    volatileToGasMap_(volatileData_.size()),
    residualCoeff_(readScalar(this->coeffDict().lookup("residualCoeff")))
{
    if (volatileData_.empty())
    {
        WarningInFunction
            << "Devolatilisation model selected, but no volatiles defined"
            << nl << endl;
    }
    else
    {
        Info<< "Participating volatile species:" << endl;

        // Determine mapping between active volatiles and cloud gas components
        const label idGas = owner.composition().idGas();
        const scalar YGasTot = owner.composition().YMixture0()[idGas];
        const scalarField& YGas = owner.composition().Y0(idGas);
        forAll(volatileData_, i)
        {
            const word& specieName = volatileData_[i].name();
            const label id = owner.composition().localId(idGas, specieName);
            volatileToGasMap_[i] = id;
            YVolatile0_[i] = YGasTot*YGas[id];

            Info<< "    " << specieName << ": particle mass fraction = "
                << YVolatile0_[i] << endl;
        }
    }
}


template<class CloudType>
CML::SingleKineticRateDevolatilisation<CloudType>::
SingleKineticRateDevolatilisation
(
    const SingleKineticRateDevolatilisation<CloudType>& dm
)
:
    DevolatilisationModel<CloudType>(dm),
    volatileData_(dm.volatileData_),
    YVolatile0_(dm.YVolatile0_),
    volatileToGasMap_(dm.volatileToGasMap_),
    residualCoeff_(dm.residualCoeff_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::SingleKineticRateDevolatilisation<CloudType>::
~SingleKineticRateDevolatilisation()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::SingleKineticRateDevolatilisation<CloudType>::calculate
(
    const scalar dt,
    const scalar age,
    const scalar mass0,
    const scalar mass,
    const scalar T,
    const scalarField& YGasEff,
    const scalarField& YLiquidEff,
    const scalarField& YSolidEff,
    label& canCombust,
    scalarField& dMassDV
) const
{
    bool done = true;
    forAll(volatileData_, i)
    {
        const label id = volatileToGasMap_[i];
        const scalar massVolatile0 = mass0*YVolatile0_[i];
        const scalar massVolatile = mass*YGasEff[id];

        // Combustion allowed once all volatile components evolved
        done = done && (massVolatile <= residualCoeff_*massVolatile0);

        // Model coefficients
        const scalar A1 = volatileData_[i].A1();
        const scalar E = volatileData_[i].E();

        // Kinetic rate
        const scalar kappa = A1*exp(-E/(RR*T));

        // Mass transferred from particle to carrier gas phase
        dMassDV[id] = min(dt*kappa*massVolatile, massVolatile);
    }

    if (done && canCombust != -1)
    {
        canCombust = 1;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
