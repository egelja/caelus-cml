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

Namespace
    CML::compressibilityModels

Description
    Namespace for compressibility models.


Class
    CML::barotropicCompressibilityModel

Description
    Abstract class for barotropic compressibility models

SourceFiles
    barotropicCompressibilityModel.cpp
    newbarotropicCompressibilityModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef barotropicCompressibilityModel_H
#define barotropicCompressibilityModel_H

#include "IOdictionary.hpp"
#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"
#include "volFields.hpp"
#include "dimensionedScalar.hpp"
#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                Class barotropicCompressibilityModel Declaration
\*---------------------------------------------------------------------------*/

class barotropicCompressibilityModel
{

protected:

    dictionary compressibilityProperties_;

    volScalarField psi_;
    const volScalarField& gamma_;

    //- Disallow copy construct
    barotropicCompressibilityModel(const barotropicCompressibilityModel&);

    //- Disallow default bitwise assignment
    void operator=(const barotropicCompressibilityModel&);


public:

    //- Runtime type information
    TypeName("barotropicCompressibilityModel");


    // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        barotropicCompressibilityModel,
        dictionary,
        (
            const dictionary& compressibilityProperties,
            const volScalarField& gamma,
            const word& psiName
        ),
        (compressibilityProperties, gamma, psiName)
    );


    // Selectors
    //- Return a reference to the selected compressibility model
    static autoPtr<barotropicCompressibilityModel> New
    (
        const dictionary& compressibilityProperties,
        const volScalarField& gamma,
        const word& psiName = "psi"
    );


    //- Construct from components
    barotropicCompressibilityModel
    (
        const dictionary& compressibilityProperties,
        const volScalarField& gamma,
        const word& psiName = "psi"
    );


    //- Destructor
    virtual ~barotropicCompressibilityModel()
    {}


    // Member Functions

    //- Return the phase transport properties dictionary
    const dictionary& compressibilityProperties() const
    {
        return compressibilityProperties_;
    }

    //- Return the compressibility
    const volScalarField& psi() const
    {
        return psi_;
    }

    //- Correct the compressibility
    virtual void correct() = 0;

    //- Read compressibilityProperties dictionary
    virtual bool read(const dictionary& compressibilityProperties) = 0;

};


} // End namespace CML


#endif
