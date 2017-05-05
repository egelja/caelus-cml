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
    CML::fieldMinMax

Description
    Calculates scalar minimim and maximum field values.

    For variables with rank > 0, computes the magnitude of the min/max
    values.

    Data written to the file \<timeDir\>/fieldMinMax.dat


SourceFiles
    fieldMinMax.cpp
    IOfieldMinMax.hpp

\*---------------------------------------------------------------------------*/

#ifndef fieldMinMax_H
#define fieldMinMax_H

#include "primitiveFieldsFwd.hpp"
#include "volFieldsFwd.hpp"
#include "HashSet.hpp"
#include "OFstream.hpp"
#include "Switch.hpp"
#include "pointFieldFwd.hpp"
#include "NamedEnum.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                       Class fieldMinMax Declaration
\*---------------------------------------------------------------------------*/

class fieldMinMax
{
public:

    enum modeType
    {
        mdMag,
        mdCmpt
    };

protected:

    // Protected data

        //- Mode type names
        static const NamedEnum<modeType, 2> modeTypeNames_;

        //- Name of this set of field min/max.
        //  Also used as the name of the output directory.
        word name_;

        const objectRegistry& obr_;

        //- on/off switch
        bool active_;

        //- Switch to enable/disable writing to file
        Switch write_;

        //- Switch to send output to Info as well
        Switch log_;

        //- Mode for min/max - only applicable for ranks > 0
        modeType mode_;

        //- Fields to assess min/max
        wordList fieldSet_;

        //- Min/max file ptr
        autoPtr<OFstream> fieldMinMaxFilePtr_;


    // Private Member Functions

        //- If the output file has not been created create it
        void makeFile();

        //- Disallow default bitwise copy construct
        fieldMinMax(const fieldMinMax&);

        //- Disallow default bitwise assignment
        void operator=(const fieldMinMax&);

        //- Output file header information
        virtual void writeFileHeader();


public:

    //- Runtime type information
    TypeName("fieldMinMax");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        fieldMinMax
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~fieldMinMax();


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

        //- Calculate the field min/max
        template<class Type>
        void calcMinMaxFields(const word& fieldName);

        //- Write the fieldMinMax
        virtual void write();

        //- Update for changes of mesh
        virtual void updateMesh(const mapPolyMesh&)
        {}

        //- Update for changes of mesh
        virtual void movePoints(const pointField&)
        {}
};


// Template specialisation for scalar fields
template<>
void fieldMinMax::calcMinMaxFields<scalar>(const word& fieldName);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "volFields.hpp"
#include "dictionary.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void CML::fieldMinMax::calcMinMaxFields(const word& fieldName)
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    if (obr_.foundObject<fieldType>(fieldName))
    {
        const fieldType& field = obr_.lookupObject<fieldType>(fieldName);
        switch (mode_)
        {
            case mdMag:
            {
                const scalar minValue = min(mag(field)).value();
                const scalar maxValue = max(mag(field)).value();

                if (Pstream::master())
                {
                    if (write_)
                    {
                        fieldMinMaxFilePtr_()
                            << obr_.time().value() << tab
                            << fieldName << tab << minValue << tab << maxValue
                            << endl;
                    }

                    if (log_)
                    {
                        Info<< "fieldMinMax output:" << nl
                            << "    min(mag(" << fieldName << ")) = "
                            << minValue << nl
                            << "    max(mag(" << fieldName << ")) = "
                            << maxValue << nl
                            << endl;
                    }
                }
                break;
            }
            case mdCmpt:
            {
                const Type minValue = min(field).value();
                const Type maxValue = max(field).value();

                if (Pstream::master())
                {
                    if (write_)
                    {
                        fieldMinMaxFilePtr_()
                            << obr_.time().value() << tab
                            << fieldName << tab << minValue << tab << maxValue
                            << endl;
                    }

                    if (log_)
                    {
                        Info<< "fieldMinMax output:" << nl
                            << "    cmptMin(" << fieldName << ") = "
                            << minValue << nl
                            << "    cmptMax(" << fieldName << ") = "
                            << maxValue << nl
                            << endl;
                    }
                }
                break;
            }
            default:
            {
                FatalErrorIn
                (
                    "CML::fieldMinMax::calcMinMaxFields"
                    "(const word& fieldName)"
                )<< "Unknown min/max mode: " << modeTypeNames_[mode_]
                 << exit(FatalError);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
