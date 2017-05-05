/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

// Take the name prefix into account.
#define yylex   parserPatchlex

/* First part of user declarations.  */

/* Line 293 of lalr1.cc  */
#line 40 "../PatchValueExpressionParser.yy"

#include <volFields.hpp>

#include <functional>
#include <cmath>

    namespace CML {
        class PatchValueExpressionDriver;
    }

    const CML::scalar HugeVal=1e40;

    using CML::PatchValueExpressionDriver;

#include "PatchValuePluginFunction.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"



/* Line 293 of lalr1.cc  */
#line 62 "PatchValueExpressionParser.tab.cc"


#include "PatchValueExpressionParser.tab.hh"

/* User implementation prologue.  */

/* Line 299 of lalr1.cc  */
#line 96 "../PatchValueExpressionParser.yy"

#include "PatchValueExpressionDriverYY.hpp"
#include "swakChecks.hpp"
namespace CML {
    template<class T>
    autoPtr<Field<T> > PatchValueExpressionDriver::evaluatePluginFunction(
        const word &name,
        const parserPatch::location &loc,
        int &scanned,
        bool isPoint
    ) {
        if(debug || traceParsing()) {
            Info << "Excuting plugin-function " << name << " ( returning type "
                << pTraits<T>::typeName << ") on " << this->content()
                << " position "
                << loc.end.column-1 << endl;
        }

        autoPtr<PatchValuePluginFunction> theFunction(
            PatchValuePluginFunction::New(
                *this,
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



/* Line 299 of lalr1.cc  */
#line 119 "PatchValueExpressionParser.tab.cc"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                               \
 do                                                                    \
   if (N)                                                              \
     {                                                                 \
       (Current).begin = YYRHSLOC (Rhs, 1).begin;                      \
       (Current).end   = YYRHSLOC (Rhs, N).end;                        \
     }                                                                 \
   else                                                                \
     {                                                                 \
       (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;        \
     }                                                                 \
 while (false)
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace parserPatch {

/* Line 382 of lalr1.cc  */
#line 205 "PatchValueExpressionParser.tab.cc"

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  PatchValueExpressionParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  PatchValueExpressionParser::PatchValueExpressionParser (void * scanner_yyarg, PatchValueExpressionDriver& driver_yyarg, int start_token_yyarg, int numberOfFunctionChars_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg),
      start_token (start_token_yyarg),
      numberOfFunctionChars (numberOfFunctionChars_yyarg)
  {
  }

  PatchValueExpressionParser::~PatchValueExpressionParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  PatchValueExpressionParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
        case 3: /* "\"timeline\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 283 "PatchValueExpressionParser.tab.cc"
	break;
      case 4: /* "\"lookup\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 292 "PatchValueExpressionParser.tab.cc"
	break;
      case 5: /* "\"scalarID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 301 "PatchValueExpressionParser.tab.cc"
	break;
      case 6: /* "\"vectorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 310 "PatchValueExpressionParser.tab.cc"
	break;
      case 7: /* "\"logicalID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 319 "PatchValueExpressionParser.tab.cc"
	break;
      case 8: /* "\"tensorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 328 "PatchValueExpressionParser.tab.cc"
	break;
      case 9: /* "\"symmTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 337 "PatchValueExpressionParser.tab.cc"
	break;
      case 10: /* "\"sphericalTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 346 "PatchValueExpressionParser.tab.cc"
	break;
      case 11: /* "\"pointScalarID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 355 "PatchValueExpressionParser.tab.cc"
	break;
      case 12: /* "\"pointVectorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 364 "PatchValueExpressionParser.tab.cc"
	break;
      case 13: /* "\"pointLogicalID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 373 "PatchValueExpressionParser.tab.cc"
	break;
      case 14: /* "\"pointTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 382 "PatchValueExpressionParser.tab.cc"
	break;
      case 15: /* "\"pointSymmTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 391 "PatchValueExpressionParser.tab.cc"
	break;
      case 16: /* "\"pointSphericalTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 400 "PatchValueExpressionParser.tab.cc"
	break;
      case 17: /* "\"F_scalarID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 409 "PatchValueExpressionParser.tab.cc"
	break;
      case 18: /* "\"F_pointScalarID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 418 "PatchValueExpressionParser.tab.cc"
	break;
      case 19: /* "\"F_vectorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 427 "PatchValueExpressionParser.tab.cc"
	break;
      case 20: /* "\"F_pointVectorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 436 "PatchValueExpressionParser.tab.cc"
	break;
      case 21: /* "\"F_tensorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 445 "PatchValueExpressionParser.tab.cc"
	break;
      case 22: /* "\"F_pointTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 454 "PatchValueExpressionParser.tab.cc"
	break;
      case 23: /* "\"F_symmTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 463 "PatchValueExpressionParser.tab.cc"
	break;
      case 24: /* "\"F_pointSymmTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 472 "PatchValueExpressionParser.tab.cc"
	break;
      case 25: /* "\"F_sphericalTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 481 "PatchValueExpressionParser.tab.cc"
	break;
      case 26: /* "\"F_pointSphericalTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 490 "PatchValueExpressionParser.tab.cc"
	break;
      case 27: /* "\"F_logicalID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 499 "PatchValueExpressionParser.tab.cc"
	break;
      case 28: /* "\"F_pointLogicalID\"" */

/* Line 449 of lalr1.cc  */
#line 368 "../PatchValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 508 "PatchValueExpressionParser.tab.cc"
	break;
      case 29: /* "\"value\"" */

/* Line 449 of lalr1.cc  */
#line 374 "../PatchValueExpressionParser.yy"
	{ debug_stream () << (yyvaluep->val); };

/* Line 449 of lalr1.cc  */
#line 517 "PatchValueExpressionParser.tab.cc"
	break;
      case 30: /* "\"integer\"" */

/* Line 449 of lalr1.cc  */
#line 374 "../PatchValueExpressionParser.yy"
	{ debug_stream () << (yyvaluep->integer); };

/* Line 449 of lalr1.cc  */
#line 526 "PatchValueExpressionParser.tab.cc"
	break;
      case 31: /* "\"vector\"" */

/* Line 449 of lalr1.cc  */
#line 369 "../PatchValueExpressionParser.yy"
	{
    CML::OStringStream buff;
    buff << *(yyvaluep->vec);
    debug_stream () << buff.str().c_str();
};

/* Line 449 of lalr1.cc  */
#line 539 "PatchValueExpressionParser.tab.cc"
	break;
      case 32: /* "\"sexpression\"" */

/* Line 449 of lalr1.cc  */
#line 374 "../PatchValueExpressionParser.yy"
	{ debug_stream () << (yyvaluep->val); };

/* Line 449 of lalr1.cc  */
#line 548 "PatchValueExpressionParser.tab.cc"
	break;
      case 33: /* "\"expression\"" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->sfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 559 "PatchValueExpressionParser.tab.cc"
	break;
      case 34: /* "\"pexpression\"" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->sfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 570 "PatchValueExpressionParser.tab.cc"
	break;
      case 35: /* "\"lexpression\"" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->lfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 581 "PatchValueExpressionParser.tab.cc"
	break;
      case 36: /* "\"plexpression\"" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->lfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 592 "PatchValueExpressionParser.tab.cc"
	break;
      case 37: /* "\"vexpression\"" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->vfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 603 "PatchValueExpressionParser.tab.cc"
	break;
      case 38: /* "\"pvexpression\"" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->vfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 614 "PatchValueExpressionParser.tab.cc"
	break;
      case 39: /* "\"texpression\"" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->tfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 625 "PatchValueExpressionParser.tab.cc"
	break;
      case 40: /* "\"ptexpression\"" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->tfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 636 "PatchValueExpressionParser.tab.cc"
	break;
      case 41: /* "\"yexpression\"" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->yfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 647 "PatchValueExpressionParser.tab.cc"
	break;
      case 42: /* "\"pyexpression\"" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->yfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 658 "PatchValueExpressionParser.tab.cc"
	break;
      case 43: /* "\"hexpression\"" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->hfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 669 "PatchValueExpressionParser.tab.cc"
	break;
      case 44: /* "\"phexpression\"" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->hfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 680 "PatchValueExpressionParser.tab.cc"
	break;
      case 204: /* "vexp" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->vfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 691 "PatchValueExpressionParser.tab.cc"
	break;
      case 205: /* "evaluateVectorFunction" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->vfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 702 "PatchValueExpressionParser.tab.cc"
	break;
      case 206: /* "scalar" */

/* Line 449 of lalr1.cc  */
#line 374 "../PatchValueExpressionParser.yy"
	{ debug_stream () << (yyvaluep->val); };

/* Line 449 of lalr1.cc  */
#line 711 "PatchValueExpressionParser.tab.cc"
	break;
      case 207: /* "sreduced" */

/* Line 449 of lalr1.cc  */
#line 374 "../PatchValueExpressionParser.yy"
	{ debug_stream () << (yyvaluep->val); };

/* Line 449 of lalr1.cc  */
#line 720 "PatchValueExpressionParser.tab.cc"
	break;
      case 208: /* "vreduced" */

/* Line 449 of lalr1.cc  */
#line 369 "../PatchValueExpressionParser.yy"
	{
    CML::OStringStream buff;
    buff << *(yyvaluep->vec);
    debug_stream () << buff.str().c_str();
};

/* Line 449 of lalr1.cc  */
#line 733 "PatchValueExpressionParser.tab.cc"
	break;
      case 209: /* "exp" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->sfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 744 "PatchValueExpressionParser.tab.cc"
	break;
      case 210: /* "evaluateScalarFunction" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->sfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 755 "PatchValueExpressionParser.tab.cc"
	break;
      case 211: /* "texp" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->tfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 766 "PatchValueExpressionParser.tab.cc"
	break;
      case 212: /* "evaluateTensorFunction" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->tfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 777 "PatchValueExpressionParser.tab.cc"
	break;
      case 213: /* "yexp" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->yfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 788 "PatchValueExpressionParser.tab.cc"
	break;
      case 214: /* "evaluateSymmTensorFunction" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->yfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 799 "PatchValueExpressionParser.tab.cc"
	break;
      case 215: /* "hexp" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->hfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 810 "PatchValueExpressionParser.tab.cc"
	break;
      case 216: /* "evaluateSphericalTensorFunction" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->hfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 821 "PatchValueExpressionParser.tab.cc"
	break;
      case 217: /* "lexp" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->lfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 832 "PatchValueExpressionParser.tab.cc"
	break;
      case 218: /* "evaluateLogicalFunction" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->lfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 843 "PatchValueExpressionParser.tab.cc"
	break;
      case 219: /* "vector" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->vfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 854 "PatchValueExpressionParser.tab.cc"
	break;
      case 220: /* "tensor" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->tfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 865 "PatchValueExpressionParser.tab.cc"
	break;
      case 221: /* "symmTensor" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->yfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 876 "PatchValueExpressionParser.tab.cc"
	break;
      case 222: /* "sphericalTensor" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->hfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 887 "PatchValueExpressionParser.tab.cc"
	break;
      case 223: /* "pvexp" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->vfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 898 "PatchValueExpressionParser.tab.cc"
	break;
      case 224: /* "evaluatePointVectorFunction" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->vfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 909 "PatchValueExpressionParser.tab.cc"
	break;
      case 225: /* "pexp" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->sfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 920 "PatchValueExpressionParser.tab.cc"
	break;
      case 226: /* "evaluatePointScalarFunction" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->sfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 931 "PatchValueExpressionParser.tab.cc"
	break;
      case 227: /* "ptexp" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->tfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 942 "PatchValueExpressionParser.tab.cc"
	break;
      case 228: /* "evaluatePointTensorFunction" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->tfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 953 "PatchValueExpressionParser.tab.cc"
	break;
      case 229: /* "pyexp" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->yfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 964 "PatchValueExpressionParser.tab.cc"
	break;
      case 230: /* "evaluatePointSymmTensorFunction" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->yfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 975 "PatchValueExpressionParser.tab.cc"
	break;
      case 231: /* "phexp" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->hfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 986 "PatchValueExpressionParser.tab.cc"
	break;
      case 232: /* "evaluatePointSphericalTensorFunction" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->hfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 997 "PatchValueExpressionParser.tab.cc"
	break;
      case 233: /* "pvector" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->vfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 1008 "PatchValueExpressionParser.tab.cc"
	break;
      case 234: /* "ptensor" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->tfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 1019 "PatchValueExpressionParser.tab.cc"
	break;
      case 235: /* "psymmTensor" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->yfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 1030 "PatchValueExpressionParser.tab.cc"
	break;
      case 236: /* "psphericalTensor" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->hfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 1041 "PatchValueExpressionParser.tab.cc"
	break;
      case 237: /* "plexp" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->lfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 1052 "PatchValueExpressionParser.tab.cc"
	break;
      case 238: /* "evaluatePointLogicalFunction" */

/* Line 449 of lalr1.cc  */
#line 375 "../PatchValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->lfield) /* ->name().c_str() */ ;
};

/* Line 449 of lalr1.cc  */
#line 1063 "PatchValueExpressionParser.tab.cc"
	break;
       default:
	  break;
      }
  }


  void
  PatchValueExpressionParser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  PatchValueExpressionParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
        case 3: /* "\"timeline\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1102 "PatchValueExpressionParser.tab.cc"
	break;
      case 4: /* "\"lookup\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1111 "PatchValueExpressionParser.tab.cc"
	break;
      case 5: /* "\"scalarID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1120 "PatchValueExpressionParser.tab.cc"
	break;
      case 6: /* "\"vectorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1129 "PatchValueExpressionParser.tab.cc"
	break;
      case 7: /* "\"logicalID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1138 "PatchValueExpressionParser.tab.cc"
	break;
      case 8: /* "\"tensorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1147 "PatchValueExpressionParser.tab.cc"
	break;
      case 9: /* "\"symmTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1156 "PatchValueExpressionParser.tab.cc"
	break;
      case 10: /* "\"sphericalTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1165 "PatchValueExpressionParser.tab.cc"
	break;
      case 11: /* "\"pointScalarID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1174 "PatchValueExpressionParser.tab.cc"
	break;
      case 12: /* "\"pointVectorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1183 "PatchValueExpressionParser.tab.cc"
	break;
      case 13: /* "\"pointLogicalID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1192 "PatchValueExpressionParser.tab.cc"
	break;
      case 14: /* "\"pointTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1201 "PatchValueExpressionParser.tab.cc"
	break;
      case 15: /* "\"pointSymmTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1210 "PatchValueExpressionParser.tab.cc"
	break;
      case 16: /* "\"pointSphericalTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1219 "PatchValueExpressionParser.tab.cc"
	break;
      case 17: /* "\"F_scalarID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1228 "PatchValueExpressionParser.tab.cc"
	break;
      case 18: /* "\"F_pointScalarID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1237 "PatchValueExpressionParser.tab.cc"
	break;
      case 19: /* "\"F_vectorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1246 "PatchValueExpressionParser.tab.cc"
	break;
      case 20: /* "\"F_pointVectorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1255 "PatchValueExpressionParser.tab.cc"
	break;
      case 21: /* "\"F_tensorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1264 "PatchValueExpressionParser.tab.cc"
	break;
      case 22: /* "\"F_pointTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1273 "PatchValueExpressionParser.tab.cc"
	break;
      case 23: /* "\"F_symmTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1282 "PatchValueExpressionParser.tab.cc"
	break;
      case 24: /* "\"F_pointSymmTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1291 "PatchValueExpressionParser.tab.cc"
	break;
      case 25: /* "\"F_sphericalTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1300 "PatchValueExpressionParser.tab.cc"
	break;
      case 26: /* "\"F_pointSphericalTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1309 "PatchValueExpressionParser.tab.cc"
	break;
      case 27: /* "\"F_logicalID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1318 "PatchValueExpressionParser.tab.cc"
	break;
      case 28: /* "\"F_pointLogicalID\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1327 "PatchValueExpressionParser.tab.cc"
	break;
      case 29: /* "\"value\"" */

/* Line 480 of lalr1.cc  */
#line 366 "../PatchValueExpressionParser.yy"
	{};

/* Line 480 of lalr1.cc  */
#line 1336 "PatchValueExpressionParser.tab.cc"
	break;
      case 30: /* "\"integer\"" */

/* Line 480 of lalr1.cc  */
#line 366 "../PatchValueExpressionParser.yy"
	{};

/* Line 480 of lalr1.cc  */
#line 1345 "PatchValueExpressionParser.tab.cc"
	break;
      case 31: /* "\"vector\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->vec); };

/* Line 480 of lalr1.cc  */
#line 1354 "PatchValueExpressionParser.tab.cc"
	break;
      case 32: /* "\"sexpression\"" */

/* Line 480 of lalr1.cc  */
#line 366 "../PatchValueExpressionParser.yy"
	{};

/* Line 480 of lalr1.cc  */
#line 1363 "PatchValueExpressionParser.tab.cc"
	break;
      case 33: /* "\"expression\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->sfield); };

/* Line 480 of lalr1.cc  */
#line 1372 "PatchValueExpressionParser.tab.cc"
	break;
      case 34: /* "\"pexpression\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->sfield); };

/* Line 480 of lalr1.cc  */
#line 1381 "PatchValueExpressionParser.tab.cc"
	break;
      case 35: /* "\"lexpression\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->lfield); };

/* Line 480 of lalr1.cc  */
#line 1390 "PatchValueExpressionParser.tab.cc"
	break;
      case 36: /* "\"plexpression\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->lfield); };

/* Line 480 of lalr1.cc  */
#line 1399 "PatchValueExpressionParser.tab.cc"
	break;
      case 37: /* "\"vexpression\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->vfield); };

/* Line 480 of lalr1.cc  */
#line 1408 "PatchValueExpressionParser.tab.cc"
	break;
      case 38: /* "\"pvexpression\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->vfield); };

/* Line 480 of lalr1.cc  */
#line 1417 "PatchValueExpressionParser.tab.cc"
	break;
      case 39: /* "\"texpression\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->tfield); };

/* Line 480 of lalr1.cc  */
#line 1426 "PatchValueExpressionParser.tab.cc"
	break;
      case 40: /* "\"ptexpression\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->tfield); };

/* Line 480 of lalr1.cc  */
#line 1435 "PatchValueExpressionParser.tab.cc"
	break;
      case 41: /* "\"yexpression\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->yfield); };

/* Line 480 of lalr1.cc  */
#line 1444 "PatchValueExpressionParser.tab.cc"
	break;
      case 42: /* "\"pyexpression\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->yfield); };

/* Line 480 of lalr1.cc  */
#line 1453 "PatchValueExpressionParser.tab.cc"
	break;
      case 43: /* "\"hexpression\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->hfield); };

/* Line 480 of lalr1.cc  */
#line 1462 "PatchValueExpressionParser.tab.cc"
	break;
      case 44: /* "\"phexpression\"" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->hfield); };

/* Line 480 of lalr1.cc  */
#line 1471 "PatchValueExpressionParser.tab.cc"
	break;
      case 204: /* "vexp" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->vfield); };

/* Line 480 of lalr1.cc  */
#line 1480 "PatchValueExpressionParser.tab.cc"
	break;
      case 205: /* "evaluateVectorFunction" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->vfield); };

/* Line 480 of lalr1.cc  */
#line 1489 "PatchValueExpressionParser.tab.cc"
	break;
      case 206: /* "scalar" */

/* Line 480 of lalr1.cc  */
#line 366 "../PatchValueExpressionParser.yy"
	{};

/* Line 480 of lalr1.cc  */
#line 1498 "PatchValueExpressionParser.tab.cc"
	break;
      case 207: /* "sreduced" */

/* Line 480 of lalr1.cc  */
#line 366 "../PatchValueExpressionParser.yy"
	{};

/* Line 480 of lalr1.cc  */
#line 1507 "PatchValueExpressionParser.tab.cc"
	break;
      case 208: /* "vreduced" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->vec); };

/* Line 480 of lalr1.cc  */
#line 1516 "PatchValueExpressionParser.tab.cc"
	break;
      case 209: /* "exp" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->sfield); };

/* Line 480 of lalr1.cc  */
#line 1525 "PatchValueExpressionParser.tab.cc"
	break;
      case 210: /* "evaluateScalarFunction" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->sfield); };

/* Line 480 of lalr1.cc  */
#line 1534 "PatchValueExpressionParser.tab.cc"
	break;
      case 211: /* "texp" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->tfield); };

/* Line 480 of lalr1.cc  */
#line 1543 "PatchValueExpressionParser.tab.cc"
	break;
      case 212: /* "evaluateTensorFunction" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->tfield); };

/* Line 480 of lalr1.cc  */
#line 1552 "PatchValueExpressionParser.tab.cc"
	break;
      case 213: /* "yexp" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->yfield); };

/* Line 480 of lalr1.cc  */
#line 1561 "PatchValueExpressionParser.tab.cc"
	break;
      case 214: /* "evaluateSymmTensorFunction" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->yfield); };

/* Line 480 of lalr1.cc  */
#line 1570 "PatchValueExpressionParser.tab.cc"
	break;
      case 215: /* "hexp" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->hfield); };

/* Line 480 of lalr1.cc  */
#line 1579 "PatchValueExpressionParser.tab.cc"
	break;
      case 216: /* "evaluateSphericalTensorFunction" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->hfield); };

/* Line 480 of lalr1.cc  */
#line 1588 "PatchValueExpressionParser.tab.cc"
	break;
      case 217: /* "lexp" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->lfield); };

/* Line 480 of lalr1.cc  */
#line 1597 "PatchValueExpressionParser.tab.cc"
	break;
      case 218: /* "evaluateLogicalFunction" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->lfield); };

/* Line 480 of lalr1.cc  */
#line 1606 "PatchValueExpressionParser.tab.cc"
	break;
      case 219: /* "vector" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->vfield); };

/* Line 480 of lalr1.cc  */
#line 1615 "PatchValueExpressionParser.tab.cc"
	break;
      case 220: /* "tensor" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->tfield); };

/* Line 480 of lalr1.cc  */
#line 1624 "PatchValueExpressionParser.tab.cc"
	break;
      case 221: /* "symmTensor" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->yfield); };

/* Line 480 of lalr1.cc  */
#line 1633 "PatchValueExpressionParser.tab.cc"
	break;
      case 222: /* "sphericalTensor" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->hfield); };

/* Line 480 of lalr1.cc  */
#line 1642 "PatchValueExpressionParser.tab.cc"
	break;
      case 223: /* "pvexp" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->vfield); };

/* Line 480 of lalr1.cc  */
#line 1651 "PatchValueExpressionParser.tab.cc"
	break;
      case 224: /* "evaluatePointVectorFunction" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->vfield); };

/* Line 480 of lalr1.cc  */
#line 1660 "PatchValueExpressionParser.tab.cc"
	break;
      case 225: /* "pexp" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->sfield); };

/* Line 480 of lalr1.cc  */
#line 1669 "PatchValueExpressionParser.tab.cc"
	break;
      case 226: /* "evaluatePointScalarFunction" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->sfield); };

/* Line 480 of lalr1.cc  */
#line 1678 "PatchValueExpressionParser.tab.cc"
	break;
      case 227: /* "ptexp" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->tfield); };

/* Line 480 of lalr1.cc  */
#line 1687 "PatchValueExpressionParser.tab.cc"
	break;
      case 228: /* "evaluatePointTensorFunction" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->tfield); };

/* Line 480 of lalr1.cc  */
#line 1696 "PatchValueExpressionParser.tab.cc"
	break;
      case 229: /* "pyexp" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->yfield); };

/* Line 480 of lalr1.cc  */
#line 1705 "PatchValueExpressionParser.tab.cc"
	break;
      case 230: /* "evaluatePointSymmTensorFunction" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->yfield); };

/* Line 480 of lalr1.cc  */
#line 1714 "PatchValueExpressionParser.tab.cc"
	break;
      case 231: /* "phexp" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->hfield); };

/* Line 480 of lalr1.cc  */
#line 1723 "PatchValueExpressionParser.tab.cc"
	break;
      case 232: /* "evaluatePointSphericalTensorFunction" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->hfield); };

/* Line 480 of lalr1.cc  */
#line 1732 "PatchValueExpressionParser.tab.cc"
	break;
      case 233: /* "pvector" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->vfield); };

/* Line 480 of lalr1.cc  */
#line 1741 "PatchValueExpressionParser.tab.cc"
	break;
      case 234: /* "ptensor" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->tfield); };

/* Line 480 of lalr1.cc  */
#line 1750 "PatchValueExpressionParser.tab.cc"
	break;
      case 235: /* "psymmTensor" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->yfield); };

/* Line 480 of lalr1.cc  */
#line 1759 "PatchValueExpressionParser.tab.cc"
	break;
      case 236: /* "psphericalTensor" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->hfield); };

/* Line 480 of lalr1.cc  */
#line 1768 "PatchValueExpressionParser.tab.cc"
	break;
      case 237: /* "plexp" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->lfield); };

/* Line 480 of lalr1.cc  */
#line 1777 "PatchValueExpressionParser.tab.cc"
	break;
      case 238: /* "evaluatePointLogicalFunction" */

/* Line 480 of lalr1.cc  */
#line 364 "../PatchValueExpressionParser.yy"
	{ delete (yyvaluep->lfield); };

/* Line 480 of lalr1.cc  */
#line 1786 "PatchValueExpressionParser.tab.cc"
	break;

	default:
	  break;
      }
  }

  void
  PatchValueExpressionParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  PatchValueExpressionParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  PatchValueExpressionParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  PatchValueExpressionParser::debug_level_type
  PatchValueExpressionParser::debug_level () const
  {
    return yydebug_;
  }

  void
  PatchValueExpressionParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  PatchValueExpressionParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  PatchValueExpressionParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  PatchValueExpressionParser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[3];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    
/* Line 565 of lalr1.cc  */
#line 73 "../PatchValueExpressionParser.yy"
{
	     // Initialize the initial location.
	     //     @$.begin.filename = @$.end.filename = &driver.file;
    numberOfFunctionChars=0;
}

/* Line 565 of lalr1.cc  */
#line 1885 "PatchValueExpressionParser.tab.cc"

    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc, scanner, driver, start_token, numberOfFunctionChars);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yy_table_value_is_error_ (yyn))
	  goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 2:

/* Line 690 of lalr1.cc  */
#line 384 "../PatchValueExpressionParser.yy"
    { driver.parserLastPos()=(yylocation_stack_[(1) - (1)]).end.column; }
    break;

  case 4:

/* Line 690 of lalr1.cc  */
#line 389 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::scalar>((yysemantic_stack_[(3) - (2)].sfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 5:

/* Line 690 of lalr1.cc  */
#line 395 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::scalar>((yysemantic_stack_[(3) - (2)].sfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 6:

/* Line 690 of lalr1.cc  */
#line 401 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::scalar>((yysemantic_stack_[(3) - (2)].sfield),true);
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 7:

/* Line 690 of lalr1.cc  */
#line 407 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::scalar>((yysemantic_stack_[(3) - (2)].sfield),true);
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 8:

/* Line 690 of lalr1.cc  */
#line 413 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::vector>((yysemantic_stack_[(3) - (2)].vfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 9:

/* Line 690 of lalr1.cc  */
#line 419 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::vector>((yysemantic_stack_[(3) - (2)].vfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 10:

/* Line 690 of lalr1.cc  */
#line 425 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::vector>((yysemantic_stack_[(3) - (2)].vfield),true);
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 11:

/* Line 690 of lalr1.cc  */
#line 431 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::vector>((yysemantic_stack_[(3) - (2)].vfield),true);
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 12:

/* Line 690 of lalr1.cc  */
#line 437 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::tensor>((yysemantic_stack_[(3) - (2)].tfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 13:

/* Line 690 of lalr1.cc  */
#line 443 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::tensor>((yysemantic_stack_[(3) - (2)].tfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 14:

/* Line 690 of lalr1.cc  */
#line 449 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::tensor>((yysemantic_stack_[(3) - (2)].tfield),true);
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 15:

/* Line 690 of lalr1.cc  */
#line 455 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::tensor>((yysemantic_stack_[(3) - (2)].tfield),true);
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 16:

/* Line 690 of lalr1.cc  */
#line 461 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::symmTensor>((yysemantic_stack_[(3) - (2)].yfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 17:

/* Line 690 of lalr1.cc  */
#line 467 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::symmTensor>((yysemantic_stack_[(3) - (2)].yfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 18:

/* Line 690 of lalr1.cc  */
#line 473 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::symmTensor>((yysemantic_stack_[(3) - (2)].yfield),true);
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 19:

/* Line 690 of lalr1.cc  */
#line 479 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::symmTensor>((yysemantic_stack_[(3) - (2)].yfield),true);
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 20:

/* Line 690 of lalr1.cc  */
#line 485 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::sphericalTensor>((yysemantic_stack_[(3) - (2)].hfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 21:

/* Line 690 of lalr1.cc  */
#line 491 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::sphericalTensor>((yysemantic_stack_[(3) - (2)].hfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 22:

/* Line 690 of lalr1.cc  */
#line 497 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::sphericalTensor>((yysemantic_stack_[(3) - (2)].hfield),true);
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 23:

/* Line 690 of lalr1.cc  */
#line 503 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<CML::sphericalTensor>((yysemantic_stack_[(3) - (2)].hfield),true);
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 24:

/* Line 690 of lalr1.cc  */
#line 509 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<bool>((yysemantic_stack_[(3) - (2)].lfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 25:

/* Line 690 of lalr1.cc  */
#line 515 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<bool>((yysemantic_stack_[(3) - (2)].lfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 26:

/* Line 690 of lalr1.cc  */
#line 521 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<bool>((yysemantic_stack_[(3) - (2)].lfield),true);
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 27:

