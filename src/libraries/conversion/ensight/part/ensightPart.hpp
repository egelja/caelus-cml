/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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
    CML::ensightPart

Description
    Base class for ensightPartCells and ensightPartFaces

SourceFiles
    ensightPart.cpp
    ensightPartIO.cpp
    ensightPartTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef ensightPart_H
#define ensightPart_H

#include "ensightFile.hpp"
#include "ensightGeoFile.hpp"
#include "typeInfo.hpp"
#include "labelList.hpp"
#include "polyMesh.hpp"
#include "Field.hpp"
#include "IOPtrList.hpp"
#include "IOstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class ensightPart;

Ostream& operator<<(Ostream&, const ensightPart&);
ensightGeoFile& operator<<(ensightGeoFile&, const ensightPart&);


/*---------------------------------------------------------------------------*\
                         Class ensightPart Declaration
\*---------------------------------------------------------------------------*/

class ensightPart
{
    // Private data

        // Static data members
        static const List<word> elemTypes_;


protected:

    // Protected data

        //- part number
        label number_;

        //- part name (or description)
        string name_;

        //- simple labelList with a name
        labelListList elemLists_;

        //- start offset for elemLists_
        label offset_;

        //- number of elements in this part
        label size_;

        //- cell or face data
        bool isCellData_;

        //- material id (numeric)
        label matId_;

        //- pointField referenced
        const pointField& points_;


    // Protected Classes

        //- track the points used by the part and map global to local indices
        class localPoints
        {
        public:
            //- number of points used
            label nPoints;

            //- map global to local indices
            labelList list;

            //- null constructor
            localPoints()
            :
                nPoints(0),
                list(0)
            {}

            //- construct for mesh points
            localPoints(const pointField& pts)
            :
                nPoints(0),
                list(pts.size(), -1)
            {}
        };


    // Protected Member Functions

        //- Reconstruct part characteristics (eg, element types) from Istream
        //  A part reconstructed in this manner can be used when writing fields,
        //  but cannot be used to write a new geometry
        void reconstruct(Istream&);

        //- check for fully defined fields
        bool isFieldDefined(const List<scalar>&) const;

        //- write the part header
        void writeHeader(ensightFile&, bool withDescription=false) const;

        //- write a scalar field for idList
        //  A null reference for idList writes the perNode values
        void writeFieldList
        (
            ensightFile& os,
            const List<scalar>& field,
            const labelUList& idList
        ) const;

        //- track points used
        virtual localPoints calcLocalPoints() const
        {
            return localPoints();
        }

        //- write connectivities
        virtual void writeConnectivity
        (
            ensightGeoFile&,
            const word& key,
            const labelUList& idList,
            const labelUList& pointMap
        ) const
        {}


public:

    //- Runtime type information
    TypeName("ensightPart");


    // Constructors

        //- Construct null
        ensightPart();

        //- Construct empty part with number and description
        ensightPart(label partNumber, const string& partDescription);

        //- Construct part with number, description and points reference
        ensightPart
        (
            label partNumber,
            const string& partDescription,
            const pointField& points
        );

        //- Construct as copy
        ensightPart(const ensightPart&);


    // Selectors

        // Declare run-time constructor selection table
        declareRunTimeSelectionTable
        (
            autoPtr,
            ensightPart,
            istream,
            (
                Istream& is
            ),
            (is)
        );

        //- Construct and return clone
        autoPtr<ensightPart> clone() const
        {
            return autoPtr<ensightPart>(new ensightPart(*this));
        };

        //- Reconstruct part characteristics on freestore from Istream
        //  \sa reconstruct
        static autoPtr<ensightPart> New(Istream&);


    //- Destructor
    virtual ~ensightPart();


    // Static members

        virtual const List<word>& elementTypes() const
        {
            return elemTypes_;
        }


    // Access

        //- number of elements in this part
        label size() const
        {
            return size_;
        }

        //- represents cell data
        bool isCellData() const
        {
            return isCellData_;
        }

        //- represents face data
        bool isFaceData() const
        {
            return !isCellData_;
        }

        //- part number
        label number() const
        {
            return number_;
        }

        //- part name or description
        const string& name() const
        {
            return name_;
        }

        //- material id
        label materialId() const
        {
            return matId_;
        }

        //- non-const access to part name or description
        void name(const string& value)
        {
            name_ = value;
        }

        //- non-const access to material id
        void materialId(const label value)
        {
            matId_ = value;
        }

        //- simple labelList with a name
        const labelListList& elemLists() const
        {
            return elemLists_;
        }

        //- offset for element ids
        label offset() const
        {
            return offset_;
        }


    // Edit

        //- renumber elements
        void renumber(const labelUList&);

        //- write summary information about the object
        bool writeSummary(Ostream&) const;

        //- write reconstruction information for the object
        bool writeData(Ostream&) const;

        //- Write geometry
        virtual void writeGeometry(ensightGeoFile&) const
        {}

        //- Helper: write geometry given the pointField
        void writeGeometry(ensightGeoFile&, const pointField&) const;

        //- write scalar field
        //  optionally write data per node
        void writeScalarField
        (
            ensightFile&,
            const List<scalar>& field,
            const bool perNode = false
        ) const;

        //- write vector field components
        //  optionally write data per node
        void writeVectorField
        (
            ensightFile&,
            const List<scalar>& field0,
            const List<scalar>& field1,
            const List<scalar>& field2,
            const bool perNode = false
        ) const;


        //- write generalized field components
        //  optionally write data per node
        template<class Type>
        void writeField
        (
            ensightFile&,
            const Field<Type>&,
            const bool perNode = false
        ) const;


    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const ensightPart&)
        {
            NotImplemented;
        }


    // IOstream Operators

        //- write data (reconstruction information)
        friend Ostream& operator<<(Ostream&, const ensightPart&);

        //- write geometry
        friend ensightGeoFile& operator<<(ensightGeoFile&, const ensightPart&);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::ensightPart::writeField
(
    ensightFile& os,
    const Field<Type>& field,
    const bool perNode
) const
{
    if (this->size() && field.size())
    {
        writeHeader(os);

        if (perNode)
        {
            os.writeKeyword("coordinates");
            for
            (
                direction cmpt=0;
                cmpt < pTraits<Type>::nComponents;
                ++cmpt
            )
            {
                writeFieldList(os, field.component(cmpt), labelUList::null());
            }
        }
        else
        {
            forAll(elementTypes(), elemI)
            {
                const labelUList& idList = elemLists_[elemI];

                if (idList.size())
                {
                    os.writeKeyword(elementTypes()[elemI]);

                    for
                    (
                        direction cmpt=0;
                        cmpt < pTraits<Type>::nComponents;
                        ++cmpt
                    )
                    {
                        writeFieldList(os, field.component(cmpt), idList);
                    }
                }
            }
        }
    }
}


#endif

// ************************************************************************* //
