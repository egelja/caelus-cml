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
    CML::lagrangianWriter

Description
    Write fields (internal).

SourceFiles
    lagrangianWriter.cpp
    lagrangianWriterTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef lagrangianWriter_H
#define lagrangianWriter_H

#include "OFstream.hpp"
#include "Cloud.hpp"
#include "volFields.hpp"
#include "pointFields.hpp"
#include "vtkMesh.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class volPointInterpolation;

/*---------------------------------------------------------------------------*\
                           Class lagrangianWriter Declaration
\*---------------------------------------------------------------------------*/

class lagrangianWriter
{
    const vtkMesh& vMesh_;

    const bool binary_;

    const fileName fName_;

    const word cloudName_;

    std::ofstream os_;

    label nParcels_;


public:

    // Constructors

        //- Construct from components
        lagrangianWriter
        (
            const vtkMesh&,
            const bool binary,
            const fileName&,
            const word&,
            const bool dummyCloud
        );


    // Member Functions

        std::ofstream& os()
        {
            return os_;
        }

        void writeParcelHeader(const label nFields);

        //- Write IOField
        template<class Type>
        void writeIOField(const wordList&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "writeFuns.hpp"
#include "IOField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void CML::lagrangianWriter::writeIOField(const wordList& objects)
{
    forAll(objects, i)
    {
        const word& object = objects[i];

        IOobject header
        (
            object,
            vMesh_.mesh().time().timeName(),
            cloud::prefix/cloudName_,
            vMesh_.mesh(),
            IOobject::MUST_READ,
            IOobject::NO_WRITE,
            false
        );

        IOField<Type> fld(header);

        os_ << object << ' ' << int(pTraits<Type>::nComponents) << ' '
            << fld.size() << " float" << std::endl;

        DynamicList<floatScalar> fField(pTraits<Type>::nComponents*fld.size());

        writeFuns::insert(fld, fField);

        writeFuns::write(os_, binary_, fField);
    }
}

#endif

// ************************************************************************* //
