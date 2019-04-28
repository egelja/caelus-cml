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
    CML::GeometricField

Description
    Generic GeometricField class.

SourceFiles
    GeometricField.cpp
    GeometricBoundaryField.cpp
    GeometricFieldFunctions.hpp

\*---------------------------------------------------------------------------*/

#ifndef GeometricField_H
#define GeometricField_H

#include "regIOobject.hpp"
#include "dimensionedTypes.hpp"
#include "DimensionedField.hpp"
#include "FieldField.hpp"
#include "lduInterfaceFieldPtrsList.hpp"
#include "BlockLduInterfaceFieldPtrsList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class dictionary;

// Forward declaration of friend functions and operators

template<class Type, template<class> class PatchField, class GeoMesh>
class GeometricField;

template<class Type, template<class> class PatchField, class GeoMesh>
Ostream& operator<<
(
    Ostream&,
    const GeometricField<Type, PatchField, GeoMesh>&
);

template<class Type, template<class> class PatchField, class GeoMesh>
Ostream& operator<<
(
    Ostream&,
    const tmp<GeometricField<Type, PatchField, GeoMesh> >&
);


/*---------------------------------------------------------------------------*\
                           Class GeometricField Declaration
\*---------------------------------------------------------------------------*/

template<class Type, template<class> class PatchField, class GeoMesh>
class GeometricField
:
    public DimensionedField<Type, GeoMesh>
{
    // Private Member Functions

        //- Read from file if it is present
        bool readIfPresent();

        //- Read old time field from file if it is present
        bool readOldTimeIfPresent();


public:

    // Public typedefs

        typedef typename GeoMesh::Mesh Mesh;
        typedef typename GeoMesh::BoundaryMesh BoundaryMesh;

        typedef DimensionedField<Type, GeoMesh> DimensionedInternalField;
        typedef Field<Type> InternalField;
        typedef PatchField<Type> PatchFieldType;


    class GeometricBoundaryField
    :
        public FieldField<PatchField, Type>
    {
        // Private data

            //- Reference to BoundaryMesh for which this field is defined
            const BoundaryMesh& bmesh_;


    public:

        // Constructors

            //- Construct from a BoundaryMesh,
            //  reference to the internal field
            //  and a patch type
            GeometricBoundaryField
            (
                const BoundaryMesh&,
                const DimensionedInternalField&,
                const word&
            );

            //- Construct from a BoundaryMesh,
            //  reference to the internal field
            //  and a wordList of patch types and optional the actual patch
            //  types (to override constraint patches)
            GeometricBoundaryField
            (
                const BoundaryMesh&,
                const DimensionedInternalField&,
                const wordList& wantedPatchTypes,
                const wordList& actualPatchTypes = wordList()
            );

            //- Construct from a BoundaryMesh,
            //  reference to the internal field
            //  and a PtrList<PatchField<Type> >
            GeometricBoundaryField
            (
                const BoundaryMesh&,
                const DimensionedInternalField&,
                const PtrList<PatchField<Type> >&
            );

            //- Construct as copy setting the reference to the internal field
            GeometricBoundaryField
            (
                const DimensionedInternalField&,
                const GeometricBoundaryField&
            );

            //- Construct as copy
            //  Dangerous because Field may be set to a field which gets deleted
            //  Need new type of BoundaryField, one which is part of a geometric
            //  field for which snGrad etc. may be called and a free standing
            //  BoundaryField for which such operations are unavailable.
            GeometricBoundaryField
            (
                const GeometricBoundaryField&
            );

            //- Construct from dictionary
            GeometricBoundaryField
            (
                const BoundaryMesh&,
                const DimensionedInternalField&,
                const dictionary&
            );


        // Member functions

            //- Update the boundary condition coefficients
            void updateCoeffs();

            //- Evaluate boundary conditions
            void evaluate();

            //- Return a list of the patch types
            wordList types() const;

            //- Return BoundaryField of the cell values neighbouring
            //  the boundary
            GeometricBoundaryField boundaryInternalField() const;

            //- Return a list of pointers for each patch field with only those
            //  pointing to interfaces being set
            lduInterfaceFieldPtrsList interfaces() const;

            //- Return a list of pointers for each patch field with only those
            //  pointing to block-coupled interfaces being set
            typename BlockLduInterfaceFieldPtrsList<Type>::Type
            blockInterfaces() const;

            //- Write boundary field as dictionary entry
            void writeEntry(const word& keyword, Ostream& os) const;


        // Member operators

            //- Assignment to BoundaryField<Type, PatchField, BoundaryMesh>
            void operator=(const GeometricBoundaryField&);

            //- Assignment to FieldField<PatchField, Type>
            void operator=(const FieldField<PatchField, Type>&);

            //- Assignment to Type
            void operator=(const Type&);


            //- Forced assignment to
            //  BoundaryField<Type, PatchField, BoundaryMesh>
            void operator==(const GeometricBoundaryField&);

            //- Forced assignment to FieldField<PatchField, Type>
            void operator==(const FieldField<PatchField, Type>&);

            //- Forced assignment to Type
            void operator==(const Type&);
    };


private:

    // Private data

        //- Current time index.
        //  Used to trigger the storing of the old-time value
        mutable label timeIndex_;

        //- Pointer to old time field
        mutable GeometricField<Type, PatchField, GeoMesh>* field0Ptr_;

        //-  Pointer to previous iteration (used for under-relaxation)
        mutable GeometricField<Type, PatchField, GeoMesh>* fieldPrevIterPtr_;

        //- Boundary Type field containing boundary field values
        GeometricBoundaryField boundaryField_;


    // Private Member Functions

        //- Read the field from the dictionary
        tmp<GeometricBoundaryField> readField(const dictionary&);

        //- Read the field from the given stream
        tmp<GeometricBoundaryField> readField(Istream& is);


public:

    //- Runtime type information
    TypeName("GeometricField");


    // Public typedefs

        typedef typename Field<Type>::cmptType cmptType;

    // Static Member Functions

        //- Return a null geometric field
        inline static const GeometricField<Type, PatchField, GeoMesh>& null();


    // Constructors

        //- Constructor given IOobject, mesh, dimensions and patch type.
        //  This allocates storage for the field but not values.
        //  Used only within this class to create TEMPORARY variables
        GeometricField
        (
            const IOobject&,
            const Mesh&,
            const dimensionSet&,
            const word& patchFieldType=PatchField<Type>::calculatedType()
        );

        //- Constructor given IOobject, mesh, dimensions and patch types.
        //  This allocates storage for the field but not values.
        //  Used only within this class to create TEMPORARY variables
        GeometricField
        (
            const IOobject&,
            const Mesh&,
            const dimensionSet&,
            const wordList& wantedPatchTypes,
            const wordList& actualPatchTypes = wordList()
        );

        //- Constructor given IOobject, mesh, dimensioned<Type> and patch type.
        GeometricField
        (
            const IOobject&,
            const Mesh&,
            const dimensioned<Type>&,
            const word& patchFieldType=PatchField<Type>::calculatedType()
        );

        //- Constructor given IOobject, mesh, dimensioned<Type> and patch types.
        GeometricField
        (
            const IOobject&,
            const Mesh&,
            const dimensioned<Type>&,
            const wordList& wantedPatchTypes,
            const wordList& actualPatchTypes = wordList()
        );

        //- Constructor from components
        GeometricField
        (
            const IOobject&,
            const Mesh&,
            const dimensionSet&,
            const Field<Type>&,
            const PtrList<PatchField<Type> >&
        );

        //- Construct and read given IOobject
        GeometricField
        (
            const IOobject&,
            const Mesh&,
            const bool readOldTime = true
        );

        //- Construct from dictionary
        GeometricField
        (
            const IOobject&,
            const Mesh&,
            const dictionary&
        );

        //- Construct as copy
        GeometricField
        (
            const GeometricField<Type, PatchField, GeoMesh>&
        );

        //- Construct as copy of tmp<GeometricField> deleting argument
        #ifdef ConstructFromTmp
        GeometricField
        (
            const tmp<GeometricField<Type, PatchField, GeoMesh> >&
        );
        #endif

        //- Construct as copy resetting IO parameters
        GeometricField
        (
            const IOobject&,
            const GeometricField<Type, PatchField, GeoMesh>&
        );

        //- Construct as copy of tmp<GeometricField> resetting IO parameters
        #ifdef ConstructFromTmp
        GeometricField
        (
            const IOobject&,
            const tmp<GeometricField<Type, PatchField, GeoMesh> >&
        );
        #endif

        //- Construct as copy resetting name
        GeometricField
        (
            const word& newName,
            const GeometricField<Type, PatchField, GeoMesh>&
        );

        //- Construct as copy resetting name
        #ifdef ConstructFromTmp
        GeometricField
        (
            const word& newName,
            const tmp<GeometricField<Type, PatchField, GeoMesh> >&
        );
        #endif

        //- Construct as copy resetting IO parameters and patch type
        GeometricField
        (
            const IOobject&,
            const GeometricField<Type, PatchField, GeoMesh>&,
            const word& patchFieldType
        );

        //- Construct as copy resetting IO parameters and boundary types
        GeometricField
        (
            const IOobject&,
            const GeometricField<Type, PatchField, GeoMesh>&,
            const wordList& patchFieldTypes,
            const wordList& actualPatchTypes = wordList()
        );


    //- Destructor
    virtual ~GeometricField();


    // Member Functions

        //- Return dimensioned internal field
        DimensionedInternalField& dimensionedInternalField();

        //- Return dimensioned internal field
        inline const DimensionedInternalField& dimensionedInternalField() const;

        //- Return internal field
        InternalField& internalField();

        //- Return internal field
        inline const InternalField& internalField() const;

        //- Return reference to GeometricBoundaryField
        GeometricBoundaryField& boundaryField();

        //- Return reference to GeometricBoundaryField for const field
        inline const GeometricBoundaryField& boundaryField() const;

        //- Return the time index of the field
        inline label timeIndex() const;

        //- Return the time index of the field
        inline label& timeIndex();

        //- Store the old-time fields
        void storeOldTimes() const;

        //- Store the old-time field
        void storeOldTime() const;

        //- Return the number of old time fields stored
        label nOldTimes() const;

        //- Return old time field
        const GeometricField<Type, PatchField, GeoMesh>& oldTime() const;

        //- Return non-const old time field
        //  (Not a good idea but it is used for sub-cycling)
        GeometricField<Type, PatchField, GeoMesh>& oldTime();

        //- Store the field as the previous iteration value
        void storePrevIter() const;

        //- Return previous iteration field
        const GeometricField<Type, PatchField, GeoMesh>& prevIter() const;

        //- Correct boundary field
        void correctBoundaryConditions();

        //- Does the field need a reference level for solution
        bool needReference() const;

        //- Return a component of the field
        tmp<GeometricField<cmptType, PatchField, GeoMesh> > component
        (
            const direction
        ) const;

        //- WriteData member function required by regIOobject
        bool writeData(Ostream&) const;

        //- Return transpose (only if it is a tensor field)
        tmp<GeometricField<Type, PatchField, GeoMesh> > T() const;

        //- Relax field (for steady-state solution).
        //  alpha = 1 : no relaxation
        //  alpha < 1 : relaxation
        //  alpha = 0 : do nothing
        void relax(const scalar alpha);

        //- Relax field (for steady-state solution).
        //  alpha is read from controlDict
        void relax();

        //- Select the final iteration parameters if `final' is true
        //  by returning the field name + "Final"
        //  otherwise the standard parameters by returning the field name
        word select(bool final) const;


    // Member function *this operators

        void negate();

        void replace
        (
            const direction,
            const GeometricField<cmptType, PatchField, GeoMesh>&
        );

        void replace
        (
            const direction,
            const dimensioned<cmptType>&
        );

        void max(const dimensioned<Type>&);
        void min(const dimensioned<Type>&);

        void max
        (
            const GeometricField<Type, PatchField, GeoMesh>&,
            const dimensioned<Type>&
        );

        void min
        (
            const GeometricField<Type, PatchField, GeoMesh>&,
            const dimensioned<Type>&
        );

        void scale
        (
            const GeometricField<Type, PatchField, GeoMesh>&,
            const GeometricField<Type, PatchField, GeoMesh>&
        );

        void scale
        (
            const GeometricField<Type, PatchField, GeoMesh>&,
            const dimensioned<Type>&
        );


    // Member operators

        void operator=(const GeometricField<Type, PatchField, GeoMesh>&);
        void operator=(const tmp<GeometricField<Type, PatchField, GeoMesh> >&);
        void operator=(const dimensioned<Type>&);

        void operator==(const tmp<GeometricField<Type, PatchField, GeoMesh> >&);
        void operator==(const dimensioned<Type>&);

        void operator+=(const GeometricField<Type, PatchField, GeoMesh>&);
        void operator+=(const tmp<GeometricField<Type, PatchField, GeoMesh> >&);

        void operator-=(const GeometricField<Type, PatchField, GeoMesh>&);
        void operator-=(const tmp<GeometricField<Type, PatchField, GeoMesh> >&);

        void operator*=(const GeometricField<scalar, PatchField, GeoMesh>&);
        void operator*=(const tmp<GeometricField<scalar,PatchField,GeoMesh> >&);

        void operator/=(const GeometricField<scalar, PatchField, GeoMesh>&);
        void operator/=(const tmp<GeometricField<scalar,PatchField,GeoMesh> >&);

        void operator+=(const dimensioned<Type>&);
        void operator-=(const dimensioned<Type>&);

        void operator*=(const dimensioned<scalar>&);
        void operator/=(const dimensioned<scalar>&);


    // Ostream operators

        friend Ostream& operator<< <Type, PatchField, GeoMesh>
        (
            Ostream&,
            const GeometricField<Type, PatchField, GeoMesh>&
        );

        friend Ostream& operator<< <Type, PatchField, GeoMesh>
        (
            Ostream&,
            const tmp<GeometricField<Type, PatchField, GeoMesh> >&
        );
};


