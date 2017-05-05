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
    writeAndEndFieldRangeFunctionObject

Description
    Checks whether a field exceeds given limits (min and max). If the
    limits are exceeded everything is written to disk and the run ends gracefully

SourceFiles
    writeAndEndFieldRangeFunctionObject.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef writeAndEndFieldRangeFunctionObject_H
#define writeAndEndFieldRangeFunctionObject_H

#include "writeAndEndFunctionObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class writeAndEndFieldRangeFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class writeAndEndFieldRangeFunctionObject
:
    public writeAndEndFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    writeAndEndFieldRangeFunctionObject(const writeAndEndFieldRangeFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const writeAndEndFieldRangeFunctionObject&);

    //- Name of the field that is to be checked
    word fieldName_;

    //- maximum value that is to be looked for
    scalar maximum_;

    //- minimum value that is to be looked for
    scalar minimum_;

    //- do the actual checking
    template <class T>
    scalar getMin() const;

    //- do the actual checking
    template <class T>
    scalar getMax() const;

    //- wrapper for the checking
    template <class T>
    bool check() const;

protected:

    //- do the actual checking
    virtual bool endRunNow();

public:

    //- Runtime type information
    TypeName("writeAndEndFieldRange");


    // Constructors

    //- Construct from components
    writeAndEndFieldRangeFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    bool start();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "volumeFieldFunctionObject.hpp"
#include "volFields.hpp"
#include "IOmanip.hpp"
#include "fvMesh.hpp"
#include "fvCFD.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<>
scalar writeAndEndFieldRangeFunctionObject::getMin<volScalarField>() const
{
    const volScalarField &fld=obr_.lookupObject<volScalarField>(fieldName_);

    return min(fld).value();
}

template<>
scalar writeAndEndFieldRangeFunctionObject::getMax<volScalarField>() const
{
    const volScalarField &fld=obr_.lookupObject<volScalarField>(fieldName_);

    return max(fld).value();
}

template<class T>
scalar writeAndEndFieldRangeFunctionObject::getMin() const
{
    const T &fld=obr_.lookupObject<T>(fieldName_);

    return min(mag(fld)).value();
}

template<class T>
scalar writeAndEndFieldRangeFunctionObject::getMax() const
{
    const T &fld=obr_.lookupObject<T>(fieldName_);

    return max(mag(fld)).value();
}

template<class T>
bool writeAndEndFieldRangeFunctionObject::check() const
{
    if(obr_.foundObject<T>(fieldName_)) {
        scalar mini=getMin<T>();
        scalar maxi=getMax<T>();

        if(mini<minimum_ || maxi>maximum_) {
            // make sure this field gets written, even if its not set to AUTO_WRITE
            obr_.lookupObject<T>(fieldName_).write();

            bool result=const_cast<Time&>(time()).writeNow();

            // makes sure that all processes finished writing before dumping
            reduce(result,andOp<bool>());

            Info << " Finishing run, because the field " << fieldName_
                << " exceeds the valid limits [ " << minimum_ << " , "
                << maximum_ << " ] with the current limits [ "
                << mini << " , " << maxi << " ]\n" << endl;

            return true;
        }
    }

    return false;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
