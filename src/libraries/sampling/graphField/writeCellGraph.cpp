#include "writeCellGraph.hpp"
#include "volFields.hpp"
#include "fvMesh.hpp"
#include "graph.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void writeCellGraph
(
    const volScalarField& vsf,
    const word& graphFormat
)
{
    fileName path(vsf.time().path()/"graphs"/vsf.time().timeName());
    mkDir(path);

    graph
    (
        vsf.name(),
        "x",
        vsf.name(),
        vsf.mesh().C().internalField().component(vector::X),
        vsf.internalField()
    ).write(path/vsf.name(), graphFormat);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
