/*----------------------- -*- C++ -*- ---------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
-------------------------------------------------------------------------------
License
    This file is part of swak.

    swak is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    swak is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with swak.  If not, see <http://www.gnu.org/licenses/>.

Description


Contributors/Copyright:
    2006-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
    2013 Georg Reiss <georg.reiss@ice-sf.at>
    2014 Hrvoje Jasak <h.jasak@wikki.co.uk>

\*---------------------------------------------------------------------------*/

%skeleton "lalr1.cc"
/* %require "2.1a" */
%defines
%define "parser_class_name" "FieldValueExpressionParser"

// make reentrant to allow sub-parsers
// %define api.pure // not possible with C++?
%pure-parser

%{
#include <uLabel.hpp>
#include <label.hpp>
#include <volFields.hpp>
#include <surfaceFields.hpp>
#include <fvcGrad.hpp>
#include <fvcCurl.hpp>
#include <fvcMagSqrGradGrad.hpp>
#include <fvcSnGrad.hpp>
#include <fvcDiv.hpp>
#include <fvcSurfaceIntegrate.hpp>
#include <fvcReconstruct.hpp>
#include <fvcAverage.hpp>
#include <surfaceInterpolate.hpp>
#include <fvcLaplacian.hpp>
#include <fvcDdt.hpp>
#include <fvcD2dt2.hpp>
#include <fvcMeshPhi.hpp>
#include <fvcFlux.hpp>

#include <fvMatrix.hpp>

#include <functional>
#include <cmath>

    namespace CML {
        class FieldValueExpressionDriver;
    }

    using CML::FieldValueExpressionDriver;

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

#include "FieldValuePluginFunction.hpp"

%}

%name-prefix="parserField"

%parse-param {void * scanner}
%parse-param { FieldValueExpressionDriver& driver }
%parse-param { int start_token }
%parse-param { int numberOfFunctionChars }
%lex-param {void * scanner}
%lex-param { FieldValueExpressionDriver& driver }
%lex-param { int &start_token }
%lex-param { int &numberOfFunctionChars }

%locations
%initial-action
{
	     // Initialize the initial location.
	     //     @$.begin.filename = @$.end.filename = &driver.file;
    numberOfFunctionChars=0;
};

%debug
%error-verbose

%union
{
    CML::scalar val;
    CML::label integer;
    CML::vector *vec;
    CML::tensor *ten;
    CML::symmTensor *yten;
    CML::sphericalTensor *hten;
    CML::word *name;
    CML::volVectorField *vfield;
    CML::volScalarField *sfield;
    CML::volTensorField *tfield;
    CML::volSymmTensorField *yfield;
    CML::volSphericalTensorField *hfield;
    CML::surfaceScalarField *fsfield;
    CML::surfaceVectorField *fvfield;
    CML::surfaceTensorField *ftfield;
    CML::surfaceSymmTensorField *fyfield;
    CML::surfaceSphericalTensorField *fhfield;
    CML::pointScalarField *psfield;
    CML::pointVectorField *pvfield;
    CML::pointTensorField *ptfield;
    CML::pointSymmTensorField *pyfield;
    CML::pointSphericalTensorField *phfield;
};

%{
#include "FieldValueExpressionDriverYY.hpp"
#include "FieldValueExpressionDriverLogicalTemplates.hpp"

#include "swakChecks.hpp"
namespace CML {
template<class T>
autoPtr<T> FieldValueExpressionDriver::evaluatePluginFunction(
    const word &name,
    const parserField::location &loc,
    int &scanned
) {
    if(debug || traceParsing()) {
        Info << "Excuting plugin-function " << name << " ( returning type "
            << pTraits<T>::typeName << ") on " << this->content() << " position "
            << loc.end.column-1 << endl;
    }

    autoPtr<FieldValuePluginFunction> theFunction(
        FieldValuePluginFunction::New(
            *this,
            name
        )
    );

    //    scanned+=1;

    autoPtr<T> result(
        theFunction->evaluate<T>(
            this->content().substr(
                loc.end.column-1
            ),
            scanned
        ).ptr()
    );

    if(debug || traceParsing()) {
        Info << "Scanned: " << scanned << endl;
    }

    return result;
}
}

%}

%token <name>   TOKEN_LINE  "timeline"
%token <name>   TOKEN_LOOKUP  "lookup"
%token <name>   TOKEN_SID   "scalarID"
%token <name>  TOKEN_VID   "vectorID"
%token <name>  TOKEN_TID   "tensorID"
%token <name>  TOKEN_YID   "symmTensorID"
%token <name>  TOKEN_HID   "sphericalTensorID"
%token <name> TOKEN_FSID  "faceScalarID"
%token <name> TOKEN_FVID  "faceVectorID"
%token <name>  TOKEN_FTID   "faceTensorID"
%token <name>  TOKEN_FYID   "faceSymmTensorID"
%token <name>  TOKEN_FHID   "faceSphericalTensorID"
%token <name> TOKEN_PSID  "pointScalarID"
%token <name> TOKEN_PVID  "pointVectorID"
%token <name>  TOKEN_PTID   "pointTensorID"
%token <name>  TOKEN_PYID   "pointSymmTensorID"
%token <name>  TOKEN_PHID   "pointSphericalTensorID"
%token <name>   TOKEN_FUNCTION_SID   "F_scalarID"
%token <name>   TOKEN_FUNCTION_FSID   "F_faceScalarID"
%token <name>   TOKEN_FUNCTION_PSID   "F_pointScalarID"
%token <name>   TOKEN_FUNCTION_VID   "F_vectorID"
%token <name>   TOKEN_FUNCTION_FVID   "F_faceVectorID"
%token <name>   TOKEN_FUNCTION_PVID   "F_pointVectorID"
%token <name>   TOKEN_FUNCTION_TID   "F_tensorID"
%token <name>   TOKEN_FUNCTION_FTID   "F_faceTensorID"
%token <name>   TOKEN_FUNCTION_PTID   "F_pointTensorID"
%token <name>   TOKEN_FUNCTION_YID   "F_symmTensorID"
%token <name>   TOKEN_FUNCTION_FYID   "F_faceSymmTensorID"
%token <name>   TOKEN_FUNCTION_PYID   "F_pointSymmTensorID"
%token <name>   TOKEN_FUNCTION_HID   "F_sphericalTensorID"
%token <name>   TOKEN_FUNCTION_FHID   "F_faceSphericalTensorID"
%token <name>   TOKEN_FUNCTION_PHID   "F_pointSphericalTensorID"
%token <name>   TOKEN_FUNCTION_LID   "F_logicalID"
%token <name>   TOKEN_FUNCTION_FLID   "F_faceLogicalID"
%token <name>   TOKEN_FUNCTION_PLID   "F_pointLogicalID"

%token <name>   TOKEN_OTHER_MESH_ID   "F_otherMeshID"
%token <name>   TOKEN_OTHER_MESH_SID   "F_otherMeshScalarID"
%token <name>   TOKEN_OTHER_MESH_VID   "F_otherMeshVectorID"
%token <name>   TOKEN_OTHER_MESH_TID   "F_otherMeshTensorID"
%token <name>   TOKEN_OTHER_MESH_HID   "F_otherMeshSymmTensorID"
%token <name>   TOKEN_OTHER_MESH_YID   "F_otherMeshSphericalTensorID"

%token <name> TOKEN_SETID "cellSetID"
%token <name> TOKEN_ZONEID "cellZoneID"
%token <name> TOKEN_FSETID "faceSetID"
%token <name> TOKEN_FZONEID "faceZoneID"
%token <name> TOKEN_PSETID "pointSetID"
%token <name> TOKEN_PZONEID "pointZoneID"
%token <name> TOKEN_PATCHID "patchID"
%token <val>    TOKEN_NUM   "number"
%token <integer>    TOKEN_INT   "integer"
%token <vec>    TOKEN_VEC   "vector"
%token <ten>    TOKEN_TEN   "tensor"
%token <yten>    TOKEN_YTEN   "symmTensor"
%token <hten>    TOKEN_HTEN   "sphericalTensor"
%type  <val>    scalar      "sexpression"
%type  <sfield>    exp        "expression"
%type  <sfield>    lexp       "lexpression"
%type  <fsfield>   flexp       "flexpression"
%type  <psfield>   plexp       "plexpression"
%type  <vfield>    vexp       "vexpression"
%type  <fsfield>   fsexp      "fsexpression"
%type  <fvfield>   fvexp      "fvexpression"
%type  <psfield>   psexp      "psexpression"
%type  <pvfield>   pvexp      "pvexpression"
%type  <tfield>    texp       "texpression"
%type  <yfield>    yexp       "yexpression"
%type  <hfield>    hexp       "hexpression"
%type  <ftfield>    ftexp       "ftexpression"
%type  <fyfield>    fyexp       "fyexpression"
%type  <fhfield>    fhexp       "fhexpression"
%type  <ptfield>    ptexp       "ptexpression"
%type  <pyfield>    pyexp       "pyexpression"
%type  <phfield>    phexp       "phexpression"
%type  <vfield>    vector
%type  <fvfield>    fvector
%type  <tfield>    tensor
%type  <yfield>    symmTensor
%type  <hfield>    sphericalTensor
%type  <ftfield>    ftensor
%type  <fyfield>    fsymmTensor
%type  <fhfield>    fsphericalTensor
%type  <pvfield>    pvector
%type  <ptfield>    ptensor
%type  <pyfield>    psymmTensor
%type  <phfield>    psphericalTensor

%type  <sfield> evaluateScalarFunction;
%type <fsfield> evaluateFaceScalarFunction;
%type <psfield> evaluatePointScalarFunction;
%type  <vfield> evaluateVectorFunction;
%type <fvfield> evaluateFaceVectorFunction;
%type <pvfield> evaluatePointVectorFunction;
%type  <tfield> evaluateTensorFunction;
%type <ftfield> evaluateFaceTensorFunction;
%type <ptfield> evaluatePointTensorFunction;
%type  <yfield> evaluateSymmTensorFunction;
%type <fyfield> evaluateFaceSymmTensorFunction;
%type <pyfield> evaluatePointSymmTensorFunction;
%type  <hfield> evaluateSphericalTensorFunction;
%type <fhfield> evaluateFaceSphericalTensorFunction;
%type <phfield> evaluatePointSphericalTensorFunction;
%type  <sfield> evaluateLogicalFunction;
%type <fsfield> evaluateFaceLogicalFunction;
%type <psfield> evaluatePointLogicalFunction;

%token START_DEFAULT
%token START_VOL_SCALAR_COMMA
%token START_VOL_SCALAR_CLOSE
%token START_VOL_VECTOR_COMMA
%token START_VOL_VECTOR_CLOSE
%token START_VOL_TENSOR_COMMA
%token START_VOL_TENSOR_CLOSE
%token START_VOL_YTENSOR_COMMA
%token START_VOL_YTENSOR_CLOSE
%token START_VOL_HTENSOR_COMMA
%token START_VOL_HTENSOR_CLOSE
%token START_VOL_LOGICAL_COMMA
%token START_VOL_LOGICAL_CLOSE
%token START_SURFACE_SCALAR_COMMA
%token START_SURFACE_SCALAR_CLOSE
%token START_SURFACE_VECTOR_COMMA
%token START_SURFACE_VECTOR_CLOSE
%token START_SURFACE_TENSOR_COMMA
%token START_SURFACE_TENSOR_CLOSE
%token START_SURFACE_YTENSOR_COMMA
%token START_SURFACE_YTENSOR_CLOSE
%token START_SURFACE_HTENSOR_COMMA
%token START_SURFACE_HTENSOR_CLOSE
%token START_SURFACE_LOGICAL_COMMA
%token START_SURFACE_LOGICAL_CLOSE
%token START_POINT_SCALAR_COMMA
%token START_POINT_SCALAR_CLOSE
%token START_POINT_VECTOR_COMMA
%token START_POINT_VECTOR_CLOSE
%token START_POINT_TENSOR_COMMA
%token START_POINT_TENSOR_CLOSE
%token START_POINT_YTENSOR_COMMA
%token START_POINT_YTENSOR_CLOSE
%token START_POINT_HTENSOR_COMMA
%token START_POINT_HTENSOR_CLOSE
%token START_POINT_LOGICAL_COMMA
%token START_POINT_LOGICAL_CLOSE

%token START_CLOSE_ONLY
%token START_COMMA_ONLY

%token TOKEN_LAST_FUNCTION_CHAR
%token TOKEN_IN_FUNCTION_CHAR

%token TOKEN_VECTOR
%token TOKEN_TENSOR
%token TOKEN_SYMM_TENSOR
%token TOKEN_SPHERICAL_TENSOR

%token TOKEN_TRUE
%token TOKEN_FALSE

%token TOKEN_x
%token TOKEN_y
%token TOKEN_z

%token TOKEN_xx
%token TOKEN_xy
%token TOKEN_xz
%token TOKEN_yx
%token TOKEN_yy
%token TOKEN_yz
%token TOKEN_zx
%token TOKEN_zy
%token TOKEN_zz
%token TOKEN_ii

%token TOKEN_unitTensor
%token TOKEN_pi
%token TOKEN_rand
%token TOKEN_randFixed
%token TOKEN_id
%token TOKEN_cpu
%token TOKEN_weight
%token TOKEN_randNormal
%token TOKEN_randNormalFixed
%token TOKEN_position
%token TOKEN_fposition
%token TOKEN_fprojection
%token TOKEN_pposition
%token TOKEN_face
%token TOKEN_area
%token TOKEN_volume
%token TOKEN_dist
%token TOKEN_distToPatch
%token TOKEN_nearDist
%token TOKEN_rdist

%token TOKEN_set
%token TOKEN_zone
%token TOKEN_fset
%token TOKEN_fzone
%token TOKEN_pset
%token TOKEN_pzone

%token TOKEN_onPatch
%token TOKEN_internalFace

%token TOKEN_div
%token TOKEN_grad
%token TOKEN_curl
%token TOKEN_snGrad
%token TOKEN_magSqrGradGrad;
%token TOKEN_laplacian
%token TOKEN_ddt
%token TOKEN_oldTime
%token TOKEN_d2dt2
%token TOKEN_meshPhi
%token TOKEN_flux

%token TOKEN_integrate
%token TOKEN_surfSum
%token TOKEN_interpolate
%token TOKEN_interpolateToPoint
%token TOKEN_interpolateToCell
%token TOKEN_faceAverage
%token TOKEN_reconstruct

%token TOKEN_surf
%token TOKEN_point

%token TOKEN_deltaT
%token TOKEN_time

%token TOKEN_pow
%token TOKEN_log
%token TOKEN_exp
%token TOKEN_mag
%token TOKEN_magSqr
%token TOKEN_sin
%token TOKEN_cos
%token TOKEN_tan
%token TOKEN_min
%token TOKEN_max
%token TOKEN_minPosition
%token TOKEN_maxPosition
%token TOKEN_sum
%token TOKEN_average
%token TOKEN_sqr
%token TOKEN_sqrt

%token TOKEN_transpose
%token TOKEN_diag
%token TOKEN_tr
%token TOKEN_dev
%token TOKEN_symm
%token TOKEN_skew
%token TOKEN_det
%token TOKEN_cof
%token TOKEN_inv
%token TOKEN_sph
%token TOKEN_twoSymm
%token TOKEN_dev2
%token TOKEN_eigenValues
%token TOKEN_eigenVectors

%token TOKEN_log10
%token TOKEN_asin
%token TOKEN_acos
%token TOKEN_atan
%token TOKEN_sinh
%token TOKEN_cosh
%token TOKEN_tanh
%token TOKEN_asinh
%token TOKEN_acosh
%token TOKEN_atanh
%token TOKEN_erf
%token TOKEN_erfc
%token TOKEN_lgamma
%token TOKEN_besselJ0
%token TOKEN_besselJ1
%token TOKEN_besselY0
%token TOKEN_besselY1

%token TOKEN_sign
%token TOKEN_pos
%token TOKEN_neg

%left '?' ':'
%left TOKEN_OR
%left TOKEN_AND
%left TOKEN_EQ TOKEN_NEQ
%left TOKEN_LEQ TOKEN_GEQ '<' '>'
%left '-' '+'
%left '%' '*' '/' '&' '^'
%left TOKEN_NEG TOKEN_NOT TOKEN_HODGE
// %right '^'
%left '.'

%destructor          {} <val> <integer>
%destructor          { delete $$; } <name> <vec> <ten> <yten> <hten>
       <vfield> <sfield> <tfield> <yfield> <hfield>
       <fvfield> <fsfield> <ftfield> <fyfield> <fhfield>
       <pvfield> <psfield> <ptfield> <pyfield> <phfield>

%printer             { debug_stream () << $$; } <val> <integer>
%printer             { debug_stream () << *$$; } <name>
%printer             {
    CML::OStringStream buff;
    buff << *$$; debug_stream () << buff.str().c_str();
} <vec> <ten> <yten> <hten>
%printer             { debug_stream () << $$->name().c_str(); }
       <vfield> <sfield> <tfield> <yfield> <hfield>
       <fvfield> <fsfield> <ftfield> <fyfield> <fhfield>
       <pvfield> <psfield> <ptfield> <pyfield> <phfield>


%%
%start switch_start;

switch_start: switch_expr
              { driver.parserLastPos()=@1.end.column; }
;

