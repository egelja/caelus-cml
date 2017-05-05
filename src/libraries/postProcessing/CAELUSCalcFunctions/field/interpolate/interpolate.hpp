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
    CML::calcTypes::interpolate

Description
    Interpolates volume fields to surface fields for each time.

SourceFiles
    interpolate.cpp

\*---------------------------------------------------------------------------*/

#ifndef interpolate_H
#define interpolate_H

#include "calcType.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace calcTypes
{

/*---------------------------------------------------------------------------*\
                          Class interpolate Declaration
\*---------------------------------------------------------------------------*/

class interpolate
:
    public calcType
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        interpolate(const interpolate&);

        //- Disallow default bitwise assignment
        void operator=(const interpolate&);


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

            //- Write interpolate fields
            template<class Type>
            void writeInterpolateField
            (
                const IOobject& header,
                const fvMesh& mesh,
                bool& processed
            );


public:

    //- Runtime type information
    TypeName("interpolate");


    // Constructors

        //- Construct null
        interpolate();


    //- Destructor
    virtual ~interpolate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace calcTypes
} // End namespace CML

template<class Type>
void CML::calcTypes::interpolate::writeInterpolateField
(
    const IOobject& header,
    const fvMesh& mesh,
    bool& processed
)
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> surfaceFieldType;

    if (header.headerClassName() == fieldType::typeName)
    {
        Info<< "    Reading " << header.name() << endl;
        fieldType field(header, mesh);

        Info<< "    Calculating interpolate" << header.name() << endl;
        surfaceFieldType interpolateField
        (
            IOobject
            (
                "interpolate" + header.name(),
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ
            ),
            fvc::interpolate(field)
        );
        interpolateField.write();

        processed = true;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
