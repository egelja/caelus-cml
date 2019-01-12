/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::fv::gradScheme

Description
    Abstract base class for gradient schemes.


\*---------------------------------------------------------------------------*/

#ifndef gradScheme_H
#define gradScheme_H

#include "tmp.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

/*---------------------------------------------------------------------------*\
                           Class gradScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class gradScheme
:
    public refCount
{
    // Private data

        const fvMesh& mesh_;


    // Private Member Functions

        //- Disallow copy construct
        gradScheme(const gradScheme&);

        //- Disallow default bitwise assignment
        void operator=(const gradScheme&);


public:

    //- Runtime type information
    virtual const word& type() const = 0;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            gradScheme,
            Istream,
            (const fvMesh& mesh, Istream& schemeData),
            (mesh, schemeData)
        );


    // Constructors

        //- Construct from mesh
        gradScheme(const fvMesh& mesh)
        :
            mesh_(mesh)
        {}


    // Selectors

        //- Return a pointer to a new gradScheme created on freestore
        static tmp<gradScheme<Type> > New
        (
            const fvMesh& mesh,
            Istream& schemeData
        );


    //- Destructor
    virtual ~gradScheme();


    // Member Functions

        //- Return mesh reference
        const fvMesh& mesh() const
        {
            return mesh_;
        }

        //- Calculate and return the grad of the given field.
        //  Used by grad either to recalculate the cached gradient when it is
        //  out of date with respect to the field or when it is not cached.
        virtual tmp
        <
            GeometricField
            <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
        > calcGrad
        (
            const GeometricField<Type, fvPatchField, volMesh>&,
            const word& name
        ) const = 0;

        //- Calculate and return the grad of the given field
        //  which may have been cached
        tmp
        <
            GeometricField
            <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
        > grad
        (
            const GeometricField<Type, fvPatchField, volMesh>&,
            const word& name
        ) const;

        //- Calculate and return the grad of the given field
        //  with the default name
        //  which may have been cached
        tmp
        <
            GeometricField
            <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
        > grad
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const;

        //- Calculate and return the grad of the given field
        //  with the default name
        //  which may have been cached
        tmp
        <
            GeometricField
            <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
        > grad
        (
            const tmp<GeometricField<Type, fvPatchField, volMesh> >&
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makeFvGradTypeScheme(SS, Type)                                         \
    defineNamedTemplateTypeNameAndDebug(CML::fv::SS<CML::Type>, 0);            \
                                                                               \
    namespace CML                                                              \
    {                                                                          \
        namespace fv                                                           \
        {                                                                      \
            gradScheme<Type>::addIstreamConstructorToTable<SS<Type> >          \
                add##SS##Type##IstreamConstructorToTable_;                     \
        }                                                                      \
    }


#define makeFvGradScheme(SS)                                                   \
                                                                               \
makeFvGradTypeScheme(SS, scalar)                                               \
makeFvGradTypeScheme(SS, vector)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fv.hpp"
#include "objectRegistry.hpp"
#include "solution.hpp"

// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::fv::gradScheme<Type> > CML::fv::gradScheme<Type>::New
(
    const fvMesh& mesh,
    Istream& schemeData
)
{
    if (fv::debug)
    {
        Info<< "gradScheme<Type>::New"
               "(const fvMesh& mesh, Istream& schemeData) : "
               "constructing gradScheme<Type>"
            << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorInFunction(schemeData)
            << "Grad scheme not specified" << endl << endl
            << "Valid grad schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word schemeName(schemeData);

    typename IstreamConstructorTable::iterator cstrIter =
        IstreamConstructorTablePtr_->find(schemeName);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(schemeData)
            << "Unknown grad scheme " << schemeName << nl << nl
            << "Valid grad schemes are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return cstrIter()(mesh, schemeData);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::fv::gradScheme<Type>::~gradScheme()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    template<class Type>
    inline void cachePrintMessage
    (
        const char* message,
        const word& name,
        const GeometricField<Type, fvPatchField, volMesh>& vf
    )
    {
        if (solution::debug)
        {
            Info<< "Cache: " << message << token::SPACE << name
                << ", " << vf.name() << " event No. " << vf.eventNo()
                << endl;
        }
    }
}

template<class Type>
CML::tmp
<
    CML::GeometricField
    <
        typename CML::outerProduct<CML::vector, Type>::type,
        CML::fvPatchField,
        CML::volMesh
    >
>
CML::fv::gradScheme<Type>::grad
(
    const GeometricField<Type, fvPatchField, volMesh>& vsf,
    const word& name
) const
{
    typedef typename outerProduct<vector, Type>::type GradType;
    typedef GeometricField<GradType, fvPatchField, volMesh> GradFieldType;

    if (!this->mesh().changing() && this->mesh().cache(name))
    {
        if (!mesh().objectRegistry::template foundObject<GradFieldType>(name))
        {
            cachePrintMessage("Calculating and caching", name, vsf);
            tmp<GradFieldType> tgGrad = calcGrad(vsf, name);
            regIOobject::store(tgGrad.ptr());
        }

        cachePrintMessage("Retrieving", name, vsf);
        GradFieldType& gGrad = const_cast<GradFieldType&>
        (
            mesh().objectRegistry::template lookupObject<GradFieldType>(name)
        );

        if (gGrad.upToDate(vsf))
        {
            return gGrad;
        }
        else
        {
            cachePrintMessage("Deleting", name, vsf);
            gGrad.release();
            delete &gGrad;

            cachePrintMessage("Recalculating", name, vsf);
            tmp<GradFieldType> tgGrad = calcGrad(vsf, name);

            cachePrintMessage("Storing", name, vsf);
            regIOobject::store(tgGrad.ptr());
            GradFieldType& gGrad = const_cast<GradFieldType&>
            (
                mesh().objectRegistry::template lookupObject<GradFieldType>
                (
                    name
                )
            );

            return gGrad;
        }
    }
    else
    {
        if (mesh().objectRegistry::template foundObject<GradFieldType>(name))
        {
            GradFieldType& gGrad = const_cast<GradFieldType&>
            (
                mesh().objectRegistry::template lookupObject<GradFieldType>
                (
                    name
                )
            );

            if (gGrad.ownedByRegistry())
            {
                cachePrintMessage("Deleting", name, vsf);
                gGrad.release();
                delete &gGrad;
            }
        }

        cachePrintMessage("Calculating", name, vsf);
        return calcGrad(vsf, name);
    }
}


template<class Type>
CML::tmp
<
    CML::GeometricField
    <
        typename CML::outerProduct<CML::vector, Type>::type,
        CML::fvPatchField,
        CML::volMesh
    >
>
CML::fv::gradScheme<Type>::grad
(
    const GeometricField<Type, fvPatchField, volMesh>& vsf
) const
{
    return grad(vsf, "grad(" + vsf.name() + ')');
}


template<class Type>
CML::tmp
<
    CML::GeometricField
    <
        typename CML::outerProduct<CML::vector, Type>::type,
        CML::fvPatchField,
        CML::volMesh
    >
>
CML::fv::gradScheme<Type>::grad
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvsf
) const
{
    typedef typename outerProduct<vector, Type>::type GradType;
    typedef GeometricField<GradType, fvPatchField, volMesh> GradFieldType;

    tmp<GradFieldType> tgrad = grad(tvsf());
    tvsf.clear();
    return tgrad;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
