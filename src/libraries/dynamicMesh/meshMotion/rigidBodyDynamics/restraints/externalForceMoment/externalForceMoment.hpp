/*---------------------------------------------------------------------------*\
Copyright (C) 2017 Applied CCM Pty Ltd
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
    CML::RBD::restrains::externalForce

Description
    External force and moment.

SourceFiles
    externalForce.cpp

\*---------------------------------------------------------------------------*/

#ifndef RBD_restraints_externalForce_hpp
#define RBD_restraints_externalForce_hpp

#include "rigidBodyRestraint.hpp"
#include "point.hpp"
#include "Switch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
namespace restraints
{

/*---------------------------------------------------------------------------*\
                   Class externalForce Declaration
\*---------------------------------------------------------------------------*/
class externalForceMoment
:
    public restraint
{
    // Private data

        //- Reference point of application to the solid body
        point refApplicationPt_;

        //- Is force in global frame
        Switch global_;

        //- Force on body (N)
        vector force_;

        //- Moment on body (Nm)
        vector moment_;


public:

    //- Runtime type information
    TypeName("externalForceMoment");


    // Constructors

        //- Construct from components
        externalForceMoment
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
                new externalForceMoment(*this)
            );
        }


    //- Destructor
    virtual ~externalForceMoment();


    // Member Functions

        //- Accumulate the load forces into the tau field and
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
} // End namespace restraints
} // End namespace RBD
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
