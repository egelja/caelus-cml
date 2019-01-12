/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::fv::SemiImplicitSource

Description
    Semi-implicit source, described using an input dictionary.  The injection
    rate coefficients are specified as pairs of Su-Sp coefficients, i.e.

        \f[
            S(x) = S_u + S_p x
        \f]

    where
    \vartable
        S(x)    | net source for field 'x'
        S_u     | explicit source contribution
        S_p     | linearised implicit contribution
    \endvartable

    Example of the source specification:

    \verbatim
    <Type>SemiImplicitSourceCoeffs
    {
        volumeMode      absolute; // specific
        injectionRateSuSp
        {
            k           (30.7 0);
            epsilon     (1.5  0);
        }
    }
    \endverbatim

    Valid options for the \c volumeMode entry include:
    - absolute: values are given as \<quantity\>
    - specific: values are given as \<quantity\>/m3

SeeAlso
    CML::fvOption

SourceFiles
    SemiImplicitSource.cpp

\*---------------------------------------------------------------------------*/

#ifndef SemiImplicitSource_H
#define SemiImplicitSource_H

#include "Tuple2.hpp"
#include "fvOption.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fv
{

// Forward declaration of classes

template<class Type>
class SemiImplicitSource;


// Forward declaration of friend functions

template<class Type>
Ostream& operator<<
(
    Ostream&,
    const SemiImplicitSource<Type>&
);


/*---------------------------------------------------------------------------*\
                     Class SemiImplicitSource Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class SemiImplicitSource
:
    public option
{
public:

    // Public data

        //- Enumeration for volume types
        enum volumeModeType
        {
            vmAbsolute,
            vmSpecific
        };

        //- Word list of volume mode type names
        static const wordList volumeModeTypeNames_;


protected:

    // Protected data

        //- Volume mode
        volumeModeType volumeMode_;

        //- Volume normalisation
        scalar VDash_;

        //- Source field values
        List<Tuple2<Type, scalar> > injectionRate_;


    // Protected functions

        //- Helper function to convert from a word to a volumeModeType
        volumeModeType wordToVolumeModeType(const word& vtName) const;

        //- Helper function to convert from a volumeModeType to a word
        word volumeModeTypeToWord(const volumeModeType& vtType) const;

        //- Set the local field data
        void setFieldData(const dictionary& dict);


public:

    //- Runtime type information
    TypeName("SemiImplicitSource");


    // Constructors

        //- Construct from components
        SemiImplicitSource
        (
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );


    // Member Functions

        // Access

            //- Return const access to the volume mode
            inline const volumeModeType& volumeMode() const;

            //- Return const access to the source field values
            inline const List<Tuple2<Type, scalar> >& injectionRate() const;


        // Edit

            //- Return access to the volume mode
            inline volumeModeType& volumeMode();

            //- Return access to the source field values
            inline List<Tuple2<Type, scalar> >& injectionRate();


        // Evaluation

            //- Add explicit contribution to equation
            virtual void addSup
            (
                fvMatrix<Type>& eqn,
                const label fieldI
            );

            //- Add explicit contribution to compressible equation
            virtual void addSup
            (
                const volScalarField& rho,
                fvMatrix<Type>& eqn,
                const label fieldI
            );


        // I-O

            //- Write the source properties
            virtual void writeData(Ostream&) const;

            //- Read source dictionary
            virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvMesh.hpp"
#include "fvMatrices.hpp"
#include "DimensionedField.hpp"
#include "fvmSup.hpp"

// * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * * //

template<class Type>
const CML::wordList CML::fv::SemiImplicitSource<Type>::volumeModeTypeNames_
(
    IStringStream("(absolute specific)")()
);


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class Type>
typename CML::fv::SemiImplicitSource<Type>::volumeModeType
CML::fv::SemiImplicitSource<Type>::wordToVolumeModeType
(
    const word& vmtName
) const
{
    forAll(volumeModeTypeNames_, i)
    {
        if (vmtName == volumeModeTypeNames_[i])
        {
            return volumeModeType(i);
        }
    }

    FatalErrorInFunction
        << "Unknown volumeMode type " << vmtName
        << ". Valid volumeMode types are:" << nl << volumeModeTypeNames_
        << exit(FatalError);

    return volumeModeType(0);
}


template<class Type>
CML::word CML::fv::SemiImplicitSource<Type>::volumeModeTypeToWord
(
    const volumeModeType& vmtType
) const
{
    if (vmtType > volumeModeTypeNames_.size())
    {
        return "UNKNOWN";
    }
    else
    {
        return volumeModeTypeNames_[vmtType];
    }
}


template<class Type>
void CML::fv::SemiImplicitSource<Type>::setFieldData(const dictionary& dict)
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

    // Set volume normalisation
    if (volumeMode_ == vmAbsolute)
    {
        VDash_ = V_;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::fv::SemiImplicitSource<Type>::SemiImplicitSource
(
    const word& name,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    option(name, modelType, dict, mesh),
    volumeMode_(vmAbsolute),
    VDash_(1.0),
    injectionRate_()
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::fv::SemiImplicitSource<Type>::addSup
(
    fvMatrix<Type>& eqn,
    const label fieldI
)
{
    if (debug)
    {
        Info<< "SemiImplicitSource<" << pTraits<Type>::typeName
            << ">::addSup for source " << name_ << endl;
    }

    const GeometricField<Type, fvPatchField, volMesh>& psi = eqn.psi();

    DimensionedField<Type, volMesh> Su
    (
        IOobject
        (
            name_ + fieldNames_[fieldI] + "Su",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh_,
        dimensioned<Type>
        (
            "zero",
            eqn.dimensions()/dimVolume,
            pTraits<Type>::zero
        ),
        false
    );

    UIndirectList<Type>(Su, cells_) = injectionRate_[fieldI].first()/VDash_;

    DimensionedField<scalar, volMesh> Sp
    (
        IOobject
        (
            name_ + fieldNames_[fieldI] + "Sp",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh_,
        dimensioned<scalar>
        (
            "zero",
            Su.dimensions()/psi.dimensions(),
            0.0
        ),
        false
    );

    UIndirectList<scalar>(Sp, cells_) = injectionRate_[fieldI].second()/VDash_;

    eqn += Su + fvm::SuSp(Sp, psi);
}


template<class Type>
void CML::fv::SemiImplicitSource<Type>::addSup
(
    const volScalarField& rho,
    fvMatrix<Type>& eqn,
    const label fieldI
)
{
    if (debug)
    {
        Info<< "SemiImplicitSource<" << pTraits<Type>::typeName
            << ">::addSup for source " << name_ << endl;
    }

    return this->addSup(eqn, fieldI);
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::fv::SemiImplicitSource<Type>::writeData(Ostream& os) const
{
    os  << indent << name_ << endl;
    dict_.write(os);
}


template<class Type>
bool CML::fv::SemiImplicitSource<Type>::read(const dictionary& dict)
{
    if (option::read(dict))
    {
        volumeMode_ = wordToVolumeModeType(coeffs_.lookup("volumeMode"));
        setFieldData(coeffs_.subDict("injectionRateSuSp"));

        return true;
    }
    else
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
inline const typename CML::fv::SemiImplicitSource<Type>::volumeModeType&
CML::fv::SemiImplicitSource<Type>::volumeMode() const
{
    return volumeMode_;
}


template<class Type>
inline const CML::List<CML::Tuple2<Type, CML::scalar> >&
CML::fv::SemiImplicitSource<Type>::injectionRate() const
{
    return injectionRate_;
}


template<class Type>
inline typename CML::fv::SemiImplicitSource<Type>::volumeModeType&
CML::fv::SemiImplicitSource<Type>::volumeMode()
{
    return volumeMode_;
}


template<class Type>
inline CML::List<CML::Tuple2<Type,
CML::scalar> >& CML::fv::SemiImplicitSource<Type>::injectionRate()
{
    return injectionRate_;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
