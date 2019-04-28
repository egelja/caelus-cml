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
    CML::ReaderParticle

Description

SourceFiles

Contributors/Copyright:
    2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef ReaderParticle_H
#define ReaderParticle_H

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

#include "particle.hpp"

#include "IOstream.hpp"
#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class ReaderParticle Declaration
\*---------------------------------------------------------------------------*/

    class ReaderParticleCloud;

    typedef particle ReaderParticleBaseClass;

class ReaderParticle
:
    public ReaderParticleBaseClass
{

public:
    TypeName("ReaderParticle");

    // Constructors

        //- Construct from components
        ReaderParticle
        (
            const polyMesh& mesh,
            const barycentric& position,
            const label celli,
            const label tetFaceI,
            const label tetPtI

        )
        :
            ReaderParticleBaseClass(mesh, position, celli, tetFaceI, tetPtI)
        {}

        ReaderParticle
        (
            const polyMesh& mesh,
            const vector& position,
            const label cellI,
            bool doCellFacePt = true
        )
        :
            ReaderParticleBaseClass(mesh, position, cellI)
        {}

        //- Construct from Istream
        ReaderParticle
        (
            const polyMesh& mesh,
            Istream& is,
            bool readFields = true
        )
        :
            ReaderParticleBaseClass(mesh, is, readFields)
        {}


        //- Construct as copy
        ReaderParticle(const ReaderParticle& p)
        :
            ReaderParticleBaseClass(p)
        {}

        //- Construct and return a clone
        virtual autoPtr<ReaderParticleBaseClass> clone() const
        {
            return autoPtr<ReaderParticleBaseClass>(new ReaderParticle(*this));
        }

        static void readFields (ReaderParticleCloud &c);

        static void writeFields (const Cloud<ReaderParticle> &c);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
