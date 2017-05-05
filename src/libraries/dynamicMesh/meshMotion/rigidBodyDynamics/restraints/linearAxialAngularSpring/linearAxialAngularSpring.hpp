/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
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
    CML::RBD::restraints::linearAxialAngularSpring

Description
    Linear axial angular spring restraint.

SourceFiles
    linearAxialAngularSpring.cpp

\*---------------------------------------------------------------------------*/

#ifndef linearAxialAngularSpring_HPP
#define linearAxialAngularSpring_HPP

#include "rigidBodyRestraint.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
namespace restraints
{

/*---------------------------------------------------------------------------*\
                   Class linearAxialAngularSpring Declaration
\*---------------------------------------------------------------------------*/

class linearAxialAngularSpring
:
    public restraint
{
    // Private data

        //- Reference orientation where there is no moment
        tensor refQ_;

        //- Global unit axis around which the motion is sprung
        vector axis_;

        //- Spring stiffness coefficient [Nm/rad]
        scalar stiffness_;

        //- Damping coefficient [Nms/rad]
        scalar damping_;


public:

    //- Runtime type information
    TypeName("linearAxialAngularSpring");


    // Constructors

        //- Construct from components
        linearAxialAngularSpring
        (
            const word& name,
            const dictionary& dict,
            const rigidBodyModel& model
        );

        //- Construct and return a clone
        virtual autoPtr<restraint> clone() const
        {
            return autoPtr<restraint>
            (
                new linearAxialAngularSpring(*this)
            );
        }


    //- Destructor
    virtual ~linearAxialAngularSpring();


    // Member Functions

        //- Accumulate the retraint internal joint forces into the tau field and
        //  external forces into the fx field
        virtual void restrain
        (
            scalarField& tau,
            Field<spatialVector>& fx
        ) const;

        //- Update properties from given dictionary
        virtual bool read(const dictionary& dict);

        //- Write
        virtual void write(Ostream&) const;

        //- Write output to a file
        virtual void writeState(autoPtr<OFstream> &stateFilePtr) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RBD
} // End namespace RBD
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
