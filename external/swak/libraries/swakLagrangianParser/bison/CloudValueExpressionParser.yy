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
    2010-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

%skeleton "lalr1.cc"
/* %require "2.1a" */
%defines
%define "parser_class_name" "CloudValueExpressionParser"

%pure-parser

%{
#include <volFields.hpp>

#include <functional>
#include <cmath>

    namespace CML {
        class CloudValueExpressionDriver;
    }

    const CML::scalar HugeVal=1e40;

    using CML::CloudValueExpressionDriver;

    void yyerror(char *);

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

%}

%name-prefix="parserCloud"

%parse-param {void * scanner}
%parse-param { CloudValueExpressionDriver& driver }
%parse-param { int start_token }
%parse-param { int numberOfFunctionChars }
%lex-param {void * scanner}
%lex-param { CloudValueExpressionDriver& driver }
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
    CML::word *name;
    CML::vectorField *vfield;
    CML::scalarField *sfield;
    CML::Field<bool> *lfield;
    CML::tensorField *tfield;
    CML::symmTensorField *yfield;
    CML::sphericalTensorField *hfield;
};

%{
#include "CloudValueExpressionDriverYY.hpp"
#include "swakChecks.hpp"
#include "CommonPluginFunction.hpp"

namespace CML {
    template<class T>
    autoPtr<Field<T> > CloudValueExpressionDriver::evaluatePluginFunction(
        const word &name,
        const parserCloud::location &loc,
        int &scanned,
        bool isPoint
    ) {
        if(debug || traceParsing()) {
            Info << "Excuting plugin-function " << name << " ( returning type "
                << pTraits<T>::typeName << ") on " << this->content()
                << " position "
                << loc.end.column-1 << endl;
        }

        autoPtr<CommonPluginFunction> theFunction(
            this->newPluginFunction(
                name
            )
        );

        //    scanned+=1;

        autoPtr<Field<T> > result(
            theFunction->evaluate<T>(
                this->content().substr(
                    loc.end.column-1
                ),
                scanned,
                isPoint
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
%token <name>   TOKEN_VID   "vectorID"
%token <name>   TOKEN_LID   "logicalID"
%token <name>   TOKEN_TID   "tensorID"
%token <name>   TOKEN_YID   "symmTensorID"
%token <name>   TOKEN_HID   "sphericalTensorID"
%token <name>   TOKEN_GAS_SID   "G_scalarID"
%token <name>   TOKEN_GAS_VID   "G_vectorID"
%token <name>   TOKEN_GAS_TID   "G_tensorID"
%token <name>   TOKEN_GAS_YID   "G_symmTensorID"
%token <name>   TOKEN_GAS_HID   "G_sphericalTensorID"
%token <name>   TOKEN_FUNCTION_SID   "F_scalarID"
%token <name>   TOKEN_FUNCTION_VID   "F_vectorID"
%token <name>   TOKEN_FUNCTION_TID   "F_tensorID"
%token <name>   TOKEN_FUNCTION_YID   "F_symmTensorID"
%token <name>   TOKEN_FUNCTION_HID   "F_sphericalTensorID"
%token <name>   TOKEN_FUNCTION_LID   "F_logicalID"
%token <name> TOKEN_SETID "cellSetID"
%token <name> TOKEN_ZONEID "cellZoneID"
%token <val>    TOKEN_NUM   "value"
%token <integer>    TOKEN_INT   "integer"
%token <vec>    TOKEN_VEC   "vector"
%type  <val>    sreduced;
%type  <vec>    vreduced;
%type  <sfield>    exp        "expression"
%type  <lfield>    lexp       "lexpression"
%type  <vfield>    vexp       "vexpression"
%type  <vfield>    vector
%type  <tfield>    tensor
%type  <tfield>    texp       "texpression"
%type  <yfield>    symmTensor
%type  <yfield>    yexp       "yexpression"
%type  <hfield>    sphericalTensor
%type  <hfield>    hexp       "hexpression"

%type  <sfield> evaluateScalarFunction;
%type  <vfield> evaluateVectorFunction;
%type  <tfield> evaluateTensorFunction;
%type  <yfield> evaluateSymmTensorFunction;
%type  <hfield> evaluateSphericalTensorFunction;
%type  <lfield> evaluateLogicalFunction;

%token START_DEFAULT

%token START_CLOUD_SCALAR_COMMA
%token START_CLOUD_SCALAR_CLOSE
%token START_CLOUD_VECTOR_COMMA
%token START_CLOUD_VECTOR_CLOSE
%token START_CLOUD_TENSOR_COMMA
%token START_CLOUD_TENSOR_CLOSE
%token START_CLOUD_YTENSOR_COMMA
%token START_CLOUD_YTENSOR_CLOSE
%token START_CLOUD_HTENSOR_COMMA
%token START_CLOUD_HTENSOR_CLOSE
%token START_CLOUD_LOGICAL_COMMA
%token START_CLOUD_LOGICAL_CLOSE

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

%token TOKEN_fluidPhase

%token TOKEN_unitTensor
%token TOKEN_pi
%token TOKEN_rand
%token TOKEN_randFixed
%token TOKEN_id
%token TOKEN_randNormal
%token TOKEN_randNormalFixed
%token TOKEN_position

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
%token TOKEN_average
%token TOKEN_sum
%token TOKEN_sqr
%token TOKEN_sqrt

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

%token TOKEN_cpu
%token TOKEN_weight

%token TOKEN_set
%token TOKEN_zone

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

%destructor          { delete $$; } <vec> <name>
    <vfield> <sfield> <lfield> <tfield> <yfield> <hfield>
%destructor          {} <val> <integer>

%printer             { debug_stream () << *$$; } <name>
%printer             {
    CML::OStringStream buff;
    buff << *$$;
    debug_stream () << buff.str().c_str();
} <vec>
%printer             { debug_stream () << $$; } <val> <integer>
%printer             {
    debug_stream () << "<No name field>" << $$;
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
} <vfield> <sfield> <lfield> <tfield> <yfield> <hfield>


%%
%start switch_start;

switch_start: switch_expr
              { driver.parserLastPos()=@1.end.column; }
;

switch_expr:      START_DEFAULT unit
                | START_CLOUD_SCALAR_COMMA exp ','
                  {
                      driver.setResult<CML::scalar>($2);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_CLOUD_SCALAR_CLOSE exp ')'
                  {
                      driver.setResult<CML::scalar>($2);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_CLOUD_VECTOR_COMMA vexp ','
                  {
                      driver.setResult<CML::vector>($2);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_CLOUD_VECTOR_CLOSE vexp ')'
                  {
                      driver.setResult<CML::vector>($2);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_CLOUD_TENSOR_COMMA texp ','
                  {
                      driver.setResult<CML::tensor>($2);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_CLOUD_TENSOR_CLOSE texp ')'
                  {
                      driver.setResult<CML::tensor>($2);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_CLOUD_YTENSOR_COMMA yexp ','
                  {
                      driver.setResult<CML::symmTensor>($2);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_CLOUD_YTENSOR_CLOSE yexp ')'
                  {
                      driver.setResult<CML::symmTensor>($2);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_CLOUD_HTENSOR_COMMA hexp ','
                  {
                      driver.setResult<CML::sphericalTensor>($2);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_CLOUD_HTENSOR_CLOSE hexp ')'
                  {
                      driver.setResult<CML::sphericalTensor>($2);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_CLOUD_LOGICAL_COMMA lexp ','
                  {
                      driver.setResult<bool>($2);
                      driver.parserLastPos()=@3.end.column-1;
                      YYACCEPT;
                  }
                | START_CLOUD_LOGICAL_CLOSE lexp ')'
                  {
                      driver.setResult<bool>($2);
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

restOfFunction:    TOKEN_LAST_FUNCTION_CHAR
                   | TOKEN_IN_FUNCTION_CHAR restOfFunction;

unit:   exp                     { driver.setResult<CML::scalar>($1);  }
        | vexp                  { driver.setResult<CML::vector>($1);  }
        | lexp                  { driver.setResult<bool>($1); }
        | texp                  { driver.setResult<CML::tensor>($1);  }
        | yexp                  { driver.setResult<CML::symmTensor>($1);  }
        | hexp                  {
            driver.setResult<CML::sphericalTensor>($1);
          }
;

vectorComponentSwitch: /* empty rule */{ driver.startVectorComponent(); }
;

tensorComponentSwitch: /* empty rule */{ driver.startTensorComponent(); }
;

eatCharactersSwitch: /* empty rule */{ driver.startEatCharacters(); }
;

vexp:   vector                  { $$ = $1; }
        | vreduced              {
            $$ = driver.makeField<CML::vector>(*$1).ptr();
            delete $1;
          }
        | vexp '+' vexp 		{
            sameSize($1,$3);
            $$ = new CML::vectorField(*$1 + *$3);
            delete $1; delete $3;
          }
        | exp '*' vexp 		        {
            sameSize($1,$3);
            $$ = new CML::vectorField(*$1 * *$3);
            delete $1; delete $3;
          }
        | vexp '*' exp 		        {
            sameSize($1,$3);
            $$ = new CML::vectorField(*$1 * *$3);
            delete $1; delete $3;
          }
        | texp '&' vexp 		{
            sameSize($1,$3);
            $$ = new CML::vectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | vexp '&' texp 		{
            sameSize($1,$3);
            $$ = new CML::vectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | yexp '&' vexp 		{
            sameSize($1,$3);
            $$ = new CML::vectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | vexp '&' yexp 		{
            sameSize($1,$3);
            $$ = new CML::vectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | hexp '&' vexp 		{
            sameSize($1,$3);
            $$ = new CML::vectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | vexp '&' hexp 		{
            sameSize($1,$3);
            $$ = new CML::vectorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | vexp '/' exp 		        {
            sameSize($1,$3);
            $$ = new CML::vectorField(*$1 / *$3);
            delete $1; delete $3;
          }
        | vexp '^' vexp 		{
            sameSize($1,$3);
            $$ = new CML::vectorField(*$1 ^ *$3);
            delete $1; delete $3;
          }
        | vexp '-' vexp 		{
            sameSize($1,$3);
            $$ = new CML::vectorField(*$1 - *$3);
            delete $1; delete $3;
          }
        | '-' vexp %prec TOKEN_NEG 	        {
            $$ = new CML::vectorField(-*$2);
            delete $2;
          }
        | '*' texp %prec TOKEN_HODGE 	        {
            $$ = new CML::vectorField(*(*$2));
            delete $2;
          }
        | '*' yexp %prec TOKEN_HODGE 	        {
            $$ = new CML::vectorField(*(*$2));
            delete $2;
          }
        | '(' vexp ')'		        { $$ = $2; }
        | TOKEN_eigenValues '(' texp ')'       {
            $$ = new CML::vectorField(CML::eigenValues(*$3));
            delete $3;
          }
        | TOKEN_eigenValues '(' yexp ')'       {
            $$ = new CML::vectorField(CML::eigenValues(*$3));
            delete $3;
          }
        | texp '.' tensorComponentSwitch  TOKEN_x     {
            $$ = driver.composeVectorField(
                $1->component(CML::tensor::XX)(),
                $1->component(CML::tensor::XY)(),
                $1->component(CML::tensor::XZ)()
            ).ptr();
            delete $1;
          }
        | texp '.' tensorComponentSwitch  TOKEN_y     {
            $$ = driver.composeVectorField(
                $1->component(CML::tensor::YX)(),
                $1->component(CML::tensor::YY)(),
                $1->component(CML::tensor::YZ)()
            ).ptr();
            delete $1;
          }
        | texp '.' tensorComponentSwitch  TOKEN_z     {
            $$ = driver.composeVectorField(
                $1->component(CML::tensor::ZX)(),
                $1->component(CML::tensor::ZY)(),
                $1->component(CML::tensor::ZZ)()
            ).ptr();
            delete $1;
          }
        | TOKEN_diag '(' texp ')'       {
            //            $$ = new CML::vectorField( CML::diag(*$3) ); // not implemented?
            $$ = driver.composeVectorField(
                $3->component(CML::tensor::XX)(),
                $3->component(CML::tensor::YY)(),
                $3->component(CML::tensor::ZZ)()
            ).ptr();
            delete $3;
          }
        | TOKEN_diag '(' yexp ')'       {
            //            $$ = new CML::vectorField( CML::diag(*$3) ); // not implemented?
            $$ = driver.composeVectorField(
                $3->component(CML::symmTensor::XX)(),
                $3->component(CML::symmTensor::YY)(),
                $3->component(CML::symmTensor::ZZ)()
            ).ptr();
            delete $3;
          }
        | lexp '?' vexp ':' vexp        {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(*$1,*$3,*$5).ptr();
            delete $1; delete $3; delete $5;
          }
        | TOKEN_position '(' ')'        {
            $$ = driver.makePositionField().ptr();
          }
        | evaluateVectorFunction restOfFunction
        | TOKEN_VID {
            $$=driver.getVectorField(*$1).ptr();
            delete $1;
                    }
        | TOKEN_min '(' vexp ',' vexp  ')'           {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' vexp ',' vexp  ')'           {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
          }
	| TOKEN_fluidPhase '(' TOKEN_GAS_VID ')'		{
            $$=driver.getFluidField<CML::vector>(*$3).ptr();
            delete $3;
          }
;

evaluateVectorFunction: TOKEN_FUNCTION_VID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::vector>(
          *$1,
          @2,
          numberOfFunctionChars,
          false
      ).ptr();
      delete $1;
  }
;

sreduced: TOKEN_min '(' exp ')'       {
            $$ = CML::gMin(*$3);
            delete $3;
          }
        | TOKEN_max '(' exp ')'       {
            $$ = CML::gMax(*$3);
            delete $3;
          }
        | TOKEN_sum '(' exp ')'       {
            $$ = CML::gSum(*$3);
            delete $3;
          }
        | TOKEN_average '(' exp ')'   {
            $$ = CML::gAverage(*$3);
            delete $3;
          }
;

vreduced: TOKEN_min '(' vexp ')'       {
          CML::vector tmp(HugeVal,HugeVal,HugeVal);
            if(($3->size())>0) {
                tmp=CML::min(*$3);
            }
            CML::reduce(tmp,CML::minOp<CML::vector>());
            $$ = new CML::vector(tmp);
            delete $3;
          }
        | TOKEN_max '(' vexp ')'       {
            CML::vector tmp(-HugeVal,-HugeVal,-HugeVal);
            if(($3->size())>0) {
                tmp=CML::max(*$3);
            }
            CML::reduce(tmp,CML::maxOp<CML::vector>());
            $$ = new CML::vector(tmp);
            delete $3;
          }
        | TOKEN_minPosition '(' exp ')'           {
            CML::vectorField *pos=driver.makePositionField().ptr();
            $$ = new CML::vector(
                driver.getPositionOfMinimum(
                    *$3,
                    *pos
                )
            );
            delete pos;
            delete $3;
        }
        | TOKEN_maxPosition '(' exp ')'           {
            CML::vectorField *pos=driver.makePositionField().ptr();
            $$ = new CML::vector(
                driver.getPositionOfMaximum(
                    *$3,
                    *pos
                )
            );
            delete pos;
            delete $3;
        }
        | TOKEN_sum '(' vexp ')'       {
            $$ = new CML::vector(CML::gSum(*$3));
            delete $3;
          }
        | TOKEN_average '(' vexp ')'   {
            $$ = new CML::vector(CML::gAverage(*$3));
            delete $3;
          }
;

exp:    TOKEN_NUM                  { $$ = driver.makeField($1).ptr(); }
        | sreduced                 { $$ = driver.makeField($1).ptr(); }
        | exp '+' exp 		{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 + *$3);
            delete $1; delete $3;
          }
        | exp '-' exp 		{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 - *$3);
            delete $1; delete $3;
          }
        | exp '*' exp 		{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 * *$3);
            delete $1; delete $3;
          }
        | exp '%' exp 		{
            sameSize($1,$3);
            $$ = driver.makeModuloField(*$1,*$3).ptr();
            delete $1; delete $3;
          }
        | exp '/' exp 		{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 / *$3);
            delete $1; delete $3;
          }
// Produces a reduce/reduce-conflict
        // | TOKEN_pow '(' exp ',' scalar ')'		{
        //     $$ = new CML::scalarField(CML::pow(*$3, $5));
        //     delete $3;
        //   }
        | TOKEN_pow '(' exp ',' exp ')'		{
            sameSize($3,$5);
            $$ = new CML::scalarField(CML::pow(*$3, *$5));
            delete $3; delete $5;
          }
        | TOKEN_log '(' exp ')'       {
            $$ = new CML::scalarField(CML::log(*$3));
            delete $3;
          }
        | TOKEN_exp '(' exp ')'       {
            $$ = new CML::scalarField(CML::exp(*$3));
            delete $3;
          }
        | vexp '&' vexp 	{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 & *$3);
            delete $1; delete $3;
          }
        | texp TOKEN_AND texp 	{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | texp TOKEN_AND yexp 	{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | texp TOKEN_AND hexp 	{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | yexp TOKEN_AND texp 	{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | yexp TOKEN_AND yexp 	{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | yexp TOKEN_AND hexp 	{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | hexp TOKEN_AND texp 	{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | hexp TOKEN_AND yexp 	{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | hexp TOKEN_AND hexp 	{
            sameSize($1,$3);
            $$ = new CML::scalarField(*$1 && *$3);
            delete $1; delete $3;
          }
        | '-' exp %prec TOKEN_NEG 	{
            $$ = new CML::scalarField(-*$2);
            delete $2;
          }
	| '(' exp ')'		{ $$ = $2; }
        | TOKEN_sqr '(' exp ')'       {
            $$ = new CML::scalarField(CML::sqr(*$3));
            delete $3;
          }
        | TOKEN_sqrt '(' exp ')'       {
            $$ = new CML::scalarField(CML::sqrt(*$3));
            delete $3;
          }
        | TOKEN_sin '(' exp ')'       {
            $$ = new CML::scalarField(CML::sin(*$3));
            delete $3;
          }
        | TOKEN_cos '(' exp ')'       {
            $$ = new CML::scalarField(CML::cos(*$3));
            delete $3;
          }
        | TOKEN_tan '(' exp ')'       {
            $$ = new CML::scalarField(CML::tan(*$3));
            delete $3;
          }
        | TOKEN_log10 '(' exp ')'         {
            $$ = new CML::scalarField(CML::log10(*$3));
            delete $3;
          }
        | TOKEN_asin '(' exp ')'          {
            $$ = new CML::scalarField(CML::asin(*$3));
            delete $3;
          }
        | TOKEN_acos '(' exp ')'          {
            $$ = new CML::scalarField(CML::acos(*$3));
            delete $3;
          }
        | TOKEN_atan '(' exp ')'          {
            $$ = new CML::scalarField(CML::atan(*$3));
            delete $3;
          }
        | TOKEN_sinh '(' exp ')'          {
            $$ = new CML::scalarField(CML::sinh(*$3));
            delete $3;
          }
        | TOKEN_cosh '(' exp ')'          {
            $$ = new CML::scalarField(CML::cosh(*$3));
            delete $3;
          }
        | TOKEN_tanh '(' exp ')'          {
            $$ = new CML::scalarField(CML::tanh(*$3));
            delete $3;
          }
        | TOKEN_asinh '(' exp ')'         {
            $$ = new CML::scalarField(CML::asinh(*$3));
            delete $3;
          }
        | TOKEN_acosh '(' exp ')'         {
            $$ = new CML::scalarField(CML::acosh(*$3));
            delete $3;
          }
        | TOKEN_atanh '(' exp ')'         {
            $$ = new CML::scalarField(CML::atanh(*$3));
            delete $3;
          }
        | TOKEN_erf '(' exp ')'           {
            $$ = new CML::scalarField(CML::erf(*$3));
            delete $3;
          }
        | TOKEN_erfc '(' exp ')'          {
            $$ = new CML::scalarField(CML::erfc(*$3));
            delete $3;
          }
        | TOKEN_lgamma '(' exp ')'        {
            $$ = new CML::scalarField(CML::lgamma(*$3));
            delete $3;
          }
        | TOKEN_besselJ0 '(' exp ')'      {
            $$ = new CML::scalarField(CML::j0(*$3));
            delete $3;
          }
        | TOKEN_besselJ1 '(' exp ')'      {
            $$ = new CML::scalarField(CML::j1(*$3));
            delete $3;
          }
        | TOKEN_besselY0 '(' exp ')'      {
            $$ = new CML::scalarField(CML::y0(*$3));
            delete $3;
          }
        | TOKEN_besselY1 '(' exp ')'      {
            $$ = new CML::scalarField(CML::y1(*$3));
            delete $3;
          }
        | TOKEN_sign '(' exp ')'          {
            $$ = new CML::scalarField(CML::sign(*$3));
            delete $3;
          }
        | TOKEN_pos '(' exp ')'           {
            $$ = new CML::scalarField(CML::pos(*$3));
            delete $3;
          }
        | TOKEN_neg '(' exp ')'           {
            $$ = new CML::scalarField(CML::neg(*$3));
            delete $3;
          }
        | TOKEN_mag '(' exp ')'       {
            $$ = new CML::scalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_mag '(' vexp ')'      {
            $$ = new CML::scalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_mag '(' texp ')'      {
            $$ = new CML::scalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_mag '(' yexp ')'      {
            $$ = new CML::scalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_mag '(' hexp ')'      {
            $$ = new CML::scalarField(CML::mag(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' exp ')'       {
            $$ = new CML::scalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' vexp ')'      {
            $$ = new CML::scalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' texp ')'      {
            $$ = new CML::scalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' yexp ')'      {
            $$ = new CML::scalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_magSqr '(' hexp ')'      {
            $$ = new CML::scalarField(CML::magSqr(*$3));
            delete $3;
          }
        | TOKEN_tr '(' texp ')'       {
            $$ = new CML::scalarField( CML::tr(*$3) );
            delete $3;
          }
        | TOKEN_tr '(' yexp ')'       {
            $$ = new CML::scalarField( CML::tr(*$3) );
            delete $3;
          }
        | TOKEN_tr '(' hexp ')'       {
            $$ = new CML::scalarField( CML::tr(*$3) );
            delete $3;
          }
        | TOKEN_det '(' texp ')'      {
            $$ = new CML::scalarField( CML::det(*$3) );
            delete $3;
          }
        | TOKEN_det '(' yexp ')'      {
            $$ = new CML::scalarField( CML::det(*$3) );
            delete $3;
          }
        | TOKEN_det '(' hexp ')'      {
            $$ = new CML::scalarField( CML::det(*$3) );
            delete $3;
          }
        | vexp '.' vectorComponentSwitch TOKEN_x            {
            $$ = new CML::scalarField($1->component(0));
            delete $1;
          }
        | vexp '.' vectorComponentSwitch TOKEN_y            {
            $$ = new CML::scalarField($1->component(1));
            delete $1;
          }
        | vexp '.' vectorComponentSwitch TOKEN_z            {
            $$ = new CML::scalarField($1->component(2));
            delete $1;
          }
        | texp '.' tensorComponentSwitch TOKEN_xx       {
            $$ = new CML::scalarField($1->component(0));
            delete $1;
          }
        | texp '.' tensorComponentSwitch TOKEN_xy       {
            $$ = new CML::scalarField($1->component(1));
            delete $1;
          }
        | texp '.' tensorComponentSwitch TOKEN_xz       {
            $$ = new CML::scalarField($1->component(2));
            delete $1;
          }
        | texp '.' tensorComponentSwitch TOKEN_yx       {
            $$ = new CML::scalarField($1->component(3));
            delete $1;
          }
        | texp '.' tensorComponentSwitch TOKEN_yy       {
            $$ = new CML::scalarField($1->component(4));
            delete $1;
          }
        | texp '.' tensorComponentSwitch TOKEN_yz       {
            $$ = new CML::scalarField($1->component(5));
            delete $1;
          }
        | texp '.' tensorComponentSwitch TOKEN_zx       {
            $$ = new CML::scalarField($1->component(6));
            delete $1;
          }
        | texp '.' tensorComponentSwitch TOKEN_zy       {
            $$ = new CML::scalarField($1->component(7));
            delete $1;
          }
        | texp '.' tensorComponentSwitch TOKEN_zz       {
            $$ = new CML::scalarField($1->component(8));
            delete $1;
          }
        | yexp '.' tensorComponentSwitch TOKEN_xx       {
            $$ = new CML::scalarField($1->component(0));
            delete $1;
          }
        | yexp '.' tensorComponentSwitch TOKEN_xy       {
            $$ = new CML::scalarField($1->component(1));
            delete $1;
          }
        | yexp '.' tensorComponentSwitch TOKEN_xz       {
            $$ = new CML::scalarField($1->component(2));
            delete $1;
          }
        | yexp '.' tensorComponentSwitch TOKEN_yy       {
            $$ = new CML::scalarField($1->component(3));
            delete $1;
          }
        | yexp '.' tensorComponentSwitch TOKEN_yz       {
            $$ = new CML::scalarField($1->component(4));
            delete $1;
          }
        | yexp '.' tensorComponentSwitch TOKEN_zz       {
            $$ = new CML::scalarField($1->component(5));
            delete $1;
          }
        | hexp '.' tensorComponentSwitch TOKEN_ii       {
            $$ = new CML::scalarField($1->component(0));
            delete $1;
          }
        | lexp '?' exp ':' exp        {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(*$1,*$3,*$5).ptr();
            delete $1; delete $3; delete $5;
          }
        | TOKEN_pi {
	  $$ = driver.makeField(
            CML::constant::mathematical::pi
          ).ptr();
          }
        | TOKEN_id '(' ')'                         {
            $$ = driver.makeIdField().ptr();
          }
        | TOKEN_cpu '(' ')'       {
            $$ = driver.makeField(
                CML::scalar(CML::Pstream::myProcNo())
            ).ptr();
          }
        | TOKEN_weight'(' ')'                          {
            $$ = driver.weights(driver.size()).ptr();
          }
        | TOKEN_rand '(' ')'        { $$ = driver.makeRandomField().ptr(); }
        | TOKEN_rand '(' TOKEN_INT ')'        {
            $$ = driver.makeRandomField(-$3).ptr();
          }
        | TOKEN_randNormal '(' ')'        {
            $$ = driver.makeGaussRandomField().ptr();
          }
        | TOKEN_randNormal '(' TOKEN_INT ')'        {
            $$ = driver.makeGaussRandomField(-$3).ptr();
          }
        | TOKEN_randFixed '(' ')'        {
            $$ = driver.makeRandomField(1).ptr();
          }
        | TOKEN_randFixed '(' TOKEN_INT ')'        {
            $$ = driver.makeRandomField($3+1).ptr();
          }
        | TOKEN_randNormalFixed '(' ')'        {
            $$ = driver.makeGaussRandomField(1).ptr();
          }
        | TOKEN_randNormalFixed '(' TOKEN_INT ')'        {
            $$ = driver.makeGaussRandomField($3+1).ptr();
          }
        | TOKEN_deltaT '(' ')'   {
            $$ = driver.makeField(driver.runTime().deltaT().value()).ptr();
          }
        | TOKEN_time '(' ')'   {
            $$ = driver.makeField(driver.runTime().time().value()).ptr();
          }
//        | TOKEN_toFace '(' pexp ')'        {
          //   $$ = driver.toFace(*$3);
          //   delete $3;
          // }
        | evaluateScalarFunction restOfFunction
	| TOKEN_SID		{
            $$=driver.getScalarField(*$1).ptr();
            delete $1;
				}
	| TOKEN_LINE		{
            $$=driver.getLine(*$1,driver.runTime().time().value()).ptr();
            delete $1;
				}
	| TOKEN_LOOKUP '(' exp ')' {
            $$=driver.getLookup(*$1,*$3).ptr();
            delete $1; delete$3;
                                    }
        | TOKEN_min '(' exp ',' exp  ')'           {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' exp ',' exp  ')'           {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
          }
	| TOKEN_fluidPhase '(' TOKEN_GAS_SID ')'		{
            $$=driver.getFluidField<CML::scalar>(*$3).ptr();
            delete $3;
          }
;

evaluateScalarFunction: TOKEN_FUNCTION_SID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::scalar>(
          *$1,
          @2,
          numberOfFunctionChars,
          false
      ).ptr();
      delete $1;
  }
;

texp:   tensor                  { $$ = $1; }
        | texp '+' texp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 + *$3);
            delete $1; delete $3;
          }
        | texp '+' yexp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 + *$3);
            delete $1; delete $3;
          }
        | texp '+' hexp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 + *$3);
            delete $1; delete $3;
          }
        | yexp '+' texp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 + *$3);
            delete $1; delete $3;
          }
        | hexp '+' texp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 + *$3);
            delete $1; delete $3;
          }
        | exp '*' texp 		        {
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 * *$3);
            delete $1; delete $3;
          }
        | texp '*' exp 		        {
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 * *$3);
            delete $1; delete $3;
          }
        | vexp '*' vexp 	        {
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 * *$3);
            delete $1; delete $3;
          }
        | texp '&' texp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | yexp '&' texp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | texp '&' yexp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | hexp '&' texp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | texp '&' hexp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | texp '/' exp 		        {
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 / *$3);
            delete $1; delete $3;
          }
        | texp '-' texp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 - *$3);
            delete $1; delete $3;
          }
        | texp '-' yexp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 - *$3);
            delete $1; delete $3;
          }
        | texp '-' hexp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 - *$3);
            delete $1; delete $3;
          }
        | yexp '-' texp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 - *$3);
            delete $1; delete $3;
          }
        | hexp '-' texp 		{
            sameSize($1,$3);
            $$ = new CML::tensorField(*$1 - *$3);
            delete $1; delete $3;
          }
        | '-' texp %prec TOKEN_NEG 	        {
            $$ = new CML::tensorField(-*$2);
            delete $2;
          }
        | '(' texp ')'		        { $$ = $2; }
        | TOKEN_skew '(' texp ')'       {
            $$ = new CML::tensorField( CML::skew(*$3) );
            delete $3;
          }
        | TOKEN_eigenVectors '(' texp ')'       {
            $$ = new CML::tensorField(CML::eigenVectors(*$3));
            delete $3;
          }
        | TOKEN_eigenVectors '(' yexp ')'       {
            $$ = new CML::tensorField(CML::eigenVectors(*$3));
            delete $3;
          }
        | TOKEN_inv '(' texp ')'       {
            $$ = new CML::tensorField( CML::inv(*$3) );
            delete $3;
          }
        | TOKEN_cof '(' texp ')'       {
            $$ = new CML::tensorField( CML::cof(*$3) );
            delete $3;
          }
        | TOKEN_dev '(' texp ')'       {
            $$ = new CML::tensorField( CML::dev(*$3) );
            delete $3;
          }
        | TOKEN_dev2 '(' texp ')'       {
            $$ = new CML::tensorField( CML::dev2(*$3) );
            delete $3;
          }
        | texp '.' tensorComponentSwitch TOKEN_transpose '(' ')'  {
            $$ = new CML::tensorField( $1->T() );
            delete $1;
          }
        | lexp '?' texp ':' texp        {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(*$1,*$3,*$5).ptr();
            delete $1; delete $3; delete $5;
          }
        | evaluateTensorFunction restOfFunction
        | TOKEN_TID {
            $$=driver.getTensorField(*$1).ptr();
            delete $1;
                    }
        | TOKEN_min '(' texp ',' texp  ')'           {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' texp ',' texp  ')'           {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
          }
	| TOKEN_fluidPhase '(' TOKEN_GAS_TID ')'		{
            $$=driver.getFluidField<CML::tensor>(*$3).ptr();
            delete $3;
          }
;

evaluateTensorFunction: TOKEN_FUNCTION_TID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::tensor>(
          *$1,
          @2,
          numberOfFunctionChars,
          false
      ).ptr();
      delete $1;
  }
