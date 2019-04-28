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
    EvolveCloudFunctionObject

Description
    Basis for the other function objects here. Evaluates the most common options

SourceFiles
    EvolveCloudFunctionObject.cpp

Contributors/Copyright:
    2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef EvolveCloudFunctionObject_H
#define EvolveCloudFunctionObject_H

#include "functionObject.hpp"
#include "dictionary.hpp"

#include "autoPtr.hpp"
#include "objectRegistry.hpp"
#include "volFields.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"
#include "switchableNotImplemented.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class EvolveCloudFunctionObject Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class EvolveCloudFunctionObject
:
    public functionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    EvolveCloudFunctionObject(const EvolveCloudFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const EvolveCloudFunctionObject&);

    // Private data
    autoPtr<CloudType> cloud_;

    //- Input dictionary
    dictionary dict_;

    //- name of the region
    word regionName_;

    //- the registry
    const objectRegistry& obr_;

    //- name of the cloud
    word cloudName_;

    //- direction of gravity
    dimensionedVector g_;

protected:

    //- get a reference to the field
    template <class FieldType >
    const FieldType &getField(const word &fieldName);

    const word &cloudName() const { return cloudName_; }

    autoPtr<CloudType> &cloud() { return cloud_; }

    const dimensionedVector &g() const { return g_; }

    const objectRegistry &obr() const { return obr_; }

public:

    // Constructors

    //- Construct from components
    EvolveCloudFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );


    // Member Functions

    // //- start is called at the start of the time-loop
    // virtual bool start();

    //- execute is called at each ++ or += of the time-loop
    virtual bool execute(bool forceWrite);

    //- Read and set the function object if it's data has changed
    virtual bool read(const dictionary& dict);

    // make functionObject happy but fail because this is not an implementation
    virtual void updateMesh(const mapPolyMesh&) {
        switchableNotImplemented(
            "EvolveCloudFunctionObject::updateMesh",
            dict_
        );
    }

    // make functionObject happy but fail because this is not an implementation
    virtual void movePoints(const polyMesh&) {
        switchableNotImplemented(
            "EvolveCloudFunctionObject::movePoints",
            dict_
        );
    }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

#include "uniformDimensionedFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
EvolveCloudFunctionObject<CloudType>::EvolveCloudFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    functionObject(
        name
    ),
    dict_(dict),
    regionName_(
        dict_.found("region")
        ? dict_.lookup("region")
        : polyMesh::defaultRegion
    ),
    obr_(t.lookupObject<objectRegistry>(regionName_)),
    cloudName_(
        dict.lookup("cloudName")
    ),
    g_("dummy",dimless,vector::zero)
{
    if(dict_.found("g")) {
        dimensionedVector newG(dict_.lookup("g"));
        g_.dimensions().reset(newG.dimensions());
        g_=newG;
    } else {
        const Time &runTime=t;
        //        const fvMesh &mesh=dynamicCast<const fvMesh &>(obr_);
        const fvMesh &mesh=dynamic_cast<const fvMesh &>(obr_);

        #include "readGravitationalAcceleration.hpp"
        g_.dimensions().reset(g.dimensions());
        g_=g;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// template<class CloudType>
// bool EvolveCloudFunctionObject<CloudType>::start()
// {
//     return true;
// }

template<class CloudType>
template <class FieldType >
const FieldType &EvolveCloudFunctionObject<CloudType>::getField(const word &fieldName)
{
    return obr_.lookupObject<FieldType>(
        word(
            dict_.lookup(fieldName)
        )
    );
}

template<class CloudType>
bool EvolveCloudFunctionObject<CloudType>::execute(bool forceWrite)
{
    cloud_->evolve();

    if(
        obr().time().outputTime()
        ||
        forceWrite
    ) {
        Info << "Writing cloud " << cloud_->name() << endl;
        cloud_->write();
    }

    return true;
}

template<class CloudType>
bool EvolveCloudFunctionObject<CloudType>::read(const dictionary& dict)
{
    if(dict_!=dict) {
        WarningInFunction
            << "Can't change the cloud of " << this->name()
            << " during the run"
            << endl;
    }

    return true;
}

} // namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
