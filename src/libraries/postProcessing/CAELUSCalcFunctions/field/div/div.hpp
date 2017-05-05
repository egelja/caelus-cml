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
    CML::calcTypes::div

Description
    Writes scalar fields corresponding to the divergence of the supplied
    field (name) for each time.

SourceFiles
    div.cpp

\*---------------------------------------------------------------------------*/

#ifndef div_H
#define div_H

#include "calcType.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace calcTypes
{

/*---------------------------------------------------------------------------*\
                          Class div Declaration
\*---------------------------------------------------------------------------*/

class div
:
    public calcType
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        div(const div&);

        //- Disallow default bitwise assignment
        void operator=(const div&);


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

            //- Write div fields
            template<class Type>
            void writeDivField
            (
                const IOobject& header,
                const fvMesh& mesh,
                bool& processed
            );


public:

    //- Runtime type information
    TypeName("div");


    // Constructors

        //- Construct null
        div();


    //- Destructor
    virtual ~div();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace calcTypes
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void CML::calcTypes::div::writeDivField
(
    const IOobject& header,
    const fvMesh& mesh,
    bool& processed
)
{
    if (header.headerClassName() == Type::typeName)
    {
        Info<< "    Reading " << header.name() << endl;
        Type field(header, mesh);

        Info<< "    Calculating div" << header.name() << endl;
        volScalarField divField
        (
            IOobject
            (
                "div" + header.name(),
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ
            ),
            fvc::div(field)
        );
        divField.write();

        processed = true;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
