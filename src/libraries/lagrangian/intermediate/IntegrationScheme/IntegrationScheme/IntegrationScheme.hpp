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
    CML::IntegrationScheme

Description
    Top level model for Integration schemes


\*---------------------------------------------------------------------------*/

#ifndef IntegrationScheme_H
#define IntegrationScheme_H

#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "dictionary.hpp"
#include "error.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class IntegrationScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class IntegrationScheme
{

public:

    //- Helper class to supply results of integration
    class integrationResult
    {
        //- Integration value
        Type value_;

        //- Average value across integration step
        Type average_;


    public:

        //- Constructor
        integrationResult()
        :
            value_(pTraits<Type>::zero),
            average_(pTraits<Type>::zero)
        {}


        // Member functions

            // Access

                //- Return const access to the value
                Type value() const
                {
                    return value_;
                }

                //- Return const access to the average
                Type average() const
                {
                    return average_;
                }


            // Edit

                //- Return access to the value for changing
                Type& value()
                {
                    return value_;
                }

                //- Return access to the average for changing
                Type& average()
                {
                    return average_;
                }
        };


private:

    // Private data

        //- Name of the Integration variable
        const word& phiName_;

        //- Reference to the dictionary
        const dictionary& dict_;


    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const IntegrationScheme&);


public:

    //- Runtime type information
    TypeName("integrationScheme");


    //- Declare runtime constructor selection table

         declareRunTimeSelectionTable
         (
             autoPtr,
             IntegrationScheme,
             dictionary,
             (
                 const word& phiName,
                 const dictionary& dict
             ),
             (phiName, dict)
         );


    // Constructors

        //- Construct from components
        IntegrationScheme(const word& phiName, const dictionary& dict);

        //- Copy constructor
        IntegrationScheme(const IntegrationScheme& is);

        //- Construct and return clone
        virtual autoPtr<IntegrationScheme<Type> > clone() const
        {
            return autoPtr<IntegrationScheme<Type> >
            (
                new IntegrationScheme<Type>(*this)
            );
        }


    // Selectors

        //- Return a reference to the selected radiation model
        static autoPtr<IntegrationScheme> New
        (
            const word& phiName,
            const dictionary& dict
        );


    //- Destructor
    virtual ~IntegrationScheme();


    // Member Functions

        //- Perform the Integration
        virtual integrationResult integrate
        (
            const Type& phi,
            const scalar dt,
            const Type& alphaBeta,
            const scalar beta
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeIntegrationScheme(Type)                                           \
                                                                              \
    defineNamedTemplateTypeNameAndDebug(IntegrationScheme<Type>, 0);          \
                                                                              \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
        IntegrationScheme<Type>,                                              \
        dictionary                                                            \
    );


#define makeIntegrationSchemeType(SS, Type)                                   \
                                                                              \
    defineNamedTemplateTypeNameAndDebug(SS<Type>, 0);                         \
                                                                              \
    IntegrationScheme<Type>::adddictionaryConstructorToTable<SS<Type> >       \
        add##SS##Type##ConstructorToTable_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::IntegrationScheme<Type>::IntegrationScheme
(
    const word& phiName,
    const dictionary& dict
)
:
   phiName_(phiName),
   dict_(dict)
{}


template<class Type>
CML::IntegrationScheme<Type>::IntegrationScheme(const IntegrationScheme& is)
:
    phiName_(is.phiName_),
    dict_(is.dict_)
{}


// * * * * * * * * * * * * * * * * Destructor    * * * * * * * * * * * * * * //

template<class Type>
CML::IntegrationScheme<Type>::~IntegrationScheme()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
typename CML::IntegrationScheme<Type>::integrationResult
CML::IntegrationScheme<Type>::integrate
(
    const Type& phi,
    const scalar dt,
    const Type& alphaBeta,
    const scalar beta
) const
{
    notImplemented
    (
        "CML::IntegrationScheme<Type>::integrationResult"
        "CML::IntegrationScheme<Type>::integrate"
        "("
            "const Type&, "
            "const scalar, "
            "const Type&, "
            "const scalar"
        ") const"
    );

    typename IntegrationScheme<Type>::integrationResult retValue;
    retValue.average() = pTraits<Type>::zero;
    retValue.value() = pTraits<Type>::zero;

    return retValue;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::autoPtr<CML::IntegrationScheme<Type> >
CML::IntegrationScheme<Type>::New
(
    const word& phiName,
    const dictionary& dict
)
{
    const word schemeName(dict.lookup(phiName));

    Info<< "Selecting " << phiName << " integration scheme "
        << schemeName << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(schemeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "IntegrationScheme::New(const dictionary&)"
        )   << "Unknown integration scheme type "
            << schemeName << nl << nl
            << "Valid integration scheme types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc() << nl
            << exit(FatalError);
    }

    return autoPtr<IntegrationScheme<Type> >(cstrIter()(phiName, dict));
}


#endif

// ************************************************************************* //
