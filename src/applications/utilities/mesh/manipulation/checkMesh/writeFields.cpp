#include "writeFields.hpp"
#include "volFields.hpp"
#include "polyMeshTools.hpp"
#include "zeroGradientFvPatchFields.hpp"
#include "syncTools.hpp"
#include "unitConversion.hpp"

using namespace CML;

void maxFaceToCell
(
    const scalarField& faceData,
    volScalarField& cellData
)
{
    const cellList& cells = cellData.mesh().cells();

    scalarField& cellFld = cellData.internalField();

    cellFld = -GREAT;
    forAll(cells, cellI)
    {
        const cell& cFaces = cells[cellI];
        forAll(cFaces, i)
        {
            cellFld[cellI] = max(cellFld[cellI], faceData[cFaces[i]]);
        }
    }

    forAll(cellData.boundaryField(), patchI)
    {
        fvPatchScalarField& fvp = cellData.boundaryField()[patchI];

        fvp = fvp.patch().patchSlice(faceData);
    }
    cellData.correctBoundaryConditions();
}


void minFaceToCell
(
    const scalarField& faceData,
    volScalarField& cellData
)
{
    const cellList& cells = cellData.mesh().cells();

    scalarField& cellFld = cellData.internalField();

    cellFld = GREAT;
    forAll(cells, cellI)
    {
        const cell& cFaces = cells[cellI];
        forAll(cFaces, i)
        {
            cellFld[cellI] = min(cellFld[cellI], faceData[cFaces[i]]);
        }
    }

    forAll(cellData.boundaryField(), patchI)
    {
        fvPatchScalarField& fvp = cellData.boundaryField()[patchI];

        fvp = fvp.patch().patchSlice(faceData);
    }
    cellData.correctBoundaryConditions();
}


void CML::writeFields
(
    const fvMesh& mesh,
    const HashSet<word>& selectedFields
)
{
    if (selectedFields.empty())
    {
        return;
    }

    Info<< "Writing fields with mesh quality parameters" << endl;

    if (selectedFields.found("nonOrthoAngle"))
    {
        //- Face based orthogonality
        const scalarField faceOrthogonality
        (
            polyMeshTools::faceOrthogonality
            (
                mesh,
                mesh.faceAreas(),
                mesh.cellCentres()
            )
        );

        //- Face based angle
        const scalarField nonOrthoAngle
        (
            radToDeg
            (
                CML::acos(min(scalar(1.0), faceOrthogonality))
            )
        );

        //- Cell field - max of either face
        volScalarField cellNonOrthoAngle
        (
            IOobject
            (
                "nonOrthoAngle",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedScalar("angle", dimless, 0),
            calculatedFvPatchScalarField::typeName
        );
        //- Take max
        maxFaceToCell(nonOrthoAngle, cellNonOrthoAngle);
        Info<< "    Writing non-orthogonality (angle) to "
            << cellNonOrthoAngle.name() << endl;
        cellNonOrthoAngle.write();
    }

    if (selectedFields.found("faceWeight"))
    {
        const scalarField faceWeights
        (
            polyMeshTools::faceWeights
            (
                mesh,
                mesh.faceCentres(),
                mesh.faceAreas(),
                mesh.cellCentres()
            )
        );

        volScalarField cellWeights
        (
            IOobject
            (
                "faceWeight",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedScalar("weight", dimless, 0),
            calculatedFvPatchScalarField::typeName
        );
        //- Take min
        minFaceToCell(faceWeights, cellWeights);
        Info<< "    Writing face interpolation weights (0..0.5) to "
            << cellWeights.name() << endl;
        cellWeights.write();
    }


    // Skewness
    // ~~~~~~~~

    if (selectedFields.found("skewness"))
    {
        //- Face based skewness
        const scalarField faceSkewness
        (
            polyMeshTools::faceSkewness
            (
                mesh,
                mesh.points(),
                mesh.faceCentres(),
                mesh.faceAreas(),
                mesh.cellCentres()
            )
        );

        //- Cell field - max of either face
        volScalarField cellSkewness
        (
            IOobject
            (
                "skewness",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedScalar("skewness", dimless, 0),
            calculatedFvPatchScalarField::typeName
        );
        //- Take max
        maxFaceToCell(faceSkewness, cellSkewness);
        Info<< "    Writing face skewness to " << cellSkewness.name() << endl;
        cellSkewness.write();
    }


    // cellDeterminant
    // ~~~~~~~~~~~~~~~

    if (selectedFields.found("cellDeterminant"))
    {
        volScalarField cellDeterminant
        (
            IOobject
            (
                "cellDeterminant",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE,
                false
            ),
            mesh,
            dimensionedScalar("cellDeterminant", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
        );
        cellDeterminant.internalField() =
            primitiveMeshTools::cellDeterminant
            (
                mesh,
                mesh.geometricD(),
                mesh.faceAreas(),
                syncTools::getInternalOrCoupledFaces(mesh)
            );
        cellDeterminant.correctBoundaryConditions();
        Info<< "    Writing cell determinant to "
            << cellDeterminant.name() << endl;
        cellDeterminant.write();
    }


    // Aspect ratio
    // ~~~~~~~~~~~~
    if (selectedFields.found("aspectRatio"))
    {
        volScalarField aspectRatio
        (
            IOobject
            (
                "aspectRatio",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE,
                false
            ),
            mesh,
            dimensionedScalar("aspectRatio", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
        );


        scalarField cellOpenness;
        polyMeshTools::cellClosedness
        (
            mesh,
            mesh.geometricD(),
            mesh.faceAreas(),
            mesh.cellVolumes(),
            cellOpenness,
            aspectRatio.internalField()
        );

        aspectRatio.correctBoundaryConditions();
        Info<< "    Writing aspect ratio to " << aspectRatio.name() << endl;
        aspectRatio.write();
    }


    // cell type
    // ~~~~~~~~~
    if (selectedFields.found("cellShapes"))
    {
        volScalarField shape
        (
            IOobject
            (
                "cellShapes",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE,
                false
            ),
            mesh,
            dimensionedScalar("cellShapes", dimless, 0),
            zeroGradientFvPatchScalarField::typeName
        );
        const cellShapeList& cellShapes = mesh.cellShapes();
        forAll(cellShapes, cellI)
        {
            const cellModel& model = cellShapes[cellI].model();
            shape[cellI] = model.index();
        }
        shape.correctBoundaryConditions();
        Info<< "    Writing cell shape (hex, tet etc.) to " << shape.name()
            << endl;
        shape.write();
    }

    if (selectedFields.found("cellVolume"))
    {
        volScalarField V
        (
            IOobject
            (
                "cellVolume",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE,
                false
            ),
            mesh,
            dimensionedScalar("cellVolume", dimVolume, 0),
            calculatedFvPatchScalarField::typeName
        );
        V.internalField() = mesh.V();
        Info<< "    Writing cell volume to " << V.name() << endl;
        V.write();
    }

    if (selectedFields.found("cellVolumeRatio"))
    {
        const scalarField faceVolumeRatio
        (
            polyMeshTools::volRatio
            (
                mesh,
                mesh.V()
            )
        );

        volScalarField cellVolumeRatio
        (
            IOobject
            (
                "cellVolumeRatio",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            mesh,
            dimensionedScalar("cellVolumeRatio", dimless, 0),
            calculatedFvPatchScalarField::typeName
        );
        //- Take min
        minFaceToCell(faceVolumeRatio, cellVolumeRatio);
        Info<< "    Writing cell volume ratio to "
            << cellVolumeRatio.name() << endl;
        cellVolumeRatio.write();
    }

    Info<< endl;
}
