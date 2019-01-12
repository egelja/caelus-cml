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
    CML::SwakSetValue

Description
    Fixes field values according to an expression

SourceFiles
    SwakSetValue.cpp

Contributors/Copyright:
    2010, 2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef SwakSetValue_H
#define SwakSetValue_H

#include "SwakBasicSourceCommon.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fv
{
/*---------------------------------------------------------------------------*\
                           Class SwakSetValue Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class SwakSetValue
:
    public SwakBasicSourceCommon<T>
{
    // Private data

    SwakSetValue(const SwakSetValue&);

    bool useMaskExpression_;

    exprString maskExpression_;

    bool getMask(DynamicList<label> &,const word &psi);

public:

    //- Runtime type information
    TypeName("SwakSetValue");

    // Constructors

        //- Construct from a dictionary
        SwakSetValue
        (
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );

    // Destructor

        virtual ~SwakSetValue();

        // Evaluation

            //- Set value on vector field
            virtual void setValue(fvMatrix<T>& eqn, const label fieldI);


};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
} // End namespace fv
} // End namespace CML

#include "polyMesh.hpp"
#include "cellSet.hpp"
#include "fvMatrix.hpp"

#include "FieldValueExpressionDriver.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from dictionary
template<class T>
CML::fv::SwakSetValue<T>::SwakSetValue
(
    const word& name,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    SwakBasicSourceCommon<T>(name, modelType, dict, mesh),
    useMaskExpression_(readBool(this->coeffs().lookup("useMaskExpression"))),
    maskExpression_(
        useMaskExpression_
        ?
        exprString(
	    this->coeffs().lookup("maskExpression"),
	    this->coeffs()
	)
        :
        exprString("")
    )
{
    this->read(dict);

    this->driver().createWriterAndRead(
        dict.name().name()+"_"+this->type()+"<"+
        pTraits<T>::typeName+">"
    );

    if(this->verbose_) {
        WarningInFunction
            << "Fixing to the fields " << this->fieldNames_
            << " to the values " << this->expressions_
            << " with the mask " << maskExpression_
            << " will be verbose. To switch this off set the "
            << "parameter 'verbose' to false" << endl;
    }

}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class T>
CML::fv::SwakSetValue<T>::~SwakSetValue()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
template<class T>
bool CML::fv::SwakSetValue<T>::getMask(DynamicList<label> &cellIDs,const word &psi)
{
    this->driver().parse(maskExpression_);
    if(
        !this->driver().
        FieldValueExpressionDriver::template resultIsTyp<volScalarField>(true)
    ) {
        FatalErrorInFunction
            << "Result of " << maskExpression_ << " is not a logical expression"
                << endl
                << exit(FatalError);
    }

    const volScalarField &cond=this->driver().
        FieldValueExpressionDriver::template getResult<volScalarField>();
    volScalarField usedCond(0*cond);
    forAll(this->cells_,i) {
        label cellI=this->cells_[i];
        usedCond[cellI]=cond[cellI];
    }

    forAll(usedCond,cellI) {
        if(usedCond[cellI]!=0) {
            cellIDs.append(cellI);
        }
    }

    cellIDs.shrink();
    label size=cellIDs.size();
    reduce(size,plusOp<label>());

    if(size==0) {
        if(this->verbose_) {
            Info << "No cells fixed for field " << psi << endl;
        }
        return false;
    }
    if(this->verbose_) {
        Info << size << " cells fixed for field " << psi << endl;
    }

    return true;
}


template<class T>
void CML::fv::SwakSetValue<T>::setValue
(
    fvMatrix<T>& eqn,
    const label fieldI
)
{
    this->driver().clearVariables();
    this->driver().parse(this->expressions_[fieldI]);
    if(
        !this->driver().
        FieldValueExpressionDriver::template resultIsTyp<typename SwakSetValue<T>::resultField>()
    ) {
        FatalErrorInFunction
            << "Result of " << this->expressions_[fieldI] << " is not a "
                << pTraits<T>::typeName
                << endl
                << exit(FatalError);
    }

    typename SwakSetValue<T>::resultField result(
        this->driver().
        FieldValueExpressionDriver::template getResult<typename SwakSetValue<T>::resultField>()
    );

    if(this->dimensions_[fieldI]!=eqn.psi().dimensions()) {
        FatalErrorInFunction
            << "Dimension " << this->dimensions_[fieldI] << " for field "
                << eqn.psi().name() << " in " << this->name()
                << " is not the required " << eqn.psi().dimensions()
                << endl
                << exit(FatalError);
    };

    DynamicList<label> cellIDs;

    if(useMaskExpression_) {
        if(!getMask(cellIDs,eqn.psi().name())) {
            return;
        }
    } else {
        cellIDs=this->cells_;
    }

    List<T> values(cellIDs.size());

    //    UIndirectList<Type>(values, cells_) = injectionRate_[fieldI];
    forAll(cellIDs,i)
    {
	label cellI=cellIDs[i];

        values[i]=result[cellI];
    }

    eqn.setValues(cellIDs, values);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
