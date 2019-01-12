/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
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
    CML::residuals

Description
    This function object writes out the initial residual for specified fields.

    Example of function object specification:
    \verbatim
    residuals
    {
        type            residuals;
        outputControl   timeStep;
        outputInterval  1;
        fields
        (
            U
            p
        );
    }
    \endverbatim

    Output data is written to the dir postProcessing/residuals/\<timeDir\>/
    For vector/tensor fields, e.g. U, where an equation is solved for each
    component, the largest residual of each component is written out.

SeeAlso
    CML::functionObject
    CML::OutputFilterFunctionObject

SourceFiles
    residuals.cpp
    IOresiduals.hpp

\*---------------------------------------------------------------------------*/

#ifndef residuals_H
#define residuals_H

#include "functionObjectFile.hpp"
#include "primitiveFieldsFwd.hpp"
#include "volFieldsFwd.hpp"
#include "HashSet.hpp"
#include "OFstream.hpp"
#include "Switch.hpp"
#include "NamedEnum.hpp"
#include "lduMatrix.hpp"
#include "pointFieldFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                       Class residuals Declaration
\*---------------------------------------------------------------------------*/

class residuals
:
    public functionObjectFile
{
protected:

    // Protected data

        //- Name of this set of residuals
        //  Also used as the name of the output directory
        word name_;

        const objectRegistry& obr_;

        //- on/off switch
        bool active_;

        //- Fields to write residuals
        wordList fieldSet_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        residuals(const residuals&);

        //- Disallow default bitwise assignment
        void operator=(const residuals&);

        //- Output file header information
        virtual void writeFileHeader(const label i);


public:

    //- Runtime type information
    TypeName("residuals");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        residuals
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~residuals();


    // Member Functions

        //- Return name of the set of field min/max
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the field min/max data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Calculate the field min/max
        template<class Type>
        void writeResidual(const word& fieldName);

        //- Write the residuals
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

#include "residuals.hpp"
#include "volFields.hpp"
#include "dictionary.hpp"
#include "Time.hpp"
#include "ListOps.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void CML::residuals::writeResidual
(
    const word& fieldName
)
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    if (obr_.foundObject<fieldType>(fieldName))
    {
        const fieldType& field = obr_.lookupObject<fieldType>(fieldName);
        const fvMesh& mesh = field.mesh();
        const CML::dictionary& solverDict = mesh.solverPerformanceDict();

        if (solverDict.found(fieldName))
        {
            const List<solverPerformance> sp(solverDict.lookup(fieldName));
            const scalar residual = sp.first().initialResidual();
            file() << token::TAB << residual;
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
