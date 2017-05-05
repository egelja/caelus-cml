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
    CML::ExplicitSetValue

Description
    Explicit set values on fields.

    Sources described by:

        <Type>ExplicitSetValueCoeffs
        {
            injectionRate
            {
                k           30.7;
                epsilon     1.5;
            }
        }


SourceFiles
    ExplicitSetValue_.cpp

\*---------------------------------------------------------------------------*/

#ifndef ExplicitSetValue__H
#define ExplicitSetValue__H

#include "basicSource.hpp"
#include "Tuple2.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class explicitSetValue Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class ExplicitSetValue
:
    public basicSource
{

protected:

    // Protected data

        //- Source value per field
        List<Type> injectionRate_;


    // Protected functions

        //- Set the local field data
        void setFieldData(const dictionary& dict);


public:

    //- Runtime type information
    TypeName("explicitSetValue");


    // Constructors

        //- Construct from components
        ExplicitSetValue
        (
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );


    // Member Functions

        // Evaluation

            //- Set value on vector field
            virtual void setValue(fvMatrix<Type>& eqn, const label fieldI);


        // I-O

            //- Write the source properties
            virtual void writeData(Ostream&) const;

            //- Read source dictionary
            virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvMesh.hpp"
#include "fvMatrices.hpp"
#include "DimensionedField.hpp"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class Type>
void CML::ExplicitSetValue<Type>::setFieldData(const dictionary& dict)
{
    fieldNames_.setSize(dict.toc().size());
    injectionRate_.setSize(fieldNames_.size());

    applied_.setSize(fieldNames_.size(), false);

    label i = 0;
    forAllConstIter(dictionary, dict, iter)
    {
        fieldNames_[i] = iter().keyword();
        dict.lookup(iter().keyword()) >> injectionRate_[i];
        i++;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::ExplicitSetValue<Type>::ExplicitSetValue
(
    const word& name,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    basicSource(name, modelType, dict, mesh),
    injectionRate_()
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::ExplicitSetValue<Type>::setValue
(
    fvMatrix<Type>& eqn,
    const label fieldI
)
{
    if (debug)
    {
        Info<< "ExplicitSetValue<"<< pTraits<Type>::typeName
            << ">::setValue for source " << name_ << endl;
    }

    List<Type> values(cells_.size());

    UIndirectList<Type>(values, cells_) = injectionRate_[fieldI];

    eqn.setValues(cells_, values);
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::ExplicitSetValue<Type>::writeData(Ostream& os) const
{
    os  << indent << name_ << endl;
    dict_.write(os);
}


template<class Type>
bool CML::ExplicitSetValue<Type>::read(const dictionary& dict)
{
    if (basicSource::read(dict))
    {
        setFieldData(coeffs_.subDict("injectionRate"));
        return true;
    }
    else
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
