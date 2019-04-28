/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2016 Applied CCM
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
    CML::fv::limitedSnGrad

Description
    Central-difference snGrad scheme with limited non-orthogonal correction.

    The limiter is controlled by a coefficient with a value between 0 and 1
    which when 0 switches the correction off and the scheme behaves as
    uncorrectedSnGrad, when set to 1 the full correction is applied and the
    scheme behaves as correctedSnGrad and when set to 0.5 the limiter is
    calculated such that the non-orthogonal contribution does not exceed the
    orthogonal part.

    Format:
        limited \<corrected scheme\> \<coefficient\>;

        or

        limited \<coefficient\>;  // Backward compatibility

SourceFiles
    limitedSnGrad.C

\*---------------------------------------------------------------------------*/

#ifndef limitedSnGrad_H
#define limitedSnGrad_H

#include "snGradScheme.hpp"
#include "fv.hpp"
#include "limitedSnGrad.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "correctedSnGrad.hpp"
#include "localMax.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

/*---------------------------------------------------------------------------*\
                 Class limitedSnGrad Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class limitedSnGrad
:
    public snGradScheme<Type>
{
    // Private data

        tmp<snGradScheme<Type> > correctedScheme_;

        scalar limitCoeff_;


    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const limitedSnGrad&);

        //- Lookup function for the corrected to support backward compatibility
        //  of dictionary specification
        tmp<snGradScheme<Type> > lookupCorrectedScheme(Istream& is)
        {
            token nextToken(is);

            if (nextToken.isNumber())
            {
                limitCoeff_ = nextToken.number();
                return tmp<snGradScheme<Type> >
                (
                    new correctedSnGrad<Type>(this->mesh())
                );
            }
            else
            {
                is.putBack(nextToken);
                tmp<snGradScheme<Type> > tcorrectedScheme
                (
                    fv::snGradScheme<Type>::New(this->mesh(), is)
                );

                is >> limitCoeff_;

                return tcorrectedScheme;
            }
        }


public:

    //- Runtime type information
    TypeName("limited");


    // Constructors

        //- Construct from mesh
        limitedSnGrad(const fvMesh& mesh)
        :
            snGradScheme<Type>(mesh),
            correctedScheme_(new correctedSnGrad<Type>(this->mesh())),
            limitCoeff_(1)
        {}


        //- Construct from mesh and data stream
        limitedSnGrad(const fvMesh& mesh, Istream& is)
        :
            snGradScheme<Type>(mesh),
            correctedScheme_(lookupCorrectedScheme(is))
        {
            if (limitCoeff_ < 0 || limitCoeff_ > 1)
            {
                FatalIOErrorInFunction(is)
                    << "limitCoeff is specified as " << limitCoeff_
                    << " but should be >= 0 && <= 1"
                    << exit(FatalIOError);
            }
        }


    //- Destructor
    virtual ~limitedSnGrad();


    // Member Functions

        //- Return the interpolation weighting factors for the given field
        virtual tmp<surfaceScalarField> deltaCoeffs
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const
        {
            return this->mesh().nonOrthDeltaCoeffs();
        }

        //- Return true if this scheme uses an explicit correction
        virtual bool corrected() const
        {
            return true;
        }

        //- Return the explicit correction to the limitedSnGrad
        //  for the given field
        virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        correction(const GeometricField<Type, fvPatchField, volMesh>&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
limitedSnGrad<Type>::~limitedSnGrad()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
limitedSnGrad<Type>::correction
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    const GeometricField<Type, fvsPatchField, surfaceMesh> corr
    (
        correctedScheme_().correction(vf)
    );

    const surfaceScalarField limiter
    (
        min
        (
            limitCoeff_
           *mag(snGradScheme<Type>::snGrad(vf, deltaCoeffs(vf), "orthSnGrad"))
           /(
                (1 - limitCoeff_)*mag(corr)
              + dimensionedScalar("small", corr.dimensions(), SMALL)
            ),
            dimensionedScalar("one", dimless, 1.0)
        )
    );

    if (fv::debug)
    {
        Info<< "limitedSnGrad :: limiter min: " << min(limiter.internalField())
            << " max: "<< max(limiter.internalField())
            << " avg: " << average(limiter.internalField()) << endl;
    }

    return limiter*corr;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
