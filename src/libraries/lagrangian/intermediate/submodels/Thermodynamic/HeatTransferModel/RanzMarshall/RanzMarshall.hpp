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
    CML::RanzMarshall

Description
    The Ranz-Marshall correlation for heat transfer

\*---------------------------------------------------------------------------*/

#ifndef RanzMarshall_H
#define RanzMarshall_H

#include "HeatTransferModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class RanzMarshall Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class RanzMarshall
:
    public HeatTransferModel<CloudType>
{

public:

    //- Runtime type information
    TypeName("RanzMarshall");


    // Constructors

        //- Construct from dictionary
        RanzMarshall(const dictionary& dict, CloudType& cloud);

        //- Construct copy
        RanzMarshall(const RanzMarshall<CloudType>& im);

        //- Construct and return a clone
        virtual autoPtr<HeatTransferModel<CloudType> > clone() const
        {
            return autoPtr<HeatTransferModel<CloudType> >
            (
                new RanzMarshall<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~RanzMarshall();


    // Member Functions

        // Evaluation

            //- Nusselt number
            virtual scalar Nu
            (
                const scalar Re,
                const scalar Pr
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::RanzMarshall<CloudType>::RanzMarshall
(
    const dictionary& dict,
    CloudType& cloud
)
:
    HeatTransferModel<CloudType>(dict, cloud, typeName)
{}


template<class CloudType>
CML::RanzMarshall<CloudType>::RanzMarshall(const RanzMarshall<CloudType>& htm)
:
    HeatTransferModel<CloudType>(htm)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::RanzMarshall<CloudType>::~RanzMarshall()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::RanzMarshall<CloudType>::Nu
(
    const scalar Re,
    const scalar Pr
) const
{
    return 2.0 + 0.6*sqrt(Re)*cbrt(Pr);
}


#endif

// ************************************************************************* //
