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
    CML::calcTypes::randomise

Description
    Adds a random component to a field, with a given perturbation magnitude.

SourceFiles
    randomise.cpp

\*---------------------------------------------------------------------------*/

#ifndef randomise_H
#define randomise_H

#include "calcType.hpp"
#include "Random.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace calcTypes
{

/*---------------------------------------------------------------------------*\
                         Class randomise Declaration
\*---------------------------------------------------------------------------*/

class randomise
:
    public calcType
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        randomise(const randomise&);

        //- Disallow default bitwise assignment
        void operator=(const randomise&);


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
            void writeRandomField
            (
                const IOobject& header,
                const scalar pertMag,
                Random& rand,
                const fvMesh& mesh,
                bool& processed
            );


public:

    //- Runtime type information
    TypeName("randomise");


    // Constructors

        //- Construct null
        randomise();


    //- Destructor
    virtual ~randomise();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace calcTypes
} // End namespace CML

template <class Type>
void CML::calcTypes::randomise::writeRandomField
(
    const IOobject& header,
    const scalar pertMag,
    Random& rand,
    const fvMesh& mesh,
    bool& processed
)
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    if (header.headerClassName() == fieldType::typeName)
    {
        Info<< "    Reading " << header.name() << endl;
        fieldType field(header, mesh);

        forAll(field, cellI)
        {
            Type rndPert = rand.sample01<Type>();
            rndPert = 2.0*rndPert - pTraits<Type>::one;
            rndPert /= mag(rndPert);
            field[cellI] += pertMag*rndPert;
        }

        fieldType randomisedField
        (
            IOobject
            (
                header.name() + "Random",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ
            ),
            field
        );

        Info<< "    Writing " << header.name() << "Random" << endl;
        randomisedField.write();

        processed = true;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
