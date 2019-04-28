/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::fileFormats::FTRsurfaceFormat

Description
    Reading of the (now deprecated and infrequently used)
    CML Trisurface Format.

Deprecated
    Other formats are better. (deprecated Mar 2009)


\*---------------------------------------------------------------------------*/

#ifndef FTRsurfaceFormat_H
#define FTRsurfaceFormat_H

#include "IFstream.hpp"
#include "MeshedSurface.hpp"
#include "UnsortedMeshedSurface.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fileFormats
{

/*---------------------------------------------------------------------------*\
                      Class FTRsurfaceFormat Declaration
\*---------------------------------------------------------------------------*/

template<class Face>
class FTRsurfaceFormat
:
    public UnsortedMeshedSurface<Face>
{
    // Private classes

        //- read compatibility for ftr patch definitions
        class ftrPatch
        {
            //- Name of patch
            word name_;

            //- Type of patch (ignored since it is usually "empty")
            word type_;

        public:

            const word& name() const
            {
                return name_;
            }

            friend Istream& operator>>(Istream& is, ftrPatch& p)
            {
                is >> p.name_ >> p.type_;
                return is;
            }
			//CAELUSFIX
			friend Ostream& operator<<(Ostream& os, const ftrPatch& p)
            {
                os << p.name_ << p.type_<<endl;
                return os;
            }
        };


    // Private Member Functions

        //- Disallow default bitwise copy construct
        FTRsurfaceFormat(const FTRsurfaceFormat<Face>&);

        //- Disallow default bitwise assignment
        void operator=(const FTRsurfaceFormat<Face>&);


public:

    // Constructors

        //- Construct from file name
        FTRsurfaceFormat(const fileName&);


    // Selectors

        //- Read file and return surface
        static autoPtr<UnsortedMeshedSurface<Face> > New(const fileName& name)
        {
            return autoPtr<UnsortedMeshedSurface<Face> >
            (
                new FTRsurfaceFormat<Face>(name)
            );
        }


    //- Destructor
    virtual ~FTRsurfaceFormat()
    {}


    // Member Functions

        //- Read from file
        virtual bool read(const fileName&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fileFormats
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "Keyed.hpp"
#include "IFstream.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Face>
CML::fileFormats::FTRsurfaceFormat<Face>::FTRsurfaceFormat
(
    const fileName& filename
)
{
    read(filename);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Face>
bool CML::fileFormats::FTRsurfaceFormat<Face>::read
(
    const fileName& filename
)
{
    this->clear();

    IFstream is(filename);
    if (!is.good())
    {
        FatalErrorInFunction
            << "Cannot read file " << filename
            << exit(FatalError);
    }

    List<ftrPatch> ftrPatches(is);

    // points read directly
    is >> this->storedPoints();

    // triFaces read with attached keys
    List< Keyed<triFace> > facesRead(is);

    List<Face>  faceLst(facesRead.size());
    List<label> zoneIds(facesRead.size());

    // disentangle faces/keys - already triangulated
    forAll(facesRead, faceI)
    {
        // unfortunately cannot transfer to save memory
        faceLst[faceI] = facesRead[faceI];
        zoneIds[faceI] = facesRead[faceI].key();
    }

    this->storedFaces().transfer(faceLst);
    this->storedZoneIds().transfer(zoneIds);
    facesRead.clear();

    // change ftrPatch into surfZoneIdentifier
    List<surfZoneIdentifier> newZones(ftrPatches.size());
    forAll(newZones, zoneI)
    {
        newZones[zoneI] = surfZoneIdentifier
        (
            ftrPatches[zoneI].name(),
            zoneI
        );
    }

    this->storedZoneToc().transfer(newZones);
    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
