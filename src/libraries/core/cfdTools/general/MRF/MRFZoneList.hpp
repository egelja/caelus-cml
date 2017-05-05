/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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
    CML::MRFZoneList

Description
    List container for MRF zones

\*---------------------------------------------------------------------------*/

#ifndef MRFZoneList_H
#define MRFZoneList_H

#include "fvMesh.hpp"
#include "dictionary.hpp"
#include "fvMatricesFwd.hpp"
#include "MRFZone.hpp"

namespace CML
{

// Forward declaration of friend functions and operators
class MRFZoneList;
Ostream& operator<<(Ostream& os, const MRFZoneList& models);

class MRFZoneList
:
    PtrList<MRFZone>
{
private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        MRFZoneList(const MRFZoneList&);

        //- Disallow default bitwise assignment
        void operator=(const MRFZoneList&);


protected:

    // Protected data

        //- Reference to the mesh database
        const fvMesh& mesh_;


public:

    //- Constructor
    MRFZoneList(const fvMesh& mesh, const dictionary& dict);

    //- Destructor
    ~MRFZoneList();


    // Member Functions

        //- Return active status
        bool active(const bool warn = false) const;

        //- Reset the source list
        void reset(const dictionary& dict);

        //- Add the Coriolis force contribution to the acceleration field
        void addCoriolis(const volVectorField& U, volVectorField& ddtU) const;

        //- Add the Coriolis force contribution to the momentum equation
        void addCoriolis(fvVectorMatrix& UEqn) const;

        //- Add the Coriolis force contribution to the momentum equation
        void addCoriolis(const volScalarField& rho, fvVectorMatrix& UEqn) const;

        //- Return the frame acceleration
        tmp<volVectorField> operator()
        (
            const volVectorField& U
        );

        //- Return the frame acceleration force
        tmp<volVectorField> operator()
        (
            const volScalarField& rho,
            const volVectorField& U
        );

        //- Make the given absolute velocity relative within the MRF region
        void makeRelative(volVectorField& U) const;

        //- Make the given relative velocity absolute within the MRF region
        void makeAbsolute(volVectorField& U) const;

        //- Make the given absolute flux relative within the MRF region
        void makeRelative(surfaceScalarField& phi) const;

        //- Return the given absolute boundary flux relative within
        //  the MRF region
        tmp<FieldField<fvsPatchField, scalar> > relative
        (
            const tmp<FieldField<fvsPatchField, scalar> >& tphi
        ) const;

        //- Make the given absolute mass-flux relative within the MRF region
        void makeRelative
        (
            const surfaceScalarField& rho,
            surfaceScalarField& phi
        ) const;

        //- Make the given relative flux absolute within the MRF region
        void makeAbsolute(surfaceScalarField& phi) const;

        //- Make the given relative mass-flux absolute within the MRF region
        void makeAbsolute
        (
            const surfaceScalarField& rho,
            surfaceScalarField& phi
        ) const;

        //- Correct the boundary velocity for the roation of the MRF region
        void correctBoundaryVelocity(volVectorField& U) const;


        // I-O

            //- Read dictionary
            bool read(const dictionary& dict);

            //- Write data to Ostream
            bool writeData(Ostream& os) const;

            //- Ostream operator
            friend Ostream& operator<<
            (
                Ostream& os,
                const MRFZoneList& models
            );
};

} // End namespace CML

#endif

