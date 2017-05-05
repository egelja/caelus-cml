/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2013 OpenFOAM Foundation
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
    CML::calcFvcGrad

Group
    grpFVFunctionObjects

Description
    This function object calculates the gradient of a field.  The operation is
    limited to scalar and vector volume or surface fields, and the output is a
    volume vector or tensor field.

SourceFiles
    calcFvcGrad.cpp
    IOcalcFvcGrad.hpp

\*---------------------------------------------------------------------------*/

#ifndef calcFvcGrad_H
#define calcFvcGrad_H

#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "pointFieldFwd.hpp"
#include "OFstream.hpp"
#include "Switch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class dimensionSet;

/*---------------------------------------------------------------------------*\
                         Class calcFvcGrad Declaration
\*---------------------------------------------------------------------------*/

class calcFvcGrad
{
    // Private data

        //- Name of this calcFvcGrad object
        word name_;

        //- Reference to the database
        const objectRegistry& obr_;

        //- On/off switch
        bool active_;

        //- Name of field to process
        word fieldName_;

        //- Name of result field
        word resultName_;


    // Private Member Functions

        //- Helper function to create/store/return the gradient field
        template<class Type>
        GeometricField
        <
            typename outerProduct<vector, Type>::type,
            fvPatchField,
            volMesh
        >&
        gradField(const word& gradName, const dimensionSet& dims);

        //- Helper function to calculate the gradient of different field types
        template<class Type>
        void calcGrad
        (
            const word& fieldName,
            const word& resultName,
            bool& processed
        );

        //- Disallow default bitwise copy construct
        calcFvcGrad(const calcFvcGrad&);

        //- Disallow default bitwise assignment
        void operator=(const calcFvcGrad&);


public:

    //- Runtime type information
    TypeName("calcFvcGrad");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        calcFvcGrad
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~calcFvcGrad();


    // Member Functions

        //- Return name of the set of calcFvcGrad
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the calcFvcGrad data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Calculate the calcFvcGrad and write
        virtual void write();

        //- Update for changes of mesh
        virtual void updateMesh(const mapPolyMesh&)
        {}

        //- Update for changes of mesh
        virtual void movePoints(const pointField&)
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvMesh.hpp"
#include "fvcGrad.hpp"

template<class Type>
CML::GeometricField
<
    typename CML::outerProduct<CML::vector, Type>::type,
    CML::fvPatchField,
    CML::volMesh
>&
CML::calcFvcGrad::gradField(const word& gradName, const dimensionSet& dims)
{
    typedef typename outerProduct<vector, Type>::type gradType;
    typedef GeometricField<gradType, fvPatchField, volMesh> vfGradType;

    const fvMesh& mesh = refCast<const fvMesh>(obr_);

    if (!mesh.foundObject<vfGradType>(gradName))
    {
        vfGradType* gradFieldPtr
        (
            new vfGradType
            (
                IOobject
                (
                    gradName,
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensioned<gradType>
                (
                    "zero",
                    dims/dimLength,
                    pTraits<gradType>::zero
                )
            )
        );

        mesh.objectRegistry::store(gradFieldPtr);
    }

    const vfGradType& field = mesh.lookupObject<vfGradType>(gradName);

    return const_cast<vfGradType&>(field);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::calcFvcGrad::calcGrad
(
    const word& fieldName,
    const word& resultName,
    bool& processed
)
{
    typedef GeometricField<Type, fvPatchField, volMesh> vfType;
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> sfType;

    typedef typename outerProduct<vector, Type>::type gradType;
    typedef GeometricField<gradType, fvPatchField, volMesh> vfGradType;

    const fvMesh& mesh = refCast<const fvMesh>(obr_);


    if (mesh.foundObject<vfType>(fieldName))
    {
        const vfType& vf = mesh.lookupObject<vfType>(fieldName);

        vfGradType& field = gradField<Type>(resultName, vf.dimensions());

        field = fvc::grad(vf);

        processed = true;
    }
    else if (mesh.foundObject<sfType>(fieldName))
    {
        const sfType& sf = mesh.lookupObject<sfType>(fieldName);

        vfGradType& field = gradField<Type>(resultName, sf.dimensions());

        field = fvc::grad(sf);

        processed = true;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