switch_expr:      START_DEFAULT unit
                | START_VOL_SCALAR_COMMA exp ','
                  {
                      driver.setResult($2,false,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_VOL_SCALAR_CLOSE exp ')'
                  {
                      driver.setResult($2,false,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_VOL_VECTOR_COMMA vexp ','
                  {
                      driver.setResult($2,false,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_VOL_VECTOR_CLOSE vexp ')'
                  {
                      driver.setResult($2,false,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_VOL_TENSOR_COMMA texp ','
                  {
                      driver.setResult($2,false,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_VOL_TENSOR_CLOSE texp ')'
                  {
                      driver.setResult($2,false,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_VOL_YTENSOR_COMMA yexp ','
                  {
                      driver.setResult($2,false,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_VOL_YTENSOR_CLOSE yexp ')'
                  {
                      driver.setResult($2,false,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_VOL_HTENSOR_COMMA hexp ','
                  {
                      driver.setResult($2,false,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_VOL_HTENSOR_CLOSE hexp ')'
                  {
                      driver.setResult($2,false,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_VOL_LOGICAL_COMMA lexp ','
                  {
                      driver.setLogicalResult($2,false,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_VOL_LOGICAL_CLOSE lexp ')'
                  {
                      driver.setLogicalResult($2,false,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_SURFACE_SCALAR_COMMA fsexp ','
                  {
                      driver.setResult($2,true,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_SURFACE_SCALAR_CLOSE fsexp ')'
                  {
                      driver.setResult($2,true,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_SURFACE_VECTOR_COMMA fvexp ','
                  {
                      driver.setResult($2,true,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_SURFACE_VECTOR_CLOSE fvexp ')'
                  {
                      driver.setResult($2,true,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_SURFACE_TENSOR_COMMA ftexp ','
                  {
                      driver.setResult($2,true,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_SURFACE_TENSOR_CLOSE ftexp ')'
                  {
                      driver.setResult($2,true,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_SURFACE_YTENSOR_COMMA fyexp ','
                  {
                      driver.setResult($2,true,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_SURFACE_YTENSOR_CLOSE fyexp ')'
                  {
                      driver.setResult($2,true,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_SURFACE_HTENSOR_COMMA fhexp ','
                  {
                      driver.setResult($2,true,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_SURFACE_HTENSOR_CLOSE fhexp ')'
                  {
                      driver.setResult($2,true,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_SURFACE_LOGICAL_COMMA flexp ','
                  {
                      driver.setLogicalResult($2,true,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_SURFACE_LOGICAL_CLOSE flexp ')'
                  {
                      driver.setLogicalResult($2,true,false);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_POINT_SCALAR_COMMA psexp ','
                  {
                      driver.setResult($2,false,true);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_POINT_SCALAR_CLOSE psexp ')'
                  {
                      driver.setResult($2,false,true);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_POINT_VECTOR_COMMA pvexp ','
                  {
                      driver.setResult($2,false,true);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_POINT_VECTOR_CLOSE pvexp ')'
                  {
                      driver.setResult($2,false,true);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_POINT_TENSOR_COMMA ptexp ','
                  {
                      driver.setResult($2,false,true);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_POINT_TENSOR_CLOSE ptexp ')'
                  {
                      driver.setResult($2,false,true);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_POINT_YTENSOR_COMMA pyexp ','
                  {
                      driver.setResult($2,false,true);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_POINT_YTENSOR_CLOSE pyexp ')'
                  {
                      driver.setResult($2,false,true);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_POINT_HTENSOR_COMMA phexp ','
                  {
                      driver.setResult($2,false,true);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_POINT_HTENSOR_CLOSE phexp ')'
                  {
                      driver.setResult($2,false,true);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_POINT_LOGICAL_COMMA plexp ','
                  {
                      driver.setLogicalResult($2,false,true);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_POINT_LOGICAL_CLOSE plexp ')'
                  {
                      driver.setLogicalResult($2,false,true);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_CLOSE_ONLY ')'
                  {
                      driver.parserLastPos()=@2.end.column-1;
                      YYACCEPT;
                  }
                | START_COMMA_ONLY ','
                  {
                      driver.parserLastPos()=@2.end.column-1;
                      YYACCEPT;
                  }
;

unit:   exp                     { driver.setResult($1,false,false);  }
        | vexp                  { driver.setResult($1,false,false);  }
        | texp                  { driver.setResult($1,false,false);  }
        | yexp                  { driver.setResult($1,false,false);  }
        | hexp                  { driver.setResult($1,false,false);  }
        | lexp                  { driver.setLogicalResult($1,false,false); }
        | fsexp                 { driver.setResult($1,true,false);  }
        | fvexp                 { driver.setResult($1,true,false);  }
        | ftexp                 { driver.setResult($1,true,false);  }
        | fyexp                 { driver.setResult($1,true,false);  }
        | fhexp                 { driver.setResult($1,true,false);  }
        | flexp                 { driver.setLogicalResult($1,true,false); }
        | psexp                 { driver.setResult($1,false,true);  }
        | pvexp                 { driver.setResult($1,false,true);  }
        | ptexp                 { driver.setResult($1,false,true);  }
        | pyexp                 { driver.setResult($1,false,true);  }
        | phexp                 { driver.setResult($1,false,true);  }
        | plexp                 { driver.setLogicalResult($1,false,true); }
;

vectorComponentSwitch: /* empty rule */{ driver.startVectorComponent(); }
;

tensorComponentSwitch: /* empty rule */{ driver.startTensorComponent(); }
;

eatCharactersSwitch: /* empty rule */{ driver.startEatCharacters(); }
;


vexp:   vector                                    { $$ = $1; }
        | vexp '+' vexp		          {
            sameSize($1,$3); $$ = new CML::volVectorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | exp '*' vexp	        	          {
            sameSize($1,$3); $$ = new CML::volVectorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | vexp '*' exp		                  {
            sameSize($1,$3); $$ = new CML::volVectorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp '&' vexp 	                  {
            sameSize($1,$3); $$ = new CML::volVectorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | vexp '&' texp 	                  {
            sameSize($1,$3); $$ = new CML::volVectorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '&' vexp 	                  {
            sameSize($1,$3); $$ = new CML::volVectorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | vexp '&' yexp 	                  {
            sameSize($1,$3); $$ = new CML::volVectorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp '&' vexp 	                  {
            sameSize($1,$3); $$ = new CML::volVectorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | vexp '&' hexp 	                  {
            sameSize($1,$3); $$ = new CML::volVectorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | vexp '/' exp 		           {
            sameSize($1,$3); $$ = new CML::volVectorField(*$1 / *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | vexp '^' vexp 		           {
            sameSize($1,$3); $$ = new CML::volVectorField(*$1 ^ *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | vexp '-' vexp 		           {
            sameSize($1,$3); $$ = new CML::volVectorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | '-' vexp %prec TOKEN_NEG                 {
            $$ = new CML::volVectorField(-*$2);
            delete $2;
            driver.setCalculatedPatches(*$$);
          }
        | '*' texp %prec TOKEN_HODGE 	        {
            $$ = new CML::volVectorField(*(*$2));
            delete $2;
            driver.setCalculatedPatches(*$$);
          }
        | '*' yexp %prec TOKEN_HODGE 	        {
            $$ = new CML::volVectorField(*(*$2));
            delete $2;
            driver.setCalculatedPatches(*$$);
          }
        | '(' vexp ')'		                   { $$ = $2; }
        | TOKEN_eigenValues '(' texp ')'       {
            $$ = new CML::volVectorField(CML::eigenValues(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_eigenValues '(' yexp ')'       {
            $$ = new CML::volVectorField(CML::eigenValues(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp '.' tensorComponentSwitch  TOKEN_x                              {
            $$ = driver.makeVectorField(
                $1->component(CML::tensor::XX)(),
                $1->component(CML::tensor::XY)(),
                $1->component(CML::tensor::XZ)()
            ).ptr();
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | texp '.' tensorComponentSwitch  TOKEN_y                              {
            $$ = driver.makeVectorField(
                $1->component(CML::tensor::YX)(),
                $1->component(CML::tensor::YY)(),
                $1->component(CML::tensor::YZ)()
            ).ptr();
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | texp '.' tensorComponentSwitch  TOKEN_z                              {
            $$ = driver.makeVectorField(
                $1->component(CML::tensor::ZX)(),
                $1->component(CML::tensor::ZY)(),
                $1->component(CML::tensor::ZZ)()
            ).ptr();
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_diag '(' texp ')'       {
            $$ = driver.makeVectorField(
                $3->component(CML::tensor::XX)(),
                $3->component(CML::tensor::YY)(),
                $3->component(CML::tensor::ZZ)()
            ).ptr();
            delete $3;
          }
        | TOKEN_diag '(' yexp ')'       {
            $$ = driver.makeVectorField(
                $3->component(CML::symmTensor::XX)(),
                $3->component(CML::symmTensor::YY)(),
                $3->component(CML::symmTensor::ZZ)()
            ).ptr();
            delete $3;
          }
        | lexp '?' vexp ':' vexp                   {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_position '(' ')'                   {
            $$ = driver.makePositionField().ptr();
          }
        | TOKEN_laplacian '(' vexp ')'               {
            $$ = new CML::volVectorField(CML::fvc::laplacian(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' fsexp ',' vexp ')'   {
            $$ = new CML::volVectorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        // | TOKEN_laplacian '(' fvexp ',' vexp ')'   {
        //     $$ = new CML::volVectorField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //     driver.setCalculatedPatches(*$$);
        //   }
        | TOKEN_laplacian '(' ftexp ',' vexp ')'   {
            $$ = new CML::volVectorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_laplacian '(' fyexp ',' vexp ')'   {
            $$ = new CML::volVectorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        // | TOKEN_laplacian '(' fhexp ',' vexp ')'   {
        //     $$ = new CML::volVectorField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //     driver.setCalculatedPatches(*$$);
        //   }
        | TOKEN_laplacian '(' exp ',' vexp ')'   {
            $$ = new CML::volVectorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        // | TOKEN_laplacian '(' vexp ',' vexp ')'   {
        //     $$ = new CML::volVectorField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //     driver.setCalculatedPatches(*$$);
        //   }
        | TOKEN_laplacian '(' texp ',' vexp ')'   {
            $$ = new CML::volVectorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_laplacian '(' yexp ',' vexp ')'   {
            $$ = new CML::volVectorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        // | TOKEN_laplacian '(' hexp ',' vexp ')'   {
        //     $$ = new CML::volVectorField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //     driver.setCalculatedPatches(*$$);
        //   }
        | TOKEN_faceAverage '(' fvexp ')'          {
            $$ = new CML::volVectorField(CML::fvc::average(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_integrate '(' fvexp ')'            {
            $$ = new CML::volVectorField(CML::fvc::surfaceIntegrate(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_surfSum '(' fvexp ')'              {
            $$ = new CML::volVectorField(CML::fvc::surfaceSum(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_interpolateToCell '(' pvexp ')'    {
            $$ = driver.pointToCellInterpolate(*$3).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_min '(' vexp ',' vexp  ')'         {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_max '(' vexp ',' vexp  ')'         {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_min '(' vexp ')'                   {
            $$ = driver.makeConstantField<CML::volVectorField>(
                CML::min(*$3).value()
            ).ptr();
            delete $3; }
        | TOKEN_minPosition '(' exp ')'           {
            $$ = driver.makeConstantField<CML::volVectorField>(
                driver.getPositionOfMinimum(
                    *$3,
                    driver.mesh().C()
                )
            ).ptr();
            delete $3;
        }
        | TOKEN_max '(' vexp ')'                  {
            $$ = driver.makeConstantField<CML::volVectorField>(
                CML::max(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_maxPosition '(' exp ')'           {
            $$ = driver.makeConstantField<CML::volVectorField>(
                driver.getPositionOfMaximum(
                    *$3,
                    driver.mesh().C()
                )
            ).ptr();
            delete $3;
        }
        | TOKEN_sum '(' vexp ')'                 {
            $$ = driver.makeConstantField<CML::volVectorField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' vexp ')'             {
            $$ = driver.makeConstantField<CML::volVectorField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_grad '(' exp ')'                  {
            $$ = new CML::volVectorField(CML::fvc::grad(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_reconstruct '(' fsexp ')'         {
            $$ = new CML::volVectorField(CML::fvc::reconstruct(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_curl '(' vexp ')'                 {
            $$ = new CML::volVectorField(CML::fvc::curl(*$3));
            delete $3; $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_div '(' texp ')'                    {
            $$ = new CML::volVectorField(CML::fvc::div(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_div '(' yexp ')'                    {
            $$ = new CML::volVectorField(CML::fvc::div(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_div '(' hexp ')'                    {
            $$ = new CML::volVectorField(CML::fvc::div(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_div '(' fsexp ',' vexp ')'        {
            $$ = new CML::volVectorField(CML::fvc::div(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_div '(' fvexp ')'        {
            $$ = new CML::volVectorField(CML::fvc::div(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
          }
        | evaluateVectorFunction restOfFunction
        | TOKEN_VID                               {
            $$=driver.getField<CML::volVectorField>(*$1).ptr();
            delete $1;
          }
        | TOKEN_OTHER_MESH_ID '(' TOKEN_OTHER_MESH_VID ')' {
            $$ = driver.interpolateForeignField<CML::volVectorField>(
                *$1,*$3,
                CML::MeshesRepository::getRepository().
                getInterpolationOrder(*$1)
            ).ptr();
            delete $1; delete $3;
          }
        | TOKEN_ddt '(' TOKEN_VID ')'		  {
            $$ = CML::fvc::ddt(
                driver.getOrReadField<CML::volVectorField>(
                    *$3,true,true
                )()
            ).ptr();
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
            delete $3;
          }
        | TOKEN_d2dt2 '(' TOKEN_VID ')'	  {
            $$ = CML::fvc::d2dt2(
                driver.getOrReadField<CML::volVectorField>(
                    *$3,true,true)()
            ).ptr();
            $$->dimensions().reset(CML::dimless);
            driver.setCalculatedPatches(*$$);
            delete $3;
          }
        | TOKEN_oldTime '(' TOKEN_VID ')'	   {
            $$ = new CML::volVectorField(
                driver.getOrReadField<CML::volVectorField>(
                    *$3,true,true
                )->oldTime()
            );
            driver.setCalculatedPatches(*$$);
            delete $3;
          }
;

evaluateVectorFunction: TOKEN_FUNCTION_VID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::volVectorField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;


fsexp:  TOKEN_surf '(' scalar ')'           {
          $$ = driver.makeConstantField<CML::surfaceScalarField>($3).ptr();
          }
        | fsexp '+' fsexp 		    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1 + *$3);
            delete $1; delete $3;
          }
        | fsexp '*' fsexp 		    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1 * *$3);
            delete $1; delete $3;
          }
        | fvexp '&' fvexp 		    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1 & *$3);
            delete $1; delete $3;
          }
        | ftexp TOKEN_AND ftexp 	    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | fyexp TOKEN_AND ftexp 	    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | fhexp TOKEN_AND ftexp 	    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | ftexp TOKEN_AND fyexp 	    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | fyexp TOKEN_AND fyexp 	    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | fhexp TOKEN_AND fyexp 	    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | ftexp TOKEN_AND fhexp 	    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | fyexp TOKEN_AND fhexp 	    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | fhexp TOKEN_AND fhexp 	    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | fsexp '/' fsexp 		    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1);
	    (*$$).internalField()/=(*$3).internalField();
            delete $1; delete $3;
          }
        | fsexp '-' fsexp 		    {
            sameSize($1,$3);
            $$ = new CML::surfaceScalarField(*$1 - *$3);
            delete $1; delete $3;
          }
        | TOKEN_pow '(' fsexp ',' scalar ')'{
            $$ = new CML::surfaceScalarField(CML::pow(*$3, $5));
            delete $3;
          }
        | TOKEN_pow '(' fsexp ',' fsexp ')'{
            $$ = new CML::surfaceScalarField(CML::pow(*$3, *$5));
            delete $3; delete $5;
          }
        | TOKEN_log '(' fsexp ')'           {
            $$ = new CML::surfaceScalarField(CML::log(*$3));
            delete $3;
          }
        | TOKEN_exp '(' fsexp ')'           {
            $$ = new CML::surfaceScalarField(CML::exp(*$3));
            delete $3;
          }
        | TOKEN_sqr '(' fsexp ')'           {
            $$ = new CML::surfaceScalarField(CML::sqr(*$3));
            delete $3;
          }
        | TOKEN_sqrt '(' fsexp ')'          {
            $$ = new CML::surfaceScalarField(CML::sqrt(*$3));
            delete $3;
          }
        | TOKEN_sin '(' fsexp ')'           {
            $$ = new CML::surfaceScalarField(CML::sin(*$3));
            delete $3;
          }
        | TOKEN_cos '(' fsexp ')'           {
            $$ = new CML::surfaceScalarField(CML::cos(*$3));
            delete $3;
          }
        | TOKEN_tan '(' fsexp ')'           {
            $$ = new CML::surfaceScalarField(CML::tan(*$3));
            delete $3;
          }
        | TOKEN_log10 '(' fsexp ')'         {
            $$ = new CML::surfaceScalarField(CML::log10(*$3));
            delete $3;
          }
        | TOKEN_asin '(' fsexp ')'          {
            $$ = new CML::surfaceScalarField(CML::asin(*$3));
            delete $3;
          }
        | TOKEN_acos '(' fsexp ')'          {
            $$ = new CML::surfaceScalarField(CML::acos(*$3));
            delete $3;
          }
        | TOKEN_atan '(' fsexp ')'          {
            $$ = new CML::surfaceScalarField(CML::atan(*$3));
            delete $3;
          }
        | TOKEN_sinh '(' fsexp ')'          {
            $$ = new CML::surfaceScalarField(CML::sinh(*$3));
            delete $3;
          }
        | TOKEN_cosh '(' fsexp ')'          {
            $$ = new CML::surfaceScalarField(CML::cosh(*$3));
            delete $3;
          }
        | TOKEN_tanh '(' fsexp ')'          {
            $$ = new CML::surfaceScalarField(CML::tanh(*$3));
            delete $3;
          }
        | TOKEN_asinh '(' fsexp ')'         {
            $$ = new CML::surfaceScalarField(CML::asinh(*$3));
            delete $3;
          }
        | TOKEN_acosh '(' fsexp ')'         {
            $$ = new CML::surfaceScalarField(CML::acosh(*$3));
            delete $3;
          }
        | TOKEN_atanh '(' fsexp ')'         {
            $$ = new CML::surfaceScalarField(CML::atanh(*$3));
            delete $3;
          }
        | TOKEN_erf '(' fsexp ')'           {
            $$ = new CML::surfaceScalarField(CML::erf(*$3));
            delete $3;
          }
        | TOKEN_erfc '(' fsexp ')'          {
            $$ = new CML::surfaceScalarField(CML::erfc(*$3));
            delete $3;
          }
        | TOKEN_lgamma '(' fsexp ')'        {
            $$ = new CML::surfaceScalarField(CML::lgamma(*$3));
            delete $3;
          }
        | TOKEN_besselJ0 '(' fsexp ')'      {
            $$ = new CML::surfaceScalarField(CML::j0(*$3));
            delete $3;
          }
        | TOKEN_besselJ1 '(' fsexp ')'      {
            $$ = new CML::surfaceScalarField(CML::j1(*$3));
            delete $3;
          }
        | TOKEN_besselY0 '(' fsexp ')'      {
            $$ = new CML::surfaceScalarField(CML::y0(*$3));
            delete $3;
          }
        | TOKEN_besselY1 '(' fsexp ')'      {
            $$ = new CML::surfaceScalarField(CML::y1(*$3));
            delete $3;
          }
        | TOKEN_sign '(' fsexp ')'          {
            $$ = new CML::surfaceScalarField(CML::sign(*$3));
            delete $3;
          }
        | TOKEN_pos '(' fsexp ')'           {
            $$ = new CML::surfaceScalarField(CML::pos(*$3));
            delete $3;
          }
        | TOKEN_neg '(' fsexp ')'           {
            $$ = new CML::surfaceScalarField(CML::neg(*$3));
            delete $3;
          }
        | TOKEN_min '(' fsexp ',' fsexp  ')'           {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' fsexp ',' fsexp  ')'           {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_min '(' fsexp ')'           {
            $$ = driver.makeConstantField<CML::surfaceScalarField>(
                CML::min(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_max '(' fsexp ')'           {
            $$ = driver.makeConstantField<CML::surfaceScalarField>(
                CML::max(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_sum '(' fsexp ')'           {
            $$ = driver.makeConstantField<CML::surfaceScalarField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' fsexp ')'       {
            $$ = driver.makeConstantField<CML::surfaceScalarField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | '-' fsexp %prec TOKEN_NEG         {
            $$ = new CML::surfaceScalarField(-*$2);
            delete $2;
          }
        | '(' fsexp ')'		            { $$ = $2; }
        | fvexp '.' vectorComponentSwitch TOKEN_x                     {
            $$ = new CML::surfaceScalarField($1->component(0));
            delete $1;
          }
        | fvexp '.' vectorComponentSwitch TOKEN_y                     {
            $$ = new CML::surfaceScalarField($1->component(1));
            delete $1;
          }
        | fvexp '.' vectorComponentSwitch TOKEN_z                     {
            $$ = new CML::surfaceScalarField($1->component(2));
            delete $1;
          }
        | ftexp '.' tensorComponentSwitch TOKEN_xx                 {
            $$ = new CML::surfaceScalarField($1->component(0));
            delete $1;
          }
        | ftexp '.' tensorComponentSwitch TOKEN_xy                 {
            $$ = new CML::surfaceScalarField($1->component(1));
            delete $1;
          }
        | ftexp '.' tensorComponentSwitch TOKEN_xz                 {
            $$ = new CML::surfaceScalarField($1->component(2));
            delete $1;
          }
        | ftexp '.' tensorComponentSwitch TOKEN_yx                 {
            $$ = new CML::surfaceScalarField($1->component(3));
            delete $1;
          }
        | ftexp '.' tensorComponentSwitch TOKEN_yy                 {
            $$ = new CML::surfaceScalarField($1->component(4));
            delete $1;
          }
        | ftexp '.' tensorComponentSwitch TOKEN_yz                 {
            $$ = new CML::surfaceScalarField($1->component(5));
            delete $1;
          }
        | ftexp '.' tensorComponentSwitch TOKEN_zx                 {
            $$ = new CML::surfaceScalarField($1->component(6));
            delete $1;
          }
        | ftexp '.' tensorComponentSwitch TOKEN_zy                 {
            $$ = new CML::surfaceScalarField($1->component(7));
            delete $1;
          }
        | ftexp '.' tensorComponentSwitch TOKEN_zz                 {
            $$ = new CML::surfaceScalarField($1->component(8));
            delete $1;
          }
        | fyexp '.' tensorComponentSwitch TOKEN_xx                 {
            $$ = new CML::surfaceScalarField($1->component(0));
            delete $1;
          }
        | fyexp '.' tensorComponentSwitch TOKEN_xy                 {
            $$ = new CML::surfaceScalarField($1->component(1));
            delete $1;
          }
        | fyexp '.' tensorComponentSwitch TOKEN_xz                 {
            $$ = new CML::surfaceScalarField($1->component(2));
            delete $1;
          }
        | fyexp '.' tensorComponentSwitch TOKEN_yy                 {
            $$ = new CML::surfaceScalarField($1->component(3));
            delete $1;
          }
        | fyexp '.' tensorComponentSwitch TOKEN_yz                 {
            $$ = new CML::surfaceScalarField($1->component(4));
            delete $1;
          }
        | fyexp '.' tensorComponentSwitch TOKEN_zz                 {
            $$ = new CML::surfaceScalarField($1->component(5));
            delete $1;
          }
        | fhexp '.' tensorComponentSwitch TOKEN_ii                 {
            $$ = new CML::surfaceScalarField($1->component(0));
            delete $1;
          }
        | flexp '?' fsexp ':' fsexp         {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
          }
        | TOKEN_mag '(' fsexp ')'            {
            $$ = new CML::surfaceScalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_mag '(' fvexp ')'            {
            $$ = new CML::surfaceScalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_mag '(' ftexp ')'            {
            $$ = new CML::surfaceScalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_mag '(' fyexp ')'            {
            $$ = new CML::surfaceScalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_mag '(' fhexp ')'            {
            $$ = new CML::surfaceScalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' fsexp ')'            {
            $$ = new CML::surfaceScalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' fvexp ')'            {
            $$ = new CML::surfaceScalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' ftexp ')'            {
            $$ = new CML::surfaceScalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' fyexp ')'            {
            $$ = new CML::surfaceScalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' fhexp ')'            {
            $$ = new CML::surfaceScalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_tr '(' ftexp ')'             {
            $$ = new CML::surfaceScalarField(CML::tr(*$3));
            delete $3;
          }
        | TOKEN_tr '(' fyexp ')'             {
            $$ = new CML::surfaceScalarField(CML::tr(*$3));
            delete $3;
          }
// doesn't work directly        | TOKEN_tr '(' fhexp ')'            {$$ = new CML::surfaceScalarField(CML::tr(*$3));
//            delete $3;
//          }
        | TOKEN_tr '(' fhexp ')'            {
            $$ = driver.makeField<CML::surfaceScalarField>(
                CML::tr($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_det '(' ftexp ')'            {
            $$ = new CML::surfaceScalarField(CML::det(*$3));
            delete $3;
          }
        | TOKEN_det '(' fyexp ')'            {
            $$ = new CML::surfaceScalarField(CML::det(*$3));
            delete $3;
          }
        | TOKEN_det '(' fhexp ')'           {
            $$ = driver.makeField<CML::surfaceScalarField>(
                CML::det($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_area '(' ')'                 {
            $$ = driver.makeAreaField().ptr();
          }
        | TOKEN_snGrad '(' exp ')'           {
            $$ = new CML::surfaceScalarField(CML::fvc::snGrad(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_interpolate '(' exp ')'      {
            $$ = new CML::surfaceScalarField(CML::fvc::interpolate(*$3));
            delete $3;
          }
        | TOKEN_FSID                         {
            $$ = driver.getField<CML::surfaceScalarField>(*$1).ptr();
            delete $1;
          }
        | evaluateFaceScalarFunction restOfFunction
//        | TOKEN_ddt '(' TOKEN_FSID ')'		    {
          //   $$ = CML::fvc::ddt(
          //       driver.getOrReadField<CML::surfaceScalarField>(
          //           *$3,true,true)()
          //   ).ptr();
          //   delete $3;
          // } // no fvc::ddt for surface Fields
        | TOKEN_oldTime '(' TOKEN_FSID ')'		    {
            $$ = new CML::surfaceScalarField(
                driver.getOrReadField<CML::surfaceScalarField>(
                    *$3,true,true
                )->oldTime());
            delete $3;
          }
        | TOKEN_meshPhi '(' vexp ')'       {
            $$ = CML::fvc::meshPhi(*$3).ptr();
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_meshPhi '(' exp ',' vexp ')'       {
            $$ = CML::fvc::meshPhi(*$3,*$5).ptr();
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_flux '(' fsexp ',' exp ')'        {
            $$ = CML::fvc::flux(*$3,*$5).ptr();
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_LOOKUP '(' fsexp ')'	     {
            $$ = driver.makeField<CML::surfaceScalarField>(
                driver.getLookup(*$1,*$3)
            ).ptr();
            delete $1; delete $3;
          }
;

evaluateFaceScalarFunction: TOKEN_FUNCTION_FSID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::surfaceScalarField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;

fvexp:  fvector                            { $$ = $1; }
        | fvexp '+' fvexp 		   {
            sameSize($1,$3);
            $$ = new CML::surfaceVectorField(*$1 + *$3);
            delete $1; delete $3;
          }
        | fsexp '*' fvexp 		   {
            sameSize($1,$3);
            $$ = new CML::surfaceVectorField(*$1 * *$3);
            delete $1; delete $3;
          }
        | fvexp '*' fsexp 		   {
            sameSize($1,$3);
            $$ = new CML::surfaceVectorField(*$1 * *$3);
            delete $1; delete $3;
          }
        | ftexp '&' fvexp 		   {
            sameSize($1,$3);
            $$ = new CML::surfaceVectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | fvexp '&' ftexp 		   {
            sameSize($1,$3);
            $$ = new CML::surfaceVectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | fhexp '&' fvexp 		   {
            sameSize($1,$3);
            $$ = new CML::surfaceVectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | fvexp '&' fhexp 		   {
            sameSize($1,$3);
            $$ = new CML::surfaceVectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | fyexp '&' fvexp 		   {
            sameSize($1,$3);
            $$ = new CML::surfaceVectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | fvexp '&' fyexp 		   {
            sameSize($1,$3);
            $$ = new CML::surfaceVectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | fvexp '^' fvexp 		   {
            sameSize($1,$3);
            $$ = new CML::surfaceVectorField(*$1 ^ *$3);
            delete $1; delete $3;
          }
        | fvexp '/' fsexp 		   {
            sameSize($1,$3);
            //$$ = new CML::surfaceVectorField(*$1 / *$3);
	    $$ = new CML::surfaceVectorField(*$1);
	    (*$$).internalField()/=(*$3).internalField();
            delete $1; delete $3;
          }
        | fvexp '-' fvexp 		   {
            sameSize($1,$3);
            $$ = new CML::surfaceVectorField(*$1 - *$3);
            delete $1; delete $3;}
        | '-' fvexp %prec TOKEN_NEG 	    {
            $$ = new CML::surfaceVectorField(-*$2);
            delete $2;
          }
        | '*' ftexp %prec TOKEN_HODGE 	        {
            $$ = new CML::surfaceVectorField(*(*$2));
            delete $2;
          }
        | '*' fyexp %prec TOKEN_HODGE 	        {
            $$ = new CML::surfaceVectorField(*(*$2));
            delete $2;
          }
        | '(' fvexp ')'		           { $$ = $2; }
        | TOKEN_eigenValues '(' ftexp ')'       {
            $$ = new CML::surfaceVectorField(CML::eigenValues(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_eigenValues '(' fyexp ')'       {
            $$ = new CML::surfaceVectorField(CML::eigenValues(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '.' tensorComponentSwitch  TOKEN_x                   {
            $$ = driver.makeSurfaceVectorField(
                $1->component(CML::tensor::XX)(),
                $1->component(CML::tensor::XY)(),
                $1->component(CML::tensor::XZ)()
            ).ptr();
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '.' tensorComponentSwitch  TOKEN_y                   {
            $$ = driver.makeSurfaceVectorField(
                $1->component(CML::tensor::YX)(),
                $1->component(CML::tensor::YY)(),
                $1->component(CML::tensor::YZ)()
            ).ptr();
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '.' tensorComponentSwitch  TOKEN_z                   {
            $$ = driver.makeSurfaceVectorField(
                $1->component(CML::tensor::ZX)(),
                $1->component(CML::tensor::ZY)(),
                $1->component(CML::tensor::ZZ)()
            ).ptr();
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_diag '(' ftexp ')'       {
            $$ = driver.makeSurfaceVectorField(
                $3->component(CML::tensor::XX)(),
                $3->component(CML::tensor::YY)(),
                $3->component(CML::tensor::ZZ)()
            ).ptr();
            delete $3;
          }
        | TOKEN_diag '(' fyexp ')'       {
            $$ = driver.makeSurfaceVectorField(
                $3->component(CML::symmTensor::XX)(),
                $3->component(CML::symmTensor::YY)(),
                $3->component(CML::symmTensor::ZZ)()
            ).ptr();
            delete $3;
          }
        | flexp '?' fvexp ':' fvexp        {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
          }
        | TOKEN_fposition '(' ')'           {
            $$ = driver.makeFacePositionField().ptr();
          }
        | TOKEN_fprojection '(' ')'         {
            $$ = driver.makeFaceProjectionField().ptr();
          }
        | TOKEN_face '(' ')'                {
            $$ = driver.makeFaceField().ptr();
          }
        | TOKEN_snGrad '(' vexp ')'         {
            $$ = new CML::surfaceVectorField(CML::fvc::snGrad(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_interpolate '(' vexp ')'    {
            $$ = new CML::surfaceVectorField(CML::fvc::interpolate(*$3));
            delete $3;
          }
        | TOKEN_min '(' fvexp ',' fvexp  ')'            {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' fvexp ',' fvexp  ')'            {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_min '(' fvexp ')'           {
            $$ = driver.makeConstantField<CML::surfaceVectorField>(
                CML::min(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_minPosition '(' fsexp ')'           {
            $$ = driver.makeConstantField<CML::surfaceVectorField>(
                driver.getPositionOfMinimum(
                    *$3,
                    driver.mesh().Cf()
                )
            ).ptr();
            delete $3;
        }
        | TOKEN_max '(' fvexp ')'           {
            $$ = driver.makeConstantField<CML::surfaceVectorField>(
                CML::max(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_maxPosition '(' fsexp ')'           {
            $$ = driver.makeConstantField<CML::surfaceVectorField>(
                driver.getPositionOfMaximum(
                    *$3,
                    driver.mesh().Cf()
                )
            ).ptr();
            delete $3;
        }
        | TOKEN_sum '(' fvexp ')'           {
            $$ = driver.makeConstantField<CML::surfaceVectorField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' fvexp ')'       {
            $$ = driver.makeConstantField<CML::surfaceVectorField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | evaluateFaceVectorFunction restOfFunction
        | TOKEN_FVID                        {
            $$ = driver.getField<CML::surfaceVectorField>(*$1).ptr();
            delete $1;
          }
//        | TOKEN_ddt '(' TOKEN_FVID ')'		    {
          //   $$ = CML::fvc::ddt( driver.getOrReadField<CML::surfaceVectorField>(*$3,true,true)() ).ptr();
          //   delete $3;
          // }// no fvc::ddt for surface Fields
        | TOKEN_oldTime '(' TOKEN_FVID ')'		    {
            $$ = new CML::surfaceVectorField(
                driver.getOrReadField<CML::surfaceVectorField>(
                    *$3,true,true
                )->oldTime());
            delete $3;
          }
        | TOKEN_flux '(' fsexp ',' vexp ')'        {
            $$ = CML::fvc::flux(*$3,*$5).ptr();
            delete $3; delete $5; $$->dimensions().reset(CML::dimless);
          }
;

evaluateFaceVectorFunction: TOKEN_FUNCTION_FVID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::surfaceVectorField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;

scalar:	TOKEN_NUM		        { $$ = $1; }
        | '-' TOKEN_NUM         	{ $$ = -$2; }
;

exp:    TOKEN_NUM                                   {
            $$ = driver.makeConstantField<CML::volScalarField>($1).ptr();
          }
        | exp '+' exp 		                   {
            sameSize($1,$3);
            $$ = new CML::volScalarField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | exp '-' exp 		                   {
            sameSize($1,$3);
            $$ = new CML::volScalarField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | exp '*' exp 		                   {
            sameSize($1,$3);
            $$ = new CML::volScalarField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | exp '%' exp 		                   {
            sameSize($1,$3);
            $$ = driver.makeModuloField(*$1,*$3).ptr();
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | exp '/' exp 		                   {
            sameSize($1,$3);
            $$ = new CML::volScalarField(*$1 / *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
// produces a reduce/reduc-conflict with the rule below
        // | TOKEN_pow '(' exp ',' scalar ')'	    {
        //     $$ = new CML::volScalarField(CML::pow(*$3, $5));
        //     delete $3;
        //     driver.setCalculatedPatches(*$$);
        //   }
        | TOKEN_pow '(' exp ',' exp ')'	    {
            $$ = new CML::volScalarField(CML::pow(*$3, *$5));
            delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_log '(' exp ')'                     {
            $$ = new CML::volScalarField(CML::log(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_exp '(' exp ')'                     {
            $$ = new CML::volScalarField(CML::exp(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | vexp '&' vexp 	                    {
            $$ = new CML::volScalarField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp TOKEN_AND texp 	                    {
            $$ = new CML::volScalarField(*$1 && *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp TOKEN_AND texp 	                    {
            $$ = new CML::volScalarField(*$1 && *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp TOKEN_AND texp 	                    {
            $$ = new CML::volScalarField(*$1 && *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp TOKEN_AND yexp 	                    {
            $$ = new CML::volScalarField(*$1 && *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp TOKEN_AND yexp 	                    {
            $$ = new CML::volScalarField(*$1 && *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp TOKEN_AND yexp 	                    {
            $$ = new CML::volScalarField(*$1 && *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp TOKEN_AND hexp 	                    {
            $$ = new CML::volScalarField(*$1 && *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp TOKEN_AND hexp 	                    {
            $$ = new CML::volScalarField(*$1 && *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp TOKEN_AND hexp 	                    {
            $$ = new CML::volScalarField(*$1 && *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | '-' exp %prec TOKEN_NEG 	            {
            $$ = new CML::volScalarField(-*$2);
            delete $2;
            driver.setCalculatedPatches(*$$);
          }
	| '(' exp ')'		                   { $$ = $2; }
        | TOKEN_sqr '(' exp ')'                     {
            $$ = new CML::volScalarField(CML::sqr(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_sqrt '(' exp ')'                    {
            $$ = new CML::volScalarField(CML::sqrt(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_sin '(' exp ')'                     {
            $$ = new CML::volScalarField(CML::sin(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_cos '(' exp ')'                     {
            $$ = new CML::volScalarField(CML::cos(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_tan '(' exp ')'                     {
            $$ = new CML::volScalarField(CML::tan(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_log10 '(' exp ')'                   {
            $$ = new CML::volScalarField(CML::log10(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_asin '(' exp ')'                    {
            $$ = new CML::volScalarField(CML::asin(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_acos '(' exp ')'                    {
            $$ = new CML::volScalarField(CML::acos(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_atan '(' exp ')'                    {
            $$ = new CML::volScalarField(CML::atan(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_sinh '(' exp ')'                    {
            $$ = new CML::volScalarField(CML::sinh(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_cosh '(' exp ')'                    {
            $$ = new CML::volScalarField(CML::cosh(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_tanh '(' exp ')'                    {
            $$ = new CML::volScalarField(CML::tanh(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_asinh '(' exp ')'                   {
            $$ = new CML::volScalarField(CML::asinh(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_acosh '(' exp ')'                   {
            $$ = new CML::volScalarField(CML::acosh(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_atanh '(' exp ')'                   {
            $$ = new CML::volScalarField(CML::atanh(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_erf '(' exp ')'                     {
            $$ = new CML::volScalarField(CML::erf(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_erfc '(' exp ')'                    {
            $$ = new CML::volScalarField(CML::erfc(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_lgamma '(' exp ')'                  {
            $$ = new CML::volScalarField(CML::lgamma(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_besselJ0 '(' exp ')'                {
            $$ = new CML::volScalarField(CML::j0(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_besselJ1 '(' exp ')'                {
            $$ = new CML::volScalarField(CML::j1(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_besselY0 '(' exp ')'                {
            $$ = new CML::volScalarField(CML::y0(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_besselY1 '(' exp ')'                {
            $$ = new CML::volScalarField(CML::y1(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_sign '(' exp ')'                    {
            $$ = new CML::volScalarField(CML::sign(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_pos '(' exp ')'                     {
            $$ = new CML::volScalarField(CML::pos(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_neg '(' exp ')'                     {
            $$ = new CML::volScalarField(CML::neg(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_min '(' exp ',' exp  ')'            {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_max '(' exp ',' exp  ')'            {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_min '(' exp ')'                     {
            $$ = driver.makeConstantField<CML::volScalarField>(
                CML::min(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_max '(' exp ')'                     {
            $$ = driver.makeConstantField<CML::volScalarField>(
                CML::max(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_sum '(' exp ')'                     {
            $$ = driver.makeConstantField<CML::volScalarField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' exp ')'                 {
            $$ = driver.makeConstantField<CML::volScalarField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_mag '(' exp ')'                     {
            $$ = new CML::volScalarField(CML::mag(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_mag '(' vexp ')'                    {
            $$ = new CML::volScalarField(CML::mag(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_mag '(' texp ')'                    {
            $$ = new CML::volScalarField(CML::mag(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_mag '(' yexp ')'                    {
            $$ = new CML::volScalarField(CML::mag(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_mag '(' hexp ')'                    {
            $$ = new CML::volScalarField(CML::mag(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_magSqr '(' exp ')'                     {
            $$ = new CML::volScalarField(CML::magSqr(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_magSqr '(' vexp ')'                    {
            $$ = new CML::volScalarField(CML::magSqr(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_magSqr '(' texp ')'                    {
            $$ = new CML::volScalarField(CML::magSqr(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_magSqr '(' yexp ')'                    {
            $$ = new CML::volScalarField(CML::magSqr(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_magSqr '(' hexp ')'                    {
            $$ = new CML::volScalarField(CML::magSqr(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_tr '(' texp ')'                     {
            $$ = new CML::volScalarField(CML::tr(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_tr '(' yexp ')'                     {
            $$ = new CML::volScalarField(CML::tr(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_tr '(' hexp ')'                    {
            $$ = driver.makeField<CML::volScalarField>(
                CML::tr($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_det '(' texp ')'                    {
            $$ = new CML::volScalarField(CML::det(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_det '(' yexp ')'                    {
            $$ = new CML::volScalarField(CML::det(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_det '(' hexp ')'                   {
            $$ = driver.makeField<CML::volScalarField>(
                CML::det($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_magSqrGradGrad '(' exp ')'          {
            $$ = new CML::volScalarField(CML::fvc::magSqrGradGrad(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_div '(' vexp ')'                    {
            $$ = new CML::volScalarField(CML::fvc::div(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_div '(' fsexp ')'                   {
            $$ = new CML::volScalarField(CML::fvc::div(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_div '(' fsexp ',' exp ')'           {
            $$ = new CML::volScalarField(CML::fvc::div(*$3,*$5));
            delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' exp ')'               {
            $$ = new CML::volScalarField(CML::fvc::laplacian(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' exp ',' exp ')'       {
            $$ = new CML::volScalarField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' vexp ',' exp ')'       {
        //     $$ = new CML::volScalarField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     driver.setCalculatedPatches(*$$);
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_laplacian '(' texp ',' exp ')'       {
            $$ = new CML::volScalarField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' yexp ',' exp ')'       {
            $$ = new CML::volScalarField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' hexp ',' exp ')'       {
        //     $$ = new CML::volScalarField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     driver.setCalculatedPatches(*$$);
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_laplacian '(' fsexp ',' exp ')'     {
            $$ = new CML::volScalarField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' fvexp ',' exp ')'     {
        //     $$ = new CML::volScalarField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     driver.setCalculatedPatches(*$$);
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_laplacian '(' ftexp ',' exp ')'     {
            $$ = new CML::volScalarField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' fyexp ',' exp ')'     {
            $$ = new CML::volScalarField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' fhexp ',' exp ')'     {
        //     $$ = new CML::volScalarField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     driver.setCalculatedPatches(*$$);
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_faceAverage '(' fsexp ')'           {
            $$ = new CML::volScalarField(CML::fvc::average(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_integrate '(' fsexp ')'             {
            $$ = new CML::volScalarField(CML::fvc::surfaceIntegrate(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_surfSum '(' fsexp ')'               {
            $$ = new CML::volScalarField(CML::fvc::surfaceSum(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_interpolateToCell '(' psexp ')'               {
            $$ = driver.pointToCellInterpolate(*$3).ptr();
            delete $3;
          }
        | vexp '.' vectorComponentSwitch  TOKEN_x                              {
            $$ = new CML::volScalarField($1->component(0));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | vexp '.' vectorComponentSwitch  TOKEN_y                              {
            $$ = new CML::volScalarField($1->component(1));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | vexp '.' vectorComponentSwitch  TOKEN_z                              {
            $$ = new CML::volScalarField($1->component(2));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | texp '.' tensorComponentSwitch TOKEN_xx                         {
            $$ = new CML::volScalarField($1->component(0));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | texp '.' tensorComponentSwitch TOKEN_xy                         {
            $$ = new CML::volScalarField($1->component(1));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | texp '.' tensorComponentSwitch TOKEN_xz                         {
            $$ = new CML::volScalarField($1->component(2));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | texp '.' tensorComponentSwitch TOKEN_yx                         {
            $$ = new CML::volScalarField($1->component(3));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | texp '.' tensorComponentSwitch TOKEN_yy                         {
            $$ = new CML::volScalarField($1->component(4));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | texp '.' tensorComponentSwitch TOKEN_yz                         {
            $$ = new CML::volScalarField($1->component(5));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | texp '.' tensorComponentSwitch TOKEN_zx                         {
            $$ = new CML::volScalarField($1->component(6));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | texp '.' tensorComponentSwitch TOKEN_zy                         {
            $$ = new CML::volScalarField($1->component(7));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | texp '.' tensorComponentSwitch TOKEN_zz                         {
            $$ = new CML::volScalarField($1->component(8));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '.' tensorComponentSwitch TOKEN_xx                         {
            $$ = new CML::volScalarField($1->component(0));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '.' tensorComponentSwitch TOKEN_xy                         {
            $$ = new CML::volScalarField($1->component(1));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '.' tensorComponentSwitch TOKEN_xz                         {
            $$ = new CML::volScalarField($1->component(2));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '.' tensorComponentSwitch TOKEN_yy                         {
            $$ = new CML::volScalarField($1->component(3));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '.' tensorComponentSwitch TOKEN_yz                         {
            $$ = new CML::volScalarField($1->component(4));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '.' tensorComponentSwitch TOKEN_zz                         {
            $$ = new CML::volScalarField($1->component(5));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | hexp '.' tensorComponentSwitch TOKEN_ii                         {
            $$ = new CML::volScalarField($1->component(0));
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | lexp '?' exp ':' exp                     {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
            driver.setCalculatedPatches(*$$);}
        | TOKEN_pi                                  {
            $$ = driver.makeConstantField<CML::volScalarField>(
               CML::constant::mathematical::pi
	    ).ptr();
          }
        | TOKEN_dist '(' ')'                        {
            $$ = driver.makeDistanceField().ptr();
          }
        | TOKEN_distToPatch '(' TOKEN_PATCHID ')'                        {
            $$ = driver.makeDistanceToPatchField( *$3 ).ptr();
            delete $3;
          }
        | TOKEN_nearDist '(' ')'                    {
            $$ = driver.makeNearDistanceField().ptr();
          }
        | TOKEN_rdist '(' vexp ')'                  {
            $$ = driver.makeRDistanceField(*$3).ptr();
            delete $3;
          }
        | TOKEN_volume '(' ')'                      {
            $$ = driver.makeVolumeField().ptr();
          }
        | TOKEN_rand '(' ')'                        {
            $$ = driver.makeRandomField().ptr();
          }
        | TOKEN_rand '(' TOKEN_INT ')'                        {
            $$ = driver.makeRandomField(-$3).ptr();
          }
        | TOKEN_randNormal '(' ')'                  {
            $$ = driver.makeGaussRandomField().ptr();
          }
        | TOKEN_randNormal '(' TOKEN_INT ')'                  {
            $$ = driver.makeGaussRandomField(-$3).ptr();
          }
        | TOKEN_randFixed '(' ')'                        {
            $$ = driver.makeRandomField(1).ptr();
          }
        | TOKEN_randFixed '(' TOKEN_INT ')'                        {
            $$ = driver.makeRandomField($3+1).ptr();
          }
        | TOKEN_randNormalFixed '(' ')'                  {
            $$ = driver.makeGaussRandomField(1).ptr();
          }
        | TOKEN_randNormalFixed '(' TOKEN_INT ')'                  {
            $$ = driver.makeGaussRandomField($3+1).ptr();
          }
        | TOKEN_id '(' ')'                          {
            $$ = driver.makeCellIdField().ptr();
          }
        | TOKEN_cpu'(' ')'                          {
            $$ = driver.makeConstantField<CML::volScalarField>(
                CML::Pstream::myProcNo()
            ).ptr();
          }
        | TOKEN_weight'(' ')'                          {
            $$ = driver.makeField<CML::volScalarField>(
                driver.weights(driver.size())
            ).ptr();
          }
        | TOKEN_deltaT '(' ')'                      {
            $$ = driver.makeConstantField<CML::volScalarField>(
                driver.runTime().deltaT().value()
            ).ptr();
          }
        | TOKEN_time '(' ')'                        {
            $$ = driver.makeConstantField<CML::volScalarField>(
                driver.runTime().time().value()
            ).ptr();
          }
        | TOKEN_SID		                    {
            $$ = driver.getField<CML::volScalarField>(*$1).ptr();
            delete $1;
          }
        | TOKEN_OTHER_MESH_ID '(' TOKEN_OTHER_MESH_SID ')' {
            $$ = driver.interpolateForeignField<CML::volScalarField>(
                *$1,*$3,
                CML::MeshesRepository::getRepository().
                getInterpolationOrder(*$1)
            ).ptr();
            delete $1; delete $3;
          }
        | evaluateScalarFunction restOfFunction
          { $$=$1; }
        | TOKEN_ddt '(' TOKEN_SID ')'		    {
            $$ = CML::fvc::ddt(
                driver.getOrReadField<CML::volScalarField>(
                    *$3,true,true
                )() ).ptr();
            $$->dimensions().reset(CML::dimless);
            delete $3;
          }
        | TOKEN_d2dt2 '(' TOKEN_SID ')'		    {
            $$ = CML::fvc::d2dt2(
                driver.getOrReadField<CML::volScalarField>(
                    *$3,true,true
                )() ).ptr();
            $$->dimensions().reset(CML::dimless);
            delete $3;
          }
        | TOKEN_oldTime '(' TOKEN_SID ')'	    {
            $$ = new CML::volScalarField(
                driver.getOrReadField<CML::volScalarField>(
                    *$3,true,true
                )->oldTime());
            delete $3;
          }
        | TOKEN_LINE            		    {
            $$ = driver.makeConstantField<CML::volScalarField>(
                driver.getLineValue(*$1,driver.runTime().time().value())
            ).ptr();
            delete $1;
          }
        | TOKEN_LOOKUP '(' exp ')'		    {
            $$ = driver.makeField<CML::volScalarField>(
                driver.getLookup(*$1,*$3)
            ).ptr();
            delete $1; delete$3;
          }
;

restOfFunction:    TOKEN_LAST_FUNCTION_CHAR
                   | TOKEN_IN_FUNCTION_CHAR restOfFunction;

evaluateScalarFunction: TOKEN_FUNCTION_SID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::volScalarField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;

lexp: TOKEN_TRUE                        {
            $$ = driver.makeConstantField<CML::volScalarField>(1).ptr();
          }
    | TOKEN_FALSE                       {
            $$ = driver.makeConstantField<CML::volScalarField>(0).ptr();
          }
    | TOKEN_set '(' TOKEN_SETID ')'    {
        $$ = driver.makeCellSetField(*$3).ptr();
        delete $3;
      }
    | TOKEN_zone '(' TOKEN_ZONEID ')'  {
        $$ = driver.makeCellZoneField(*$3).ptr();
        delete $3;
      }
    | exp '<' exp                      {
            sameSize($1,$3);
            $$ = driver.doCompare(*$1,std::less<CML::scalar>(),*$3).ptr();
            delete $1; delete $3;
          }
    | exp '>' exp                      {
            sameSize($1,$3);
            $$ = driver.doCompare(*$1,std::greater<CML::scalar>(),*$3).ptr();
            delete $1; delete $3;
          }
    | exp TOKEN_LEQ exp                {
            sameSize($1,$3);
            $$ = driver.doCompare(
                *$1,
                std::less_equal<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | exp TOKEN_GEQ exp                {
            sameSize($1,$3);
            $$ = driver.doCompare(
                *$1,
                std::greater_equal<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | exp TOKEN_EQ exp                 {
            sameSize($1,$3);
            $$ = driver.doCompare(
                *$1,
                std::equal_to<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | exp TOKEN_NEQ exp                {
            sameSize($1,$3);
            $$ = driver.doCompare(
                *$1,
                std::not_equal_to<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | '(' lexp ')'		       { $$ = $2; }
    | lexp TOKEN_AND lexp              {
            sameSize($1,$3);
            $$ = driver.doLogicalOp(
                *$1,
                std::logical_and<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | lexp TOKEN_OR lexp               {
            sameSize($1,$3);
            $$ = driver.doLogicalOp(
                *$1,
                std::logical_or<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | '!' lexp %prec TOKEN_NOT          {
            $$ = driver.doLogicalNot(*$2).ptr();
            delete $2;
          }
    | evaluateLogicalFunction restOfFunction
;

evaluateLogicalFunction: TOKEN_FUNCTION_LID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::volScalarField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;


flexp: TOKEN_surf '(' TOKEN_TRUE ')'   {
            $$ = driver.makeConstantField<CML::surfaceScalarField>(1).ptr();
          }
    | TOKEN_surf '(' TOKEN_FALSE ')'   {
            $$ = driver.makeConstantField<CML::surfaceScalarField>(0).ptr();
          }
    | TOKEN_fset '(' TOKEN_FSETID ')'    {
        $$ = driver.makeFaceSetField(*$3).ptr();
        delete $3;
      }
    | TOKEN_fzone '(' TOKEN_FZONEID ')'  {
        $$ = driver.makeFaceZoneField(*$3).ptr();
        delete $3;
      }
    | TOKEN_onPatch '(' TOKEN_PATCHID ')'  {
        $$ = driver.makeOnPatchField(*$3).ptr();
        delete $3;
      }
    | TOKEN_internalFace '(' ')'       {
            $$ = driver.makeInternalFaceField().ptr();
          }
    | fsexp '<' fsexp                 {
            sameSize($1,$3);
            $$ = driver.doCompare(*$1,std::less<CML::scalar>(),*$3).ptr();
            delete $1; delete $3;
          }
    | fsexp '>' fsexp                 {
            sameSize($1,$3);
            $$ = driver.doCompare(*$1,std::greater<CML::scalar>(),*$3).ptr();
            delete $1; delete $3;
          }
    | fsexp TOKEN_LEQ fsexp           {
            sameSize($1,$3);
            $$ = driver.doCompare(
                *$1,
                std::less_equal<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | fsexp TOKEN_GEQ fsexp           {
            sameSize($1,$3);
            $$ = driver.doCompare(
                *$1,
                std::greater_equal<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | fsexp TOKEN_EQ fsexp            {
            sameSize($1,$3);
            $$ = driver.doCompare(
                *$1,
                std::equal_to<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | fsexp TOKEN_NEQ fsexp           {
            sameSize($1,$3);
            $$ = driver.doCompare(
                *$1,
                std::not_equal_to<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | '(' flexp ')'		      { $$ = $2; }
    | flexp TOKEN_AND flexp           {
            sameSize($1,$3);
            $$ = driver.doLogicalOp(
                *$1,
                std::logical_and<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | flexp TOKEN_OR flexp            {
            sameSize($1,$3);
            $$ = driver.doLogicalOp(
                *$1,
                std::logical_or<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | '!' flexp %prec TOKEN_NOT        {
            $$ = driver.doLogicalNot(*$2).ptr();
            delete $2;
          }
    | evaluateFaceLogicalFunction restOfFunction
;

evaluateFaceLogicalFunction: TOKEN_FUNCTION_FLID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::surfaceScalarField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;


texp:   tensor                  { $$ = $1; }
        | texp '+' texp 		          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp '+' yexp 		          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp '+' hexp 		          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '+' texp 		          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp '+' texp 		          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | exp '*' texp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp '*' exp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | vexp '*' vexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp '&' texp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '&' texp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp '&' yexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp '&' texp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp '&' hexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp '/' exp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 / *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp '-' texp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp '-' yexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp '-' hexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '-' texp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp '-' texp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | '-' texp %prec TOKEN_NEG 	            {
            $$ = new CML::volTensorField(-*$2);
            delete $2;
            driver.setCalculatedPatches(*$$);
          }
        | '(' texp ')'		        { $$ = $2; }
        | TOKEN_skew '(' texp ')' 	            {
            $$ = new CML::volTensorField( CML::skew(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_eigenVectors '(' texp ')'       {
            $$ = new CML::volTensorField(CML::eigenVectors(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_eigenVectors '(' yexp ')'       {
            $$ = driver.makeField<CML::volTensorField>(
                CML::eigenVectors($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_inv '(' texp ')' 	            {
            $$ = new CML::volTensorField( CML::inv(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_cof '(' texp ')' 	            {
            $$ = driver.makeField<CML::volTensorField>(
                CML::cof($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_dev '(' texp ')' 	            {
            $$ = new CML::volTensorField( CML::dev(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_dev2 '(' texp ')' 	            {
            $$ = new CML::volTensorField( CML::dev2(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | texp '.' tensorComponentSwitch TOKEN_transpose '(' ')'  {
            $$ = new CML::volTensorField( $1->T() );
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | lexp '?' texp ':' texp                   {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_laplacian '(' texp ')'   {
            $$ = new CML::volTensorField(CML::fvc::laplacian(*$3));
            delete $3;;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' fsexp ',' texp ')'   {
            $$ = new CML::volTensorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' fvexp ',' texp ')'   {
        //     $$ = new CML::volTensorField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_laplacian '(' ftexp ',' texp ')'   {
            $$ = new CML::volTensorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' fyexp ',' texp ')'   {
            $$ = new CML::volTensorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' fhexp ',' texp ')'   {
        //     $$ = new CML::volTensorField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_laplacian '(' exp ',' texp ')'   {
            $$ = new CML::volTensorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' vexp ',' texp ')'   {
        //     $$ = new CML::volTensorField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_laplacian '(' texp ',' texp ')'   {
            $$ = new CML::volTensorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' yexp ',' texp ')'   {
            $$ = new CML::volTensorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' hexp ',' texp ')'   {
        //     $$ = new CML::volTensorField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_faceAverage '(' ftexp ')'          {
            $$ = new CML::volTensorField(CML::fvc::average(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_integrate '(' ftexp ')'            {
            $$ = new CML::volTensorField(CML::fvc::surfaceIntegrate(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_surfSum '(' ftexp ')'              {
            $$ = new CML::volTensorField(CML::fvc::surfaceSum(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_interpolateToCell '(' ptexp ')'               {
            $$ = driver.pointToCellInterpolate(*$3).ptr();
            delete $3;
          }
        | TOKEN_min '(' texp ',' texp  ')'         {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' texp ',' texp  ')'         {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_min '(' texp ')'                  {
            $$ = driver.makeConstantField<CML::volTensorField>(
                CML::min(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_max '(' texp ')'                  {
            $$ = driver.makeConstantField<CML::volTensorField>(
                CML::max(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_sum '(' texp ')'                  {
            $$ = driver.makeConstantField<CML::volTensorField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' texp ')'              {
            $$ = driver.makeConstantField<CML::volTensorField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_grad '(' vexp ')'                  {
            $$ = new CML::volTensorField(CML::fvc::grad(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_div '(' ftexp ')'         {
            $$ = new CML::volTensorField(CML::fvc::div(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_div '(' fsexp ',' texp ')'         {
            $$ = new CML::volTensorField(CML::fvc::div(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        | evaluateTensorFunction restOfFunction
        | TOKEN_TID                                {
            $$=driver.getField<CML::volTensorField>(*$1).ptr();
            delete $1;
          }
        | TOKEN_OTHER_MESH_ID '(' TOKEN_OTHER_MESH_TID ')' {
            $$ = driver.interpolateForeignField<CML::volTensorField>(
                *$1,*$3,
                CML::MeshesRepository::getRepository().
                getInterpolationOrder(*$1)
            ).ptr();
            delete $1; delete $3;
          }
        | TOKEN_ddt '(' TOKEN_TID ')'		    {
            $$ = CML::fvc::ddt(
                driver.getOrReadField<CML::volTensorField>(
                    *$3,true,true
                )()
            ).ptr();
            $$->dimensions().reset(CML::dimless);
            delete $3;
          }
        | TOKEN_d2dt2 '(' TOKEN_TID ')'		    {
            $$ = CML::fvc::d2dt2(
                driver.getOrReadField<CML::volTensorField>(
                    *$3,true,true
                )()
            ).ptr();
            $$->dimensions().reset(CML::dimless);
            delete $3;
          }
        | TOKEN_oldTime '(' TOKEN_TID ')'		    {
            $$ = new CML::volTensorField(
                driver.getOrReadField<CML::volTensorField>(
                    *$3,true,true
                )->oldTime()
            );
            delete $3;
          }
;

evaluateTensorFunction: TOKEN_FUNCTION_TID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::volTensorField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;

yexp:   symmTensor                  { $$ = $1; }
        | yexp '+' yexp 		          {
            sameSize($1,$3);
            $$ = new CML::volSymmTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp '+' yexp 		          {
            sameSize($1,$3);
            $$ = new CML::volSymmTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '+' hexp 		          {
            sameSize($1,$3);
            $$ = new CML::volSymmTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | exp '*' yexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::volSymmTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '*' exp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::volSymmTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '&' yexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volSymmTensorField(
                symm(*$1 & *$3)
            );
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp '&' yexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volSymmTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '&' hexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volSymmTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '/' exp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::volSymmTensorField(*$1 / *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '-' yexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volSymmTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp '-' yexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volSymmTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '-' hexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volSymmTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | '-' yexp %prec TOKEN_NEG 	            {
            $$ = new CML::volSymmTensorField(-*$2);
            delete $2;
            driver.setCalculatedPatches(*$$);
          }
        | '(' yexp ')'		        { $$ = $2; }
        | TOKEN_symm '(' texp ')' 	            {
            $$ = new CML::volSymmTensorField( CML::symm(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_symm '(' yexp ')' 	            {
            $$ = new CML::volSymmTensorField( CML::symm(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_twoSymm '(' texp ')' 	            {
            $$ = new CML::volSymmTensorField( CML::twoSymm(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_twoSymm '(' yexp ')' 	            {
            $$ = new CML::volSymmTensorField( CML::twoSymm(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_inv '(' yexp ')' 	            {
            $$ = new CML::volSymmTensorField( CML::inv(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_cof '(' yexp ')' 	            {
            $$ = driver.makeField<CML::volSymmTensorField>(
                CML::cof($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_dev '(' yexp ')' 	            {
            $$ = new CML::volSymmTensorField( CML::dev(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_dev2 '(' yexp ')' 	            {
            $$ = new CML::volSymmTensorField( CML::dev2(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_sqr '(' vexp ')' 	            {
            $$ = new CML::volSymmTensorField( CML::sqr(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | yexp '.' tensorComponentSwitch TOKEN_transpose '(' ')'  {
            $$ = $1;
          }
        | lexp '?' yexp ':' yexp                   {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_laplacian '(' yexp ')'   {
            $$ = new CML::volSymmTensorField(CML::fvc::laplacian(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' fsexp ',' yexp ')'   {
            $$ = new CML::volSymmTensorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' fvexp ',' yexp ')'   {
        //     $$ = new CML::volSymmTensorField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_laplacian '(' ftexp ',' yexp ')'   {
            $$ = new CML::volSymmTensorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' fyexp ',' yexp ')'   {
            $$ = new CML::volSymmTensorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' fhexp ',' yexp ')'   {
        //     $$ = new CML::volSymmTensorField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_laplacian '(' exp ',' yexp ')'   {
            $$ = new CML::volSymmTensorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' vexp ',' yexp ')'   {
        //     $$ = new CML::volSymmTensorField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_laplacian '(' texp ',' yexp ')'   {
            $$ = new CML::volSymmTensorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' yexp ',' yexp ')'   {
            $$ = new CML::volSymmTensorField(CML::fvc::laplacian(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' hexp ',' yexp ')'   {
        //     $$ = new CML::volSymmTensorField(CML::fvc::laplacian(*$3,*$5));
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_faceAverage '(' fyexp ')'          {
            $$ = new CML::volSymmTensorField(CML::fvc::average(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_integrate '(' fyexp ')'            {
            $$ = new CML::volSymmTensorField(CML::fvc::surfaceIntegrate(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_surfSum '(' fyexp ')'              {
            $$ = new CML::volSymmTensorField(CML::fvc::surfaceSum(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_interpolateToCell '(' pyexp ')'               {
            $$ = driver.pointToCellInterpolate(*$3).ptr();
            delete $3;
          }
        | TOKEN_min '(' yexp ',' yexp  ')'         {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' yexp ',' yexp  ')'         {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_min '(' yexp ')'                  {
            $$ = driver.makeConstantField<CML::volSymmTensorField>(
                CML::min(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_max '(' yexp ')'                  {
            $$ = driver.makeConstantField<CML::volSymmTensorField>(
                CML::max(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_sum '(' yexp ')'                  {
            $$ = driver.makeConstantField<CML::volSymmTensorField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' yexp ')'              {
            $$ = driver.makeConstantField<CML::volSymmTensorField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_div '(' fyexp ')'         {
            $$ = new CML::volSymmTensorField(CML::fvc::div(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_div '(' fsexp ',' yexp ')'         {
            $$ = new CML::volSymmTensorField(CML::fvc::div(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        | evaluateSymmTensorFunction restOfFunction
        | TOKEN_YID                                {
            $$=driver.getField<CML::volSymmTensorField>(*$1).ptr();
            delete $1;
          }
        | TOKEN_OTHER_MESH_ID '(' TOKEN_OTHER_MESH_YID ')' {
            $$ = driver.interpolateForeignField<CML::volSymmTensorField>(
                *$1,*$3,
                CML::MeshesRepository::getRepository().
                getInterpolationOrder(*$1)
            ).ptr();
            delete $1; delete $3;
          }
        | TOKEN_ddt '(' TOKEN_YID ')'		    {
            $$ = CML::fvc::ddt(
                driver.getOrReadField<CML::volSymmTensorField>(
                    *$3,true,true
                )()
            ).ptr();
            $$->dimensions().reset(CML::dimless);
            delete $3;
          }
        | TOKEN_d2dt2 '(' TOKEN_YID ')'		    {
            $$ = CML::fvc::d2dt2(
                driver.getOrReadField<CML::volSymmTensorField>(
                    *$3,true,true
                )()
            ).ptr();
            $$->dimensions().reset(CML::dimless);
            delete $3;
          }
        | TOKEN_oldTime '(' TOKEN_YID ')'	    {
            $$ = new CML::volSymmTensorField(
                driver.getOrReadField<CML::volSymmTensorField>(
                    *$3,true,true
                )->oldTime()
            );
            delete $3;
          }
;

evaluateSymmTensorFunction: TOKEN_FUNCTION_YID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::volSymmTensorField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;


hexp:   sphericalTensor                  { $$ = $1; }
        | TOKEN_unitTensor                        {
            $$ = driver.makeConstantField<CML::volSphericalTensorField>(
                CML::sphericalTensor(1)
            ).ptr();
          }
        | hexp '+' hexp 		          {
            sameSize($1,$3);
            $$ = new CML::volSphericalTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | exp '*' hexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::volSphericalTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp '*' exp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::volSphericalTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp '&' hexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volSphericalTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp '/' exp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::volSphericalTensorField(*$1 / *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp '-' hexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::volSphericalTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | '-' hexp %prec TOKEN_NEG 	            {
            $$ = new CML::volSphericalTensorField(-*$2);
            delete $2;
            driver.setCalculatedPatches(*$$);
          }
        | '(' hexp ')'		        { $$ = $2; }
        | TOKEN_sph '(' texp ')'              {
            $$ = driver.makeField<CML::volSphericalTensorField>(
                CML::sph($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_sph '(' yexp ')'              {
            $$ = driver.makeField<CML::volSphericalTensorField>(
                CML::sph($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_sph '(' hexp ')'              {
            $$ = driver.makeField<CML::volSphericalTensorField>(
                CML::sph($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_inv '(' hexp ')' 	           {
            $$ = driver.makeField<CML::volSphericalTensorField>(
                CML::inv($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | hexp '.' tensorComponentSwitch TOKEN_transpose '(' ')'  {
            $$ = $1;
          }
        | lexp '?' hexp ':' hexp                   {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
            driver.setCalculatedPatches(*$$);}
        | TOKEN_laplacian '(' hexp ')'   {
            $$ = new CML::volSphericalTensorField(
                CML::fvc::laplacian(*$3)
            );
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' fsexp ',' hexp ')'   {
            $$ = new CML::volSphericalTensorField(
                CML::fvc::laplacian(*$3,*$5)
            );
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' fvexp ',' hexp ')'   {
        //     $$ = new CML::volSphericalTensorField(
        //         CML::fvc::laplacian(*$3,*$5)
        //     );
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_laplacian '(' ftexp ',' hexp ')'   {
            $$ = new CML::volSphericalTensorField(
                CML::fvc::laplacian(*$3,*$5)
            );
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' fyexp ',' hexp ')'   {
            $$ = new CML::volSphericalTensorField(
                CML::fvc::laplacian(*$3,*$5)
            );
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' fhexp ',' hexp ')'   {
        //     $$ = new CML::volSphericalTensorField(
        //         CML::fvc::laplacian(*$3,*$5)
        //     );
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_laplacian '(' exp ',' hexp ')'   {
            $$ = new CML::volSphericalTensorField(
                CML::fvc::laplacian(*$3,*$5)
            );
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' vexp ',' hexp ')'   {
        //     $$ = new CML::volSphericalTensorField(
        //         CML::fvc::laplacian(*$3,*$5)
        //     );
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_laplacian '(' texp ',' hexp ')'   {
            $$ = new CML::volSphericalTensorField(
                CML::fvc::laplacian(*$3,*$5)
            );
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_laplacian '(' yexp ',' hexp ')'   {
            $$ = new CML::volSphericalTensorField(
                CML::fvc::laplacian(*$3,*$5)
            );
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        // | TOKEN_laplacian '(' hexp ',' hexp ')'   {
        //     $$ = new CML::volSphericalTensorField(
        //         CML::fvc::laplacian(*$3,*$5)
        //     );
        //     delete $3; delete $5;
        //     $$->dimensions().reset(CML::dimless);
        //   }
        | TOKEN_faceAverage '(' fhexp ')'          {
            $$ = new CML::volSphericalTensorField(CML::fvc::average(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_integrate '(' fhexp ')'            {
            $$ = new CML::volSphericalTensorField(
                CML::fvc::surfaceIntegrate(*$3)
            );
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_surfSum '(' fhexp ')'              {
            $$ = new CML::volSphericalTensorField(
                CML::fvc::surfaceSum(*$3)
            );
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_interpolateToCell '(' phexp ')'               {
            $$ = driver.pointToCellInterpolate(*$3).ptr();
            delete $3;
          }
        | TOKEN_min '(' hexp ',' hexp  ')'         {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' hexp ',' hexp  ')'         {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_min '(' hexp ')'                  {
            $$ = driver.makeConstantField<CML::volSphericalTensorField>(
                CML::min(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_max '(' hexp ')'                  {
            $$ = driver.makeConstantField<CML::volSphericalTensorField>(
                CML::max(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_sum '(' hexp ')'                  {
            $$ = driver.makeConstantField<CML::volSphericalTensorField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' hexp ')'              {
            $$ = driver.makeConstantField<CML::volSphericalTensorField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_div '(' fhexp ')'         {
            $$ = new CML::volSphericalTensorField(CML::fvc::div(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_div '(' fsexp ',' hexp ')'         {
            $$ = new CML::volSphericalTensorField(CML::fvc::div(*$3,*$5));
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
        | evaluateSphericalTensorFunction restOfFunction
        | TOKEN_HID                                {
            $$=driver.getField<CML::volSphericalTensorField>(*$1).ptr();
            delete $1;
          }
        | TOKEN_OTHER_MESH_ID '(' TOKEN_OTHER_MESH_HID ')' {
            $$ = driver.interpolateForeignField<CML::volSphericalTensorField>(
                *$1,*$3,
                CML::MeshesRepository::getRepository().
                getInterpolationOrder(*$1)
            ).ptr();
            delete $1; delete $3;
          }
        | TOKEN_ddt '(' TOKEN_HID ')'		    {
            $$ = CML::fvc::ddt(
                driver.getOrReadField<CML::volSphericalTensorField>(
                    *$3,true,true
                )()
            ).ptr();
            $$->dimensions().reset(CML::dimless);
            delete $3;
          }
        | TOKEN_d2dt2 '(' TOKEN_HID ')'		    {
            $$ = CML::fvc::d2dt2(
                driver.getOrReadField<CML::volSphericalTensorField>(
                    *$3,true,true
                )()
            ).ptr();
            $$->dimensions().reset(CML::dimless);
            delete $3;
          }
        | TOKEN_oldTime '(' TOKEN_HID ')'	    {
            $$ = new CML::volSphericalTensorField(
                driver.getOrReadField<CML::volSphericalTensorField>(
                    *$3,true,true
                )->oldTime()
            );
            delete $3;
          }
;

evaluateSphericalTensorFunction: TOKEN_FUNCTION_HID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::volSphericalTensorField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;

ftexp:   ftensor                  { $$ = $1; }
        | ftexp '+' ftexp 		          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '+' fyexp 		          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '+' fhexp 		          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fyexp '+' ftexp 		          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fhexp '+' ftexp 		          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fsexp '*' ftexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '*' fsexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fvexp '*' fvexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '&' ftexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fyexp '&' ftexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '&' fyexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fhexp '&' ftexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '&' fhexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '/' fsexp 	   	                  {
            sameSize($1,$3);
            //$$ = new CML::surfaceTensorField(*$1 / *$3);
	    $$ = new CML::surfaceTensorField(*$1);
	    (*$$).internalField()/(*$3).internalField();
	    delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '-' ftexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '-' fyexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '-' fhexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fyexp '-' ftexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fhexp '-' ftexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | '-' ftexp %prec TOKEN_NEG 	            {
            $$ = new CML::surfaceTensorField(-*$2);
            delete $2;
            driver.setCalculatedPatches(*$$);
          }
        | '(' ftexp ')'		        { $$ = $2; }
        | TOKEN_skew '(' ftexp ')' 	            {
            $$ = new CML::surfaceTensorField( CML::skew(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_eigenVectors '(' ftexp ')'       {
            $$ = new CML::surfaceTensorField(CML::eigenVectors(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_eigenVectors '(' fyexp ')'       {
            $$ = driver.makeField<CML::surfaceTensorField>(
                CML::eigenVectors($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_inv '(' ftexp ')' 	            {
            $$ = new CML::surfaceTensorField( CML::inv(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_cof '(' ftexp ')' 	            {
            $$ = driver.makeField<CML::surfaceTensorField>(
                CML::cof($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_dev '(' ftexp ')' 	            {
            $$ = new CML::surfaceTensorField( CML::dev(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_dev2 '(' ftexp ')' 	            {
            $$ = new CML::surfaceTensorField( CML::dev2(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ftexp '.' tensorComponentSwitch TOKEN_transpose '(' ')'  {
            $$ = new CML::surfaceTensorField( $1->T() );
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | flexp '?' ftexp ':' ftexp                   {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
            driver.setCalculatedPatches(*$$);}
        | TOKEN_snGrad '(' texp ')'            {
            $$ = new CML::surfaceTensorField(CML::fvc::snGrad(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_interpolate '(' texp ')'            {
            $$ = new CML::surfaceTensorField(CML::fvc::interpolate(*$3));
            delete $3;
          }
        | TOKEN_min '(' ftexp ',' ftexp  ')'         {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' ftexp ',' ftexp  ')'         {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_min '(' ftexp ')'                  {
            $$ = driver.makeConstantField<CML::surfaceTensorField>(
                CML::min(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_max '(' ftexp ')'                  {
            $$ = driver.makeConstantField<CML::surfaceTensorField>(
                CML::max(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_sum '(' ftexp ')'                  {
            $$ = driver.makeConstantField<CML::surfaceTensorField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' ftexp ')'              {
            $$ = driver.makeConstantField<CML::surfaceTensorField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | evaluateFaceTensorFunction restOfFunction
        | TOKEN_FTID                                {
            $$=driver.getField<CML::surfaceTensorField>(*$1).ptr();
            delete $1;
          }
//        | TOKEN_ddt '(' TOKEN_FTID ')'		    {
            // $$ = CML::fvc::ddt( driver.getOrReadField<CML::surfaceTensorField>(*$3,true,true)() ).ptr();
            // delete $3;
          // } // no fvc::ddt for surface Fields
        | TOKEN_oldTime '(' TOKEN_FTID ')'	    {
            $$ = new CML::surfaceTensorField(
                driver.getOrReadField<CML::surfaceTensorField>(
                    *$3,true,true
                )->oldTime());
            delete $3;
          }
        | TOKEN_flux '(' fsexp ',' texp ')'        {
            $$ = CML::fvc::flux(*$3,*$5).ptr();
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
;

evaluateFaceTensorFunction: TOKEN_FUNCTION_FTID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::surfaceTensorField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;


fyexp:   fsymmTensor                  { $$ = $1; }
        | fyexp '+' fyexp 		          {
            sameSize($1,$3);
            $$ = new CML::surfaceSymmTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fhexp '+' fyexp 		          {
            sameSize($1,$3);
            $$ = new CML::surfaceSymmTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fyexp '+' fhexp 		          {
            sameSize($1,$3);
            $$ = new CML::surfaceSymmTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fsexp '*' fyexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::surfaceSymmTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fyexp '*' fsexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::surfaceSymmTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fyexp '&' fyexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceSymmTensorField(
                symm(*$1 & *$3)
            );
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fhexp '&' fyexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceSymmTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fyexp '&' fhexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceSymmTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fyexp '/' fsexp 	   	                  {
            sameSize($1,$3);
            //$$ = new CML::surfaceSymmTensorField(*$1 / *$3);
	    $$ = new CML::surfaceSymmTensorField(*$1);
	    (*$$).internalField()/=(*$3).internalField();
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fyexp '-' fyexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceSymmTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fhexp '-' fyexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceSymmTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fyexp '-' fhexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceSymmTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | '-' fyexp %prec TOKEN_NEG 	            {
            $$ = new CML::surfaceSymmTensorField(-*$2);
            delete $2;
            driver.setCalculatedPatches(*$$);
          }
        | '(' fyexp ')'		        { $$ = $2; }
        | TOKEN_symm '(' ftexp ')' 	            {
            $$ = new CML::surfaceSymmTensorField( CML::symm(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_symm '(' fyexp ')' 	            {
            $$ = new CML::surfaceSymmTensorField( CML::symm(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_twoSymm '(' ftexp ')' 	            {
            $$ = new CML::surfaceSymmTensorField( CML::twoSymm(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_twoSymm '(' fyexp ')' 	            {
            $$ = new CML::surfaceSymmTensorField( CML::twoSymm(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_inv '(' fyexp ')' 	            {
            $$ = new CML::surfaceSymmTensorField( CML::inv(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_cof '(' fyexp ')' 	            {
            $$ = driver.makeField<CML::surfaceSymmTensorField>(
                CML::cof($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_dev '(' fyexp ')' 	            {
            $$ = new CML::surfaceSymmTensorField( CML::dev(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_dev2 '(' fyexp ')' 	            {
            $$ = new CML::surfaceSymmTensorField( CML::dev2(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_sqr '(' fvexp ')' 	            {
            $$ = new CML::surfaceSymmTensorField( CML::sqr(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fyexp '.' tensorComponentSwitch TOKEN_transpose '(' ')'  {
            $$ = $1;
          }
        | flexp '?' fyexp ':' fyexp                   {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
            driver.setCalculatedPatches(*$$);}
        | TOKEN_snGrad '(' yexp ')'            {
            $$ = new CML::surfaceSymmTensorField(CML::fvc::snGrad(*$3));
            delete $3;
            $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_interpolate '(' yexp ')'            {
            $$ = new CML::surfaceSymmTensorField(CML::fvc::interpolate(*$3));
            delete $3;
          }
        | TOKEN_min '(' fyexp ',' fyexp  ')'         {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' fyexp ',' fyexp  ')'         {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_min '(' fyexp ')'                  {
            $$ = driver.makeConstantField<CML::surfaceSymmTensorField>(
                CML::min(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_max '(' fyexp ')'                  {
            $$ = driver.makeConstantField<CML::surfaceSymmTensorField>(
                CML::max(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_sum '(' fyexp ')'                  {
            $$ = driver.makeConstantField<CML::surfaceSymmTensorField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' fyexp ')'              {
            $$ = driver.makeConstantField<CML::surfaceSymmTensorField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | evaluateFaceSymmTensorFunction restOfFunction
        | TOKEN_FYID                                {
            $$=driver.getField<CML::surfaceSymmTensorField>(*$1).ptr();
            delete $1;
          }
//        | TOKEN_ddt '(' TOKEN_FYID ')'		    {
          //   $$ = CML::fvc::ddt(
          //       driver.getOrReadField<CML::surfaceSymmTensorField>(
          //           *$3,true,true
          //       )()
          //   ).ptr();
          //   delete $3;
          // }// no fvc::ddt for surface Fields
        | TOKEN_oldTime '(' TOKEN_FYID ')'	    {
            $$ = new CML::surfaceSymmTensorField(
                driver.getOrReadField<CML::surfaceSymmTensorField>(
                    *$3,true,true
                )->oldTime()
            );
            delete $3;
          }
        | TOKEN_flux '(' fsexp ',' yexp ')'        {
            $$ = CML::fvc::flux(*$3,*$5).ptr();
            delete $3; delete $5; $$->dimensions().reset(CML::dimless);
          }
;

evaluateFaceSymmTensorFunction: TOKEN_FUNCTION_FYID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::surfaceSymmTensorField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;

fhexp:   fsphericalTensor                  { $$ = $1; }
        | fhexp '+' fhexp 		          {
            sameSize($1,$3);
            $$ = new CML::surfaceSphericalTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fsexp '*' fhexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::surfaceSphericalTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fhexp '*' fsexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::surfaceSphericalTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fhexp '&' fhexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceSphericalTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fhexp '/' fsexp 	   	                  {
            sameSize($1,$3);
            //$$ = new CML::surfaceSphericalTensorField(*$1 / *$3);
	    $$ = new CML::surfaceSphericalTensorField(*$1);
	    (*$$).internalField()/=(*$3).internalField();
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fhexp '-' fhexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::surfaceSphericalTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | '-' fhexp %prec TOKEN_NEG 	            {
            $$ = new CML::surfaceSphericalTensorField(-*$2);
            delete $2;
            driver.setCalculatedPatches(*$$);
          }
        | '(' fhexp ')'		        { $$ = $2; }
        | TOKEN_sph '(' ftexp ')'              {
            $$ = driver.makeField<CML::surfaceSphericalTensorField>(
                CML::sph($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_sph '(' fyexp ')'              {
            $$ = driver.makeField<CML::surfaceSphericalTensorField>(
                CML::sph($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_sph '(' fhexp ')'              {
            $$ = driver.makeField<CML::surfaceSphericalTensorField>(
                CML::sph($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_inv '(' fhexp ')' 	           {
            $$ = driver.makeField<CML::surfaceSphericalTensorField>(
                CML::inv($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | fhexp '.' tensorComponentSwitch TOKEN_transpose '(' ')'  {
            $$ = $1;
          }
        | flexp '?' fhexp ':' fhexp                   {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
            driver.setCalculatedPatches(*$$);}
         | TOKEN_snGrad '(' hexp ')'            {
            $$ = new CML::surfaceSphericalTensorField(CML::fvc::snGrad(*$3));
            delete $3; $$->dimensions().reset(CML::dimless);
          }
        | TOKEN_interpolate '(' hexp ')'            {
            $$ = new CML::surfaceSphericalTensorField(
                CML::fvc::interpolate(*$3)
            );
            delete $3;
          }
       | TOKEN_min '(' fhexp ',' fhexp  ')'         {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' fhexp ',' fhexp  ')'         {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_min '(' fhexp ')'                  {
            $$ = driver.makeConstantField<CML::surfaceSphericalTensorField>(
                CML::min(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_max '(' fhexp ')'                  {
            $$ = driver.makeConstantField<CML::surfaceSphericalTensorField>(
                CML::max(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_sum '(' fhexp ')'                  {
            $$ = driver.makeConstantField<CML::surfaceSphericalTensorField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' fhexp ')'              {
            $$ = driver.makeConstantField<CML::surfaceSphericalTensorField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | evaluateFaceSphericalTensorFunction restOfFunction
        | TOKEN_FHID                                {
            $$=driver.getField<CML::surfaceSphericalTensorField>(*$1).ptr();
            delete $1;
          }
//        | TOKEN_ddt '(' TOKEN_FHID ')'		    {
          //   $$ = CML::fvc::ddt( driver.getOrReadField<CML::surfaceSphericalTensorField>(*$3,true,true)()  delete $3; ).ptr();
          // } // no fvc::ddt for surface Fields
        | TOKEN_oldTime '(' TOKEN_FHID ')'	    {
            $$ = new CML::surfaceSphericalTensorField(
                driver.getOrReadField<CML::surfaceSphericalTensorField>(
                    *$3,true,true
                )->oldTime()
            );
            delete $3;
          }
        | TOKEN_flux '(' fsexp ',' hexp ')'        {
            $$ = CML::fvc::flux(*$3,*$5).ptr();
            delete $3; delete $5;
            $$->dimensions().reset(CML::dimless);
          }
;

evaluateFaceSphericalTensorFunction: TOKEN_FUNCTION_FHID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::surfaceSphericalTensorField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;

// point fields

psexp:  TOKEN_point '(' scalar ')'            {
            $$ = driver.makePointConstantField<CML::pointScalarField>(
                $3
            ).ptr();
          }
        | psexp '+' psexp 		    {
            sameSize($1,$3);
            $$ = new CML::pointScalarField(*$1 + *$3);
            delete $1; delete $3;
          }
        | psexp '*' psexp 		    {
            sameSize($1,$3);
            $$ = driver.makePointField<CML::pointScalarField>(
                $1->internalField() * $3->internalField()
            ).ptr();
            delete $1; delete $3;
          }
        | pvexp '&' pvexp 		    {
            sameSize($1,$3);
            $$ = new CML::pointScalarField(*$1 & *$3);
            delete $1; delete $3;
          }
        | ptexp TOKEN_AND ptexp 	    {
            sameSize($1,$3);
            $$ = new CML::pointScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | pyexp TOKEN_AND ptexp 	    {
            sameSize($1,$3);
            $$ = new CML::pointScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | phexp TOKEN_AND ptexp 	    {
            sameSize($1,$3);
            $$ = new CML::pointScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | ptexp TOKEN_AND pyexp 	    {
            sameSize($1,$3);
            $$ = new CML::pointScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | pyexp TOKEN_AND pyexp 	    {
            sameSize($1,$3);
            $$ = new CML::pointScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | phexp TOKEN_AND pyexp 	    {
            sameSize($1,$3);
            $$ = new CML::pointScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | ptexp TOKEN_AND phexp 	    {
            sameSize($1,$3);
            $$ = new CML::pointScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | pyexp TOKEN_AND phexp 	    {
            sameSize($1,$3);
            $$ = new CML::pointScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | phexp TOKEN_AND phexp 	    {
            sameSize($1,$3);
            $$ = new CML::pointScalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | psexp '/' psexp 		    {
            sameSize($1,$3);
            //$$ = new CML::pointScalarField(*$1 / *$3);
	    $$ = new CML::pointScalarField(*$1);
	    (*$$).internalField()/=(*$3).internalField();
            delete $1; delete $3;
          }
        | psexp '-' psexp 		    {
            sameSize($1,$3);
            $$ = new CML::pointScalarField(*$1 - *$3);
            delete $1; delete $3;}
        | TOKEN_pow '(' psexp ',' scalar ')' {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::pow($3->internalField(),$5)()
            ).ptr();
            delete $3;
          }
        | TOKEN_pow '(' psexp ',' psexp ')' {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::pow($3->internalField(),$5->internalField())()
            ).ptr();
            delete $3; delete $5;
          }
        | TOKEN_log '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::log($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_exp '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::exp($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_sqr '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::sqr($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_sqrt '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::sqrt($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_sin '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::sin($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_cos '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::cos($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_tan '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::tan($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_log10 '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::log10($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_asin '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(CML::asin(
                $3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_acos '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::acos($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_atan '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(CML::atan(
                $3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_sinh '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(CML::sinh(
                $3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_cosh '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(CML::cosh(
                $3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_tanh '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(CML::tanh(
                $3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_asinh '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::asinh($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_acosh '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(CML::acosh(
                $3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_atanh '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::atanh($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_erf '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::erf($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_erfc '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::erfc($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_lgamma '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::lgamma($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_besselJ1 '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::j1($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_besselJ0 '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::j0($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_besselY0 '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::y0($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_besselY1 '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::y1($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_sign '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::sign($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_pos '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::pos($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_neg '(' psexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::neg($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_min '(' psexp ',' psexp  ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::min($3->internalField(),$5->internalField())
            ).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' psexp ',' psexp  ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::max($3->internalField(),$5->internalField())
            ).ptr();
            delete $3; delete $5;
          }
        | TOKEN_min '(' psexp ')'            {
            $$ = driver.makePointConstantField<CML::pointScalarField>(
                CML::gMin($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_max '(' psexp ')'            {
            $$ = driver.makePointConstantField<CML::pointScalarField>(
                CML::gMax($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_sum '(' psexp ')'            {
            $$ = driver.makePointConstantField<CML::pointScalarField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' psexp ')'        {
            $$ = driver.makePointConstantField<CML::pointScalarField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | '-' psexp %prec TOKEN_NEG          {
            $$ = driver.makePointField<CML::pointScalarField>(
                -$2->internalField()
            ).ptr();
            delete $2;
          }
        | '(' psexp ')'		            { $$ = $2; }
        | pvexp '.' vectorComponentSwitch  TOKEN_x                      {
            $$ = new CML::pointScalarField($1->component(0));
            delete $1;
          }
        | pvexp '.' vectorComponentSwitch  TOKEN_y                      {
            $$ = new CML::pointScalarField($1->component(1));
            delete $1;
          }
        | pvexp '.' vectorComponentSwitch  TOKEN_z                      {
            $$ = new CML::pointScalarField($1->component(2));
            delete $1;
          }
        | ptexp '.' tensorComponentSwitch TOKEN_xx                 {
            $$ = new CML::pointScalarField($1->component(0));
            delete $1;
          }
        | ptexp '.' tensorComponentSwitch TOKEN_xy                 {
            $$ = new CML::pointScalarField($1->component(1));
            delete $1;
          }
        | ptexp '.' tensorComponentSwitch TOKEN_xz                 {
            $$ = new CML::pointScalarField($1->component(2));
            delete $1;
          }
        | ptexp '.' tensorComponentSwitch TOKEN_yx                 {
            $$ = new CML::pointScalarField($1->component(3));
            delete $1;
          }
        | ptexp '.' tensorComponentSwitch TOKEN_yy                 {
            $$ = new CML::pointScalarField($1->component(4));
            delete $1;
          }
        | ptexp '.' tensorComponentSwitch TOKEN_yz                 {
            $$ = new CML::pointScalarField($1->component(5));
            delete $1;
          }
        | ptexp '.' tensorComponentSwitch TOKEN_zx                 {
            $$ = new CML::pointScalarField($1->component(6));
            delete $1;
          }
        | ptexp '.' tensorComponentSwitch TOKEN_zy                 {
            $$ = new CML::pointScalarField($1->component(7));
            delete $1;
          }
        | ptexp '.' tensorComponentSwitch TOKEN_zz                 {
            $$ = new CML::pointScalarField($1->component(8));
            delete $1;
          }
        | pyexp '.' tensorComponentSwitch TOKEN_xx                 {
            $$ = new CML::pointScalarField($1->component(0));
            delete $1;
          }
        | pyexp '.' tensorComponentSwitch TOKEN_xy                 {
            $$ = new CML::pointScalarField($1->component(1));
            delete $1;
          }
        | pyexp '.' tensorComponentSwitch TOKEN_xz                 {
            $$ = new CML::pointScalarField($1->component(2));
            delete $1;
          }
        | pyexp '.' tensorComponentSwitch TOKEN_yy                 {
            $$ = new CML::pointScalarField($1->component(3));
            delete $1;
          }
        | pyexp '.' tensorComponentSwitch TOKEN_yz                 {
            $$ = new CML::pointScalarField($1->component(4));
            delete $1;
          }
        | pyexp '.' tensorComponentSwitch TOKEN_zz                 {
            $$ = new CML::pointScalarField($1->component(5));
            delete $1;
          }
        | phexp '.' tensorComponentSwitch TOKEN_ii                 {
            $$ = new CML::pointScalarField($1->component(0));
            delete $1;
          }
        | plexp '?' psexp ':' psexp         {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
          }
        | TOKEN_mag '(' psexp ')'            {
            $$ = new CML::pointScalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_mag '(' pvexp ')'            {
            $$ = new CML::pointScalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_mag '(' ptexp ')'            {
            $$ = new CML::pointScalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_mag '(' pyexp ')'            {
            $$ = new CML::pointScalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_mag '(' phexp ')'            {
            $$ = new CML::pointScalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' psexp ')'            {
            $$ = new CML::pointScalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' pvexp ')'            {
            $$ = new CML::pointScalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' ptexp ')'            {
            $$ = new CML::pointScalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' pyexp ')'            {
            $$ = new CML::pointScalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' phexp ')'            {
            $$ = new CML::pointScalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_tr '(' ptexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::tr($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_tr '(' pyexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::tr($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_tr '(' phexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::tr($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_det '(' ptexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::det($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_det '(' pyexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::det($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_det '(' phexp ')'            {
            $$ = driver.makePointField<CML::pointScalarField>(
                CML::det($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_interpolateToPoint '(' exp ')'               {
            $$ = driver.cellToPointInterpolate(*$3).ptr();
            delete $3;
          }
        | evaluatePointScalarFunction restOfFunction
          | TOKEN_PSID                         {
            $$ = driver.getPointField<CML::pointScalarField>(*$1).ptr();
            delete $1;
          }
          | TOKEN_oldTime '(' TOKEN_PSID ')'		    {
            $$ = new CML::pointScalarField(
                driver.getOrReadPointField<CML::pointScalarField>(
                    *$3,true,true
                )->oldTime()
            );
            delete $3;
          }
          | TOKEN_LOOKUP '(' psexp ')'	     {
            $$ = driver.makePointField<CML::pointScalarField>(
                driver.getLookup(*$1,*$3)
            ).ptr();
            delete $1; delete $3;
          }
;

evaluatePointScalarFunction: TOKEN_FUNCTION_PSID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::pointScalarField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;

pvexp:  pvector                            { $$ = $1; }
        | pvexp '+' pvexp 		   {
            sameSize($1,$3);
            $$ = new CML::pointVectorField(*$1 + *$3);
            delete $1; delete $3;
          }
        | psexp '*' pvexp 		   {
            sameSize($1,$3);
            $$ = new CML::pointVectorField(*$1 * *$3);
            delete $1; delete $3;
          }
        | pvexp '*' psexp 		   {
            sameSize($1,$3);
            $$ = new CML::pointVectorField(*$1 * *$3);
            delete $1; delete $3;
          }
        | ptexp '&' pvexp 		   {
            sameSize($1,$3);
            $$ = new CML::pointVectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | pvexp '&' ptexp 		   {
            sameSize($1,$3);
            $$ = new CML::pointVectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | phexp '&' pvexp 		   {
            sameSize($1,$3);
            $$ = new CML::pointVectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | pvexp '&' phexp 		   {
            sameSize($1,$3);
            $$ = new CML::pointVectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | pyexp '&' pvexp 		   {
            sameSize($1,$3);
            $$ = new CML::pointVectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | pvexp '&' pyexp 		   {
            sameSize($1,$3);
            $$ = new CML::pointVectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | pvexp '^' pvexp 		   {
            sameSize($1,$3);
            $$ = new CML::pointVectorField(*$1 ^ *$3);
            delete $1; delete $3;
          }
        | pvexp '/' psexp 		   {
            sameSize($1,$3);
            //$$ = new CML::pointVectorField(*$1 / *$3);
	    $$ = new CML::pointVectorField(*$1);
	    (*$$).internalField()/=(*$3).internalField();
            delete $1; delete $3;
          }
        | pvexp '-' pvexp 		   {
            sameSize($1,$3);
            $$ = new CML::pointVectorField(*$1 - *$3);
            delete $1; delete $3;}
        | '-' pvexp %prec TOKEN_NEG          {
            $$ = driver.makePointField<CML::pointVectorField>(
                -$2->internalField()
            ).ptr();
            delete $2;
          }
        | '*' ptexp %prec TOKEN_HODGE 	        {
            $$ = driver.makePointField<CML::pointVectorField>(
                *($2->internalField())
            ).ptr();
            delete $2;
          }
        | '*' pyexp %prec TOKEN_HODGE 	        {
            $$ = driver.makePointField<CML::pointVectorField>(
                *($2->internalField())
            ).ptr();
            delete $2;
          }
        | '(' pvexp ')'		           { $$ = $2; }
        | TOKEN_eigenValues '(' ptexp ')'       {
            $$ = driver.makePointField<CML::pointVectorField>(
                CML::eigenValues($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_eigenValues '(' pyexp ')'       {
            $$ = driver.makePointField<CML::pointVectorField>(
                CML::eigenValues($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '.' tensorComponentSwitch  TOKEN_x                   {
            $$ = driver.makePointVectorField(
                $1->component(CML::tensor::XX)(),
                $1->component(CML::tensor::XY)(),
                $1->component(CML::tensor::XZ)()
            ).ptr();
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '.' tensorComponentSwitch  TOKEN_y                   {
            $$ = driver.makePointVectorField(
                $1->component(CML::tensor::YX)(),
                $1->component(CML::tensor::YY)(),
                $1->component(CML::tensor::YZ)()
            ).ptr();
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '.' tensorComponentSwitch  TOKEN_z                   {
            $$ = driver.makePointVectorField(
                $1->component(CML::tensor::ZX)(),
                $1->component(CML::tensor::ZY)(),
                $1->component(CML::tensor::ZZ)()
            ).ptr();
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_diag '(' ptexp ')'       {
            $$ = driver.makePointVectorField(
                $3->component(CML::tensor::XX)(),
                $3->component(CML::tensor::YY)(),
                $3->component(CML::tensor::ZZ)()
            ).ptr();
            delete $3;
          }
        | TOKEN_diag '(' pyexp ')'       {
            $$ = driver.makePointVectorField(
                $3->component(CML::symmTensor::XX)(),
                $3->component(CML::symmTensor::YY)(),
                $3->component(CML::symmTensor::ZZ)()
            ).ptr();
            delete $3;
          }
        | plexp '?' pvexp ':' pvexp        {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
          }
        | TOKEN_pposition '(' ')'           {
            $$ = driver.makePointPositionField().ptr();
          }
        | TOKEN_interpolateToPoint '(' vexp ')'               {
            $$ = driver.cellToPointInterpolate(*$3).ptr();
            delete $3;
          }
        | TOKEN_min '(' pvexp ',' pvexp  ')'            {
            $$ = driver.makePointField<CML::pointVectorField>(
                CML::min(
                    $3->internalField(),$5->internalField()
                )
            ).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' pvexp ',' pvexp  ')'            {
            $$ = driver.makePointField<CML::pointVectorField>(
                CML::max(
                    $3->internalField(),$5->internalField()
                )
            ).ptr();
            delete $3; delete $5;
          }
        | TOKEN_min '(' pvexp ')'            {
            $$ = driver.makePointConstantField<CML::pointVectorField>(
                CML::gMin($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_minPosition '(' psexp ')'           {
            $$ = driver.makePointConstantField<CML::pointVectorField>(
                driver.getPositionOfMinimum(
                    *$3,
                    driver.mesh().points()
                )
            ).ptr();
            delete $3;
        }
        | TOKEN_max '(' pvexp ')'            {
            $$ = driver.makePointConstantField<CML::pointVectorField>(
                CML::gMax($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_maxPosition '(' psexp ')'           {
            $$ = driver.makePointConstantField<CML::pointVectorField>(
                driver.getPositionOfMaximum(
                    *$3,
                    driver.mesh().points()
                )
            ).ptr();
            delete $3;
        }
        | TOKEN_sum '(' pvexp ')'           {
            $$ = driver.makePointConstantField<CML::pointVectorField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' pvexp ')'       {
            $$ = driver.makePointConstantField<CML::pointVectorField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | evaluatePointVectorFunction restOfFunction
        | TOKEN_PVID                        {
            $$ = driver.getPointField<CML::pointVectorField>(*$1).ptr();
            delete $1;
          }
        | TOKEN_oldTime '(' TOKEN_PVID ')'		    {
            $$ = new CML::pointVectorField(
                driver.getOrReadPointField<CML::pointVectorField>(
                    *$3,true,true
                )->oldTime()
            );
            delete $3;
          }
;

evaluatePointVectorFunction: TOKEN_FUNCTION_PVID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::pointVectorField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;

ptexp:   ptensor                  { $$ = $1; }
        | ptexp '+' ptexp 		          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '+' pyexp 		          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '+' phexp 		          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | pyexp '+' ptexp 		          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | phexp '+' ptexp 		          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | psexp '*' ptexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '*' psexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | pvexp '*' pvexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '&' ptexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | pyexp '&' ptexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '&' pyexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | phexp '&' ptexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '&' phexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '/' psexp 	   	                  {
            sameSize($1,$3);
	    // $$ = new CML::pointTensorField(*$1 / *$3);
	    $$ = new CML::pointTensorField(*$1);
	    (*$$).internalField()/=(*$3).internalField();
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '-' ptexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '-' pyexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '-' phexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | pyexp '-' ptexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | phexp '-' ptexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | '-' ptexp %prec TOKEN_NEG          {
            $$ = driver.makePointField<CML::pointTensorField>(
                -$2->internalField()
            ).ptr();
            delete $2;
          }
        | '(' ptexp ')'		        { $$ = $2; }
        | TOKEN_skew '(' ptexp ')' 	            {
            $$ = new CML::pointTensorField( CML::skew(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_eigenVectors '(' ptexp ')'       {
            $$ = new CML::pointTensorField(CML::eigenVectors(*$3));
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_eigenVectors '(' pyexp ')'       {
            $$ = driver.makePointField<CML::pointTensorField>(
                CML::eigenVectors($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_inv '(' ptexp ')' 	            {
            $$ = new CML::pointTensorField( CML::inv(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_cof '(' ptexp ')' 	            {
            $$ = driver.makePointField<CML::pointTensorField>(
                CML::cof($3->internalField())
            ).ptr();
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_dev '(' ptexp ')' 	            {
            $$ = new CML::pointTensorField( CML::dev(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | TOKEN_dev2 '(' ptexp ')' 	            {
            $$ = new CML::pointTensorField( CML::dev2(*$3) );
            delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | ptexp '.' tensorComponentSwitch TOKEN_transpose '(' ')'  {
            $$ = new CML::pointTensorField( $1->T() );
            delete $1;
            driver.setCalculatedPatches(*$$);
          }
        | plexp '?' ptexp ':' ptexp                   {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
          }
        | TOKEN_interpolateToPoint '(' texp ')'               {
            $$ = driver.cellToPointInterpolate(*$3).ptr();
            delete $3;
          }
        | TOKEN_min '(' ptexp ',' ptexp  ')'            {
            $$ = driver.makePointField<CML::pointTensorField>(
                CML::min(
                    $3->internalField(),$5->internalField()
                )
            ).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' ptexp ',' ptexp  ')'            {
            $$ = driver.makePointField<CML::pointTensorField>(
                CML::max(
                    $3->internalField(),$5->internalField()
                )
            ).ptr();
            delete $3; delete $5;
          }
        | TOKEN_min '(' ptexp ')'            {
            $$ = driver.makePointConstantField<CML::pointTensorField>(
                CML::gMin($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_max '(' ptexp ')'            {
            $$ = driver.makePointConstantField<CML::pointTensorField>(
                CML::gMax($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_sum '(' ptexp ')'                  {
            $$ = driver.makePointConstantField<CML::pointTensorField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' ptexp ')'              {
            $$ = driver.makePointConstantField<CML::pointTensorField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | evaluatePointTensorFunction restOfFunction
        | TOKEN_PTID                                {
            $$=driver.getPointField<CML::pointTensorField>(*$1).ptr();
            delete $1;
          }
        | TOKEN_oldTime '(' TOKEN_PTID ')'	    {
            $$ = new CML::pointTensorField(
                driver.getOrReadPointField<CML::pointTensorField>(
                    *$3,true,true
                )->oldTime()
            );
            delete $3;
          }
;

evaluatePointTensorFunction: TOKEN_FUNCTION_PTID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::pointTensorField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;

pyexp:   psymmTensor                  { $$ = $1; }
        | pyexp '+' pyexp 		          {
            sameSize($1,$3);
            $$ = new CML::pointSymmTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | phexp '+' pyexp 		          {
            sameSize($1,$3);
     $$ = new CML::pointSymmTensorField(*$1 + *$3);
     delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | pyexp '+' phexp 		          {
            sameSize($1,$3);
            $$ = new CML::pointSymmTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | psexp '*' pyexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::pointSymmTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | pyexp '*' psexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::pointSymmTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
//         | pyexp '&' pyexp 	   	          {
//             sameSize($1,$3);
//             $$ = new CML::pointSymmTensorField(
//                 symm(*$1 & *$3)
//             );
//             delete $1; delete $3;
//             driver.setCalculatedPatches(*$$);
//           }
        | phexp '&' pyexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointSymmTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | pyexp '&' phexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointSymmTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | pyexp '/' psexp 	   	                  {
            sameSize($1,$3);
            //$$ = new CML::pointSymmTensorField(*$1 / *$3);
	    $$ = new CML::pointSymmTensorField(*$1);
	    (*$$).internalField()/=(*$3).internalField();
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | pyexp '-' pyexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointSymmTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | phexp '-' pyexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointSymmTensorField(*$1 - *$3);
     delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | pyexp '-' phexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointSymmTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | '-' pyexp %prec TOKEN_NEG          {
            $$ = driver.makePointField<CML::pointSymmTensorField>(
                -$2->internalField()
            ).ptr();
            delete $2;
          }
        | '(' pyexp ')'		        { $$ = $2;
          }
        | TOKEN_symm '(' ptexp ')'            {
            $$ = driver.makePointField<CML::pointSymmTensorField>(
                CML::symm($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_symm '(' pyexp ')'            {
            $$ = driver.makePointField<CML::pointSymmTensorField>(
                CML::symm($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_twoSymm '(' ptexp ')'            {
            $$ = driver.makePointField<CML::pointSymmTensorField>(
                CML::twoSymm($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_twoSymm '(' pyexp ')'            {
            $$ = driver.makePointField<CML::pointSymmTensorField>(
                CML::twoSymm($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_inv '(' pyexp ')'            {
            $$ = driver.makePointField<CML::pointSymmTensorField>(
                CML::inv($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_cof '(' pyexp ')'            {
            $$ = driver.makePointField<CML::pointSymmTensorField>(
                CML::cof($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_dev '(' pyexp ')'            {
            $$ = driver.makePointField<CML::pointSymmTensorField>(
                CML::dev($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_dev2 '(' pyexp ')'            {
            $$ = driver.makePointField<CML::pointSymmTensorField>(
                CML::dev2($3->internalField())()
            ).ptr();
            delete $3;
          }
        | TOKEN_sqr '(' pvexp ')'            {
            $$ = driver.makePointField<CML::pointSymmTensorField>(
                CML::sqr($3->internalField())()
            ).ptr();
            delete $3;
          }
        | pyexp '.' tensorComponentSwitch TOKEN_transpose '(' ')'  {
            $$ = $1;
          }
        | plexp '?' pyexp ':' pyexp                   {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
          }
        | TOKEN_interpolateToPoint '(' yexp ')'               {
            $$ = driver.cellToPointInterpolate(*$3).ptr();
            delete $3;
          }
        | TOKEN_min '(' pyexp ',' pyexp  ')'            {
            $$ = driver.makePointField<CML::pointSymmTensorField>(
                CML::min($3->internalField(),$5->internalField())
            ).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' pyexp ',' pyexp  ')'            {
            $$ = driver.makePointField<CML::pointSymmTensorField>(
                CML::max(
                    $3->internalField(),$5->internalField()
                )
            ).ptr();
            delete $3; delete $5;
          }
        | TOKEN_min '(' pyexp ')'            {
            $$ = driver.makePointConstantField<CML::pointSymmTensorField>(
                CML::gMin($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_max '(' pyexp ')'            {
            $$ = driver.makePointConstantField<CML::pointSymmTensorField>(
                CML::gMax($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_sum '(' pyexp ')'                  {
            $$ = driver.makePointConstantField<CML::pointSymmTensorField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' pyexp ')'              {
            $$ = driver.makePointConstantField<CML::pointSymmTensorField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | evaluatePointSymmTensorFunction restOfFunction
        | TOKEN_PYID                                {
            $$=driver.getPointField<CML::pointSymmTensorField>(*$1).ptr();
            delete $1;
          }
        | TOKEN_oldTime '(' TOKEN_PYID ')'	    {
            $$ = new CML::pointSymmTensorField(
                driver.getOrReadPointField<CML::pointSymmTensorField>(
                    *$3,true,true
                )->oldTime()
            );
            delete $3;
          }
;

evaluatePointSymmTensorFunction: TOKEN_FUNCTION_PYID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::pointSymmTensorField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;

phexp:   psphericalTensor                  { $$ = $1; }
        | phexp '+' phexp 		          {
            sameSize($1,$3);
            $$ = new CML::pointSphericalTensorField(*$1 + *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | psexp '*' phexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::pointSphericalTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | phexp '*' psexp 	   	                  {
            sameSize($1,$3);
            $$ = new CML::pointSphericalTensorField(*$1 * *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | phexp '&' phexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointSphericalTensorField(*$1 & *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | phexp '/' psexp 	   	                  {
            sameSize($1,$3);
	    // $$ = new CML::pointSphericalTensorField(*$1 / *$3);
	    $$ = new CML::pointSphericalTensorField(*$1);
	    (*$$).internalField()/=(*$3).internalField();
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | phexp '-' phexp 	   	          {
            sameSize($1,$3);
            $$ = new CML::pointSphericalTensorField(*$1 - *$3);
            delete $1; delete $3;
            driver.setCalculatedPatches(*$$);
          }
        | '-' phexp %prec TOKEN_NEG          {
            $$ = driver.makePointField<CML::pointSphericalTensorField>(
                -$2->internalField()
            ).ptr();
            delete $2;
          }
        | '(' phexp ')'		        { $$ = $2; }
        | TOKEN_sph '(' ptexp ')'              {
            $$ = driver.makePointField<CML::pointSphericalTensorField>(
                CML::sph($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_sph '(' pyexp ')'              {
            $$ = driver.makePointField<CML::pointSphericalTensorField>(
                CML::sph($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_sph '(' phexp ')'              {
            $$ = driver.makePointField<CML::pointSphericalTensorField>(
                CML::sph($3->internalField())
            ).ptr();
            delete $3;
          }
        | TOKEN_inv '(' phexp ')'            {
            $$ = driver.makePointField<CML::pointSphericalTensorField>(
                CML::inv($3->internalField())()
            ).ptr();
            delete $3;
          }
        | phexp '.' tensorComponentSwitch TOKEN_transpose '(' ')'  {
            $$ = $1;
          }
        | plexp '?' phexp ':' phexp                   {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(
                *$1,*$3,*$5
            ).ptr();
            delete $1; delete $3; delete $5;
          }
        | TOKEN_interpolateToPoint '(' hexp ')'               {
            $$ = driver.cellToPointInterpolate(*$3).ptr();
            delete $3;
          }
        | TOKEN_min '(' phexp ',' phexp  ')'            {
            $$ = driver.makePointField<CML::pointSphericalTensorField>(
                CML::min(
                    $3->internalField(),$5->internalField()
                )
            ).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' phexp ',' phexp  ')'            {
            $$ = driver.makePointField<CML::pointSphericalTensorField>(
                CML::max(
                    $3->internalField(),$5->internalField()
                )
            ).ptr();
            delete $3; delete $5;
          }
        | TOKEN_min '(' phexp ')'            {
            $$ = driver.makePointConstantField<CML::pointSphericalTensorField>(
                CML::gMin(
                    $3->internalField()
                )
            ).ptr();
            delete $3;
          }
        | TOKEN_max '(' phexp ')'            {
            $$ = driver.makePointConstantField<CML::pointSphericalTensorField>(
                CML::gMax(
                    $3->internalField()
                )
            ).ptr();
            delete $3;
          }
        | TOKEN_sum '(' phexp ')'                  {
            $$ = driver.makePointConstantField<CML::pointSphericalTensorField>(
                CML::sum(*$3).value()
            ).ptr();
            delete $3;
          }
        | TOKEN_average '(' phexp ')'              {
            $$ = driver.makePointConstantField<CML::pointSphericalTensorField>(
                CML::average(*$3).value()
            ).ptr();
            delete $3;
          }
        | evaluatePointSphericalTensorFunction restOfFunction
        | TOKEN_PHID                                {
            $$=driver.getPointField<CML::pointSphericalTensorField>(*$1).ptr();
            delete $1;
          }
        | TOKEN_oldTime '(' TOKEN_PHID ')'	    {
            $$ = new CML::pointSphericalTensorField(
                driver.getOrReadPointField<CML::pointSphericalTensorField>(
                    *$3,true,true
                )->oldTime()
            );
            delete $3;
          }
;

evaluatePointSphericalTensorFunction: TOKEN_FUNCTION_PHID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::pointSphericalTensorField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;

plexp: TOKEN_point '(' TOKEN_TRUE ')'   {
            $$ = driver.makePointConstantField<CML::pointScalarField>(1).ptr();
          }
    | TOKEN_point '(' TOKEN_FALSE ')'   {
            $$ = driver.makePointConstantField<CML::pointScalarField>(0).ptr();
          }
    | TOKEN_pset '(' TOKEN_PSETID ')'    {
        $$ = driver.makePointSetField(*$3).ptr();
        delete $3;
      }
    | TOKEN_pzone '(' TOKEN_PZONEID ')'  {
        $$ = driver.makePointZoneField(*$3).ptr();
        delete $3;
      }
    | psexp '<' psexp                 {
            sameSize($1,$3);
            $$ = driver.doPointCompare(
                *$1,
                std::less<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | psexp '>' psexp                 {
            sameSize($1,$3);
            $$ = driver.doPointCompare(
                *$1,
                std::greater<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | psexp TOKEN_LEQ psexp           {
            sameSize($1,$3);
            $$ = driver.doPointCompare(
                *$1,
                std::less_equal<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | psexp TOKEN_GEQ psexp           {
            sameSize($1,$3);
            $$ = driver.doPointCompare(
                *$1,std::greater_equal<CML::scalar>(),*$3
            ).ptr();
            delete $1; delete $3;
          }
    | psexp TOKEN_EQ psexp            {
            sameSize($1,$3);
            $$ = driver.doPointCompare(
                *$1,std::equal_to<CML::scalar>(),*$3
            ).ptr();
            delete $1; delete $3;
          }
    | psexp TOKEN_NEQ psexp           {
            sameSize($1,$3);
            $$ = driver.doPointCompare(
                *$1,std::not_equal_to<CML::scalar>(),*$3
            ).ptr();
            delete $1; delete $3;
          }
    | '(' plexp ')'		      { $$ = $2; }
    | plexp TOKEN_AND plexp           {
            sameSize($1,$3);
            $$ = driver.doPointLogicalOp(
                *$1,std::logical_and<CML::scalar>(),*$3
            ).ptr();
            delete $1; delete $3;
          }
    | plexp TOKEN_OR plexp            {
            sameSize($1,$3);
            $$ = driver.doPointLogicalOp(
                *$1,std::logical_or<CML::scalar>(),*$3
            ).ptr();
            delete $1; delete $3;
          }
    | '!' plexp %prec TOKEN_NOT        {
            $$ = driver.doPointLogicalNot(*$2).ptr();
            delete $2;
          }
    | evaluatePointLogicalFunction restOfFunction
;

evaluatePointLogicalFunction: TOKEN_FUNCTION_PLID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::pointScalarField>(
          *$1,
          @2,
          numberOfFunctionChars
      ).ptr();
      delete $1;
  }
;


vector: TOKEN_VECTOR '(' exp ',' exp ',' exp ')' {
          $$ = driver.makeVectorField(*$3,*$5,*$7).ptr();
          delete $3; delete $5; delete $7;
        }
;

tensor: TOKEN_TENSOR '(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')' {
          $$ = driver.makeTensorField(
              *$3,*$5,*$7,
              *$9,*$11,*$13,
              *$15,*$17,*$19
          ).ptr();
          delete $3; delete $5; delete $7; delete $9; delete $11;
          delete $13; delete $15; delete $17; delete $19;
        }
;

symmTensor: TOKEN_SYMM_TENSOR '(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')' {
              $$ = driver.makeSymmTensorField(
                  *$3,*$5,*$7,
                  *$9,*$11,
                  *$13
              ).ptr();
              delete $3; delete $5; delete $7; delete $9;
              delete $11; delete $13;
          }
;

sphericalTensor: TOKEN_SPHERICAL_TENSOR '(' exp ')' {
            $$ = driver.makeSphericalTensorField(*$3).ptr();
            delete $3;
          }
;

fvector: TOKEN_VECTOR '(' fsexp ',' fsexp ',' fsexp ')' {
            $$ = driver.makeSurfaceVectorField(*$3,*$5,*$7).ptr();
            delete $3; delete $5; delete $7;
          }
;

ftensor: TOKEN_TENSOR '(' fsexp ',' fsexp ',' fsexp ',' fsexp ',' fsexp ',' fsexp ',' fsexp ',' fsexp ',' fsexp ')' {
           $$ = driver.makeSurfaceTensorField(
               *$3,*$5,*$7,
               *$9,*$11,*$13,
               *$15,*$17,*$19
           ).ptr();
           delete $3; delete $5; delete $7; delete $9; delete $11; delete $13;
           delete $15; delete $17; delete $19;
          }
;

fsymmTensor: TOKEN_SYMM_TENSOR '(' fsexp ',' fsexp ',' fsexp ',' fsexp ',' fsexp ',' fsexp ')' {
                $$ = driver.makeSurfaceSymmTensorField(
                    *$3,*$5,*$7,
                    *$9,*$11,
                    *$13
                ).ptr();

                delete $3; delete $5; delete $7; delete $9;
                delete $11; delete $13;
          }
;

fsphericalTensor: TOKEN_SPHERICAL_TENSOR '(' fsexp ')' {
            $$ = driver.makeSurfaceSphericalTensorField(*$3).ptr();
            delete $3;
          }
;

pvector: TOKEN_VECTOR '(' psexp ',' psexp ',' psexp ')' {
            $$ = driver.makePointVectorField(*$3,*$5,*$7).ptr();
            delete $3; delete $5; delete $7;
          }
;

ptensor: TOKEN_TENSOR '(' psexp ',' psexp ',' psexp ',' psexp ',' psexp ',' psexp ',' psexp ',' psexp ',' psexp ')' {
           $$ = driver.makePointTensorField(
               *$3,*$5,*$7,
               *$9,*$11,*$13,
               *$15,*$17,*$19
           ).ptr();

           delete $3; delete $5; delete $7; delete $9; delete $11;
           delete $13; delete $15; delete $17; delete $19;
          }
;

psymmTensor: TOKEN_SYMM_TENSOR '(' psexp ',' psexp ',' psexp ',' psexp ',' psexp ',' psexp ')' {
                $$ = driver.makePointSymmTensorField(
                    *$3,*$5,*$7,
                    *$9,*$11,
                    *$13
                ).ptr();

                delete $3; delete $5; delete $7; delete $9;
                delete $11; delete $13;
          }
;

psphericalTensor: TOKEN_SPHERICAL_TENSOR '(' psexp ')' {
              $$ = driver.makePointSphericalTensorField(*$3).ptr();
              delete $3;
          }
;

%%

void parserField::FieldValueExpressionParser::error (
    const parserField::FieldValueExpressionParser::location_type& l,
    const std::string& m
)
{
    driver.error (l, m);
}
