/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2016 OpenCFD Ltd
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
#include "profiling.hpp"

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

        //- Return the "options" sub-dictionary if present otherwise return dict
        const dictionary& optionsDict(const dictionary& dict) const;

        //- Read options dictionary
        bool readOptions(const dictionary& dict);

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
            template<class Type>
            tmp<fvMatrix<Type> > operator()
            (
                const volScalarField& rho,
                GeometricField<Type, fvPatchField, volMesh>& fld
            );

            //- Return source for equation with specified name
            template<class Type>
            tmp<fvMatrix<Type> > operator()
            (
                const volScalarField& rho,
                GeometricField<Type, fvPatchField, volMesh>& fld,
                const word& fieldName
            );

            //- Return source for equation
            template<class Type>
            tmp<fvMatrix<Type> > operator()
            (
                const volScalarField& alpha,
                const volScalarField& rho,
                GeometricField<Type, fvPatchField, volMesh>& fld
            );

            //- Return source for equation with specified name
            template<class Type>
            tmp<fvMatrix<Type> > operator()
            (
                const volScalarField& alpha,
                const volScalarField& rho,
                GeometricField<Type, fvPatchField, volMesh>& fld,
                const word& fieldName
            );


        // Constraints

            //- Apply constraints to equation
            template<class Type>
            void constrain(fvMatrix<Type>& eqn);

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
            addProfiling(fvopt, "fvOption::correct." + source.name());

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
            addProfiling(fvopt, "fvOption()." + source.name());

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
CML::tmp<CML::fvMatrix<Type> > CML::fv::optionList::operator()
(
    const volScalarField& rho,
    GeometricField<Type, fvPatchField, volMesh>& fld
)
{
    return this->operator()(rho, fld, fld.name());
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::fv::optionList::operator()
(
    const volScalarField& rho,
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
            addProfiling(fvopt, "fvOption()." + source.name());

            source.setApplied(fieldI);

            if (source.isActive())
            {
                if (debug)
                {
                    Info<< "Applying source " << source.name() << " to field "
                        << fieldName << endl;
                }

                source.addSup(rho, mtx, fieldI);
            }
        }
    }

    return tmtx;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::fv::optionList::operator()
(
    const volScalarField& alpha,
    const volScalarField& rho,
    GeometricField<Type, fvPatchField, volMesh>& fld
)
{
    return this->operator()(alpha, rho, fld, fld.name());
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> > CML::fv::optionList::operator()
(
    const volScalarField& alpha,
    const volScalarField& rho,
    GeometricField<Type, fvPatchField, volMesh>& fld,
    const word& fieldName
)
{
    checkApplied();

    const dimensionSet ds =
        alpha.dimensions()*rho.dimensions()*fld.dimensions()/dimTime*dimVolume;

    tmp<fvMatrix<Type> > tmtx(new fvMatrix<Type>(fld, ds));
    fvMatrix<Type>& mtx = tmtx();

    forAll(*this, i)
    {
        option& source = this->operator[](i);

        label fieldI = source.applyToField(fieldName);

        if (fieldI != -1)
        {
            addProfiling(fvopt, "fvOption()." + source.name());

            source.setApplied(fieldI);

            if (source.isActive())
            {
                if (debug)
                {
                    Info<< "Applying source " << source.name() << " to field "
                        << fieldName << endl;
                }

                source.addSup(alpha, rho, mtx, fieldI);
            }
        }
    }

    return tmtx;
}


template<class Type>
void CML::fv::optionList::constrain(fvMatrix<Type>& eqn)
{
    checkApplied();

    forAll(*this, i)
    {
        option& source = this->operator[](i);

        label fieldI = source.applyToField(eqn.psi().name());

        if (fieldI != -1)
        {
            addProfiling(fvopt, "fvOption::constrain." + eqn.psi().name());

            source.setApplied(fieldI);

            if (source.isActive())
            {
                if (debug)
                {
                    Info<< "Applying constraint " << source.name()
                        << " to field " << eqn.psi().name() << endl;
                }

                source.setValue(eqn, fieldI);
            }
        }
    }
}


#endif