;


yexp:   symmTensor                  { $$ = $1; }
        | yexp '+' yexp 		{
            sameSize($1,$3);
            $$ = new CML::symmTensorField(*$1 + *$3);
            delete $1; delete $3;
          }
        | yexp '+' hexp 		{
            sameSize($1,$3);
            $$ = new CML::symmTensorField(*$1 + *$3);
            delete $1; delete $3;
          }
        | hexp '+' yexp 		{
            sameSize($1,$3);
            $$ = new CML::symmTensorField(*$1 + *$3);
            delete $1; delete $3;
          }
        | exp '*' yexp 		        {
            sameSize($1,$3);
            $$ = new CML::symmTensorField(*$1 * *$3);
            delete $1; delete $3;
          }
        | yexp '*' exp 		        {
            sameSize($1,$3);
            $$ = new CML::symmTensorField(*$1 * *$3);
            delete $1; delete $3;
          }
        | yexp '&' yexp 		{
            sameSize($1,$3);
            $$ = new CML::symmTensorField(
                symm(*$1 & *$3)
            );
            delete $1; delete $3;
          }
        | hexp '&' yexp 		{
            sameSize($1,$3);
            $$ = new CML::symmTensorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | yexp '&' hexp 		{
            sameSize($1,$3);
            $$ = new CML::symmTensorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | yexp '/' exp 		        {
            sameSize($1,$3);
            $$ = new CML::symmTensorField(*$1 / *$3);
            delete $1; delete $3;
          }
        | yexp '-' yexp 		{
            sameSize($1,$3);
            $$ = new CML::symmTensorField(*$1 - *$3);
            delete $1; delete $3;
          }
        | yexp '-' hexp 		{
            sameSize($1,$3);
            $$ = new CML::symmTensorField(*$1 - *$3);
            delete $1; delete $3;
          }
        | hexp '-' yexp 		{
            sameSize($1,$3);
            $$ = new CML::symmTensorField(*$1 - *$3);
            delete $1; delete $3;
          }
        | '-' yexp %prec TOKEN_NEG 	        {
            $$ = new CML::symmTensorField(-*$2);
            delete $2;
          }
        | '(' yexp ')'		        { $$ = $2; }
        | TOKEN_symm '(' yexp ')'       {
            $$ = new CML::symmTensorField( CML::symm(*$3) );
            delete $3;
          }
        | TOKEN_symm '(' texp ')'       {
            $$ = new CML::symmTensorField( CML::symm(*$3) );
            delete $3;
          }
        | TOKEN_twoSymm '(' yexp ')'       {
            $$ = new CML::symmTensorField( CML::twoSymm(*$3) );
            delete $3;
          }
        | TOKEN_twoSymm '(' texp ')'       {
            $$ = new CML::symmTensorField( CML::twoSymm(*$3) );
            delete $3;
          }
        | TOKEN_inv '(' yexp ')'       {
            $$ = new CML::symmTensorField( CML::inv(*$3) );
            delete $3;
          }
        | TOKEN_cof '(' yexp ')'       {
            $$ = new CML::symmTensorField( CML::cof(*$3) );
            delete $3;
          }
        | TOKEN_dev '(' yexp ')'       {
            $$ = new CML::symmTensorField( CML::dev(*$3) );
            delete $3;
          }
        | TOKEN_dev2 '(' yexp ')'       {
            $$ = new CML::symmTensorField( CML::dev2(*$3) );
            delete $3;
          }
        | TOKEN_sqr '(' vexp ')'       {
            $$ = new CML::symmTensorField( CML::sqr(*$3) );
            delete $3;
          }
        | yexp '.' tensorComponentSwitch TOKEN_transpose '(' ')'  {
            $$ = $1;
          }
        | lexp '?' yexp ':' yexp        {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(*$1,*$3,*$5).ptr();
            delete $1; delete $3; delete $5;
          }
        | evaluateSymmTensorFunction restOfFunction
        | TOKEN_YID {
            $$=driver.getSymmTensorField(*$1).ptr();
            delete $1;
                    }
        | TOKEN_min '(' yexp ',' yexp  ')'           {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' yexp ',' yexp  ')'           {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
          }
	| TOKEN_fluidPhase '(' TOKEN_GAS_YID ')'		{
            $$=driver.getFluidField<CML::symmTensor>(*$3).ptr();
            delete $3;
          }
;

evaluateSymmTensorFunction: TOKEN_FUNCTION_YID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::symmTensor>(
          *$1,
          @2,
          numberOfFunctionChars,
          false
      ).ptr();
      delete $1;
  }