template<class Type, template<class> class PatchField, class GeoMesh>
Ostream& operator<<
(
    Ostream&,
    const typename GeometricField<Type, PatchField, GeoMesh>::
    GeometricBoundaryField&
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
inline const CML::GeometricField<Type, PatchField, GeoMesh>&
CML::GeometricField<Type, PatchField, GeoMesh>::null()
{
    return NullSingletonRef< GeometricField<Type, PatchField, GeoMesh> >();
}


template<class Type, template<class> class PatchField, class GeoMesh>
inline
const typename
CML::GeometricField<Type, PatchField, GeoMesh>::DimensionedInternalField&
CML::GeometricField<Type, PatchField, GeoMesh>::
dimensionedInternalField() const
{
    return *this;
}


template<class Type, template<class> class PatchField, class GeoMesh>
inline
const typename
CML::GeometricField<Type, PatchField, GeoMesh>::InternalField&
CML::GeometricField<Type, PatchField, GeoMesh>::internalField() const
{
    return *this;
}


template<class Type, template<class> class PatchField, class GeoMesh>
inline const typename CML::GeometricField<Type, PatchField, GeoMesh>::
GeometricBoundaryField&
CML::GeometricField<Type, PatchField, GeoMesh>::boundaryField() const
{
    return boundaryField_;
}


template<class Type, template<class> class PatchField, class GeoMesh>
inline CML::label
CML::GeometricField<Type, PatchField, GeoMesh>::timeIndex() const
{
    return timeIndex_;
}


template<class Type, template<class> class PatchField, class GeoMesh>
inline CML::label&
CML::GeometricField<Type, PatchField, GeoMesh>::timeIndex()
{
    return timeIndex_;
}


#include "Time.hpp"
#include "demandDrivenData.hpp"
#include "dictionary.hpp"
#include "data.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// check mesh for two fields

#define checkField(gf1, gf2, op)                                    \
if ((gf1).mesh() != (gf2).mesh())                                   \
{                                                                   \
    FatalErrorInFunction                                            \
        << "different mesh for fields "                             \
        << (gf1).name() << " and " << (gf2).name()                  \
        << " during operatrion " <<  op                             \
        << abort(FatalError);                                       \
}


// * * * * * * * * * * * * * Private Member Functions * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
CML::tmp
<
    typename CML::GeometricField<Type, PatchField, GeoMesh>::
    GeometricBoundaryField
>
CML::GeometricField<Type, PatchField, GeoMesh>::readField
(
    const dictionary& fieldDict
)
{
    DimensionedField<Type, GeoMesh>::readField(fieldDict, "internalField");

    tmp<GeometricBoundaryField> tboundaryField
    (
        new GeometricBoundaryField
        (
            this->mesh().boundary(),
            *this,
            fieldDict.subDict("boundaryField")
        )
    );

    if (fieldDict.found("referenceLevel"))
    {
        Type fieldAverage(pTraits<Type>(fieldDict.lookup("referenceLevel")));

        Field<Type>::operator+=(fieldAverage);

        GeometricBoundaryField& boundaryField = tboundaryField();

        forAll(boundaryField, patchi)
        {
            boundaryField[patchi] == boundaryField[patchi] + fieldAverage;
        }
    }

    return tboundaryField;
}


template<class Type, template<class> class PatchField, class GeoMesh>
CML::tmp
<
    typename CML::GeometricField<Type, PatchField, GeoMesh>::
    GeometricBoundaryField
>
CML::GeometricField<Type, PatchField, GeoMesh>::readField(Istream& is)
{
    return readField
    (
        IOdictionary
        (
            IOobject
            (
                this->name(),
                this->time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            is
        )
    );
}


template<class Type, template<class> class PatchField, class GeoMesh>
bool CML::GeometricField<Type, PatchField, GeoMesh>::readIfPresent()
{
    if
    (
        this->readOpt() == IOobject::MUST_READ
     || this->readOpt() == IOobject::MUST_READ_IF_MODIFIED
    )
    {
        WarningInFunction
            << "read option IOobject::MUST_READ or MUST_READ_IF_MODIFIED"
            << " suggests that a read constructor for field " << this->name()
            << " would be more appropriate." << endl;
    }
    else if (this->readOpt() == IOobject::READ_IF_PRESENT && this->headerOk())
    {
        boundaryField_.transfer(readField(this->readStream(typeName))());
        this->close();

        // Check compatibility between field and mesh
        if (this->size() != GeoMesh::size(this->mesh()))
        {
            FatalIOErrorInFunction(this->readStream(typeName))
                << "   number of field elements = " << this->size()
                << " number of mesh elements = "
                << GeoMesh::size(this->mesh())
                << exit(FatalIOError);
        }

        readOldTimeIfPresent();

        return true;
    }

    return false;
}


template<class Type, template<class> class PatchField, class GeoMesh>
bool CML::GeometricField<Type, PatchField, GeoMesh>::readOldTimeIfPresent()
{
    // Read the old time field if present
    IOobject field0
    (
        this->name()  + "_0",
        this->time().timeName(),
        this->db(),
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE,
        this->registerObject()
    );

    if (field0.headerOk())
    {
        if (debug)
        {
            Info<< "Reading old time level for field"
                << endl << this->info() << endl;
        }

        field0Ptr_ = new GeometricField<Type, PatchField, GeoMesh>
        (
            field0,
            this->mesh()
        );

        field0Ptr_->timeIndex_ = timeIndex_ - 1;

        if (!field0Ptr_->readOldTimeIfPresent())
        {
            field0Ptr_->oldTime();
        }

        return true;
    }

    return false;
}


// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

// Constructor given a GeometricField and dimensionSet
// This allocates storage for the field but not values.
// Note : This constructor should only be used to
//       construct TEMPORARY variables
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const IOobject& io,
    const Mesh& mesh,
    const dimensionSet& ds,
    const word& patchFieldType
)
:
    DimensionedField<Type, GeoMesh>(io, mesh, ds, false),
    timeIndex_(this->time().timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(mesh.boundary(), *this, patchFieldType)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "creating temporary"
            << endl << this->info() << endl;
    }

    readIfPresent();
}


