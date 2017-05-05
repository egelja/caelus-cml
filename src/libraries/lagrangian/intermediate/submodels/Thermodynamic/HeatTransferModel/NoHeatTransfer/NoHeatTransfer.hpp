/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::NoHeatTransfer

Description
    Dummy heat transfer model for 'none'

\*---------------------------------------------------------------------------*/

#ifndef NoHeatTransfer_H
#define NoHeatTransfer_H

#include "HeatTransferModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class NoHeatTransfer Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NoHeatTransfer
:
    public HeatTransferModel<CloudType>
{
public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from dictionary
        NoHeatTransfer(const dictionary&, CloudType& owner);

        //- Construct copy
        NoHeatTransfer(const NoHeatTransfer<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<HeatTransferModel<CloudType> > clone() const
        {
            return autoPtr<HeatTransferModel<CloudType> >
            (
                new NoHeatTransfer<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoHeatTransfer();


    // Member Functions

        //- Flag to indicate whether model activates heat transfer model
        virtual bool active() const;

        //- Nusselt number
        virtual scalar Nu(const scalar, const scalar) const;

        //- Prandtl number
        virtual scalar Pr() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoHeatTransfer<CloudType>::NoHeatTransfer
(
    const dictionary&,
    CloudType& owner
)
:
    HeatTransferModel<CloudType>(owner)
{}


template<class CloudType>
CML::NoHeatTransfer<CloudType>::NoHeatTransfer
(
    const NoHeatTransfer<CloudType>& htm
)
:
    HeatTransferModel<CloudType>(htm.owner_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NoHeatTransfer<CloudType>::~NoHeatTransfer()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::NoHeatTransfer<CloudType>::active() const
{
    return false;
}


template<class CloudType>
CML::scalar CML::NoHeatTransfer<CloudType>::Nu
(
    const scalar,
    const scalar
) const
{
    return 0.0;
}


template<class CloudType>
CML::scalar CML::NoHeatTransfer<CloudType>::Pr() const
{
    return 1.0;
}


#endif

// ************************************************************************* //
