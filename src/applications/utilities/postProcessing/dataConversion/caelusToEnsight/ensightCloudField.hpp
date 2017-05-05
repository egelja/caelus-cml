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

InApplication
    caelusToEnsight

Description

SourceFiles
    ensightCloudField.cpp

\*---------------------------------------------------------------------------*/

#ifndef ensightCloudField_H
#define ensightCloudField_H

#include "Cloud.hpp"
#include "IOobject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void ensightCloudField
(
    const IOobject& fieldObject,
    const fileName& postProcPath,
    const word& prepend,
    const label timeIndex,
    const word& timeFile,
    const word& cloudName,
    Ostream& ensightCaseFile,
    const bool dataExists
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "Time.hpp"
#include "IOField.hpp"
#include "OFstream.hpp"
#include "IOmanip.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class Type>
void ensightCloudField
(
    const IOobject& fieldObject,
    const fileName& postProcPath,
    const word& prepend,
    const label timeIndex,
    const word& cloudName,
    Ostream& ensightCaseFile,
    const bool dataExists
)
{
    if (dataExists)
    {
        Info<< "Converting cloud " << cloudName
            << " field " << fieldObject.name() << endl;
    }
    else
    {
        Info<< "Creating empty cloud " << cloudName
            << " field "  << fieldObject.name() << endl;
    }

    word timeFile = prepend + itoa(timeIndex);

    const Time& runTime = fieldObject.time();

    if (timeIndex == 0 && Pstream::master())
    {
        ensightCaseFile
            << pTraits<Type>::typeName << " per measured node:      1       ";
        ensightCaseFile.width(15);
        ensightCaseFile.setf(ios_base::left);
        ensightCaseFile
            << ("c" + fieldObject.name()).c_str()
            << (' ' + prepend + "***." + cloudName
              + "." + fieldObject.name()).c_str()
            << nl;
    }

    fileName ensightFileName
    (
        timeFile + "." + cloudName +"." + fieldObject.name()
    );

    OFstream ensightFile
    (
        postProcPath/ensightFileName,
        runTime.writeFormat(),
        runTime.writeVersion(),
        runTime.writeCompression()
    );

    ensightFile<< pTraits<Type>::typeName << " values" << nl;

    if (dataExists)
    {
        IOField<Type> vf(fieldObject);

        ensightFile.setf(ios_base::scientific, ios_base::floatfield);
        ensightFile.precision(5);

        label count = 0;
        forAll(vf, i)
        {
            Type v = vf[i];

            if (mag(v) < 1.0e-90)
            {
                v = pTraits<Type>::zero;
            }

            for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; cmpt++)
            {
                ensightFile << setw(12) << component(v, cmpt);
                if (++count % 6 == 0)
                {
                    ensightFile << nl;
                }
            }
        }

        if ((count % 6 != 0) || (count==0))
        {
            ensightFile << nl;
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
