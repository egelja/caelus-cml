#include "writePatchGraph.hpp"
#include "volFields.hpp"
#include "fvMesh.hpp"
#include "graph.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void writePatchGraph
(
    const volScalarField& vsf,
    const label patchLabel,
    const direction d,
    const word& graphFormat
)
{
    graph
    (
        vsf.name(),
        "position",
        vsf.name(),
        vsf.mesh().boundary()[patchLabel].Cf().component(d),
        vsf.boundaryField()[patchLabel]
    ).write(vsf.time().timePath()/vsf.name(), graphFormat);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
