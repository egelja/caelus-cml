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
    CML::groovyBCCommon

Description
    CML::groovyBCCommon

SourceFiles
    groovyBCCommon.cpp

Contributors/Copyright:
    2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef groovyBCCommon_H
#define groovyBCCommon_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "dictionary.hpp"
#include "exprString.hpp"

namespace CML
{

    class pointPatch;
    class fvPatch;

/*---------------------------------------------------------------------------*\
                     Class groovyBCFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class groovyBCCommon
{

    bool evaluateDuringConstruction_;

protected:

    // Protected data

    bool debug_;
    bool hasGradient_;

    // the expressions
    exprString valueExpression_;
    exprString gradientExpression_;
    exprString fractionExpression_;

    bool evaluateDuringConstruction() const
        { return evaluateDuringConstruction_; }

public:

    static exprString nullValue();

    static const fvPatch &getFvPatch(const pointPatch &pp);

    // Constructors

    //- Construct from nothing
    groovyBCCommon
    (
        bool hasGradient,
        bool isPoint=false,
        exprString fractionExpression="1"
    );

    //- Construct from dictionary
    groovyBCCommon
    (
        const dictionary&,
        bool hasGradient,
        bool isPoint=false,
        exprString fractionExpression="1"
    );

    //- Copy constructor
    groovyBCCommon
    (
        const groovyBCCommon<Type> &
    );

    // Member functions

    //- Write
    void write(Ostream&) const;
};


    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#include "fvMesh.hpp"
#include "pointMesh.hpp"

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
exprString groovyBCCommon<Type>::nullValue()
{
    if(string(pTraits<Type>::typeName)==string("vector")) {
        return exprString("vector(0,0,0)");
    } else if(string(pTraits<Type>::typeName)==string("tensor")) {
        return exprString("tensor(0,0,0,0,0,0,0,0,0)");
    } else if(string(pTraits<Type>::typeName)==string("symmTensor")) {
        return exprString("symmTensor(0,0,0,0,0,0)");
    } else if(string(pTraits<Type>::typeName)==string("sphericalTensor")) {
        return exprString("sphericalTensor(0)");
    } else {
        OStringStream tmp;
        tmp << pTraits<Type>::zero;
        return exprString(tmp.str().c_str());
    }
}

template<class Type>
const fvPatch &groovyBCCommon<Type>::getFvPatch(const pointPatch &pp) {
    if(!isA<fvMesh>(pp.boundaryMesh().mesh().db())) {
        FatalErrorInFunction
            << " This will only work if I can find a fvMesh, but I only found a "
            << typeid(pp.boundaryMesh().mesh().db()).name()
            << endl
            << exit(FatalError);
    }
    const fvMesh &fv=dynamic_cast<const fvMesh &>(pp.boundaryMesh().mesh().db());
    return fv.boundary()[pp.index()];
}


template<class Type>
groovyBCCommon<Type>::groovyBCCommon
(
    bool hasGradient,
    bool isPoint,
    exprString fractionExpression
)
:
    evaluateDuringConstruction_(false),
    debug_(false),
    hasGradient_(hasGradient),
    fractionExpression_(
        isPoint ? "toPoint("+fractionExpression+")" : fractionExpression,
        dictionary::null
    )
{
    valueExpression_ = nullValue();
    if(hasGradient_) {
        gradientExpression_ = nullValue();
    }
}


template<class Type>
groovyBCCommon<Type>::groovyBCCommon
(
    const groovyBCCommon<Type>& ptf
)
:
    evaluateDuringConstruction_(ptf.evaluateDuringConstruction_),
    debug_(ptf.debug_),
    hasGradient_(ptf.hasGradient_),
    valueExpression_(ptf.valueExpression_),
    gradientExpression_(ptf.gradientExpression_),
    fractionExpression_(ptf.fractionExpression_)
{
}


template<class Type>
groovyBCCommon<Type>::groovyBCCommon
(
    const dictionary& dict,
    bool hasGradient,
    bool isPoint,
    exprString fractionExpression
)
:
    evaluateDuringConstruction_(
        dict.lookupOrDefault<bool>("evaluateDuringConstruction",false)
    ),
    debug_(dict.lookupOrDefault<bool>("debug",false)),
    hasGradient_(hasGradient),
    fractionExpression_(
        dict.lookupOrDefault(
            "fractionExpression",
            isPoint ? string("toPoint("+fractionExpression+")") : string(fractionExpression)),
        dict
    )
{
    if (dict.found("valueExpression"))
    {
        valueExpression_=exprString(
            dict.lookup("valueExpression"),
            dict
        );
    } else {
        valueExpression_ = nullValue();
    }
    if (dict.found("gradientExpression") && hasGradient)
    {
        gradientExpression_=exprString(
            dict.lookup("gradientExpression"),
            dict
        );
    } else {
        gradientExpression_ = nullValue();
    }
    if(
        Pstream::parRun()
        &&
        Pstream::defaultCommsType == Pstream::blocking
    ) {
        WarningInFunction
            << "The commsType is set to 'blocking'. This might cause the run to"
            << " fail for groovyBC (or similar) like " << dict.name() << nl
            << "If you experience a MPI-related failure of this run go to "
            << "the file '$WM_PROJECT_DIR/etc/controlDict' and change the "
            << "setting 'commsType' to something different than 'blocking'"
            << endl;
    }
}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void groovyBCCommon<Type>::write(Ostream& os) const
{
    os.writeKeyword("valueExpression")
        << valueExpression_ << token::END_STATEMENT << nl;
    if(hasGradient_) {
        os.writeKeyword("gradientExpression")
            << gradientExpression_ << token::END_STATEMENT << nl;
        os.writeKeyword("fractionExpression")
            << fractionExpression_ << token::END_STATEMENT << nl;
    }
    os.writeKeyword("evaluateDuringConstruction")
        << evaluateDuringConstruction_ << token::END_STATEMENT << nl;

    // debug_ not written on purpose
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    #endif

// ************************************************************************* //
