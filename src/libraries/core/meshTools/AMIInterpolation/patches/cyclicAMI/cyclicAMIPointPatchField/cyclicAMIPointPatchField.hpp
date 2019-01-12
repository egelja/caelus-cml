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
    CML::cyclicAMIPointPatchField

Description
    Cyclic AMI front and back plane patch field

SourceFiles
    cyclicAMIPointPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef cyclicAMIPointPatchField_H
#define cyclicAMIPointPatchField_H

#include "coupledPointPatchField.hpp"
#include "cyclicAMIPointPatch.hpp"
#include "PrimitivePatchInterpolation_.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                  Class cyclicAMIPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cyclicAMIPointPatchField
:
    public coupledPointPatchField<Type>
{
    // Private data

        //- Local reference cast into the cyclicAMI patch
        const cyclicAMIPointPatch& cyclicAMIPatch_;

        //- Owner side patch interpolation pointer
        mutable autoPtr<PrimitivePatchInterpolation<primitivePatch> > ppiPtr_;

        //- Neighbour side patch interpolation pointer
        mutable autoPtr<PrimitivePatchInterpolation<primitivePatch> >
            nbrPpiPtr_;


    // Private Member Functions

        //- Owner side patch interpolation
        const PrimitivePatchInterpolation<primitivePatch>& ppi() const
        {
            if (!ppiPtr_.valid())
            {
                ppiPtr_.reset
                (
                    new PrimitivePatchInterpolation<primitivePatch>
                    (
                        cyclicAMIPatch_.cyclicAMIPatch()
                    )
                );
            }

            return ppiPtr_();
        }

        //- Neighbour side patch interpolation
        const PrimitivePatchInterpolation<primitivePatch>& nbrPpi() const
        {
            if (!nbrPpiPtr_.valid())
            {
                nbrPpiPtr_.reset
                (
                    new PrimitivePatchInterpolation<primitivePatch>
                    (
                        cyclicAMIPatch_.cyclicAMIPatch().neighbPatch()
                    )
                );
            }

            return nbrPpiPtr_();
        }


public:

    //- Runtime type information
    TypeName(cyclicAMIPointPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        cyclicAMIPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        cyclicAMIPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        cyclicAMIPointPatchField
        (
            const cyclicAMIPointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new cyclicAMIPointPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        cyclicAMIPointPatchField
        (
            const cyclicAMIPointPatchField<Type>&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual autoPtr<pointPatchField<Type> > clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new cyclicAMIPointPatchField<Type>
                (
                    *this, iF
                )
            );
        }


    // Member functions

        // Constraint handling

            //- Return the constraint type this pointPatchField implements
            virtual const word& constraintType() const
            {
                return cyclicAMIPointPatch::typeName;
            }


        // Cyclic AMI coupled interface functions

            //- Does the patch field perform the transfromation
            virtual bool doTransform() const
            {
                return
                   !(
                        cyclicAMIPatch_.parallel()
                     || pTraits<Type>::rank == 0
                    );
            }

            //- Return face transformation tensor
            virtual const tensorField& forwardT() const
            {
                return cyclicAMIPatch_.forwardT();
            }

            //- Return neighbour-cell transformation tensor
            virtual const tensorField& reverseT() const
            {
                return cyclicAMIPatch_.reverseT();
            }


        // Evaluation functions

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            )
            {}

            //- Complete swap of patch point values and add to local values
            virtual void swapAddSeparated
            (
                const Pstream::commsTypes commsType,
                Field<Type>&
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "Swap.hpp"
#include "transformField.hpp"
#include "pointFields.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::cyclicAMIPointPatchField<Type>::cyclicAMIPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    coupledPointPatchField<Type>(p, iF),
    cyclicAMIPatch_(refCast<const cyclicAMIPointPatch>(p)),
    ppiPtr_(nullptr),
    nbrPpiPtr_(nullptr)
{}


template<class Type>
CML::cyclicAMIPointPatchField<Type>::cyclicAMIPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    coupledPointPatchField<Type>(p, iF, dict),
    cyclicAMIPatch_(refCast<const cyclicAMIPointPatch>(p)),
    ppiPtr_(nullptr),
    nbrPpiPtr_(nullptr)
{
    if (!isType<cyclicAMIPointPatch>(p))
    {
        FatalIOErrorInFunction(dict)
            << "patch " << this->patch().index() << " not cyclicAMI type. "
            << "Patch type = " << p.type()
            << exit(FatalIOError);
    }
}


template<class Type>
CML::cyclicAMIPointPatchField<Type>::cyclicAMIPointPatchField
(
    const cyclicAMIPointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    coupledPointPatchField<Type>(ptf, p, iF, mapper),
    cyclicAMIPatch_(refCast<const cyclicAMIPointPatch>(p)),
    ppiPtr_(nullptr),
    nbrPpiPtr_(nullptr)
{
    if (!isType<cyclicAMIPointPatch>(this->patch()))
    {
        FatalErrorInFunction
            << "Field type does not correspond to patch type for patch "
            << this->patch().index() << "." << endl
            << "Field type: " << typeName << endl
            << "Patch type: " << this->patch().type()
            << exit(FatalError);
    }
}


template<class Type>
CML::cyclicAMIPointPatchField<Type>::cyclicAMIPointPatchField
(
    const cyclicAMIPointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    coupledPointPatchField<Type>(ptf, iF),
    cyclicAMIPatch_(ptf.cyclicAMIPatch_),
    ppiPtr_(nullptr),
    nbrPpiPtr_(nullptr)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::cyclicAMIPointPatchField<Type>::swapAddSeparated
(
    const Pstream::commsTypes,
    Field<Type>& pField
) const
{
    if (cyclicAMIPatch_.cyclicAMIPatch().owner())
    {
        // We inplace modify pField. To prevent the other side (which gets
        // evaluated at a later date) using already changed values we do
        // all swaps on the side that gets evaluated first.

        // Get neighbouring pointPatch
        const cyclicAMIPointPatch& nbrPatch = cyclicAMIPatch_.neighbPatch();

        // Get neighbouring pointPatchField		
	    const GeometricField<Type, pointPatchField, pointMesh>& fld =
            refCast<const GeometricField<Type, pointPatchField, pointMesh> >
            (
                this->dimensionedInternalField()
            );		

        const cyclicAMIPointPatchField<Type>& nbr =
            refCast<const cyclicAMIPointPatchField<Type> >
            (
                fld.boundaryField()[nbrPatch.index()]
            );


        Field<Type> ptFld(this->patchInternalField(pField));
        Field<Type> nbrPtFld(nbr.patchInternalField(pField));


        if (doTransform())
        {
            const tensor& forwardT = this->forwardT()[0];
            const tensor& reverseT = this->reverseT()[0];

            transform(ptFld, reverseT, ptFld);
            transform(nbrPtFld, forwardT, nbrPtFld);
        }

        // convert point field to face field, AMI interpolate, then
        // face back to point
        {
            // add neighbour side contribution to owner
            Field<Type> nbrFcFld(nbrPpi().pointToFaceInterpolate(nbrPtFld));

            // interpolate to owner
            if (cyclicAMIPatch_.cyclicAMIPatch().applyLowWeightCorrection())
            {
                Field<Type> fcFld(ppi().pointToFaceInterpolate(ptFld));

                nbrFcFld =
                    cyclicAMIPatch_.cyclicAMIPatch().interpolate
                    (
                        nbrFcFld,
                        fcFld
                    );
            }
            else
            {
                nbrFcFld =
                    cyclicAMIPatch_.cyclicAMIPatch().interpolate(nbrFcFld);
            }

            // add to internal field
            this->addToInternalField
            (
                pField,
                ppi().faceToPointInterpolate(nbrFcFld)()
            );
        }

        {
            // add owner side contribution to neighbour
            Field<Type> fcFld(ppi().pointToFaceInterpolate(ptFld));

            // interpolate to neighbour
            if (cyclicAMIPatch_.cyclicAMIPatch().applyLowWeightCorrection())
            {
                Field<Type> nbrFcFld(nbrPpi().pointToFaceInterpolate(nbrPtFld));

                fcFld =
                    cyclicAMIPatch_.cyclicAMIPatch().neighbPatch().interpolate
                    (
                        fcFld,
                        nbrFcFld
                    );
            }
            else
            {
                fcFld =
                    cyclicAMIPatch_.cyclicAMIPatch().neighbPatch().interpolate
                    (
                        fcFld
                    );
            }

            // add to internal field
            nbr.addToInternalField
            (
                pField,
                nbrPpi().faceToPointInterpolate(fcFld)()
            );
        }
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
