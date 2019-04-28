/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

\*---------------------------------------------------------------------------*/

#include "wavesPorosityModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    defineTypeNameAndDebug(wavesPorosityModel, 0);
    defineRunTimeSelectionTable(wavesPorosityModel, wavesPorosityModel);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


CML::wavesPorosityModel::wavesPorosityModel
(
	const fvMesh& mesh
)
:
    porosity_
    (
        IOobject
        (
            "porosity",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("NULL", dimless, 1.0),
        "zeroGradient"
    )
{
}


CML::wavesPorosityModel::~wavesPorosityModel()
{}


autoPtr<wavesPorosityModel> wavesPorosityModel::New
(
    const fvMesh& mesh
)
{
    word wavesPorosityModelTypeName;

    // Enclose the creation of the dictionary to ensure it is deleted before
    // the actual porosity model is created
    {
        if (mesh.thisDb().foundObject<IOdictionary>("waveProperties"))
        {
        	mesh.thisDb().lookupObject<IOdictionary>("waveProperties")
        		.lookup("porosityModel") >> wavesPorosityModelTypeName;
        }
        else
        {
        	IOdictionary wp
        	(
        	    IOobject
        	    (
        	        "waveProperties",
        	        mesh.time().constant(),
        	        mesh,
        	        IOobject::MUST_READ,
        	        IOobject::NO_WRITE
        	    )
        	);

        	wp.lookup("porosityModel") >> wavesPorosityModelTypeName;
        }
    }

    wavesPorosityModelConstructorTable::iterator cstrIter =
    		wavesPorosityModelConstructorTablePtr_->find(wavesPorosityModelTypeName);

    if (cstrIter == wavesPorosityModelConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown porosity model of type " << wavesPorosityModelTypeName
            << endl << endl
            << "Valid porosity models are :" << endl
            << wavesPorosityModelConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<wavesPorosityModel>(cstrIter()(mesh));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