;


hexp:   sphericalTensor                  { $$ = $1; }
        | TOKEN_unitTensor                        {
            $$ = driver.makeField(CML::sphericalTensor(1)).ptr();
          }
        | hexp '+' hexp 		{
            sameSize($1,$3);
            $$ = new CML::sphericalTensorField(*$1 + *$3);
            delete $1; delete $3;
          }
        | exp '*' hexp 		        {
            sameSize($1,$3);
            $$ = new CML::sphericalTensorField(*$1 * *$3);
            delete $1; delete $3;
          }
        | hexp '*' exp 		        {
            sameSize($1,$3);
            $$ = new CML::sphericalTensorField(*$1 * *$3);
            delete $1; delete $3;
          }
        | hexp '&' hexp 		{
            sameSize($1,$3);
            $$ = new CML::sphericalTensorField(*$1 & *$3);
            delete $1; delete $3;
          }
        | hexp '/' exp 		        {
            sameSize($1,$3);
            $$ = new CML::sphericalTensorField(*$1 / *$3);
            delete $1; delete $3;
          }
        | hexp '-' hexp 		{
            sameSize($1,$3);
            $$ = new CML::sphericalTensorField(*$1 - *$3);
            delete $1; delete $3;
          }
        | '-' hexp %prec TOKEN_NEG 	        {
            $$ = new CML::sphericalTensorField(-*$2);
            delete $2;
          }
        | '(' hexp ')'		        { $$ = $2; }
        | TOKEN_sph '(' texp ')'       {
            $$ = new CML::sphericalTensorField( CML::sph(*$3) );
            delete $3;
          }
        | TOKEN_sph '(' yexp ')'       {
            $$ = new CML::sphericalTensorField( CML::sph(*$3) );
            delete $3;
          }
        | TOKEN_sph '(' hexp ')'       {
            $$ = new CML::sphericalTensorField( CML::sph(*$3) );
            delete $3;
          }
        | TOKEN_inv '(' hexp ')'       {
            $$ = new CML::sphericalTensorField( CML::inv(*$3) );
            delete $3;
          }
        | hexp '.' tensorComponentSwitch TOKEN_transpose '(' ')'  {
            $$ = $1;
          }
        | lexp '?' hexp ':' hexp        {
            sameSize($1,$3); sameSize($1,$5);
            $$ = driver.doConditional(*$1,*$3,*$5).ptr();
            delete $1; delete $3; delete $5;
          }
