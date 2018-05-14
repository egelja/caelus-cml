#include "checkMeshQuality.hpp"
#include "polyMesh.hpp"
#include "cellSet.hpp"
#include "faceSet.hpp"
#include "motionSmoother.hpp"
#include "surfaceWriter.hpp"
#include "checkTools.hpp"

CML::label CML::checkMeshQuality
(
    const polyMesh& mesh,
    const dictionary& dict,
    const autoPtr<surfaceWriter>& writer
)
{
    label noFailedChecks = 0;

    {
        faceSet faces(mesh, "meshQualityFaces", mesh.nFaces()/100+1);
        motionSmoother::checkMesh(false, mesh, dict, faces);

        label nFaces = returnReduce(faces.size(), sumOp<label>());

        if (nFaces > 0)
        {
            noFailedChecks++;

            Info<< "  <<Writing " << nFaces
                << " faces in error to set " << faces.name() << endl;
            faces.instance() = mesh.pointsInstance();
            faces.write();
            if (writer.valid())
            {
                mergeAndWrite(writer(), faces);
            }
        }
    }

    return noFailedChecks;
}
