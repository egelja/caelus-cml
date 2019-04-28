/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2015 Applied CCM
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
    CML::DimensionedField

Description
    Field with dimensions and associated with geometry type GeoMesh which is
    used to size the field and a reference to it is maintained.

\*---------------------------------------------------------------------------*/

#ifndef DimensionedField_H
#define DimensionedField_H

#include "regIOobject.hpp"
#include "Field.hpp"
#include "dimensionedType.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Type, class GeoMesh> class DimensionedField;

template<class Type, class GeoMesh> Ostream& operator<<
(
    Ostream&,
    const DimensionedField<Type, GeoMesh>&
);

template<class Type, class GeoMesh> Ostream& operator<<
(
    Ostream&,
    const tmp<DimensionedField<Type, GeoMesh> >&
);


/*---------------------------------------------------------------------------*\
                           Class DimensionedField Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class GeoMesh>
class DimensionedField
:
    public regIOobject,
    public Field<Type>
{

public:

    // Public typedefs

        typedef typename GeoMesh::Mesh Mesh;
        typedef typename Field<Type>::cmptType cmptType;


private:

    // Private data

        //- Reference to mesh
        const Mesh& mesh_;

        //- Dimension set for this field
        dimensionSet dimensions_;


    // Private Member Functions

        void readIfPresent(const word& fieldDictEntry = "value");


public:

    //- Runtime type information
    TypeName("DimensionedField");

    // Static Member Functions

        //- Return a null DimensionedField
        inline static const DimensionedField<Type, GeoMesh>& null();


    // Constructors

        //- Construct from components
        DimensionedField
        (
            const IOobject&,
            const Mesh& mesh,
            const dimensionSet&,
            const Field<Type>&
        );

        //- Construct from components
        //  Used for temporary fields which are initialised after construction
        DimensionedField
        (
            const IOobject&,
            const Mesh& mesh,
            const dimensionSet&,
            const bool checkIOFlags = true
        );

        //- Construct from components
        DimensionedField
        (
            const IOobject&,
            const Mesh& mesh,
            const dimensioned<Type>&,
            const bool checkIOFlags = true
        );

        //- Construct from Istream
        DimensionedField
        (
            const IOobject&,
            const Mesh& mesh,
            const word& fieldDictEntry="value"
        );

        //- Construct from dictionary
        DimensionedField
        (
            const IOobject&,
            const Mesh& mesh,
            const dictionary& fieldDict,
            const word& fieldDictEntry="value"
        );

        //- Construct as copy
        DimensionedField
        (
            const DimensionedField<Type, GeoMesh>&
        );

        //- Construct as copy or re-use as specified.
        DimensionedField
        (
            DimensionedField<Type, GeoMesh>&,
            bool reUse
        );

        //- Construct by transferring the DimensionedField
        DimensionedField
        (
            const Xfer<DimensionedField<Type, GeoMesh> >&
        );

        //- Construct as copy of tmp<DimensionedField> deleting argument
        #ifdef ConstructFromTmp
        DimensionedField
        (
            const tmp<DimensionedField<Type, GeoMesh> >&
        );
        #endif

        //- Construct as copy resetting IO parameters
        DimensionedField
        (
            const IOobject&,
            const DimensionedField<Type, GeoMesh>&
        );

        //- Construct as copy resetting IO parameters and re-use as specified.
        DimensionedField
        (
            const IOobject&,
            DimensionedField<Type, GeoMesh>&,
            bool reUse
        );

        //- Construct as copy resetting name
        DimensionedField
        (
            const word& newName,
            const DimensionedField<Type, GeoMesh>&
        );

        //- Construct as copy resetting name and re-use as specified.
        DimensionedField
        (
            const word& newName,
            DimensionedField<Type, GeoMesh>&,
            bool reUse
        );

        //- Construct by transferring the DimensionedField with a new name
        DimensionedField
        (
            const word& newName,
            const Xfer<DimensionedField<Type, GeoMesh> >&
        );

        //- Construct as copy resetting name
        #ifdef ConstructFromTmp
        DimensionedField
        (
            const word& newName,
            const tmp<DimensionedField<Type, GeoMesh> >&
        );
        #endif

        //- Clone
        tmp<DimensionedField<Type, GeoMesh> > clone() const;


    //- Destructor
    virtual ~DimensionedField();


    // Member Functions

        void readField
        (
            const dictionary& fieldDict,
            const word& fieldDictEntry = "value"
        );

        //- Return mesh
        inline const Mesh& mesh() const;

        //- Return dimensions
        inline const dimensionSet& dimensions() const;

        //- Return non-const access to dimensions
        inline dimensionSet& dimensions();

        inline const Field<Type>& field() const;

        inline Field<Type>& field();

        //- Return a component field of the field
        tmp<DimensionedField<cmptType, GeoMesh> > component
        (
            const direction
        ) const;

        //- Replace a component field of the field
        void replace
        (
            const direction,
            const DimensionedField<cmptType, GeoMesh>&
        );

        //- Replace a component field of the field
        void replace
        (
            const direction,
            const tmp<DimensionedField<cmptType, GeoMesh> >&
        );

        //- Return the field transpose (only defined for second rank tensors)
        tmp<DimensionedField<Type, GeoMesh> > T() const;

        //- Calculate and return arithmetic average
        dimensioned<Type> average() const;

        //- Calculate and return weighted average
        dimensioned<Type> weightedAverage
        (
            const DimensionedField<scalar, GeoMesh>&
        ) const;

        //- Calculate and return weighted average
        dimensioned<Type> weightedAverage
        (
            const tmp<DimensionedField<scalar, GeoMesh> >&
        ) const;


        // Write

            bool writeData(Ostream&, const word& fieldDictEntry) const;

            bool writeData(Ostream&) const;


    // Member Operators

        void operator=(const DimensionedField<Type, GeoMesh>&);
        void operator=(const tmp<DimensionedField<Type, GeoMesh> >&);
        void operator=(const dimensioned<Type>&);

        void operator+=(const DimensionedField<Type, GeoMesh>&);
        void operator+=(const tmp<DimensionedField<Type, GeoMesh> >&);

        void operator-=(const DimensionedField<Type, GeoMesh>&);
        void operator-=(const tmp<DimensionedField<Type, GeoMesh> >&);

        void operator*=(const DimensionedField<scalar, GeoMesh>&);
        void operator*=(const tmp<DimensionedField<scalar, GeoMesh> >&);

        void operator/=(const DimensionedField<scalar, GeoMesh>&);
        void operator/=(const tmp<DimensionedField<scalar, GeoMesh> >&);

        void operator+=(const dimensioned<Type>&);
        void operator-=(const dimensioned<Type>&);

        void operator*=(const dimensioned<scalar>&);
        void operator/=(const dimensioned<scalar>&);


    // Ostream Operators

        friend Ostream& operator<< <Type, GeoMesh>
        (
            Ostream&,
            const DimensionedField<Type, GeoMesh>&
        );

        friend Ostream& operator<< <Type, GeoMesh>
        (
            Ostream&,
            const tmp<DimensionedField<Type, GeoMesh> >&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, class GeoMesh>
inline const DimensionedField<Type, GeoMesh>&
DimensionedField<Type, GeoMesh>::null()
{
    return NullSingletonRef< DimensionedField<Type, GeoMesh> >();
}


template<class Type, class GeoMesh>
inline const typename GeoMesh::Mesh&
DimensionedField<Type, GeoMesh>::mesh() const
{
    return mesh_;
}


template<class Type, class GeoMesh>
inline const dimensionSet& DimensionedField<Type, GeoMesh>::dimensions() const
{
    return dimensions_;
}

template<class Type, class GeoMesh>
inline dimensionSet& DimensionedField<Type, GeoMesh>::dimensions()
{
    return dimensions_;
}


template<class Type, class GeoMesh>
inline const Field<Type>& DimensionedField<Type, GeoMesh>::field() const
{
    return *this;
}

template<class Type, class GeoMesh>
inline Field<Type>& DimensionedField<Type, GeoMesh>::field()
{
    return *this;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML
#include "DimensionedFieldFunctions.hpp"

#include "dimensionedType.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// check mesh for two fields
#define checkField(df1, df2, op)                                    \
if (&(df1).mesh() != &(df2).mesh())                                 \
{                                                                   \
    FatalErrorInFunction                                            \
        << "different mesh for fields "                             \
        << (df1).name() << " and " << (df2).name()                  \
        << " during operatrion " <<  op                             \
        << abort(FatalError);                                       \
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::DimensionedField
(
    const IOobject& io,
    const Mesh& mesh,
    const dimensionSet& dims,
    const Field<Type>& field
)
:
    regIOobject(io),
    Field<Type>(field),
    mesh_(mesh),
    dimensions_(dims)
{
    if (field.size() && field.size() != GeoMesh::size(mesh))
    {
        FatalErrorInFunction
            << "size of field = " << field.size()
            << " is not the same as the size of mesh = "
            << GeoMesh::size(mesh)
            << abort(FatalError);
    }
}


template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::DimensionedField
(
    const IOobject& io,
    const Mesh& mesh,
    const dimensionSet& dims,
    const bool checkIOFlags
)
:
    regIOobject(io),
    Field<Type>(GeoMesh::size(mesh)),
    mesh_(mesh),
    dimensions_(dims)
{
    if (checkIOFlags)
    {
        readIfPresent();
    }
}


template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::DimensionedField
(
    const IOobject& io,
    const Mesh& mesh,
    const dimensioned<Type>& dt,
    const bool checkIOFlags
)
:
    regIOobject(io),
    Field<Type>(GeoMesh::size(mesh), dt.value()),
    mesh_(mesh),
    dimensions_(dt.dimensions())
{
    if (checkIOFlags)
    {
        readIfPresent();
    }
}


template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::DimensionedField
(
    const DimensionedField<Type, GeoMesh>& df
)
:
    regIOobject(df),
    Field<Type>(df),
    mesh_(df.mesh_),
    dimensions_(df.dimensions_)
{}


template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::DimensionedField
(
    DimensionedField<Type, GeoMesh>& df,
    bool reUse
)
:
    regIOobject(df, reUse),
    Field<Type>(df, reUse),
    mesh_(df.mesh_),
    dimensions_(df.dimensions_)
{}


template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::DimensionedField
(
    const Xfer<DimensionedField<Type, GeoMesh> >& df
)
:
    regIOobject(df(), true),
    Field<Type>(df),
    mesh_(df->mesh_),
    dimensions_(df->dimensions_)
{}


#ifdef ConstructFromTmp
template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::DimensionedField
(
    const tmp<DimensionedField<Type, GeoMesh> >& tdf
)
:
    regIOobject(tdf(), tdf.isTmp()),
    Field<Type>
    (
        const_cast<DimensionedField<Type, GeoMesh>&>(tdf()),
        tdf.isTmp()
    ),
    mesh_(tdf().mesh_),
    dimensions_(tdf().dimensions_)
{
    tdf.clear();
}
#endif


template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::DimensionedField
(
    const IOobject& io,
    const DimensionedField<Type, GeoMesh>& df
)
:
    regIOobject(io),
    Field<Type>(df),
    mesh_(df.mesh_),
    dimensions_(df.dimensions_)
{}


template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::DimensionedField
(
    const IOobject& io,
    DimensionedField<Type, GeoMesh>& df,
    bool reUse
)
:
    regIOobject(io, df),
    Field<Type>(df, reUse),
    mesh_(df.mesh_),
    dimensions_(df.dimensions_)
{}


template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::DimensionedField
(
    const word& newName,
    const DimensionedField<Type, GeoMesh>& df
)
:
    regIOobject(newName, df, newName == df.name()),
    Field<Type>(df),
    mesh_(df.mesh_),
    dimensions_(df.dimensions_)
{}


template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::DimensionedField
(
    const word& newName,
    DimensionedField<Type, GeoMesh>& df,
    bool reUse
)
:
    regIOobject(newName, df, true),
    Field<Type>(df, reUse),
    mesh_(df.mesh_),
    dimensions_(df.dimensions_)
{}


template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::DimensionedField
(
    const word& newName,
    const Xfer<DimensionedField<Type, GeoMesh> >& df
)
:
    regIOobject(newName, df, true),
    Field<Type>(df),
    mesh_(df->mesh_),
    dimensions_(df->dimensions_)
{}


#ifdef ConstructFromTmp
template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::DimensionedField
(
    const word& newName,
    const tmp<DimensionedField<Type, GeoMesh> >& tdf
)
:
    regIOobject(newName, tdf(), true),
    Field<Type>
    (
        const_cast<DimensionedField<Type, GeoMesh>&>(tdf()),
        tdf.isTmp()
    ),
    mesh_(tdf().mesh_),
    dimensions_(tdf().dimensions_)
{
    tdf().clear();
}
#endif


template<class Type, class GeoMesh>
tmp<DimensionedField<Type, GeoMesh> >
DimensionedField<Type, GeoMesh>::clone() const
{
    return tmp<DimensionedField<Type, GeoMesh> >
    (
        new DimensionedField<Type, GeoMesh>(*this)
    );
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type, class GeoMesh>
DimensionedField<Type, GeoMesh>::~DimensionedField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, class GeoMesh>
tmp
<
    DimensionedField
        <typename DimensionedField<Type, GeoMesh>::cmptType, GeoMesh>
>
DimensionedField<Type, GeoMesh>::component
(
    const direction d
) const
{
    tmp<DimensionedField<cmptType, GeoMesh> > result
    (
        new DimensionedField<cmptType, GeoMesh>
        (
            IOobject
            (
                name() + ".component(" + ::CML::name(d) + ')',
                instance(),
                db()
            ),
            mesh_,
            dimensions_
        )
    );

    CML::component(result(), *this, d);

    return result;
}


template<class Type, class GeoMesh>
void DimensionedField<Type, GeoMesh>::replace
(
    const direction d,
    const DimensionedField
        <typename DimensionedField<Type, GeoMesh>::cmptType, GeoMesh>& df
)
{
    Field<Type>::replace(d, df);
}


template<class Type, class GeoMesh>
void DimensionedField<Type, GeoMesh>::replace
(
    const direction d,
    const tmp
    <
        DimensionedField
            <typename DimensionedField<Type, GeoMesh>::cmptType, GeoMesh>
    >& tdf
)
{
    replace(d, tdf());
    tdf.clear();
}


template<class Type, class GeoMesh>
tmp<DimensionedField<Type, GeoMesh> >
DimensionedField<Type, GeoMesh>::T() const
{
    tmp<DimensionedField<Type, GeoMesh> > result
    (
        new DimensionedField<Type, GeoMesh>
        (
            IOobject
            (
                name() + ".T()",
                instance(),
                db()
            ),
            mesh_,
            dimensions_
        )
    );

    CML::T(result(), *this);

    return result;
}


template<class Type, class GeoMesh>
dimensioned<Type> DimensionedField<Type, GeoMesh>::average() const
{
    dimensioned<Type> Average
    (
        this->name() + ".average()",
        this->dimensions(),
        gAverage(field())
    );

    return Average;
}


template<class Type, class GeoMesh>
dimensioned<Type> DimensionedField<Type, GeoMesh>::weightedAverage
(
    const DimensionedField<scalar, GeoMesh>& weightField
) const
{
    return
    (
        dimensioned<Type>
        (
            this->name() + ".weightedAverage(weights)",
            this->dimensions(),
            gSum(weightField*field())/gSum(weightField)
        )
    );
}


template<class Type, class GeoMesh>
dimensioned<Type> DimensionedField<Type, GeoMesh>::weightedAverage
(
    const tmp<DimensionedField<scalar, GeoMesh> >& tweightField
) const
{
    dimensioned<Type> wa = weightedAverage(tweightField());
    tweightField.clear();
    return wa;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type, class GeoMesh>
void DimensionedField<Type, GeoMesh>::operator=
(
    const DimensionedField<Type, GeoMesh>& df
)
{
    // Check for assignment to self
    if (this == &df)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    checkField(*this, df, "=");

    dimensions_ = df.dimensions();
    Field<Type>::operator=(df);
}


template<class Type, class GeoMesh>
void DimensionedField<Type, GeoMesh>::operator=
(
    const tmp<DimensionedField<Type, GeoMesh> >& tdf
)
{
    const DimensionedField<Type, GeoMesh>& df = tdf();

    // Check for assignment to self
    if (this == &df)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    checkField(*this, df, "=");

    dimensions_ = df.dimensions();
    this->transfer(const_cast<DimensionedField<Type, GeoMesh>&>(df));
    tdf.clear();
}


template<class Type, class GeoMesh>
void DimensionedField<Type, GeoMesh>::operator=
(
    const dimensioned<Type>& dt
)
{
    dimensions_ = dt.dimensions();
    Field<Type>::operator=(dt.value());
}


#define COMPUTED_ASSIGNMENT(TYPE, op)                                         \
                                                                              \
template<class Type, class GeoMesh>                                           \
void DimensionedField<Type, GeoMesh>::operator op                             \
(                                                                             \
    const DimensionedField<TYPE, GeoMesh>& df                                 \
)                                                                             \
{                                                                             \
    checkField(*this, df, #op);                                               \
                                                                              \
    dimensions_ op df.dimensions();                                           \
    Field<Type>::operator op(df);                                             \
}                                                                             \
                                                                              \
template<class Type, class GeoMesh>                                           \
void DimensionedField<Type, GeoMesh>::operator op                             \
(                                                                             \
    const tmp<DimensionedField<TYPE, GeoMesh> >& tdf                          \
)                                                                             \
{                                                                             \
    operator op(tdf());                                                       \
    tdf.clear();                                                              \
}                                                                             \
                                                                              \
template<class Type, class GeoMesh>                                           \
void DimensionedField<Type, GeoMesh>::operator op                             \
(                                                                             \
    const dimensioned<TYPE>& dt                                               \
)                                                                             \
{                                                                             \
    dimensions_ op dt.dimensions();                                           \
    Field<Type>::operator op(dt.value());                                     \
}

COMPUTED_ASSIGNMENT(Type, +=)
COMPUTED_ASSIGNMENT(Type, -=)
COMPUTED_ASSIGNMENT(scalar, *=)
COMPUTED_ASSIGNMENT(scalar, /=)

#undef COMPUTED_ASSIGNMENT


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#undef checkField

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IOstreams.hpp"


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type, class GeoMesh>
void CML::DimensionedField<Type, GeoMesh>::readField
(
    const dictionary& fieldDict,
    const word& fieldDictEntry
)
{
    dimensions_.reset(dimensionSet(fieldDict.lookup("dimensions")));

    Field<Type> f(fieldDictEntry, fieldDict, GeoMesh::size(mesh_));
    this->transfer(f);
}


template<class Type, class GeoMesh>
void CML::DimensionedField<Type, GeoMesh>::readIfPresent
(
    const word& fieldDictEntry
)
{
    if
    (
        (this->readOpt() == IOobject::READ_IF_PRESENT && this->headerOk())
     || this->readOpt() == IOobject::MUST_READ
     || this->readOpt() == IOobject::MUST_READ_IF_MODIFIED
    )
    {
        readField(dictionary(readStream(typeName)), fieldDictEntry);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type, class GeoMesh>
CML::DimensionedField<Type, GeoMesh>::DimensionedField
(
    const IOobject& io,
    const Mesh& mesh,
    const word& fieldDictEntry
)
:
    regIOobject(io),
    Field<Type>(0),
    mesh_(mesh),
    dimensions_(dimless)
{
    readField(dictionary(readStream(typeName)), fieldDictEntry);
}


template<class Type, class GeoMesh>
CML::DimensionedField<Type, GeoMesh>::DimensionedField
(
    const IOobject& io,
    const Mesh& mesh,
    const dictionary& fieldDict,
    const word& fieldDictEntry
)
:
    regIOobject(io),
    Field<Type>(0),
    mesh_(mesh),
    dimensions_(dimless)
{
    readField(fieldDict, fieldDictEntry);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, class GeoMesh>
bool CML::DimensionedField<Type, GeoMesh>::writeData
(
    Ostream& os,
    const word& fieldDictEntry
) const
{
    os.writeKeyword("dimensions") << dimensions() << token::END_STATEMENT
        << nl << nl;

    Field<Type>::writeEntry(fieldDictEntry, os);

    // Check state of Ostream
    os.check
    (
        "bool DimensionedField<Type, GeoMesh>::writeData"
        "(Ostream& os, const word& fieldDictEntry) const"
    );

    return (os.good());
}


template<class Type, class GeoMesh>
bool CML::DimensionedField<Type, GeoMesh>::writeData(Ostream& os) const
{
    return writeData(os, "value");
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type, class GeoMesh>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const DimensionedField<Type, GeoMesh>& df
)
{
    df.writeData(os);

    return os;
}


template<class Type, class GeoMesh>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const tmp<DimensionedField<Type, GeoMesh> >& tdf
)
{
    tdf().writeData(os);
    tdf.clear();

    return os;
}

#include "DimensionedFieldReuseFunctions.hpp"

#define TEMPLATE template<class Type, class GeoMesh>
#include "DimensionedFieldFunctionsM.hxx"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * //

template<class Type, class GeoMesh, direction r>
tmp<DimensionedField<typename powProduct<Type, r>::type, GeoMesh> >
pow
(
    const DimensionedField<Type, GeoMesh>& df,
    typename powProduct<Type, r>::type
)
{
    typedef typename powProduct<Type, r>::type powProductType;

    tmp<DimensionedField<powProductType, GeoMesh> > tPow
    (
        new DimensionedField<powProductType, GeoMesh>
        (
            IOobject
            (
                "pow(" + df.name() + ',' + name(r) + ')',
                df.instance(),
                df.db()
            ),
            df.mesh(),
            pow(df.dimensions(), r)
        )
    );

    pow<Type, r, GeoMesh>(tPow().field(), df.field());

    return tPow;
}


template<class Type, class GeoMesh, direction r>
tmp<DimensionedField<typename powProduct<Type, r>::type, GeoMesh> >
pow
(
    const tmp<DimensionedField<Type, GeoMesh> >& tdf,
    typename powProduct<Type, r>::type
)
{
    typedef typename powProduct<Type, r>::type powProductType;

    const DimensionedField<Type, GeoMesh>& df = tdf();

    tmp<DimensionedField<powProductType, GeoMesh> > tPow =
        reuseTmpDimensionedField<powProductType, Type, GeoMesh>::New
        (
            tdf,
            "pow(" + df.name() + ',' + name(r) + ')',
            pow(df.dimensions(), r)
        );

    pow<Type, r, GeoMesh>(tPow().field(), df.field());

    reuseTmpDimensionedField<powProductType, Type, GeoMesh>::clear(tdf);

    return tPow;
}

template<class Type, class GeoMesh>
tmp<DimensionedField<typename outerProduct<Type, Type>::type, GeoMesh> >
sqr(const DimensionedField<Type, GeoMesh>& df)
{
    typedef typename outerProduct<Type, Type>::type outerProductType;

    tmp<DimensionedField<outerProductType, GeoMesh> > tSqr
    (
        new DimensionedField<outerProductType, GeoMesh>
        (
            IOobject
            (
                "sqr(" + df.name() + ')',
                df.instance(),
                df.db()
            ),
            df.mesh(),
            sqr(df.dimensions())
        )
    );

    sqr(tSqr().field(), df.field());

    return tSqr;
}

template<class Type, class GeoMesh>
tmp<DimensionedField<typename outerProduct<Type, Type>::type, GeoMesh> >
sqr(const tmp<DimensionedField<Type, GeoMesh> >& tdf)
{
    typedef typename outerProduct<Type, Type>::type outerProductType;

    const DimensionedField<Type, GeoMesh>& df = tdf();

    tmp<DimensionedField<outerProductType, GeoMesh> > tSqr =
        reuseTmpDimensionedField<outerProductType, Type, GeoMesh>::New
        (
            tdf,
            "sqr(" + df.name() + ')',
            sqr(df.dimensions())
        );

    sqr(tSqr().field(), df.field());

    reuseTmpDimensionedField<outerProductType, Type, GeoMesh>::clear(tdf);

    return tSqr;
}


template<class Type, class GeoMesh>
tmp<DimensionedField<scalar, GeoMesh> > magSqr
(
    const DimensionedField<Type, GeoMesh>& df
)
{
    tmp<DimensionedField<scalar, GeoMesh> > tMagSqr
    (
        new DimensionedField<scalar, GeoMesh>
        (
            IOobject
            (
                "magSqr(" + df.name() + ')',
                df.instance(),
                df.db()
            ),
            df.mesh(),
            sqr(df.dimensions())
        )
    );

    magSqr(tMagSqr().field(), df.field());

    return tMagSqr;
}

template<class Type, class GeoMesh>
tmp<DimensionedField<scalar, GeoMesh> > magSqr
(
    const tmp<DimensionedField<Type, GeoMesh> >& tdf
)
{
    const DimensionedField<Type, GeoMesh>& df = tdf();

    tmp<DimensionedField<scalar, GeoMesh> > tMagSqr =
        reuseTmpDimensionedField<scalar, Type, GeoMesh>::New
        (
            tdf,
            "magSqr(" + df.name() + ')',
            sqr(df.dimensions())
        );

    magSqr(tMagSqr().field(), df.field());

    reuseTmpDimensionedField<scalar, Type, GeoMesh>::clear(tdf);

    return tMagSqr;
}


template<class Type, class GeoMesh>
tmp<DimensionedField<scalar, GeoMesh> > mag
(
    const DimensionedField<Type, GeoMesh>& df
)
{
    tmp<DimensionedField<scalar, GeoMesh> > tMag
    (
        new DimensionedField<scalar, GeoMesh>
        (
            IOobject
            (
                "mag(" + df.name() + ')',
                df.instance(),
                df.db()
            ),
            df.mesh(),
            df.dimensions()
        )
    );

    mag(tMag().field(), df.field());

    return tMag;
}

template<class Type, class GeoMesh>
tmp<DimensionedField<scalar, GeoMesh> > mag
(
    const tmp<DimensionedField<Type, GeoMesh> >& tdf
)
{
    const DimensionedField<Type, GeoMesh>& df = tdf();

    tmp<DimensionedField<scalar, GeoMesh> > tMag =
        reuseTmpDimensionedField<scalar, Type, GeoMesh>::New
        (
            tdf,
            "mag(" + df.name() + ')',
            df.dimensions()
        );

    mag(tMag().field(), df.field());

    reuseTmpDimensionedField<scalar, Type, GeoMesh>::clear(tdf);

    return tMag;
}


template<class Type, class GeoMesh>
tmp
<
    DimensionedField
        <typename DimensionedField<Type, GeoMesh>::cmptType, GeoMesh>
>
cmptAv(const DimensionedField<Type, GeoMesh>& df)
{
    typedef typename DimensionedField<Type, GeoMesh>::cmptType cmptType;

    tmp<DimensionedField<cmptType, GeoMesh> > CmptAv
    (
        new DimensionedField<scalar, GeoMesh>
        (
            IOobject
            (
                "cmptAv(" + df.name() + ')',
                df.instance(),
                df.db()
            ),
            df.mesh(),
            df.dimensions()
        )
    );

    cmptAv(CmptAv().field(), df.field());

    return CmptAv;
}

template<class Type, class GeoMesh>
tmp
<
    DimensionedField
        <typename DimensionedField<Type, GeoMesh>::cmptType, GeoMesh>
>
cmptAv(const tmp<DimensionedField<Type, GeoMesh> >& tdf)
{
    typedef typename DimensionedField<Type, GeoMesh>::cmptType
        cmptType;

    const DimensionedField<Type, GeoMesh>& df = tdf();

    tmp<DimensionedField<cmptType, GeoMesh> > CmptAv =
        reuseTmpDimensionedField<cmptType, Type, GeoMesh>::New
        (
            tdf,
            "cmptAv(" + df.name() + ')',
            df.dimensions()
        );

    cmptAv(CmptAv().field(), df.field());

    reuseTmpDimensionedField<cmptType, Type, GeoMesh>::clear(tdf);

    return CmptAv;
}

#define UNARY_REDUCTION_FUNCTION(returnType, func, dfunc)                     \
                                                                              \
template<class Type, class GeoMesh>                                           \
dimensioned<returnType> func                                                  \
(                                                                             \
    const DimensionedField<Type, GeoMesh>& df                                 \
)                                                                             \
{                                                                             \
    return dimensioned<Type>                                                  \
    (                                                                         \
        #func "(" + df.name() + ')',                                          \
        df.dimensions(),                                                      \
        dfunc(df.field())                                                     \
    );                                                                        \
}                                                                             \
                                                                              \
template<class Type, class GeoMesh>                                           \
dimensioned<returnType> func                                                  \
(                                                                             \
    const tmp<DimensionedField<Type, GeoMesh> >& tdf1                         \
)                                                                             \
{                                                                             \
    dimensioned<returnType> res = func(tdf1());                               \
    tdf1.clear();                                                             \
    return res;                                                               \
}

UNARY_REDUCTION_FUNCTION(Type, max, gMax)
UNARY_REDUCTION_FUNCTION(Type, min, gMin)
UNARY_REDUCTION_FUNCTION(Type, sum, gSum)
UNARY_REDUCTION_FUNCTION(scalar, sumMag, gSumMag)
UNARY_REDUCTION_FUNCTION(Type, average, gAverage)

#undef UNARY_REDUCTION_FUNCTION


BINARY_FUNCTION(Type, Type, Type, max)
BINARY_FUNCTION(Type, Type, Type, min)
BINARY_FUNCTION(Type, Type, Type, cmptMultiply)
BINARY_FUNCTION(Type, Type, Type, cmptDivide)

BINARY_TYPE_FUNCTION(Type, Type, Type, max)
BINARY_TYPE_FUNCTION(Type, Type, Type, min)
BINARY_TYPE_FUNCTION(Type, Type, Type, cmptMultiply)
BINARY_TYPE_FUNCTION(Type, Type, Type, cmptDivide)


// * * * * * * * * * * * * * * * Global operators  * * * * * * * * * * * * * //

UNARY_OPERATOR(Type, Type, -, negate, transform)

BINARY_OPERATOR(Type, Type, scalar, *, '*', multiply)
BINARY_OPERATOR(Type, scalar, Type, *, '*', multiply)
BINARY_OPERATOR(Type, Type, scalar, /, '|', divide)

BINARY_TYPE_OPERATOR_SF(Type, scalar, Type, *, '*', multiply)
BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, *, '*', multiply)

BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, /, '|', divide)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define PRODUCT_OPERATOR(product, op, opFunc)                                 \
                                                                              \
template<class Type1, class Type2, class GeoMesh>                             \
tmp<DimensionedField<typename product<Type1, Type2>::type, GeoMesh> >         \
operator op                                                                   \
(                                                                             \
    const DimensionedField<Type1, GeoMesh>& df1,                              \
    const DimensionedField<Type2, GeoMesh>& df2                               \
)                                                                             \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
    tmp<DimensionedField<productType, GeoMesh> > tRes                         \
    (                                                                         \
        new DimensionedField<productType, GeoMesh>                            \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + df1.name() + #op + df2.name() + ')',                    \
                df1.instance(),                                               \
                df1.db()                                                      \
            ),                                                                \
            df1.mesh(),                                                       \
            df1.dimensions() op df2.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    CML::opFunc(tRes().field(), df1.field(), df2.field());                    \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type1, class Type2, class GeoMesh>                             \
tmp<DimensionedField<typename product<Type1, Type2>::type, GeoMesh> >         \
operator op                                                                   \
(                                                                             \
    const DimensionedField<Type1, GeoMesh>& df1,                              \
    const tmp<DimensionedField<Type2, GeoMesh> >& tdf2                        \
)                                                                             \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
                                                                              \
    const DimensionedField<Type2, GeoMesh>& df2 = tdf2();                     \
                                                                              \
    tmp<DimensionedField<productType, GeoMesh> > tRes =                       \
        reuseTmpDimensionedField<productType, Type2, GeoMesh>::New            \
        (                                                                     \
            tdf2,                                                             \
            '(' + df1.name() + #op + df2.name() + ')',                        \
            df1.dimensions() op df2.dimensions()                              \
        );                                                                    \
                                                                              \
    CML::opFunc(tRes().field(), df1.field(), df2.field());                    \
                                                                              \
    reuseTmpDimensionedField<productType, Type2, GeoMesh>::clear(tdf2);       \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type1, class Type2, class GeoMesh>                             \
tmp<DimensionedField<typename product<Type1, Type2>::type, GeoMesh> >         \
operator op                                                                   \
(                                                                             \
    const tmp<DimensionedField<Type1, GeoMesh> >& tdf1,                       \
    const DimensionedField<Type2, GeoMesh>& df2                               \
)                                                                             \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1 = tdf1();                     \
                                                                              \
    tmp<DimensionedField<productType, GeoMesh> > tRes =                       \
        reuseTmpDimensionedField<productType, Type1, GeoMesh>::New            \
        (                                                                     \
            tdf1,                                                             \
            '(' + df1.name() + #op + df2.name() + ')',                        \
            df1.dimensions() op df2.dimensions()                              \
        );                                                                    \
                                                                              \
    CML::opFunc(tRes().field(), df1.field(), df2.field());                    \
                                                                              \
    reuseTmpDimensionedField<productType, Type1, GeoMesh>::clear(tdf1);       \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Type1, class Type2, class GeoMesh>                             \
tmp<DimensionedField<typename product<Type1, Type2>::type, GeoMesh> >         \
operator op                                                                   \
(                                                                             \
    const tmp<DimensionedField<Type1, GeoMesh> >& tdf1,                       \
    const tmp<DimensionedField<Type2, GeoMesh> >& tdf2                        \
)                                                                             \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1 = tdf1();                     \
    const DimensionedField<Type2, GeoMesh>& df2 = tdf2();                     \
                                                                              \
    tmp<DimensionedField<productType, GeoMesh> > tRes =                       \
        reuseTmpTmpDimensionedField                                           \
        <productType, Type1, Type1, Type2, GeoMesh>::New                      \
        (                                                                     \
            tdf1,                                                             \
            tdf2,                                                             \
            '(' + df1.name() + #op + df2.name() + ')',                        \
            df1.dimensions() op df2.dimensions()                              \
        );                                                                    \
                                                                              \
    CML::opFunc(tRes().field(), df1.field(), df2.field());                    \
                                                                              \
    reuseTmpTmpDimensionedField                                               \
        <productType, Type1, Type1, Type2, GeoMesh>::clear(tdf1, tdf2);       \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Form, class Type, class GeoMesh>                               \
tmp<DimensionedField<typename product<Type, Form>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
    const DimensionedField<Type, GeoMesh>& df1,                               \
    const dimensioned<Form>& dvs                                              \
)                                                                             \
{                                                                             \
    typedef typename product<Type, Form>::type productType;                   \
                                                                              \
    tmp<DimensionedField<productType, GeoMesh> > tRes                         \
    (                                                                         \
        new DimensionedField<productType, GeoMesh>                            \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + df1.name() + #op + dvs.name() + ')',                    \
                df1.instance(),                                               \
                df1.db()                                                      \
            ),                                                                \
            df1.mesh(),                                                       \
            df1.dimensions() op dvs.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    CML::opFunc(tRes().field(), df1.field(), dvs.value());                    \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Form, class Cmpt, direction nCmpt, class Type, class GeoMesh>  \
tmp<DimensionedField<typename product<Form, Type>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
    const DimensionedField<Type, GeoMesh>& df1,                               \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                    \
)                                                                             \
{                                                                             \
    return df1 op dimensioned<Form>(static_cast<const Form&>(vs));            \
}                                                                             \
                                                                              \
                                                                              \
template<class Form, class Type, class GeoMesh>                               \
tmp<DimensionedField<typename product<Type, Form>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
    const tmp<DimensionedField<Type, GeoMesh> >& tdf1,                        \
    const dimensioned<Form>& dvs                                              \
)                                                                             \
{                                                                             \
    typedef typename product<Type, Form>::type productType;                   \
                                                                              \
    const DimensionedField<Type, GeoMesh>& df1 = tdf1();                      \
                                                                              \
    tmp<DimensionedField<productType, GeoMesh> > tRes =                       \
        reuseTmpDimensionedField<productType, Type, GeoMesh>::New             \
        (                                                                     \
            tdf1,                                                             \
            '(' + df1.name() + #op + dvs.name() + ')',                        \
            df1.dimensions() op dvs.dimensions()                              \
        );                                                                    \
                                                                              \
    CML::opFunc(tRes().field(), df1.field(), dvs.value());                    \
                                                                              \
    reuseTmpDimensionedField<productType, Type, GeoMesh>::clear(tdf1);        \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Form, class Cmpt, direction nCmpt, class Type, class GeoMesh>  \
tmp<DimensionedField<typename product<Form, Type>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
    const tmp<DimensionedField<Type, GeoMesh> >& tdf1,                        \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                    \
)                                                                             \
{                                                                             \
    return tdf1 op dimensioned<Form>(static_cast<const Form&>(vs));           \
}                                                                             \
                                                                              \
                                                                              \
template<class Form, class Type, class GeoMesh>                               \
tmp<DimensionedField<typename product<Form, Type>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
    const dimensioned<Form>& dvs,                                             \
    const DimensionedField<Type, GeoMesh>& df1                                \
)                                                                             \
{                                                                             \
    typedef typename product<Form, Type>::type productType;                   \
    tmp<DimensionedField<productType, GeoMesh> > tRes                         \
    (                                                                         \
        new DimensionedField<productType, GeoMesh>                            \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + dvs.name() + #op + df1.name() + ')',                    \
                df1.instance(),                                               \
                df1.db()                                                      \
            ),                                                                \
            df1.mesh(),                                                       \
            dvs.dimensions() op df1.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    CML::opFunc(tRes().field(), dvs.value(), df1.field());                    \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Form, class Cmpt, direction nCmpt, class Type, class GeoMesh>  \
tmp<DimensionedField<typename product<Form, Type>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
    const VectorSpace<Form,Cmpt,nCmpt>& vs,                                   \
    const DimensionedField<Type, GeoMesh>& df1                                \
)                                                                             \
{                                                                             \
    return dimensioned<Form>(static_cast<const Form&>(vs)) op df1;            \
}                                                                             \
                                                                              \
template<class Form, class Type, class GeoMesh>                               \
tmp<DimensionedField<typename product<Form, Type>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
    const dimensioned<Form>& dvs,                                             \
    const tmp<DimensionedField<Type, GeoMesh> >& tdf1                         \
)                                                                             \
{                                                                             \
    typedef typename product<Form, Type>::type productType;                   \
                                                                              \
    const DimensionedField<Type, GeoMesh>& df1 = tdf1();                      \
                                                                              \
    tmp<DimensionedField<productType, GeoMesh> > tRes =                       \
        reuseTmpDimensionedField<productType, Type, GeoMesh>::New             \
        (                                                                     \
            tdf1,                                                             \
            '(' + dvs.name() + #op + df1.name() + ')',                        \
            dvs.dimensions() op df1.dimensions()                              \
        );                                                                    \
                                                                              \
    CML::opFunc(tRes().field(), dvs.value(), df1.field());                    \
                                                                              \
    reuseTmpDimensionedField<productType, Type, GeoMesh>::clear(tdf1);        \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template<class Form, class Cmpt, direction nCmpt, class Type, class GeoMesh>  \
tmp<DimensionedField<typename product<Form, Type>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
    const VectorSpace<Form,Cmpt,nCmpt>& vs,                                   \
    const tmp<DimensionedField<Type, GeoMesh> >& tdf1                         \
)                                                                             \
{                                                                             \
    return dimensioned<Form>(static_cast<const Form&>(vs)) op tdf1;           \
}

PRODUCT_OPERATOR(typeOfSum, +, add)
PRODUCT_OPERATOR(typeOfSum, -, subtract)

PRODUCT_OPERATOR(outerProduct, *, outer)
PRODUCT_OPERATOR(crossProduct, ^, cross)
PRODUCT_OPERATOR(innerProduct, &, dot)
PRODUCT_OPERATOR(scalarProduct, &&, dotdot)

#undef PRODUCT_OPERATOR


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "undefFieldFunctionsM.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
