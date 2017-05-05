# swak

## License

swak is a port of the swak4Foam library and utility suite from OpenFOAM to Caelus. It is currently compatible with swak4Foam and OpenFOAM.

It is released under the GPL: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

## Copyright and Contributors

### Original Author

Bernhard Gschaider (bgschaid@ice-sf.at)

### Contributors

In alphabetical order of the surname

- **Martin Beaudoin:** `trackDictionary` in `simpleFunctionObjects`
- **Martin Becker:** The `potentialPitzDaily`-case (demonstrating a
    problem with `groovyBC`)
- **Oliver Borm:** `patchMassFlowAverage` in `simpleFunctionObjects`
- **Hrvoje Jasak:** Fixes to compile on Intel and CLang
- **Peter Keller:** `sprinklerInlet`-case
- **Martin Kroeger:** `mtv` surface writer in `simpleFunctionObjects`
- **Andreas Otto:** fixed the `circulatingSplash`-case
- **Alexey Petrov:** `pythonFlu`-integration
- **Bruno Santos:** -   Compilation with Intel compiler and Mingw
    - Rewrite of `mybison` and `myflex` to allow parallel compilation with `WM_COMPPROCS`
    - Generation of dependencies for files that need Bison generated tab.hh files. This allows not having race conditions during parallel compilation
- **Chris Sideroff and Darrin Stephens:** Port to Caelus

According to the commits in the `mercurial`-repository (and the repositories of the projects from which swak emerged) contributors are (ordered by the year of their first contribution):

-   2006-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
-   2008 Hannes Kroeger (hannes@kroegeronline.net)
-   2008-2009, 2012 Martin Beaudoin, Hydro-Quebec (beaudoin.martin@ireq.ca)
-   2010 Marianne Mataln <mmataln@ice-sf>
-   2010 Oliver Borm (oli.borm@web.de)
-   2011 Alexey Petrov <alexey.petrov.nnov@gmail.com>
-   2011 Petr Vita <petr.vita@unileoben.ac.at>
-   2012-2014 Bruno Santos <wyldckat@gmail.com>
-   2013 Georg Reiss <georg.reiss@ice-sf.at>
-   2014 Hrvoje Jasak <h.jasak@wikki.co.uk>
-   2015 Chris Sideroff <c.sideroff@appliedccm.ca>
-   2015 Darrin Stephens <d.stephens@appliedccm.com.au>

### Documentation
Refer to files in ./documentation or visit <http://openfoamwiki.net/index.php/contrib/swak4Foam>

Current version: based off of swak4Foam v 0.3.1

## Libraries

- **swakParsers**: The basis of swak. Contains the expression parsers with the logic to access the Caelus data-structures. None of the other software compiles without it.

- **groovyBC**: A boundary condition that allows abitrary expressions on the field file

- **groovyStandardBCs**: A collection of boundary conditions that give standard boundary conditions the possibility to use an expression for the coefficients.

- **simpleFunctionObjects**: A collection of function objects that was previously separately available at <http://openfoamwiki.net/index.php/Contrib_simpleFunctionObjects>. Provides consistent output of values (on patches and fields) and more.

- **simpleLagrangianFunctionObjects**: Function objects that allow the easy addition of lagrangian clouds to a case.

- **simpleSearchableSurfaces**: Additional `searchableSurfaces` (for use in `snappyHexMesh`) which include boolean operations for other surfaces and coordinate transformations.

- **simpleSwakFunctionObjects**: Function objects based on the `simpleFunctionObjects` library and a prerequisite for compiling it. Evaluate expressions and output the results.

- **swakFunctionObjects**: Function objects that have no additional requirements. Mainly used for manipulating and creating fields with expressions.

- **swakFvOptions**: Adds expression parsing to `fvOptions`.

- **swakLagrangianParser**: Parser for calculating expressions on clouds of Lagrangian particles.

- **swakSourceFields**: These classes allow to manipulate the solution. To use these the solver has to be modified.

- **swakTopoSources**: `topoSources` for `cellSet` and `faceSet`. Can be used with the
`cellSet` and `faceSet` utilities.

- **functionPlugins**: See `./documentation` directory for further information.

## Utilities

- **`funkySetFields`**: Utility that allows creation and manipulation of files with expressions.

- **`funkySetBoundaryField`**: Sets any field on a boundary to a non-uniform value based on an expression. Acts without deeper understanding of the underlying boundary condition.

- **`replayTransientBC`**: Quickly test whether a groovyBC gives the expected results. Writes the specified fields with the applied boundary condition but doesn't do anything else. Can be used for other BCs as well.

- **`funkyDoCalc`**: Evaluate expressions listed in a dictionary using data that is found and prints summarized data (min, max, average, sum) to the screen.

- **`calcNonUniformOffsetsForMapped`**: Calculate the `offsets`-entry in the `polyMesh/boundary` file according to the specification in a dictionary. Only needed if you have mapped patches and the regular uniform offset is not enough for your purposes.

- **`fieldReport`**: Quickly calculate some quantitative analysis (minimum, maximum, average etc ) on a field on the disc (internal field but also patches, sets, zones, &#x2026;)

- **`funkySetLagrangianField`**: Calculate fields for a lagrangian cloud (or setting it up from scratch)

- **`funkyWarpMesh`**: Warp a mesh with new coordinates calculated for the points.

