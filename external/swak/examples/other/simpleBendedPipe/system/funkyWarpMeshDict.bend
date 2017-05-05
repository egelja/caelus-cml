/*---------------------------------------------------------------------------*
Caelus 5.10
Web:   www.caelus-cml.com
\*---------------------------------------------------------------------------*/
FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    location    "system";
    object      funkyWarpMeshDict;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

mode set;

relative false;

variables (
    "span=max(pts().y)-min(pts().y);"
    "x1=interpolateToPoint(pi);"
    "x2=interpolateToPoint(pi)+x1;"
    "center=vector(x1,interpolateToPoint(1),pts().z);"
    "radius=mag(pts().y-center.y);"
    "s=pts().x-x1;"
    "bend=vector(x1,center.y,center.z)+radius*vector(sin(s),-cos(s),interpolateToPoint(0));"
    "back=vector(x1-(pts().x-x2),center.y-(pts().y-center.y),pts().z);"
    "wobble=vector(interpolateToPoint(0),span*interpolateToPoint(0.3)*cos(interpolateToPoint(4)*pts().x+interpolateToPoint(pi/2)),interpolateToPoint(0));"
);

expression "pts().x<x1 ? pts()+wobble : (pts().x<x2 ? bend : back-wobble)";

// ************************************************************************* //