/* Line 690 of lalr1.cc  */
#line 527 "../PatchValueExpressionParser.yy"
    {
                      driver.setResult<bool>((yysemantic_stack_[(3) - (2)].lfield),true);
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 28:

/* Line 690 of lalr1.cc  */
#line 533 "../PatchValueExpressionParser.yy"
    {
                      driver.parserLastPos()=(yylocation_stack_[(2) - (2)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 29:

/* Line 690 of lalr1.cc  */
#line 538 "../PatchValueExpressionParser.yy"
    {
                      driver.parserLastPos()=(yylocation_stack_[(2) - (2)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 32:

/* Line 690 of lalr1.cc  */
#line 547 "../PatchValueExpressionParser.yy"
    { driver.setResult<CML::scalar>((yysemantic_stack_[(1) - (1)].sfield));  }
    break;

  case 33:

/* Line 690 of lalr1.cc  */
#line 548 "../PatchValueExpressionParser.yy"
    { driver.setResult<CML::vector>((yysemantic_stack_[(1) - (1)].vfield));  }
    break;

  case 34:

/* Line 690 of lalr1.cc  */
#line 549 "../PatchValueExpressionParser.yy"
    { driver.setResult<bool>((yysemantic_stack_[(1) - (1)].lfield)); }
    break;

  case 35:

/* Line 690 of lalr1.cc  */
#line 550 "../PatchValueExpressionParser.yy"
    { driver.setResult<CML::scalar>((yysemantic_stack_[(1) - (1)].sfield),true);  }
    break;

  case 36:

/* Line 690 of lalr1.cc  */
#line 551 "../PatchValueExpressionParser.yy"
    { driver.setResult<CML::vector>((yysemantic_stack_[(1) - (1)].vfield),true);  }
    break;

  case 37:

/* Line 690 of lalr1.cc  */
#line 552 "../PatchValueExpressionParser.yy"
    { driver.setResult<bool>((yysemantic_stack_[(1) - (1)].lfield),true); }
    break;

  case 38:

/* Line 690 of lalr1.cc  */
#line 553 "../PatchValueExpressionParser.yy"
    { driver.setResult<CML::tensor>((yysemantic_stack_[(1) - (1)].tfield));  }
    break;

  case 39:

/* Line 690 of lalr1.cc  */
#line 554 "../PatchValueExpressionParser.yy"
    { driver.setResult<CML::tensor>((yysemantic_stack_[(1) - (1)].tfield),true);  }
    break;

  case 40:

/* Line 690 of lalr1.cc  */
#line 555 "../PatchValueExpressionParser.yy"
    { driver.setResult<CML::symmTensor>((yysemantic_stack_[(1) - (1)].yfield));  }
    break;

  case 41:

/* Line 690 of lalr1.cc  */
#line 556 "../PatchValueExpressionParser.yy"
    {
            driver.setResult<CML::symmTensor>((yysemantic_stack_[(1) - (1)].yfield),true);
          }
    break;

  case 42:

/* Line 690 of lalr1.cc  */
#line 559 "../PatchValueExpressionParser.yy"
    {
            driver.setResult<CML::sphericalTensor>((yysemantic_stack_[(1) - (1)].hfield));
          }
    break;

  case 43:

/* Line 690 of lalr1.cc  */
#line 562 "../PatchValueExpressionParser.yy"
    {
            driver.setResult<CML::sphericalTensor>((yysemantic_stack_[(1) - (1)].hfield),true);
          }
    break;

  case 44:

/* Line 690 of lalr1.cc  */
#line 567 "../PatchValueExpressionParser.yy"
    { driver.startVectorComponent(); }
    break;

  case 45:

/* Line 690 of lalr1.cc  */
#line 570 "../PatchValueExpressionParser.yy"
    { driver.startTensorComponent(); }
    break;

  case 46:

/* Line 690 of lalr1.cc  */
#line 573 "../PatchValueExpressionParser.yy"
    { driver.startEatCharacters(); }
    break;

  case 47:

/* Line 690 of lalr1.cc  */
#line 576 "../PatchValueExpressionParser.yy"
    { (yyval.vfield) = (yysemantic_stack_[(1) - (1)].vfield); }
    break;

  case 48:

/* Line 690 of lalr1.cc  */
#line 577 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.makeField<CML::vector>(*(yysemantic_stack_[(1) - (1)].vec)).ptr();
            delete (yysemantic_stack_[(1) - (1)].vec);
          }
    break;

  case 49:

/* Line 690 of lalr1.cc  */
#line 581 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) + *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 50:

/* Line 690 of lalr1.cc  */
#line 586 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 51:

/* Line 690 of lalr1.cc  */
#line 591 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 52:

/* Line 690 of lalr1.cc  */
#line 596 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].tfield) & *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 53:

/* Line 690 of lalr1.cc  */
#line 601 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) & *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 54:

/* Line 690 of lalr1.cc  */
#line 606 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].yfield) & *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 55:

/* Line 690 of lalr1.cc  */
#line 611 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) & *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 56:

/* Line 690 of lalr1.cc  */
#line 616 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].hfield) & *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 57:

/* Line 690 of lalr1.cc  */
#line 621 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) & *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 58:

/* Line 690 of lalr1.cc  */
#line 626 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 59:

/* Line 690 of lalr1.cc  */
#line 631 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) ^ *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 60:

/* Line 690 of lalr1.cc  */
#line 636 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) - *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 61:

/* Line 690 of lalr1.cc  */
#line 641 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(-*(yysemantic_stack_[(2) - (2)].vfield));
            delete (yysemantic_stack_[(2) - (2)].vfield);
          }
    break;

  case 62:

/* Line 690 of lalr1.cc  */
#line 645 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(*(*(yysemantic_stack_[(2) - (2)].tfield)));
            delete (yysemantic_stack_[(2) - (2)].tfield);
          }
    break;

  case 63:

/* Line 690 of lalr1.cc  */
#line 649 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(*(*(yysemantic_stack_[(2) - (2)].yfield)));
            delete (yysemantic_stack_[(2) - (2)].yfield);
          }
    break;

  case 64:

/* Line 690 of lalr1.cc  */
#line 653 "../PatchValueExpressionParser.yy"
    { (yyval.vfield) = (yysemantic_stack_[(3) - (2)].vfield); }
    break;

  case 65:

/* Line 690 of lalr1.cc  */
#line 654 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(CML::eigenValues(*(yysemantic_stack_[(4) - (3)].tfield)));
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 66:

/* Line 690 of lalr1.cc  */
#line 658 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(CML::eigenValues(*(yysemantic_stack_[(4) - (3)].yfield)));
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 67:

/* Line 690 of lalr1.cc  */
#line 662 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.composeVectorField(
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::XX)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::XY)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::XZ)()
            ).ptr();
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 68:

/* Line 690 of lalr1.cc  */
#line 670 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.composeVectorField(
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::YX)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::YY)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::YZ)()
            ).ptr();
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 69:

/* Line 690 of lalr1.cc  */
#line 678 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.composeVectorField(
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::ZX)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::ZY)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::ZZ)()
            ).ptr();
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 70:

/* Line 690 of lalr1.cc  */
#line 686 "../PatchValueExpressionParser.yy"
    {
            //            $$ = new CML::vectorField( CML::diag(*$3) ); // not implemented?
            (yyval.vfield) = driver.composeVectorField(
                (yysemantic_stack_[(4) - (3)].tfield)->component(CML::tensor::XX)(),
                (yysemantic_stack_[(4) - (3)].tfield)->component(CML::tensor::YY)(),
                (yysemantic_stack_[(4) - (3)].tfield)->component(CML::tensor::ZZ)()
            ).ptr();
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 71:

/* Line 690 of lalr1.cc  */
#line 695 "../PatchValueExpressionParser.yy"
    {
            //            $$ = new CML::vectorField( CML::diag(*$3) ); // not implemented?
            (yyval.vfield) = driver.composeVectorField(
                (yysemantic_stack_[(4) - (3)].yfield)->component(CML::symmTensor::XX)(),
                (yysemantic_stack_[(4) - (3)].yfield)->component(CML::symmTensor::YY)(),
                (yysemantic_stack_[(4) - (3)].yfield)->component(CML::symmTensor::ZZ)()
            ).ptr();
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 72:

/* Line 690 of lalr1.cc  */
#line 704 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].vfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].vfield));
            (yyval.vfield) = driver.doConditional(
                *(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].vfield),*(yysemantic_stack_[(5) - (5)].vfield)
            ).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].vfield); delete (yysemantic_stack_[(5) - (5)].vfield);
          }
    break;

  case 73:

/* Line 690 of lalr1.cc  */
#line 711 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.makePositionField().ptr();
          }
    break;

  case 74:

/* Line 690 of lalr1.cc  */
#line 714 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.makeFaceNormalField().ptr();
          }
    break;

  case 75:

/* Line 690 of lalr1.cc  */
#line 717 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.makeFaceAreaField().ptr();
          }
    break;

  case 76:

/* Line 690 of lalr1.cc  */
#line 720 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.makeCellNeighbourField().ptr();
          }
    break;

  case 77:

/* Line 690 of lalr1.cc  */
#line 723 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.makeDeltaField().ptr();
          }
    break;

  case 78:

/* Line 690 of lalr1.cc  */
#line 726 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.toFace(*(yysemantic_stack_[(4) - (3)].vfield)).ptr();
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 80:

/* Line 690 of lalr1.cc  */
#line 731 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield)=driver.getField<CML::vector>(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
          }
    break;

  case 81:

/* Line 690 of lalr1.cc  */
#line 735 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield)=driver.getOldTimeField<CML::vector>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 82:

/* Line 690 of lalr1.cc  */
#line 739 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield)=driver.mapField<CML::vector>(*(yysemantic_stack_[(4) - (3)].name)).ptr(); delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 83:

/* Line 690 of lalr1.cc  */
#line 742 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield)=driver.mapField<CML::vector>(*(yysemantic_stack_[(4) - (3)].name),true).ptr(); delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 84:

/* Line 690 of lalr1.cc  */
#line 745 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield)=driver.getSurfaceNormalField<CML::vector>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 85:

/* Line 690 of lalr1.cc  */
#line 749 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield)=driver.getPatchInternalField<CML::vector>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 86:

/* Line 690 of lalr1.cc  */
#line 753 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield)=driver.getPatchNeighbourField<CML::vector>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 87:

/* Line 690 of lalr1.cc  */
#line 757 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = CML::min(*(yysemantic_stack_[(6) - (3)].vfield),*(yysemantic_stack_[(6) - (5)].vfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].vfield); delete (yysemantic_stack_[(6) - (5)].vfield);
          }
    break;

  case 88:

/* Line 690 of lalr1.cc  */
#line 761 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = CML::max(*(yysemantic_stack_[(6) - (3)].vfield),*(yysemantic_stack_[(6) - (5)].vfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].vfield); delete (yysemantic_stack_[(6) - (5)].vfield);
          }
    break;

  case 89:

/* Line 690 of lalr1.cc  */
#line 768 "../PatchValueExpressionParser.yy"
    {
      (yyval.vfield)=driver.evaluatePluginFunction<CML::vector>(
          *(yysemantic_stack_[(3) - (1)].name),
          (yylocation_stack_[(3) - (2)]),
          numberOfFunctionChars,
          false
      ).ptr();
  }
    break;

  case 90:

/* Line 690 of lalr1.cc  */
#line 778 "../PatchValueExpressionParser.yy"
    { (yyval.val) = (yysemantic_stack_[(1) - (1)].val); }
    break;

  case 91:

/* Line 690 of lalr1.cc  */
#line 779 "../PatchValueExpressionParser.yy"
    { (yyval.val) = -(yysemantic_stack_[(2) - (2)].val); }
    break;

  case 92:

/* Line 690 of lalr1.cc  */
#line 782 "../PatchValueExpressionParser.yy"
    { (yyval.val) = CML::gMin(*(yysemantic_stack_[(4) - (3)].sfield));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 93:

/* Line 690 of lalr1.cc  */
#line 785 "../PatchValueExpressionParser.yy"
    { (yyval.val) = CML::gMax(*(yysemantic_stack_[(4) - (3)].sfield));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 94:

/* Line 690 of lalr1.cc  */
#line 788 "../PatchValueExpressionParser.yy"
    { (yyval.val) = CML::gMin(*(yysemantic_stack_[(4) - (3)].sfield));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 95:

/* Line 690 of lalr1.cc  */
#line 791 "../PatchValueExpressionParser.yy"
    { (yyval.val) = CML::gMax(*(yysemantic_stack_[(4) - (3)].sfield));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 96:

/* Line 690 of lalr1.cc  */
#line 794 "../PatchValueExpressionParser.yy"
    { (yyval.val) = CML::gSum(*(yysemantic_stack_[(4) - (3)].sfield));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 97:

/* Line 690 of lalr1.cc  */
#line 797 "../PatchValueExpressionParser.yy"
    { (yyval.val) = CML::gSum(*(yysemantic_stack_[(4) - (3)].sfield));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 98:

/* Line 690 of lalr1.cc  */
#line 800 "../PatchValueExpressionParser.yy"
    { (yyval.val) = CML::gAverage(*(yysemantic_stack_[(4) - (3)].sfield));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 99:

/* Line 690 of lalr1.cc  */
#line 803 "../PatchValueExpressionParser.yy"
    { (yyval.val) = CML::gAverage(*(yysemantic_stack_[(4) - (3)].sfield));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 100:

/* Line 690 of lalr1.cc  */
#line 808 "../PatchValueExpressionParser.yy"
    {
            CML::vector tmp(HugeVal,HugeVal,HugeVal);
            if(((yysemantic_stack_[(4) - (3)].vfield)->size())>0) {
                tmp=CML::min(*(yysemantic_stack_[(4) - (3)].vfield));
            }
            CML::reduce(tmp,CML::minOp<CML::vector>());
            (yyval.vec) = new CML::vector(tmp);
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 101:

/* Line 690 of lalr1.cc  */
#line 817 "../PatchValueExpressionParser.yy"
    {
            CML::vector tmp(-HugeVal,-HugeVal,-HugeVal);
            if(((yysemantic_stack_[(4) - (3)].vfield)->size())>0) {
                tmp=CML::max(*(yysemantic_stack_[(4) - (3)].vfield));
            }
            CML::reduce(tmp,CML::maxOp<CML::vector>());
            (yyval.vec) = new CML::vector(tmp);
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 102:

/* Line 690 of lalr1.cc  */
#line 826 "../PatchValueExpressionParser.yy"
    {
          CML::vector tmp(HugeVal,HugeVal,HugeVal);
            if(((yysemantic_stack_[(4) - (3)].vfield)->size())>0) {
                tmp=CML::min(*(yysemantic_stack_[(4) - (3)].vfield));
            }
            CML::reduce(tmp,CML::minOp<CML::vector>());
            (yyval.vec) = new CML::vector(tmp);
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 103:

/* Line 690 of lalr1.cc  */
#line 835 "../PatchValueExpressionParser.yy"
    {
            CML::vector tmp(-HugeVal,-HugeVal,-HugeVal);
            if(((yysemantic_stack_[(4) - (3)].vfield)->size())>0) {
                tmp=CML::max(*(yysemantic_stack_[(4) - (3)].vfield));
            }
            CML::reduce(tmp,CML::maxOp<CML::vector>());
            (yyval.vec) = new CML::vector(tmp);
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 104:

/* Line 690 of lalr1.cc  */
#line 844 "../PatchValueExpressionParser.yy"
    {
            (yyval.vec) = new CML::vector(
                driver.getPositionOfMinimum(
                    *(yysemantic_stack_[(4) - (3)].sfield),
                    driver.patch().Cf()
                )
            );
            delete (yysemantic_stack_[(4) - (3)].sfield);
        }
    break;

  case 105:

/* Line 690 of lalr1.cc  */
#line 853 "../PatchValueExpressionParser.yy"
    {
            (yyval.vec) = new CML::vector(
                driver.getPositionOfMaximum(
                    *(yysemantic_stack_[(4) - (3)].sfield),
                    driver.patch().Cf()
                )
            );
            delete (yysemantic_stack_[(4) - (3)].sfield);
        }
    break;

  case 106:

/* Line 690 of lalr1.cc  */
#line 862 "../PatchValueExpressionParser.yy"
    {
            (yyval.vec) = new CML::vector(
                driver.getPositionOfMinimum(
                    *(yysemantic_stack_[(4) - (3)].sfield),
                    driver.patch().patch().localPoints()
                )
            );
            delete (yysemantic_stack_[(4) - (3)].sfield);
        }
    break;

  case 107:

/* Line 690 of lalr1.cc  */
#line 871 "../PatchValueExpressionParser.yy"
    {
            (yyval.vec) = new CML::vector(
                driver.getPositionOfMaximum(
                    *(yysemantic_stack_[(4) - (3)].sfield),
                    driver.patch().patch().localPoints()
                )
            );
            delete (yysemantic_stack_[(4) - (3)].sfield);
        }
    break;

  case 108:

/* Line 690 of lalr1.cc  */
#line 880 "../PatchValueExpressionParser.yy"
    {
            (yyval.vec) = new CML::vector(CML::gSum(*(yysemantic_stack_[(4) - (3)].vfield)));
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 109:

/* Line 690 of lalr1.cc  */
#line 884 "../PatchValueExpressionParser.yy"
    {
            (yyval.vec) = new CML::vector(CML::gSum(*(yysemantic_stack_[(4) - (3)].vfield)));
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 110:

/* Line 690 of lalr1.cc  */
#line 888 "../PatchValueExpressionParser.yy"
    {
            (yyval.vec) = new CML::vector(CML::gAverage(*(yysemantic_stack_[(4) - (3)].vfield)));
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 111:

/* Line 690 of lalr1.cc  */
#line 892 "../PatchValueExpressionParser.yy"
    {
            (yyval.vec) = new CML::vector(CML::gAverage(*(yysemantic_stack_[(4) - (3)].vfield)));
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 112:

/* Line 690 of lalr1.cc  */
#line 898 "../PatchValueExpressionParser.yy"
    { (yyval.sfield) = driver.makeField((yysemantic_stack_[(1) - (1)].val)).ptr(); }
    break;

  case 113:

/* Line 690 of lalr1.cc  */
#line 899 "../PatchValueExpressionParser.yy"
    { (yyval.sfield) = driver.makeField((yysemantic_stack_[(1) - (1)].val)).ptr(); }
    break;

  case 114:

/* Line 690 of lalr1.cc  */
#line 900 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].sfield) + *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 115:

/* Line 690 of lalr1.cc  */
#line 905 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].sfield) - *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 116:

/* Line 690 of lalr1.cc  */
#line 910 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 117:

/* Line 690 of lalr1.cc  */
#line 915 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = driver.makeModuloField(*(yysemantic_stack_[(3) - (1)].sfield),*(yysemantic_stack_[(3) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 118:

/* Line 690 of lalr1.cc  */
#line 920 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].sfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 119:

/* Line 690 of lalr1.cc  */
#line 930 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(6) - (3)].sfield),(yysemantic_stack_[(6) - (5)].sfield));
            (yyval.sfield) = new CML::scalarField(CML::pow(*(yysemantic_stack_[(6) - (3)].sfield), *(yysemantic_stack_[(6) - (5)].sfield)));
            delete (yysemantic_stack_[(6) - (3)].sfield); delete (yysemantic_stack_[(6) - (5)].sfield);
          }
    break;

  case 120:

/* Line 690 of lalr1.cc  */
#line 935 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::log(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 121:

/* Line 690 of lalr1.cc  */
#line 939 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::exp(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 122:

/* Line 690 of lalr1.cc  */
#line 943 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].vfield) & *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 123:

/* Line 690 of lalr1.cc  */
#line 947 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].tfield) && *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 124:

/* Line 690 of lalr1.cc  */
#line 952 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].tfield) && *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 125:

/* Line 690 of lalr1.cc  */
#line 957 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].tfield) && *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 126:

/* Line 690 of lalr1.cc  */
#line 962 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].yfield) && *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 127:

/* Line 690 of lalr1.cc  */
#line 967 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].yfield) && *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 128:

/* Line 690 of lalr1.cc  */
#line 972 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].yfield) && *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 129:

/* Line 690 of lalr1.cc  */
#line 977 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].hfield) && *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 130:

/* Line 690 of lalr1.cc  */
#line 982 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].hfield) && *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 131:

/* Line 690 of lalr1.cc  */
#line 987 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].hfield) && *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 132:

/* Line 690 of lalr1.cc  */
#line 992 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(-*(yysemantic_stack_[(2) - (2)].sfield));
            delete (yysemantic_stack_[(2) - (2)].sfield);
          }
    break;

  case 133:

/* Line 690 of lalr1.cc  */
#line 996 "../PatchValueExpressionParser.yy"
    { (yyval.sfield) = (yysemantic_stack_[(3) - (2)].sfield); }
    break;

  case 134:

/* Line 690 of lalr1.cc  */
#line 997 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sqr(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 135:

/* Line 690 of lalr1.cc  */
#line 1001 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sqrt(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 136:

/* Line 690 of lalr1.cc  */
#line 1005 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sin(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 137:

/* Line 690 of lalr1.cc  */
#line 1009 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::cos(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 138:

/* Line 690 of lalr1.cc  */
#line 1013 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::tan(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 139:

/* Line 690 of lalr1.cc  */
#line 1017 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::log10(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 140:

/* Line 690 of lalr1.cc  */
#line 1021 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::asin(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 141:

/* Line 690 of lalr1.cc  */
#line 1025 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::acos(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 142:

/* Line 690 of lalr1.cc  */
#line 1029 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::atan(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 143:

/* Line 690 of lalr1.cc  */
#line 1033 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sinh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 144:

/* Line 690 of lalr1.cc  */
#line 1037 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::cosh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 145:

/* Line 690 of lalr1.cc  */
#line 1041 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::tanh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 146:

/* Line 690 of lalr1.cc  */
#line 1045 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::asinh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 147:

/* Line 690 of lalr1.cc  */
#line 1049 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::acosh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 148:

/* Line 690 of lalr1.cc  */
#line 1053 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::atanh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 149:

/* Line 690 of lalr1.cc  */
#line 1057 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::erf(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 150:

/* Line 690 of lalr1.cc  */
#line 1061 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::erfc(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 151:

/* Line 690 of lalr1.cc  */
#line 1065 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::lgamma(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 152:

/* Line 690 of lalr1.cc  */
#line 1069 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::j0(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 153:

/* Line 690 of lalr1.cc  */
#line 1073 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::j1(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 154:

/* Line 690 of lalr1.cc  */
#line 1077 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::y0(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 155:

/* Line 690 of lalr1.cc  */
#line 1081 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::y1(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 156:

/* Line 690 of lalr1.cc  */
#line 1085 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sign(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 157:

/* Line 690 of lalr1.cc  */
#line 1089 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::pos(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 158:

/* Line 690 of lalr1.cc  */
#line 1093 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::neg(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 159:

/* Line 690 of lalr1.cc  */
#line 1097 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 160:

/* Line 690 of lalr1.cc  */
#line 1101 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].vfield)));
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 161:

/* Line 690 of lalr1.cc  */
#line 1105 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].tfield)));
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 162:

/* Line 690 of lalr1.cc  */
#line 1109 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].yfield)));
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 163:

/* Line 690 of lalr1.cc  */
#line 1113 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].hfield)));
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 164:

/* Line 690 of lalr1.cc  */
#line 1117 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 165:

/* Line 690 of lalr1.cc  */
#line 1121 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].vfield)));
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 166:

/* Line 690 of lalr1.cc  */
#line 1125 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].tfield)));
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 167:

/* Line 690 of lalr1.cc  */
#line 1129 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].yfield)));
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 168:

/* Line 690 of lalr1.cc  */
#line 1133 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].hfield)));
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 169:

/* Line 690 of lalr1.cc  */
#line 1137 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField( CML::tr(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 170:

/* Line 690 of lalr1.cc  */
#line 1141 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField( CML::tr(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 171:

/* Line 690 of lalr1.cc  */
#line 1145 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField( CML::tr(*(yysemantic_stack_[(4) - (3)].hfield)) );
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 172:

/* Line 690 of lalr1.cc  */
#line 1149 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField( CML::det(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 173:

/* Line 690 of lalr1.cc  */
#line 1153 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField( CML::det(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 174:

/* Line 690 of lalr1.cc  */
#line 1157 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField( CML::det(*(yysemantic_stack_[(4) - (3)].hfield)) );
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 175:

/* Line 690 of lalr1.cc  */
#line 1161 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].vfield)->component(0));
            delete (yysemantic_stack_[(4) - (1)].vfield);
          }
    break;

  case 176:

/* Line 690 of lalr1.cc  */
#line 1165 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].vfield)->component(1));
            delete (yysemantic_stack_[(4) - (1)].vfield);
          }
    break;

  case 177:

/* Line 690 of lalr1.cc  */
#line 1169 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].vfield)->component(2));
            delete (yysemantic_stack_[(4) - (1)].vfield);
          }
    break;

  case 178:

/* Line 690 of lalr1.cc  */
#line 1173 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(0));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 179:

/* Line 690 of lalr1.cc  */
#line 1177 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(1));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 180:

/* Line 690 of lalr1.cc  */
#line 1181 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(2));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 181:

/* Line 690 of lalr1.cc  */
#line 1185 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(3));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 182:

/* Line 690 of lalr1.cc  */
#line 1189 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(4));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 183:

/* Line 690 of lalr1.cc  */
#line 1193 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(5));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 184:

/* Line 690 of lalr1.cc  */
#line 1197 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(6));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 185:

/* Line 690 of lalr1.cc  */
#line 1201 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(7));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 186:

/* Line 690 of lalr1.cc  */
#line 1205 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(8));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 187:

/* Line 690 of lalr1.cc  */
#line 1209 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(0));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 188:

/* Line 690 of lalr1.cc  */
#line 1213 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(1));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 189:

/* Line 690 of lalr1.cc  */
#line 1217 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(2));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 190:

/* Line 690 of lalr1.cc  */
#line 1221 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(3));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 191:

/* Line 690 of lalr1.cc  */
#line 1225 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(4));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 192:

/* Line 690 of lalr1.cc  */
#line 1229 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(5));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 193:

/* Line 690 of lalr1.cc  */
#line 1233 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].hfield)->component(0));
            delete (yysemantic_stack_[(4) - (1)].hfield);
          }
    break;

  case 194:

/* Line 690 of lalr1.cc  */
#line 1237 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].sfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].sfield));
            (yyval.sfield) = driver.doConditional(
                *(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].sfield),*(yysemantic_stack_[(5) - (5)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].sfield); delete (yysemantic_stack_[(5) - (5)].sfield);
          }
    break;

  case 195:

/* Line 690 of lalr1.cc  */
#line 1244 "../PatchValueExpressionParser.yy"
    {
	  (yyval.sfield) = driver.makeField(
            CML::constant::mathematical::pi
          ).ptr();
          }
    break;

  case 196:

/* Line 690 of lalr1.cc  */
#line 1253 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeFaceIdField().ptr();
          }
    break;

  case 197:

/* Line 690 of lalr1.cc  */
#line 1256 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeNearDistField().ptr();
          }
    break;

  case 198:

/* Line 690 of lalr1.cc  */
#line 1259 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeField(
                CML::scalar(CML::Pstream::myProcNo())
            ).ptr();
          }
    break;

  case 199:

/* Line 690 of lalr1.cc  */
#line 1264 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.weights(driver.size()).ptr();
          }
    break;

  case 200:

/* Line 690 of lalr1.cc  */
#line 1267 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeRandomField().ptr();
          }
    break;

  case 201:

/* Line 690 of lalr1.cc  */
#line 1270 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeRandomField(-(yysemantic_stack_[(4) - (3)].integer)).ptr();
          }
    break;

  case 202:

/* Line 690 of lalr1.cc  */
#line 1273 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeRandomField(1).ptr();
          }
    break;

  case 203:

/* Line 690 of lalr1.cc  */
#line 1276 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeRandomField((yysemantic_stack_[(4) - (3)].integer)+1).ptr();
          }
    break;

  case 204:

/* Line 690 of lalr1.cc  */
#line 1279 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeWeightsField().ptr();
          }
    break;

  case 205:

/* Line 690 of lalr1.cc  */
#line 1282 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeGaussRandomField().ptr();
          }
    break;

  case 206:

/* Line 690 of lalr1.cc  */
#line 1285 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeGaussRandomField(-(yysemantic_stack_[(4) - (3)].integer)).ptr();
          }
    break;

  case 207:

/* Line 690 of lalr1.cc  */
#line 1288 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeGaussRandomField(1).ptr();
          }
    break;

  case 208:

/* Line 690 of lalr1.cc  */
#line 1291 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeGaussRandomField((yysemantic_stack_[(4) - (3)].integer)+1).ptr();
          }
    break;

  case 209:

/* Line 690 of lalr1.cc  */
#line 1294 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeField(driver.runTime().deltaT().value()).ptr();
          }
    break;

  case 210:

/* Line 690 of lalr1.cc  */
#line 1297 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeField(driver.runTime().time().value()).ptr();
          }
    break;

  case 211:

/* Line 690 of lalr1.cc  */
#line 1300 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.toFace(*(yysemantic_stack_[(4) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 212:

/* Line 690 of lalr1.cc  */
#line 1304 "../PatchValueExpressionParser.yy"
    {
            CML::vectorField *Sf=driver.makeFaceAreaField().ptr();
            (yyval.sfield) = new CML::scalarField(CML::mag(*Sf));
            delete Sf;
          }
    break;

  case 214:

/* Line 690 of lalr1.cc  */
#line 1310 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getField<CML::scalar>(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
          }
    break;

  case 215:

/* Line 690 of lalr1.cc  */
#line 1314 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getOldTimeField<CML::scalar>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 216:

/* Line 690 of lalr1.cc  */
#line 1318 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.mapField<CML::scalar>(*(yysemantic_stack_[(4) - (3)].name)).ptr(); delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 217:

/* Line 690 of lalr1.cc  */
#line 1321 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.mapField<CML::scalar>(*(yysemantic_stack_[(4) - (3)].name),true).ptr(); delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 218:

/* Line 690 of lalr1.cc  */
#line 1324 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getLine(*(yysemantic_stack_[(1) - (1)].name),driver.runTime().time().value()).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
          }
    break;

  case 219:

/* Line 690 of lalr1.cc  */
#line 1328 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getLookup(*(yysemantic_stack_[(4) - (1)].name),*(yysemantic_stack_[(4) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(4) - (1)].name); delete(yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 220:

/* Line 690 of lalr1.cc  */
#line 1332 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getSurfaceNormalField<CML::scalar>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 221:

/* Line 690 of lalr1.cc  */
#line 1336 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getPatchInternalField<CML::scalar>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 222:

/* Line 690 of lalr1.cc  */
#line 1340 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getPatchNeighbourField<CML::scalar>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 223:

/* Line 690 of lalr1.cc  */
#line 1344 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = CML::min(*(yysemantic_stack_[(6) - (3)].sfield),*(yysemantic_stack_[(6) - (5)].sfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].sfield); delete (yysemantic_stack_[(6) - (5)].sfield);
          }
    break;

  case 224:

/* Line 690 of lalr1.cc  */
#line 1348 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = CML::max(*(yysemantic_stack_[(6) - (3)].sfield),*(yysemantic_stack_[(6) - (5)].sfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].sfield); delete (yysemantic_stack_[(6) - (5)].sfield);
          }
    break;

  case 225:

/* Line 690 of lalr1.cc  */
#line 1355 "../PatchValueExpressionParser.yy"
    {
      (yyval.sfield)=driver.evaluatePluginFunction<CML::scalar>(
          *(yysemantic_stack_[(3) - (1)].name),
          (yylocation_stack_[(3) - (2)]),
          numberOfFunctionChars,
          false
      ).ptr();
      delete (yysemantic_stack_[(3) - (1)].name);
  }
    break;

  case 226:

