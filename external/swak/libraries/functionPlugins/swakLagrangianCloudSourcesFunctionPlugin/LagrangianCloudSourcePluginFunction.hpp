/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
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
    CML::LagrangianCloudSourcePluginFunction

Description

SourceFiles
    LagrangianCloudSourcePluginFunction.cpp

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef LagrangianCloudSourcePluginFunction_H
#define LagrangianCloudSourcePluginFunction_H

#include "FieldValuePluginFunction.hpp"

#include "runTimeSelectionTables.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class LagrangianCloudSourcePluginFunction Declaration
\*---------------------------------------------------------------------------*/

    // template<class CloudType>
class LagrangianCloudSourcePluginFunction
:
    public FieldValuePluginFunction
{
    //- Disallow default bitwise assignment
    void operator=(const LagrangianCloudSourcePluginFunction &);

    LagrangianCloudSourcePluginFunction(const LagrangianCloudSourcePluginFunction &);

    word cloudName_;

    HashSet<word> supportedTypes_;

protected:
    //    typedef CloudType cloudType;

    template<class LookupCloudType>
    bool hasCloudAs();

    Ostream& listAvailableClouds(Ostream &o);

    template<class OtherCloudType,class LookupCloudType>
    const OtherCloudType &getCloudAs();

    const word &cloudName() const { return cloudName_; }

    const HashSet<word> &supportedTypes() const
        { return supportedTypes_; }

public:
    LagrangianCloudSourcePluginFunction(
        const FieldValueExpressionDriver &parentDriver,
        const word &name,
        const word &returnValueType,
        const string &additionalArgs=""
    );

    virtual ~LagrangianCloudSourcePluginFunction() {}

    //    TypeName("LagrangianCloudSourcePluginFunction");

    void setArgument(label index,const word &);

};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML {

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


template<class LookupCloudType>
bool LagrangianCloudSourcePluginFunction::hasCloudAs()
{
    supportedTypes_.insert(typeid(LookupCloudType).name());
    //    supportedTypes_.insert(LookupCloudType::typeName);

    return mesh().template foundObject<LookupCloudType>
        (
            cloudName()
        );
}

template<class OtherCloudType,class LookupCloudType>
const OtherCloudType &LagrangianCloudSourcePluginFunction::getCloudAs()
{
    return
        //        dynamicCast<const OtherCloudType &>(
        dynamic_cast<const OtherCloudType &>(
            mesh().template lookupObject<LookupCloudType>
            (
                cloudName()
            )
        );
}



// * * * * * * * * * * * * * * * Concrete implementations * * * * * * * * * //


} // namespace


// return a autoPtr to the needed field. This is required because
// "similar" clouds don't really inherit (because of the template paramter)
#define tryCall(rType,neededType,usedType,theCall)    \
    if(hasCloudAs<neededType>()) {                             \
        return autoPtr<rType> (                                \
            new rType(                                         \
                getCloudAs<neededType,usedType>().theCall      \
            )                                                  \
        );                                                     \
    }

// Fail if no cloud data was found
#define noCloudFound(pVar) \
    if(!pVar.valid()) {                                                 \
        listAvailableClouds(Info);                                      \
        FatalErrorInFunction                                            \
            << "No matching cloud " << this->cloudName() << " found"    \
            << endl                                                     \
            << "Supported cloud types (typeid): "                       \
            << supportedTypes() << endl                                 \
            << exit(FatalError);                                        \
    }


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
