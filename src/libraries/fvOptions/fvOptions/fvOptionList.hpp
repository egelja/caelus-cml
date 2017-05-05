/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::fv::optionList

Description
    List of finite volume options

SourceFile
    optionList.cpp

\*---------------------------------------------------------------------------*/

#ifndef optionList_HPP
#define optionList_HPP

#include "PtrList.hpp"
#include "GeometricField.hpp"
#include "fvPatchField.hpp"
#include "fvOption.hpp"

namespace CML
{
namespace fv
{

class optionList
:
    public PtrList<option>
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
        optionList(const optionList&);

        //- Disallow default bitwise assignment
        void operator=(const optionList&);


public:

    //- Runtime type information
    TypeName("optionList");


    // Constructors

        //- Construct null
        optionList(const fvMesh& mesh);

        //- Construct from mesh and dictionary
        optionList(const fvMesh& mesh, const dictionary& dict);


    //- Destructor
    virtual ~optionList()
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


        // Flux manipulations

            //- Make the given absolute flux relative
            void relativeFlux(surfaceScalarField& phi) const;

            //- Make the given absolute mass-flux relative
            void relativeFlux
            (
                const surfaceScalarField& rho,
                surfaceScalarField& phi
            ) const;

            //- Make the given relative flux absolute
            void absoluteFlux(surfaceScalarField& phi) const;

            //- Make the given relative mass-flux absolute
            void absoluteFlux
            (
                const surfaceScalarField& rho,
                surfaceScalarField& phi
            ) const;


        // I-O

            //- Read dictionary
            virtual bool read(const dictionary& dict);

            //- Write data to Ostream
            virtual bool writeData(Ostream& os) const;

            //- Ostream operator
            friend Ostream& operator<<
            (
                Ostream& os,
                const optionList& options
            );
};

} // End namespace fv
} // End namespace CML


template<class Type>
void CML::fv::optionList::correct
(
    GeometricField<Type, fvPatchField, volMesh>& fld
)
{
    const word& fieldName = fld.name();

    forAll(*this, i)
    {
        option& source = this->operator[](i);

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
CML::tmp<CML::fvMatrix<Type> > CML::fv::optionList::operator()
(
    GeometricField<Type, fvPatchField, volMesh>& fld
)
{
    return this->operator()(fld, fld.name());
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::fv::optionList::operator()
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
        option& source = this->operator[](i);

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
CML::tmp<CML::fvMatrix<Type> > CML::fv::optionList::operator()
(
    const RhoType& rho,
    GeometricField<Type, fvPatchField, volMesh>& fld
)
{
    return this->operator()(rho, fld, fld.name());
}


template<class Type, class RhoType>
CML::tmp<CML::fvMatrix<Type> > CML::fv::optionList::operator()
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
        option& source = this->operator[](i);

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
void CML::fv::optionList::constrain(fvMatrix<Type>& eqn)
{
    constrain(eqn, eqn.psi().name());
}


template<class Type>
void CML::fv::optionList::constrain
(
    fvMatrix<Type>& eqn,
    const word& fieldName
)
{
    checkApplied();

    forAll(*this, i)
    {
        option& source = this->operator[](i);

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


#endif


