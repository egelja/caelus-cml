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
    temperatureCoupledBase

Description
    Common functions for use in temperature coupled boundaries. For now only

    K() : heat conduction at patch. Gets supplied how to lookup/calculate K:

    - 'lookup' : lookup volScalarField (or volSymmTensorField) with name
    - 'basicThermo' : use basicThermo and default compressible::turbulenceModel
       to calculate K
    - 'solidThermo' : use basicSolidThermo K()
    - 'directionalSolidThermo' directionalK()

SourceFiles
    temperatureCoupledBase.cpp

\*---------------------------------------------------------------------------*/

#ifndef temperatureCoupledBase_H
#define temperatureCoupledBase_H

#include "scalarField.hpp"
#include "NamedEnum.hpp"
#include "fvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
        Class temperatureCoupledBase Declaration
\*---------------------------------------------------------------------------*/

class temperatureCoupledBase
{
public:
        //- Type of supplied K
        enum KMethodType
        {
            BASICTHERMO,
            SOLIDTHERMO,
            DIRECTIONALSOLIDTHERMO,
            LOOKUP
        };


protected:

    // Protected data

        static const NamedEnum<KMethodType, 4> KMethodTypeNames_;

        //- Underlying patch
        const fvPatch& patch_;

        //- How to get K
        const KMethodType method_;

        //- Name of thermal conductivity field (if looked up from database)
        const word KName_;


public:

    // Constructors

        //- Construct from patch and K name
        temperatureCoupledBase
        (
            const fvPatch& patch,
            const word& calculationMethod,
            const word& KName
        );

        //- Construct from patch and dictionary
        temperatureCoupledBase
        (
            const fvPatch& patch,
            const dictionary& dict
        );


    // Member functions

        //- Method to obtain K
        word KMethod() const
        {
            return KMethodTypeNames_[method_];
        }

        //- Name of thermal conductivity field
        const word& KName() const
        {
            return KName_;
        }

        //- Given patch temperature calculate corresponding K field
        tmp<scalarField> K(const scalarField& Tp) const;

        //- Write
        void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