// Constructor given a GeometricField and dimensionSet
// This allocates storage for the field but not values.
// Note : This constructor should only be used to
//       construct TEMPORARY variables
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const IOobject& io,
    const Mesh& mesh,
    const dimensionSet& ds,
    const wordList& patchFieldTypes,
    const wordList& actualPatchTypes
)
:
    DimensionedField<Type, GeoMesh>(io, mesh, ds, false),
    timeIndex_(this->time().timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(mesh.boundary(), *this, patchFieldTypes, actualPatchTypes)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "creating temporary"
            << endl << this->info() << endl;
    }

    readIfPresent();
}


// Constructor given a GeometricField and dimensioned<Type>
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const IOobject& io,
    const Mesh& mesh,
    const dimensioned<Type>& dt,
    const word& patchFieldType
)
:
    DimensionedField<Type, GeoMesh>(io, mesh, dt, false),
    timeIndex_(this->time().timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(mesh.boundary(), *this, patchFieldType)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "creating temporary"
            << endl << this->info() << endl;
    }

    boundaryField_ == dt.value();

    readIfPresent();
}


// Constructor given a GeometricField and dimensioned<Type>
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const IOobject& io,
    const Mesh& mesh,
    const dimensioned<Type>& dt,
    const wordList& patchFieldTypes,
    const wordList& actualPatchTypes
)
:
    DimensionedField<Type, GeoMesh>(io, mesh, dt, false),
    timeIndex_(this->time().timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(mesh.boundary(), *this, patchFieldTypes, actualPatchTypes)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "creating temporary"
            << endl << this->info() << endl;
    }

    boundaryField_ == dt.value();

    readIfPresent();
}


//  construct from components
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const IOobject& io,
    const Mesh& mesh,
    const dimensionSet& ds,
    const Field<Type>& iField,
    const PtrList<PatchField<Type> >& ptfl
)
:
    DimensionedField<Type, GeoMesh>(io, mesh, ds, iField),
    timeIndex_(this->time().timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(mesh.boundary(), *this, ptfl)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "constructing from components"
            << endl << this->info() << endl;
    }

    readIfPresent();
}


template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const IOobject& io,
    const Mesh& mesh,
    const bool readOldTime
)
:
    DimensionedField<Type, GeoMesh>(io, mesh, dimless, false),
    timeIndex_(this->time().timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(*this, readField(this->readStream(typeName)))
{
    this->close();

    // Check compatibility between field and mesh

    if (this->size() != GeoMesh::size(this->mesh()))
    {
        FatalIOErrorInFunction(this->readStream(typeName))
            << "   number of field elements = " << this->size()
            << " number of mesh elements = " << GeoMesh::size(this->mesh())
            << exit(FatalIOError);
    }

    if (readOldTime)
    {
        readOldTimeIfPresent();
    }

    if (debug)
    {
        Info<< "Finishing read-construct of "
               "GeometricField<Type, PatchField, GeoMesh>"
            << endl << this->info() << endl;
    }
}


template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const IOobject& io,
    const Mesh& mesh,
    const dictionary& dict
)
:
    DimensionedField<Type, GeoMesh>(io, mesh, dimless, false),
    timeIndex_(this->time().timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(*this, readField(dict))
{
    // Check compatibility between field and mesh

    if (this->size() != GeoMesh::size(this->mesh()))
    {
        FatalErrorInFunction
            << "   number of field elements = " << this->size()
            << " number of mesh elements = " << GeoMesh::size(this->mesh())
            << exit(FatalIOError);
    }

    readOldTimeIfPresent();

    if (debug)
    {
        Info<< "Finishing dictionary-construct of "
               "GeometricField<Type, PatchField, GeoMesh>"
            << endl << this->info() << endl;
    }
}


// construct as copy
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
:
    DimensionedField<Type, GeoMesh>(gf),
    timeIndex_(gf.timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(*this, gf.boundaryField_)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "constructing as copy"
            << endl << this->info() << endl;
    }

    if (gf.field0Ptr_)
    {
        field0Ptr_ = new GeometricField<Type, PatchField, GeoMesh>
        (
            *gf.field0Ptr_
        );
    }

    this->writeOpt() = IOobject::NO_WRITE;
}

// construct as copy of tmp<GeometricField> deleting argument
#ifdef ConstructFromTmp
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf
)
:
    DimensionedField<Type, GeoMesh>
    (
        const_cast<GeometricField<Type, PatchField, GeoMesh>&>(tgf()),
        tgf.isTmp()
    ),
    timeIndex_(tgf().timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(*this, tgf().boundaryField_)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "constructing as copy"
            << endl << this->info() << endl;
    }

    this->writeOpt() = IOobject::NO_WRITE;

    tgf.clear();
}
#endif


// construct as copy resetting IO parameters
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const IOobject& io,
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
:
    DimensionedField<Type, GeoMesh>(io, gf),
    timeIndex_(gf.timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(*this, gf.boundaryField_)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "constructing as copy resetting IO params"
            << endl << this->info() << endl;
    }

    if (!readIfPresent() && gf.field0Ptr_)
    {
        field0Ptr_ = new GeometricField<Type, PatchField, GeoMesh>
        (
            io.name() + "_0",
            *gf.field0Ptr_
        );
    }
}


#ifdef ConstructFromTmp
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const IOobject& io,
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf
)
:
    DimensionedField<Type, GeoMesh>
    (
        io,
        const_cast<GeometricField<Type, PatchField, GeoMesh>&>(tgf()),
        tgf.isTmp()
    ),
    timeIndex_(tgf().timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(*this, tgf().boundaryField_)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "constructing from tmp resetting IO params"
            << endl << this->info() << endl;
    }

    tgf.clear();

    readIfPresent();
}
#endif


// construct as copy resetting name
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const word& newName,
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
:
    DimensionedField<Type, GeoMesh>(newName, gf),
    timeIndex_(gf.timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(*this, gf.boundaryField_)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "constructing as copy resetting name"
            << endl << this->info() << endl;
    }

    if (!readIfPresent() && gf.field0Ptr_)
    {
        field0Ptr_ = new GeometricField<Type, PatchField, GeoMesh>
        (
            newName + "_0",
            *gf.field0Ptr_
        );
    }
}


// construct as copy resetting name
#ifdef ConstructFromTmp
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const word& newName,
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf
)
:
    DimensionedField<Type, GeoMesh>
    (
        newName,
        const_cast<GeometricField<Type, PatchField, GeoMesh>&>(tgf()),
        tgf.isTmp()
    ),
    timeIndex_(tgf().timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(*this, tgf().boundaryField_)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "constructing from tmp resetting name"
            << endl << this->info() << endl;
    }

    tgf.clear();
}
#endif

// construct as copy resetting IO parameters and patch type
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const IOobject& io,
    const GeometricField<Type, PatchField, GeoMesh>& gf,
    const word& patchFieldType
)
:
    DimensionedField<Type, GeoMesh>(io, gf),
    timeIndex_(gf.timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_(this->mesh().boundary(), *this, patchFieldType)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "constructing as copy resetting IO params"
            << endl << this->info() << endl;
    }

    boundaryField_ == gf.boundaryField_;

    if (!readIfPresent() && gf.field0Ptr_)
    {
        field0Ptr_ = new GeometricField<Type, PatchField, GeoMesh>
        (
            io.name() + "_0",
            *gf.field0Ptr_
        );
    }
}


