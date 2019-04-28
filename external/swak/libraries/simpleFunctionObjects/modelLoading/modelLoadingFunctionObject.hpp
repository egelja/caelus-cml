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
    modelLoadingFunctionObject

Description
    Basis for the other function objects the load and correct a model

SourceFiles
    modelLoadingFunctionObject.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef modelLoadingFunctionObject_H
#define modelLoadingFunctionObject_H

#include "simpleFunctionObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class modelLoadingFunctionObject Declaration
\*---------------------------------------------------------------------------*/

template <class ModelType>
class modelLoadingFunctionObject
:
    public simpleFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    modelLoadingFunctionObject(const modelLoadingFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const modelLoadingFunctionObject&);

    //- the actual model we're working with
    autoPtr<ModelType> model_;

    //- whether the model should be corrected
    bool correctModel_;

    //- whether the model can be reloaded later
    bool allowReload_;

    //- fail if a model of this type already exists
    bool failIfModelTypeExists_;
protected:
    virtual autoPtr<ModelType> initModel() = 0;

public:

    //- Runtime type information
    //    TypeName("modelLoadingFunctionObject");


    // Constructors

    //- Construct from components
    modelLoadingFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );


    // Member Functions

    //- start is called at the start of the time-loop
    virtual inline bool start();

    //- Sample and write
    virtual void write();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    //    defineTemplateTypeNameAndDebug(modelLoadingFunctionObject, 0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template <class ModelType>
modelLoadingFunctionObject<ModelType>::modelLoadingFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    simpleFunctionObject(name,t,dict)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <class ModelType>
bool modelLoadingFunctionObject<ModelType>::start()
{
    simpleFunctionObject::start();

    correctModel_=readBool(dict_.lookup("correctModel"));
    allowReload_=readBool(dict_.lookup("allowReload"));
    failIfModelTypeExists_=dict_.lookupOrDefault<bool>(
        "failIfModelTypeExists",true
    );

    if(!model_.valid()) {
        if(
            this->obr().template lookupClass<ModelType>().size()>0
            &&
            failIfModelTypeExists_
        ) {
            FatalErrorInFunction
                << "Model of type " << ModelType::typeName
                    << " in " << this->name()
                    << " already existing. If this is OK overrule this "
                    << "message by setting 'failIfModelTypeExists' to 'false'"
                    << endl
                    << exit(FatalError);

        } else {
            model_.set(initModel().ptr());
        }
    } else {
        if(allowReload_) {
            model_.set(initModel().ptr());
        } else {
            WarningInFunction
                << "Not reloading model because it is not allowed"
                    << endl;
        }
    }

    return true;
}

template <class ModelType>
void modelLoadingFunctionObject<ModelType>::write()
{
    if(correctModel_) {
        if(model_.valid()) {
            Info << "Correcting model for " << this->name() << endl;

            model_->correct();
        } else {
            FatalErrorInFunction
                << "Model has never been intialized"
                    << endl
                    << exit(FatalError);
        }
    }
}

} // namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
