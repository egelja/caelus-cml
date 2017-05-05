/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2010 OpenCFD Ltd.
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
    CML::CloudProxyForReaderParticle

Description

SourceFiles
    CloudProxyForReaderParticle.cpp

Contributors/Copyright:
    2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef CloudProxyForReaderParticle_H
#define CloudProxyForReaderParticle_H

#include "CloudProxyForParticle.hpp"

#include "ReaderParticleCloud.hpp"

#include "HashPtrTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class CloudProxyForReaderParticle Declaration
\*---------------------------------------------------------------------------*/

class CloudProxyForReaderParticle
:
    public CloudProxyForParticle<ReaderParticleCloud>
{
    //- Disallow default bitwise assignment
    void operator=(const CloudProxyForReaderParticle&);

    //- Disallow copy construct
    CloudProxyForReaderParticle(const CloudProxyForReaderParticle &);

    const ReaderParticleCloud &readerCloud_;

    template <typename T>
    void internalAddFields(const wordList &);

public:
    TypeName("CloudProxyForReaderParticle");

    // Constructors

    //- Construct from components
    CloudProxyForReaderParticle
    (
        const cloud &
    );

    // Destructor

    virtual ~CloudProxyForReaderParticle();

    virtual tmp<Field<scalar> > getScalarField(
        const word &name
    ) const;

    virtual tmp<Field<vector> > getVectorField(
        const word &name
    ) const;

    virtual tmp<Field<tensor> > getTensorField(
        const word &name
    ) const;

    virtual tmp<Field<symmTensor> > getSymmTensorField(
        const word &name
    ) const;

    virtual tmp<Field<sphericalTensor> > getSphericalTensorField(
        const word &name
    ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

template <typename T>
void CloudProxyForReaderParticle::internalAddFields(const wordList &names)
{
    forAll(names,i)
    {
	this->addField<T>(
            names[i],
            "Read from disk"
        );
    }
}

} // namespace end


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
