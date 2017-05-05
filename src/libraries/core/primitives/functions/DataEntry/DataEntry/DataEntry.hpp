/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    CML::DataEntry

Description
    Top level data entry class for use in dictionaries. Provides a mechanism
    to specify a variable as a certain type, e.g. constant or table, and
    provide functions to return the (interpolated) value, and integral between
    limits.

SourceFiles
    DataEntry.cpp
    DataEntryNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef DataEntry_H
#define DataEntry_H

#include "dictionary.hpp"
#include "Field.hpp"
#include "dimensionedType.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
class Time;

template<class Type>
class DataEntry;

template<class Type>
Ostream& operator<<
(
    Ostream&,
    const DataEntry<Type>&
);

/*---------------------------------------------------------------------------*\
                         Class DataEntry Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class DataEntry
:
    public refCount
{
    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const DataEntry<Type>&);


protected:

    // Protected data

        //- Name of entry
        const word name_;


public:

    //- Runtime type information
    TypeName("DataEntry")

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        DataEntry,
        dictionary,
        (
            const word& entryName,
            const dictionary& dict
        ),
        (entryName, dict)
    );


    // Constructor

        //- Construct from entry name
        DataEntry(const word& entryName);

        //- Copy constructor
        DataEntry(const DataEntry<Type>& de);

        //- Construct and return a clone
        virtual tmp<DataEntry<Type> > clone() const
        {
            return tmp<DataEntry<Type> >(new DataEntry<Type>(*this));
        }


    //- Selector
    static autoPtr<DataEntry<Type> > New
    (
        const word& entryName,
        const dictionary& dict
    );


    //- Destructor
    virtual ~DataEntry();


    // Member Functions

        // Access

            //- Return the name of the entry
            const word& name() const;


        // Manipulation

            //- Convert time
            virtual void convertTimeBase(const Time& t);


        // Evaluation

            //- Return value as a function of (scalar) independent variable
            virtual Type value(const scalar x) const;

            //- Return value as a function of (scalar) independent variable
            virtual tmp<Field<Type> > value(const scalarField& x) const;

            //- Integrate between two (scalar) values
            virtual Type integrate(const scalar x1, const scalar x2) const;

            //- Integrate between two (scalar) values
            virtual tmp<Field<Type> > integrate
            (
                const scalarField& x1,
                const scalarField& x2
            ) const;

            //- Return dimensioned type
            virtual dimensioned<Type> dimValue(const scalar x) const;

            //- Return dimensioned type as a function of (scalar)
            virtual tmp<Field<dimensioned<Type> > > dimValue
            (
                const scalarField& x
            ) const;

            //- Integrate between two scalars and return a dimensioned type
            virtual dimensioned<Type> dimIntegrate
            (
                const scalar x1,
                const scalar x2
            ) const;

            //- Integrate between two scalar fields and return a field of
            //  dimensioned type
            virtual tmp<Field<dimensioned<Type> > > dimIntegrate
            (
                const scalarField& x1,
                const scalarField& x2
            ) const;


        // I/O

            //- Ostream Operator
            friend Ostream& operator<< <Type>
            (
                Ostream& os,
                const DataEntry<Type>& de
            );

            //- Write in dictionary format
            virtual void writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeDataEntry(Type)                                                   \
                                                                              \
    defineNamedTemplateTypeNameAndDebug(DataEntry<Type>, 0);                  \
                                                                              \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
        DataEntry<Type>,                                                      \
        dictionary                                                            \
    );


#define makeDataEntryType(SS, Type)                                           \
                                                                              \
    defineNamedTemplateTypeNameAndDebug(SS<Type>, 0);                         \
                                                                              \
    DataEntry<Type>::adddictionaryConstructorToTable<SS<Type> >               \
        add##SS##Type##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "Time.hpp"

// * * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntry<Type>::DataEntry(const word& entryName)
:
    refCount(),
    name_(entryName)
{}


template<class Type>
CML::DataEntry<Type>::DataEntry(const DataEntry<Type>& de)
:
    refCount(),
    name_(de.name_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntry<Type>::~DataEntry()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
const CML::word& CML::DataEntry<Type>::name() const
{
    return name_;
}


template<class Type>
void CML::DataEntry<Type>::convertTimeBase(const Time&)
{
    // do nothing
}


template<class Type>
Type CML::DataEntry<Type>::value(const scalar x) const
{
    notImplemented("Type CML::DataEntry<Type>::value(const scalar) const");

    return pTraits<Type>::zero;
}


template<class Type>
Type CML::DataEntry<Type>::integrate(const scalar x1, const scalar x2) const
{
    notImplemented
    (
        "Type CML::DataEntry<Type>::integrate"
        "("
            "const scalar, "
            "const scalar"
        ") const"
    );

    return pTraits<Type>::zero;
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::DataEntry<Type>::value
(
    const scalarField& x
) const
{
    tmp<Field<Type> > tfld(new Field<Type>(x.size()));
    Field<Type>& fld = tfld();

    forAll(x, i)
    {
        fld[i] = this->value(x[i]);
    }
    return tfld;
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::DataEntry<Type>::integrate
(
    const scalarField& x1,
    const scalarField& x2
) const
{
    tmp<Field<Type> > tfld(new Field<Type>(x1.size()));
    Field<Type>& fld = tfld();

    forAll(x1, i)
    {
        fld[i] = this->integrate(x1[i], x2[i]);
    }
    return tfld;
}



template<class Type>
CML::dimensioned<Type> CML::DataEntry<Type>::dimValue(const scalar x) const
{
    notImplemented
    (
        "dimensioned<Type> CML::DataEntry<dimensioned<Type> >::dimValue"
        "(const scalar) const"
    );

    return dimensioned<Type>("zero", dimless, pTraits<Type>::zero);
}


template<class Type>
CML::dimensioned<Type> CML::DataEntry<Type>::dimIntegrate
(
    const scalar x1,
    const scalar x2
) const
{
    notImplemented
    (
        "dimensioned<Type> CML::DataEntry<Type>::dimIntegrate"
        "("
            "const scalar, "
            "const scalar"
        ") const"
    );

    return dimensioned<Type>("zero", dimless, pTraits<Type>::zero);
}


template<class Type>
CML::tmp<CML::Field<CML::dimensioned<Type> > >
CML::DataEntry<Type>::dimValue
(
    const scalarField& x
) const
{

    tmp<Field<dimensioned<Type> > > tfld
    (
        new Field<dimensioned<Type> >
        (
            x.size(),
            dimensioned<Type>("zero", dimless, pTraits<Type>::zero)
        )
    );

    Field<dimensioned<Type> >& fld = tfld();

    forAll(x, i)
    {
        fld[i] = this->dimValue(x[i]);
    }
    return tfld;
}


template<class Type>
CML::tmp<CML::Field<CML::dimensioned<Type> > >
CML::DataEntry<Type>::dimIntegrate
(
    const scalarField& x1,
    const scalarField& x2
) const
{
    tmp<Field<dimensioned<Type> > > tfld
    (
        new Field<dimensioned<Type> >(x1.size())
    );

    Field<dimensioned<Type> >& fld = tfld();

    forAll(x1, i)
    {
        fld[i] = this->dimIntegrate(x1[i], x2[i]);
    }
    return tfld;
}


// * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const DataEntry<Type>& de
)
{
    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const DataEntry<Type>&)"
    );

    os  << de.name_;

    return os;
}


template<class Type>
void CML::DataEntry<Type>::writeData(Ostream& os) const
{
    os.writeKeyword(name_) << type();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
CML::autoPtr<CML::DataEntry<Type> > CML::DataEntry<Type>::New
(
    const word& entryName,
    const dictionary& dict
)
{
    Istream& is(dict.lookup(entryName, false));

    token firstToken(is);

    word DataEntryType;
    if (firstToken.isWord())
    {
        // Dimensioned type default compatibility
        if (firstToken.wordToken() == entryName)
        {
            DataEntryType = "CompatibilityConstant";
        }
        else
        {
            DataEntryType = firstToken.wordToken();
        }
    }
    else
    {
        // DataEntryType = CompatibilityConstant<Type>::typeName;
        DataEntryType = "CompatibilityConstant";
    }

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(DataEntryType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn("DataEntry<Type>::New(const word&, const dictionary&)")
            << "Unknown DataEntry type "
            << DataEntryType << " for DataEntry "
            << entryName << nl << nl
            << "Valid DataEntry types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc() << nl
            << exit(FatalError);
    }

    return autoPtr<DataEntry<Type> >(cstrIter()(entryName, dict));
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
