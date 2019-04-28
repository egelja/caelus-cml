/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2013 OpenFOAM Foundation
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
    CML::fieldAverage

Group
    grpFieldFunctionObjects

Description
    This function object calculates average quantities for a user-specified
    selection of volumetric and surface fields.  Fields are entered as a list
    of sub-dictionaries, which indicate the type of averages to perform, and
    can be updated during the calculation.  The current options include:
    - \c mean: arithmetic mean:
        \f[
            \overline{x} = \frac{1}{N}\displaystyle\sum\limits_{i=0}^N x_i
        \f]
    - \c prime2Mean: prime-squared mean
        \f[
            \overline{x'}^2 = \frac{1}{N}\displaystyle\sum\limits_{i=0}^N
            (x_i - \overline{x})^2
        \f]
    - base: average over 'time', or 'iteration' (\f$N\f$ in the above)
    - window: optional averaging window, specified in 'base' units

    Average field names are constructed by concatenating the base field with
    the averaging type, e.g. when averaging field 'U', the resultant fields
    are:
    - arithmetic mean field, UMean
    - prime-squared field, UPrime2Mean

    Information regarding the number of averaging steps, and total averaging
    time are written on a per-field basis to the
    \c fieldAveragingProperties dictionary, located in \<time\>/uniform

    When restarting form a previous calculation, the averaging is continuous.
    However, the averaging process can be restarted using the \c resetOnRestart
    option.

    To restart the averaging process after each calculation output time, use
    the \c resetOnOutput option.

    Example of function object specification:
    \verbatim
    fieldAverage1
    {
        type fieldAverage;
        functionObjectLibs ("libfieldFunctionObjects.so");
        ...
        resetOnRestart true;
        resetOnOutput false;
        fields
        (
            U
            {
                mean            on;
                prime2Mean      on;
                base            time;
                window          10.0;
                windowName      w1;
            }
            p
            {
                mean            on;
                prime2Mean      on;
                base            time;
            }
        );
    }
    \endverbatim

    \heading Function object usage
    \table
        Property     | Description             | Required    | Default value
        type         | type name: fieldAverage | yes         |
        resetOnRestart | flag to reset the averaging on restart | yes  |
        resetOnOutput| flag to reset the averaging on output | yes |
        fields       | list of fields and averaging options | yes |
    \endtable


Note
    To employ the \c prime2Mean option, the \c mean option must be selecetd.

SeeAlso
    CML::functionObject
    CML::OutputFilterFunctionObject

SourceFiles
    fieldAverage.cpp
    fieldAverageItem.cpp

\*---------------------------------------------------------------------------*/

#ifndef fieldAverage_H
#define fieldAverage_H

#include "pointFieldFwd.hpp"
#include "volFieldsFwd.hpp"
#include "Switch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class fieldAverageItem;
template<class Type>
class List;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                        Class fieldAverage Declaration
\*---------------------------------------------------------------------------*/

class fieldAverage
{
protected:

    // Protected data

        //- Name of this set of field averages.
        word name_;

        //- Database this class is registered to
        const objectRegistry& obr_;

        //- On/off switch
        bool active_;

        //- Time at last call, prevents repeated averaging
        label prevTimeIndex_;

        //- Reset the averaging process on restart flag
        Switch resetOnRestart_;

        //- Reset the averaging process on output flag
        Switch resetOnOutput_;

        //- Initialised flag
        bool initialised_;

        //- List of field average items, describing what averages to be
        //  calculated and output
        List<fieldAverageItem> faItems_;

        // Counters

            //- Iteration steps counter
            List<label> totalIter_;

            //- Total time counter
            List<scalar> totalTime_;


    // Private Member Functions

        // Initialisation routines

            //- Checkout fields (causes deletion) from the database
            //  and reset lists
            void resetFields();

            //- Reset lists (clear existing values) and initialize averaging.
            //  Check requested field averages are valid, populate field lists
            void initialize();

            //- Add mean average field to database
            template<class Type>
            void addMeanFieldType(const label fieldI);

            //- Add mean average field to database
            template<class Type>
            void addMeanField(const label fieldI);

            //- Add prime-squared average field to database
            template<class Type1, class Type2>
            void addPrime2MeanFieldType(const label fieldI);

            //- Add prime-squared average field to database
            template<class Type1, class Type2>
            void addPrime2MeanField(const label fieldI);


        // Calculation functions

            //- Main calculation routine
            virtual void calcAverages();

            //- Calculate mean average fields
            template<class Type>
            void calculateMeanFieldType(const label fieldI) const;

            //- Calculate mean average fields
            template<class Type>
            void calculateMeanFields() const;

            //- Calculate prime-squared average fields
            template<class Type1, class Type2>
            void calculatePrime2MeanFieldType(const label fieldI) const;

            //- Calculate prime-squared average fields
            template<class Type1, class Type2>
            void calculatePrime2MeanFields() const;

            //- Add mean-squared field value to prime-squared mean field
            template<class Type1, class Type2>
            void addMeanSqrToPrime2MeanType(const label fieldI) const;

            //- Add mean-squared field value to prime-squared mean field
            template<class Type1, class Type2>
            void addMeanSqrToPrime2Mean() const;


        // I-O

            //- Write averages
            virtual void writeAverages() const;

            //- Write fields
            template<class Type>
            void writeFieldType(const word& fieldName) const;

            //- Write fields
            template<class Type>
            void writeFields() const;

            //- Write averaging properties - steps and time
            void writeAveragingProperties() const;

            //- Read averaging properties - steps and time
            void readAveragingProperties();


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

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Calculate the field average data and write
        virtual void write();

        //- Update mesh
        virtual void updateMesh(const mapPolyMesh&);

        //- Move points
        virtual void movePoints(const pointField&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fieldAverageItem.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "OFstream.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void CML::fieldAverage::addMeanFieldType(const label fieldI)
{
    faItems_[fieldI].active() = true;

    const word& fieldName = faItems_[fieldI].fieldName();
    const word& meanFieldName = faItems_[fieldI].meanFieldName();

    Info<< "    Reading/initialising field " << meanFieldName << endl;

    if (obr_.foundObject<Type>(meanFieldName))
    {
       // do nothing
    }
    else if (obr_.found(meanFieldName))
    {
        Info<< "    Cannot allocate average field " << meanFieldName
            << " since an object with that name already exists."
            << " Disabling averaging for field." << endl;

        faItems_[fieldI].mean() = false;
    }
    else
    {
        const Type& baseField = obr_.lookupObject<Type>(fieldName);

        // Store on registry
        obr_.store
        (
            new Type
            (
                IOobject
                (
                    meanFieldName,
                    obr_.time().timeName(obr_.time().startTime().value()),
                    obr_,
                    resetOnOutput_
                  ? IOobject::NO_READ
                  : IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE
                ),
                1*baseField
            )
        );
    }
}


template<class Type>
void CML::fieldAverage::addMeanField(const label fieldI)
{
    if (faItems_[fieldI].mean())
    {
        typedef GeometricField<Type, fvPatchField, volMesh> volFieldType;
        typedef GeometricField<Type, fvsPatchField, surfaceMesh> surfFieldType;

        const word& fieldName = faItems_[fieldI].fieldName();

        if (obr_.foundObject<volFieldType>(fieldName))
        {
            addMeanFieldType<volFieldType>(fieldI);
        }
        else if (obr_.foundObject<surfFieldType>(fieldName))
        {
            addMeanFieldType<surfFieldType>(fieldI);
        }
    }
}


template<class Type1, class Type2>
void CML::fieldAverage::addPrime2MeanFieldType(const label fieldI)
{
    const word& fieldName = faItems_[fieldI].fieldName();
    const word& meanFieldName = faItems_[fieldI].meanFieldName();
    const word& prime2MeanFieldName = faItems_[fieldI].prime2MeanFieldName();

    Info<< "    Reading/initialising field " << prime2MeanFieldName << nl;

    if (obr_.foundObject<Type2>(prime2MeanFieldName))
    {
        // do nothing
    }
    else if (obr_.found(prime2MeanFieldName))
    {
        Info<< "    Cannot allocate average field " << prime2MeanFieldName
            << " since an object with that name already exists."
            << " Disabling averaging for field." << nl;

        faItems_[fieldI].prime2Mean() = false;
    }
    else
    {
        const Type1& baseField = obr_.lookupObject<Type1>(fieldName);
        const Type1& meanField = obr_.lookupObject<Type1>(meanFieldName);

        // Store on registry
        obr_.store
        (
            new Type2
            (
                IOobject
                (
                    prime2MeanFieldName,
                    obr_.time().timeName(obr_.time().startTime().value()),
                    obr_,
                    resetOnOutput_
                  ? IOobject::NO_READ
                  : IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE
                ),
                sqr(baseField) - sqr(meanField)
            )
        );
    }
}


template<class Type1, class Type2>
void CML::fieldAverage::addPrime2MeanField(const label fieldI)
{
    typedef GeometricField<Type1, fvPatchField, volMesh> volFieldType1;
    typedef GeometricField<Type1, fvsPatchField, surfaceMesh> surfFieldType1;

    typedef GeometricField<Type2, fvPatchField, volMesh> volFieldType2;
    typedef GeometricField<Type2, fvsPatchField, surfaceMesh> surfFieldType2;

    if (faItems_[fieldI].prime2Mean())
    {
        const word& fieldName = faItems_[fieldI].fieldName();

        if (!faItems_[fieldI].mean())
        {
            FatalErrorInFunction
                << "To calculate the prime-squared average, the "
                << "mean average must also be selected for field "
                << fieldName << nl << exit(FatalError);
        }

        if (obr_.foundObject<volFieldType1>(fieldName))
        {
            addPrime2MeanFieldType<volFieldType1, volFieldType2>(fieldI);
        }
        else if (obr_.foundObject<surfFieldType1>(fieldName))
        {
            addPrime2MeanFieldType<surfFieldType1, surfFieldType2>(fieldI);
        }
    }
}


template<class Type>
void CML::fieldAverage::calculateMeanFieldType(const label fieldI) const
{
    const word& fieldName = faItems_[fieldI].fieldName();

    if (obr_.foundObject<Type>(fieldName))
    {
        const Type& baseField = obr_.lookupObject<Type>(fieldName);

        Type& meanField = const_cast<Type&>
        (
            obr_.lookupObject<Type>(faItems_[fieldI].meanFieldName())
        );

        scalar dt = obr_.time().deltaTValue();
        scalar Dt = totalTime_[fieldI];

        if (faItems_[fieldI].iterBase())
        {
            dt = 1.0;
            Dt = scalar(totalIter_[fieldI]);
        }

        scalar alpha = (Dt - dt)/Dt;
        scalar beta = dt/Dt;

        if (faItems_[fieldI].window() > 0)
        {
            const scalar w = faItems_[fieldI].window();

            if (Dt - dt >= w)
            {
                alpha = (w - dt)/w;
                beta = dt/w;
            }
        }

        meanField = alpha*meanField + beta*baseField;
    }
}


template<class Type>
void CML::fieldAverage::calculateMeanFields() const
{
    typedef GeometricField<Type, fvPatchField, volMesh> volFieldType;
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> surfFieldType;

    forAll(faItems_, i)
    {
        if (faItems_[i].mean())
        {
            calculateMeanFieldType<volFieldType>(i);
            calculateMeanFieldType<surfFieldType>(i);
        }
    }
}


template<class Type1, class Type2>
void CML::fieldAverage::calculatePrime2MeanFieldType(const label fieldI) const
{
    const word& fieldName = faItems_[fieldI].fieldName();

    if (obr_.foundObject<Type1>(fieldName))
    {
        const Type1& baseField = obr_.lookupObject<Type1>(fieldName);
        const Type1& meanField =
            obr_.lookupObject<Type1>(faItems_[fieldI].meanFieldName());

        Type2& prime2MeanField = const_cast<Type2&>
        (
            obr_.lookupObject<Type2>(faItems_[fieldI].prime2MeanFieldName())
        );

        scalar dt = obr_.time().deltaTValue();
        scalar Dt = totalTime_[fieldI];

        if (faItems_[fieldI].iterBase())
        {
            dt = 1.0;
            Dt = scalar(totalIter_[fieldI]);
        }

        scalar alpha = (Dt - dt)/Dt;
        scalar beta = dt/Dt;

        if (faItems_[fieldI].window() > 0)
        {
            const scalar w = faItems_[fieldI].window();

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


template<class Type1, class Type2>
void CML::fieldAverage::calculatePrime2MeanFields() const
{
    typedef GeometricField<Type1, fvPatchField, volMesh> volFieldType1;
    typedef GeometricField<Type1, fvsPatchField, surfaceMesh> surfFieldType1;

    typedef GeometricField<Type2, fvPatchField, volMesh> volFieldType2;
    typedef GeometricField<Type2, fvsPatchField, surfaceMesh> surfFieldType2;

    forAll(faItems_, i)
    {
        if (faItems_[i].prime2Mean())
        {
            calculatePrime2MeanFieldType<volFieldType1, volFieldType2>(i);
            calculatePrime2MeanFieldType<surfFieldType1, surfFieldType2>(i);
        }
    }
}


template<class Type1, class Type2>
void CML::fieldAverage::addMeanSqrToPrime2MeanType(const label fieldI) const
{
    const word& fieldName = faItems_[fieldI].fieldName();

    if (obr_.foundObject<Type1>(fieldName))
    {
        const Type1& meanField =
            obr_.lookupObject<Type1>(faItems_[fieldI].meanFieldName());

        Type2& prime2MeanField = const_cast<Type2&>
        (
            obr_.lookupObject<Type2>(faItems_[fieldI].prime2MeanFieldName())
        );

        prime2MeanField += sqr(meanField);
    }
}


template<class Type1, class Type2>
void CML::fieldAverage::addMeanSqrToPrime2Mean() const
{
    typedef GeometricField<Type1, fvPatchField, volMesh> volFieldType1;
    typedef GeometricField<Type1, fvsPatchField, surfaceMesh> surfFieldType1;

    typedef GeometricField<Type2, fvPatchField, volMesh> volFieldType2;
    typedef GeometricField<Type2, fvsPatchField, surfaceMesh> surfFieldType2;

    forAll(faItems_, i)
    {
        if (faItems_[i].prime2Mean())
        {
            addMeanSqrToPrime2MeanType<volFieldType1, volFieldType2>(i);
            addMeanSqrToPrime2MeanType<surfFieldType1, surfFieldType2>(i);
        }
    }
}


template<class Type>
void CML::fieldAverage::writeFieldType(const word& fieldName) const
{
    if (obr_.foundObject<Type>(fieldName))
    {
        const Type& f = obr_.lookupObject<Type>(fieldName);
        f.write();
    }
}


template<class Type>
void CML::fieldAverage::writeFields() const
{
    typedef GeometricField<Type, fvPatchField, volMesh> volFieldType;
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> surfFieldType;

    forAll(faItems_, i)
    {
        if (faItems_[i].mean())
        {
            const word& fieldName = faItems_[i].meanFieldName();
            writeFieldType<volFieldType>(fieldName);
            writeFieldType<surfFieldType>(fieldName);
        }
        if (faItems_[i].prime2Mean())
        {
            const word& fieldName = faItems_[i].prime2MeanFieldName();
            writeFieldType<volFieldType>(fieldName);
            writeFieldType<surfFieldType>(fieldName);
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
