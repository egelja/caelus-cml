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
    CML::calcTypes::magSqr

Description
    Calculates and writes the magnitude-sqaured of a field for each time

SourceFiles
    magSqr.cpp

\*---------------------------------------------------------------------------*/

#ifndef magSqr_H
#define magSqr_H

#include "calcType.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace calcTypes
{

/*---------------------------------------------------------------------------*\
                          Class magSqr Declaration
\*---------------------------------------------------------------------------*/

class magSqr
:
    public calcType
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        magSqr(const magSqr&);

        //- Disallow default bitwise assignment
        void operator=(const magSqr&);


protected:

    // Member Functions

        // Calculation routines

            //- Initialise - typically setting static variables,
            //  e.g. command line arguments
            virtual void init();

            //- Pre-time loop calculations
            virtual void preCalc
            (
                const argList& args,
                const Time& runTime,
                const fvMesh& mesh
            );

            //- Time loop calculations
            virtual void calc
            (
                const argList& args,
                const Time& runTime,
                const fvMesh& mesh
            );


        // I-O

            //- Write component fields
            template<class Type>
            void writeMagSqrField
            (
                const IOobject& header,
                const fvMesh& mesh,
                bool& processed
            );


public:

    //- Runtime type information
    TypeName("magSqr");


    // Constructors

        //- Construct null
        magSqr();


    //- Destructor
    virtual ~magSqr();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace calcTypes
} // End namespace CML

template<class Type>
void CML::calcTypes::magSqr::writeMagSqrField
(
    const IOobject& header,
    const fvMesh& mesh,
    bool& processed
)
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    if (header.headerClassName() == fieldType::typeName)
    {
        Info<< "    Reading " << header.name() << endl;
        fieldType field(header, mesh);

        Info<< "    Calculating magSqr" << header.name() << endl;
        volScalarField magSqrField
        (
            IOobject
            (
                "magSqr" + header.name(),
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ
            ),
            CML::magSqr(field)
        );
        magSqrField.write();

        processed = true;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
