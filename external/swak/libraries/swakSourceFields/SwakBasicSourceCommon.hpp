/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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
    CML::expressionSource

Description
    Generates a field according to an expression

SourceFiles
    expressionSource.cpp

Contributors/Copyright:
    2010, 2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef expressionSource_H
#define expressionSource_H

#include "FieldValueExpressionDriver.hpp"
#include "swakFvOption.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class expressionSource Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class SwakBasicSourceCommon
:
    public swakFvOptionType
{
    // Private data

    SwakBasicSourceCommon(const SwakBasicSourceCommon&);

    FieldValueExpressionDriver driver_;

protected:

    List<exprString> expressions_;

    HashTable<dimensionSet,label> dimensions_;

    bool verbose_;

    FieldValueExpressionDriver &driver() { return driver_; }

    // Protected functions

        //- Set the local field data
        void setFieldData(const dictionary& dict);

public:

    //- Runtime type information
    //    TypeName("SwakBasicSourceCommon");

    // Constructors

        //- Construct from a dictionary
        SwakBasicSourceCommon
        (
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );

    // Destructor

        virtual ~SwakBasicSourceCommon();

    typedef GeometricField<T,fvPatchField,volMesh> resultField;

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

namespace CML {

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from dictionary
template<class T>
SwakBasicSourceCommon<T>::SwakBasicSourceCommon
(
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
)
:
    swakFvOptionType(name, modelType, dict, mesh),
    driver_(coeffs(),mesh),
    verbose_(dict.lookupOrDefault<bool>("verbose",true))
{
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class T>
SwakBasicSourceCommon<T>::~SwakBasicSourceCommon()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
void SwakBasicSourceCommon<T>::setFieldData(const dictionary& dict)
{
    fieldNames_.setSize(dict.toc().size());
    expressions_.setSize(fieldNames_.size());

    applied_.setSize(fieldNames_.size(), false);

    label i = 0;
    forAllConstIter(dictionary, dict, iter)
    {
        fieldNames_[i] = iter().keyword();
        dimensionSet dimension(dimless);

        ITstream in(dict.lookup(iter().keyword()));

        expressions_[i]=exprString(
            in,
            dict
        );
        in >> dimension;

        dimensions_.set(i,dimension);

        i++;
    }
}

template<class T>
void SwakBasicSourceCommon<T>::writeData(Ostream& os) const {
    os  << indent << name_ << endl;
    dict_.write(os);
}


template<class T>
bool SwakBasicSourceCommon<T>::read(const dictionary& dict) {
    if (swakFvOptionType::read(dict))
    {
        setFieldData(coeffs_.subDict("expressions"));
        return true;
    }
    else
    {
        return false;
    }
}

} // end namespace

#endif

// ************************************************************************* //
