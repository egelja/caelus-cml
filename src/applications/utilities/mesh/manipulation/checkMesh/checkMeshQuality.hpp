#include "polyMesh.hpp"

namespace CML
{
    class surfaceWriter;

    label checkMeshQuality
    (
        const polyMesh&,
        const dictionary&,
        const autoPtr<surfaceWriter>&
    );
}