/* Line 690 of lalr1.cc  */
#line 1367 "../PatchValueExpressionParser.yy"
    { (yyval.tfield) = (yysemantic_stack_[(1) - (1)].tfield); }
    break;

  case 227:

/* Line 690 of lalr1.cc  */
#line 1368 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) + *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 228:

/* Line 690 of lalr1.cc  */
#line 1373 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) + *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 229:

/* Line 690 of lalr1.cc  */
#line 1378 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) + *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 230:

/* Line 690 of lalr1.cc  */
#line 1383 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].yfield) + *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 231:

/* Line 690 of lalr1.cc  */
#line 1388 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].hfield) + *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 232:

/* Line 690 of lalr1.cc  */
#line 1393 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 233:

/* Line 690 of lalr1.cc  */
#line 1398 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 234:

/* Line 690 of lalr1.cc  */
#line 1403 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].vfield) * *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 235:

/* Line 690 of lalr1.cc  */
#line 1408 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) & *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 236:

/* Line 690 of lalr1.cc  */
#line 1413 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].yfield) & *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 237:

/* Line 690 of lalr1.cc  */
#line 1418 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) & *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 238:

/* Line 690 of lalr1.cc  */
#line 1423 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].hfield) & *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 239:

/* Line 690 of lalr1.cc  */
#line 1428 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) & *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 240:

/* Line 690 of lalr1.cc  */
#line 1433 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 241:

/* Line 690 of lalr1.cc  */
#line 1438 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) - *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 242:

/* Line 690 of lalr1.cc  */
#line 1443 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) - *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 243:

/* Line 690 of lalr1.cc  */
#line 1448 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) - *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 244:

/* Line 690 of lalr1.cc  */
#line 1453 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].yfield) - *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 245:

/* Line 690 of lalr1.cc  */
#line 1458 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].hfield) - *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 246:

/* Line 690 of lalr1.cc  */
#line 1463 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField(-*(yysemantic_stack_[(2) - (2)].tfield));
            delete (yysemantic_stack_[(2) - (2)].tfield);
          }
    break;

  case 247:

/* Line 690 of lalr1.cc  */
#line 1467 "../PatchValueExpressionParser.yy"
    { (yyval.tfield) = (yysemantic_stack_[(3) - (2)].tfield); }
    break;

  case 248:

/* Line 690 of lalr1.cc  */
#line 1468 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::skew(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 249:

/* Line 690 of lalr1.cc  */
#line 1472 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField(CML::eigenVectors(*(yysemantic_stack_[(4) - (3)].tfield)));
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 250:

/* Line 690 of lalr1.cc  */
#line 1476 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField(CML::eigenVectors(*(yysemantic_stack_[(4) - (3)].yfield)));
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 251:

/* Line 690 of lalr1.cc  */
#line 1480 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::inv(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 252:

/* Line 690 of lalr1.cc  */
#line 1484 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::cof(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 253:

/* Line 690 of lalr1.cc  */
#line 1488 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::dev(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 254:

/* Line 690 of lalr1.cc  */
#line 1492 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::dev2(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 255:

/* Line 690 of lalr1.cc  */
#line 1496 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( (yysemantic_stack_[(6) - (1)].tfield)->T() );
            delete (yysemantic_stack_[(6) - (1)].tfield);
          }
    break;

  case 256:

/* Line 690 of lalr1.cc  */
#line 1500 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].tfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].tfield));
            (yyval.tfield) = driver.doConditional(
                *(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].tfield),*(yysemantic_stack_[(5) - (5)].tfield)
            ).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].tfield); delete (yysemantic_stack_[(5) - (5)].tfield);
          }
    break;

  case 257:

/* Line 690 of lalr1.cc  */
#line 1507 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = driver.toFace(*(yysemantic_stack_[(4) - (3)].tfield)).ptr();
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 259:

/* Line 690 of lalr1.cc  */
#line 1512 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield)=driver.getField<CML::tensor>(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
          }
    break;

  case 260:

/* Line 690 of lalr1.cc  */
#line 1516 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield)=driver.getOldTimeField<CML::tensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 261:

/* Line 690 of lalr1.cc  */
#line 1520 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield)=driver.mapField<CML::tensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr(); delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 262:

/* Line 690 of lalr1.cc  */
#line 1523 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield)=driver.mapField<CML::tensor>(*(yysemantic_stack_[(4) - (3)].name),true).ptr(); delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 263:

/* Line 690 of lalr1.cc  */
#line 1526 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield)=driver.getSurfaceNormalField<CML::tensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 264:

/* Line 690 of lalr1.cc  */
#line 1530 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield)=driver.getPatchInternalField<CML::tensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 265:

/* Line 690 of lalr1.cc  */
#line 1534 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield)=driver.getPatchNeighbourField<CML::tensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 266:

/* Line 690 of lalr1.cc  */
#line 1538 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = CML::min(*(yysemantic_stack_[(6) - (3)].tfield),*(yysemantic_stack_[(6) - (5)].tfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].tfield); delete (yysemantic_stack_[(6) - (5)].tfield);
          }
    break;

  case 267:

/* Line 690 of lalr1.cc  */
#line 1542 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = CML::max(*(yysemantic_stack_[(6) - (3)].tfield),*(yysemantic_stack_[(6) - (5)].tfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].tfield); delete (yysemantic_stack_[(6) - (5)].tfield);
          }
    break;

  case 268:

/* Line 690 of lalr1.cc  */
#line 1549 "../PatchValueExpressionParser.yy"
    {
      (yyval.tfield)=driver.evaluatePluginFunction<CML::tensor>(
          *(yysemantic_stack_[(3) - (1)].name),
          (yylocation_stack_[(3) - (2)]),
          numberOfFunctionChars,
          false
      ).ptr();
      delete (yysemantic_stack_[(3) - (1)].name);
  }
    break;

  case 269:

/* Line 690 of lalr1.cc  */
#line 1560 "../PatchValueExpressionParser.yy"
    { (yyval.yfield) = (yysemantic_stack_[(1) - (1)].yfield); }
    break;

  case 270:

/* Line 690 of lalr1.cc  */
#line 1561 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) + *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 271:

/* Line 690 of lalr1.cc  */
#line 1566 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) + *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 272:

/* Line 690 of lalr1.cc  */
#line 1571 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].hfield) + *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 273:

/* Line 690 of lalr1.cc  */
#line 1576 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 274:

/* Line 690 of lalr1.cc  */
#line 1581 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 275:

/* Line 690 of lalr1.cc  */
#line 1586 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(
                symm(*(yysemantic_stack_[(3) - (1)].yfield) & *(yysemantic_stack_[(3) - (3)].yfield))
            );
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 276:

/* Line 690 of lalr1.cc  */
#line 1597 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].hfield) & *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 277:

/* Line 690 of lalr1.cc  */
#line 1602 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) & *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 278:

/* Line 690 of lalr1.cc  */
#line 1607 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 279:

/* Line 690 of lalr1.cc  */
#line 1612 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) - *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 280:

/* Line 690 of lalr1.cc  */
#line 1617 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) - *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 281:

/* Line 690 of lalr1.cc  */
#line 1622 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].hfield) - *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 282:

/* Line 690 of lalr1.cc  */
#line 1627 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField(-*(yysemantic_stack_[(2) - (2)].yfield));
            delete (yysemantic_stack_[(2) - (2)].yfield);
          }
    break;

  case 283:

/* Line 690 of lalr1.cc  */
#line 1631 "../PatchValueExpressionParser.yy"
    { (yyval.yfield) = (yysemantic_stack_[(3) - (2)].yfield); }
    break;

  case 284:

/* Line 690 of lalr1.cc  */
#line 1632 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::symm(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 285:

/* Line 690 of lalr1.cc  */
#line 1636 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::symm(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 286:

/* Line 690 of lalr1.cc  */
#line 1640 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::twoSymm(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 287:

/* Line 690 of lalr1.cc  */
#line 1644 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::twoSymm(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 288:

/* Line 690 of lalr1.cc  */
#line 1648 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::inv(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 289:

/* Line 690 of lalr1.cc  */
#line 1652 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::cof(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 290:

/* Line 690 of lalr1.cc  */
#line 1656 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::dev(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 291:

/* Line 690 of lalr1.cc  */
#line 1660 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::dev2(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 292:

/* Line 690 of lalr1.cc  */
#line 1664 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::sqr(*(yysemantic_stack_[(4) - (3)].vfield)) );
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 293:

/* Line 690 of lalr1.cc  */
#line 1668 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = (yysemantic_stack_[(6) - (1)].yfield);
          }
    break;

  case 294:

/* Line 690 of lalr1.cc  */
#line 1671 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].yfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].yfield));
            (yyval.yfield) = driver.doConditional(
                *(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].yfield),*(yysemantic_stack_[(5) - (5)].yfield)
            ).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].yfield); delete (yysemantic_stack_[(5) - (5)].yfield);
          }
    break;

  case 295:

/* Line 690 of lalr1.cc  */
#line 1678 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = driver.toFace(*(yysemantic_stack_[(4) - (3)].yfield)).ptr();
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 297:

/* Line 690 of lalr1.cc  */
#line 1683 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield)=driver.getField<CML::symmTensor>(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
          }
    break;

  case 298:

/* Line 690 of lalr1.cc  */
#line 1687 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield)=driver.getOldTimeField<CML::symmTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 299:

/* Line 690 of lalr1.cc  */
#line 1691 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield)=driver.mapField<CML::symmTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr(); delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 300:

/* Line 690 of lalr1.cc  */
#line 1694 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield)=driver.mapField<CML::symmTensor>(*(yysemantic_stack_[(4) - (3)].name),true).ptr(); delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 301:

/* Line 690 of lalr1.cc  */
#line 1697 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield)=driver.getSurfaceNormalField<CML::symmTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 302:

/* Line 690 of lalr1.cc  */
#line 1701 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield)=driver.getPatchInternalField<CML::symmTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 303:

/* Line 690 of lalr1.cc  */
#line 1705 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield)=driver.getPatchNeighbourField<CML::symmTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 304:

/* Line 690 of lalr1.cc  */
#line 1709 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = CML::min(*(yysemantic_stack_[(6) - (3)].yfield),*(yysemantic_stack_[(6) - (5)].yfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].yfield); delete (yysemantic_stack_[(6) - (5)].yfield);
          }
    break;

  case 305:

/* Line 690 of lalr1.cc  */
#line 1713 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = CML::max(*(yysemantic_stack_[(6) - (3)].yfield),*(yysemantic_stack_[(6) - (5)].yfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].yfield); delete (yysemantic_stack_[(6) - (5)].yfield);
          }
    break;

  case 306:

/* Line 690 of lalr1.cc  */
#line 1720 "../PatchValueExpressionParser.yy"
    {
      (yyval.yfield)=driver.evaluatePluginFunction<CML::symmTensor>(
          *(yysemantic_stack_[(3) - (1)].name),
          (yylocation_stack_[(3) - (2)]),
          numberOfFunctionChars,
          false
      ).ptr();
      delete (yysemantic_stack_[(3) - (1)].name);
  }
    break;

  case 307:

/* Line 690 of lalr1.cc  */
#line 1731 "../PatchValueExpressionParser.yy"
    { (yyval.hfield) = (yysemantic_stack_[(1) - (1)].hfield); }
    break;

  case 308:

/* Line 690 of lalr1.cc  */
#line 1732 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = driver.makeField(CML::sphericalTensor(1)).ptr();
          }
    break;

  case 309:

/* Line 690 of lalr1.cc  */
#line 1735 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) + *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 310:

/* Line 690 of lalr1.cc  */
#line 1740 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 311:

/* Line 690 of lalr1.cc  */
#line 1745 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 312:

/* Line 690 of lalr1.cc  */
#line 1750 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) & *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 313:

/* Line 690 of lalr1.cc  */
#line 1755 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 314:

/* Line 690 of lalr1.cc  */
#line 1760 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) - *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 315:

/* Line 690 of lalr1.cc  */
#line 1765 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField(-*(yysemantic_stack_[(2) - (2)].hfield));
            delete (yysemantic_stack_[(2) - (2)].hfield);
          }
    break;

  case 316:

/* Line 690 of lalr1.cc  */
#line 1769 "../PatchValueExpressionParser.yy"
    { (yyval.hfield) = (yysemantic_stack_[(3) - (2)].hfield); }
    break;

  case 317:

/* Line 690 of lalr1.cc  */
#line 1770 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField( CML::sph(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 318:

/* Line 690 of lalr1.cc  */
#line 1774 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField( CML::sph(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 319:

/* Line 690 of lalr1.cc  */
#line 1778 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField( CML::sph(*(yysemantic_stack_[(4) - (3)].hfield)) );
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 320:

/* Line 690 of lalr1.cc  */
#line 1782 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField( CML::inv(*(yysemantic_stack_[(4) - (3)].hfield)) );
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 321:

/* Line 690 of lalr1.cc  */
#line 1786 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = (yysemantic_stack_[(6) - (1)].hfield);
          }
    break;

  case 322:

/* Line 690 of lalr1.cc  */
#line 1789 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].hfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].hfield));
            (yyval.hfield) = driver.doConditional(
                *(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].hfield),*(yysemantic_stack_[(5) - (5)].hfield)
            ).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].hfield); delete (yysemantic_stack_[(5) - (5)].hfield);
          }
    break;

  case 323:

/* Line 690 of lalr1.cc  */
#line 1796 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = driver.toFace(*(yysemantic_stack_[(4) - (3)].hfield)).ptr();
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 325:

/* Line 690 of lalr1.cc  */
#line 1801 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield)=driver.getField<CML::sphericalTensor>(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
          }
    break;

  case 326:

/* Line 690 of lalr1.cc  */
#line 1805 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield)=driver.getOldTimeField<CML::sphericalTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 327:

/* Line 690 of lalr1.cc  */
#line 1809 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield)=driver.mapField<CML::sphericalTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr(); delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 328:

/* Line 690 of lalr1.cc  */
#line 1812 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield)=driver.mapField<CML::sphericalTensor>(*(yysemantic_stack_[(4) - (3)].name),true).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 329:

/* Line 690 of lalr1.cc  */
#line 1816 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield)=driver.getSurfaceNormalField<CML::sphericalTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 330:

/* Line 690 of lalr1.cc  */
#line 1820 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield)=driver.getPatchInternalField<CML::sphericalTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 331:

/* Line 690 of lalr1.cc  */
#line 1824 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield)=driver.getPatchNeighbourField<CML::sphericalTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 332:

/* Line 690 of lalr1.cc  */
#line 1828 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = CML::min(*(yysemantic_stack_[(6) - (3)].hfield),*(yysemantic_stack_[(6) - (5)].hfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].hfield); delete (yysemantic_stack_[(6) - (5)].hfield);
          }
    break;

  case 333:

/* Line 690 of lalr1.cc  */
#line 1832 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = CML::max(*(yysemantic_stack_[(6) - (3)].hfield),*(yysemantic_stack_[(6) - (5)].hfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].hfield); delete (yysemantic_stack_[(6) - (5)].hfield);
          }
    break;

  case 334:

/* Line 690 of lalr1.cc  */
#line 1839 "../PatchValueExpressionParser.yy"
    {
      (yyval.hfield)=driver.evaluatePluginFunction<CML::sphericalTensor>(
          *(yysemantic_stack_[(3) - (1)].name),
          (yylocation_stack_[(3) - (2)]),
          numberOfFunctionChars,
          false
      ).ptr();
      delete (yysemantic_stack_[(3) - (1)].name);
  }
    break;

  case 335:

/* Line 690 of lalr1.cc  */
#line 1850 "../PatchValueExpressionParser.yy"
    { (yyval.lfield) = driver.makeField(true).ptr(); }
    break;

  case 336:

/* Line 690 of lalr1.cc  */
#line 1851 "../PatchValueExpressionParser.yy"
    { (yyval.lfield) = driver.makeField(false).ptr(); }
    break;

  case 337:

/* Line 690 of lalr1.cc  */
#line 1852 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(
                *(yysemantic_stack_[(3) - (1)].sfield),
                std::less<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 338:

/* Line 690 of lalr1.cc  */
#line 1861 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(
                *(yysemantic_stack_[(3) - (1)].sfield),
                std::greater<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 339:

/* Line 690 of lalr1.cc  */
#line 1870 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(
                *(yysemantic_stack_[(3) - (1)].sfield),
                std::less_equal<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 340:

/* Line 690 of lalr1.cc  */
#line 1879 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(
                *(yysemantic_stack_[(3) - (1)].sfield),
                std::greater_equal<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 341:

/* Line 690 of lalr1.cc  */
#line 1888 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(
                *(yysemantic_stack_[(3) - (1)].sfield),
                std::equal_to<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 342:

/* Line 690 of lalr1.cc  */
#line 1897 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(
                *(yysemantic_stack_[(3) - (1)].sfield),
                std::not_equal_to<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 343:

/* Line 690 of lalr1.cc  */
#line 1906 "../PatchValueExpressionParser.yy"
    { (yyval.lfield) = (yysemantic_stack_[(3) - (2)].lfield); }
    break;

  case 344:

/* Line 690 of lalr1.cc  */
#line 1907 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].lfield),(yysemantic_stack_[(3) - (3)].lfield));
            (yyval.lfield) = driver.doLogicalOp(
                *(yysemantic_stack_[(3) - (1)].lfield),
                std::logical_and<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].lfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].lfield); delete (yysemantic_stack_[(3) - (3)].lfield);
          }
    break;

  case 345:

/* Line 690 of lalr1.cc  */
#line 1916 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].lfield),(yysemantic_stack_[(3) - (3)].lfield));
            (yyval.lfield) = driver.doLogicalOp(
                *(yysemantic_stack_[(3) - (1)].lfield),
                std::logical_or<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].lfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].lfield); delete (yysemantic_stack_[(3) - (3)].lfield);
          }
    break;

  case 346:

/* Line 690 of lalr1.cc  */
#line 1925 "../PatchValueExpressionParser.yy"
    {
            (yyval.lfield) = driver.doLogicalNot(*(yysemantic_stack_[(2) - (2)].lfield)).ptr();
            delete (yysemantic_stack_[(2) - (2)].lfield);
          }
    break;

  case 348:

/* Line 690 of lalr1.cc  */
#line 1930 "../PatchValueExpressionParser.yy"
    {
        (yyval.lfield)=driver.getVariable<bool>(*(yysemantic_stack_[(1) - (1)].name),driver.size()).ptr();
        delete (yysemantic_stack_[(1) - (1)].name);
      }
    break;

  case 349:

/* Line 690 of lalr1.cc  */
#line 1937 "../PatchValueExpressionParser.yy"
    {
      (yyval.lfield)=driver.evaluatePluginFunction<bool>(
          *(yysemantic_stack_[(3) - (1)].name),
          (yylocation_stack_[(3) - (2)]),
          numberOfFunctionChars,
          false
      ).ptr();
      delete (yysemantic_stack_[(3) - (1)].name);
  }
    break;

  case 350:

/* Line 690 of lalr1.cc  */
#line 1948 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.composeVectorField(*(yysemantic_stack_[(8) - (3)].sfield),*(yysemantic_stack_[(8) - (5)].sfield),*(yysemantic_stack_[(8) - (7)].sfield)).ptr();
            delete (yysemantic_stack_[(8) - (3)].sfield); delete (yysemantic_stack_[(8) - (5)].sfield); delete (yysemantic_stack_[(8) - (7)].sfield);
          }
    break;

  case 351:

/* Line 690 of lalr1.cc  */
#line 1954 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = driver.composeTensorField(
                *(yysemantic_stack_[(20) - (3)].sfield),*(yysemantic_stack_[(20) - (5)].sfield),*(yysemantic_stack_[(20) - (7)].sfield),
                *(yysemantic_stack_[(20) - (9)].sfield),*(yysemantic_stack_[(20) - (11)].sfield),*(yysemantic_stack_[(20) - (13)].sfield),
                *(yysemantic_stack_[(20) - (15)].sfield),*(yysemantic_stack_[(20) - (17)].sfield),*(yysemantic_stack_[(20) - (19)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(20) - (3)].sfield); delete (yysemantic_stack_[(20) - (5)].sfield); delete (yysemantic_stack_[(20) - (7)].sfield); delete (yysemantic_stack_[(20) - (9)].sfield); delete (yysemantic_stack_[(20) - (11)].sfield);
            delete (yysemantic_stack_[(20) - (13)].sfield); delete (yysemantic_stack_[(20) - (15)].sfield); delete (yysemantic_stack_[(20) - (17)].sfield); delete (yysemantic_stack_[(20) - (19)].sfield);
          }
    break;

  case 352:

/* Line 690 of lalr1.cc  */
#line 1965 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = driver.composeSymmTensorField(
                *(yysemantic_stack_[(14) - (3)].sfield),*(yysemantic_stack_[(14) - (5)].sfield),*(yysemantic_stack_[(14) - (7)].sfield),
                *(yysemantic_stack_[(14) - (9)].sfield),*(yysemantic_stack_[(14) - (11)].sfield),
                *(yysemantic_stack_[(14) - (13)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(14) - (3)].sfield); delete (yysemantic_stack_[(14) - (5)].sfield); delete (yysemantic_stack_[(14) - (7)].sfield); delete (yysemantic_stack_[(14) - (9)].sfield); delete (yysemantic_stack_[(14) - (11)].sfield); delete (yysemantic_stack_[(14) - (13)].sfield);
          }
    break;

  case 353:

/* Line 690 of lalr1.cc  */
#line 1975 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = driver.composeSphericalTensorField(*(yysemantic_stack_[(4) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 354:

/* Line 690 of lalr1.cc  */
#line 1981 "../PatchValueExpressionParser.yy"
    { (yyval.vfield) = (yysemantic_stack_[(1) - (1)].vfield); }
    break;

  case 355:

/* Line 690 of lalr1.cc  */
#line 1982 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) + *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 356:

/* Line 690 of lalr1.cc  */
#line 1987 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 357:

/* Line 690 of lalr1.cc  */
#line 1992 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 358:

/* Line 690 of lalr1.cc  */
#line 1997 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].tfield) & *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 359:

/* Line 690 of lalr1.cc  */
#line 2002 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) & *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 360:

/* Line 690 of lalr1.cc  */
#line 2007 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].yfield) & *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 361:

/* Line 690 of lalr1.cc  */
#line 2012 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) & *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 362:

/* Line 690 of lalr1.cc  */
#line 2017 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].hfield) & *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 363:

/* Line 690 of lalr1.cc  */
#line 2022 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) & *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 364:

/* Line 690 of lalr1.cc  */
#line 2027 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 365:

/* Line 690 of lalr1.cc  */
#line 2032 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) ^ *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 366:

/* Line 690 of lalr1.cc  */
#line 2037 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) - *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 367:

/* Line 690 of lalr1.cc  */
#line 2042 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(-*(yysemantic_stack_[(2) - (2)].vfield));
            delete (yysemantic_stack_[(2) - (2)].vfield);
          }
    break;

  case 368:

/* Line 690 of lalr1.cc  */
#line 2046 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(*(*(yysemantic_stack_[(2) - (2)].tfield)));
            delete (yysemantic_stack_[(2) - (2)].tfield);
          }
    break;

  case 369:

/* Line 690 of lalr1.cc  */
#line 2050 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(*(*(yysemantic_stack_[(2) - (2)].yfield)));
            delete (yysemantic_stack_[(2) - (2)].yfield);
          }
    break;

  case 370:

/* Line 690 of lalr1.cc  */
#line 2054 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(CML::eigenValues(*(yysemantic_stack_[(4) - (3)].tfield)));
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 371:

/* Line 690 of lalr1.cc  */
#line 2058 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(CML::eigenValues(*(yysemantic_stack_[(4) - (3)].yfield)));
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 372:

/* Line 690 of lalr1.cc  */
#line 2062 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.composeVectorField(
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::XX)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::XY)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::XZ)()
            ).ptr();
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 373:

/* Line 690 of lalr1.cc  */
#line 2070 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.composeVectorField(
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::YX)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::YY)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::YZ)()
            ).ptr();
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 374:

/* Line 690 of lalr1.cc  */
#line 2078 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.composeVectorField(
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::ZX)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::ZY)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::ZZ)()
            ).ptr();
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 375:

/* Line 690 of lalr1.cc  */
#line 2086 "../PatchValueExpressionParser.yy"
    {
            //            $$ = new CML::vectorField( CML::diag(*$3) ); // not implemented?
            (yyval.vfield) = driver.composeVectorField(
                (yysemantic_stack_[(4) - (3)].tfield)->component(CML::tensor::XX)(),
                (yysemantic_stack_[(4) - (3)].tfield)->component(CML::tensor::YY)(),
                (yysemantic_stack_[(4) - (3)].tfield)->component(CML::tensor::ZZ)()
            ).ptr();
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 376:

/* Line 690 of lalr1.cc  */
#line 2095 "../PatchValueExpressionParser.yy"
    {
            //            $$ = new CML::vectorField( CML::diag(*$3) ); // not implemented?
            (yyval.vfield) = driver.composeVectorField(
                (yysemantic_stack_[(4) - (3)].yfield)->component(CML::symmTensor::XX)(),
                (yysemantic_stack_[(4) - (3)].yfield)->component(CML::symmTensor::YY)(),
                (yysemantic_stack_[(4) - (3)].yfield)->component(CML::symmTensor::ZZ)()
            ).ptr();
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 377:

/* Line 690 of lalr1.cc  */
#line 2104 "../PatchValueExpressionParser.yy"
    { (yyval.vfield) = (yysemantic_stack_[(3) - (2)].vfield); }
    break;

  case 378:

/* Line 690 of lalr1.cc  */
#line 2105 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].vfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].vfield));
            (yyval.vfield) = driver.doConditional(
                *(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].vfield),*(yysemantic_stack_[(5) - (5)].vfield)
            ).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].vfield); delete (yysemantic_stack_[(5) - (5)].vfield);
          }
    break;

  case 379:

/* Line 690 of lalr1.cc  */
#line 2112 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.makePointField().ptr();
          }
    break;

  case 380:

/* Line 690 of lalr1.cc  */
#line 2115 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.toPoint(*(yysemantic_stack_[(4) - (3)].vfield)).ptr();
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 382:

/* Line 690 of lalr1.cc  */
#line 2120 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield)=driver.getField<CML::vector>(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
                    }
    break;

  case 383:

/* Line 690 of lalr1.cc  */
#line 2124 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield)=driver.getOldTimeField<CML::vector>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
				}
    break;

  case 384:

/* Line 690 of lalr1.cc  */
#line 2131 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = CML::min(*(yysemantic_stack_[(6) - (3)].vfield),*(yysemantic_stack_[(6) - (5)].vfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].vfield); delete (yysemantic_stack_[(6) - (5)].vfield);
          }
    break;

  case 385:

/* Line 690 of lalr1.cc  */
#line 2135 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = CML::max(*(yysemantic_stack_[(6) - (3)].vfield),*(yysemantic_stack_[(6) - (5)].vfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].vfield); delete (yysemantic_stack_[(6) - (5)].vfield);
          }
    break;

  case 386:

/* Line 690 of lalr1.cc  */
#line 2142 "../PatchValueExpressionParser.yy"
    {
      (yyval.vfield)=driver.evaluatePluginFunction<CML::vector>(
          *(yysemantic_stack_[(3) - (1)].name),
          (yylocation_stack_[(3) - (2)]),
          numberOfFunctionChars,
          true
      ).ptr();
      delete (yysemantic_stack_[(3) - (1)].name);
  }
    break;

  case 387:

/* Line 690 of lalr1.cc  */
#line 2153 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].sfield) + *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 388:

/* Line 690 of lalr1.cc  */
#line 2158 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].sfield) - *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 389:

/* Line 690 of lalr1.cc  */
#line 2163 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 390:

/* Line 690 of lalr1.cc  */
#line 2168 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = driver.makeModuloField(*(yysemantic_stack_[(3) - (1)].sfield),*(yysemantic_stack_[(3) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 391:

/* Line 690 of lalr1.cc  */
#line 2173 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].sfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 392:

/* Line 690 of lalr1.cc  */
#line 2178 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::pow(*(yysemantic_stack_[(6) - (3)].sfield), (yysemantic_stack_[(6) - (5)].val)));
            delete (yysemantic_stack_[(6) - (3)].sfield);
          }
    break;

  case 393:

/* Line 690 of lalr1.cc  */
#line 2182 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(6) - (3)].sfield),(yysemantic_stack_[(6) - (5)].sfield));
            (yyval.sfield) = new CML::scalarField(CML::pow(*(yysemantic_stack_[(6) - (3)].sfield), *(yysemantic_stack_[(6) - (5)].sfield)));
            delete (yysemantic_stack_[(6) - (3)].sfield); delete(yysemantic_stack_[(6) - (5)].sfield);
          }
    break;

  case 394:

/* Line 690 of lalr1.cc  */
#line 2187 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::log(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 395:

/* Line 690 of lalr1.cc  */
#line 2191 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::exp(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 396:

/* Line 690 of lalr1.cc  */
#line 2195 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].vfield) & *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 397:

/* Line 690 of lalr1.cc  */
#line 2199 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].tfield) && *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 398:

/* Line 690 of lalr1.cc  */
#line 2203 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].tfield) && *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 399:

/* Line 690 of lalr1.cc  */
#line 2207 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].tfield) && *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 400:

/* Line 690 of lalr1.cc  */
#line 2211 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].yfield) && *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 401:

/* Line 690 of lalr1.cc  */
#line 2215 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].yfield) && *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 402:

/* Line 690 of lalr1.cc  */
#line 2219 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].yfield) && *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 403:

/* Line 690 of lalr1.cc  */
#line 2223 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].hfield) && *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 404:

/* Line 690 of lalr1.cc  */
#line 2227 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].hfield) && *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 405:

/* Line 690 of lalr1.cc  */
#line 2231 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].hfield) && *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 406:

/* Line 690 of lalr1.cc  */
#line 2235 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(-*(yysemantic_stack_[(2) - (2)].sfield));
            delete (yysemantic_stack_[(2) - (2)].sfield);
          }
    break;

  case 407:

/* Line 690 of lalr1.cc  */
#line 2239 "../PatchValueExpressionParser.yy"
    { (yyval.sfield) = (yysemantic_stack_[(3) - (2)].sfield); }
    break;

  case 408:

/* Line 690 of lalr1.cc  */
#line 2240 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sqr(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 409:

/* Line 690 of lalr1.cc  */
#line 2244 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sqrt(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 410:

/* Line 690 of lalr1.cc  */
#line 2248 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sin(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 411:

