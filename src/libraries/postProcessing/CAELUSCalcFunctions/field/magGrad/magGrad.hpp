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
    CML::calcTypes::magGrad

Description
    Writes scalar fields corresponding to the magnitude ot the gradient
    of the supplied field (name) for each time.

SourceFiles
    magGrad.cpp

\*---------------------------------------------------------------------------*/

#ifndef magGrad_H
#define magGrad_H

#include "calcType.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace calcTypes
{

/*---------------------------------------------------------------------------*\
                          Class magGrad Declaration
\*---------------------------------------------------------------------------*/

class magGrad
:
    public calcType
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        magGrad(const magGrad&);

        //- Disallow default bitwise assignment
        void operator=(const magGrad&);


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

            //- Write magGrad fields
            template<class Type>
            void writeMagGradField
            (
                const IOobject& header,
                const fvMesh& mesh,
                bool& processed
            );


public:

    //- Runtime type information
    TypeName("magGrad");


    // Constructors

        //- Construct null
        magGrad();


    //- Destructor
    virtual ~magGrad();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace calcTypes
} // End namespace CML

template<class Type>
void CML::calcTypes::magGrad::writeMagGradField
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

        Info<< "    Calculating magGrad" << header.name() << endl;
        volScalarField magGradField
        (
            IOobject
            (
                "magGrad" + header.name(),
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ
            ),
            CML::mag(fvc::grad(field))
        );
        magGradField.write();

        processed = true;
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
