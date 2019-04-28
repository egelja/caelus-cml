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
    CML::regionModels::singleLayerRegion

Description
    Base class for single layer region models


\*---------------------------------------------------------------------------*/

#ifndef singleLayerRegion_H
#define singleLayerRegion_H

#include "regionModel.hpp"

#include "zeroGradientFvPatchFields.hpp"
#include "mappedFixedInternalValueFvPatchFields.hpp"
#include "mappedFixedPushedInternalValueFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{

/*---------------------------------------------------------------------------*\
                      Class singleLayerRegion Declaration
\*---------------------------------------------------------------------------*/

class singleLayerRegion
:
    public regionModel
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        singleLayerRegion(const singleLayerRegion&);

        //- Disallow default bitwise assignment
        void operator=(const singleLayerRegion&);

        //- Construct region mesh and fields
        void constructMeshObjects();

        //- Initialise the region
        void initialise();


protected:

    // Protected data

        // Region addressing

            //- Patch normal vectors
            autoPtr<volVectorField> nHatPtr_;

            //- Face area magnitudes / [m2]
            autoPtr<volScalarField> magSfPtr_;


        // Addressing

            //- List of patch IDs opposite to internally coupled patches
            labelList passivePatchIDs_;


    // Protected member functions

        //- Read control parameters from dictionary
        virtual bool read();


public:

    //- Runtime type information
    TypeName("regionModel");


    // Constructors

        //- Construct null
        singleLayerRegion(const fvMesh& mesh, const word& regionType);

        //- Construct from mesh, region type and name
        singleLayerRegion
        (
            const fvMesh& mesh,
            const word& regionType,
            const word& modelName,
            bool readFields = true
        );


    //- Destructor
    virtual ~singleLayerRegion();


    // Member Functions

        // Access

            // Region geometry

                //- Return the patch normal vectors
                virtual const volVectorField& nHat() const;

                //- Return the face area magnitudes / [m2]
                virtual const volScalarField& magSf() const;


            // Addressing

                //- Return the list of patch IDs opposite to internally
                //  coupled patches
                virtual const labelList& passivePatchIDs() const;


        // Patch type information

            //- Return boundary types for mapped field patches
            //  Also maps internal field value
            //  Mapping region prescribed by underlying mapped poly patch
            template<class Type>
            wordList mappedFieldAndInternalPatchTypes() const;

            //- Return boundary types for pushed mapped field patches
            //  Mapping region prescribed by underlying mapped poly patch
            template<class Type>
            wordList mappedPushedFieldPatchTypes() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace regionModels
} // End namespace CML


// * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //

template<class Type>
CML::wordList
CML::regionModels::singleLayerRegion::mappedFieldAndInternalPatchTypes() const
{
    wordList bTypes(regionMesh().boundaryMesh().size());

    bTypes = zeroGradientFvPatchField<Type>::typeName;

    forAll(intCoupledPatchIDs_, i)
    {
        const label patchi = intCoupledPatchIDs_[i];
        bTypes[patchi] = mappedFixedInternalValueFvPatchField<Type>::typeName;
    }

    return bTypes;
}


template<class Type>
CML::wordList
CML::regionModels::singleLayerRegion::mappedPushedFieldPatchTypes() const
{
    wordList bTypes(regionMesh().boundaryMesh().size());

    bTypes = zeroGradientFvPatchField<Type>::typeName;

    forAll(intCoupledPatchIDs_, i)
    {
        const label patchi = intCoupledPatchIDs_[i];
        bTypes[patchi] =
            mappedFixedPushedInternalValueFvPatchField<Type>::typeName;
    }

    return bTypes;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
