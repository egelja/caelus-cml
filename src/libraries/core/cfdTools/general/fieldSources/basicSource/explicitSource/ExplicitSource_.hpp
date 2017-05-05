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
    CML::ExplicitSource

Description
    Explicit source

    Sources described by:

        <Type>ExplicitSourceCoeffs
        {
            volumeMode      absolute; // specific
            injectionRate
            {
                k           30.7;
                epsilon     1.5;
            }
        }

    If volumeMode = 
        - absolute: values are given as <quantity>
        - specific: values are given as <quantity>/m3


SourceFiles
    ExplicitSource.cpp

\*---------------------------------------------------------------------------*/

#ifndef ExplicitSource__H
#define ExplicitSource__H

#include "Tuple2.hpp"
#include "basicSource.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

class fvMesh;

template<class Type>
class ExplicitSource;

// Forward declaration of friend functions

template<class Type>
Ostream& operator<<
(
    Ostream&,
    const ExplicitSource<Type>&
);

/*---------------------------------------------------------------------------*\
                      Class ExplicitSource Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class ExplicitSource
:
    public basicSource
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
        List<Type> injectionRate_;


    // Protected functions

        //- Helper function to convert from a word to a volumeModeType
        volumeModeType wordToVolumeModeType(const word& vtName) const;

        //- Helper function to convert from a volumeModeType to a word
        word volumeModeTypeToWord(const volumeModeType& vtType) const;

        //- Set the local field data
        void setFieldData(const dictionary& dict);


public:

    //- Runtime type information
    TypeName("ExplicitSource");


    // Constructors

        //- Construct from components
        ExplicitSource
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
            inline const List<Type>& injectionRate() const;


        // Edit

            //- Return access to the volume mode
            inline volumeModeType& volumeMode();

            //- Return access to the source field values
            inline List<Type>& injectionRate();


        // Evaluation

            //- Add explicit contribution to equation
            virtual void addSup(fvMatrix<Type>& eqn, const label fieldI);


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

// * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * * //

template<class Type>
const CML::wordList CML::ExplicitSource<Type>::
volumeModeTypeNames_
(
    IStringStream("(absolute specific)")()
);


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class Type>
typename CML::ExplicitSource<Type>::volumeModeType
CML::ExplicitSource<Type>::wordToVolumeModeType
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

    FatalErrorIn
    (
        "ExplicitSource<Type>::volumeModeType"
        "ExplicitSource<Type>::wordToVolumeModeType(const word&)"
    )   << "Unknown volumeMode type " << vmtName
        << ". Valid volumeMode types are:" << nl << volumeModeTypeNames_
        << exit(FatalError);

    return volumeModeType(0);
}


template<class Type>
CML::word CML::ExplicitSource<Type>::volumeModeTypeToWord
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
void CML::ExplicitSource<Type>::setFieldData(const dictionary& dict)
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
CML::ExplicitSource<Type>::ExplicitSource
(
    const word& name,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    basicSource(name, modelType, dict, mesh),
    volumeMode_(vmAbsolute),
    VDash_(1.0),
    injectionRate_()
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::ExplicitSource<Type>::addSup
(
    fvMatrix<Type>& eqn,
    const label fieldI
)
{
    if (debug)
    {
        Info<< "ExplicitSource<"<< pTraits<Type>::typeName
            << ">::addSup for source " << name_ << endl;
    }

    DimensionedField<Type, volMesh> Su
    (
        IOobject
        (
            name_ + fieldNames_[fieldI] + "Sup",
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

    UIndirectList<Type>(Su, cells_) = injectionRate_[fieldI]/VDash_;

    eqn += Su;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::ExplicitSource<Type>::writeData(Ostream& os) const
{
    os  << indent << name_ << endl;
    dict_.write(os);
}


template<class Type>
bool CML::ExplicitSource<Type>::read(const dictionary& dict)
{
    if (basicSource::read(dict))
    {
        volumeMode_ = wordToVolumeModeType(coeffs_.lookup("volumeMode"));
        setFieldData(coeffs_.subDict("injectionRate"));

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
inline const typename CML::ExplicitSource<Type>::volumeModeType&
CML::ExplicitSource<Type>::volumeMode() const
{
    return volumeMode_;
}


template<class Type>
inline const CML::List<Type>& CML::ExplicitSource<Type>::injectionRate() const
{
    return injectionRate_;
}


template<class Type>
inline typename CML::ExplicitSource<Type>::volumeModeType&
CML::ExplicitSource<Type>::volumeMode()
{
    return volumeMode_;
}


template<class Type>
inline CML::List<Type>& CML::ExplicitSource<Type>::injectionRate()
{
    return injectionRate_;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
