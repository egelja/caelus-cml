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
    CML::MeshedSurfaceProxy

Description
    A proxy for writing MeshedSurface, UnsortedMeshedSurface and surfMesh
    to various file formats.


\*---------------------------------------------------------------------------*/

#ifndef MeshedSurfaceProxy_H
#define MeshedSurfaceProxy_H

#include "pointField.hpp"
#include "face.hpp"
#include "triFace.hpp"

#include "surfZoneList.hpp"
#include "surfaceFormatsCore.hpp"
#include "runTimeSelectionTables.hpp"
#include "memberFunctionSelectionTables.hpp"
#include "HashSet.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Face> class MeshedSurface;

/*---------------------------------------------------------------------------*\
                     Class MeshedSurfaceProxy Declaration
\*---------------------------------------------------------------------------*/

template<class Face>
class MeshedSurfaceProxy
:
    public fileFormats::surfaceFormatsCore
{
    //- Private data

        const pointField& points_;

        const List<Face>& faces_;

        const List<surfZone>& zones_;

        const List<label>& faceMap_;


public:

    // Public typedefs

        //- Face type used
        typedef Face FaceType;


    // Static

        //- Runtime type information
        ClassName("MeshedSurfaceProxy");

        //- The file format types that can be written via MeshedSurfaceProxy
        static wordHashSet writeTypes();

        //- Can this file format type be written via MeshedSurfaceProxy?
        static bool canWriteType(const word& ext, const bool verbose=false);


    // Constructors

        //- Construct from component references
        MeshedSurfaceProxy
        (
            const pointField&,
            const List<Face>&,
            const List<surfZone>& = List<surfZone>(),
            const List<label>& faceMap = List<label>()
        );


    //- Destructor
    virtual ~MeshedSurfaceProxy();


    // Member Function Selectors

        declareMemberFunctionSelectionTable
        (
            void,
            MeshedSurfaceProxy,
            write,
            fileExtension,
            (
                const fileName& name,
                const MeshedSurfaceProxy<Face>& surf
            ),
            (name, surf)
        );

        //- Write to file
        static void write(const fileName&, const MeshedSurfaceProxy<Face>&);


    // Member Functions

        // Access

            //- Return const access to the points
            inline const pointField& points() const
            {
                return points_;
            }

            //- Return const access to the faces
            inline const List<Face>& faces() const
            {
                return faces_;
            }

            //- Const access to the surface zones.
            //  If zones are defined, they must be contiguous and cover the
            //  entire surface
            inline const List<surfZone>& surfZones() const
            {
                return zones_;
            }

            //- Const access to the faceMap, zero-sized when unused
            inline const List<label>& faceMap() const
            {
                return faceMap_;
            }

            //- Use faceMap?
            inline bool useFaceMap() const
            {
                return faceMap_.size() == faces_.size();
            }

        // Write

            //- Generic write routine. Chooses writer based on extension.
            virtual void write(const fileName& name) const
            {
                write(name, *this);
            }

            //- Write to database
            virtual void write(const Time&, const word& surfName = "") const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "Time.hpp"
#include "surfMesh.hpp"
#include "OFstream.hpp"
#include "ListOps.hpp"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

template<class Face>
CML::wordHashSet CML::MeshedSurfaceProxy<Face>::writeTypes()
{
    return wordHashSet(*writefileExtensionMemberFunctionTablePtr_);
}


template<class Face>
bool CML::MeshedSurfaceProxy<Face>::canWriteType
(
    const word& ext,
    const bool verbose
)
{
    return fileFormats::surfaceFormatsCore::checkSupport
    (
        writeTypes(), ext, verbose, "writing"
    );
}


template<class Face>
void CML::MeshedSurfaceProxy<Face>::write
(
    const fileName& name,
    const MeshedSurfaceProxy& surf
)
{
    if (debug)
    {
        Info<< "MeshedSurfaceProxy::write"
            "(const fileName&, const MeshedSurfaceProxy&) : "
            "writing to " << name
            << endl;
    }

    word ext = name.ext();

    typename writefileExtensionMemberFunctionTable::iterator mfIter =
        writefileExtensionMemberFunctionTablePtr_->find(ext);

    if (mfIter == writefileExtensionMemberFunctionTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown file extension " << ext << nl << nl
            << "Valid types are :" << endl
            << writeTypes()
            << exit(FatalError);
    }

    mfIter()(name, surf);
}


template<class Face>
void CML::MeshedSurfaceProxy<Face>::write
(
    const Time& t,
    const word& surfName
) const
{
    // the surface name to be used
    word name(surfName.size() ? surfName : surfaceRegistry::defaultName);

    if (debug)
    {
        Info<< "MeshedSurfaceProxy::write"
            "(const Time&, const word&) : "
            "writing to " << name
            << endl;
    }


    // the local location
    const fileName objectDir
    (
        t.timePath()/surfaceRegistry::prefix/name/surfMesh::meshSubDir
    );

    if (!isDir(objectDir))
    {
        mkDir(objectDir);
    }


    // write surfMesh/points
    {
        pointIOField io
        (
            IOobject
            (
                "points",
                t.timeName(),
                surfMesh::meshSubDir,
                t,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            )
        );

        OFstream os
        (
            objectDir/io.name(),
            t.writeFormat(),
            IOstream::currentVersion,
            t.writeCompression()
        );

        io.writeHeader(os);

        os  << this->points();

        io.writeEndDivider(os);
    }


    // write surfMesh/faces
    {
        faceCompactIOList io
        (
            IOobject
            (
                "faces",
                t.timeName(),
                surfMesh::meshSubDir,
                t,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            )
        );

        OFstream os
        (
            objectDir/io.name(),
            t.writeFormat(),
            IOstream::currentVersion,
            t.writeCompression()
        );
        io.writeHeader(os);

        if (this->useFaceMap())
        {
            // this is really a bit annoying (and wasteful) but no other way
            os  << reorder(this->faceMap(), this->faces());
        }
        else
        {
            os  << this->faces();
        }

        io.writeEndDivider(os);
    }


    // write surfMesh/surfZones
    {
        surfZoneIOList io
        (
            IOobject
            (
                "surfZones",
                t.timeName(),
                surfMesh::meshSubDir,
                t,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            )
        );

        // write as ascii
        OFstream os(objectDir/io.name());
        io.writeHeader(os);

        os  << this->surfZones();

        io.writeEndDivider(os);
    }

}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Face>
CML::MeshedSurfaceProxy<Face>::MeshedSurfaceProxy
(
    const pointField& pointLst,
    const List<Face>& faceLst,
    const List<surfZone>& zoneLst,
    const List<label>& faceMap
)
:
    points_(pointLst),
    faces_(faceLst),
    zones_(zoneLst),
    faceMap_(faceMap)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Face>
CML::MeshedSurfaceProxy<Face>::~MeshedSurfaceProxy()
{}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
