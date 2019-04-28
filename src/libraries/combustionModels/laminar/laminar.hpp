/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::combustionModels::laminar

Description
    Laminar combustion model.


\*---------------------------------------------------------------------------*/

#ifndef laminar_HPP
#define laminar_HPP

#include "ChemistryCombustion.hpp"
#include "fvmSup.hpp"
#include "localEulerDdtScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                            Class laminar Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class laminar
:
    public ChemistryCombustion<ReactionThermo>
{

    //- Integrate reaction rate over the time-step
    //  using the selected ODE solver
    bool integrateReactionRate_;

protected:

    // Protected Member Functions

    //- Return the chemical time scale
    tmp<volScalarField> tc() const;

private:

    // Private Member Functions

    //- Disallow copy construct
    laminar(const laminar&);

    //- Disallow default bitwise assignment
    void operator=(const laminar&);


public:

    //- Runtime type information
    TypeName("laminar");


    //- Construct from components
    laminar
    (
        const word& modelType,
        ReactionThermo& thermo,
        const compressible::turbulenceModel& turb,
        const word& combustionProperties
    );


    //- Destructor
    virtual ~laminar()
    {}


    // Member Functions

    //- Correct combustion rate
    virtual void correct();

    //- Fuel consumption rate matrix.
    virtual tmp<fvScalarMatrix> R(volScalarField& Y) const;

    //- Heat release rate [kg/m/s3]
    virtual tmp<volScalarField> Qdot() const;

    //- Update properties from given dictionary
    virtual bool read();

};


} // End namespace combustionModels
} // End namespace Foam


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::combustionModels::laminar<ReactionThermo>::laminar
(
    const word& modelType,
    ReactionThermo& thermo,
    const compressible::turbulenceModel& turb,
    const word& combustionProperties
)
:
    ChemistryCombustion<ReactionThermo>
    (
        modelType,
        thermo,
        turb,
        combustionProperties
    ),
    integrateReactionRate_
    (
        this->coeffs().lookupOrDefault("integrateReactionRate", true)
    )
{
    if (integrateReactionRate_)
    {
        Info<< "    using integrated reaction rate" << endl;
    }
    else
    {
        Info<< "    using instantaneous reaction rate" << endl;
    }
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::tmp<CML::volScalarField>
CML::combustionModels::laminar<ReactionThermo>::tc() const
{
    return this->chemistryPtr_->tc();
}


template<class ReactionThermo>
void CML::combustionModels::laminar<ReactionThermo>::correct()
{
    if (integrateReactionRate_)
    {
        if (fv::localEulerDdt::enabled(this->mesh()))
        {
            const scalarField& rDeltaT =
                fv::localEulerDdt::localRDeltaT(this->mesh());

            if (this->coeffs().found("maxIntegrationTime"))
            {
                const scalar maxIntegrationTime
                (
                    readScalar(this->coeffs().lookup("maxIntegrationTime"))
                );

                this->chemistryPtr_->solve
                (
                    min(1.0/rDeltaT, maxIntegrationTime)()
                );
            }
            else
            {
                this->chemistryPtr_->solve((1.0/rDeltaT)());
            }
        }
        else
        {
            this->chemistryPtr_->solve(this->mesh().time().deltaTValue());
        }
    }
}


template<class ReactionThermo>
CML::tmp<CML::fvScalarMatrix>
CML::combustionModels::laminar<ReactionThermo>::R(volScalarField& Y) const
{
    tmp<fvScalarMatrix> tSu(new fvScalarMatrix(Y, dimMass/dimTime));
    fvScalarMatrix& Su = tSu();

    const label specieI = this->thermo().composition().species()[Y.member()];
    Su += this->chemistryPtr_->RR(specieI);

    return tSu;
}


template<class ReactionThermo>
CML::tmp<CML::volScalarField>
CML::combustionModels::laminar<ReactionThermo>::Qdot() const
{
    return this->chemistryPtr_->Qdot();
}


template<class ReactionThermo>
bool CML::combustionModels::laminar<ReactionThermo>::read()
{
    if (ChemistryCombustion<ReactionThermo>::read())
    {
        integrateReactionRate_ =
            this->coeffs().lookupOrDefault("integrateReactionRate", true);
        return true;
    }
    else
    {
        return false;
    }
}


#endif
