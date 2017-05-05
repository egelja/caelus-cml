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

InClass
    CML::makeSurfaceWriterMethods

Description
    Convenience macros for instantiating writer methods for surfaceWriter
    classes.

\*---------------------------------------------------------------------------*/

#ifndef makeSurfaceWriterMethods_H
#define makeSurfaceWriterMethods_H

#include "surfaceWriter.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeSurfaceWriterType(ThisClass)                                      \
    defineTypeNameAndDebug(ThisClass, 0);                                     \
    addToRunTimeSelectionTable(surfaceWriter, ThisClass, word)


#define defineSurfaceWriterWriteField(ThisClass, FieldType)                   \
    void ThisClass::write                                                     \
    (                                                                         \
        const fileName& outputDir,                                            \
        const fileName& surfaceName,                                          \
        const pointField& points,                                             \
        const faceList& faces,                                                \
        const word& fieldName,                                                \
        const Field<FieldType>& values,                                       \
        const bool isNodeValues,                                              \
        const bool verbose                                                    \
    ) const                                                                   \
    {                                                                         \
        writeTemplate                                                         \
        (                                                                     \
            outputDir,                                                        \
            surfaceName,                                                      \
            points,                                                           \
            faces,                                                            \
            fieldName,                                                        \
            values,                                                           \
            isNodeValues,                                                     \
            verbose                                                           \
        );                                                                    \
    }


#define defineSurfaceWriterWriteFields(ThisClass)                             \
    defineSurfaceWriterWriteField(ThisClass, scalar);                         \
    defineSurfaceWriterWriteField(ThisClass, vector);                         \
    defineSurfaceWriterWriteField(ThisClass, sphericalTensor);                \
    defineSurfaceWriterWriteField(ThisClass, symmTensor);                     \
    defineSurfaceWriterWriteField(ThisClass, tensor)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
