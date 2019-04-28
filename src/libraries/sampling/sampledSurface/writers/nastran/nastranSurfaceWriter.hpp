/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2015 OpenFOAM Foundation
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
    CML::nastranSurfaceWriter

Description
    A surface writer for the Nastran file format - both surface mesh and fields

    formatOptions
    {
        nastran
        {
            // From CAELUS field name to Nastran field name
            fields ((pMean PLOAD2));
            // Optional scale
            scale 2.0;
            // Optional format
            format free;    //short, long, free
        }
    };

SourceFiles
    nastranSurfaceWriter.cpp

\*---------------------------------------------------------------------------*/

#ifndef nastranSurfaceWriter_H
#define nastranSurfaceWriter_H

#include "surfaceWriter.hpp"
#include "NamedEnum.hpp"
#include "OFstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class nastranSurfaceWriter Declaration
\*---------------------------------------------------------------------------*/

class nastranSurfaceWriter
:
    public surfaceWriter
{
public:

    enum writeFormat
    {
        wfShort,
        wfLong,
        wfFree
    };

    static const NamedEnum<writeFormat, 3> writeFormatNames_;


private:

    // Private data

        //- Write option
        writeFormat writeFormat_;

        //- Map of CAELUS field name vs nastran field name
        HashTable<word> fieldMap_;

        //- Scale to apply to values (default = 1.0)
        scalar scale_;


    // Private Member Functions

        //- Initialise the output stream format params
        void formatOS(OFstream& os) const;

        //- Write a co-ordinate
        void writeCoord
        (
            const point& p,
            const label pointI,
            OFstream& os
        ) const;

        //- Write a face element (CTRIA3 or CQUAD4)
        void writeFace
        (
            const word& faceType,
            const labelList& facePts,
            label& nFace,
            OFstream& os
        ) const;

        //- Main driver to write the surface mesh geometry
        void writeGeometry
        (
            const pointField& points,
            const faceList& faces,
            List<DynamicList<face> >& decomposedFaces,
            OFstream& os
        ) const;

        //- Write a face-based value
        template<class Type>
        void writeFaceValue
        (
            const word& nasFieldName,
            const Type& value,
            const label EID,
            OFstream& os
        ) const;

        //- Templated write operation
        template<class Type>
        void writeTemplate
        (
            const fileName& outputDir,
            const fileName& surfaceName,
            const pointField& points,
            const faceList& faces,
            const word& fieldName,
            const Field<Type>& values,
            const bool isNodeValues,
            const bool verbose
        ) const;


public:

    //- Runtime type information
    TypeName("nastran");


    // Constructors

        //- Construct null
        nastranSurfaceWriter();

        //- Construct with some output options
        nastranSurfaceWriter(const dictionary& options);


    //- Destructor
    virtual ~nastranSurfaceWriter();


    // Member Functions

        //- True if the surface format supports geometry in a separate file.
        //  False if geometry and field must be in a single file
        virtual bool separateGeometry()
        {
            return false;
        }

        //- Write single surface geometry to file.
        virtual void write
        (
            const fileName& outputDir,
            const fileName& surfaceName,
            const pointField& points,
            const faceList& faces,
            const bool verbose = false
        ) const;

        //- Write scalarField for a single surface to file.
        //  One value per face or vertex (isNodeValues = true)
        virtual void write
        (
            const fileName& outputDir,
            const fileName& surfaceName,
            const pointField& points,
            const faceList& faces,
            const word& fieldName,
            const Field<scalar>& values,
            const bool isNodeValues,
            const bool verbose = false
        ) const;

        //- Write vectorField for a single surface to file.
        //  One value per face or vertex (isNodeValues = true)
        virtual void write
        (
            const fileName& outputDir,
            const fileName& surfaceName,
            const pointField& points,
            const faceList& faces,
            const word& fieldName,
            const Field<vector>& values,
            const bool isNodeValues,
            const bool verbose = false
        ) const;

        //- Write sphericalTensorField for a single surface to file.
        //  One value per face or vertex (isNodeValues = true)
        virtual void write
        (
            const fileName& outputDir,
            const fileName& surfaceName,
            const pointField& points,
            const faceList& faces,
            const word& fieldName,
            const Field<sphericalTensor>& values,
            const bool isNodeValues,
            const bool verbose = false
        ) const;

        //- Write symmTensorField for a single surface to file.
        //  One value per face or vertex (isNodeValues = true)
        virtual void write
        (
            const fileName& outputDir,
            const fileName& surfaceName,
            const pointField& points,
            const faceList& faces,
            const word& fieldName,
            const Field<symmTensor>& values,
            const bool isNodeValues,
            const bool verbose = false
        ) const;

        //- Write tensorField for a single surface to file.
        //  One value per face or vertex (isNodeValues = true)
        virtual void write
        (
            const fileName& outputDir,
            const fileName& surfaceName,
            const pointField& points,
            const faceList& faces,
            const word& fieldName,
            const Field<tensor>& values,
            const bool isNodeValues,
            const bool verbose = false
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "OFstream.hpp"
#include "IOmanip.hpp"
#include "OSspecific.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void CML::nastranSurfaceWriter::writeFaceValue
(
    const word& nasFieldName,
    const Type& value,
    const label EID,
    OFstream& os
) const
{
    // Fixed short/long formats:
    // 1 Nastran distributed load type, e.g. PLOAD4
    // 2 SID  : load set ID
    // 3 EID  : element ID
    // 4 onwards: load values

    label SID = 1;

    Type scaledValue = scale_*value;

    switch (writeFormat_)
    {
        case wfShort:
        {
            os.setf(ios_base::left);
            os  << setw(8) << nasFieldName;
            os.unsetf(ios_base::left);
            os.setf(ios_base::right);
            os  << setw(8) << SID
                << setw(8) << EID;

            for (direction dirI = 0; dirI < pTraits<Type>::nComponents; dirI++)
            {
                os  << setw(8) << component(scaledValue, dirI);
            }

            os.unsetf(ios_base::right);

            break;
        }
        case wfLong:
        {
            os.setf(ios_base::left);
            os  << setw(8) << word(nasFieldName + "*");
            os.unsetf(ios_base::left);
            os.setf(ios_base::right);
            os  << setw(16) << SID
                << setw(16) << EID;

            for (direction dirI = 0; dirI < pTraits<Type>::nComponents; dirI++)
            {
                os  << setw(16) << component(scaledValue, dirI);
            }

            os.unsetf(ios_base::right);

            os  << nl;

            os.setf(ios_base::left);
            os  << '*';
            os.unsetf(ios_base::left);

            break;
        }
        case wfFree:
        {
            os  << nasFieldName << ','
                << SID << ','
                << EID;

            for (direction dirI = 0; dirI < pTraits<Type>::nComponents; dirI++)
            {
                os  << ',' << component(scaledValue, dirI);
            }

            break;
        }
        default:
        {
        }
    }

    os << nl;
}


template<class Type>
void CML::nastranSurfaceWriter::writeTemplate
(
    const fileName& outputDir,
    const fileName& surfaceName,
    const pointField& points,
    const faceList& faces,
    const word& fieldName,
    const Field<Type>& values,
    const bool isNodeValues,
    const bool verbose
) const
{
    if (!fieldMap_.found(fieldName))
    {
        WarningInFunction
            << "No mapping found between field " << fieldName
            << " and corresponding Nastran field.  Available types are:"
            << fieldMap_
            << exit(FatalError);

        return;
    }

    const word& nasFieldName(fieldMap_[fieldName]);

    if (!isDir(outputDir/fieldName))
    {
        mkDir(outputDir/fieldName);
    }

    // const scalar timeValue = CML::name(this->mesh().time().timeValue());
    const scalar timeValue = 0.0;

    OFstream os(outputDir/fieldName/surfaceName + ".dat");
    formatOS(os);

    if (verbose)
    {
        Info<< "Writing nastran file to " << os.name() << endl;
    }

    os  << "TITLE=CAELUS " << surfaceName.c_str() << " " << fieldName
        << " data" << nl
        << "$" << nl
        << "TIME " << timeValue << nl
        << "$" << nl
        << "BEGIN BULK" << nl;

    List<DynamicList<face> > decomposedFaces(faces.size());

    writeGeometry(points, faces, decomposedFaces, os);


    os  << "$" << nl
        << "$ Field data" << nl
        << "$" << nl;

    if (isNodeValues)
    {
        label n = 0;

        forAll(decomposedFaces, i)
        {
            const DynamicList<face>& dFaces = decomposedFaces[i];
            forAll(dFaces, faceI)
            {
                Type v = pTraits<Type>::zero;
                const face& f = dFaces[faceI];

                forAll(f, fptI)
                {
                    v += values[f[fptI]];
                }
                v /= f.size();

                writeFaceValue(nasFieldName, v, ++n, os);
            }
        }
    }
    else
    {
        label n = 0;

        forAll(decomposedFaces, i)
        {
            const DynamicList<face>& dFaces = decomposedFaces[i];

            forAll(dFaces, faceI)
            {
                writeFaceValue(nasFieldName, values[faceI], ++n, os);
            }
        }
    }

    os  << "ENDDATA" << endl;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