/* Line 690 of lalr1.cc  */
#line 2252 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::cos(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 412:

/* Line 690 of lalr1.cc  */
#line 2256 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::tan(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 413:

/* Line 690 of lalr1.cc  */
#line 2260 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::log10(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 414:

/* Line 690 of lalr1.cc  */
#line 2264 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::asin(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 415:

/* Line 690 of lalr1.cc  */
#line 2268 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::acos(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 416:

/* Line 690 of lalr1.cc  */
#line 2272 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::atan(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 417:

/* Line 690 of lalr1.cc  */
#line 2276 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sinh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 418:

/* Line 690 of lalr1.cc  */
#line 2280 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::cosh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 419:

/* Line 690 of lalr1.cc  */
#line 2284 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::tanh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 420:

/* Line 690 of lalr1.cc  */
#line 2288 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::asinh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 421:

/* Line 690 of lalr1.cc  */
#line 2292 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::acosh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 422:

/* Line 690 of lalr1.cc  */
#line 2296 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::atanh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 423:

/* Line 690 of lalr1.cc  */
#line 2300 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::erf(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 424:

/* Line 690 of lalr1.cc  */
#line 2304 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::erfc(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 425:

/* Line 690 of lalr1.cc  */
#line 2308 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::lgamma(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 426:

/* Line 690 of lalr1.cc  */
#line 2312 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::j0(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 427:

/* Line 690 of lalr1.cc  */
#line 2316 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::j1(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 428:

/* Line 690 of lalr1.cc  */
#line 2320 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::y0(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 429:

/* Line 690 of lalr1.cc  */
#line 2324 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::y1(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 430:

/* Line 690 of lalr1.cc  */
#line 2328 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sign(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 431:

/* Line 690 of lalr1.cc  */
#line 2332 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::pos(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 432:

/* Line 690 of lalr1.cc  */
#line 2336 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::neg(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 433:

/* Line 690 of lalr1.cc  */
#line 2340 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 434:

/* Line 690 of lalr1.cc  */
#line 2344 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].vfield)));
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 435:

/* Line 690 of lalr1.cc  */
#line 2348 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].tfield)));
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 436:

/* Line 690 of lalr1.cc  */
#line 2352 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].yfield)));
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 437:

/* Line 690 of lalr1.cc  */
#line 2356 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].hfield)));
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 438:

/* Line 690 of lalr1.cc  */
#line 2360 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 439:

/* Line 690 of lalr1.cc  */
#line 2364 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].vfield)));
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 440:

/* Line 690 of lalr1.cc  */
#line 2368 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].tfield)));
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 441:

/* Line 690 of lalr1.cc  */
#line 2372 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].yfield)));
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 442:

/* Line 690 of lalr1.cc  */
#line 2376 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].hfield)));
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 443:

/* Line 690 of lalr1.cc  */
#line 2380 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].vfield)->component(0));
            delete (yysemantic_stack_[(4) - (1)].vfield);
          }
    break;

  case 444:

/* Line 690 of lalr1.cc  */
#line 2384 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].vfield)->component(1));
            delete (yysemantic_stack_[(4) - (1)].vfield);
          }
    break;

  case 445:

/* Line 690 of lalr1.cc  */
#line 2388 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].vfield)->component(2));
            delete (yysemantic_stack_[(4) - (1)].vfield);
          }
    break;

  case 446:

/* Line 690 of lalr1.cc  */
#line 2392 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(0));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 447:

/* Line 690 of lalr1.cc  */
#line 2396 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(1));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 448:

/* Line 690 of lalr1.cc  */
#line 2400 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(2));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 449:

/* Line 690 of lalr1.cc  */
#line 2404 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(3));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 450:

/* Line 690 of lalr1.cc  */
#line 2408 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(4));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 451:

/* Line 690 of lalr1.cc  */
#line 2412 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(5));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 452:

/* Line 690 of lalr1.cc  */
#line 2416 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(6));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 453:

/* Line 690 of lalr1.cc  */
#line 2420 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(7));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 454:

/* Line 690 of lalr1.cc  */
#line 2424 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(8));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 455:

/* Line 690 of lalr1.cc  */
#line 2428 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(0));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 456:

/* Line 690 of lalr1.cc  */
#line 2432 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(1));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 457:

/* Line 690 of lalr1.cc  */
#line 2436 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(2));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 458:

/* Line 690 of lalr1.cc  */
#line 2440 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(3));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 459:

/* Line 690 of lalr1.cc  */
#line 2444 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(4));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 460:

/* Line 690 of lalr1.cc  */
#line 2448 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(5));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 461:

/* Line 690 of lalr1.cc  */
#line 2452 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].hfield)->component(0));
            delete (yysemantic_stack_[(4) - (1)].hfield);
          }
    break;

  case 462:

/* Line 690 of lalr1.cc  */
#line 2456 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].sfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].sfield));
            (yyval.sfield) = driver.doConditional(
                *(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].sfield),*(yysemantic_stack_[(5) - (5)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].sfield); delete (yysemantic_stack_[(5) - (5)].sfield);
          }
    break;

  case 463:

/* Line 690 of lalr1.cc  */
#line 2463 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.toPoint(*(yysemantic_stack_[(4) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 465:

/* Line 690 of lalr1.cc  */
#line 2468 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getField<CML::scalar>(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
          }
    break;

  case 466:

/* Line 690 of lalr1.cc  */
#line 2472 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getOldTimeField<CML::scalar>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 467:

/* Line 690 of lalr1.cc  */
#line 2479 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getLookup(*(yysemantic_stack_[(4) - (1)].name),*(yysemantic_stack_[(4) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(4) - (1)].name); delete(yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 468:

/* Line 690 of lalr1.cc  */
#line 2483 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = CML::min(*(yysemantic_stack_[(6) - (3)].sfield),*(yysemantic_stack_[(6) - (5)].sfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].sfield); delete (yysemantic_stack_[(6) - (5)].sfield);
          }
    break;

  case 469:

/* Line 690 of lalr1.cc  */
#line 2487 "../PatchValueExpressionParser.yy"
    {
            (yyval.sfield) = CML::max(*(yysemantic_stack_[(6) - (3)].sfield),*(yysemantic_stack_[(6) - (5)].sfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].sfield); delete (yysemantic_stack_[(6) - (5)].sfield);
          }
    break;

  case 470:

/* Line 690 of lalr1.cc  */
#line 2494 "../PatchValueExpressionParser.yy"
    {
      (yyval.sfield)=driver.evaluatePluginFunction<CML::scalar>(
          *(yysemantic_stack_[(3) - (1)].name),
          (yylocation_stack_[(3) - (2)]),
          numberOfFunctionChars,
          true
      ).ptr();
      delete (yysemantic_stack_[(3) - (1)].name);
  }
    break;

  case 471:

/* Line 690 of lalr1.cc  */
#line 2505 "../PatchValueExpressionParser.yy"
    { (yyval.tfield) = (yysemantic_stack_[(1) - (1)].tfield); }
    break;

  case 472:

/* Line 690 of lalr1.cc  */
#line 2506 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) + *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 473:

/* Line 690 of lalr1.cc  */
#line 2511 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 474:

/* Line 690 of lalr1.cc  */
#line 2516 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 475:

/* Line 690 of lalr1.cc  */
#line 2521 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].vfield) * *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 476:

/* Line 690 of lalr1.cc  */
#line 2526 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) & *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 477:

/* Line 690 of lalr1.cc  */
#line 2531 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].yfield) & *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 478:

/* Line 690 of lalr1.cc  */
#line 2536 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) & *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 479:

/* Line 690 of lalr1.cc  */
#line 2541 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].hfield) & *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 480:

/* Line 690 of lalr1.cc  */
#line 2546 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) & *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 481:

/* Line 690 of lalr1.cc  */
#line 2551 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 482:

/* Line 690 of lalr1.cc  */
#line 2556 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) - *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 483:

/* Line 690 of lalr1.cc  */
#line 2561 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField(-*(yysemantic_stack_[(2) - (2)].tfield));
            delete (yysemantic_stack_[(2) - (2)].tfield);
          }
    break;

  case 484:

/* Line 690 of lalr1.cc  */
#line 2565 "../PatchValueExpressionParser.yy"
    { (yyval.tfield) = (yysemantic_stack_[(3) - (2)].tfield); }
    break;

  case 485:

/* Line 690 of lalr1.cc  */
#line 2566 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::skew(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 486:

/* Line 690 of lalr1.cc  */
#line 2570 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField(CML::eigenVectors(*(yysemantic_stack_[(4) - (3)].tfield)));
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 487:

/* Line 690 of lalr1.cc  */
#line 2574 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField(CML::eigenVectors(*(yysemantic_stack_[(4) - (3)].yfield)));
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 488:

/* Line 690 of lalr1.cc  */
#line 2578 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::inv(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 489:

/* Line 690 of lalr1.cc  */
#line 2582 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::cof(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 490:

/* Line 690 of lalr1.cc  */
#line 2586 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::dev(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 491:

/* Line 690 of lalr1.cc  */
#line 2590 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::dev2(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 492:

/* Line 690 of lalr1.cc  */
#line 2594 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( (yysemantic_stack_[(6) - (1)].tfield)->T() );
            delete (yysemantic_stack_[(6) - (1)].tfield);
          }
    break;

  case 493:

/* Line 690 of lalr1.cc  */
#line 2598 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].tfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].tfield));
            (yyval.tfield) = driver.doConditional(
                *(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].tfield),*(yysemantic_stack_[(5) - (5)].tfield)
            ).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].tfield); delete (yysemantic_stack_[(5) - (5)].tfield);
          }
    break;

  case 494:

/* Line 690 of lalr1.cc  */
#line 2605 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = driver.toPoint(*(yysemantic_stack_[(4) - (3)].tfield)).ptr();
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 496:

/* Line 690 of lalr1.cc  */
#line 2610 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield)=driver.getField<CML::tensor>(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
          }
    break;

  case 497:

/* Line 690 of lalr1.cc  */
#line 2614 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield)=driver.getOldTimeField<CML::tensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 498:

/* Line 690 of lalr1.cc  */
#line 2621 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = CML::min(*(yysemantic_stack_[(6) - (3)].tfield),*(yysemantic_stack_[(6) - (5)].tfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].tfield); delete (yysemantic_stack_[(6) - (5)].tfield);
          }
    break;

  case 499:

/* Line 690 of lalr1.cc  */
#line 2625 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = CML::max(*(yysemantic_stack_[(6) - (3)].tfield),*(yysemantic_stack_[(6) - (5)].tfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].tfield); delete (yysemantic_stack_[(6) - (5)].tfield);
          }
    break;

  case 500:

/* Line 690 of lalr1.cc  */
#line 2632 "../PatchValueExpressionParser.yy"
    {
      (yyval.tfield)=driver.evaluatePluginFunction<CML::tensor>(
          *(yysemantic_stack_[(3) - (1)].name),
          (yylocation_stack_[(3) - (2)]),
          numberOfFunctionChars,
          true
      ).ptr();
      delete (yysemantic_stack_[(3) - (1)].name);
  }
    break;

  case 501:

/* Line 690 of lalr1.cc  */
#line 2643 "../PatchValueExpressionParser.yy"
    { (yyval.yfield) = (yysemantic_stack_[(1) - (1)].yfield); }
    break;

  case 502:

/* Line 690 of lalr1.cc  */
#line 2644 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) + *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 503:

/* Line 690 of lalr1.cc  */
#line 2649 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 504:

/* Line 690 of lalr1.cc  */
#line 2654 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 505:

/* Line 690 of lalr1.cc  */
#line 2659 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(
                symm(*(yysemantic_stack_[(3) - (1)].yfield) & *(yysemantic_stack_[(3) - (3)].yfield))
            );
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 506:

/* Line 690 of lalr1.cc  */
#line 2670 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].hfield) & *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 507:

/* Line 690 of lalr1.cc  */
#line 2675 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) & *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 508:

/* Line 690 of lalr1.cc  */
#line 2680 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 509:

/* Line 690 of lalr1.cc  */
#line 2685 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) - *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 510:

/* Line 690 of lalr1.cc  */
#line 2690 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField(-*(yysemantic_stack_[(2) - (2)].yfield));
            delete (yysemantic_stack_[(2) - (2)].yfield);
          }
    break;

  case 511:

/* Line 690 of lalr1.cc  */
#line 2694 "../PatchValueExpressionParser.yy"
    { (yyval.yfield) = (yysemantic_stack_[(3) - (2)].yfield); }
    break;

  case 512:

/* Line 690 of lalr1.cc  */
#line 2695 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::symm(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 513:

/* Line 690 of lalr1.cc  */
#line 2699 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::symm(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 514:

/* Line 690 of lalr1.cc  */
#line 2703 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::twoSymm(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 515:

/* Line 690 of lalr1.cc  */
#line 2707 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::twoSymm(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 516:

/* Line 690 of lalr1.cc  */
#line 2711 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::inv(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 517:

/* Line 690 of lalr1.cc  */
#line 2715 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::cof(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 518:

/* Line 690 of lalr1.cc  */
#line 2719 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::dev(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 519:

/* Line 690 of lalr1.cc  */
#line 2723 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::dev2(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 520:

/* Line 690 of lalr1.cc  */
#line 2727 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::sqr(*(yysemantic_stack_[(4) - (3)].vfield)) );
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 521:

/* Line 690 of lalr1.cc  */
#line 2731 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = (yysemantic_stack_[(6) - (1)].yfield);
          }
    break;

  case 522:

/* Line 690 of lalr1.cc  */
#line 2734 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].yfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].yfield));
            (yyval.yfield) = driver.doConditional(
                *(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].yfield),*(yysemantic_stack_[(5) - (5)].yfield)
            ).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].yfield); delete (yysemantic_stack_[(5) - (5)].yfield);
          }
    break;

  case 523:

/* Line 690 of lalr1.cc  */
#line 2741 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = driver.toPoint(*(yysemantic_stack_[(4) - (3)].yfield)).ptr();
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 525:

/* Line 690 of lalr1.cc  */
#line 2746 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield)=driver.getField<CML::symmTensor>(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
          }
    break;

  case 526:

/* Line 690 of lalr1.cc  */
#line 2750 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield)=driver.getOldTimeField<CML::symmTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 527:

/* Line 690 of lalr1.cc  */
#line 2756 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = CML::min(*(yysemantic_stack_[(6) - (3)].yfield),*(yysemantic_stack_[(6) - (5)].yfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].yfield); delete (yysemantic_stack_[(6) - (5)].yfield);
          }
    break;

  case 528:

/* Line 690 of lalr1.cc  */
#line 2760 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = CML::max(*(yysemantic_stack_[(6) - (3)].yfield),*(yysemantic_stack_[(6) - (5)].yfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].yfield); delete (yysemantic_stack_[(6) - (5)].yfield);
          }
    break;

  case 529:

/* Line 690 of lalr1.cc  */
#line 2767 "../PatchValueExpressionParser.yy"
    {
      (yyval.yfield)=driver.evaluatePluginFunction<CML::symmTensor>(
          *(yysemantic_stack_[(3) - (1)].name),
          (yylocation_stack_[(3) - (2)]),
          numberOfFunctionChars,
          true
      ).ptr();
      delete (yysemantic_stack_[(3) - (1)].name);
  }
    break;

  case 530:

/* Line 690 of lalr1.cc  */
#line 2778 "../PatchValueExpressionParser.yy"
    { (yyval.hfield) = (yysemantic_stack_[(1) - (1)].hfield); }
    break;

  case 531:

/* Line 690 of lalr1.cc  */
#line 2779 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) + *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 532:

/* Line 690 of lalr1.cc  */
#line 2784 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 533:

/* Line 690 of lalr1.cc  */
#line 2789 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 534:

/* Line 690 of lalr1.cc  */
#line 2794 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) & *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 535:

/* Line 690 of lalr1.cc  */
#line 2799 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 536:

/* Line 690 of lalr1.cc  */
#line 2804 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) - *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 537:

/* Line 690 of lalr1.cc  */
#line 2809 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField(-*(yysemantic_stack_[(2) - (2)].hfield));
            delete (yysemantic_stack_[(2) - (2)].hfield);
          }
    break;

  case 538:

/* Line 690 of lalr1.cc  */
#line 2813 "../PatchValueExpressionParser.yy"
    { (yyval.hfield) = (yysemantic_stack_[(3) - (2)].hfield); }
    break;

  case 539:

/* Line 690 of lalr1.cc  */
#line 2814 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField( CML::sph(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 540:

/* Line 690 of lalr1.cc  */
#line 2818 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField( CML::sph(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 541:

/* Line 690 of lalr1.cc  */
#line 2822 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField( CML::sph(*(yysemantic_stack_[(4) - (3)].hfield)) );
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 542:

/* Line 690 of lalr1.cc  */
#line 2826 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = (yysemantic_stack_[(6) - (1)].hfield);
          }
    break;

  case 543:

/* Line 690 of lalr1.cc  */
#line 2829 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].hfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].hfield));
            (yyval.hfield) = driver.doConditional(
                *(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].hfield),*(yysemantic_stack_[(5) - (5)].hfield)
            ).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].hfield); delete (yysemantic_stack_[(5) - (5)].hfield);
          }
    break;

  case 544:

/* Line 690 of lalr1.cc  */
#line 2836 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = driver.toPoint(*(yysemantic_stack_[(4) - (3)].hfield)).ptr();
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 546:

/* Line 690 of lalr1.cc  */
#line 2841 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield)=driver.getField<CML::sphericalTensor>(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
          }
    break;

  case 547:

/* Line 690 of lalr1.cc  */
#line 2845 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield)=driver.getOldTimeField<CML::sphericalTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 548:

/* Line 690 of lalr1.cc  */
#line 2852 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = CML::min(*(yysemantic_stack_[(6) - (3)].hfield),*(yysemantic_stack_[(6) - (5)].hfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].hfield); delete (yysemantic_stack_[(6) - (5)].hfield);
          }
    break;

  case 549:

/* Line 690 of lalr1.cc  */
#line 2856 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = CML::max(*(yysemantic_stack_[(6) - (3)].hfield),*(yysemantic_stack_[(6) - (5)].hfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].hfield); delete (yysemantic_stack_[(6) - (5)].hfield);
          }
    break;

  case 550:

/* Line 690 of lalr1.cc  */
#line 2863 "../PatchValueExpressionParser.yy"
    {
      (yyval.hfield)=driver.evaluatePluginFunction<CML::sphericalTensor>(
          *(yysemantic_stack_[(3) - (1)].name),
          (yylocation_stack_[(3) - (2)]),
          numberOfFunctionChars,
          true
      ).ptr();
      delete (yysemantic_stack_[(3) - (1)].name);
  }
    break;

  case 551:

/* Line 690 of lalr1.cc  */
#line 2874 "../PatchValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.composeVectorField(*(yysemantic_stack_[(8) - (3)].sfield),*(yysemantic_stack_[(8) - (5)].sfield),*(yysemantic_stack_[(8) - (7)].sfield)).ptr();
            delete (yysemantic_stack_[(8) - (3)].sfield); delete (yysemantic_stack_[(8) - (5)].sfield); delete (yysemantic_stack_[(8) - (7)].sfield);
          }
    break;

  case 552:

