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

Description
    Miscellaneous collection of functions and template related to Ensight data

SourceFiles
    ensightOutputFunctions.cpp

\*---------------------------------------------------------------------------*/

#ifndef ensightOutputFunctions_H
#define ensightOutputFunctions_H

#include "ensightFile.hpp"
#include "Cloud.hpp"
#include "polyMesh.hpp"
#include "IOobject.hpp"

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void ensightCaseEntry
(
    OFstream& caseFile,
    const string& ensightType,
    const word& fieldName,
    const fileName& dataMask,
    const fileName& local=fileName::null,
    const label cloudNo=-1,
    const label timeSet=1
);


void ensightParticlePositions
(
    const polyMesh& mesh,
    const fileName& dataDir,
    const fileName& subDir,
    const word& cloudName,
    IOstream::streamFormat format
);


//- write lagrangian parcels
template<class Type>
void ensightLagrangianField
(
    const IOobject& fieldObject,
    const fileName& dataDir,
    const fileName& subDir,
    const word& cloudName,
    IOstream::streamFormat format
);

//- write generalized field components
template<class Type>
void ensightVolField
(
    const ensightParts& partsList,
    const IOobject& fieldObject,
    const fvMesh& mesh,
    const fileName& dataDir,
    const fileName& subDir,
    IOstream::streamFormat format
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // namespace CML

#include "passiveParticle.hpp"
#include "IOField.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

#include "OFstream.hpp"
#include "IOmanip.hpp"


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

void CML::ensightCaseEntry
(
    OFstream& caseFile,
    const string& ensightType,
    const word& fieldName,
    const fileName& dataMask,
    const fileName& local,
    const label cloudNo,
    const label timeSet
)
{
    caseFile.setf(ios_base::left);

    fileName dirName(dataMask);
    if (local.size())
    {
        dirName = dirName/local;
    }

    if (cloudNo >= 0)
    {
        label ts = 1;
        if (timeSet > ts)
        {
            ts = timeSet;
        }

        // prefix variables with 'c' (cloud)
        caseFile
            << ensightType.c_str()
            << " per measured node: " << ts << " "
            << setw(15)
            << ("c" + CML::name(cloudNo) + fieldName).c_str()
            << " "
            << (dirName/fieldName).c_str()
            << nl;
    }
    else
    {
        caseFile
            << ensightType.c_str()
            << " per element: "
            << setw(15) << fieldName
            << " "
            << (dirName/fieldName).c_str()
            << nl;
    }
}


void CML::ensightParticlePositions
(
    const polyMesh& mesh,
    const fileName& dataDir,
    const fileName& subDir,
    const word& cloudName,
    IOstream::streamFormat format
)
{
    Cloud<passiveParticle> parcels(mesh, cloudName, false);

    fileName cloudDir = subDir/cloud::prefix/cloudName;
    fileName postFileName = cloudDir/"positions";

    // the ITER/lagrangian subdirectory must exist
    mkDir(dataDir/cloudDir);
    ensightFile os(dataDir/postFileName, format);

    // tag binary format (just like geometry files)
    os.writeBinaryHeader();
    os.write(postFileName);
    os.newline();
    os.write("particle coordinates");
    os.newline();
    os.write(parcels.size(), 8);   // unusual width
    os.newline();

    // binary write is Ensight6 - first ids, then positions
    if (format == IOstream::BINARY)
    {
        forAll(parcels, i)
        {
            os.write(i+1);
        }

        forAllConstIter(Cloud<passiveParticle>, parcels, elmnt)
        {
            const vector& p = elmnt().position();

            os.write(p.x());
            os.write(p.y());
            os.write(p.z());
        }
    }
    else
    {
        label nParcels = 0;

        forAllConstIter(Cloud<passiveParticle>, parcels, elmnt)
        {
            const vector& p = elmnt().position();

            os.write(++nParcels, 8);    // unusual width
            os.write(p.x());
            os.write(p.y());
            os.write(p.z());
            os.newline();
        }
    }
}



template<class Type>
void CML::ensightLagrangianField
(
    const IOobject& fieldObject,
    const fileName& dataDir,
    const fileName& subDir,
    const word& cloudName,
    IOstream::streamFormat format
)
{
    Info<< " " << fieldObject.name() << flush;

    fileName cloudDir = subDir/cloud::prefix/cloudName;
    fileName postFileName = cloudDir/fieldObject.name();

    string title =
        postFileName + " with " + pTraits<Type>::typeName + " values";

    ensightFile os(dataDir/postFileName, format);
    os.write(title);
    os.newline();

    IOField<Type> field(fieldObject);

    // 6 values per line
    label count = 0;

    forAll(field, i)
    {
        Type val = field[i];

        if (mag(val) < 1.0e-90)
        {
            val = pTraits<Type>::zero;
        }

        for (direction cmpt=0; cmpt < pTraits<Type>::nComponents; cmpt++)
        {
            os.write( component(val, cmpt) );
        }

        count += pTraits<Type>::nComponents;

        if (count % 6 == 0)
        {
            os.newline();
        }
    }

    // add final newline if required
    if (count % 6)
    {
        os.newline();
    }
}


//- write generalized field components
template <class Type>
void CML::ensightVolField
(
    const ensightParts& partsList,
    const IOobject& fieldObject,
    const fvMesh& mesh,
    const fileName& dataDir,
    const fileName& subDir,
    IOstream::streamFormat format
)
{
    Info<< " " << fieldObject.name() << flush;

    fileName postFileName = subDir/fieldObject.name();

    ensightFile os(dataDir/postFileName, format);
    os.write(postFileName);
    os.newline();

    // ie, volField<Type>
    partsList.writeField
    (
        os,
        GeometricField<Type, fvPatchField, volMesh>
        (
            fieldObject,
            mesh
        )
    );
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