//        | TOKEN_toFace '(' phexp ')'        {
          //   $$ = driver.toFace(*$3);
          //   delete $3;
          // }
        | evaluateSphericalTensorFunction restOfFunction
        | TOKEN_HID {
            $$=driver.getSphericalTensorField(*$1).ptr();
            delete $1;
                    }
        | TOKEN_min '(' hexp ',' hexp  ')'           {
            $$ = CML::min(*$3,*$5).ptr();
            delete $3; delete $5;
          }
        | TOKEN_max '(' hexp ',' hexp  ')'           {
            $$ = CML::max(*$3,*$5).ptr();
            delete $3; delete $5;
          }
	| TOKEN_fluidPhase '(' TOKEN_GAS_HID ')'		{
            $$=driver.getFluidField<CML::sphericalTensor>(*$3).ptr();
            delete $3;
          }
;

evaluateSphericalTensorFunction: TOKEN_FUNCTION_HID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<CML::sphericalTensor>(
          *$1,
          @2,
          numberOfFunctionChars,
          false
      ).ptr();
      delete $1;
  }
;


lexp: TOKEN_TRUE   { $$ = driver.makeField(true).ptr(); }
    | TOKEN_FALSE  { $$ = driver.makeField(false).ptr(); }
    | TOKEN_set '(' TOKEN_SETID ')'    {
        $$ = driver.makeCellSetField(*$3).ptr();
        delete $3;
      }
    | TOKEN_zone '(' TOKEN_ZONEID ')'  {
        $$ = driver.makeCellZoneField(*$3).ptr();
        delete $3;
      }
    | exp '<' exp  {
            sameSize($1,$3);
            $$ = driver.doCompare(*$1,std::less<CML::scalar>(),*$3).ptr();
            delete $1; delete $3;
          }
    | exp '>' exp  {
            sameSize($1,$3);
            $$ = driver.doCompare(*$1,std::greater<CML::scalar>(),*$3).ptr();
            delete $1; delete $3;
          }
    | exp TOKEN_LEQ exp  {
            sameSize($1,$3);
            $$ = driver.doCompare(*$1,std::less_equal<CML::scalar>(),*$3).ptr();
            delete $1; delete $3;
          }
    | exp TOKEN_GEQ exp  {
            sameSize($1,$3);
            $$ = driver.doCompare(
                *$1,
                std::greater_equal<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | exp TOKEN_EQ exp  {
            sameSize($1,$3);
            $$ = driver.doCompare(*$1,std::equal_to<CML::scalar>(),*$3).ptr();
            delete $1; delete $3;
          }
    | exp TOKEN_NEQ exp  {
            sameSize($1,$3);
            $$ = driver.doCompare(
                *$1,
                std::not_equal_to<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | '(' lexp ')'		{ $$ = $2; }
    | lexp TOKEN_AND lexp  {
            sameSize($1,$3);
            $$ = driver.doLogicalOp(
                *$1,
                std::logical_and<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | lexp TOKEN_OR lexp   {
            sameSize($1,$3);
            $$ = driver.doLogicalOp(
                *$1,
                std::logical_or<CML::scalar>(),
                *$3
            ).ptr();
            delete $1; delete $3;
          }
    | '!' lexp %prec TOKEN_NOT {
            $$ = driver.doLogicalNot(*$2).ptr();
            delete $2;
          }
        | evaluateLogicalFunction restOfFunction
    | TOKEN_LID		{
            $$=driver.getBoolField(*$1).ptr();
            delete $1;
      }
;

evaluateLogicalFunction: TOKEN_FUNCTION_LID '(' eatCharactersSwitch
  {
      $$=driver.evaluatePluginFunction<bool>(
          *$1,
          @2,
          numberOfFunctionChars,
          false
      ).ptr();
      delete $1;
  }
;


vector: TOKEN_VECTOR '(' exp ',' exp ',' exp ')' {
            $$ = driver.composeVectorField(*$3,*$5,*$7).ptr();
            delete $3; delete $5; delete $7;
          }
;

tensor: TOKEN_TENSOR '(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')' {
            $$ = driver.composeTensorField(
                *$3,*$5,*$7,
                *$9,*$11,*$13,
                *$15,*$17,*$19
            ).ptr();
            delete $3; delete $5; delete $7; delete $9; delete $11;
            delete $13; delete $15; delete $17; delete $19;
          }

symmTensor: TOKEN_SYMM_TENSOR '(' exp ',' exp ',' exp ',' exp ',' exp ',' exp ')' {
            $$ = driver.composeSymmTensorField(
                *$3,*$5,*$7,
                *$9,*$11,
                *$13
            ).ptr();
            delete $3; delete $5; delete $7; delete $9; delete $11; delete $13;
          }

sphericalTensor: TOKEN_SPHERICAL_TENSOR '(' exp ')' {
            $$ = driver.composeSphericalTensorField(*$3).ptr();
            delete $3;
          }

%%

void parserCloud::CloudValueExpressionParser::error (
    const parserCloud::CloudValueExpressionParser::location_type& l,
    const std::string& m
)
{
     driver.error (l, m);
}
