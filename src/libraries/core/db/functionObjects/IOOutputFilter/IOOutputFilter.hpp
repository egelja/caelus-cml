/*---------------------------------------------------------------------------* \
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
    CML::IOOutputFilter

Description
    IOdictionary wrapper around OutputFilter to allow them to read from
    their associated dictionaries.

Note
    The IOobject or the objectRegistry will normally have to be
    derived from a fvMesh for a subsequent cast (within OutputFilter)
    to work correctly.


\*---------------------------------------------------------------------------*/

#ifndef IOOutputFilter_H
#define IOOutputFilter_H

#include "IOdictionary.hpp"
#include "pointFieldFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                       Class IOOutputFilter Declaration
\*---------------------------------------------------------------------------*/

template<class OutputFilter>
class IOOutputFilter
:
    public IOdictionary,
    public OutputFilter
{
    // Private Member Functions

        // Disallow default bitwise copy construct and assignment
        IOOutputFilter(const IOOutputFilter&);
        void operator=(const IOOutputFilter&);


public:

    // Constructors

        //- Construct from an IOobject for IOdictionary
        //  Allow the possibility to load fields from files
        IOOutputFilter
        (
            const word& outputFilterName,
            const IOobject& ioDict,
            const bool loadFromFile = false
        );

        //- Construct for given objectRegistry and dictionary
        //  Allow dictionary to be optional
        //  Allow the possibility to load fields from files
        IOOutputFilter
        (
            const word& outputFilterName,
            const objectRegistry&,
            const fileName& dictName = OutputFilter::typeName() + "Dict",
            const IOobject::readOption rOpt = IOobject::MUST_READ_IF_MODIFIED,
            const bool loadFromFile = false
        );


    //- Destructor
    virtual ~IOOutputFilter();


    // Member Functions

        //- Return name
        virtual const word& name() const
        {
            return IOdictionary::name();
        }

        //- Read output filter properties
        virtual bool read();

        //- Sample and write
        virtual void write();

        //- Update for changes of mesh
        virtual void updateMesh(const mapPolyMesh& mpm)
        {
            read();
            OutputFilter::updateMesh(mpm);
        }

        //- Update for changes of mesh
        virtual void movePoints(const pointField& points)
        {
            read();
            OutputFilter::movePoints(points);
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "Time.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class OutputFilter>
CML::IOOutputFilter<OutputFilter>::IOOutputFilter
(
    const word& outputFilterName,
    const IOobject& ioDict,
    const bool readFromFiles
)
:
    IOdictionary(ioDict),
    OutputFilter(outputFilterName, ioDict.db(), *this, readFromFiles)
{}


template<class OutputFilter>
CML::IOOutputFilter<OutputFilter>::IOOutputFilter
(
    const word& outputFilterName,
    const objectRegistry& obr,
    const fileName& dictName,
    const IOobject::readOption rOpt,
    const bool readFromFiles
)
:
    IOdictionary
    (
        IOobject
        (
            dictName,
            obr.time().system(),
            obr,
            rOpt,
            IOobject::NO_WRITE
        )
    ),
    OutputFilter(outputFilterName, obr, *this, readFromFiles)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class OutputFilter>
CML::IOOutputFilter<OutputFilter>::~IOOutputFilter()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class OutputFilter>
bool CML::IOOutputFilter<OutputFilter>::read()
{
    if (regIOobject::read())
    {
        OutputFilter::read(*this);
        return true;
    }
    else
    {
        return false;
    }
}


template<class OutputFilter>
void CML::IOOutputFilter<OutputFilter>::write()
{
    OutputFilter::write();
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