// construct as copy resetting IO parameters and boundary types
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricField
(
    const IOobject& io,
    const GeometricField<Type, PatchField, GeoMesh>& gf,
    const wordList& patchFieldTypes,
    const wordList& actualPatchTypes

)
:
    DimensionedField<Type, GeoMesh>(io, gf),
    timeIndex_(gf.timeIndex()),
    field0Ptr_(nullptr),
    fieldPrevIterPtr_(nullptr),
    boundaryField_
    (
        this->mesh().boundary(),
        *this,
        patchFieldTypes,
        actualPatchTypes
    )
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::GeometricField : "
               "constructing as copy resetting IO params"
            << endl << this->info() << endl;
    }

    boundaryField_ == gf.boundaryField_;

    if (!readIfPresent() && gf.field0Ptr_)
    {
        field0Ptr_ = new GeometricField<Type, PatchField, GeoMesh>
        (
            io.name() + "_0",
            *gf.field0Ptr_
        );
    }
}


// * * * * * * * * * * * * * * * Destructor * * * * * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::~GeometricField()
{
    deleteDemandDrivenData(field0Ptr_);
    deleteDemandDrivenData(fieldPrevIterPtr_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
typename
CML::GeometricField<Type, PatchField, GeoMesh>::DimensionedInternalField&
CML::GeometricField<Type, PatchField, GeoMesh>::dimensionedInternalField()
{
    this->setUpToDate();
    storeOldTimes();
    return *this;
}


template<class Type, template<class> class PatchField, class GeoMesh>
typename
CML::GeometricField<Type, PatchField, GeoMesh>::InternalField&
CML::GeometricField<Type, PatchField, GeoMesh>::internalField()
{
    this->setUpToDate();
    storeOldTimes();
    return *this;
}


// Return reference to GeometricBoundaryField
template<class Type, template<class> class PatchField, class GeoMesh>
typename
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField&
CML::GeometricField<Type, PatchField, GeoMesh>::boundaryField()
{
    this->setUpToDate();
    storeOldTimes();
    return boundaryField_;
}


// Store old-time field
template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::storeOldTimes() const
{
    if
    (
        field0Ptr_
     && timeIndex_ != this->time().timeIndex()
     && !(
            this->name().size() > 2
         && this->name()(this->name().size()-2, 2) == "_0"
         )
    )
    {
        storeOldTime();
    }

    // Correct time index
    timeIndex_ = this->time().timeIndex();
}

// Store old-time field
template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::storeOldTime() const
{
    if (field0Ptr_)
    {
        field0Ptr_->storeOldTime();

        if (debug)
        {
            Info<< "Storing old time field for field" << endl
                << this->info() << endl;
        }

        *field0Ptr_ == *this;
        field0Ptr_->timeIndex_ = timeIndex_;

        if (field0Ptr_->field0Ptr_)
        {
            field0Ptr_->writeOpt() = this->writeOpt();
        }
    }
}

// Return the number of old time fields stored
template<class Type, template<class> class PatchField, class GeoMesh>
CML::label CML::GeometricField<Type, PatchField, GeoMesh>::nOldTimes() const
{
    if (field0Ptr_)
    {
        return field0Ptr_->nOldTimes() + 1;
    }
    else
    {
        return 0;
    }
}

// Return old time internal field
template<class Type, template<class> class PatchField, class GeoMesh>
const CML::GeometricField<Type, PatchField, GeoMesh>&
CML::GeometricField<Type, PatchField, GeoMesh>::oldTime() const
{
    if (!field0Ptr_)
    {
        field0Ptr_ = new GeometricField<Type, PatchField, GeoMesh>
        (
            IOobject
            (
                this->name() + "_0",
                this->time().timeName(),
                this->db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                this->registerObject()
            ),
            *this
        );
    }
    else
    {
        storeOldTimes();
    }

    return *field0Ptr_;
}

// Return old time internal field
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>&
CML::GeometricField<Type, PatchField, GeoMesh>::oldTime()
{
    static_cast<const GeometricField<Type, PatchField, GeoMesh>&>(*this)
        .oldTime();

    return *field0Ptr_;
}


// Store previous iteration field
template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::storePrevIter() const
{
    if (!fieldPrevIterPtr_)
    {
        if (debug)
        {
            Info<< "Allocating previous iteration field" << endl
                << this->info() << endl;
        }

        fieldPrevIterPtr_ = new GeometricField<Type, PatchField, GeoMesh>
        (
            this->name() + "PrevIter",
            *this
        );
    }
    else
    {
        *fieldPrevIterPtr_ == *this;
    }
}


// Return previous iteration field
template<class Type, template<class> class PatchField, class GeoMesh>
const CML::GeometricField<Type, PatchField, GeoMesh>&
CML::GeometricField<Type, PatchField, GeoMesh>::prevIter() const
{
    if (!fieldPrevIterPtr_)
    {
        FatalErrorInFunction
            << "previous iteration field" << endl << this->info() << endl
            << "  not stored."
            << "  Use field.storePrevIter() at start of iteration."
            << abort(FatalError);
    }

    return *fieldPrevIterPtr_;
}


// Correct the boundary conditions
template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::
correctBoundaryConditions()
{
    this->setUpToDate();
    storeOldTimes();
    boundaryField_.evaluate();
}


// Does the field need a reference level for solution
template<class Type, template<class> class PatchField, class GeoMesh>
bool CML::GeometricField<Type, PatchField, GeoMesh>::needReference() const
{
    // Search all boundary conditions, if any are
    // fixed-value or mixed (Robin) do not set reference level for solution.

    bool needRef = true;

    forAll(boundaryField_, patchi)
    {
        if (boundaryField_[patchi].fixesValue())
        {
            needRef = false;
            break;
        }
    }

    reduce(needRef, andOp<bool>());

    return needRef;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::relax(const scalar alpha)
{
    if (debug)
    {
        InfoInFunction
           << "Relaxing" << endl << this->info() << " by " << alpha << endl;
    }

    operator==(prevIter() + alpha*(*this - prevIter()));
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::relax()
{
    word name = this->name();

    if
    (
        this->mesh().data::template lookupOrDefault<bool>
        (
            "finalIteration",
            false
        )
    )
    {
        name += "Final";
    }

    if (this->mesh().relaxField(name))
    {
        relax(this->mesh().fieldRelaxationFactor(name));
    }
}


template<class Type, template<class> class PatchField, class GeoMesh>
CML::word CML::GeometricField<Type, PatchField, GeoMesh>::select
(
    bool final
) const
{
    if (final)
    {
        return this->name() + "Final";
    }
    else
    {
        return this->name();
    }
}


// writeData member function required by regIOobject
template<class Type, template<class> class PatchField, class GeoMesh>
bool CML::GeometricField<Type, PatchField, GeoMesh>::
writeData(Ostream& os) const
{
    os << *this;
    return os.good();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
CML::tmp<CML::GeometricField<Type, PatchField, GeoMesh> >
CML::GeometricField<Type, PatchField, GeoMesh>::T() const
{
    tmp<GeometricField<Type, PatchField, GeoMesh> > result
    (
        new GeometricField<Type, PatchField, GeoMesh>
        (
            IOobject
            (
                this->name() + ".T()",
                this->instance(),
                this->db()
            ),
            this->mesh(),
            this->dimensions()
        )
    );

    CML::T(result().internalField(), internalField());
    CML::T(result().boundaryField(), boundaryField());

    return result;
}


template<class Type, template<class> class PatchField, class GeoMesh>
CML::tmp
<
    CML::GeometricField
    <
        typename CML::GeometricField<Type, PatchField, GeoMesh>::cmptType,
        PatchField,
        GeoMesh
    >
>
CML::GeometricField<Type, PatchField, GeoMesh>::component
(
    const direction d
) const
{
    tmp<GeometricField<cmptType, PatchField, GeoMesh> > Component
    (
        new GeometricField<cmptType, PatchField, GeoMesh>
        (
            IOobject
            (
                this->name() + ".component(" + CML::name(d) + ')',
                this->instance(),
                this->db()
            ),
            this->mesh(),
            this->dimensions()
        )
    );

    CML::component(Component().internalField(), internalField(), d);
    CML::component(Component().boundaryField(), boundaryField(), d);

    return Component;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::replace
(
    const direction d,
    const GeometricField
    <
        typename GeometricField<Type, PatchField, GeoMesh>::cmptType,
        PatchField,
        GeoMesh
     >& gcf
)
{
    internalField().replace(d, gcf.internalField());
    boundaryField().replace(d, gcf.boundaryField());
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::replace
(
    const direction d,
    const dimensioned<cmptType>& ds
)
{
    internalField().replace(d, ds.value());
    boundaryField().replace(d, ds.value());
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::max
(
    const dimensioned<Type>& dt
)
{
    CML::max(internalField(), internalField(), dt.value());
    CML::max(boundaryField(), boundaryField(), dt.value());
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::min
(
    const dimensioned<Type>& dt
)
{
    CML::min(internalField(), internalField(), dt.value());
    CML::min(boundaryField(), boundaryField(), dt.value());
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::negate()
{
    internalField().negate();
    boundaryField().negate();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::operator=
(
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
    if (this == &gf)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    checkField(*this, gf, "=");

    // only equate field contents not ID

    dimensionedInternalField() = gf.dimensionedInternalField();
    boundaryField() = gf.boundaryField();
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::operator=
(
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf
)
{
    if (this == &(tgf()))
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();

    checkField(*this, gf, "=");

    // only equate field contents not ID

    this->dimensions() = gf.dimensions();

    // This is dodgy stuff, don't try it at home.
    internalField().transfer
    (
        const_cast<Field<Type>&>(gf.internalField())
    );

    boundaryField() = gf.boundaryField();

    tgf.clear();
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::operator=
(
    const dimensioned<Type>& dt
)
{
    dimensionedInternalField() = dt;
    boundaryField() = dt.value();
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::operator==
(
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf
)
{
    const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();

    checkField(*this, gf, "==");

    // only equate field contents not ID

    dimensionedInternalField() = gf.dimensionedInternalField();
    boundaryField() == gf.boundaryField();

    tgf.clear();
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::operator==
(
    const dimensioned<Type>& dt
)
{
    dimensionedInternalField() = dt;
    boundaryField() == dt.value();
}


#define COMPUTED_ASSIGNMENT(TYPE, op)                                         \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
void CML::GeometricField<Type, PatchField, GeoMesh>::operator op             \
(                                                                             \
    const GeometricField<TYPE, PatchField, GeoMesh>& gf                       \
)                                                                             \
{                                                                             \
    checkField(*this, gf, #op);                                               \
                                                                              \
    dimensionedInternalField() op gf.dimensionedInternalField();              \
    boundaryField() op gf.boundaryField();                                    \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
void CML::GeometricField<Type, PatchField, GeoMesh>::operator op             \
(                                                                             \
    const tmp<GeometricField<TYPE, PatchField, GeoMesh> >& tgf                \
)                                                                             \
{                                                                             \
    operator op(tgf());                                                       \
    tgf.clear();                                                              \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
void CML::GeometricField<Type, PatchField, GeoMesh>::operator op             \
(                                                                             \
    const dimensioned<TYPE>& dt                                               \
)                                                                             \
{                                                                             \
    dimensionedInternalField() op dt;                                         \
    boundaryField() op dt.value();                                            \
}

COMPUTED_ASSIGNMENT(Type, +=)
COMPUTED_ASSIGNMENT(Type, -=)
COMPUTED_ASSIGNMENT(scalar, *=)
COMPUTED_ASSIGNMENT(scalar, /=)

#undef COMPUTED_ASSIGNMENT


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
    gf.dimensionedInternalField().writeData(os, "internalField");
    os  << nl;
    gf.boundaryField().writeEntry("boundaryField", os);

    // Check state of IOstream
    os.check
    (
        "Ostream& operator<<(Ostream&, "
        "const GeometricField<Type, PatchField, GeoMesh>&)"
    );

    return (os);
}


template<class Type, template<class> class PatchField, class GeoMesh>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf
)
{
    os << tgf();
    tgf.clear();
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#undef checkField

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "emptyPolyPatch.hpp"
#include "commSchedule.hpp"
#include "globalMeshData.hpp"
#include "cyclicPolyPatch.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
GeometricBoundaryField
(
    const BoundaryMesh& bmesh,
    const DimensionedField<Type, GeoMesh>& field,
    const word& patchFieldType
)
:
    FieldField<PatchField, Type>(bmesh.size()),
    bmesh_(bmesh)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::"
               "GeometricBoundaryField::"
               "GeometricBoundaryField(const BoundaryMesh&, "
               "const Field<Type>&, const word&)"
            << endl;
    }

    forAll(bmesh_, patchi)
    {
        this->set
        (
            patchi,
            PatchField<Type>::New
            (
                patchFieldType,
                bmesh_[patchi],
                field
            )
        );
    }
}


template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
GeometricBoundaryField
(
    const BoundaryMesh& bmesh,
    const DimensionedField<Type, GeoMesh>& field,
    const wordList& patchFieldTypes,
    const wordList& constraintTypes
)
:
    FieldField<PatchField, Type>(bmesh.size()),
    bmesh_(bmesh)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::"
               "GeometricBoundaryField::"
               "GeometricBoundaryField(const BoundaryMesh&, "
               "const Field<Type>&, const wordList&, const wordList&)"
            << endl;
    }

    if
    (
        patchFieldTypes.size() != this->size()
     || (constraintTypes.size() && (constraintTypes.size() != this->size()))
    )
    {
        FatalErrorInFunction
            << "Incorrect number of patch type specifications given" << nl
            << "    Number of patches in mesh = " << bmesh.size()
            << " number of patch type specifications = "
            << patchFieldTypes.size()
            << abort(FatalError);
    }

    if (constraintTypes.size())
    {
        forAll(bmesh_, patchi)
        {
            this->set
            (
                patchi,
                PatchField<Type>::New
                (
                    patchFieldTypes[patchi],
                    constraintTypes[patchi],
                    bmesh_[patchi],
                    field
                )
            );
        }
    }
    else
    {
        forAll(bmesh_, patchi)
        {
            this->set
            (
                patchi,
                PatchField<Type>::New
                (
                    patchFieldTypes[patchi],
                    bmesh_[patchi],
                    field
                )
            );
        }
    }
}


template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
GeometricBoundaryField
(
    const BoundaryMesh& bmesh,
    const DimensionedField<Type, GeoMesh>& field,
    const PtrList<PatchField<Type> >& ptfl
)
:
    FieldField<PatchField, Type>(bmesh.size()),
    bmesh_(bmesh)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::"
               "GeometricBoundaryField::"
               "GeometricBoundaryField(const BoundaryMesh&, "
               "const Field<Type>&, const PatchField<Type>List&)"
            << endl;
    }

    forAll(bmesh_, patchi)
    {
        this->set(patchi, ptfl[patchi].clone(field));
    }
}


template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
GeometricBoundaryField
(
    const DimensionedField<Type, GeoMesh>& field,
    const typename GeometricField<Type, PatchField, GeoMesh>::
    GeometricBoundaryField& btf
)
:
    FieldField<PatchField, Type>(btf.size()),
    bmesh_(btf.bmesh_)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::"
               "GeometricBoundaryField::"
               "GeometricBoundaryField(const GeometricBoundaryField<Type, "
               "PatchField, BoundaryMesh>&)"
            << endl;
    }

    forAll(bmesh_, patchi)
    {
        this->set(patchi, btf[patchi].clone(field));
    }
}


// Construct as copy
// Dangerous because Field may be set to a field which gets deleted.
// Need new type of GeometricBoundaryField, one which IS part of a geometric
// field for which snGrad etc. may be called and a free standing
// GeometricBoundaryField for which such operations are unavailable.
template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
GeometricBoundaryField
(
    const typename GeometricField<Type, PatchField, GeoMesh>::
    GeometricBoundaryField& btf
)
:
    FieldField<PatchField, Type>(btf),
    bmesh_(btf.bmesh_)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::"
               "GeometricBoundaryField::"
               "GeometricBoundaryField(const GeometricBoundaryField<Type, "
               "PatchField, BoundaryMesh>&)"
            << endl;
    }
}


template<class Type, template<class> class PatchField, class GeoMesh>
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
GeometricBoundaryField
(
    const BoundaryMesh& bmesh,
    const DimensionedField<Type, GeoMesh>& field,
    const dictionary& dict
)
:
    FieldField<PatchField, Type>(bmesh.size()),
    bmesh_(bmesh)
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::"
               "GeometricBoundaryField::"
               "GeometricBoundaryField"
               "(const BoundaryMesh&, const Field<Type>&, const dictionary&)"
            << endl;
    }

    forAll(bmesh_, patchi)
    {
        if (bmesh_[patchi].type() != emptyPolyPatch::typeName)
        {
            if
            (
                bmesh_[patchi].type() == cyclicPolyPatch::typeName
             && !dict.found(bmesh_[patchi].name())
            )
            {
                FatalIOErrorInFunction(dict)
                    << "Cannot find patchField entry for cyclic "
                    << bmesh_[patchi].name() << endl
                    << "Is your field uptodate with split cyclics?" << endl
                    << "Run caelusUpgradeCyclics to convert mesh and fields"
                    << " to split cyclics." << exit(FatalIOError);
            }

            this->set
            (
                patchi,
                PatchField<Type>::New
                (
                    bmesh_[patchi],
                    field,
                    dict.subDict(bmesh_[patchi].name())
                )
            );
        }
        else
        {
            this->set
            (
                patchi,
                PatchField<Type>::New
                (
                    emptyPolyPatch::typeName,
                    bmesh_[patchi],
                    field
                )
            );
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
updateCoeffs()
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::"
               "GeometricBoundaryField::"
               "updateCoeffs()" << endl;
    }

    forAll(*this, patchi)
    {
        this->operator[](patchi).updateCoeffs();
    }
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
evaluate()
{
    if (debug)
    {
        Info<< "GeometricField<Type, PatchField, GeoMesh>::"
               "GeometricBoundaryField::"
               "evaluate()" << endl;
    }

    if
    (
        Pstream::defaultCommsType == Pstream::blocking
     || Pstream::defaultCommsType == Pstream::nonBlocking
    )
    {
        label nReq = Pstream::nRequests();

        forAll(*this, patchi)
        {
            this->operator[](patchi).initEvaluate(Pstream::defaultCommsType);
        }

        // Block for any outstanding requests
        if
        (
            Pstream::parRun()
         && Pstream::defaultCommsType == Pstream::nonBlocking
        )
        {
            Pstream::waitRequests(nReq);
        }

        forAll(*this, patchi)
        {
            this->operator[](patchi).evaluate(Pstream::defaultCommsType);
        }
    }
    else if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        const lduSchedule& patchSchedule =
            bmesh_.mesh().globalData().patchSchedule();

        forAll(patchSchedule, patchEvali)
        {
            if (patchSchedule[patchEvali].init)
            {
                this->operator[](patchSchedule[patchEvali].patch)
                    .initEvaluate(Pstream::scheduled);
            }
            else
            {
                this->operator[](patchSchedule[patchEvali].patch)
                    .evaluate(Pstream::scheduled);
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "Unsupported communications type "
            << Pstream::commsTypeNames[Pstream::defaultCommsType]
            << exit(FatalError);
    }
}


template<class Type, template<class> class PatchField, class GeoMesh>
CML::wordList
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
types() const
{
    const FieldField<PatchField, Type>& pff = *this;

    wordList Types(pff.size());

    forAll(pff, patchi)
    {
        Types[patchi] = pff[patchi].type();
    }

    return Types;
}


template<class Type, template<class> class PatchField, class GeoMesh>
typename CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
boundaryInternalField() const
{
    typename GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField
        BoundaryInternalField(*this);

    forAll(BoundaryInternalField, patchi)
    {
        BoundaryInternalField[patchi] ==
            this->operator[](patchi).patchInternalField();
    }

    return BoundaryInternalField;
}


template<class Type, template<class> class PatchField, class GeoMesh>
CML::lduInterfaceFieldPtrsList
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
interfaces() const
{
    lduInterfaceFieldPtrsList interfaces(this->size());

    forAll(interfaces, patchi)
    {
        if (isA<lduInterfaceField>(this->operator[](patchi)))
        {
            interfaces.set
            (
                patchi,
                &refCast<const lduInterfaceField>(this->operator[](patchi))
            );
        }
    }

    return interfaces;
}


template<class Type, template<class> class PatchField, class GeoMesh>
typename CML::BlockLduInterfaceFieldPtrsList<Type>::Type
CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
blockInterfaces() const
{
    typename BlockLduInterfaceFieldPtrsList<Type>::Type interfaces
    (
        this->size()
    );

    forAll (interfaces, patchi)
    {
        if (isA<BlockLduInterfaceField<Type> >(this->operator[](patchi)))
        {
            interfaces.set
            (
                patchi,
                &refCast<const BlockLduInterfaceField<Type> >
                (
                    this->operator[](patchi)
                )
            );
        }
    }

    return interfaces;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
writeEntry(const word& keyword, Ostream& os) const
{
    os  << keyword << nl << token::BEGIN_BLOCK << incrIndent << nl;

    forAll(*this, patchi)
    {
        os  << indent << this->operator[](patchi).patch().name() << nl
            << indent << token::BEGIN_BLOCK << nl
            << incrIndent << this->operator[](patchi) << decrIndent
            << indent << token::END_BLOCK << endl;
    }

    os  << decrIndent << token::END_BLOCK << endl;

    // Check state of IOstream
    os.check
    (
        "GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::"
        "writeEntry(const word& keyword, Ostream& os) const"
    );
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
operator=
(
    const typename GeometricField<Type, PatchField, GeoMesh>::
    GeometricBoundaryField& bf
)
{
    FieldField<PatchField, Type>::operator=(bf);
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
operator=
(
    const FieldField<PatchField, Type>& ptff
)
{
    FieldField<PatchField, Type>::operator=(ptff);
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
operator=
(
    const Type& t
)
{
    FieldField<PatchField, Type>::operator=(t);
}


// Forced assignments
template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
operator==
(
    const typename GeometricField<Type, PatchField, GeoMesh>::
    GeometricBoundaryField& bf
)
{
    forAll((*this), patchI)
    {
        this->operator[](patchI) == bf[patchI];
    }
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
operator==
(
    const FieldField<PatchField, Type>& ptff
)
{
    forAll((*this), patchI)
    {
        this->operator[](patchI) == ptff[patchI];
    }
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::GeometricField<Type, PatchField, GeoMesh>::GeometricBoundaryField::
operator==
(
    const Type& t
)
{
    forAll((*this), patchI)
    {
        this->operator[](patchI) == t;
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const typename GeometricField<Type, PatchField, GeoMesh>::
    GeometricBoundaryField& bf
)
{
    os << static_cast<const FieldField<PatchField, Type>&>(bf);
    return os;
}


#include "GeometricFieldReuseFunctions.hpp"

#define TEMPLATE \
    template<class Type, template<class> class PatchField, class GeoMesh>
#include "GeometricFieldFunctionsM.hxx"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
void component
(
    GeometricField
    <
        typename GeometricField<Type, PatchField, GeoMesh>::cmptType,
        PatchField,
        GeoMesh
    >& gcf,
    const GeometricField<Type, PatchField, GeoMesh>& gf,
    const direction d
)
{
    component(gcf.internalField(), gf.internalField(), d);
    component(gcf.boundaryField(), gf.boundaryField(), d);
}


template<class Type, template<class> class PatchField, class GeoMesh>
void T
(
     GeometricField<Type, PatchField, GeoMesh>& gf,
     const GeometricField<Type, PatchField, GeoMesh>& gf1
)
{
    T(gf.internalField(), gf1.internalField());
    T(gf.boundaryField(), gf1.boundaryField());
}


template<class Type, template<class> class PatchField, class GeoMesh, direction r>
void pow
(
    GeometricField<typename powProduct<Type, r>::type, PatchField, GeoMesh>& gf,
    const GeometricField<Type, PatchField, GeoMesh>& gf1
)
{
    pow(gf.internalField(), gf1.internalField(), r);
    pow(gf.boundaryField(), gf1.boundaryField(), r);
}

template<class Type, template<class> class PatchField, class GeoMesh, direction r>
tmp<GeometricField<typename powProduct<Type, r>::type, PatchField, GeoMesh> >
pow
(
    const GeometricField<Type, PatchField, GeoMesh>& gf,
    typename powProduct<Type, r>::type
)
{
    typedef typename powProduct<Type, r>::type powProductType;

    tmp<GeometricField<powProductType, PatchField, GeoMesh> > tPow
    (
        new GeometricField<powProductType, PatchField, GeoMesh>
        (
            IOobject
            (
                "pow(" + gf.name() + ',' + name(r) + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            pow(gf.dimensions(), r)
        )
    );

    pow<Type, r, PatchField, GeoMesh>(tPow(), gf);

    return tPow;
}


template<class Type, template<class> class PatchField, class GeoMesh, direction r>
tmp<GeometricField<typename powProduct<Type, r>::type, PatchField, GeoMesh> >
pow
(
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf,
    typename powProduct<Type, r>::type
)
{
    typedef typename powProduct<Type, r>::type powProductType;

    const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();

    tmp<GeometricField<powProductType, PatchField, GeoMesh> > tPow
    (
        new GeometricField<powProductType, PatchField, GeoMesh>
        (
            IOobject
            (
                "pow(" + gf.name() + ',' + name(r) + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            pow(gf.dimensions(), r)
        )
    );

    pow<Type, r, PatchField, GeoMesh>(tPow(), gf);

    tgf.clear();

    return tPow;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void sqr
(
    GeometricField
    <typename outerProduct<Type, Type>::type, PatchField, GeoMesh>& gf,
    const GeometricField<Type, PatchField, GeoMesh>& gf1
)
{
    sqr(gf.internalField(), gf1.internalField());
    sqr(gf.boundaryField(), gf1.boundaryField());
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp
<
    GeometricField
    <
        typename outerProduct<Type, Type>::type,
        PatchField,
        GeoMesh
    >
>
sqr(const GeometricField<Type, PatchField, GeoMesh>& gf)
{
    typedef typename outerProduct<Type, Type>::type outerProductType;

    tmp<GeometricField<outerProductType, PatchField, GeoMesh> > tSqr
    (
        new GeometricField<outerProductType, PatchField, GeoMesh>
        (
            IOobject
            (
                "sqr(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            sqr(gf.dimensions())
        )
    );

    sqr(tSqr(), gf);

    return tSqr;
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp
<
    GeometricField
    <
        typename outerProduct<Type, Type>::type,
        PatchField,
        GeoMesh
    >
>
sqr(const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf)
{
    typedef typename outerProduct<Type, Type>::type outerProductType;

    const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();

    tmp<GeometricField<outerProductType, PatchField, GeoMesh> > tSqr
    (
        new GeometricField<outerProductType, PatchField, GeoMesh>
        (
            IOobject
            (
                "sqr(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            sqr(gf.dimensions())
        )
    );

    sqr(tSqr(), gf);

    tgf.clear();

    return tSqr;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void magSqr
(
    GeometricField<scalar, PatchField, GeoMesh>& gsf,
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
    magSqr(gsf.internalField(), gf.internalField());
    magSqr(gsf.boundaryField(), gf.boundaryField());
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<scalar, PatchField, GeoMesh> > magSqr
(
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
    tmp<GeometricField<scalar, PatchField, GeoMesh> > tMagSqr
    (
        new GeometricField<scalar, PatchField, GeoMesh>
        (
            IOobject
            (
                "magSqr(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            sqr(gf.dimensions())
        )
    );

    magSqr(tMagSqr(), gf);

    return tMagSqr;
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<scalar, PatchField, GeoMesh> > magSqr
(
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf
)
{
    const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();

    tmp<GeometricField<scalar, PatchField, GeoMesh> > tMagSqr
    (
        new GeometricField<scalar, PatchField, GeoMesh>
        (
            IOobject
            (
                "magSqr(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            sqr(gf.dimensions())
        )
    );

    magSqr(tMagSqr(), gf);

    tgf.clear();

    return tMagSqr;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mag
(
    GeometricField<scalar, PatchField, GeoMesh>& gsf,
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
    mag(gsf.internalField(), gf.internalField());
    mag(gsf.boundaryField(), gf.boundaryField());
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<scalar, PatchField, GeoMesh> > mag
(
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
    tmp<GeometricField<scalar, PatchField, GeoMesh> > tMag
    (
        new GeometricField<scalar, PatchField, GeoMesh>
        (
            IOobject
            (
                "mag(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            gf.dimensions()
        )
    );

    mag(tMag(), gf);

    return tMag;
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp<GeometricField<scalar, PatchField, GeoMesh> > mag
(
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf
)
{
    const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();

    tmp<GeometricField<scalar, PatchField, GeoMesh> > tMag
    (
        new GeometricField<scalar, PatchField, GeoMesh>
        (
            IOobject
            (
                "mag(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            gf.dimensions()
        )
    );

    mag(tMag(), gf);

    tgf.clear();

    return tMag;
}


template<class Type, template<class> class PatchField, class GeoMesh>
void cmptAv
(
    GeometricField
    <
        typename GeometricField<Type, PatchField, GeoMesh>::cmptType,
        PatchField,
        GeoMesh
    >& gcf,
    const GeometricField<Type, PatchField, GeoMesh>& gf
)
{
    cmptAv(gcf.internalField(), gf.internalField());
    cmptAv(gcf.boundaryField(), gf.boundaryField());
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp
<
    GeometricField
    <
        typename GeometricField<Type, PatchField, GeoMesh>::cmptType,
        PatchField,
        GeoMesh
    >
>
cmptAv(const GeometricField<Type, PatchField, GeoMesh>& gf)
{
    typedef typename GeometricField<Type, PatchField, GeoMesh>::cmptType
        cmptType;

    tmp<GeometricField<cmptType, PatchField, GeoMesh> > CmptAv
    (
        new GeometricField<scalar, PatchField, GeoMesh>
        (
            IOobject
            (
                "cmptAv(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            gf.dimensions()
        )
    );

    cmptAv(CmptAv(), gf);

    return CmptAv;
}

template<class Type, template<class> class PatchField, class GeoMesh>
tmp
<
    GeometricField
    <
        typename GeometricField<Type, PatchField, GeoMesh>::cmptType,
        PatchField,
        GeoMesh
    >
>
cmptAv(const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf)
{
    typedef typename GeometricField<Type, PatchField, GeoMesh>::cmptType
        cmptType;

    const GeometricField<Type, PatchField, GeoMesh>& gf = tgf();

    tmp<GeometricField<cmptType, PatchField, GeoMesh> > CmptAv
    (
        new GeometricField<scalar, PatchField, GeoMesh>
        (
            IOobject
            (
                "cmptAv(" + gf.name() + ')',
                gf.instance(),
                gf.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            gf.mesh(),
            gf.dimensions()
        )
    );

    cmptAv(CmptAv(), gf);

    tgf.clear();

    return CmptAv;
}


#define UNARY_REDUCTION_FUNCTION_WITH_BOUNDARY(returnType, func, gFunc)       \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
dimensioned<returnType> func                                                  \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf                       \
)                                                                             \
{                                                                             \
    return dimensioned<Type>                                                  \
    (                                                                         \
        #func "(" + gf.name() + ')',                                          \
        gf.dimensions(),                                                      \
        CML::func(gFunc(gf.internalField()), gFunc(gf.boundaryField()))      \
    );                                                                        \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
dimensioned<returnType> func                                                  \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1               \
)                                                                             \
{                                                                             \
    dimensioned<returnType> res = func(tgf1());                               \
    tgf1.clear();                                                             \
    return res;                                                               \
}

UNARY_REDUCTION_FUNCTION_WITH_BOUNDARY(Type, max, gMax)
UNARY_REDUCTION_FUNCTION_WITH_BOUNDARY(Type, min, gMin)

#undef UNARY_REDUCTION_FUNCTION_WITH_BOUNDARY


#define UNARY_REDUCTION_FUNCTION(returnType, func, gFunc)                     \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
dimensioned<returnType> func                                                  \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf                       \
)                                                                             \
{                                                                             \
    return dimensioned<Type>                                                  \
    (                                                                         \
        #func "(" + gf.name() + ')',                                          \
        gf.dimensions(),                                                      \
        gFunc(gf.internalField())                                             \
    );                                                                        \
}                                                                             \
                                                                              \
template<class Type, template<class> class PatchField, class GeoMesh>         \
dimensioned<returnType> func                                                  \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1               \
)                                                                             \
{                                                                             \
    dimensioned<returnType> res = func(tgf1());                               \
    tgf1.clear();                                                             \
    return res;                                                               \
}

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

#ifndef __INTEL_COMPILER
BINARY_OPERATOR(Type, Type, scalar, *, '*', multiply)
BINARY_OPERATOR(Type, scalar, Type, *, '*', multiply)
#endif
BINARY_OPERATOR(Type, Type, scalar, /, '|', divide)

BINARY_TYPE_OPERATOR_SF(Type, scalar, Type, *, '*', multiply)
BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, *, '*', multiply)

BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, /, '|', divide)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define PRODUCT_OPERATOR(product, op, opFunc)                                 \
                                                                              \
template                                                                      \
<class Type1, class Type2, template<class> class PatchField, class GeoMesh>   \
void opFunc                                                                   \
(                                                                             \
    GeometricField                                                            \
    <typename product<Type1, Type2>::type, PatchField, GeoMesh>& gf,          \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1,                    \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2                     \
)                                                                             \
{                                                                             \
    CML::opFunc(gf.internalField(), gf1.internalField(), gf2.internalField());\
    CML::opFunc(gf.boundaryField(), gf1.boundaryField(), gf2.boundaryField());\
}                                                                             \
                                                                              \
template                                                                      \
<class Type1, class Type2, template<class> class PatchField, class GeoMesh>   \
tmp                                                                           \
<                                                                             \
    GeometricField<typename product<Type1, Type2>::type, PatchField, GeoMesh> \
>                                                                             \
operator op                                                                   \
(                                                                             \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1,                    \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2                     \
)                                                                             \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes               \
    (                                                                         \
        new GeometricField<productType, PatchField, GeoMesh>                  \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + #op + gf2.name() + ')',                    \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            gf1.dimensions() op gf2.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    CML::opFunc(tRes(), gf1, gf2);                                           \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<class Type1, class Type2, template<class> class PatchField, class GeoMesh>   \
tmp                                                                           \
<                                                                             \
    GeometricField<typename product<Type1, Type2>::type, PatchField, GeoMesh> \
>                                                                             \
operator op                                                                   \
(                                                                             \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1,                    \
    const tmp<GeometricField<Type2, PatchField, GeoMesh> >& tgf2              \
)                                                                             \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
                                                                              \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2 = tgf2();           \
                                                                              \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes =             \
        reuseTmpGeometricField<productType, Type2, PatchField, GeoMesh>::New  \
        (                                                                     \
            tgf2,                                                             \
            '(' + gf1.name() + #op + gf2.name() + ')',                        \
            gf1.dimensions() op gf2.dimensions()                              \
        );                                                                    \
                                                                              \
    CML::opFunc(tRes(), gf1, gf2);                                           \
                                                                              \
    reuseTmpGeometricField<productType, Type2, PatchField, GeoMesh>           \
    ::clear(tgf2);                                                            \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<class Type1, class Type2, template<class> class PatchField, class GeoMesh>   \
tmp                                                                           \
<                                                                             \
    GeometricField<typename product<Type1, Type2>::type, PatchField, GeoMesh> \
>                                                                             \
operator op                                                                   \
(                                                                             \
    const tmp<GeometricField<Type1, PatchField, GeoMesh> >& tgf1,             \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2                     \
)                                                                             \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
                                                                              \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1 = tgf1();           \
                                                                              \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes =             \
        reuseTmpGeometricField<productType, Type1, PatchField, GeoMesh>::New  \
        (                                                                     \
            tgf1,                                                             \
            '(' + gf1.name() + #op + gf2.name() + ')',                        \
            gf1.dimensions() op gf2.dimensions()                              \
        );                                                                    \
                                                                              \
    CML::opFunc(tRes(), gf1, gf2);                                           \
                                                                              \
    reuseTmpGeometricField<productType, Type1, PatchField, GeoMesh>           \
    ::clear(tgf1);                                                            \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<class Type1, class Type2, template<class> class PatchField, class GeoMesh>   \
tmp                                                                           \
<                                                                             \
    GeometricField<typename product<Type1, Type2>::type, PatchField, GeoMesh> \
>                                                                             \
operator op                                                                   \
(                                                                             \
    const tmp<GeometricField<Type1, PatchField, GeoMesh> >& tgf1,             \
    const tmp<GeometricField<Type2, PatchField, GeoMesh> >& tgf2              \
)                                                                             \
{                                                                             \
    typedef typename product<Type1, Type2>::type productType;                 \
                                                                              \
    const GeometricField<Type1, PatchField, GeoMesh>& gf1 = tgf1();           \
    const GeometricField<Type2, PatchField, GeoMesh>& gf2 = tgf2();           \
                                                                              \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes =             \
        reuseTmpTmpGeometricField                                             \
        <productType, Type1, Type1, Type2, PatchField, GeoMesh>::New          \
        (                                                                     \
            tgf1,                                                             \
            tgf2,                                                             \
            '(' + gf1.name() + #op + gf2.name() + ')',                        \
            gf1.dimensions() op gf2.dimensions()                              \
        );                                                                    \
                                                                              \
    CML::opFunc(tRes(), gf1, gf2);                                           \
                                                                              \
    reuseTmpTmpGeometricField                                                 \
        <productType, Type1, Type1, Type2, PatchField, GeoMesh>               \
    ::clear(tgf1, tgf2);                                                      \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<class Form, class Type, template<class> class PatchField, class GeoMesh>     \
void opFunc                                                                   \
(                                                                             \
    GeometricField                                                            \
    <typename product<Type, Form>::type, PatchField, GeoMesh>& gf,            \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const dimensioned<Form>& dvs                                              \
)                                                                             \
{                                                                             \
    CML::opFunc(gf.internalField(), gf1.internalField(), dvs.value());       \
    CML::opFunc(gf.boundaryField(), gf1.boundaryField(), dvs.value());       \
}                                                                             \
                                                                              \
template                                                                      \
<class Form, class Type, template<class> class PatchField, class GeoMesh>     \
tmp<GeometricField<typename product<Type, Form>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const dimensioned<Form>& dvs                                              \
)                                                                             \
{                                                                             \
    typedef typename product<Type, Form>::type productType;                   \
                                                                              \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes               \
    (                                                                         \
        new GeometricField<productType, PatchField, GeoMesh>                  \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + gf1.name() + #op + dvs.name() + ')',                    \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            gf1.dimensions() op dvs.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    CML::opFunc(tRes(), gf1, dvs);                                            \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<                                                                             \
    class Form,                                                               \
    class Cmpt,                                                               \
    direction nCmpt,                                                          \
    class Type, template<class> class PatchField,                             \
    class GeoMesh                                                             \
>                                                                             \
tmp<GeometricField<typename product<Form, Type>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1,                     \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                    \
)                                                                             \
{                                                                             \
    return gf1 op dimensioned<Form>(static_cast<const Form&>(vs));            \
}                                                                             \
                                                                              \
                                                                              \
template                                                                      \
<class Form, class Type, template<class> class PatchField, class GeoMesh>     \
tmp<GeometricField<typename product<Type, Form>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1,              \
    const dimensioned<Form>& dvs                                              \
)                                                                             \
{                                                                             \
    typedef typename product<Type, Form>::type productType;                   \
                                                                              \
    const GeometricField<Type, PatchField, GeoMesh>& gf1 = tgf1();            \
                                                                              \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes =             \
        reuseTmpGeometricField<productType, Type, PatchField, GeoMesh>::New   \
        (                                                                     \
            tgf1,                                                             \
            '(' + gf1.name() + #op + dvs.name() + ')',                        \
            gf1.dimensions() op dvs.dimensions()                              \
        );                                                                    \
                                                                              \
    CML::opFunc(tRes(), gf1, dvs);                                            \
                                                                              \
    reuseTmpGeometricField<productType, Type, PatchField, GeoMesh>            \
    ::clear(tgf1);                                                            \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<                                                                             \
    class Form,                                                               \
    class Cmpt,                                                               \
    direction nCmpt,                                                          \
    class Type, template<class> class PatchField,                             \
    class GeoMesh                                                             \
>                                                                             \
tmp<GeometricField<typename product<Form, Type>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1,              \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                    \
)                                                                             \
{                                                                             \
    return tgf1 op dimensioned<Form>(static_cast<const Form&>(vs));           \
}                                                                             \
                                                                              \
                                                                              \
template                                                                      \
<class Form, class Type, template<class> class PatchField, class GeoMesh>     \
void opFunc                                                                   \
(                                                                             \
    GeometricField                                                            \
    <typename product<Form, Type>::type, PatchField, GeoMesh>& gf,            \
    const dimensioned<Form>& dvs,                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1                      \
)                                                                             \
{                                                                             \
    CML::opFunc(gf.internalField(), dvs.value(), gf1.internalField());       \
    CML::opFunc(gf.boundaryField(), dvs.value(), gf1.boundaryField());       \
}                                                                             \
                                                                              \
template                                                                      \
<class Form, class Type, template<class> class PatchField, class GeoMesh>     \
tmp<GeometricField<typename product<Form, Type>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const dimensioned<Form>& dvs,                                             \
    const GeometricField<Type, PatchField, GeoMesh>& gf1                      \
)                                                                             \
{                                                                             \
    typedef typename product<Form, Type>::type productType;                   \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes               \
    (                                                                         \
        new GeometricField<productType, PatchField, GeoMesh>                  \
        (                                                                     \
            IOobject                                                          \
            (                                                                 \
                '(' + dvs.name() + #op + gf1.name() + ')',                    \
                gf1.instance(),                                               \
                gf1.db(),                                                     \
                IOobject::NO_READ,                                            \
                IOobject::NO_WRITE                                            \
            ),                                                                \
            gf1.mesh(),                                                       \
            dvs.dimensions() op gf1.dimensions()                              \
        )                                                                     \
    );                                                                        \
                                                                              \
    CML::opFunc(tRes(), dvs, gf1);                                            \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<                                                                             \
    class Form,                                                               \
    class Cmpt,                                                               \
    direction nCmpt,                                                          \
    class Type, template<class> class PatchField,                             \
    class GeoMesh                                                             \
>                                                                             \
tmp<GeometricField<typename product<Form, Type>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const VectorSpace<Form,Cmpt,nCmpt>& vs,                                   \
    const GeometricField<Type, PatchField, GeoMesh>& gf1                      \
)                                                                             \
{                                                                             \
    return dimensioned<Form>(static_cast<const Form&>(vs)) op gf1;            \
}                                                                             \
                                                                              \
template                                                                      \
<class Form, class Type, template<class> class PatchField, class GeoMesh>     \
tmp<GeometricField<typename product<Form, Type>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const dimensioned<Form>& dvs,                                             \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1               \
)                                                                             \
{                                                                             \
    typedef typename product<Form, Type>::type productType;                   \
                                                                              \
    const GeometricField<Type, PatchField, GeoMesh>& gf1 = tgf1();            \
                                                                              \
    tmp<GeometricField<productType, PatchField, GeoMesh> > tRes =             \
        reuseTmpGeometricField<productType, Type, PatchField, GeoMesh>::New   \
        (                                                                     \
            tgf1,                                                             \
            '(' + dvs.name() + #op + gf1.name() + ')',                        \
            dvs.dimensions() op gf1.dimensions()                              \
        );                                                                    \
                                                                              \
    CML::opFunc(tRes(), dvs, gf1);                                            \
                                                                              \
    reuseTmpGeometricField<productType, Type, PatchField, GeoMesh>            \
    ::clear(tgf1);                                                            \
                                                                              \
    return tRes;                                                              \
}                                                                             \
                                                                              \
template                                                                      \
<                                                                             \
    class Form,                                                               \
    class Cmpt,                                                               \
    direction nCmpt,                                                          \
    class Type, template<class> class PatchField,                             \
    class GeoMesh                                                             \
>                                                                             \
tmp<GeometricField<typename product<Form, Type>::type, PatchField, GeoMesh> > \
operator op                                                                   \
(                                                                             \
    const VectorSpace<Form,Cmpt,nCmpt>& vs,                                   \
    const tmp<GeometricField<Type, PatchField, GeoMesh> >& tgf1               \
)                                                                             \
{                                                                             \
    return dimensioned<Form>(static_cast<const Form&>(vs)) op tgf1;           \
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


#include "GeometricFieldFunctions.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
