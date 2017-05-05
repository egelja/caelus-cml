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
    CML::basicSourceList

Description
    List of explict sources

SourceFile
    basicSourceList.cpp

\*---------------------------------------------------------------------------*/

#ifndef basicSourceList_H
#define basicSourceList_H

#include "PtrList.hpp"
#include "GeometricField.hpp"
#include "fvPatchField.hpp"
#include "basicSource.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class basicSourceList Declaration
\*---------------------------------------------------------------------------*/

class basicSourceList
:
    public PtrList<basicSource>
{
protected:

    // Protected data

        //- Reference to the mesh database
        const fvMesh& mesh_;

        //- Time index to check that all defined sources have been applied
        label checkTimeIndex_;


    // Protected Member Functions

        //- Check that all sources have been applied
        void checkApplied() const;

        //- Disallow default bitwise copy construct
        basicSourceList(const basicSourceList&);

        //- Disallow default bitwise assignment
        void operator=(const basicSourceList&);


public:

    //- Runtime type information
    TypeName("basicSourceList");


    // Constructors

        //- Construct null
        basicSourceList(const fvMesh& mesh);

        //- Construct from components with list of field names
        basicSourceList(const fvMesh& mesh, const dictionary& dict);


    //- Destructor
    virtual ~basicSourceList()
    {}


    // Member Functions

        //- Reset the source list
        void reset(const dictionary& dict);

        //- Correct
        template<class Type>
        void correct(GeometricField<Type, fvPatchField, volMesh>& fld);


        // Sources

            //- Return source for equation
            template<class Type>
            tmp<fvMatrix<Type> > operator()
            (
                GeometricField<Type, fvPatchField, volMesh>& fld
            );

            //- Return source for equation with specified name
            template<class Type>
            tmp<fvMatrix<Type> > operator()
            (
                GeometricField<Type, fvPatchField, volMesh>& fld,
                const word& fieldName
            );

            //- Return source for equation
            template<class Type, class RhoType>
            tmp<fvMatrix<Type> > operator()
            (
                const RhoType& rho,
                GeometricField<Type, fvPatchField, volMesh>& fld
            );

            //- Return source for equation with specified name
            template<class Type, class RhoType>
            tmp<fvMatrix<Type> > operator()
            (
                const RhoType& rho,
                GeometricField<Type, fvPatchField, volMesh>& fld,
                const word& fieldName
            );


        // Constraints

            //- Apply constraints to equation
            template<class Type>
            void constrain(fvMatrix<Type>& eqn);

            //- Apply constraints to equation with specified name
            template<class Type>
            void constrain(fvMatrix<Type>& eqn, const word& fieldName);


        // I-O

            //- Read dictionary
            virtual bool read(const dictionary& dict);

            //- Write data to Istream
            virtual bool writeData(Ostream& os) const;

            //- Ostream operator
            friend Ostream& operator<<
            (
                Ostream& os,
                const basicSourceList& sources
            );
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::basicSourceList::correct
(
    GeometricField<Type, fvPatchField, volMesh>& fld
)
{
    const word& fieldName = fld.name();

    forAll(*this, i)
    {
        basicSource& source = this->operator[](i);

        label fieldI = source.applyToField(fieldName);

        if (fieldI != -1)
        {
            source.setApplied(fieldI);

            if (source.isActive())
            {
                if (debug)
                {
                    Info<< "Correcting source " << source.name()
                        << " for field " << fieldName << endl;
                }

                source.correct(fld);
            }
        }
    }
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::basicSourceList::operator()
(
    GeometricField<Type, fvPatchField, volMesh>& fld
)
{
    return this->operator()(fld, fld.name());
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::basicSourceList::operator()
(
    GeometricField<Type, fvPatchField, volMesh>& fld,
    const word& fieldName
)
{
    checkApplied();

    const dimensionSet ds = fld.dimensions()/dimTime*dimVolume;

    tmp<fvMatrix<Type> > tmtx(new fvMatrix<Type>(fld, ds));

    fvMatrix<Type>& mtx = tmtx();


    forAll(*this, i)
    {
        basicSource& source = this->operator[](i);

        label fieldI = source.applyToField(fieldName);

        if (fieldI != -1)
        {
            source.setApplied(fieldI);

            if (source.isActive())
            {
                if (debug)
                {
                    Info<< "Applying source " << source.name() << " to field "
                        << fieldName << endl;
                }

                source.addSup(mtx, fieldI);
            }
        }
    }

    return tmtx;
}


template<class Type, class RhoType>
CML::tmp<CML::fvMatrix<Type> > CML::basicSourceList::operator()
(
    const RhoType& rho,
    GeometricField<Type, fvPatchField, volMesh>& fld
)
{
    return this->operator()(rho, fld, fld.name());
}


template<class Type, class RhoType>
CML::tmp<CML::fvMatrix<Type> > CML::basicSourceList::operator()
(
    const RhoType& rho,
    GeometricField<Type, fvPatchField, volMesh>& fld,
    const word& fieldName
)
{
    checkApplied();

    const dimensionSet ds = rho.dimensions()*fld.dimensions()/dimTime*dimVolume;

    tmp<fvMatrix<Type> > tmtx(new fvMatrix<Type>(fld, ds));

    fvMatrix<Type>& mtx = tmtx();


    forAll(*this, i)
    {
        basicSource& source = this->operator[](i);

        label fieldI = source.applyToField(fieldName);

        if (fieldI != -1)
        {
            source.setApplied(fieldI);

            if (source.isActive())
            {
                if (debug)
                {
                    Info<< "Applying source " << source.name() << " to field "
                        << fieldName << endl;
                }

                source.addSup(mtx, fieldI);
            }
        }
    }

    return tmtx;
}


template<class Type>
void CML::basicSourceList::constrain(fvMatrix<Type>& eqn)
{
    constrain(eqn, eqn.psi().name());
}


template<class Type>
void CML::basicSourceList::constrain
(
    fvMatrix<Type>& eqn,
    const word& fieldName
)
{
    checkApplied();

    forAll(*this, i)
    {
        basicSource& source = this->operator[](i);

        label fieldI = source.applyToField(fieldName);

        if (fieldI != -1)
        {
            source.setApplied(fieldI);

            if (source.isActive())
            {
                if (debug)
                {
                    Info<< "Applying constraint " << source.name()
                        << " to field " << fieldName << endl;
                }

                source.setValue(eqn, fieldI);
            }
        }
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
