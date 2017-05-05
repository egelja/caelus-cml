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
    CML::fieldToCell

Description
    A topoSetSource to select cells based on field values.

SourceFiles
    fieldToCell.cpp

\*---------------------------------------------------------------------------*/

#ifndef fieldToCell_H
#define fieldToCell_H

#include "topoSetSource.hpp"
#include "scalarField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class fieldToCell Declaration
\*---------------------------------------------------------------------------*/

class fieldToCell
:
    public topoSetSource
{

    // Private data

        //- Add usage string
        static addToUsageTable usage_;


        //- Name of volScalarField
        word fieldName_;

        //- min to subset field values with
        scalar min_;

        //- max ,,
        scalar max_;


    // Private Member Functions

        //- Depending on field values add to or delete from cellSet.
        void applyToSet
        (
            const topoSetSource::setAction action,
            const scalarField& field,
            topoSet& set
        ) const;

public:

    //- Runtime type information
    TypeName("fieldToCell");

    // Constructors

        //- Construct from components
        fieldToCell
        (
            const polyMesh& mesh,
            const word& fieldName,
            const scalar min,
            const scalar max
        );

        //- Construct from dictionary
        fieldToCell
        (
            const polyMesh& mesh,
            const dictionary& dict
        );

        //- Construct from Istream
        fieldToCell
        (
            const polyMesh& mesh,
            Istream&
        );


    //- Destructor
    virtual ~fieldToCell();


    // Member Functions

        virtual sourceType setType() const
        {
            return CELLSETSOURCE;
        }

        virtual void applyToSet
        (
            const topoSetSource::setAction action,
            topoSet&
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
