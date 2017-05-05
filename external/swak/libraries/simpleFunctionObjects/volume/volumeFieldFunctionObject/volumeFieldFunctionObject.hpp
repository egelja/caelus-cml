/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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
    volumeFieldFunctionObject

Description
    Basis for functionObjects that might work on every field on a volume

SourceFiles
    volumeFieldFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef volumeFieldFunctionObject_H
#define volumeFieldFunctionObject_H

#include "timelineFunctionObject.hpp"

#include "tensor.hpp"
#include "sphericalTensor.hpp"
#include "symmTensor.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class volumeFieldFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class volumeFieldFunctionObject
:
    public timelineFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    volumeFieldFunctionObject(const volumeFieldFunctionObject&);
    
    //- Disallow default bitwise assignment
    void operator=(const volumeFieldFunctionObject&);

    //- Names of fields to probe
    wordList fieldNames_;

    //- Indices (into fieldNames_) of scalar/vector/tensor fields
    wordList scalarFields_;
    wordList vectorFields_;
    wordList sphericalTensorFields_;
    wordList symmTensorFields_;
    wordList tensorFields_;
    
     //- Process and write field
    template<class T>
    void processAndWrite(const word& fieldName);

    //- Process and write field
    template<class T>
    void processAndWrite(const wordList& typeFields);

    template<class T>
    void findFields(wordList& typeFieldNames, boolList& foundFields);

//     //- process single field on all patches
//     template <class T>
//     Field<T> process(const word& fieldName) const;

protected:

    //- Names of the files
    virtual wordList fileNames();

    // This is clumsy, but virtual+templated member functions are not allowed
    virtual scalarField process(const word& fieldName,scalar preset)=0;
    virtual Field<vector> process(const word& fieldName,vector preset)=0;
    virtual Field<sphericalTensor> process(const word& fieldName,sphericalTensor preset)=0;
    virtual Field<symmTensor> process(const word& fieldName,symmTensor preset)=0;
    virtual Field<tensor> process(const word& fieldName,tensor preset)=0;

public:

    //- Runtime type information
    TypeName("volumeFieldFunctionObject");


    // Constructors

    //- Construct from components
    volumeFieldFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );


    // Member Functions

    //- start is called at the start of the time-loop
    virtual bool start();

    virtual void write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
#include "IOmanip.hpp"
#include "fvMesh.hpp"
#include "fvCFD.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class T>
void volumeFieldFunctionObject::findFields(wordList& typeFieldNames, boolList& foundFields)
{
    typeFieldNames.setSize(fieldNames_.size());
    label typeFieldI = 0;

    forAll(fieldNames_, fieldI)
    {
        const word& fldName = fieldNames_[fieldI];

        if (obr_.foundObject<T>(fldName))
        {
            typeFieldNames[typeFieldI++] = fldName;
            foundFields[fieldI] = true;
        }
    }

    typeFieldNames.setSize(typeFieldI);
}


template <class T>
void volumeFieldFunctionObject::processAndWrite(const word& fieldName)
{
    const GeometricField<T, fvPatchField, volMesh>& fld =
        obr_.lookupObject<GeometricField<T, fvPatchField, volMesh> >
        (
            fieldName
        );

    // Make sure all processors call sample
    Field<T> vals(process(fieldName,-VGREAT*pTraits<T>::one));

    if (Pstream::master())
    {
        writeTime(fieldName,fld.time().value());
        writeData(fieldName,vals);
        endData(fieldName);
    }
}

template <class T>
void volumeFieldFunctionObject::processAndWrite(const wordList& typeFields)
{
    forAll(typeFields, i)
    {
        processAndWrite<T>(typeFields[i]);
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML



#endif

// ************************************************************************* //
