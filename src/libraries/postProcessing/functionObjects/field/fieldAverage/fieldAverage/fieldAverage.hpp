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
    CML::fieldAverage

Description
    Calculates the field averages given list of fieldAverageItems, e.g.

    \verbatim
    fieldAverage1
    {
        // Type of functionObject
        type fieldAverage;

        // Where to load it from (if not already in solver)
        functionObjectLibs ("libfieldFunctionObjects.so");

        // Whether to perform a clean restart, or start from previous
        // averaging info if available
        cleanRestart true;

        // Whether to reset the averaged fields after they have been written.
        // Used to average over only the preceding write interval for transient
        // cases.
        resetOnOutput true;

        // Fields to be averaged. runTime modifiable!
        fields
        (
            U
            {
                mean            on;
                prime2Mean      on;
                base            time;
            }
            p
            {
                mean            on;
                prime2Mean      on;
                base            time;
            }
        );
    \endverbatim

    Member function calcAverages() calculates the averages.

    Member function fieldAverage::write() calls calcAverages(). Average
    field names are constructed by concatenating the base field with the
    averaging type, e.g.
    - base field, U
    - arithmetic mean field, UMean
    - prime-squared field, UPrime2Mean

    Information regarding the number of averaging steps, and total averaging
    time are written on a (base) per-field basis to the
    fieldAveragingProperties dictionary, located in \<time\>/uniform

SourceFiles
    fieldAverage.cpp
    fieldAverageTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef fieldAverage_H
#define fieldAverage_H

#include "volFieldsFwd.hpp"
#include "pointFieldFwd.hpp"
#include "Switch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class fieldAverageItem;
class OFstream;
template<class Type>
class List;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                         Class fieldAverage Declaration
\*---------------------------------------------------------------------------*/

class fieldAverage
{
protected:

    // File and field name extensions

        //- Mean average
        static const word EXT_MEAN;

        //- Prime-squared average
        static const word EXT_PRIME2MEAN;


    // Private data

        //- Name of this set of field averages.
        word name_;

        //- Database this class is registered to
        const objectRegistry& obr_;

        //- On/off switch
        bool active_;

        //- Time at last call, prevents repeated averaging
        label prevTimeIndex_;

        //- Clean restart flag
        Switch cleanRestart_;

        //- resetOnOutput flag
        Switch resetOnOutput_;

        //- Initialised flag
        bool initialised_;

        //- List of field average items, describing what averages to be
        //  calculated and output
        List<fieldAverageItem> faItems_;


        // Lists of averages

            // Arithmetic mean fields
            wordList meanScalarFields_;
            wordList meanVectorFields_;
            wordList meanSphericalTensorFields_;
            wordList meanSymmTensorFields_;
            wordList meanTensorFields_;

            // Prime-squared fields
            // Only applicable to volScalarFields / volVectorFields
            wordList prime2MeanScalarFields_;
            wordList prime2MeanSymmTensorFields_;


        // Counters

            //- Iteration steps counter
            List<label> totalIter_;

            //- Total time counter
            List<scalar> totalTime_;


    // Private Member Functions

        // Initialisation routines

            //- Checkout fields (causes deletion) from the database
            //  and reset lists
            void resetFields(wordList&);

            //- Reset lists (clear existing values) and initialize averaging.
            //  Check requested field averages are valid, populate field lists
            void initialize();

            //- Add mean average field to list
            template<class Type>
            void addMeanField(const label, wordList&) const;

            //- Add prime-squared average field to list
            template<class Type1, class Type2>
            void addPrime2MeanField
            (
                const label,
                const wordList&,
                wordList&
            ) const;


        // Calculation functions

            //- Main calculation routine
            virtual void calcAverages();

            //- Calculate mean average fields
            template<class Type>
            void calculateMeanFields(const wordList&) const;

            //- Add mean-squared field value to prime-squared mean field
            template<class Type1, class Type2>
            void addMeanSqrToPrime2Mean
            (
                const wordList&,
                const wordList&
            ) const;

            //- Calculate prime-squared average fields
            template<class Type1, class Type2>
            void calculatePrime2MeanFields
            (
                const wordList&,
                const wordList&
            ) const;


        // I-O

            //- Write averages
            virtual void writeAverages() const;

            //- Write fields
            template<class Type>
            void writeFieldList(const wordList&) const;

            //- Write averaging properties - steps and time
            void writeAveragingProperties() const;

            //- Read averaging properties - steps and time
            void readAveragingProperties();


        // Functions to be over-ridden from IOoutputFilter class

            //- Update mesh
            virtual void updateMesh(const mapPolyMesh&);

            //- Move points
            virtual void movePoints(const Field<point>&);


        //- Disallow default bitwise copy construct
        fieldAverage(const fieldAverage&);

        //- Disallow default bitwise assignment
        void operator=(const fieldAverage&);


public:

    //- Runtime type information
    TypeName("fieldAverage");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        fieldAverage
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~fieldAverage();


    // Member Functions

        //- Return name of the set of field averages
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the field average data
        virtual void read(const dictionary&);

        //- Execute the averaging
        virtual void execute();

        //- Execute the averaging at the final time-loop, currently does nothing
        virtual void end();

        //- Calculate the field average data and write
        virtual void write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "fieldAverageItem.hpp"
#include "volFields.hpp"
#include "OFstream.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void CML::fieldAverage::addMeanField
(
    const label fieldI,
    wordList& meanFieldList
) const
{
    if (faItems_[fieldI].mean())
    {
        typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

        const word& fieldName = faItems_[fieldI].fieldName();

        const word meanFieldName = fieldName + EXT_MEAN;

        Info<< "Reading/calculating field " << meanFieldName << nl << endl;

        if (obr_.foundObject<fieldType>(meanFieldName))
        {
            meanFieldList[fieldI] = meanFieldName;
        }
        else if (obr_.found(meanFieldName))
        {
            Info<< "Cannot allocate average field " << meanFieldName
                << " since an object with that name already exists."
                << " Disabling averaging." << nl << endl;
            meanFieldList[fieldI] = word::null;
        }
        else
        {
            const fieldType& baseField =
                obr_.lookupObject<fieldType>(fieldName);

            // Store on registry
            obr_.store
            (
                new fieldType
                (
                    IOobject
                    (
                        meanFieldName,
                        obr_.time().timeName(obr_.time().startTime().value()),
                        obr_,
                        IOobject::READ_IF_PRESENT,
                        IOobject::NO_WRITE
                    ),
                    1*baseField
                )
            );

            meanFieldList[fieldI] = meanFieldName;
        }
    }
}


template<class Type1, class Type2>
void CML::fieldAverage::addPrime2MeanField
(
    const label fieldI,
    const wordList& meanFieldList,
    wordList& prime2MeanFieldList
) const
{
    if (faItems_[fieldI].mean() && meanFieldList[fieldI].size())
    {
        typedef GeometricField<Type1, fvPatchField, volMesh> fieldType1;
        typedef GeometricField<Type2, fvPatchField, volMesh> fieldType2;

        const word& fieldName = faItems_[fieldI].fieldName();

        const word meanFieldName = fieldName + EXT_PRIME2MEAN;
        Info<< "Reading/calculating field " << meanFieldName << nl << endl;

        if (obr_.foundObject<fieldType2>(meanFieldName))
        {
            prime2MeanFieldList[fieldI] = meanFieldName;
        }
        else if (obr_.found(meanFieldName))
        {
            Info<< "Cannot allocate average field " << meanFieldName
                << " since an object with that name already exists."
                << " Disabling averaging." << nl << endl;
            prime2MeanFieldList[fieldI] = word::null;
        }
        else
        {
            const fieldType1& baseField =
                obr_.lookupObject<fieldType1>(fieldName);
            const fieldType1& meanField =
                obr_.lookupObject<fieldType1>(meanFieldList[fieldI]);

            obr_.store
            (
                new fieldType2
                (
                    IOobject
                    (
                        meanFieldName,
                        obr_.time().timeName(obr_.time().startTime().value()),
                        obr_,
                        IOobject::READ_IF_PRESENT,
                        IOobject::NO_WRITE
                    ),
                    sqr(baseField) - sqr(meanField)
                )
            );

            prime2MeanFieldList[fieldI] = meanFieldName;
        }
    }
}


template<class Type>
void CML::fieldAverage::calculateMeanFields(const wordList& meanFieldList)
const
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    scalar dt = obr_.time().deltaTValue();

    forAll(faItems_, i)
    {
        if (faItems_[i].mean() && meanFieldList[i].size())
        {
            const word& fieldName = faItems_[i].fieldName();
            const fieldType& baseField =
                obr_.lookupObject<fieldType>(fieldName);
            fieldType& meanField = const_cast<fieldType&>
            (
                obr_.lookupObject<fieldType>(meanFieldList[i])
            );

            scalar Dt = totalTime_[i];
            if (faItems_[i].iterBase())
            {
                dt = 1.0;
                Dt = scalar(totalIter_[i]);
            }

            scalar alpha = (Dt - dt)/Dt;
            scalar beta = dt/Dt;
            if (faItems_[i].window() > 0)
            {
                const scalar w = faItems_[i].window();

                if (Dt - dt >= w)
                {
                    alpha = (w - dt)/w;
                    beta = dt/w;
                }
            }

            meanField = alpha*meanField + beta*baseField;
        }
    }
}


template<class Type1, class Type2>
void CML::fieldAverage::calculatePrime2MeanFields
(
    const wordList& meanFieldList,
    const wordList& prime2MeanFieldList
) const
{
    typedef GeometricField<Type1, fvPatchField, volMesh> fieldType1;
    typedef GeometricField<Type2, fvPatchField, volMesh> fieldType2;

    scalar dt = obr_.time().deltaTValue();

    forAll(faItems_, i)
    {
        if
        (
            faItems_[i].prime2Mean()
         && meanFieldList[i].size()
         && prime2MeanFieldList[i].size()
        )
        {
            const word& fieldName = faItems_[i].fieldName();
            const fieldType1& baseField =
                obr_.lookupObject<fieldType1>(fieldName);
            const fieldType1& meanField =
                obr_.lookupObject<fieldType1>(meanFieldList[i]);
            fieldType2& prime2MeanField = const_cast<fieldType2&>
            (
                obr_.lookupObject<fieldType2>(prime2MeanFieldList[i])
            );

            scalar Dt = totalTime_[i];
            if (faItems_[i].iterBase())
            {
                dt = 1.0;
                Dt = scalar(totalIter_[i]);
            }

            scalar alpha = (Dt - dt)/Dt;
            scalar beta = dt/Dt;
            if (faItems_[i].window() > 0)
            {
                const scalar w = faItems_[i].window();

                if (Dt - dt >= w)
                {
                    alpha = (w - dt)/w;
                    beta = dt/w;
                }
            }

            prime2MeanField =
                alpha*prime2MeanField
              + beta*sqr(baseField)
              - sqr(meanField);
        }
    }
}


template<class Type1, class Type2>
void CML::fieldAverage::addMeanSqrToPrime2Mean
(
    const wordList& meanFieldList,
    const wordList& prime2MeanFieldList
) const
{
    typedef GeometricField<Type1, fvPatchField, volMesh> fieldType1;
    typedef GeometricField<Type2, fvPatchField, volMesh> fieldType2;

    forAll(faItems_, i)
    {
        if
        (
            faItems_[i].prime2Mean()
         && meanFieldList[i].size()
         && prime2MeanFieldList[i].size()
        )
        {
            const fieldType1& meanField =
                obr_.lookupObject<fieldType1>(meanFieldList[i]);
            fieldType2& prime2MeanField = const_cast<fieldType2&>
            (
                obr_.lookupObject<fieldType2>(prime2MeanFieldList[i])
            );

            prime2MeanField += sqr(meanField);
        }
    }
}


template<class Type>
void CML::fieldAverage::writeFieldList(const wordList& fieldList) const
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    forAll(fieldList, i)
    {
        if (fieldList[i].size())
        {
            const fieldType& f = obr_.lookupObject<fieldType>(fieldList[i]);
            f.write();
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