/* Line 690 of lalr1.cc  */
#line 2880 "../PatchValueExpressionParser.yy"
    {
            (yyval.tfield) = driver.composeTensorField(
                *(yysemantic_stack_[(20) - (3)].sfield),*(yysemantic_stack_[(20) - (5)].sfield),*(yysemantic_stack_[(20) - (7)].sfield),
                *(yysemantic_stack_[(20) - (9)].sfield),*(yysemantic_stack_[(20) - (11)].sfield),*(yysemantic_stack_[(20) - (13)].sfield),
                *(yysemantic_stack_[(20) - (15)].sfield),*(yysemantic_stack_[(20) - (17)].sfield),*(yysemantic_stack_[(20) - (19)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(20) - (3)].sfield); delete (yysemantic_stack_[(20) - (5)].sfield); delete (yysemantic_stack_[(20) - (7)].sfield); delete (yysemantic_stack_[(20) - (9)].sfield); delete (yysemantic_stack_[(20) - (11)].sfield);
            delete (yysemantic_stack_[(20) - (13)].sfield); delete (yysemantic_stack_[(20) - (15)].sfield); delete (yysemantic_stack_[(20) - (17)].sfield); delete (yysemantic_stack_[(20) - (19)].sfield);
          }
    break;

  case 553:

/* Line 690 of lalr1.cc  */
#line 2890 "../PatchValueExpressionParser.yy"
    {
            (yyval.yfield) = driver.composeSymmTensorField(
                *(yysemantic_stack_[(14) - (3)].sfield),*(yysemantic_stack_[(14) - (5)].sfield),*(yysemantic_stack_[(14) - (7)].sfield),
                *(yysemantic_stack_[(14) - (9)].sfield),*(yysemantic_stack_[(14) - (11)].sfield),
                *(yysemantic_stack_[(14) - (13)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(14) - (3)].sfield); delete (yysemantic_stack_[(14) - (5)].sfield); delete (yysemantic_stack_[(14) - (7)].sfield); delete (yysemantic_stack_[(14) - (9)].sfield); delete (yysemantic_stack_[(14) - (11)].sfield); delete (yysemantic_stack_[(14) - (13)].sfield);
          }
    break;

  case 554:

/* Line 690 of lalr1.cc  */
#line 2899 "../PatchValueExpressionParser.yy"
    {
            (yyval.hfield) = driver.composeSphericalTensorField(*(yysemantic_stack_[(4) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 555:

/* Line 690 of lalr1.cc  */
#line 2905 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(
                *(yysemantic_stack_[(3) - (1)].sfield),
                std::less<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 556:

/* Line 690 of lalr1.cc  */
#line 2914 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(
                *(yysemantic_stack_[(3) - (1)].sfield),
                std::greater<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 557:

/* Line 690 of lalr1.cc  */
#line 2923 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(
                *(yysemantic_stack_[(3) - (1)].sfield),
                std::less_equal<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 558:

/* Line 690 of lalr1.cc  */
#line 2932 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(
                *(yysemantic_stack_[(3) - (1)].sfield),
                std::greater_equal<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 559:

/* Line 690 of lalr1.cc  */
#line 2941 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(
                *(yysemantic_stack_[(3) - (1)].sfield),
                std::equal_to<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 560:

/* Line 690 of lalr1.cc  */
#line 2950 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(
                *(yysemantic_stack_[(3) - (1)].sfield),
                std::not_equal_to<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 561:

/* Line 690 of lalr1.cc  */
#line 2959 "../PatchValueExpressionParser.yy"
    { (yyval.lfield) = (yysemantic_stack_[(3) - (2)].lfield); }
    break;

  case 562:

/* Line 690 of lalr1.cc  */
#line 2960 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].lfield),(yysemantic_stack_[(3) - (3)].lfield));
            (yyval.lfield) = driver.doLogicalOp(
                *(yysemantic_stack_[(3) - (1)].lfield),
                std::logical_and<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].lfield)
            ).ptr();
            delete (yysemantic_stack_[(3) - (1)].lfield); delete (yysemantic_stack_[(3) - (3)].lfield);
          }
    break;

  case 563:

/* Line 690 of lalr1.cc  */
#line 2969 "../PatchValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].lfield),(yysemantic_stack_[(3) - (3)].lfield));
            (yyval.lfield) = driver.doLogicalOp(
                *(yysemantic_stack_[(3) - (1)].lfield),
                std::logical_or<CML::scalar>(),
                *(yysemantic_stack_[(3) - (3)].lfield)).ptr();
            delete (yysemantic_stack_[(3) - (1)].lfield); delete (yysemantic_stack_[(3) - (3)].lfield);
          }
    break;

  case 564:

/* Line 690 of lalr1.cc  */
#line 2977 "../PatchValueExpressionParser.yy"
    {
            (yyval.lfield) = driver.doLogicalNot(*(yysemantic_stack_[(2) - (2)].lfield)).ptr();
            delete (yysemantic_stack_[(2) - (2)].lfield);
          }
    break;

  case 566:

/* Line 690 of lalr1.cc  */
#line 2982 "../PatchValueExpressionParser.yy"
    {
        (yyval.lfield)=driver.getVariable<bool>(*(yysemantic_stack_[(1) - (1)].name),driver.pointSize()).ptr();
        delete (yysemantic_stack_[(1) - (1)].name);
 }
    break;

  case 567:

/* Line 690 of lalr1.cc  */
#line 2989 "../PatchValueExpressionParser.yy"
    {
      (yyval.lfield)=driver.evaluatePluginFunction<bool>(
          *(yysemantic_stack_[(3) - (1)].name),
          (yylocation_stack_[(3) - (2)]),
          numberOfFunctionChars,
          true
      ).ptr();
      delete (yysemantic_stack_[(3) - (1)].name);
  }
    break;



/* Line 690 of lalr1.cc  */
#line 7771 "PatchValueExpressionParser.tab.cc"
	default:
          break;
      }
    /* User semantic actions sometimes alter yychar, and that requires
       that yytoken be updated with the new translation.  We take the
       approach of translating immediately before every use of yytoken.
       One alternative is translating here after every semantic action,
       but that translation would be missed if the semantic action
       invokes YYABORT, YYACCEPT, or YYERROR immediately after altering
       yychar.  In the case of YYABORT or YYACCEPT, an incorrect
       destructor might then be invoked immediately.  In the case of
       YYERROR, subsequent parser actions might lead to an incorrect
       destructor call or verbose syntax error message before the
       lookahead is translated.  */
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* Make sure we have latest lookahead translation.  See comments at
       user semantic actions for why this is necessary.  */
    yytoken = yytranslate_ (yychar);

    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	if (yychar == yyempty_)
	  yytoken = yyempty_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[1] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[1] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (!yy_pact_value_is_default_ (yyn))
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[1] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[2] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      {
        /* Make sure we have latest lookahead translation.  See comments
           at user semantic actions for why this is necessary.  */
        yytoken = yytranslate_ (yychar);
        yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval,
                     &yylloc);
      }

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  PatchValueExpressionParser::yysyntax_error_ (int yystate, int yytoken)
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yychar.
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            /* Stay within bounds of both yycheck and yytname.  */
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = 0;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const short int PatchValueExpressionParser::yypact_ninf_ = -434;
  const short int
  PatchValueExpressionParser::yypact_[] =
  {
        61,  2389,  2593,  2593,  2593,  2593,  2593,  2593,  2593,  2593,
    2593,  2593,  2593,  2593,  3169,  3169,  3169,  3169,  3169,  3169,
    3169,  3169,  3169,  3169,  3169,  3169,  -187,  -171,    31,  -434,
    -434,  -169,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -161,  -159,  -153,  -145,  -143,  -141,
    -121,  -103,  -100,   -94,   -90,   -56,  -434,   -50,   -49,   -42,
     -37,  -434,  -434,  -434,  -434,   -33,    -8,    -4,     8,    23,
      27,    36,    40,    41,    44,    47,    56,    60,    65,    66,
      67,    69,    73,    78,    79,    80,    82,    86,    91,    92,
      93,    95,    99,   103,   105,   113,   114,   117,   122,   123,
     124,   126,   132,   137,   139,   141,   147,   149,   151,   152,
     155,   159,   160,   161,   162,   207,   217,   219,   223,   225,
     227,   228,   231,   235,   237,   245,   246,   249,   254,   255,
     256,   258,   262,   267,   268,   269,   271,   275,   280,   281,
    2389,  2389,  2389,  2389,  -434,  6063,   -57,  -434,  -434,  6036,
     -57,  2404,   -57,  5621,   -57,  5634,   -57,  -134,   -57,  -434,
    -434,  -434,  -434,  6071,   -57,  6047,   -57,  5647,   -57,  5660,
     -57,  5673,   -57,  -434,  -434,  -434,  -434,  -117,   -57,   282,
     284,   288,   295,   297,   300,   305,   306,   318,   333,   340,
     341,   351,   352,   355,   360,   364,   405,   406,   407,   413,
     448,   453,   460,   461,   473,   474,   475,   476,   479,   481,
     482,   483,   484,   489,   496,   501,   504,   508,   509,   511,
     513,   516,   519,   528,   532,   536,   537,   538,  2593,  2593,
    2593,  2593,  6063,  4979,  2404,  5621,  5634,  -134,   897,  2584,
    6036,  3047,   166,    -6,  1407,     7,  1664,    25,  -147,  -151,
     539,   541,   545,   549,   555,   592,   595,   596,   599,   603,
     604,   605,   608,   612,   615,   619,   620,   621,   624,   627,
     631,   639,   640,   641,   646,   652,   659,   661,   665,   670,
     671,   672,   674,   678,   683,   684,   685,   687,   691,   696,
     697,   698,   700,   704,   709,   710,   711,   713,   717,  3169,
    3169,  3169,  3169,  6071,  4997,  5647,  5660,  5673,  -117,  3042,
    5994,  6047,  5709,  2119,    38,  2900,    58,  3085,    71,   -15,
     -38,  -434,  -434,  -434,  2389,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  2389,  2389,  2389,
    2389,   -29,   -28,  -150,  -119,   -27,   -26,   -30,   -19,    15,
      32,   111,   129,   243,   732,   934,  1048,  6158,   722,   723,
    2389,  2389,  2389,  2389,  2389,  2389,  2389,  2389,  2389,  2389,
    2389,  2389,  2389,  2389,  2389,  2389,  2389,  2389,  2389,  2389,
    2389,  2389,  2389,  2389,  2389,  2389,  2389,  2389,  2389,  2389,
    2389,  2389,  2389,  2389,  2389,  2389,  2593,  3169,  1291,  1315,
     725,  2389,  2593,  2389,  2389,  2389,  2593,  2389,  2389,  2389,
    2389,  2389,  2389,  2389,   727,   729,   -54,  -434,   462,   503,
     518,   736,  -434,   737,   738,   740,   462,   503,   737,   738,
    5722,  3072,    84,    97,   110,   -31,  5735,  3163,   128,   221,
     242,   -20,  -434,  -434,  2593,  2593,  2593,  2593,  2593,  2593,
    -434,  -434,   -57,  -434,  2593,  2593,  2593,  2593,  2593,  2593,
    2593,  2593,  2593,  2593,  2593,  -434,  2593,  2593,  2593,  2593,
    2593,  2593,  -434,  -434,  2593,  2593,  2593,  2593,  2593,  2593,
    -434,  -434,  2593,  2593,  2593,  2593,  2593,  2593,  -434,  -434,
    2593,  2593,  2593,  -434,  3169,  3169,  3169,  3169,  3169,  3169,
    -434,  -434,  3169,  3169,  3169,  3169,  3169,  3169,  3169,  3169,
    3169,  3169,  3169,  -434,  3169,  3169,  3169,  3169,  3169,  3169,
    -434,  -434,  3169,  3169,  3169,  3169,  3169,  3169,  -434,  -434,
    3169,  3169,  3169,  3169,  3169,  3169,  -434,  -434,  3169,  3169,
    3169,  -434,  2593,  2593,  2593,  2593,  2593,  1496,  2593,  2593,
    2593,  2593,  2593,  2593,  2593,  2593,  2389,  2389,  2593,  2593,
    2593,  2593,  2593,  2593,  2593,  2593,  2593,  2593,  2593,  2593,
    2593,  2593,  2593,  2593,  2593,  2593,  2593,  2593,  2593,  2593,
    2593,  2593,  2593,  2593,  2593,  2593,  2593,  2593,  2593,  2593,
    2593,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  3169,  3169,  3169,  3169,  3169,   827,  3169,
    3169,  3169,  3169,  3169,  3169,  3169,  3169,  3169,  3169,  3169,
    3169,  3169,  3169,  3169,  3169,  3169,  3169,  3169,  3169,  3169,
    3169,  3169,  3169,  3169,  3169,  3169,  3169,  3169,  3169,  3169,
    3169,  3169,  3169,  3169,  3169,  3169,  3169,  3169,  3169,  3169,
    3169,  3169,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  3190,  3209,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  5015,  5033,
    5051,  5069,  5087,  5105,  3228,  3247,   730,  -434,   739,  -434,
    -434,  -434,   743,  -434,   744,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,   752,   754,   757,   758,   759,   760,   769,
     770,   771,   773,   777,   783,   786,   790,   795,   796,   799,
     803,   807,   808,   809,   810,   812,   815,   816,  -434,  -434,
    5123,  5141,  3266,  3285,  3304,  3323,  5748,  3342,   260,   273,
     286,  5761,  3361,   299,   500,   515,  5774,  3380,   530,   543,
     610,  5787,  3399,   626,   663,   676,  3418,  3437,  3456,  3475,
    3494,  3513,   459,  2793,  4818,  4831,  4844,  1276,  2887,  4857,
    4870,  4883,  2288,  2978,  4896,  4909,  4922,  2492,  3023,  4935,
    4948,  4961,  3532,  3551,  3570,  3589,  5800,  3608,  5813,  3627,
    5826,  3646,  5839,  3665,  5852,  3684,  5865,  3703,  3722,  3741,
    3760,  3779,  3798,  3817,  3836,  3855,  3874,  3893,  3912,  3931,
    3950,  3969,  3988,  4007,  4026,  4045,  4064,  4083,  4102,  4121,
    4140,  4159,  4178,  4197,  4216,  4235,  4254,  4273,  4292,  4311,
    4330,  4349,  4368,  4387,  4406,  4425,  4444,  4463,  4482,  4501,
    5878,  4520,   689,   702,   715,  5891,  4539,   728,   774,   787,
     817,   820,   823,   828,   829,   830,   832,   834,   836,   837,
    -434,   800,   813,   833,   859,   918,   932,   958,   976,   991,
    1031,  1049,  1062,  1075,  1088,  1154,  1197,  1225,  1352,  1394,
    1430,  1468,  1546,  1559,  1612,  1625,  1638,  1651,  1680,  1746,
    1760,  1774,  1818,  1832,  1845,  1858,  1885,  1923,  1952,  1975,
    1989,  2002,  2039,  2066,  2080,  2093,  2106,  2135,  2148,  2161,
    2174,  2190,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,    10,    10,   -54,  -434,
    -434,   -54,   462,   503,   518,   -54,    90,  -434,  6036,  6036,
    6036,  6036,  6036,  6036,    30,    30,  -434,   -54,  -434,   462,
     503,   518,  -434,   337,  2026,  6079,  -174,    42,   224,  -174,
      42,   224,  -434,  -434,   -54,   462,   503,   518,   -21,   337,
    2026,  6079,  -174,    42,   224,  -174,    42,   224,  -434,  -434,
     -54,   462,   503,   518,     5,   337,  2026,  6079,  -174,    42,
     224,  -174,    42,   224,  -434,  -434,   -54,   462,   503,   518,
     -84,   143,  5690,  1446,  1507,  2249,   244,  -434,   213,   213,
     736,  -434,  -434,   736,   737,   738,   740,   736,   102,  6047,
    6047,  6047,  6047,  6047,  6047,    63,    63,  -434,   736,  -434,
     737,   738,   740,  -434,  6087,  6095,  6103,   302,   302,  -434,
    -434,   736,   737,   738,   740,    -5,  6087,  6095,  6103,   329,
     329,  -434,  -434,   736,   737,   738,   740,    14,  6087,  6095,
    6103,   345,   345,  -434,  -434,   736,   737,   738,   740,   -83,
     366,  5704,  2264,  2390,  2443,   846,  -434,  5904,  4558,  5917,
    4577,  6006,  5159,  6018,  5177,  -434,  -434,  2593,  3169,  2593,
    3169,  2593,  3169,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  2593,  2799,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  2593,  -434,  2593,  -434,
    2593,  2593,  2593,  3169,  -434,  3169,  -434,  3169,  3169,  3169,
    2593,  -434,  2593,  -434,  2593,  2593,  2593,  3169,  -434,  3169,
    -434,  3169,  3169,  3169,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,   840,  -434,
    -434,  -434,  -434,  -434,  -434,   841,  -434,   844,  2593,  2593,
    2593,  2593,  2593,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,   845,  -434,
    -434,  -434,  -434,  -434,  -434,   848,  -434,   852,  3169,  3169,
    3169,  3169,  3169,  5195,  5213,  5231,  5249,  5267,  5285,  4596,
    -434,  2984,   854,  4615,  5930,  4634,  2459,  2472,  2598,  5943,
    4653,  2611,  2648,  2663,  5956,  4672,  2676,  2696,  2709,  5969,
    4691,  2833,  2846,  2859,   855,   856,   858,  6063,  6036,  2404,
    5621,  5634,   862,   866,   878,  6071,  6047,  5647,  5660,  5673,
    2593,  3169,  2593,  3169,  2593,  3169,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,  -434,
    -434,  -434,  -434,  -434,  -434,  -434,  4710,  4729,  5303,  5321,
    5339,  5357,  -434,  -434,  2593,  3169,  2593,  3169,  5375,  5393,
    5411,  5429,  2593,  3169,  2593,  3169,  5447,  5465,  5483,  5501,
    2593,  3169,  2593,  3169,  5519,  5537,  4748,  4767,  2593,  3169,
    -434,  -434,  5555,  5573,  2593,  3169,  5591,  5609,  2593,  3169,
    4786,  4805,  -434,  -434
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned short int
  PatchValueExpressionParser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     2,
     218,     0,   214,    80,   348,   259,   297,   325,   465,   382,
     566,   496,   525,   546,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   112,     0,     0,     0,
       0,   335,   336,   308,   195,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     3,    33,     0,   113,    48,    32,
       0,    38,     0,    40,     0,    42,     0,    34,     0,    47,
     226,   269,   307,    36,     0,    35,     0,    39,     0,    41,
       0,    43,     0,   354,   471,   501,   530,    37,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    29,     1,     0,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,   132,   246,   282,
     315,   367,   406,   483,   510,   537,    62,    63,   368,   369,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   346,   564,     0,     0,     0,     0,     0,     0,
      44,    30,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   213,     0,     0,     0,     0,
       0,     0,    45,   258,     0,     0,     0,     0,     0,     0,
      45,   296,     0,     0,     0,     0,     0,     0,    45,   324,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
      44,   381,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   464,     0,     0,     0,     0,     0,     0,
      45,   495,     0,     0,     0,     0,     0,     0,    45,   524,
       0,     0,     0,     0,     0,     0,    45,   545,     0,     0,
       0,   565,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     8,     9,    12,    13,    16,    17,    20,
      21,    24,    25,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     6,     7,    10,    11,    14,    15,    18,    19,
      22,    23,    26,    27,     0,     0,   225,   470,    89,   386,
     268,   500,   306,   529,   334,   550,   349,   567,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   200,     0,   202,
     196,   197,     0,   205,     0,   207,    73,   212,    75,    76,
      77,   204,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   209,   210,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   198,   199,    64,   133,   247,   283,   316,   343,
     377,   407,   484,   511,   538,   561,    60,    49,   234,    51,
      58,   122,    53,    55,    57,    59,     0,    31,   342,   341,
     337,   338,   340,   339,   115,   114,   117,    50,   116,   232,
     273,   310,   118,   123,   124,   125,   241,   242,   243,   227,
     228,   229,   233,   240,    52,   235,   237,   239,     0,   126,
     127,   128,   244,   279,   280,   230,   270,   271,   274,   278,
      54,   236,   275,   277,     0,   129,   130,   131,   245,   281,
     314,   231,   272,   309,   311,   313,    56,   238,   276,   312,
       0,     0,     0,     0,     0,     0,   345,   344,   366,   355,
     475,   357,   364,   396,   359,   361,   363,   365,     0,   560,
     559,   555,   556,   558,   557,   388,   387,   390,   356,   389,
     473,   503,   532,   391,   397,   398,   399,   482,   472,   474,
     481,   358,   476,   478,   480,     0,   400,   401,   402,   509,
     502,   504,   508,   360,   477,   505,   507,     0,   403,   404,
     405,   536,   531,   533,   535,   362,   479,   506,   534,     0,
       0,     0,     0,     0,     0,   563,   562,     0,     0,     0,
       0,     0,     0,     0,     0,   219,   467,     0,     0,     0,
       0,     0,     0,   353,   554,   201,   203,   206,   208,   220,
      84,   263,   301,   329,   221,    85,   264,   302,   330,   222,
      86,   265,   303,   331,   215,    81,   260,   298,   326,   466,
     383,   497,   526,   547,     0,     0,   120,   394,   121,   395,
     160,   159,   161,   162,   163,   434,   433,   435,   436,   437,
     165,   164,   166,   167,   168,   439,   438,   440,   441,   442,
     136,   410,   137,   411,   138,   412,     0,   100,     0,    92,
       0,     0,     0,     0,   102,     0,    94,     0,     0,     0,
       0,   101,     0,    93,     0,     0,     0,     0,   103,     0,
      95,     0,     0,     0,   104,   106,   105,   107,   110,    98,
     111,    99,   108,    96,   109,    97,   292,   134,   520,   408,
     135,   409,   139,   413,   140,   414,   141,   415,   142,   416,
     143,   417,   144,   418,   145,   419,   146,   420,   147,   421,
     148,   422,   149,   423,   150,   424,   151,   425,   152,   426,
     153,   427,   154,   428,   155,   429,   156,   430,   157,   431,
     158,   432,   380,   463,   494,   523,   544,    78,   211,   257,
     295,   323,   216,    82,   261,   299,   327,   217,    83,   262,
     300,   328,    70,    71,   375,   376,   169,   170,   171,   253,
     290,   490,   518,   285,   284,   513,   512,   248,   485,   172,
     173,   174,   252,   289,   489,   517,   251,   288,   320,   488,
     516,   317,   318,   319,   539,   540,   541,   287,   286,   515,
     514,   254,   291,   491,   519,    65,    66,   370,   371,   249,
     250,   486,   487,   175,   176,   177,    67,    68,    69,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   187,
     188,   189,   190,   191,   192,     0,   193,     0,     0,     0,
       0,     0,     0,   443,   444,   445,   372,   373,   374,   446,
     447,   448,   449,   450,   451,   452,   453,   454,     0,   455,
     456,   457,   458,   459,   460,     0,   461,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    72,   194,   256,
     294,   322,     0,     0,     0,   378,   462,   493,   522,   543,
       0,     0,     0,     0,     0,     0,   119,    91,   392,   393,
      87,   223,   266,   304,   332,   384,   468,   498,   527,   548,
      88,   224,   267,   305,   333,   385,   469,   499,   528,   549,
     255,   293,   321,   492,   521,   542,     0,     0,     0,     0,
       0,     0,   350,   551,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     352,   553,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   552
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  PatchValueExpressionParser::yypgoto_[] =
  {
      -434,  -434,  -434,  -120,  -434,   586,  -433,  5907,  1756,  -434,
    -434,  -434,  -434,   202,  -434,  1506,  -434,  1301,  -434,  1087,
    -434,     6,  -434,  -434,  -434,  -434,  -434,   835,  -434,    -1,
    -434,   419,  -434,   784,  -434,   989,  -434,  -434,  -434,  -434,
    -434,     4,  -434
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  PatchValueExpressionParser::yydefgoto_[] =
  {
        -1,    28,    29,   453,   144,   936,   968,   666,   232,   146,
    1382,   147,   148,   240,   150,   234,   152,   235,   154,   236,
     156,   237,   158,   159,   160,   161,   162,   303,   164,   311,
     166,   305,   168,   306,   170,   307,   172,   173,   174,   175,
     176,   308,   178
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char PatchValueExpressionParser::yytable_ninf_ = -1;
  const unsigned short int
  PatchValueExpressionParser::yytable_[] =
  {
       165,   686,   688,   692,   694,   177,   321,   157,  1336,  1366,
     469,   470,   471,   304,   309,   451,   452,   472,   248,   249,
     490,   322,   491,   492,   490,   324,   491,   492,   319,   320,
     465,   323,   473,   325,   481,   326,   489,   490,   493,   491,
     492,   327,   602,   690,   501,   601,   513,   984,   521,   328,
     529,   329,   537,   330,   538,  1000,   539,   540,   541,  1316,
    1317,  1318,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1326,
    1327,  1337,  1367,   331,   691,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1353,  1354,  1355,  1356,  1357,  1045,  1329,  1330,
    1331,   332,  1332,  1333,   333,  1057,  1334,  1359,  1360,  1361,
     334,  1362,  1363,  1069,   335,  1364,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,   538,  1328,   539,   540,   450,   336,   422,
     490,   437,   491,   492,   337,   338,   441,   443,   435,   442,
    1358,   538,   339,   539,   540,   663,   538,   340,   539,   540,
    1335,   341,   919,   696,   687,   689,   693,   695,   466,  1365,
    1313,  1314,  1315,   925,   697,   467,   468,   662,   469,   470,
     471,   474,  1343,  1344,  1345,   472,   342,   596,   475,   476,
     343,   477,   478,   479,   446,   447,   448,   449,   480,   482,
     598,   450,   344,   149,   233,   238,   483,   484,   698,   485,
     486,   487,   514,   462,   463,   464,   488,   345,   600,   515,
     516,   346,   517,   518,   519,   699,   477,   478,   479,   520,
     347,   657,   522,   480,   348,   349,   435,   442,   350,   523,
     524,   351,   525,   526,   527,   530,   510,   511,   512,   528,
     352,   659,   531,   532,   353,   533,   534,   535,   466,   354,
     355,   356,   536,   357,   661,   467,   468,   358,   469,   470,
     471,   474,   359,   360,   361,   472,   362,   916,   475,   476,
     363,   477,   478,   479,   482,   364,   365,   366,   480,   367,
     917,   483,   484,   368,   485,   486,   487,   369,   422,   370,
     437,   488,   514,   918,   700,   441,   443,   371,   372,   515,
     516,   373,   517,   518,   519,  1338,   374,   375,   376,   520,
     377,   922,   701,   665,   444,   445,   378,   446,   447,   448,
     449,   379,   937,   380,   450,   381,   679,   681,   683,   685,
     466,   382,   417,   383,   431,   384,   385,   467,   468,   386,
     469,   470,   471,   387,   388,   389,   390,   472,   595,   731,
     733,   735,   742,   752,   757,   759,   761,   768,   778,   783,
     785,   789,   793,   797,   799,   801,   803,   805,   807,   809,
     811,   813,   815,   817,   819,   821,   823,   825,   827,   829,
     831,   833,   835,   837,   839,   522,   846,   496,   497,   498,
     499,   391,   523,   524,   500,   525,   526,   527,   485,   486,
     487,   392,   528,   393,   923,   488,   530,   394,   492,   395,
     167,   396,   397,   531,   532,   398,   533,   534,   535,   399,
     417,   400,   431,   536,   466,   924,   702,   313,   314,   401,
     402,   467,   468,   403,   469,   470,   471,   474,   404,   405,
     406,   472,   407,  1132,   475,   476,   408,   477,   478,   479,
     482,   409,   410,   411,   480,   412,  1133,   483,   484,   413,
     485,   486,   487,   514,   414,   415,   542,   488,   543,  1134,
     515,   516,   544,   517,   518,   519,   517,   518,   519,   545,
     520,   546,  1137,   520,   547,  1011,  1012,  1006,  1007,   548,
     549,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,
    1029,  1033,   550,   525,   526,   527,  1039,  1040,   467,   468,
     528,   469,   470,   471,  1051,  1052,   664,   551,   472,   533,
     534,   535,  1063,  1064,   552,   553,   536,  1071,  1368,   678,
     680,   682,   684,  1075,  1076,   554,   555,   494,   495,   556,
     496,   497,   498,   499,   557,  1078,  1080,   500,   558,   423,
     428,   438,   730,   732,   734,   737,   747,   756,   758,   760,
     763,   773,   782,   784,   787,   791,   795,   798,   800,   802,
     804,   806,   808,   810,   812,   814,   816,   818,   820,   822,
     824,   826,   828,   830,   832,   834,   836,   838,   841,   559,
     560,   561,   665,   679,   681,   683,   685,   562,   731,   733,
     735,   742,   752,   757,   759,   761,  1082,  1084,   797,   799,
     801,   803,   805,   807,   809,   811,   813,   815,   817,   819,
     821,   823,   825,   827,   829,   831,   833,   835,   837,   839,
     444,   445,   563,   446,   447,   448,   449,   564,   929,   930,
     450,  1156,  1157,   472,   565,   566,   938,   939,   940,   941,
     942,   943,   944,   945,   946,   948,   952,   567,   568,   569,
     570,   962,   963,   571,   522,   572,   573,   574,   575,   978,
     979,   523,   524,   576,   525,   526,   527,   994,   995,   530,
     577,   528,  1002,  1138,   480,   578,   531,   532,   579,   533,
     534,   535,   580,   581,   466,   582,   536,   583,  1139,   488,
     584,   467,   468,   585,   469,   470,   471,   474,   423,   428,
     438,   472,   586,  1142,   475,   476,   587,   477,   478,   479,
     588,   589,   590,   603,   480,   604,  1143,   703,   704,   605,
     705,   706,   707,   606,   664,   678,   680,   682,   684,   607,
     730,   732,   734,   737,   747,   756,   758,   760,   763,   773,
     795,   798,   800,   802,   804,   806,   808,   810,   812,   814,
     816,   818,   820,   822,   824,   826,   828,   830,   832,   834,
     836,   838,   743,   753,   482,   169,   608,   769,   779,   609,
     610,   483,   484,   611,   485,   486,   487,   612,   613,   614,
     514,   488,   615,  1144,   315,   316,   616,   515,   516,   617,
     517,   518,   519,   618,   619,   620,   847,   520,   621,  1147,
     863,   622,   870,   874,   877,   623,   883,   888,   893,   898,
     902,   906,   910,   624,   625,   626,   163,   522,   723,   724,
     627,   725,   726,   727,   523,   524,   628,   525,   526,   527,
     530,   310,   312,   629,   528,   630,  1148,   531,   532,   631,
     533,   534,   535,   466,   632,   633,   634,   536,   635,  1149,
     467,   468,   636,   469,   470,   471,   474,   637,   638,   639,
     472,   640,  1244,   475,   476,   641,   477,   478,   479,   482,
     642,   643,   644,   480,   645,  1245,   483,   484,   646,   485,
     486,   487,   514,   647,   648,   649,   488,   650,  1246,   515,
     516,   651,   517,   518,   519,   728,   729,  1014,   860,   520,
     912,  1249,   913,  1095,   424,   429,   439,   500,   520,   528,
    1030,   536,  1096,  1034,  1037,  1038,  1097,  1098,  1042,   708,
     709,  1046,   710,   711,   712,  1099,  1054,  1100,   522,  1058,
    1101,  1102,  1103,  1104,  1066,   523,   524,  1072,   525,   526,
     527,   530,  1105,  1106,  1107,   528,  1108,  1250,   531,   532,
    1109,   533,   534,   535,   466,   421,  1110,   436,   536,  1111,
    1251,   467,   468,  1112,   469,   470,   471,   474,  1113,  1114,
     171,   472,  1115,  1262,   475,   476,  1116,   477,   478,   479,
    1117,  1118,  1119,  1120,   480,  1121,  1263,   514,  1122,  1123,
    1252,   317,   318,  1253,   515,   516,  1254,   517,   518,   519,
     540,  1255,  1256,  1257,   520,  1258,  1264,  1259,     0,  1260,
    1261,   743,   753,   522,  1404,  1405,   769,   779,  1406,  1412,
     523,   524,  1413,   525,   526,   527,  1414,  1428,  1450,  1451,
     528,  1452,  1265,   713,   714,  1453,   715,   716,   717,  1454,
     863,   870,   874,   877,   883,   888,   893,   898,   902,   906,
     910,  1455,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   424,   429,   439,  1018,  1374,   155,  1376,
     592,  1378,   466,     0,     0,     0,     0,   246,   247,   467,
     468,     0,   469,   470,   471,     0,   474,     0,     0,   472,
       0,  1266,     0,   475,   476,     0,   477,   478,   479,     0,
       0,     0,     0,   480,  1383,  1267,     0,     0,     0,   425,
       0,   440,   482,     0,   421,     0,   436,     0,     0,   483,
     484,     0,   485,   486,   487,     0,     0,   744,   754,   488,
     466,  1268,   770,   780,     0,     0,     0,   467,   468,     0,
     469,   470,   471,     0,  1390,   474,     0,   472,     0,  1269,
       0,     0,   475,   476,     0,   477,   478,   479,  1400,     0,
       0,   848,   480,     0,  1270,   864,     0,   871,   875,     0,
       0,   884,   889,   894,   899,   903,   907,   911,   741,   751,
       0,     0,     0,   767,   777,   514,     0,   788,   792,   796,
       0,     0,   515,   516,     0,   517,   518,   519,     0,     0,
       0,     0,   520,   522,  1271,     0,     0,   420,     0,   434,
     523,   524,   845,   525,   526,   527,   466,     0,     0,     0,
     528,     0,  1272,   467,   468,     0,   469,   470,   471,   474,
       0,     0,     0,   472,     0,  1273,   475,   476,     0,   477,
     478,   479,   514,     0,     0,     0,   480,     0,  1274,   515,
     516,     0,   517,   518,   519,     0,     0,     0,     0,   520,
       0,  1275,  1015,     0,     0,     0,     0,     0,   425,  1373,
     440,  1375,     0,  1377,     0,  1031,   850,   851,  1035,   852,
     853,   854,   153,  1043,     0,     0,  1047,  1049,  1050,   244,
     245,  1055,     0,     0,  1059,   420,     0,   434,     0,  1067,
     855,   856,  1073,   857,   858,   859,  1379,     0,   522,  1008,
    1009,  1010,     0,  1013,  1017,   523,   524,     0,   525,   526,
     527,     0,     0,     0,     0,   528,  1028,  1276,     0,     0,
       0,     0,   745,   755,  1041,     0,     0,   771,   781,     0,
    1385,     0,  1053,     0,     0,     0,     0,     0,  1416,     0,
    1065,   466,     0,  1070,  1395,     0,     0,     0,   467,   468,
     422,   469,   470,   471,     0,     0,   849,     0,   472,     0,
    1277,  1077,  1079,     0,     0,     0,   744,   754,   895,   514,
       0,   770,   780,     0,     0,     0,   515,   516,     0,   517,
     518,   519,     0,     0,     0,     0,   520,     0,  1278,     0,
    1457,     0,  1459,     0,  1461,   864,   871,   875,     0,   884,
     889,   894,   899,   903,   907,   911,     0,     0,     0,     0,
       0,   419,   427,   433,     0,     0,     0,   741,   751,     0,
     740,   750,  1081,  1083,   796,   766,   776,   494,   495,     0,
     496,   497,   498,   499,  1469,     0,  1471,   500,  1163,  1164,
       0,     0,  1477,     0,  1479,     0,     0,     0,     0,     0,
    1485,     0,  1487,   844,     0,     0,     0,  1016,  1493,   867,
       0,     0,     0,   880,  1497,   887,   892,     0,  1501,     0,
    1032,   718,   719,  1036,   720,   721,   722,   151,  1044,     0,
       0,  1048,   242,   243,     0,     0,  1056,     0,     0,  1060,
    1061,  1062,     0,     0,  1068,     0,   466,  1074,     0,   419,
     427,   433,     0,   467,   468,   934,   469,   470,   471,     0,
       0,  1408,     0,   472,     0,  1279,     0,     0,     0,     0,
     951,     0,     0,   955,   958,   961,     0,     0,   967,     0,
       0,   971,   974,   977,     0,     0,   983,     0,   474,   987,
     990,   993,     0,     0,   999,   475,   476,  1005,   477,   478,
     479,   474,     0,     0,     0,   480,  1391,  1280,   475,   476,
       0,   477,   478,   479,     0,     0,     0,     0,   480,   597,
    1401,   745,   755,     0,   482,     0,   771,   781,     0,     0,
       0,   483,   484,     0,   485,   486,   487,     0,  1340,     0,
     466,   488,  1456,  1281,  1458,     0,  1460,   467,   468,     0,
     469,   470,   471,     0,     0,     0,   895,   472,   740,   750,
       0,     0,   466,   766,   776,     0,   418,   426,   432,   467,
     468,     0,   469,   470,   471,     0,     0,     0,     0,   472,
       0,  1282,     0,     0,   739,   749,  1468,     0,  1470,   765,
     775,     0,   887,   892,  1476,     0,  1478,     0,     0,  1341,
       0,   474,  1484,     0,  1486,     0,     0,     0,   475,   476,
    1492,   477,   478,   479,     0,     0,  1496,   843,   480,     0,
    1500,     0,   862,   866,   869,   873,     0,   879,   882,   886,
     891,   897,   901,   905,   909,     0,     0,     0,     0,     0,
     474,     0,     0,     0,     0,     0,     0,   475,   476,     0,
     477,   478,   479,   514,   418,   426,   432,   480,     0,  1283,
     515,   516,     0,   517,   518,   519,     0,     0,     0,   933,
     520,     0,  1284,     0,     0,     0,     0,   145,     0,     0,
     239,   241,     0,     0,   950,     0,     0,   954,   957,   960,
       0,     0,   966,     0,     0,   970,   973,   976,     0,     0,
     982,     0,     0,   986,   989,   992,   522,     0,   998,  1417,
       0,  1004,     0,   523,   524,     0,   525,   526,   527,   466,
     423,     0,     0,   528,     0,  1285,   467,   468,     0,   469,
     470,   471,   474,     0,     0,     0,   472,     0,  1286,   475,
     476,     0,   477,   478,   479,   482,     0,     0,     0,   480,
       0,  1287,   483,   484,     0,   485,   486,   487,   482,     0,
       0,     0,   488,     0,  1288,   483,   484,     0,   485,   486,
     487,     0,   739,   749,   514,   488,   599,   765,   775,     0,
       0,   515,   516,     0,   517,   518,   519,     0,     0,   738,
     748,   520,     0,  1289,   764,   774,     0,     0,     0,     0,
       0,   862,   869,   873,     0,   882,   886,   891,   897,   901,
     905,   909,     0,     0,     0,     0,   416,     0,   430,     0,
       0,     0,   842,     0,     0,     0,     0,   861,   865,   868,
     872,   876,   878,   881,   885,   890,   896,   900,   904,   908,
     522,     0,     0,     0,     0,     0,     0,   523,   524,     0,
     525,   526,   527,     0,   466,     0,     0,   528,     0,  1290,
       0,   467,   468,     0,   469,   470,   471,     0,   474,     0,
       0,   472,  1392,  1291,   932,   475,   476,     0,   477,   478,
     479,     0,     0,     0,     0,   480,  1402,  1292,     0,   949,
       0,     0,   953,   956,   959,     0,     0,   965,     0,     0,
     969,   972,   975,     0,   416,   981,   430,     0,   985,   988,
     991,     0,   482,   997,     0,     0,  1003,     0,  1389,   483,
     484,     0,   485,   486,   487,     0,   514,     0,     0,   488,
       0,  1293,  1399,   515,   516,     0,   517,   518,   519,   522,
       0,     0,     0,   520,     0,  1294,   523,   524,     0,   525,
     526,   527,   530,     0,     0,     0,   528,     0,  1295,   531,
     532,     0,   533,   534,   535,     0,     0,     0,     0,   536,
       0,  1296,     0,     0,     0,     0,     0,   738,   748,   466,
       0,     0,   764,   774,     0,     0,   467,   468,     0,   469,
     470,   471,     0,     0,     0,     0,   472,     0,  1297,     0,
       0,     0,     0,     0,     0,     0,   861,   868,   872,   876,
     881,   885,   890,   896,   900,   904,   908,   474,     0,     0,
       0,     0,     0,     0,   475,   476,     0,   477,   478,   479,
       0,     0,     0,     0,   480,     0,  1298,     0,     0,   736,
     746,     0,     0,     0,   762,   772,   514,     0,   786,   790,
     794,     0,     0,   515,   516,     0,   517,   518,   519,     0,
       0,     0,     0,   520,     0,  1299,     0,     0,     0,   522,
       0,     0,   840,     0,     0,  1418,   523,   524,  1393,   525,
     526,   527,     0,   466,     0,   424,   528,     0,  1300,     0,
     467,   468,  1403,   469,   470,   471,   474,     0,     0,     0,
     472,     0,  1301,   475,   476,     0,   477,   478,   479,     0,
       0,     0,     0,   480,     0,  1302,     0,     0,     0,     0,
     926,   927,   928,  1415,   931,   935,     0,   475,   476,     0,
     477,   478,   479,   514,     0,     0,   421,   480,     0,   947,
     515,   516,     0,   517,   518,   519,     0,   964,     0,     0,
     520,     0,  1303,     0,     0,   980,     0,     0,     0,     0,
     522,     0,     0,   996,     0,     0,  1001,   523,   524,  1388,
     525,   526,   527,     0,   466,     0,     0,   528,     0,  1304,
       0,   467,   468,  1398,   469,   470,   471,   474,     0,     0,
       0,   472,     0,  1305,   475,   476,     0,   477,   478,   479,
     514,     0,     0,     0,   480,     0,  1306,   515,   516,     0,
     517,   518,   519,   514,     0,     0,     0,   520,     0,  1307,
     515,   516,     0,   517,   518,   519,     0,   736,   746,   522,
     520,   656,   762,   772,   794,     0,   523,   524,     0,   525,
     526,   527,   466,     0,     0,     0,   528,     0,  1308,   467,
     468,     0,   469,   470,   471,   474,     0,     0,     0,   472,
       0,  1309,   475,   476,     0,   477,   478,   479,   514,     0,
       0,     0,   480,     0,  1310,   515,   516,     0,   517,   518,
     519,  1419,     0,     0,   522,   520,     0,  1311,     0,     0,
     425,   523,   524,     0,   525,   526,   527,     0,     0,     0,
       0,   528,     0,  1312,     0,     0,     0,     0,     0,     0,
       0,     0,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,     0,
       0,  1342,     0,   482,     0,     0,     0,     0,     0,  1411,
     483,   484,     0,   485,   486,   487,  1370,     0,   514,     0,
     488,     0,     0,     0,     0,   515,   516,     0,   517,   518,
     519,     0,     0,     0,     0,   520,     0,     0,     0,     0,
       0,     0,  1387,    57,    58,    59,    60,    61,    62,   444,
     445,     0,   446,   447,   448,   449,  1397,     0,     0,   450,
    1170,  1171,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,     0,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
       0,     0,  1371,     0,   522,     0,     0,     0,     0,     0,
     140,   523,   524,   141,   525,   526,   527,     0,   466,     0,
       0,   528,     0,   142,   143,   467,   468,     0,   469,   470,
     471,     0,     0,     0,     0,   472,    30,   179,    32,    33,
      34,    35,    36,    37,     0,     0,     0,     0,     0,     0,
      44,     0,    46,     0,    48,  1372,    50,   530,    52,     0,
      54,     0,    56,     0,   531,   532,     0,   533,   534,   535,
       0,     0,     0,   466,   536,     0,     0,     0,     0,     0,
     467,   468,  1410,   469,   470,   471,   474,     0,     0,     0,
     472,     0,  1432,   475,   476,     0,   477,   478,   479,     0,
       0,     0,     0,   480,     0,  1433,  1386,   180,   181,   182,
     183,    61,    62,   494,   495,     0,   496,   497,   498,   499,
    1396,     0,     0,   500,  1177,  1178,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,   184,    82,    83,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,    94,    95,    96,
      97,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,     0,   121,   122,   123,     0,     0,   217,
     126,   218,   219,   220,   130,   221,   222,   223,   224,   225,
     226,   227,   138,   139,     0,   444,   445,     0,   446,   447,
     448,   449,   482,     0,   228,   450,   593,   229,     0,   483,
     484,     0,   485,   486,   487,   514,     0,   230,   231,   488,
       0,  1434,   515,   516,     0,   517,   518,   519,     0,     0,
       0,     0,   520,   250,  1437,     0,     0,     0,     0,     0,
      38,    39,    40,    41,    42,    43,     0,    45,     0,    47,
       0,    49,   522,    51,     0,    53,     0,    55,  1380,   523,
     524,     0,   525,   526,   527,     0,     0,   530,     0,   528,
       0,  1438,     0,     0,   531,   532,  1409,   533,   534,   535,
     466,     0,     0,     0,   536,     0,  1439,   467,   468,     0,
     469,   470,   471,     0,     0,     0,     0,   472,     0,  1442,
     474,     0,     0,   251,   252,   253,   254,   475,   476,     0,
     477,   478,   479,   482,     0,     0,     0,   480,     0,  1443,
     483,   484,     0,   485,   486,   487,     0,     0,     0,     0,
     488,     0,  1444,     0,     0,     0,     0,     0,     0,     0,
     255,     0,  1384,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,     0,     0,     0,  1394,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   120,
       0,     0,     0,   124,     0,   288,     0,   289,   290,   291,
       0,   292,   293,   294,   295,   296,   297,   298,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,     0,
    1381,     0,     0,   300,     0,  1158,  1159,     0,   250,     0,
       0,     0,     0,   301,   302,    38,    39,    40,    41,    42,
      43,     0,    45,     0,    47,     0,    49,   514,    51,     0,
      53,     0,    55,  1427,   515,   516,     0,   517,   518,   519,
     522,     0,     0,     0,   520,     0,  1447,   523,   524,     0,
     525,   526,   527,   530,     0,     0,     0,   528,     0,  1448,
     531,   532,     0,   533,   534,   535,     0,     0,     0,     0,
     536,     0,  1449,     0,     0,     0,     0,     0,   251,   252,
     253,   254,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,   522,     0,     0,     0,     0,  1165,
    1166,   523,   524,     0,   525,   526,   527,     0,     0,     0,
       0,   528,   658,     0,  1407,   255,     0,     0,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,     0,     0,
       0,     0,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   120,     0,     0,     0,   124,     0,
     288,     0,   289,   290,   291,     0,   292,   293,   294,   295,
     296,   297,   298,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,   299,     0,     0,   300,     0,
    1172,  1173,     0,   250,     0,     0,     0,     0,   301,   302,
      38,    39,    40,    41,    42,    43,     0,    45,     0,    47,
       0,    49,     0,    51,     0,    53,     0,    55,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
       0,     0,     0,     0,     0,  1179,  1180,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   444,   445,
       0,   446,   447,   448,   449,   653,     0,     0,   450,     0,
     594,     0,     0,   251,   252,   253,   254,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,   530,
       0,     0,     0,     0,     0,   915,   531,   532,     0,   533,
     534,   535,     0,     0,     0,     0,   536,   660,     0,     0,
     255,     0,     0,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,     0,     0,     0,     0,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   120,
       0,     0,     0,   124,     0,   288,     0,   289,   290,   291,
       0,   292,   293,   294,   295,   296,   297,   298,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
     299,     0,     0,   300,     0,     0,   921,     0,     0,     0,
       0,     0,     0,   301,   302,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,     0,     0,
       0,     0,     0,  1085,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
       0,     0,  1086,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,  1093,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,     0,     0,     0,     0,     0,     0,
    1094,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,  1126,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,     0,     0,     0,     0,     0,     0,     0,  1127,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,     0,     0,     0,     0,     0,     0,  1128,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
       0,     0,     0,     0,     0,     0,  1129,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,     0,     0,  1131,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,     0,     0,     0,
       0,     0,     0,     0,  1136,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,     0,     0,
       0,     0,     0,  1141,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
       0,     0,  1146,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,  1150,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,     0,     0,     0,     0,     0,     0,
    1151,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,  1152,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,     0,     0,     0,     0,     0,     0,     0,  1153,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,     0,     0,     0,     0,     0,     0,  1154,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
       0,     0,     0,     0,     0,     0,  1155,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,     0,     0,  1184,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,     0,     0,     0,
       0,     0,     0,     0,  1185,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,     0,     0,
       0,     0,     0,  1186,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
       0,     0,  1187,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,  1189,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,     0,     0,     0,     0,     0,     0,
    1191,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,  1193,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,     0,     0,     0,     0,     0,     0,     0,  1195,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,     0,     0,     0,     0,     0,     0,  1197,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
       0,     0,     0,     0,     0,     0,  1199,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,     0,     0,  1200,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,     0,     0,     0,
       0,     0,     0,     0,  1201,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,     0,     0,
       0,     0,     0,  1202,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
       0,     0,  1203,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,  1204,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,     0,     0,     0,     0,     0,     0,
    1205,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,  1206,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,     0,     0,     0,     0,     0,     0,     0,  1207,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,     0,     0,     0,     0,     0,     0,  1208,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
       0,     0,     0,     0,     0,     0,  1209,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,     0,     0,  1210,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,     0,     0,     0,
       0,     0,     0,     0,  1211,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,     0,     0,
       0,     0,     0,  1212,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
       0,     0,  1213,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,  1214,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,     0,     0,     0,     0,     0,     0,
    1215,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,  1216,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,     0,     0,     0,     0,     0,     0,     0,  1217,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,     0,     0,     0,     0,     0,     0,  1218,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
       0,     0,     0,     0,     0,     0,  1219,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,     0,     0,  1220,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,     0,     0,     0,
       0,     0,     0,     0,  1221,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,     0,     0,
       0,     0,     0,  1222,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
       0,     0,  1223,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,  1224,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,     0,     0,     0,     0,     0,     0,
    1225,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,  1226,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,     0,     0,     0,     0,     0,     0,     0,  1227,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,     0,     0,     0,     0,     0,     0,  1228,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
       0,     0,     0,     0,     0,     0,  1229,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,     0,     0,  1230,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,     0,     0,     0,
       0,     0,     0,     0,  1231,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,     0,     0,
       0,     0,     0,  1232,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
       0,     0,  1233,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,  1234,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,     0,     0,     0,     0,     0,     0,
    1235,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,  1236,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,     0,     0,     0,     0,     0,     0,     0,  1237,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,     0,     0,     0,     0,     0,     0,  1238,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
       0,     0,     0,     0,     0,     0,  1239,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,     0,     0,  1240,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,     0,     0,     0,
       0,     0,     0,     0,  1241,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,     0,     0,
       0,     0,     0,  1243,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
       0,     0,  1248,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,     0,     0,     0,     0,     0,     0,
       0,  1166,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,     0,     0,     0,     0,     0,     0,
    1180,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,  1426,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,     0,     0,     0,     0,     0,     0,     0,  1429,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
       0,     0,     0,     0,     0,     0,     0,  1431,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
       0,     0,     0,     0,     0,     0,  1436,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,     0,     0,
       0,     0,     0,     0,     0,  1441,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,     0,     0,     0,
       0,     0,     0,     0,  1446,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,     0,     0,     0,
       0,     0,     0,  1462,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
       0,     0,  1463,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,     0,     0,     0,     0,     0,     0,
       0,  1490,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,     0,     0,     0,     0,     0,     0,
    1491,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,     0,     0,     0,     0,     0,     0,  1502,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,     0,   466,     0,     0,     0,     0,     0,  1503,   467,
     468,     0,   469,   470,   471,   474,     0,     0,     0,   472,
    1160,     0,   475,   476,     0,   477,   478,   479,   482,     0,
       0,     0,   480,  1161,     0,   483,   484,     0,   485,   486,
     487,   514,     0,     0,     0,   488,  1162,     0,   515,   516,
       0,   517,   518,   519,   522,     0,     0,     0,   520,  1167,
       0,   523,   524,     0,   525,   526,   527,   530,     0,     0,
       0,   528,  1168,     0,   531,   532,     0,   533,   534,   535,
     466,     0,     0,     0,   536,  1169,     0,   467,   468,     0,
     469,   470,   471,   474,     0,     0,     0,   472,  1174,     0,
     475,   476,     0,   477,   478,   479,   482,     0,     0,     0,
     480,  1175,     0,   483,   484,     0,   485,   486,   487,   514,
       0,     0,     0,   488,  1176,     0,   515,   516,     0,   517,
     518,   519,   522,     0,     0,     0,   520,  1181,     0,   523,
     524,     0,   525,   526,   527,   530,     0,     0,     0,   528,
    1182,     0,   531,   532,     0,   533,   534,   535,     0,     0,
       0,     0,   536,  1183,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,     0,     0,     0,     0,     0,
       0,   591,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,     0,     0,     0,     0,     0,   652,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,     0,  1087,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
       0,     0,     0,     0,     0,  1088,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,     0,  1089,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
       0,  1090,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,     0,     0,     0,     0,     0,     0,  1091,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,     0,     0,     0,     0,     0,     0,  1092,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,     0,
       0,     0,     0,     0,     0,  1124,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,     0,     0,     0,
       0,     0,     0,  1125,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
       0,  1165,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,     0,     0,     0,     0,     0,  1179,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,     0,  1420,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
       0,     0,     0,     0,     0,  1421,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,     0,  1422,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
       0,  1423,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,     0,     0,     0,     0,     0,     0,  1424,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,     0,     0,     0,     0,     0,     0,  1425,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,     0,
       0,     0,     0,     0,     0,  1464,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,     0,     0,     0,
       0,     0,     0,  1465,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,     0,     0,     0,     0,     0,
       0,  1466,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,     0,     0,     0,     0,     0,  1467,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,     0,  1472,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
       0,     0,     0,     0,     0,  1473,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,     0,  1474,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,     0,     0,     0,     0,     0,
       0,  1475,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,     0,     0,     0,     0,     0,     0,  1480,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,     0,     0,     0,     0,     0,     0,  1481,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,     0,
       0,     0,     0,     0,     0,  1482,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,     0,     0,     0,
       0,     0,     0,  1483,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,     0,     0,     0,     0,     0,
       0,  1488,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,     0,     0,     0,     0,     0,     0,  1489,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,     0,     0,     0,     0,     0,     0,  1494,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,     0,
       0,     0,     0,     0,     0,  1495,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,     0,     0,     0,
       0,     0,     0,  1498,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,   474,     0,     0,     0,     0,
       0,  1499,   475,   476,     0,   477,   478,   479,   482,     0,
       0,     0,   480,     0,     0,   483,   484,     0,   485,   486,
     487,   514,     0,     0,     0,   488,     0,     0,   515,   516,
       0,   517,   518,   519,   522,     0,     0,     0,   520,     0,
       0,   523,   524,     0,   525,   526,   527,   530,     0,     0,
       0,   528,     0,     0,   531,   532,     0,   533,   534,   535,
       0,     0,  1339,     0,   536,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,  1369,     0,     0,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   512,
     494,   495,     0,   496,   497,   498,   499,     0,     0,     0,
     500,     0,   655,   444,   445,     0,   446,   447,   448,   449,
       0,     0,     0,   450,     0,   914,   494,   495,     0,   496,
     497,   498,   499,     0,     0,     0,   500,     0,   920,   444,
     445,     0,   446,   447,   448,   449,     0,     0,     0,   450,
       0,  1130,   494,   495,     0,   496,   497,   498,   499,     0,
       0,     0,   500,     0,  1135,   444,   445,     0,   446,   447,
     448,   449,     0,     0,     0,   450,     0,  1140,   494,   495,
       0,   496,   497,   498,   499,     0,     0,     0,   500,     0,
    1145,   444,   445,     0,   446,   447,   448,   449,     0,     0,
       0,   450,     0,  1188,   494,   495,     0,   496,   497,   498,
     499,     0,     0,     0,   500,     0,  1190,   444,   445,     0,
     446,   447,   448,   449,     0,     0,     0,   450,     0,  1192,
     494,   495,     0,   496,   497,   498,   499,     0,     0,     0,
     500,     0,  1194,   444,   445,     0,   446,   447,   448,   449,
       0,     0,     0,   450,     0,  1196,   494,   495,     0,   496,
     497,   498,   499,     0,     0,     0,   500,     0,  1198,   444,
     445,     0,   446,   447,   448,   449,     0,     0,     0,   450,
       0,  1242,   494,   495,     0,   496,   497,   498,   499,     0,
       0,     0,   500,     0,  1247,   494,   495,     0,   496,   497,
     498,   499,     0,     0,     0,   500,     0,  1164,   494,   495,
       0,   496,   497,   498,   499,     0,     0,     0,   500,     0,
    1178,   444,   445,     0,   446,   447,   448,   449,     0,     0,
       0,   450,     0,  1430,   494,   495,     0,   496,   497,   498,
     499,     0,     0,     0,   500,     0,  1435,   444,   445,     0,
     446,   447,   448,   449,     0,     0,     0,   450,     0,  1440,
     494,   495,     0,   496,   497,   498,   499,     0,     0,     0,
     500,     0,  1445,   718,   719,     0,   720,   721,   722,   723,
     724,     0,   725,   726,   727,   494,   495,     0,   496,   497,
     498,   499,     0,     0,     0,   500,   654,   494,   495,     0,
     496,   497,   498,   499,     0,     0,     0,   500,  1163,   494,
     495,     0,   496,   497,   498,   499,     0,     0,     0,   500,
    1177,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   677,   444,   445,     0,   446,   447,   448,
     449,     0,   494,   495,   450,   496,   497,   498,   499,     0,
     483,   484,   500,   485,   486,   487,     0,     0,   515,   516,
     488,   517,   518,   519,     0,     0,   523,   524,   520,   525,
     526,   527,     0,     0,   531,   532,   528,   533,   534,   535,
       0,     0,     0,     0,   536
  };

  /* YYCHECK.  */
  const short int
  PatchValueExpressionParser::yycheck_[] =
  {
         1,    30,    30,    30,    30,     1,   193,     1,    92,    92,
     184,   185,   186,    14,    15,    72,    73,   191,    12,    13,
     171,   192,   173,   174,   171,   194,   173,   174,    24,    25,
     150,     0,   152,   194,   154,   194,   156,   171,   158,   173,
     174,   194,   193,   193,   164,   192,   166,   480,   168,   194,
     170,   194,   172,   194,   171,   488,   173,   174,   178,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,   155,   155,   194,   193,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,   520,    83,    84,
      85,   194,    87,    88,   194,   528,    91,    83,    84,    85,
     194,    87,    88,   536,   194,    91,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,   171,   155,   173,   174,   191,   194,   140,
     171,   142,   173,   174,   194,   194,   142,   143,   142,   143,
     155,   171,   194,   173,   174,   193,   171,   194,   173,   174,
     155,   194,   193,   193,   193,   193,   193,   193,   174,   155,
      80,    81,    82,   193,   193,   181,   182,   192,   184,   185,
     186,   174,    80,    81,    82,   191,   194,   193,   181,   182,
     194,   184,   185,   186,   184,   185,   186,   187,   191,   174,
     193,   191,   194,     1,     2,     3,   181,   182,   193,   184,
     185,   186,   174,   183,   184,   185,   191,   194,   193,   181,
     182,   194,   184,   185,   186,   193,   184,   185,   186,   191,
     194,   193,   174,   191,   194,   194,   230,   231,   194,   181,
     182,   194,   184,   185,   186,   174,   183,   184,   185,   191,
     194,   193,   181,   182,   194,   184,   185,   186,   174,   194,
     194,   194,   191,   194,   193,   181,   182,   194,   184,   185,
     186,   174,   194,   194,   194,   191,   194,   193,   181,   182,
     194,   184,   185,   186,   174,   194,   194,   194,   191,   194,
     193,   181,   182,   194,   184,   185,   186,   194,   299,   194,
     301,   191,   174,   193,   193,   301,   302,   194,   194,   181,
     182,   194,   184,   185,   186,   172,   194,   194,   194,   191,
     194,   193,   193,   324,   181,   182,   194,   184,   185,   186,
     187,   194,   452,   194,   191,   194,   337,   338,   339,   340,
     174,   194,   140,   194,   142,   194,   194,   181,   182,   194,
     184,   185,   186,   194,   194,   194,   194,   191,   192,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   174,   397,   184,   185,   186,
     187,   194,   181,   182,   191,   184,   185,   186,   184,   185,
     186,   194,   191,   194,   193,   191,   174,   194,   174,   194,
       1,   194,   194,   181,   182,   194,   184,   185,   186,   194,
     228,   194,   230,   191,   174,   193,   193,    18,    19,   194,
     194,   181,   182,   194,   184,   185,   186,   174,   194,   194,
     194,   191,   194,   193,   181,   182,   194,   184,   185,   186,
     174,   194,   194,   194,   191,   194,   193,   181,   182,   194,
     184,   185,   186,   174,   194,   194,   194,   191,   194,   193,
     181,   182,   194,   184,   185,   186,   184,   185,   186,   194,
     191,   194,   193,   191,   194,   496,   497,   491,   492,   194,
     194,   502,   503,   504,   505,   506,   507,   508,   509,   510,
     511,   512,   194,   184,   185,   186,   517,   518,   181,   182,
     191,   184,   185,   186,   525,   526,   324,   194,   191,   184,
     185,   186,   533,   534,   194,   194,   191,   538,   172,   337,
     338,   339,   340,   539,   540,   194,   194,   181,   182,   194,
     184,   185,   186,   187,   194,   556,   557,   191,   194,   140,
     141,   142,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,   393,   394,   395,   396,   194,
     194,   194,   603,   604,   605,   606,   607,   194,   609,   610,
     611,   612,   613,   614,   615,   616,   617,   618,   619,   620,
     621,   622,   623,   624,   625,   626,   627,   628,   629,   630,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
     181,   182,   194,   184,   185,   186,   187,   194,   446,   447,
     191,   192,   193,   191,   194,   194,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   194,   194,   194,
     194,   469,   470,   194,   174,   194,   194,   194,   194,   477,
     478,   181,   182,   194,   184,   185,   186,   485,   486,   174,
     194,   191,   490,   193,   191,   194,   181,   182,   194,   184,
     185,   186,   194,   194,   174,   194,   191,   194,   193,   191,
     194,   181,   182,   194,   184,   185,   186,   174,   299,   300,
     301,   191,   194,   193,   181,   182,   194,   184,   185,   186,
     194,   194,   194,   194,   191,   194,   193,     5,     6,   194,
       8,     9,    10,   194,   542,   543,   544,   545,   546,   194,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,   573,   574,   575,   576,   577,
     578,   579,   363,   364,   174,     1,   194,   368,   369,   194,
     194,   181,   182,   194,   184,   185,   186,   194,   194,   194,
     174,   191,   194,   193,    20,    21,   194,   181,   182,   194,
     184,   185,   186,   194,   194,   194,   397,   191,   194,   193,
     401,   194,   403,   404,   405,   194,   407,   408,   409,   410,
     411,   412,   413,   194,   194,   194,     1,   174,    11,    12,
     194,    14,    15,    16,   181,   182,   194,   184,   185,   186,
     174,    16,    17,   194,   191,   194,   193,   181,   182,   194,
     184,   185,   186,   174,   194,   194,   194,   191,   194,   193,
     181,   182,   194,   184,   185,   186,   174,   194,   194,   194,
     191,   194,   193,   181,   182,   194,   184,   185,   186,   174,
     194,   194,   194,   191,   194,   193,   181,   182,   194,   184,
     185,   186,   174,   194,   194,   194,   191,   194,   193,   181,
     182,   194,   184,   185,   186,   193,   193,   498,   193,   191,
     193,   193,   193,   193,   140,   141,   142,   191,   191,   191,
     511,   191,   193,   514,   515,   516,   193,   193,   519,     5,
       6,   522,     8,     9,    10,   193,   527,   193,   174,   530,
     193,   193,   193,   193,   535,   181,   182,   538,   184,   185,
     186,   174,   193,   193,   193,   191,   193,   193,   181,   182,
     193,   184,   185,   186,   174,   140,   193,   142,   191,   193,
     193,   181,   182,   193,   184,   185,   186,   174,   193,   193,
       1,   191,   193,   193,   181,   182,   193,   184,   185,   186,
     193,   193,   193,   193,   191,   193,   193,   174,   193,   193,
     193,    22,    23,   193,   181,   182,   193,   184,   185,   186,
     174,   193,   193,   193,   191,   193,   193,   193,    -1,   193,
     193,   612,   613,   174,   194,   194,   617,   618,   194,   194,
     181,   182,   194,   184,   185,   186,   194,   193,   193,   193,
     191,   193,   193,     5,     6,   193,     8,     9,    10,   193,
     641,   642,   643,   644,   645,   646,   647,   648,   649,   650,
     651,   193,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   299,   300,   301,   500,  1088,     1,  1090,
     193,  1092,   174,    -1,    -1,    -1,    -1,    10,    11,   181,
     182,    -1,   184,   185,   186,    -1,   174,    -1,    -1,   191,
      -1,   193,    -1,   181,   182,    -1,   184,   185,   186,    -1,
      -1,    -1,    -1,   191,  1125,   193,    -1,    -1,    -1,   140,
      -1,   142,   174,    -1,   299,    -1,   301,    -1,    -1,   181,
     182,    -1,   184,   185,   186,    -1,    -1,   363,   364,   191,
     174,   193,   368,   369,    -1,    -1,    -1,   181,   182,    -1,
     184,   185,   186,    -1,  1165,   174,    -1,   191,    -1,   193,
      -1,    -1,   181,   182,    -1,   184,   185,   186,  1179,    -1,
      -1,   397,   191,    -1,   193,   401,    -1,   403,   404,    -1,
      -1,   407,   408,   409,   410,   411,   412,   413,   363,   364,
      -1,    -1,    -1,   368,   369,   174,    -1,   372,   373,   374,
      -1,    -1,   181,   182,    -1,   184,   185,   186,    -1,    -1,
      -1,    -1,   191,   174,   193,    -1,    -1,   140,    -1,   142,
     181,   182,   397,   184,   185,   186,   174,    -1,    -1,    -1,
     191,    -1,   193,   181,   182,    -1,   184,   185,   186,   174,
      -1,    -1,    -1,   191,    -1,   193,   181,   182,    -1,   184,
     185,   186,   174,    -1,    -1,    -1,   191,    -1,   193,   181,
     182,    -1,   184,   185,   186,    -1,    -1,    -1,    -1,   191,
      -1,   193,   498,    -1,    -1,    -1,    -1,    -1,   299,  1087,
     301,  1089,    -1,  1091,    -1,   511,     5,     6,   514,     8,
       9,    10,     1,   519,    -1,    -1,   522,   523,   524,     8,
       9,   527,    -1,    -1,   530,   228,    -1,   230,    -1,   535,
       5,     6,   538,     8,     9,    10,  1124,    -1,   174,   494,
     495,   496,    -1,   498,   499,   181,   182,    -1,   184,   185,
     186,    -1,    -1,    -1,    -1,   191,   511,   193,    -1,    -1,
      -1,    -1,   363,   364,   519,    -1,    -1,   368,   369,    -1,
    1158,    -1,   527,    -1,    -1,    -1,    -1,    -1,  1369,    -1,
     535,   174,    -1,   538,  1172,    -1,    -1,    -1,   181,   182,
    1381,   184,   185,   186,    -1,    -1,   397,    -1,   191,    -1,
     193,   556,   557,    -1,    -1,    -1,   612,   613,   409,   174,
      -1,   617,   618,    -1,    -1,    -1,   181,   182,    -1,   184,
     185,   186,    -1,    -1,    -1,    -1,   191,    -1,   193,    -1,
    1421,    -1,  1423,    -1,  1425,   641,   642,   643,    -1,   645,
     646,   647,   648,   649,   650,   651,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,    -1,    -1,    -1,   612,   613,    -1,
     363,   364,   617,   618,   619,   368,   369,   181,   182,    -1,
     184,   185,   186,   187,  1465,    -1,  1467,   191,   192,   193,
      -1,    -1,  1473,    -1,  1475,    -1,    -1,    -1,    -1,    -1,
    1481,    -1,  1483,   396,    -1,    -1,    -1,   498,  1489,   402,
      -1,    -1,    -1,   406,  1495,   408,   409,    -1,  1499,    -1,
     511,     5,     6,   514,     8,     9,    10,     1,   519,    -1,
      -1,   522,     6,     7,    -1,    -1,   527,    -1,    -1,   530,
     531,   532,    -1,    -1,   535,    -1,   174,   538,    -1,   228,
     229,   230,    -1,   181,   182,   448,   184,   185,   186,    -1,
      -1,  1339,    -1,   191,    -1,   193,    -1,    -1,    -1,    -1,
     463,    -1,    -1,   466,   467,   468,    -1,    -1,   471,    -1,
      -1,   474,   475,   476,    -1,    -1,   479,    -1,   174,   482,
     483,   484,    -1,    -1,   487,   181,   182,   490,   184,   185,
     186,   174,    -1,    -1,    -1,   191,  1167,   193,   181,   182,
      -1,   184,   185,   186,    -1,    -1,    -1,    -1,   191,   192,
    1181,   612,   613,    -1,   174,    -1,   617,   618,    -1,    -1,
      -1,   181,   182,    -1,   184,   185,   186,    -1,   172,    -1,
     174,   191,  1420,   193,  1422,    -1,  1424,   181,   182,    -1,
     184,   185,   186,    -1,    -1,    -1,   647,   191,   551,   552,
      -1,    -1,   174,   556,   557,    -1,   140,   141,   142,   181,
     182,    -1,   184,   185,   186,    -1,    -1,    -1,    -1,   191,
      -1,   193,    -1,    -1,   363,   364,  1464,    -1,  1466,   368,
     369,    -1,   585,   586,  1472,    -1,  1474,    -1,    -1,   172,
      -1,   174,  1480,    -1,  1482,    -1,    -1,    -1,   181,   182,
    1488,   184,   185,   186,    -1,    -1,  1494,   396,   191,    -1,
    1498,    -1,   401,   402,   403,   404,    -1,   406,   407,   408,
     409,   410,   411,   412,   413,    -1,    -1,    -1,    -1,    -1,
     174,    -1,    -1,    -1,    -1,    -1,    -1,   181,   182,    -1,
     184,   185,   186,   174,   228,   229,   230,   191,    -1,   193,
     181,   182,    -1,   184,   185,   186,    -1,    -1,    -1,   448,
     191,    -1,   193,    -1,    -1,    -1,    -1,     1,    -1,    -1,
       4,     5,    -1,    -1,   463,    -1,    -1,   466,   467,   468,
      -1,    -1,   471,    -1,    -1,   474,   475,   476,    -1,    -1,
     479,    -1,    -1,   482,   483,   484,   174,    -1,   487,  1370,
      -1,   490,    -1,   181,   182,    -1,   184,   185,   186,   174,
    1381,    -1,    -1,   191,    -1,   193,   181,   182,    -1,   184,
     185,   186,   174,    -1,    -1,    -1,   191,    -1,   193,   181,
     182,    -1,   184,   185,   186,   174,    -1,    -1,    -1,   191,
      -1,   193,   181,   182,    -1,   184,   185,   186,   174,    -1,
      -1,    -1,   191,    -1,   193,   181,   182,    -1,   184,   185,
     186,    -1,   551,   552,   174,   191,   192,   556,   557,    -1,
      -1,   181,   182,    -1,   184,   185,   186,    -1,    -1,   363,
     364,   191,    -1,   193,   368,   369,    -1,    -1,    -1,    -1,
      -1,   580,   581,   582,    -1,   584,   585,   586,   587,   588,
     589,   590,    -1,    -1,    -1,    -1,   140,    -1,   142,    -1,
      -1,    -1,   396,    -1,    -1,    -1,    -1,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     174,    -1,    -1,    -1,    -1,    -1,    -1,   181,   182,    -1,
     184,   185,   186,    -1,   174,    -1,    -1,   191,    -1,   193,
      -1,   181,   182,    -1,   184,   185,   186,    -1,   174,    -1,
      -1,   191,  1168,   193,   448,   181,   182,    -1,   184,   185,
     186,    -1,    -1,    -1,    -1,   191,  1182,   193,    -1,   463,
      -1,    -1,   466,   467,   468,    -1,    -1,   471,    -1,    -1,
     474,   475,   476,    -1,   228,   479,   230,    -1,   482,   483,
     484,    -1,   174,   487,    -1,    -1,   490,    -1,  1163,   181,
     182,    -1,   184,   185,   186,    -1,   174,    -1,    -1,   191,
      -1,   193,  1177,   181,   182,    -1,   184,   185,   186,   174,
      -1,    -1,    -1,   191,    -1,   193,   181,   182,    -1,   184,
     185,   186,   174,    -1,    -1,    -1,   191,    -1,   193,   181,
     182,    -1,   184,   185,   186,    -1,    -1,    -1,    -1,   191,
      -1,   193,    -1,    -1,    -1,    -1,    -1,   551,   552,   174,
      -1,    -1,   556,   557,    -1,    -1,   181,   182,    -1,   184,
     185,   186,    -1,    -1,    -1,    -1,   191,    -1,   193,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   580,   581,   582,   583,
     584,   585,   586,   587,   588,   589,   590,   174,    -1,    -1,
      -1,    -1,    -1,    -1,   181,   182,    -1,   184,   185,   186,
      -1,    -1,    -1,    -1,   191,    -1,   193,    -1,    -1,   363,
     364,    -1,    -1,    -1,   368,   369,   174,    -1,   372,   373,
     374,    -1,    -1,   181,   182,    -1,   184,   185,   186,    -1,
      -1,    -1,    -1,   191,    -1,   193,    -1,    -1,    -1,   174,
      -1,    -1,   396,    -1,    -1,  1371,   181,   182,  1169,   184,
     185,   186,    -1,   174,    -1,  1381,   191,    -1,   193,    -1,
     181,   182,  1183,   184,   185,   186,   174,    -1,    -1,    -1,
     191,    -1,   193,   181,   182,    -1,   184,   185,   186,    -1,
      -1,    -1,    -1,   191,    -1,   193,    -1,    -1,    -1,    -1,
     444,   445,   446,  1368,   448,   449,    -1,   181,   182,    -1,
     184,   185,   186,   174,    -1,    -1,  1381,   191,    -1,   463,
     181,   182,    -1,   184,   185,   186,    -1,   471,    -1,    -1,
     191,    -1,   193,    -1,    -1,   479,    -1,    -1,    -1,    -1,
     174,    -1,    -1,   487,    -1,    -1,   490,   181,   182,  1162,
     184,   185,   186,    -1,   174,    -1,    -1,   191,    -1,   193,
      -1,   181,   182,  1176,   184,   185,   186,   174,    -1,    -1,
      -1,   191,    -1,   193,   181,   182,    -1,   184,   185,   186,
     174,    -1,    -1,    -1,   191,    -1,   193,   181,   182,    -1,
     184,   185,   186,   174,    -1,    -1,    -1,   191,    -1,   193,
     181,   182,    -1,   184,   185,   186,    -1,   551,   552,   174,
     191,   192,   556,   557,   558,    -1,   181,   182,    -1,   184,
     185,   186,   174,    -1,    -1,    -1,   191,    -1,   193,   181,
     182,    -1,   184,   185,   186,   174,    -1,    -1,    -1,   191,
      -1,   193,   181,   182,    -1,   184,   185,   186,   174,    -1,
      -1,    -1,   191,    -1,   193,   181,   182,    -1,   184,   185,
     186,  1372,    -1,    -1,   174,   191,    -1,   193,    -1,    -1,
    1381,   181,   182,    -1,   184,   185,   186,    -1,    -1,    -1,
      -1,   191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      -1,   172,    -1,   174,    -1,    -1,    -1,    -1,    -1,  1342,
     181,   182,    -1,   184,   185,   186,   172,    -1,   174,    -1,
     191,    -1,    -1,    -1,    -1,   181,   182,    -1,   184,   185,
     186,    -1,    -1,    -1,    -1,   191,    -1,    -1,    -1,    -1,
      -1,    -1,  1161,    74,    75,    76,    77,    78,    79,   181,
     182,    -1,   184,   185,   186,   187,  1175,    -1,    -1,   191,
     192,   193,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
      -1,    -1,   172,    -1,   174,    -1,    -1,    -1,    -1,    -1,
     181,   181,   182,   184,   184,   185,   186,    -1,   174,    -1,
      -1,   191,    -1,   194,   195,   181,   182,    -1,   184,   185,
     186,    -1,    -1,    -1,    -1,   191,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    19,    -1,    21,   172,    23,   174,    25,    -1,
      27,    -1,    29,    -1,   181,   182,    -1,   184,   185,   186,
      -1,    -1,    -1,   174,   191,    -1,    -1,    -1,    -1,    -1,
     181,   182,  1341,   184,   185,   186,   174,    -1,    -1,    -1,
     191,    -1,   193,   181,   182,    -1,   184,   185,   186,    -1,
      -1,    -1,    -1,   191,    -1,   193,  1160,    74,    75,    76,
      77,    78,    79,   181,   182,    -1,   184,   185,   186,   187,
    1174,    -1,    -1,   191,   192,   193,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,    -1,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,    -1,   181,   182,    -1,   184,   185,
     186,   187,   174,    -1,   181,   191,   192,   184,    -1,   181,
     182,    -1,   184,   185,   186,   174,    -1,   194,   195,   191,
      -1,   193,   181,   182,    -1,   184,   185,   186,    -1,    -1,
      -1,    -1,   191,     4,   193,    -1,    -1,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    -1,    18,    -1,    20,
      -1,    22,   174,    24,    -1,    26,    -1,    28,    29,   181,
     182,    -1,   184,   185,   186,    -1,    -1,   174,    -1,   191,
      -1,   193,    -1,    -1,   181,   182,  1340,   184,   185,   186,
     174,    -1,    -1,    -1,   191,    -1,   193,   181,   182,    -1,
     184,   185,   186,    -1,    -1,    -1,    -1,   191,    -1,   193,
     174,    -1,    -1,    74,    75,    76,    77,   181,   182,    -1,
     184,   185,   186,   174,    -1,    -1,    -1,   191,    -1,   193,
     181,   182,    -1,   184,   185,   186,    -1,    -1,    -1,    -1,
     191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,  1156,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    -1,    -1,    -1,  1170,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
      -1,    -1,    -1,   154,    -1,   156,    -1,   158,   159,   160,
      -1,   162,   163,   164,   165,   166,   167,   168,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
     181,    -1,    -1,   184,    -1,   192,   193,    -1,     4,    -1,
      -1,    -1,    -1,   194,   195,    11,    12,    13,    14,    15,
      16,    -1,    18,    -1,    20,    -1,    22,   174,    24,    -1,
      26,    -1,    28,    29,   181,   182,    -1,   184,   185,   186,
     174,    -1,    -1,    -1,   191,    -1,   193,   181,   182,    -1,
     184,   185,   186,   174,    -1,    -1,    -1,   191,    -1,   193,
     181,   182,    -1,   184,   185,   186,    -1,    -1,    -1,    -1,
     191,    -1,   193,    -1,    -1,    -1,    -1,    -1,    74,    75,
      76,    77,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,   174,    -1,    -1,    -1,    -1,   192,
     193,   181,   182,    -1,   184,   185,   186,    -1,    -1,    -1,
      -1,   191,   192,    -1,  1338,   111,    -1,    -1,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
      -1,    -1,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,    -1,    -1,    -1,   154,    -1,
     156,    -1,   158,   159,   160,    -1,   162,   163,   164,   165,
     166,   167,   168,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,   181,    -1,    -1,   184,    -1,
     192,   193,    -1,     4,    -1,    -1,    -1,    -1,   194,   195,
      11,    12,    13,    14,    15,    16,    -1,    18,    -1,    20,
      -1,    22,    -1,    24,    -1,    26,    -1,    28,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   193,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   181,   182,
      -1,   184,   185,   186,   187,   193,    -1,    -1,   191,    -1,
     193,    -1,    -1,    74,    75,    76,    77,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,   174,
      -1,    -1,    -1,    -1,    -1,   193,   181,   182,    -1,   184,
     185,   186,    -1,    -1,    -1,    -1,   191,   192,    -1,    -1,
     111,    -1,    -1,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    -1,    -1,    -1,    -1,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
      -1,    -1,    -1,   154,    -1,   156,    -1,   158,   159,   160,
      -1,   162,   163,   164,   165,   166,   167,   168,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
     181,    -1,    -1,   184,    -1,    -1,   193,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   193,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   193,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   193,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     193,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,   174,    -1,    -1,    -1,    -1,    -1,   193,   181,
     182,    -1,   184,   185,   186,   174,    -1,    -1,    -1,   191,
     192,    -1,   181,   182,    -1,   184,   185,   186,   174,    -1,
      -1,    -1,   191,   192,    -1,   181,   182,    -1,   184,   185,
     186,   174,    -1,    -1,    -1,   191,   192,    -1,   181,   182,
      -1,   184,   185,   186,   174,    -1,    -1,    -1,   191,   192,
      -1,   181,   182,    -1,   184,   185,   186,   174,    -1,    -1,
      -1,   191,   192,    -1,   181,   182,    -1,   184,   185,   186,
     174,    -1,    -1,    -1,   191,   192,    -1,   181,   182,    -1,
     184,   185,   186,   174,    -1,    -1,    -1,   191,   192,    -1,
     181,   182,    -1,   184,   185,   186,   174,    -1,    -1,    -1,
     191,   192,    -1,   181,   182,    -1,   184,   185,   186,   174,
      -1,    -1,    -1,   191,   192,    -1,   181,   182,    -1,   184,
     185,   186,   174,    -1,    -1,    -1,   191,   192,    -1,   181,
     182,    -1,   184,   185,   186,   174,    -1,    -1,    -1,   191,
     192,    -1,   181,   182,    -1,   184,   185,   186,    -1,    -1,
      -1,    -1,   191,   192,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,   192,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,   192,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,   192,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,   192,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,   192,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,   192,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,   192,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,   192,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,   192,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,   192,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,   192,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,   192,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,   192,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,   192,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,   192,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,   192,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,   192,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,   192,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,   192,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,   192,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,   192,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,   192,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,   192,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   174,    -1,    -1,    -1,    -1,
      -1,   192,   181,   182,    -1,   184,   185,   186,   174,    -1,
      -1,    -1,   191,    -1,    -1,   181,   182,    -1,   184,   185,
     186,   174,    -1,    -1,    -1,   191,    -1,    -1,   181,   182,
      -1,   184,   185,   186,   174,    -1,    -1,    -1,   191,    -1,
      -1,   181,   182,    -1,   184,   185,   186,   174,    -1,    -1,
      -1,   191,    -1,    -1,   181,   182,    -1,   184,   185,   186,
      -1,    -1,   172,    -1,   191,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   172,    -1,    -1,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     181,   182,    -1,   184,   185,   186,   187,    -1,    -1,    -1,
     191,    -1,   193,   181,   182,    -1,   184,   185,   186,   187,
      -1,    -1,    -1,   191,    -1,   193,   181,   182,    -1,   184,
     185,   186,   187,    -1,    -1,    -1,   191,    -1,   193,   181,
     182,    -1,   184,   185,   186,   187,    -1,    -1,    -1,   191,
      -1,   193,   181,   182,    -1,   184,   185,   186,   187,    -1,
      -1,    -1,   191,    -1,   193,   181,   182,    -1,   184,   185,
     186,   187,    -1,    -1,    -1,   191,    -1,   193,   181,   182,
      -1,   184,   185,   186,   187,    -1,    -1,    -1,   191,    -1,
     193,   181,   182,    -1,   184,   185,   186,   187,    -1,    -1,
      -1,   191,    -1,   193,   181,   182,    -1,   184,   185,   186,
     187,    -1,    -1,    -1,   191,    -1,   193,   181,   182,    -1,
     184,   185,   186,   187,    -1,    -1,    -1,   191,    -1,   193,
     181,   182,    -1,   184,   185,   186,   187,    -1,    -1,    -1,
     191,    -1,   193,   181,   182,    -1,   184,   185,   186,   187,
      -1,    -1,    -1,   191,    -1,   193,   181,   182,    -1,   184,
     185,   186,   187,    -1,    -1,    -1,   191,    -1,   193,   181,
     182,    -1,   184,   185,   186,   187,    -1,    -1,    -1,   191,
      -1,   193,   181,   182,    -1,   184,   185,   186,   187,    -1,
      -1,    -1,   191,    -1,   193,   181,   182,    -1,   184,   185,
     186,   187,    -1,    -1,    -1,   191,    -1,   193,   181,   182,
      -1,   184,   185,   186,   187,    -1,    -1,    -1,   191,    -1,
     193,   181,   182,    -1,   184,   185,   186,   187,    -1,    -1,
      -1,   191,    -1,   193,   181,   182,    -1,   184,   185,   186,
     187,    -1,    -1,    -1,   191,    -1,   193,   181,   182,    -1,
     184,   185,   186,   187,    -1,    -1,    -1,   191,    -1,   193,
     181,   182,    -1,   184,   185,   186,   187,    -1,    -1,    -1,
     191,    -1,   193,     5,     6,    -1,     8,     9,    10,    11,
      12,    -1,    14,    15,    16,   181,   182,    -1,   184,   185,
     186,   187,    -1,    -1,    -1,   191,   192,   181,   182,    -1,
     184,   185,   186,   187,    -1,    -1,    -1,   191,   192,   181,
     182,    -1,   184,   185,   186,   187,    -1,    -1,    -1,   191,
     192,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   181,   182,    -1,   184,   185,   186,
     187,    -1,   181,   182,   191,   184,   185,   186,   187,    -1,
     181,   182,   191,   184,   185,   186,    -1,    -1,   181,   182,
     191,   184,   185,   186,    -1,    -1,   181,   182,   191,   184,
     185,   186,    -1,    -1,   181,   182,   191,   184,   185,   186,
      -1,    -1,    -1,    -1,   191
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  PatchValueExpressionParser::yystos_[] =
  {
         0,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,   197,   198,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    74,    75,    76,
      77,    78,    79,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     181,   184,   194,   195,   200,   204,   205,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,     4,
      74,    75,    76,    77,   111,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   156,   158,   159,
     160,   162,   163,   164,   165,   166,   167,   168,   181,   184,
     194,   195,   204,   209,   211,   213,   215,   217,   209,   204,
     209,   204,   211,   211,   213,   213,   215,   215,   217,   217,
       4,    74,    75,    76,    77,   111,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   156,   158,
     159,   160,   162,   163,   164,   165,   166,   167,   168,   181,
     184,   194,   195,   223,   225,   227,   229,   231,   237,   225,
     223,   225,   223,   227,   227,   229,   229,   231,   231,   237,
     237,   193,   192,     0,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   204,   209,   211,   213,
     215,   223,   225,   227,   229,   231,   211,   213,   227,   229,
     204,   209,   211,   213,   215,   217,   223,   225,   227,   229,
     231,   237,   217,   237,   181,   182,   184,   185,   186,   187,
     191,    72,    73,   199,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   199,   174,   181,   182,   184,
     185,   186,   191,   199,   174,   181,   182,   184,   185,   186,
     191,   199,   174,   181,   182,   184,   185,   186,   191,   199,
     171,   173,   174,   199,   181,   182,   184,   185,   186,   187,
     191,   199,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   199,   174,   181,   182,   184,   185,   186,
     191,   199,   174,   181,   182,   184,   185,   186,   191,   199,
     174,   181,   182,   184,   185,   186,   191,   199,   171,   173,
     174,   199,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   192,   193,   192,   193,   192,   193,   192,   193,   192,
     193,   192,   193,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   192,   193,   192,   193,   192,   193,   192,   193,
     192,   193,   192,   193,   209,   225,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   209,   225,
     209,   225,   209,   225,   209,   225,    30,   193,    30,   193,
     193,   193,    30,   193,    30,   193,   193,   193,   193,   193,
     193,   193,   193,     5,     6,     8,     9,    10,     5,     6,
       8,     9,    10,     5,     6,     8,     9,    10,     5,     6,
       8,     9,    10,    11,    12,    14,    15,    16,   193,   193,
     209,   225,   209,   225,   209,   225,   204,   209,   211,   213,
     215,   223,   225,   227,   229,   231,   204,   209,   211,   213,
     215,   223,   225,   227,   229,   231,   209,   225,   209,   225,
     209,   225,   204,   209,   211,   213,   215,   223,   225,   227,
     229,   231,   204,   209,   211,   213,   215,   223,   225,   227,
     229,   231,   209,   225,   209,   225,   204,   209,   223,   225,
     204,   209,   223,   225,   204,   209,   223,   225,   209,   225,
     209,   225,   209,   225,   209,   225,   209,   225,   209,   225,
     209,   225,   209,   225,   209,   225,   209,   225,   209,   225,
     209,   225,   209,   225,   209,   225,   209,   225,   209,   225,
     209,   225,   209,   225,   209,   225,   209,   225,   209,   225,
     204,   209,   211,   213,   215,   223,   225,   227,   229,   231,
       5,     6,     8,     9,    10,     5,     6,     8,     9,    10,
     193,   211,   213,   227,   229,   211,   213,   215,   211,   213,
     227,   229,   211,   213,   227,   229,   211,   227,   211,   213,
     215,   211,   213,   227,   229,   211,   213,   215,   227,   229,
     211,   213,   215,   227,   229,   231,   211,   213,   227,   229,
     211,   213,   227,   229,   211,   213,   227,   229,   211,   213,
     227,   229,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   204,   204,   204,   209,
     209,   204,   211,   213,   215,   204,   201,   199,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   204,   209,   211,
     213,   215,   209,   211,   213,   215,   211,   213,   215,   211,
     213,   215,   209,   209,   204,   211,   213,   215,   202,   211,
     213,   215,   211,   213,   215,   211,   213,   215,   209,   209,
     204,   211,   213,   215,   202,   211,   213,   215,   211,   213,
     215,   211,   213,   215,   209,   209,   204,   211,   213,   215,
     202,   204,   209,   211,   213,   215,   217,   217,   223,   223,
     223,   225,   225,   223,   227,   229,   231,   223,   201,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   223,   225,
     227,   229,   231,   225,   227,   229,   231,   227,   227,   225,
     225,   223,   227,   229,   231,   202,   227,   229,   231,   229,
     229,   225,   225,   223,   227,   229,   231,   202,   227,   229,
     231,   231,   231,   225,   225,   223,   227,   229,   231,   202,
     223,   225,   227,   229,   231,   237,   237,   223,   225,   223,
     225,   223,   225,   223,   225,   193,   193,   192,   192,   192,
     192,   192,   192,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   192,   192,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   192,   193,   192,   193,
     192,   192,   192,   192,   193,   192,   193,   192,   192,   192,
     192,   193,   192,   193,   192,   192,   192,   192,   193,   192,
     193,   192,   192,   192,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,    80,    81,    82,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,   155,    83,
      84,    85,    87,    88,    91,   155,    92,   155,   172,   172,
     172,   172,   172,    80,    81,    82,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,   155,    83,
      84,    85,    87,    88,    91,   155,    92,   155,   172,   172,
     172,   172,   172,   209,   225,   209,   225,   209,   225,   209,
      29,   181,   206,   225,   204,   209,   211,   213,   215,   223,
     225,   227,   229,   231,   204,   209,   211,   213,   215,   223,
     225,   227,   229,   231,   194,   194,   194,   204,   209,   211,
     213,   215,   194,   194,   194,   223,   225,   227,   229,   231,
     192,   192,   192,   192,   192,   192,   193,    29,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   209,   225,   209,   225,
     209,   225,   193,   193,   192,   192,   192,   192,   209,   225,
     209,   225,   192,   192,   192,   192,   209,   225,   209,   225,
     192,   192,   192,   192,   209,   225,   209,   225,   192,   192,
     193,   193,   209,   225,   192,   192,   209,   225,   192,   192,
     209,   225,   193,   193
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  PatchValueExpressionParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,    63,    58,   426,   427,   428,   429,    60,    62,   430,
     431,    45,    43,    37,    42,    47,    38,    94,   432,   433,
     434,    46,    44,    41,    40,    33
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  PatchValueExpressionParser::yyr1_[] =
  {
         0,   196,   197,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     199,   199,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   201,   202,   203,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   205,
     206,   206,   207,   207,   207,   207,   207,   207,   207,   207,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   210,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   212,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   214,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   216,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   218,
     219,   220,   221,   222,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   224,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     226,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     228,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   230,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     232,   233,   234,   235,   236,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   238
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  PatchValueExpressionParser::yyr2_[] =
  {
         0,     2,     1,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     0,     0,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     3,     4,     4,     4,     4,     4,
       4,     4,     5,     3,     3,     3,     3,     3,     4,     2,
       1,     4,     4,     4,     4,     4,     4,     6,     6,     3,
       1,     2,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     1,     1,     3,     3,     3,     3,     3,     6,
       4,     4,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     5,     1,     3,     3,     3,     3,
       3,     4,     3,     4,     3,     3,     4,     3,     4,     3,
       3,     4,     3,     2,     1,     4,     4,     4,     1,     4,
       4,     4,     4,     6,     6,     3,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     4,     4,
       4,     4,     4,     4,     4,     6,     5,     4,     2,     1,
       4,     4,     4,     4,     4,     4,     6,     6,     3,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     6,     5,     4,     2,     1,     4,     4,
       4,     4,     4,     4,     6,     6,     3,     1,     1,     3,
       3,     3,     3,     3,     3,     2,     3,     4,     4,     4,
       4,     6,     5,     4,     2,     1,     4,     4,     4,     4,
       4,     4,     6,     6,     3,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     1,     3,
       8,    20,    14,     4,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       4,     4,     4,     4,     4,     4,     4,     3,     5,     3,
       4,     2,     1,     4,     6,     6,     3,     3,     3,     3,
       3,     3,     6,     6,     4,     4,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     5,     4,     2,     1,     4,     4,     6,     6,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     3,     4,     4,     4,     4,     4,
       4,     4,     6,     5,     4,     2,     1,     4,     6,     6,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     3,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     6,     5,     4,     2,     1,     4,     6,     6,     3,
       1,     3,     3,     3,     3,     3,     3,     2,     3,     4,
       4,     4,     6,     5,     4,     2,     1,     4,     6,     6,
       3,     8,    20,    14,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     1,     3
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const PatchValueExpressionParser::yytname_[] =
  {
    "$end", "error", "$undefined", "\"timeline\"", "\"lookup\"",
  "\"scalarID\"", "\"vectorID\"", "\"logicalID\"", "\"tensorID\"",
  "\"symmTensorID\"", "\"sphericalTensorID\"", "\"pointScalarID\"",
  "\"pointVectorID\"", "\"pointLogicalID\"", "\"pointTensorID\"",
  "\"pointSymmTensorID\"", "\"pointSphericalTensorID\"", "\"F_scalarID\"",
  "\"F_pointScalarID\"", "\"F_vectorID\"", "\"F_pointVectorID\"",
  "\"F_tensorID\"", "\"F_pointTensorID\"", "\"F_symmTensorID\"",
  "\"F_pointSymmTensorID\"", "\"F_sphericalTensorID\"",
  "\"F_pointSphericalTensorID\"", "\"F_logicalID\"",
  "\"F_pointLogicalID\"", "\"value\"", "\"integer\"", "\"vector\"",
  "\"sexpression\"", "\"expression\"", "\"pexpression\"",
  "\"lexpression\"", "\"plexpression\"", "\"vexpression\"",
  "\"pvexpression\"", "\"texpression\"", "\"ptexpression\"",
  "\"yexpression\"", "\"pyexpression\"", "\"hexpression\"",
  "\"phexpression\"", "START_DEFAULT", "START_FACE_SCALAR_COMMA",
  "START_FACE_SCALAR_CLOSE", "START_FACE_VECTOR_COMMA",
  "START_FACE_VECTOR_CLOSE", "START_FACE_TENSOR_COMMA",
  "START_FACE_TENSOR_CLOSE", "START_FACE_YTENSOR_COMMA",
  "START_FACE_YTENSOR_CLOSE", "START_FACE_HTENSOR_COMMA",
  "START_FACE_HTENSOR_CLOSE", "START_FACE_LOGICAL_COMMA",
  "START_FACE_LOGICAL_CLOSE", "START_POINT_SCALAR_COMMA",
  "START_POINT_SCALAR_CLOSE", "START_POINT_VECTOR_COMMA",
  "START_POINT_VECTOR_CLOSE", "START_POINT_TENSOR_COMMA",
  "START_POINT_TENSOR_CLOSE", "START_POINT_YTENSOR_COMMA",
  "START_POINT_YTENSOR_CLOSE", "START_POINT_HTENSOR_COMMA",
  "START_POINT_HTENSOR_CLOSE", "START_POINT_LOGICAL_COMMA",
  "START_POINT_LOGICAL_CLOSE", "START_CLOSE_ONLY", "START_COMMA_ONLY",
  "TOKEN_LAST_FUNCTION_CHAR", "TOKEN_IN_FUNCTION_CHAR", "TOKEN_VECTOR",
  "TOKEN_TENSOR", "TOKEN_SYMM_TENSOR", "TOKEN_SPHERICAL_TENSOR",
  "TOKEN_TRUE", "TOKEN_FALSE", "TOKEN_x", "TOKEN_y", "TOKEN_z", "TOKEN_xx",
  "TOKEN_xy", "TOKEN_xz", "TOKEN_yx", "TOKEN_yy", "TOKEN_yz", "TOKEN_zx",
  "TOKEN_zy", "TOKEN_zz", "TOKEN_ii", "TOKEN_unitTensor", "TOKEN_pi",
  "TOKEN_rand", "TOKEN_randFixed", "TOKEN_id", "TOKEN_dist",
  "TOKEN_randNormal", "TOKEN_randNormalFixed", "TOKEN_position",
  "TOKEN_area", "TOKEN_Sf", "TOKEN_Cn", "TOKEN_delta", "TOKEN_weights",
  "TOKEN_normal", "TOKEN_snGrad", "TOKEN_internalField",
  "TOKEN_neighbourField", "TOKEN_oldTime", "TOKEN_deltaT", "TOKEN_time",
  "TOKEN_pow", "TOKEN_log", "TOKEN_exp", "TOKEN_mag", "TOKEN_magSqr",
  "TOKEN_sin", "TOKEN_cos", "TOKEN_tan", "TOKEN_min", "TOKEN_max",
  "TOKEN_minPosition", "TOKEN_maxPosition", "TOKEN_average", "TOKEN_sum",
  "TOKEN_sqr", "TOKEN_sqrt", "TOKEN_log10", "TOKEN_asin", "TOKEN_acos",
  "TOKEN_atan", "TOKEN_sinh", "TOKEN_cosh", "TOKEN_tanh", "TOKEN_asinh",
  "TOKEN_acosh", "TOKEN_atanh", "TOKEN_erf", "TOKEN_erfc", "TOKEN_lgamma",
  "TOKEN_besselJ0", "TOKEN_besselJ1", "TOKEN_besselY0", "TOKEN_besselY1",
  "TOKEN_sign", "TOKEN_pos", "TOKEN_neg", "TOKEN_toPoint", "TOKEN_toFace",
  "TOKEN_mapped", "TOKEN_mappedInternal", "TOKEN_points",
  "TOKEN_transpose", "TOKEN_diag", "TOKEN_tr", "TOKEN_dev", "TOKEN_symm",
  "TOKEN_skew", "TOKEN_det", "TOKEN_cof", "TOKEN_inv", "TOKEN_sph",
  "TOKEN_twoSymm", "TOKEN_dev2", "TOKEN_eigenValues", "TOKEN_eigenVectors",
  "TOKEN_cpu", "TOKEN_weight", "'?'", "':'", "TOKEN_OR", "TOKEN_AND",
  "TOKEN_NEQ", "TOKEN_EQ", "'<'", "'>'", "TOKEN_GEQ", "TOKEN_LEQ", "'-'",
  "'+'", "'%'", "'*'", "'/'", "'&'", "'^'", "TOKEN_HODGE", "TOKEN_NOT",
  "TOKEN_NEG", "'.'", "','", "')'", "'('", "'!'", "$accept",
  "switch_start", "switch_expr", "restOfFunction", "unit",
  "vectorComponentSwitch", "tensorComponentSwitch", "eatCharactersSwitch",
  "vexp", "evaluateVectorFunction", "scalar", "sreduced", "vreduced",
  "exp", "evaluateScalarFunction", "texp", "evaluateTensorFunction",
  "yexp", "evaluateSymmTensorFunction", "hexp",
  "evaluateSphericalTensorFunction", "lexp", "evaluateLogicalFunction",
  "vector", "tensor", "symmTensor", "sphericalTensor", "pvexp",
  "evaluatePointVectorFunction", "pexp", "evaluatePointScalarFunction",
  "ptexp", "evaluatePointTensorFunction", "pyexp",
  "evaluatePointSymmTensorFunction", "phexp",
  "evaluatePointSphericalTensorFunction", "pvector", "ptensor",
  "psymmTensor", "psphericalTensor", "plexp",
  "evaluatePointLogicalFunction", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const PatchValueExpressionParser::rhs_number_type
  PatchValueExpressionParser::yyrhs_[] =
  {
       197,     0,    -1,   198,    -1,    45,   200,    -1,    46,   209,
     192,    -1,    47,   209,   193,    -1,    58,   225,   192,    -1,
      59,   225,   193,    -1,    48,   204,   192,    -1,    49,   204,
     193,    -1,    60,   223,   192,    -1,    61,   223,   193,    -1,
      50,   211,   192,    -1,    51,   211,   193,    -1,    62,   227,
     192,    -1,    63,   227,   193,    -1,    52,   213,   192,    -1,
      53,   213,   193,    -1,    64,   229,   192,    -1,    65,   229,
     193,    -1,    54,   215,   192,    -1,    55,   215,   193,    -1,
      66,   231,   192,    -1,    67,   231,   193,    -1,    56,   217,
     192,    -1,    57,   217,   193,    -1,    68,   237,   192,    -1,
      69,   237,   193,    -1,    70,   193,    -1,    71,   192,    -1,
      72,    -1,    73,   199,    -1,   209,    -1,   204,    -1,   217,
      -1,   225,    -1,   223,    -1,   237,    -1,   211,    -1,   227,
      -1,   213,    -1,   229,    -1,   215,    -1,   231,    -1,    -1,
      -1,    -1,   219,    -1,   208,    -1,   204,   182,   204,    -1,
     209,   184,   204,    -1,   204,   184,   209,    -1,   211,   186,
     204,    -1,   204,   186,   211,    -1,   213,   186,   204,    -1,
     204,   186,   213,    -1,   215,   186,   204,    -1,   204,   186,
     215,    -1,   204,   185,   209,    -1,   204,   187,   204,    -1,
     204,   181,   204,    -1,   181,   204,    -1,   184,   211,    -1,
     184,   213,    -1,   194,   204,   193,    -1,   167,   194,   211,
     193,    -1,   167,   194,   213,   193,    -1,   211,   191,   202,
      80,    -1,   211,   191,   202,    81,    -1,   211,   191,   202,
      82,    -1,   156,   194,   211,   193,    -1,   156,   194,   213,
     193,    -1,   217,   171,   204,   172,   204,    -1,   101,   194,
     193,    -1,   107,   194,   193,    -1,   103,   194,   193,    -1,
     104,   194,   193,    -1,   105,   194,   193,    -1,   151,   194,
     223,   193,    -1,   205,   199,    -1,     6,    -1,   111,   194,
       6,   193,    -1,   152,   194,     6,   193,    -1,   153,   194,
       6,   193,    -1,   108,   194,     6,   193,    -1,   109,   194,
       6,   193,    -1,   110,   194,     6,   193,    -1,   122,   194,
     204,   192,   204,   193,    -1,   123,   194,   204,   192,   204,
     193,    -1,    19,   194,   203,    -1,    29,    -1,   181,    29,
      -1,   122,   194,   209,   193,    -1,   123,   194,   209,   193,
      -1,   122,   194,   225,   193,    -1,   123,   194,   225,   193,
      -1,   127,   194,   209,   193,    -1,   127,   194,   225,   193,
      -1,   126,   194,   209,   193,    -1,   126,   194,   225,   193,
      -1,   122,   194,   204,   193,    -1,   123,   194,   204,   193,
      -1,   122,   194,   223,   193,    -1,   123,   194,   223,   193,
      -1,   124,   194,   209,   193,    -1,   125,   194,   209,   193,
      -1,   124,   194,   225,   193,    -1,   125,   194,   225,   193,
      -1,   127,   194,   204,   193,    -1,   127,   194,   223,   193,
      -1,   126,   194,   204,   193,    -1,   126,   194,   223,   193,
      -1,    29,    -1,   207,    -1,   209,   182,   209,    -1,   209,
     181,   209,    -1,   209,   184,   209,    -1,   209,   183,   209,
      -1,   209,   185,   209,    -1,   114,   194,   209,   192,   209,
     193,    -1,   115,   194,   209,   193,    -1,   116,   194,   209,
     193,    -1,   204,   186,   204,    -1,   211,   174,   211,    -1,
     211,   174,   213,    -1,   211,   174,   215,    -1,   213,   174,
     211,    -1,   213,   174,   213,    -1,   213,   174,   215,    -1,
     215,   174,   211,    -1,   215,   174,   213,    -1,   215,   174,
     215,    -1,   181,   209,    -1,   194,   209,   193,    -1,   128,
     194,   209,   193,    -1,   129,   194,   209,   193,    -1,   119,
     194,   209,   193,    -1,   120,   194,   209,   193,    -1,   121,
     194,   209,   193,    -1,   130,   194,   209,   193,    -1,   131,
     194,   209,   193,    -1,   132,   194,   209,   193,    -1,   133,
     194,   209,   193,    -1,   134,   194,   209,   193,    -1,   135,
     194,   209,   193,    -1,   136,   194,   209,   193,    -1,   137,
     194,   209,   193,    -1,   138,   194,   209,   193,    -1,   139,
     194,   209,   193,    -1,   140,   194,   209,   193,    -1,   141,
     194,   209,   193,    -1,   142,   194,   209,   193,    -1,   143,
     194,   209,   193,    -1,   144,   194,   209,   193,    -1,   145,
     194,   209,   193,    -1,   146,   194,   209,   193,    -1,   147,
     194,   209,   193,    -1,   148,   194,   209,   193,    -1,   149,
     194,   209,   193,    -1,   117,   194,   209,   193,    -1,   117,
     194,   204,   193,    -1,   117,   194,   211,   193,    -1,   117,
     194,   213,   193,    -1,   117,   194,   215,   193,    -1,   118,
     194,   209,   193,    -1,   118,   194,   204,   193,    -1,   118,
     194,   211,   193,    -1,   118,   194,   213,   193,    -1,   118,
     194,   215,   193,    -1,   157,   194,   211,   193,    -1,   157,
     194,   213,   193,    -1,   157,   194,   215,   193,    -1,   161,
     194,   211,   193,    -1,   161,   194,   213,   193,    -1,   161,
     194,   215,   193,    -1,   204,   191,   201,    80,    -1,   204,
     191,   201,    81,    -1,   204,   191,   201,    82,    -1,   211,
     191,   202,    83,    -1,   211,   191,   202,    84,    -1,   211,
     191,   202,    85,    -1,   211,   191,   202,    86,    -1,   211,
     191,   202,    87,    -1,   211,   191,   202,    88,    -1,   211,
     191,   202,    89,    -1,   211,   191,   202,    90,    -1,   211,
     191,   202,    91,    -1,   213,   191,   202,    83,    -1,   213,
     191,   202,    84,    -1,   213,   191,   202,    85,    -1,   213,
     191,   202,    87,    -1,   213,   191,   202,    88,    -1,   213,
     191,   202,    91,    -1,   215,   191,   202,    92,    -1,   217,
     171,   209,   172,   209,    -1,    94,    -1,    97,   194,   193,
      -1,    98,   194,   193,    -1,   169,   194,   193,    -1,   170,
     194,   193,    -1,    95,   194,   193,    -1,    95,   194,    30,
     193,    -1,    96,   194,   193,    -1,    96,   194,    30,   193,
      -1,   106,   194,   193,    -1,    99,   194,   193,    -1,    99,
     194,    30,   193,    -1,   100,   194,   193,    -1,   100,   194,
      30,   193,    -1,   112,   194,   193,    -1,   113,   194,   193,
      -1,   151,   194,   225,   193,    -1,   102,   194,   193,    -1,
     210,   199,    -1,     5,    -1,   111,   194,     5,   193,    -1,
     152,   194,     5,   193,    -1,   153,   194,     5,   193,    -1,
       3,    -1,     4,   194,   209,   193,    -1,   108,   194,     5,
     193,    -1,   109,   194,     5,   193,    -1,   110,   194,     5,
     193,    -1,   122,   194,   209,   192,   209,   193,    -1,   123,
     194,   209,   192,   209,   193,    -1,    17,   194,   203,    -1,
     220,    -1,   211,   182,   211,    -1,   211,   182,   213,    -1,
     211,   182,   215,    -1,   213,   182,   211,    -1,   215,   182,
     211,    -1,   209,   184,   211,    -1,   211,   184,   209,    -1,
     204,   184,   204,    -1,   211,   186,   211,    -1,   213,   186,
     211,    -1,   211,   186,   213,    -1,   215,   186,   211,    -1,
     211,   186,   215,    -1,   211,   185,   209,    -1,   211,   181,
     211,    -1,   211,   181,   213,    -1,   211,   181,   215,    -1,
     213,   181,   211,    -1,   215,   181,   211,    -1,   181,   211,
      -1,   194,   211,   193,    -1,   160,   194,   211,   193,    -1,
     168,   194,   211,   193,    -1,   168,   194,   213,   193,    -1,
     163,   194,   211,   193,    -1,   162,   194,   211,   193,    -1,
     158,   194,   211,   193,    -1,   166,   194,   211,   193,    -1,
     211,   191,   202,   155,   194,   193,    -1,   217,   171,   211,
     172,   211,    -1,   151,   194,   227,   193,    -1,   212,   199,
      -1,     8,    -1,   111,   194,     8,   193,    -1,   152,   194,
       8,   193,    -1,   153,   194,     8,   193,    -1,   108,   194,
       8,   193,    -1,   109,   194,     8,   193,    -1,   110,   194,
       8,   193,    -1,   122,   194,   211,   192,   211,   193,    -1,
     123,   194,   211,   192,   211,   193,    -1,    21,   194,   203,
      -1,   221,    -1,   213,   182,   213,    -1,   213,   182,   215,
      -1,   215,   182,   213,    -1,   209,   184,   213,    -1,   213,
     184,   209,    -1,   213,   186,   213,    -1,   215,   186,   213,
      -1,   213,   186,   215,    -1,   213,   185,   209,    -1,   213,
     181,   213,    -1,   213,   181,   215,    -1,   215,   181,   213,
      -1,   181,   213,    -1,   194,   213,   193,    -1,   159,   194,
     213,   193,    -1,   159,   194,   211,   193,    -1,   165,   194,
     213,   193,    -1,   165,   194,   211,   193,    -1,   163,   194,
     213,   193,    -1,   162,   194,   213,   193,    -1,   158,   194,
     213,   193,    -1,   166,   194,   213,   193,    -1,   128,   194,
     204,   193,    -1,   213,   191,   202,   155,   194,   193,    -1,
     217,   171,   213,   172,   213,    -1,   151,   194,   229,   193,
      -1,   214,   199,    -1,     9,    -1,   111,   194,     9,   193,
      -1,   152,   194,     9,   193,    -1,   153,   194,     9,   193,
      -1,   108,   194,     9,   193,    -1,   109,   194,     9,   193,
      -1,   110,   194,     9,   193,    -1,   122,   194,   213,   192,
     213,   193,    -1,   123,   194,   213,   192,   213,   193,    -1,
      23,   194,   203,    -1,   222,    -1,    93,    -1,   215,   182,
     215,    -1,   209,   184,   215,    -1,   215,   184,   209,    -1,
     215,   186,   215,    -1,   215,   185,   209,    -1,   215,   181,
     215,    -1,   181,   215,    -1,   194,   215,   193,    -1,   164,
     194,   211,   193,    -1,   164,   194,   213,   193,    -1,   164,
     194,   215,   193,    -1,   163,   194,   215,   193,    -1,   215,
     191,   202,   155,   194,   193,    -1,   217,   171,   215,   172,
     215,    -1,   151,   194,   231,   193,    -1,   216,   199,    -1,
      10,    -1,   111,   194,    10,   193,    -1,   152,   194,    10,
     193,    -1,   153,   194,    10,   193,    -1,   108,   194,    10,
     193,    -1,   109,   194,    10,   193,    -1,   110,   194,    10,
     193,    -1,   122,   194,   215,   192,   215,   193,    -1,   123,
     194,   215,   192,   215,   193,    -1,    25,   194,   203,    -1,
      78,    -1,    79,    -1,   209,   177,   209,    -1,   209,   178,
     209,    -1,   209,   180,   209,    -1,   209,   179,   209,    -1,
     209,   176,   209,    -1,   209,   175,   209,    -1,   194,   217,
     193,    -1,   217,   174,   217,    -1,   217,   173,   217,    -1,
     195,   217,    -1,   218,   199,    -1,     7,    -1,    27,   194,
     203,    -1,    74,   194,   209,   192,   209,   192,   209,   193,
      -1,    75,   194,   209,   192,   209,   192,   209,   192,   209,
     192,   209,   192,   209,   192,   209,   192,   209,   192,   209,
     193,    -1,    76,   194,   209,   192,   209,   192,   209,   192,
     209,   192,   209,   192,   209,   193,    -1,    77,   194,   209,
     193,    -1,   233,    -1,   223,   182,   223,    -1,   225,   184,
     223,    -1,   223,   184,   225,    -1,   227,   186,   223,    -1,
     223,   186,   227,    -1,   229,   186,   223,    -1,   223,   186,
     229,    -1,   231,   186,   223,    -1,   223,   186,   231,    -1,
     223,   185,   225,    -1,   223,   187,   223,    -1,   223,   181,
     223,    -1,   181,   223,    -1,   184,   227,    -1,   184,   229,
      -1,   167,   194,   227,   193,    -1,   167,   194,   229,   193,
      -1,   227,   191,   202,    80,    -1,   227,   191,   202,    81,
      -1,   227,   191,   202,    82,    -1,   156,   194,   227,   193,
      -1,   156,   194,   229,   193,    -1,   194,   223,   193,    -1,
     237,   171,   223,   172,   223,    -1,   154,   194,   193,    -1,
     150,   194,   204,   193,    -1,   224,   199,    -1,    12,    -1,
     111,   194,    12,   193,    -1,   122,   194,   223,   192,   223,
     193,    -1,   123,   194,   223,   192,   223,   193,    -1,    20,
     194,   203,    -1,   225,   182,   225,    -1,   225,   181,   225,
      -1,   225,   184,   225,    -1,   225,   183,   225,    -1,   225,
     185,   225,    -1,   114,   194,   225,   192,   206,   193,    -1,
     114,   194,   225,   192,   225,   193,    -1,   115,   194,   225,
     193,    -1,   116,   194,   225,   193,    -1,   223,   186,   223,
      -1,   227,   174,   227,    -1,   227,   174,   229,    -1,   227,
     174,   231,    -1,   229,   174,   227,    -1,   229,   174,   229,
      -1,   229,   174,   231,    -1,   231,   174,   227,    -1,   231,
     174,   229,    -1,   231,   174,   231,    -1,   181,   225,    -1,
     194,   225,   193,    -1,   128,   194,   225,   193,    -1,   129,
     194,   225,   193,    -1,   119,   194,   225,   193,    -1,   120,
     194,   225,   193,    -1,   121,   194,   225,   193,    -1,   130,
     194,   225,   193,    -1,   131,   194,   225,   193,    -1,   132,
     194,   225,   193,    -1,   133,   194,   225,   193,    -1,   134,
     194,   225,   193,    -1,   135,   194,   225,   193,    -1,   136,
     194,   225,   193,    -1,   137,   194,   225,   193,    -1,   138,
     194,   225,   193,    -1,   139,   194,   225,   193,    -1,   140,
     194,   225,   193,    -1,   141,   194,   225,   193,    -1,   142,
     194,   225,   193,    -1,   143,   194,   225,   193,    -1,   144,
     194,   225,   193,    -1,   145,   194,   225,   193,    -1,   146,
     194,   225,   193,    -1,   147,   194,   225,   193,    -1,   148,
     194,   225,   193,    -1,   149,   194,   225,   193,    -1,   117,
     194,   225,   193,    -1,   117,   194,   223,   193,    -1,   117,
     194,   227,   193,    -1,   117,   194,   229,   193,    -1,   117,
     194,   231,   193,    -1,   118,   194,   225,   193,    -1,   118,
     194,   223,   193,    -1,   118,   194,   227,   193,    -1,   118,
     194,   229,   193,    -1,   118,   194,   231,   193,    -1,   223,
     191,   201,    80,    -1,   223,   191,   201,    81,    -1,   223,
     191,   201,    82,    -1,   227,   191,   202,    83,    -1,   227,
     191,   202,    84,    -1,   227,   191,   202,    85,    -1,   227,
     191,   202,    86,    -1,   227,   191,   202,    87,    -1,   227,
     191,   202,    88,    -1,   227,   191,   202,    89,    -1,   227,
     191,   202,    90,    -1,   227,   191,   202,    91,    -1,   229,
     191,   202,    83,    -1,   229,   191,   202,    84,    -1,   229,
     191,   202,    85,    -1,   229,   191,   202,    87,    -1,   229,
     191,   202,    88,    -1,   229,   191,   202,    91,    -1,   231,
     191,   202,    92,    -1,   237,   171,   225,   172,   225,    -1,
     150,   194,   209,   193,    -1,   226,   199,    -1,    11,    -1,
     111,   194,    11,   193,    -1,     4,   194,   225,   193,    -1,
     122,   194,   225,   192,   225,   193,    -1,   123,   194,   225,
     192,   225,   193,    -1,    18,   194,   203,    -1,   234,    -1,
     227,   182,   227,    -1,   225,   184,   227,    -1,   227,   184,
     225,    -1,   223,   184,   223,    -1,   227,   186,   227,    -1,
     229,   186,   227,    -1,   227,   186,   229,    -1,   231,   186,
     227,    -1,   227,   186,   231,    -1,   227,   185,   225,    -1,
     227,   181,   227,    -1,   181,   227,    -1,   194,   227,   193,
      -1,   160,   194,   227,   193,    -1,   168,   194,   227,   193,
      -1,   168,   194,   229,   193,    -1,   163,   194,   227,   193,
      -1,   162,   194,   227,   193,    -1,   158,   194,   227,   193,
      -1,   166,   194,   227,   193,    -1,   227,   191,   202,   155,
     194,   193,    -1,   237,   171,   227,   172,   227,    -1,   150,
     194,   211,   193,    -1,   228,   199,    -1,    14,    -1,   111,
     194,    14,   193,    -1,   122,   194,   227,   192,   227,   193,
      -1,   123,   194,   227,   192,   227,   193,    -1,    22,   194,
     203,    -1,   235,    -1,   229,   182,   229,    -1,   225,   184,
     229,    -1,   229,   184,   225,    -1,   229,   186,   229,    -1,
     231,   186,   229,    -1,   229,   186,   231,    -1,   229,   185,
     225,    -1,   229,   181,   229,    -1,   181,   229,    -1,   194,
     229,   193,    -1,   159,   194,   229,   193,    -1,   159,   194,
     227,   193,    -1,   165,   194,   229,   193,    -1,   165,   194,
     227,   193,    -1,   163,   194,   229,   193,    -1,   162,   194,
     229,   193,    -1,   158,   194,   229,   193,    -1,   166,   194,
     229,   193,    -1,   128,   194,   223,   193,    -1,   229,   191,
     202,   155,   194,   193,    -1,   237,   171,   229,   172,   229,
      -1,   150,   194,   213,   193,    -1,   230,   199,    -1,    15,
      -1,   111,   194,    15,   193,    -1,   122,   194,   229,   192,
     229,   193,    -1,   123,   194,   229,   192,   229,   193,    -1,
      24,   194,   203,    -1,   236,    -1,   231,   182,   231,    -1,
     225,   184,   231,    -1,   231,   184,   225,    -1,   231,   186,
     231,    -1,   231,   185,   225,    -1,   231,   181,   231,    -1,
     181,   231,    -1,   194,   231,   193,    -1,   164,   194,   227,
     193,    -1,   164,   194,   229,   193,    -1,   164,   194,   231,
     193,    -1,   231,   191,   202,   155,   194,   193,    -1,   237,
     171,   231,   172,   231,    -1,   150,   194,   215,   193,    -1,
     232,   199,    -1,    16,    -1,   111,   194,    16,   193,    -1,
     122,   194,   231,   192,   231,   193,    -1,   123,   194,   231,
     192,   231,   193,    -1,    26,   194,   203,    -1,    74,   194,
     225,   192,   225,   192,   225,   193,    -1,    75,   194,   225,
     192,   225,   192,   225,   192,   225,   192,   225,   192,   225,
     192,   225,   192,   225,   192,   225,   193,    -1,    76,   194,
     225,   192,   225,   192,   225,   192,   225,   192,   225,   192,
     225,   193,    -1,    77,   194,   225,   193,    -1,   225,   177,
     225,    -1,   225,   178,   225,    -1,   225,   180,   225,    -1,
     225,   179,   225,    -1,   225,   176,   225,    -1,   225,   175,
     225,    -1,   194,   237,   193,    -1,   237,   174,   237,    -1,
     237,   173,   237,    -1,   195,   237,    -1,   238,   199,    -1,
      13,    -1,    28,   194,   203,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  PatchValueExpressionParser::yyprhs_[] =
  {
         0,     0,     3,     5,     8,    12,    16,    20,    24,    28,
      32,    36,    40,    44,    48,    52,    56,    60,    64,    68,
      72,    76,    80,    84,    88,    92,    96,   100,   104,   107,
     110,   112,   115,   117,   119,   121,   123,   125,   127,   129,
     131,   133,   135,   137,   139,   140,   141,   142,   144,   146,
     150,   154,   158,   162,   166,   170,   174,   178,   182,   186,
     190,   194,   197,   200,   203,   207,   212,   217,   222,   227,
     232,   237,   242,   248,   252,   256,   260,   264,   268,   273,
     276,   278,   283,   288,   293,   298,   303,   308,   315,   322,
     326,   328,   331,   336,   341,   346,   351,   356,   361,   366,
     371,   376,   381,   386,   391,   396,   401,   406,   411,   416,
     421,   426,   431,   433,   435,   439,   443,   447,   451,   455,
     462,   467,   472,   476,   480,   484,   488,   492,   496,   500,
     504,   508,   512,   515,   519,   524,   529,   534,   539,   544,
     549,   554,   559,   564,   569,   574,   579,   584,   589,   594,
     599,   604,   609,   614,   619,   624,   629,   634,   639,   644,
     649,   654,   659,   664,   669,   674,   679,   684,   689,   694,
     699,   704,   709,   714,   719,   724,   729,   734,   739,   744,
     749,   754,   759,   764,   769,   774,   779,   784,   789,   794,
     799,   804,   809,   814,   819,   825,   827,   831,   835,   839,
     843,   847,   852,   856,   861,   865,   869,   874,   878,   883,
     887,   891,   896,   900,   903,   905,   910,   915,   920,   922,
     927,   932,   937,   942,   949,   956,   960,   962,   966,   970,
     974,   978,   982,   986,   990,   994,   998,  1002,  1006,  1010,
    1014,  1018,  1022,  1026,  1030,  1034,  1038,  1041,  1045,  1050,
    1055,  1060,  1065,  1070,  1075,  1080,  1087,  1093,  1098,  1101,
    1103,  1108,  1113,  1118,  1123,  1128,  1133,  1140,  1147,  1151,
    1153,  1157,  1161,  1165,  1169,  1173,  1177,  1181,  1185,  1189,
    1193,  1197,  1201,  1204,  1208,  1213,  1218,  1223,  1228,  1233,
    1238,  1243,  1248,  1253,  1260,  1266,  1271,  1274,  1276,  1281,
    1286,  1291,  1296,  1301,  1306,  1313,  1320,  1324,  1326,  1328,
    1332,  1336,  1340,  1344,  1348,  1352,  1355,  1359,  1364,  1369,
    1374,  1379,  1386,  1392,  1397,  1400,  1402,  1407,  1412,  1417,
    1422,  1427,  1432,  1439,  1446,  1450,  1452,  1454,  1458,  1462,
    1466,  1470,  1474,  1478,  1482,  1486,  1490,  1493,  1496,  1498,
    1502,  1511,  1532,  1547,  1552,  1554,  1558,  1562,  1566,  1570,
    1574,  1578,  1582,  1586,  1590,  1594,  1598,  1602,  1605,  1608,
    1611,  1616,  1621,  1626,  1631,  1636,  1641,  1646,  1650,  1656,
    1660,  1665,  1668,  1670,  1675,  1682,  1689,  1693,  1697,  1701,
    1705,  1709,  1713,  1720,  1727,  1732,  1737,  1741,  1745,  1749,
    1753,  1757,  1761,  1765,  1769,  1773,  1777,  1780,  1784,  1789,
    1794,  1799,  1804,  1809,  1814,  1819,  1824,  1829,  1834,  1839,
    1844,  1849,  1854,  1859,  1864,  1869,  1874,  1879,  1884,  1889,
    1894,  1899,  1904,  1909,  1914,  1919,  1924,  1929,  1934,  1939,
    1944,  1949,  1954,  1959,  1964,  1969,  1974,  1979,  1984,  1989,
    1994,  1999,  2004,  2009,  2014,  2019,  2024,  2029,  2034,  2039,
    2044,  2049,  2054,  2060,  2065,  2068,  2070,  2075,  2080,  2087,
    2094,  2098,  2100,  2104,  2108,  2112,  2116,  2120,  2124,  2128,
    2132,  2136,  2140,  2144,  2147,  2151,  2156,  2161,  2166,  2171,
    2176,  2181,  2186,  2193,  2199,  2204,  2207,  2209,  2214,  2221,
    2228,  2232,  2234,  2238,  2242,  2246,  2250,  2254,  2258,  2262,
    2266,  2269,  2273,  2278,  2283,  2288,  2293,  2298,  2303,  2308,
    2313,  2318,  2325,  2331,  2336,  2339,  2341,  2346,  2353,  2360,
    2364,  2366,  2370,  2374,  2378,  2382,  2386,  2390,  2393,  2397,
    2402,  2407,  2412,  2419,  2425,  2430,  2433,  2435,  2440,  2447,
    2454,  2458,  2467,  2488,  2503,  2508,  2512,  2516,  2520,  2524,
    2528,  2532,  2536,  2540,  2544,  2547,  2550,  2552
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  PatchValueExpressionParser::yyrline_[] =
  {
         0,   383,   383,   387,   388,   394,   400,   406,   412,   418,
     424,   430,   436,   442,   448,   454,   460,   466,   472,   478,
     484,   490,   496,   502,   508,   514,   520,   526,   532,   537,
     544,   545,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   559,   562,   567,   570,   573,   576,   577,   581,
     586,   591,   596,   601,   606,   611,   616,   621,   626,   631,
     636,   641,   645,   649,   653,   654,   658,   662,   670,   678,
     686,   695,   704,   711,   714,   717,   720,   723,   726,   730,
     731,   735,   739,   742,   745,   749,   753,   757,   761,   767,
     778,   779,   782,   785,   788,   791,   794,   797,   800,   803,
     808,   817,   826,   835,   844,   853,   862,   871,   880,   884,
     888,   892,   898,   899,   900,   905,   910,   915,   920,   930,
     935,   939,   943,   947,   952,   957,   962,   967,   972,   977,
     982,   987,   992,   996,   997,  1001,  1005,  1009,  1013,  1017,
    1021,  1025,  1029,  1033,  1037,  1041,  1045,  1049,  1053,  1057,
    1061,  1065,  1069,  1073,  1077,  1081,  1085,  1089,  1093,  1097,
    1101,  1105,  1109,  1113,  1117,  1121,  1125,  1129,  1133,  1137,
    1141,  1145,  1149,  1153,  1157,  1161,  1165,  1169,  1173,  1177,
    1181,  1185,  1189,  1193,  1197,  1201,  1205,  1209,  1213,  1217,
    1221,  1225,  1229,  1233,  1237,  1244,  1253,  1256,  1259,  1264,
    1267,  1270,  1273,  1276,  1279,  1282,  1285,  1288,  1291,  1294,
    1297,  1300,  1304,  1309,  1310,  1314,  1318,  1321,  1324,  1328,
    1332,  1336,  1340,  1344,  1348,  1354,  1367,  1368,  1373,  1378,
    1383,  1388,  1393,  1398,  1403,  1408,  1413,  1418,  1423,  1428,
    1433,  1438,  1443,  1448,  1453,  1458,  1463,  1467,  1468,  1472,
    1476,  1480,  1484,  1488,  1492,  1496,  1500,  1507,  1511,  1512,
    1516,  1520,  1523,  1526,  1530,  1534,  1538,  1542,  1548,  1560,
    1561,  1566,  1571,  1576,  1581,  1586,  1597,  1602,  1607,  1612,
    1617,  1622,  1627,  1631,  1632,  1636,  1640,  1644,  1648,  1652,
    1656,  1660,  1664,  1668,  1671,  1678,  1682,  1683,  1687,  1691,
    1694,  1697,  1701,  1705,  1709,  1713,  1719,  1731,  1732,  1735,
    1740,  1745,  1750,  1755,  1760,  1765,  1769,  1770,  1774,  1778,
    1782,  1786,  1789,  1796,  1800,  1801,  1805,  1809,  1812,  1816,
    1820,  1824,  1828,  1832,  1838,  1850,  1851,  1852,  1861,  1870,
    1879,  1888,  1897,  1906,  1907,  1916,  1925,  1929,  1930,  1936,
    1948,  1954,  1965,  1975,  1981,  1982,  1987,  1992,  1997,  2002,
    2007,  2012,  2017,  2022,  2027,  2032,  2037,  2042,  2046,  2050,
    2054,  2058,  2062,  2070,  2078,  2086,  2095,  2104,  2105,  2112,
    2115,  2119,  2120,  2124,  2131,  2135,  2141,  2153,  2158,  2163,
    2168,  2173,  2178,  2182,  2187,  2191,  2195,  2199,  2203,  2207,
    2211,  2215,  2219,  2223,  2227,  2231,  2235,  2239,  2240,  2244,
    2248,  2252,  2256,  2260,  2264,  2268,  2272,  2276,  2280,  2284,
    2288,  2292,  2296,  2300,  2304,  2308,  2312,  2316,  2320,  2324,
    2328,  2332,  2336,  2340,  2344,  2348,  2352,  2356,  2360,  2364,
    2368,  2372,  2376,  2380,  2384,  2388,  2392,  2396,  2400,  2404,
    2408,  2412,  2416,  2420,  2424,  2428,  2432,  2436,  2440,  2444,
    2448,  2452,  2456,  2463,  2467,  2468,  2472,  2479,  2483,  2487,
    2493,  2505,  2506,  2511,  2516,  2521,  2526,  2531,  2536,  2541,
    2546,  2551,  2556,  2561,  2565,  2566,  2570,  2574,  2578,  2582,
    2586,  2590,  2594,  2598,  2605,  2609,  2610,  2614,  2621,  2625,
    2631,  2643,  2644,  2649,  2654,  2659,  2670,  2675,  2680,  2685,
    2690,  2694,  2695,  2699,  2703,  2707,  2711,  2715,  2719,  2723,
    2727,  2731,  2734,  2741,  2745,  2746,  2750,  2756,  2760,  2766,
    2778,  2779,  2784,  2789,  2794,  2799,  2804,  2809,  2813,  2814,
    2818,  2822,  2826,  2829,  2836,  2840,  2841,  2845,  2852,  2856,
    2862,  2874,  2880,  2890,  2899,  2905,  2914,  2923,  2932,  2941,
    2950,  2959,  2960,  2969,  2977,  2981,  2982,  2988
  };

  // Print the state stack on the debug stream.
  void
  PatchValueExpressionParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  PatchValueExpressionParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  PatchValueExpressionParser::token_number_type
  PatchValueExpressionParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   195,     2,     2,     2,   183,   186,     2,
     194,   193,   184,   182,   192,   181,   191,   185,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   172,     2,
     177,     2,   178,   171,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   187,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   173,   174,   175,   176,
     179,   180,   188,   189,   190
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int PatchValueExpressionParser::yyeof_ = 0;
  const int PatchValueExpressionParser::yylast_ = 6294;
  const int PatchValueExpressionParser::yynnts_ = 43;
  const int PatchValueExpressionParser::yyempty_ = -2;
  const int PatchValueExpressionParser::yyfinal_ = 323;
  const int PatchValueExpressionParser::yyterror_ = 1;
  const int PatchValueExpressionParser::yyerrcode_ = 256;
  const int PatchValueExpressionParser::yyntokens_ = 196;

  const unsigned int PatchValueExpressionParser::yyuser_token_number_max_ = 434;
  const PatchValueExpressionParser::token_number_type PatchValueExpressionParser::yyundef_token_ = 2;


} // parserPatch

/* Line 1136 of lalr1.cc  */
#line 10540 "PatchValueExpressionParser.tab.cc"


/* Line 1138 of lalr1.cc  */
#line 3001 "../PatchValueExpressionParser.yy"


void parserPatch::PatchValueExpressionParser::error (
    const parserPatch::PatchValueExpressionParser::location_type& l,
    const std::string& m
)
{
     driver.error (l, m);
}

