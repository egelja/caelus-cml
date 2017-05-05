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
#define yylex   parserCloudlex

/* First part of user declarations.  */

/* Line 293 of lalr1.cc  */
#line 40 "../CloudValueExpressionParser.yy"

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



/* Line 293 of lalr1.cc  */
#line 62 "CloudValueExpressionParser.tab.cc"


#include "CloudValueExpressionParser.tab.hh"

/* User implementation prologue.  */

/* Line 299 of lalr1.cc  */
#line 96 "../CloudValueExpressionParser.yy"

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



/* Line 299 of lalr1.cc  */
#line 120 "CloudValueExpressionParser.tab.cc"

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


namespace parserCloud {

/* Line 382 of lalr1.cc  */
#line 206 "CloudValueExpressionParser.tab.cc"

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  CloudValueExpressionParser::yytnamerr_ (const char *yystr)
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
  CloudValueExpressionParser::CloudValueExpressionParser (void * scanner_yyarg, CloudValueExpressionDriver& driver_yyarg, int start_token_yyarg, int numberOfFunctionChars_yyarg)
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

  CloudValueExpressionParser::~CloudValueExpressionParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  CloudValueExpressionParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
        case 3: /* "\"timeline\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 284 "CloudValueExpressionParser.tab.cc"
	break;
      case 4: /* "\"lookup\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 293 "CloudValueExpressionParser.tab.cc"
	break;
      case 5: /* "\"scalarID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 302 "CloudValueExpressionParser.tab.cc"
	break;
      case 6: /* "\"vectorID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 311 "CloudValueExpressionParser.tab.cc"
	break;
      case 7: /* "\"logicalID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 320 "CloudValueExpressionParser.tab.cc"
	break;
      case 8: /* "\"tensorID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 329 "CloudValueExpressionParser.tab.cc"
	break;
      case 9: /* "\"symmTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 338 "CloudValueExpressionParser.tab.cc"
	break;
      case 10: /* "\"sphericalTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 347 "CloudValueExpressionParser.tab.cc"
	break;
      case 11: /* "\"G_scalarID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 356 "CloudValueExpressionParser.tab.cc"
	break;
      case 12: /* "\"G_vectorID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 365 "CloudValueExpressionParser.tab.cc"
	break;
      case 13: /* "\"G_tensorID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 374 "CloudValueExpressionParser.tab.cc"
	break;
      case 14: /* "\"G_symmTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 383 "CloudValueExpressionParser.tab.cc"
	break;
      case 15: /* "\"G_sphericalTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 392 "CloudValueExpressionParser.tab.cc"
	break;
      case 16: /* "\"F_scalarID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 401 "CloudValueExpressionParser.tab.cc"
	break;
      case 17: /* "\"F_vectorID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 410 "CloudValueExpressionParser.tab.cc"
	break;
      case 18: /* "\"F_tensorID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 419 "CloudValueExpressionParser.tab.cc"
	break;
      case 19: /* "\"F_symmTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 428 "CloudValueExpressionParser.tab.cc"
	break;
      case 20: /* "\"F_sphericalTensorID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 437 "CloudValueExpressionParser.tab.cc"
	break;
      case 21: /* "\"F_logicalID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 446 "CloudValueExpressionParser.tab.cc"
	break;
      case 22: /* "\"cellSetID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 455 "CloudValueExpressionParser.tab.cc"
	break;
      case 23: /* "\"cellZoneID\"" */

/* Line 449 of lalr1.cc  */
#line 321 "../CloudValueExpressionParser.yy"
	{ debug_stream () << *(yyvaluep->name); };

/* Line 449 of lalr1.cc  */
#line 464 "CloudValueExpressionParser.tab.cc"
	break;
      case 24: /* "\"value\"" */

/* Line 449 of lalr1.cc  */
#line 327 "../CloudValueExpressionParser.yy"
	{ debug_stream () << (yyvaluep->val); };

/* Line 449 of lalr1.cc  */
#line 473 "CloudValueExpressionParser.tab.cc"
	break;
      case 25: /* "\"integer\"" */

/* Line 449 of lalr1.cc  */
#line 327 "../CloudValueExpressionParser.yy"
	{ debug_stream () << (yyvaluep->integer); };

/* Line 449 of lalr1.cc  */
#line 482 "CloudValueExpressionParser.tab.cc"
	break;
      case 26: /* "\"vector\"" */

/* Line 449 of lalr1.cc  */
#line 322 "../CloudValueExpressionParser.yy"
	{
    CML::OStringStream buff;
    buff << *(yyvaluep->vec);
    debug_stream () << buff.str().c_str();
};

/* Line 449 of lalr1.cc  */
#line 495 "CloudValueExpressionParser.tab.cc"
	break;
      case 27: /* "\"expression\"" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->sfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 507 "CloudValueExpressionParser.tab.cc"
	break;
      case 28: /* "\"lexpression\"" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->lfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 519 "CloudValueExpressionParser.tab.cc"
	break;
      case 29: /* "\"vexpression\"" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->vfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 531 "CloudValueExpressionParser.tab.cc"
	break;
      case 30: /* "\"texpression\"" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->tfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 543 "CloudValueExpressionParser.tab.cc"
	break;
      case 31: /* "\"yexpression\"" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->yfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 555 "CloudValueExpressionParser.tab.cc"
	break;
      case 32: /* "\"hexpression\"" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->hfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 567 "CloudValueExpressionParser.tab.cc"
	break;
      case 167: /* "vexp" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->vfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 579 "CloudValueExpressionParser.tab.cc"
	break;
      case 168: /* "evaluateVectorFunction" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->vfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 591 "CloudValueExpressionParser.tab.cc"
	break;
      case 169: /* "sreduced" */

/* Line 449 of lalr1.cc  */
#line 327 "../CloudValueExpressionParser.yy"
	{ debug_stream () << (yyvaluep->val); };

/* Line 449 of lalr1.cc  */
#line 600 "CloudValueExpressionParser.tab.cc"
	break;
      case 170: /* "vreduced" */

/* Line 449 of lalr1.cc  */
#line 322 "../CloudValueExpressionParser.yy"
	{
    CML::OStringStream buff;
    buff << *(yyvaluep->vec);
    debug_stream () << buff.str().c_str();
};

/* Line 449 of lalr1.cc  */
#line 613 "CloudValueExpressionParser.tab.cc"
	break;
      case 171: /* "exp" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->sfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 625 "CloudValueExpressionParser.tab.cc"
	break;
      case 172: /* "evaluateScalarFunction" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->sfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 637 "CloudValueExpressionParser.tab.cc"
	break;
      case 173: /* "texp" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->tfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 649 "CloudValueExpressionParser.tab.cc"
	break;
      case 174: /* "evaluateTensorFunction" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->tfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 661 "CloudValueExpressionParser.tab.cc"
	break;
      case 175: /* "yexp" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->yfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 673 "CloudValueExpressionParser.tab.cc"
	break;
      case 176: /* "evaluateSymmTensorFunction" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->yfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 685 "CloudValueExpressionParser.tab.cc"
	break;
      case 177: /* "hexp" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->hfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 697 "CloudValueExpressionParser.tab.cc"
	break;
      case 178: /* "evaluateSphericalTensorFunction" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->hfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 709 "CloudValueExpressionParser.tab.cc"
	break;
      case 179: /* "lexp" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->lfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 721 "CloudValueExpressionParser.tab.cc"
	break;
      case 180: /* "evaluateLogicalFunction" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->lfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 733 "CloudValueExpressionParser.tab.cc"
	break;
      case 181: /* "vector" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->vfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 745 "CloudValueExpressionParser.tab.cc"
	break;
      case 182: /* "tensor" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->tfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 757 "CloudValueExpressionParser.tab.cc"
	break;
      case 183: /* "symmTensor" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->yfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 769 "CloudValueExpressionParser.tab.cc"
	break;
      case 184: /* "sphericalTensor" */

/* Line 449 of lalr1.cc  */
#line 328 "../CloudValueExpressionParser.yy"
	{
    debug_stream () << "<No name field>" << (yyvaluep->hfield);
        //        << CML::pTraits<(*$$)::cmptType>::typeName << ">";
};

/* Line 449 of lalr1.cc  */
#line 781 "CloudValueExpressionParser.tab.cc"
	break;
       default:
	  break;
      }
  }


  void
  CloudValueExpressionParser::yy_symbol_print_ (int yytype,
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
  CloudValueExpressionParser::yydestruct_ (const char* yymsg,
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
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 820 "CloudValueExpressionParser.tab.cc"
	break;
      case 4: /* "\"lookup\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 829 "CloudValueExpressionParser.tab.cc"
	break;
      case 5: /* "\"scalarID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 838 "CloudValueExpressionParser.tab.cc"
	break;
      case 6: /* "\"vectorID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 847 "CloudValueExpressionParser.tab.cc"
	break;
      case 7: /* "\"logicalID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 856 "CloudValueExpressionParser.tab.cc"
	break;
      case 8: /* "\"tensorID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 865 "CloudValueExpressionParser.tab.cc"
	break;
      case 9: /* "\"symmTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 874 "CloudValueExpressionParser.tab.cc"
	break;
      case 10: /* "\"sphericalTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 883 "CloudValueExpressionParser.tab.cc"
	break;
      case 11: /* "\"G_scalarID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 892 "CloudValueExpressionParser.tab.cc"
	break;
      case 12: /* "\"G_vectorID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 901 "CloudValueExpressionParser.tab.cc"
	break;
      case 13: /* "\"G_tensorID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 910 "CloudValueExpressionParser.tab.cc"
	break;
      case 14: /* "\"G_symmTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 919 "CloudValueExpressionParser.tab.cc"
	break;
      case 15: /* "\"G_sphericalTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 928 "CloudValueExpressionParser.tab.cc"
	break;
      case 16: /* "\"F_scalarID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 937 "CloudValueExpressionParser.tab.cc"
	break;
      case 17: /* "\"F_vectorID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 946 "CloudValueExpressionParser.tab.cc"
	break;
      case 18: /* "\"F_tensorID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 955 "CloudValueExpressionParser.tab.cc"
	break;
      case 19: /* "\"F_symmTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 964 "CloudValueExpressionParser.tab.cc"
	break;
      case 20: /* "\"F_sphericalTensorID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 973 "CloudValueExpressionParser.tab.cc"
	break;
      case 21: /* "\"F_logicalID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 982 "CloudValueExpressionParser.tab.cc"
	break;
      case 22: /* "\"cellSetID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 991 "CloudValueExpressionParser.tab.cc"
	break;
      case 23: /* "\"cellZoneID\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->name); };

/* Line 480 of lalr1.cc  */
#line 1000 "CloudValueExpressionParser.tab.cc"
	break;
      case 24: /* "\"value\"" */

/* Line 480 of lalr1.cc  */
#line 319 "../CloudValueExpressionParser.yy"
	{};

/* Line 480 of lalr1.cc  */
#line 1009 "CloudValueExpressionParser.tab.cc"
	break;
      case 25: /* "\"integer\"" */

/* Line 480 of lalr1.cc  */
#line 319 "../CloudValueExpressionParser.yy"
	{};

/* Line 480 of lalr1.cc  */
#line 1018 "CloudValueExpressionParser.tab.cc"
	break;
      case 26: /* "\"vector\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->vec); };

/* Line 480 of lalr1.cc  */
#line 1027 "CloudValueExpressionParser.tab.cc"
	break;
      case 27: /* "\"expression\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->sfield); };

/* Line 480 of lalr1.cc  */
#line 1036 "CloudValueExpressionParser.tab.cc"
	break;
      case 28: /* "\"lexpression\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->lfield); };

/* Line 480 of lalr1.cc  */
#line 1045 "CloudValueExpressionParser.tab.cc"
	break;
      case 29: /* "\"vexpression\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->vfield); };

/* Line 480 of lalr1.cc  */
#line 1054 "CloudValueExpressionParser.tab.cc"
	break;
      case 30: /* "\"texpression\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->tfield); };

/* Line 480 of lalr1.cc  */
#line 1063 "CloudValueExpressionParser.tab.cc"
	break;
      case 31: /* "\"yexpression\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->yfield); };

/* Line 480 of lalr1.cc  */
#line 1072 "CloudValueExpressionParser.tab.cc"
	break;
      case 32: /* "\"hexpression\"" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->hfield); };

/* Line 480 of lalr1.cc  */
#line 1081 "CloudValueExpressionParser.tab.cc"
	break;
      case 167: /* "vexp" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->vfield); };

/* Line 480 of lalr1.cc  */
#line 1090 "CloudValueExpressionParser.tab.cc"
	break;
      case 168: /* "evaluateVectorFunction" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->vfield); };

/* Line 480 of lalr1.cc  */
#line 1099 "CloudValueExpressionParser.tab.cc"
	break;
      case 169: /* "sreduced" */

/* Line 480 of lalr1.cc  */
#line 319 "../CloudValueExpressionParser.yy"
	{};

/* Line 480 of lalr1.cc  */
#line 1108 "CloudValueExpressionParser.tab.cc"
	break;
      case 170: /* "vreduced" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->vec); };

/* Line 480 of lalr1.cc  */
#line 1117 "CloudValueExpressionParser.tab.cc"
	break;
      case 171: /* "exp" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->sfield); };

/* Line 480 of lalr1.cc  */
#line 1126 "CloudValueExpressionParser.tab.cc"
	break;
      case 172: /* "evaluateScalarFunction" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->sfield); };

/* Line 480 of lalr1.cc  */
#line 1135 "CloudValueExpressionParser.tab.cc"
	break;
      case 173: /* "texp" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->tfield); };

/* Line 480 of lalr1.cc  */
#line 1144 "CloudValueExpressionParser.tab.cc"
	break;
      case 174: /* "evaluateTensorFunction" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->tfield); };

/* Line 480 of lalr1.cc  */
#line 1153 "CloudValueExpressionParser.tab.cc"
	break;
      case 175: /* "yexp" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->yfield); };

/* Line 480 of lalr1.cc  */
#line 1162 "CloudValueExpressionParser.tab.cc"
	break;
      case 176: /* "evaluateSymmTensorFunction" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->yfield); };

/* Line 480 of lalr1.cc  */
#line 1171 "CloudValueExpressionParser.tab.cc"
	break;
      case 177: /* "hexp" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->hfield); };

/* Line 480 of lalr1.cc  */
#line 1180 "CloudValueExpressionParser.tab.cc"
	break;
      case 178: /* "evaluateSphericalTensorFunction" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->hfield); };

/* Line 480 of lalr1.cc  */
#line 1189 "CloudValueExpressionParser.tab.cc"
	break;
      case 179: /* "lexp" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->lfield); };

/* Line 480 of lalr1.cc  */
#line 1198 "CloudValueExpressionParser.tab.cc"
	break;
      case 180: /* "evaluateLogicalFunction" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->lfield); };

/* Line 480 of lalr1.cc  */
#line 1207 "CloudValueExpressionParser.tab.cc"
	break;
      case 181: /* "vector" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->vfield); };

/* Line 480 of lalr1.cc  */
#line 1216 "CloudValueExpressionParser.tab.cc"
	break;
      case 182: /* "tensor" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->tfield); };

/* Line 480 of lalr1.cc  */
#line 1225 "CloudValueExpressionParser.tab.cc"
	break;
      case 183: /* "symmTensor" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->yfield); };

/* Line 480 of lalr1.cc  */
#line 1234 "CloudValueExpressionParser.tab.cc"
	break;
      case 184: /* "sphericalTensor" */

/* Line 480 of lalr1.cc  */
#line 317 "../CloudValueExpressionParser.yy"
	{ delete (yyvaluep->hfield); };

/* Line 480 of lalr1.cc  */
#line 1243 "CloudValueExpressionParser.tab.cc"
	break;

	default:
	  break;
      }
  }

  void
  CloudValueExpressionParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  CloudValueExpressionParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  CloudValueExpressionParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  CloudValueExpressionParser::debug_level_type
  CloudValueExpressionParser::debug_level () const
  {
    return yydebug_;
  }

  void
  CloudValueExpressionParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  CloudValueExpressionParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  CloudValueExpressionParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  CloudValueExpressionParser::parse ()
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
#line 73 "../CloudValueExpressionParser.yy"
{
	     // Initialize the initial location.
	     //     @$.begin.filename = @$.end.filename = &driver.file;
    numberOfFunctionChars=0;
}

/* Line 565 of lalr1.cc  */
#line 1342 "CloudValueExpressionParser.tab.cc"

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
#line 338 "../CloudValueExpressionParser.yy"
    { driver.parserLastPos()=(yylocation_stack_[(1) - (1)]).end.column; }
    break;

  case 4:

/* Line 690 of lalr1.cc  */
#line 343 "../CloudValueExpressionParser.yy"
    {
                      driver.setResult<CML::scalar>((yysemantic_stack_[(3) - (2)].sfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 5:

/* Line 690 of lalr1.cc  */
#line 349 "../CloudValueExpressionParser.yy"
    {
                      driver.setResult<CML::scalar>((yysemantic_stack_[(3) - (2)].sfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 6:

/* Line 690 of lalr1.cc  */
#line 355 "../CloudValueExpressionParser.yy"
    {
                      driver.setResult<CML::vector>((yysemantic_stack_[(3) - (2)].vfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 7:

/* Line 690 of lalr1.cc  */
#line 361 "../CloudValueExpressionParser.yy"
    {
                      driver.setResult<CML::vector>((yysemantic_stack_[(3) - (2)].vfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 8:

/* Line 690 of lalr1.cc  */
#line 367 "../CloudValueExpressionParser.yy"
    {
                      driver.setResult<CML::tensor>((yysemantic_stack_[(3) - (2)].tfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 9:

/* Line 690 of lalr1.cc  */
#line 373 "../CloudValueExpressionParser.yy"
    {
                      driver.setResult<CML::tensor>((yysemantic_stack_[(3) - (2)].tfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 10:

/* Line 690 of lalr1.cc  */
#line 379 "../CloudValueExpressionParser.yy"
    {
                      driver.setResult<CML::symmTensor>((yysemantic_stack_[(3) - (2)].yfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 11:

/* Line 690 of lalr1.cc  */
#line 385 "../CloudValueExpressionParser.yy"
    {
                      driver.setResult<CML::symmTensor>((yysemantic_stack_[(3) - (2)].yfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 12:

/* Line 690 of lalr1.cc  */
#line 391 "../CloudValueExpressionParser.yy"
    {
                      driver.setResult<CML::sphericalTensor>((yysemantic_stack_[(3) - (2)].hfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 13:

/* Line 690 of lalr1.cc  */
#line 397 "../CloudValueExpressionParser.yy"
    {
                      driver.setResult<CML::sphericalTensor>((yysemantic_stack_[(3) - (2)].hfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 14:

/* Line 690 of lalr1.cc  */
#line 403 "../CloudValueExpressionParser.yy"
    {
                      driver.setResult<bool>((yysemantic_stack_[(3) - (2)].lfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 15:

/* Line 690 of lalr1.cc  */
#line 409 "../CloudValueExpressionParser.yy"
    {
                      driver.setResult<bool>((yysemantic_stack_[(3) - (2)].lfield));
                      driver.parserLastPos()=(yylocation_stack_[(3) - (3)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 16:

/* Line 690 of lalr1.cc  */
#line 415 "../CloudValueExpressionParser.yy"
    {
                      driver.parserLastPos()=(yylocation_stack_[(2) - (2)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 17:

/* Line 690 of lalr1.cc  */
#line 420 "../CloudValueExpressionParser.yy"
    {
                      driver.parserLastPos()=(yylocation_stack_[(2) - (2)]).end.column-1;
                      YYACCEPT;
                  }
    break;

  case 20:

/* Line 690 of lalr1.cc  */
#line 429 "../CloudValueExpressionParser.yy"
    { driver.setResult<CML::scalar>((yysemantic_stack_[(1) - (1)].sfield));  }
    break;

  case 21:

/* Line 690 of lalr1.cc  */
#line 430 "../CloudValueExpressionParser.yy"
    { driver.setResult<CML::vector>((yysemantic_stack_[(1) - (1)].vfield));  }
    break;

  case 22:

/* Line 690 of lalr1.cc  */
#line 431 "../CloudValueExpressionParser.yy"
    { driver.setResult<bool>((yysemantic_stack_[(1) - (1)].lfield)); }
    break;

  case 23:

/* Line 690 of lalr1.cc  */
#line 432 "../CloudValueExpressionParser.yy"
    { driver.setResult<CML::tensor>((yysemantic_stack_[(1) - (1)].tfield));  }
    break;

  case 24:

/* Line 690 of lalr1.cc  */
#line 433 "../CloudValueExpressionParser.yy"
    { driver.setResult<CML::symmTensor>((yysemantic_stack_[(1) - (1)].yfield));  }
    break;

  case 25:

/* Line 690 of lalr1.cc  */
#line 434 "../CloudValueExpressionParser.yy"
    {
            driver.setResult<CML::sphericalTensor>((yysemantic_stack_[(1) - (1)].hfield));
          }
    break;

  case 26:

/* Line 690 of lalr1.cc  */
#line 439 "../CloudValueExpressionParser.yy"
    { driver.startVectorComponent(); }
    break;

  case 27:

/* Line 690 of lalr1.cc  */
#line 442 "../CloudValueExpressionParser.yy"
    { driver.startTensorComponent(); }
    break;

  case 28:

/* Line 690 of lalr1.cc  */
#line 445 "../CloudValueExpressionParser.yy"
    { driver.startEatCharacters(); }
    break;

  case 29:

/* Line 690 of lalr1.cc  */
#line 448 "../CloudValueExpressionParser.yy"
    { (yyval.vfield) = (yysemantic_stack_[(1) - (1)].vfield); }
    break;

  case 30:

/* Line 690 of lalr1.cc  */
#line 449 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.makeField<CML::vector>(*(yysemantic_stack_[(1) - (1)].vec)).ptr();
            delete (yysemantic_stack_[(1) - (1)].vec);
          }
    break;

  case 31:

/* Line 690 of lalr1.cc  */
#line 453 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) + *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 32:

/* Line 690 of lalr1.cc  */
#line 458 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 33:

/* Line 690 of lalr1.cc  */
#line 463 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 34:

/* Line 690 of lalr1.cc  */
#line 468 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].tfield) & *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 35:

/* Line 690 of lalr1.cc  */
#line 473 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) & *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 36:

/* Line 690 of lalr1.cc  */
#line 478 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].yfield) & *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 37:

/* Line 690 of lalr1.cc  */
#line 483 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) & *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 38:

/* Line 690 of lalr1.cc  */
#line 488 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].hfield) & *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 39:

/* Line 690 of lalr1.cc  */
#line 493 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) & *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 40:

/* Line 690 of lalr1.cc  */
#line 498 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 41:

/* Line 690 of lalr1.cc  */
#line 503 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) ^ *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 42:

/* Line 690 of lalr1.cc  */
#line 508 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.vfield) = new CML::vectorField(*(yysemantic_stack_[(3) - (1)].vfield) - *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 43:

/* Line 690 of lalr1.cc  */
#line 513 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(-*(yysemantic_stack_[(2) - (2)].vfield));
            delete (yysemantic_stack_[(2) - (2)].vfield);
          }
    break;

  case 44:

/* Line 690 of lalr1.cc  */
#line 517 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(*(*(yysemantic_stack_[(2) - (2)].tfield)));
            delete (yysemantic_stack_[(2) - (2)].tfield);
          }
    break;

  case 45:

/* Line 690 of lalr1.cc  */
#line 521 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(*(*(yysemantic_stack_[(2) - (2)].yfield)));
            delete (yysemantic_stack_[(2) - (2)].yfield);
          }
    break;

  case 46:

/* Line 690 of lalr1.cc  */
#line 525 "../CloudValueExpressionParser.yy"
    { (yyval.vfield) = (yysemantic_stack_[(3) - (2)].vfield); }
    break;

  case 47:

/* Line 690 of lalr1.cc  */
#line 526 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(CML::eigenValues(*(yysemantic_stack_[(4) - (3)].tfield)));
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 48:

/* Line 690 of lalr1.cc  */
#line 530 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield) = new CML::vectorField(CML::eigenValues(*(yysemantic_stack_[(4) - (3)].yfield)));
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 49:

/* Line 690 of lalr1.cc  */
#line 534 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.composeVectorField(
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::XX)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::XY)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::XZ)()
            ).ptr();
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 50:

/* Line 690 of lalr1.cc  */
#line 542 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.composeVectorField(
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::YX)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::YY)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::YZ)()
            ).ptr();
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 51:

/* Line 690 of lalr1.cc  */
#line 550 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.composeVectorField(
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::ZX)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::ZY)(),
                (yysemantic_stack_[(4) - (1)].tfield)->component(CML::tensor::ZZ)()
            ).ptr();
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 52:

/* Line 690 of lalr1.cc  */
#line 558 "../CloudValueExpressionParser.yy"
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

  case 53:

/* Line 690 of lalr1.cc  */
#line 567 "../CloudValueExpressionParser.yy"
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

  case 54:

/* Line 690 of lalr1.cc  */
#line 576 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].vfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].vfield));
            (yyval.vfield) = driver.doConditional(*(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].vfield),*(yysemantic_stack_[(5) - (5)].vfield)).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].vfield); delete (yysemantic_stack_[(5) - (5)].vfield);
          }
    break;

  case 55:

/* Line 690 of lalr1.cc  */
#line 581 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.makePositionField().ptr();
          }
    break;

  case 57:

/* Line 690 of lalr1.cc  */
#line 585 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield)=driver.getVectorField(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
                    }
    break;

  case 58:

/* Line 690 of lalr1.cc  */
#line 589 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield) = CML::min(*(yysemantic_stack_[(6) - (3)].vfield),*(yysemantic_stack_[(6) - (5)].vfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].vfield); delete (yysemantic_stack_[(6) - (5)].vfield);
          }
    break;

  case 59:

/* Line 690 of lalr1.cc  */
#line 593 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield) = CML::max(*(yysemantic_stack_[(6) - (3)].vfield),*(yysemantic_stack_[(6) - (5)].vfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].vfield); delete (yysemantic_stack_[(6) - (5)].vfield);
          }
    break;

  case 60:

/* Line 690 of lalr1.cc  */
#line 597 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield)=driver.getFluidField<CML::vector>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 61:

/* Line 690 of lalr1.cc  */
#line 604 "../CloudValueExpressionParser.yy"
    {
      (yyval.vfield)=driver.evaluatePluginFunction<CML::vector>(
          *(yysemantic_stack_[(3) - (1)].name),
          (yylocation_stack_[(3) - (2)]),
          numberOfFunctionChars,
          false
      ).ptr();
      delete (yysemantic_stack_[(3) - (1)].name);
  }
    break;

  case 62:

/* Line 690 of lalr1.cc  */
#line 615 "../CloudValueExpressionParser.yy"
    {
            (yyval.val) = CML::gMin(*(yysemantic_stack_[(4) - (3)].sfield));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 63:

/* Line 690 of lalr1.cc  */
#line 619 "../CloudValueExpressionParser.yy"
    {
            (yyval.val) = CML::gMax(*(yysemantic_stack_[(4) - (3)].sfield));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 64:

/* Line 690 of lalr1.cc  */
#line 623 "../CloudValueExpressionParser.yy"
    {
            (yyval.val) = CML::gSum(*(yysemantic_stack_[(4) - (3)].sfield));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 65:

/* Line 690 of lalr1.cc  */
#line 627 "../CloudValueExpressionParser.yy"
    {
            (yyval.val) = CML::gAverage(*(yysemantic_stack_[(4) - (3)].sfield));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 66:

/* Line 690 of lalr1.cc  */
#line 633 "../CloudValueExpressionParser.yy"
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

  case 67:

/* Line 690 of lalr1.cc  */
#line 642 "../CloudValueExpressionParser.yy"
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

  case 68:

/* Line 690 of lalr1.cc  */
#line 651 "../CloudValueExpressionParser.yy"
    {
            CML::vectorField *pos=driver.makePositionField().ptr();
            (yyval.vec) = new CML::vector(
                driver.getPositionOfMinimum(
                    *(yysemantic_stack_[(4) - (3)].sfield),
                    *pos
                )
            );
            delete pos;
            delete (yysemantic_stack_[(4) - (3)].sfield);
        }
    break;

  case 69:

/* Line 690 of lalr1.cc  */
#line 662 "../CloudValueExpressionParser.yy"
    {
            CML::vectorField *pos=driver.makePositionField().ptr();
            (yyval.vec) = new CML::vector(
                driver.getPositionOfMaximum(
                    *(yysemantic_stack_[(4) - (3)].sfield),
                    *pos
                )
            );
            delete pos;
            delete (yysemantic_stack_[(4) - (3)].sfield);
        }
    break;

  case 70:

/* Line 690 of lalr1.cc  */
#line 673 "../CloudValueExpressionParser.yy"
    {
            (yyval.vec) = new CML::vector(CML::gSum(*(yysemantic_stack_[(4) - (3)].vfield)));
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 71:

/* Line 690 of lalr1.cc  */
#line 677 "../CloudValueExpressionParser.yy"
    {
            (yyval.vec) = new CML::vector(CML::gAverage(*(yysemantic_stack_[(4) - (3)].vfield)));
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 72:

/* Line 690 of lalr1.cc  */
#line 683 "../CloudValueExpressionParser.yy"
    { (yyval.sfield) = driver.makeField((yysemantic_stack_[(1) - (1)].val)).ptr(); }
    break;

  case 73:

/* Line 690 of lalr1.cc  */
#line 684 "../CloudValueExpressionParser.yy"
    { (yyval.sfield) = driver.makeField((yysemantic_stack_[(1) - (1)].val)).ptr(); }
    break;

  case 74:

/* Line 690 of lalr1.cc  */
#line 685 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].sfield) + *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 75:

/* Line 690 of lalr1.cc  */
#line 690 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].sfield) - *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 76:

/* Line 690 of lalr1.cc  */
#line 695 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 77:

/* Line 690 of lalr1.cc  */
#line 700 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = driver.makeModuloField(*(yysemantic_stack_[(3) - (1)].sfield),*(yysemantic_stack_[(3) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 78:

/* Line 690 of lalr1.cc  */
#line 705 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].sfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 79:

/* Line 690 of lalr1.cc  */
#line 715 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(6) - (3)].sfield),(yysemantic_stack_[(6) - (5)].sfield));
            (yyval.sfield) = new CML::scalarField(CML::pow(*(yysemantic_stack_[(6) - (3)].sfield), *(yysemantic_stack_[(6) - (5)].sfield)));
            delete (yysemantic_stack_[(6) - (3)].sfield); delete (yysemantic_stack_[(6) - (5)].sfield);
          }
    break;

  case 80:

/* Line 690 of lalr1.cc  */
#line 720 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::log(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 81:

/* Line 690 of lalr1.cc  */
#line 724 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::exp(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 82:

/* Line 690 of lalr1.cc  */
#line 728 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].vfield) & *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 83:

/* Line 690 of lalr1.cc  */
#line 733 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].tfield) && *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 84:

/* Line 690 of lalr1.cc  */
#line 738 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].tfield) && *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 85:

/* Line 690 of lalr1.cc  */
#line 743 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].tfield) && *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 86:

/* Line 690 of lalr1.cc  */
#line 748 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].yfield) && *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 87:

/* Line 690 of lalr1.cc  */
#line 753 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].yfield) && *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 88:

/* Line 690 of lalr1.cc  */
#line 758 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].yfield) && *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 89:

/* Line 690 of lalr1.cc  */
#line 763 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].hfield) && *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 90:

/* Line 690 of lalr1.cc  */
#line 768 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].hfield) && *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 91:

/* Line 690 of lalr1.cc  */
#line 773 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.sfield) = new CML::scalarField(*(yysemantic_stack_[(3) - (1)].hfield) && *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 92:

/* Line 690 of lalr1.cc  */
#line 778 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(-*(yysemantic_stack_[(2) - (2)].sfield));
            delete (yysemantic_stack_[(2) - (2)].sfield);
          }
    break;

  case 93:

/* Line 690 of lalr1.cc  */
#line 782 "../CloudValueExpressionParser.yy"
    { (yyval.sfield) = (yysemantic_stack_[(3) - (2)].sfield); }
    break;

  case 94:

/* Line 690 of lalr1.cc  */
#line 783 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sqr(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 95:

/* Line 690 of lalr1.cc  */
#line 787 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sqrt(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 96:

/* Line 690 of lalr1.cc  */
#line 791 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sin(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 97:

/* Line 690 of lalr1.cc  */
#line 795 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::cos(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 98:

/* Line 690 of lalr1.cc  */
#line 799 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::tan(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 99:

/* Line 690 of lalr1.cc  */
#line 803 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::log10(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 100:

/* Line 690 of lalr1.cc  */
#line 807 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::asin(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 101:

/* Line 690 of lalr1.cc  */
#line 811 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::acos(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 102:

/* Line 690 of lalr1.cc  */
#line 815 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::atan(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 103:

/* Line 690 of lalr1.cc  */
#line 819 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sinh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 104:

/* Line 690 of lalr1.cc  */
#line 823 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::cosh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 105:

/* Line 690 of lalr1.cc  */
#line 827 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::tanh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 106:

/* Line 690 of lalr1.cc  */
#line 831 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::asinh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 107:

/* Line 690 of lalr1.cc  */
#line 835 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::acosh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 108:

/* Line 690 of lalr1.cc  */
#line 839 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::atanh(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 109:

/* Line 690 of lalr1.cc  */
#line 843 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::erf(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 110:

/* Line 690 of lalr1.cc  */
#line 847 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::erfc(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 111:

/* Line 690 of lalr1.cc  */
#line 851 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::lgamma(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 112:

/* Line 690 of lalr1.cc  */
#line 855 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::j0(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 113:

/* Line 690 of lalr1.cc  */
#line 859 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::j1(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 114:

/* Line 690 of lalr1.cc  */
#line 863 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::y0(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 115:

/* Line 690 of lalr1.cc  */
#line 867 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::y1(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 116:

/* Line 690 of lalr1.cc  */
#line 871 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::sign(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 117:

/* Line 690 of lalr1.cc  */
#line 875 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::pos(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 118:

/* Line 690 of lalr1.cc  */
#line 879 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::neg(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 119:

/* Line 690 of lalr1.cc  */
#line 883 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 120:

/* Line 690 of lalr1.cc  */
#line 887 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].vfield)));
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 121:

/* Line 690 of lalr1.cc  */
#line 891 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].tfield)));
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 122:

/* Line 690 of lalr1.cc  */
#line 895 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].yfield)));
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 123:

/* Line 690 of lalr1.cc  */
#line 899 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::mag(*(yysemantic_stack_[(4) - (3)].hfield)));
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 124:

/* Line 690 of lalr1.cc  */
#line 903 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].sfield)));
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;

  case 125:

/* Line 690 of lalr1.cc  */
#line 907 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].vfield)));
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 126:

/* Line 690 of lalr1.cc  */
#line 911 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].tfield)));
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 127:

/* Line 690 of lalr1.cc  */
#line 915 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].yfield)));
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 128:

/* Line 690 of lalr1.cc  */
#line 919 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField(CML::magSqr(*(yysemantic_stack_[(4) - (3)].hfield)));
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 129:

/* Line 690 of lalr1.cc  */
#line 923 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField( CML::tr(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 130:

/* Line 690 of lalr1.cc  */
#line 927 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField( CML::tr(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 131:

/* Line 690 of lalr1.cc  */
#line 931 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField( CML::tr(*(yysemantic_stack_[(4) - (3)].hfield)) );
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 132:

/* Line 690 of lalr1.cc  */
#line 935 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField( CML::det(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 133:

/* Line 690 of lalr1.cc  */
#line 939 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField( CML::det(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 134:

/* Line 690 of lalr1.cc  */
#line 943 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField( CML::det(*(yysemantic_stack_[(4) - (3)].hfield)) );
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 135:

/* Line 690 of lalr1.cc  */
#line 947 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].vfield)->component(0));
            delete (yysemantic_stack_[(4) - (1)].vfield);
          }
    break;

  case 136:

/* Line 690 of lalr1.cc  */
#line 951 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].vfield)->component(1));
            delete (yysemantic_stack_[(4) - (1)].vfield);
          }
    break;

  case 137:

/* Line 690 of lalr1.cc  */
#line 955 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].vfield)->component(2));
            delete (yysemantic_stack_[(4) - (1)].vfield);
          }
    break;

  case 138:

/* Line 690 of lalr1.cc  */
#line 959 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(0));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 139:

/* Line 690 of lalr1.cc  */
#line 963 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(1));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 140:

/* Line 690 of lalr1.cc  */
#line 967 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(2));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 141:

/* Line 690 of lalr1.cc  */
#line 971 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(3));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 142:

/* Line 690 of lalr1.cc  */
#line 975 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(4));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 143:

/* Line 690 of lalr1.cc  */
#line 979 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(5));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 144:

/* Line 690 of lalr1.cc  */
#line 983 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(6));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 145:

/* Line 690 of lalr1.cc  */
#line 987 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(7));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 146:

/* Line 690 of lalr1.cc  */
#line 991 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].tfield)->component(8));
            delete (yysemantic_stack_[(4) - (1)].tfield);
          }
    break;

  case 147:

/* Line 690 of lalr1.cc  */
#line 995 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(0));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 148:

/* Line 690 of lalr1.cc  */
#line 999 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(1));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 149:

/* Line 690 of lalr1.cc  */
#line 1003 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(2));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 150:

/* Line 690 of lalr1.cc  */
#line 1007 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(3));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 151:

/* Line 690 of lalr1.cc  */
#line 1011 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(4));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 152:

/* Line 690 of lalr1.cc  */
#line 1015 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].yfield)->component(5));
            delete (yysemantic_stack_[(4) - (1)].yfield);
          }
    break;

  case 153:

/* Line 690 of lalr1.cc  */
#line 1019 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = new CML::scalarField((yysemantic_stack_[(4) - (1)].hfield)->component(0));
            delete (yysemantic_stack_[(4) - (1)].hfield);
          }
    break;

  case 154:

/* Line 690 of lalr1.cc  */
#line 1023 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].sfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].sfield));
            (yyval.sfield) = driver.doConditional(*(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].sfield),*(yysemantic_stack_[(5) - (5)].sfield)).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].sfield); delete (yysemantic_stack_[(5) - (5)].sfield);
          }
    break;

  case 155:

/* Line 690 of lalr1.cc  */
#line 1028 "../CloudValueExpressionParser.yy"
    {
	  (yyval.sfield) = driver.makeField(
            CML::constant::mathematical::pi
          ).ptr();
          }
    break;

  case 156:

/* Line 690 of lalr1.cc  */
#line 1037 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeIdField().ptr();
          }
    break;

  case 157:

/* Line 690 of lalr1.cc  */
#line 1040 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeField(
                CML::scalar(CML::Pstream::myProcNo())
            ).ptr();
          }
    break;

  case 158:

/* Line 690 of lalr1.cc  */
#line 1045 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.weights(driver.size()).ptr();
          }
    break;

  case 159:

/* Line 690 of lalr1.cc  */
#line 1048 "../CloudValueExpressionParser.yy"
    { (yyval.sfield) = driver.makeRandomField().ptr(); }
    break;

  case 160:

/* Line 690 of lalr1.cc  */
#line 1049 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeRandomField(-(yysemantic_stack_[(4) - (3)].integer)).ptr();
          }
    break;

  case 161:

/* Line 690 of lalr1.cc  */
#line 1052 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeGaussRandomField().ptr();
          }
    break;

  case 162:

/* Line 690 of lalr1.cc  */
#line 1055 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeGaussRandomField(-(yysemantic_stack_[(4) - (3)].integer)).ptr();
          }
    break;

  case 163:

/* Line 690 of lalr1.cc  */
#line 1058 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeRandomField(1).ptr();
          }
    break;

  case 164:

/* Line 690 of lalr1.cc  */
#line 1061 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeRandomField((yysemantic_stack_[(4) - (3)].integer)+1).ptr();
          }
    break;

  case 165:

/* Line 690 of lalr1.cc  */
#line 1064 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeGaussRandomField(1).ptr();
          }
    break;

  case 166:

/* Line 690 of lalr1.cc  */
#line 1067 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeGaussRandomField((yysemantic_stack_[(4) - (3)].integer)+1).ptr();
          }
    break;

  case 167:

/* Line 690 of lalr1.cc  */
#line 1070 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeField(driver.runTime().deltaT().value()).ptr();
          }
    break;

  case 168:

/* Line 690 of lalr1.cc  */
#line 1073 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = driver.makeField(driver.runTime().time().value()).ptr();
          }
    break;

  case 170:

/* Line 690 of lalr1.cc  */
#line 1081 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getScalarField(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
				}
    break;

  case 171:

/* Line 690 of lalr1.cc  */
#line 1085 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getLine(*(yysemantic_stack_[(1) - (1)].name),driver.runTime().time().value()).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
				}
    break;

  case 172:

/* Line 690 of lalr1.cc  */
#line 1089 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getLookup(*(yysemantic_stack_[(4) - (1)].name),*(yysemantic_stack_[(4) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(4) - (1)].name); delete(yysemantic_stack_[(4) - (3)].sfield);
                                    }
    break;

  case 173:

/* Line 690 of lalr1.cc  */
#line 1093 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = CML::min(*(yysemantic_stack_[(6) - (3)].sfield),*(yysemantic_stack_[(6) - (5)].sfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].sfield); delete (yysemantic_stack_[(6) - (5)].sfield);
          }
    break;

  case 174:

/* Line 690 of lalr1.cc  */
#line 1097 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield) = CML::max(*(yysemantic_stack_[(6) - (3)].sfield),*(yysemantic_stack_[(6) - (5)].sfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].sfield); delete (yysemantic_stack_[(6) - (5)].sfield);
          }
    break;

  case 175:

/* Line 690 of lalr1.cc  */
#line 1101 "../CloudValueExpressionParser.yy"
    {
            (yyval.sfield)=driver.getFluidField<CML::scalar>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 176:

/* Line 690 of lalr1.cc  */
#line 1108 "../CloudValueExpressionParser.yy"
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

  case 177:

/* Line 690 of lalr1.cc  */
#line 1119 "../CloudValueExpressionParser.yy"
    { (yyval.tfield) = (yysemantic_stack_[(1) - (1)].tfield); }
    break;

  case 178:

/* Line 690 of lalr1.cc  */
#line 1120 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) + *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 179:

/* Line 690 of lalr1.cc  */
#line 1125 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) + *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 180:

/* Line 690 of lalr1.cc  */
#line 1130 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) + *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 181:

/* Line 690 of lalr1.cc  */
#line 1135 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].yfield) + *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 182:

/* Line 690 of lalr1.cc  */
#line 1140 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].hfield) + *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 183:

/* Line 690 of lalr1.cc  */
#line 1145 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 184:

/* Line 690 of lalr1.cc  */
#line 1150 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 185:

/* Line 690 of lalr1.cc  */
#line 1155 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].vfield),(yysemantic_stack_[(3) - (3)].vfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].vfield) * *(yysemantic_stack_[(3) - (3)].vfield));
            delete (yysemantic_stack_[(3) - (1)].vfield); delete (yysemantic_stack_[(3) - (3)].vfield);
          }
    break;

  case 186:

/* Line 690 of lalr1.cc  */
#line 1160 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) & *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 187:

/* Line 690 of lalr1.cc  */
#line 1165 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].yfield) & *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 188:

/* Line 690 of lalr1.cc  */
#line 1170 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) & *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 189:

/* Line 690 of lalr1.cc  */
#line 1175 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].hfield) & *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 190:

/* Line 690 of lalr1.cc  */
#line 1180 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) & *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 191:

/* Line 690 of lalr1.cc  */
#line 1185 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 192:

/* Line 690 of lalr1.cc  */
#line 1190 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) - *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 193:

/* Line 690 of lalr1.cc  */
#line 1195 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) - *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 194:

/* Line 690 of lalr1.cc  */
#line 1200 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].tfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].tfield) - *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].tfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 195:

/* Line 690 of lalr1.cc  */
#line 1205 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].yfield) - *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 196:

/* Line 690 of lalr1.cc  */
#line 1210 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].tfield));
            (yyval.tfield) = new CML::tensorField(*(yysemantic_stack_[(3) - (1)].hfield) - *(yysemantic_stack_[(3) - (3)].tfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].tfield);
          }
    break;

  case 197:

/* Line 690 of lalr1.cc  */
#line 1215 "../CloudValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField(-*(yysemantic_stack_[(2) - (2)].tfield));
            delete (yysemantic_stack_[(2) - (2)].tfield);
          }
    break;

  case 198:

/* Line 690 of lalr1.cc  */
#line 1219 "../CloudValueExpressionParser.yy"
    { (yyval.tfield) = (yysemantic_stack_[(3) - (2)].tfield); }
    break;

  case 199:

/* Line 690 of lalr1.cc  */
#line 1220 "../CloudValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::skew(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 200:

/* Line 690 of lalr1.cc  */
#line 1224 "../CloudValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField(CML::eigenVectors(*(yysemantic_stack_[(4) - (3)].tfield)));
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 201:

/* Line 690 of lalr1.cc  */
#line 1228 "../CloudValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField(CML::eigenVectors(*(yysemantic_stack_[(4) - (3)].yfield)));
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 202:

/* Line 690 of lalr1.cc  */
#line 1232 "../CloudValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::inv(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 203:

/* Line 690 of lalr1.cc  */
#line 1236 "../CloudValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::cof(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 204:

/* Line 690 of lalr1.cc  */
#line 1240 "../CloudValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::dev(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 205:

/* Line 690 of lalr1.cc  */
#line 1244 "../CloudValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( CML::dev2(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 206:

/* Line 690 of lalr1.cc  */
#line 1248 "../CloudValueExpressionParser.yy"
    {
            (yyval.tfield) = new CML::tensorField( (yysemantic_stack_[(6) - (1)].tfield)->T() );
            delete (yysemantic_stack_[(6) - (1)].tfield);
          }
    break;

  case 207:

/* Line 690 of lalr1.cc  */
#line 1252 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].tfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].tfield));
            (yyval.tfield) = driver.doConditional(*(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].tfield),*(yysemantic_stack_[(5) - (5)].tfield)).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].tfield); delete (yysemantic_stack_[(5) - (5)].tfield);
          }
    break;

  case 209:

/* Line 690 of lalr1.cc  */
#line 1258 "../CloudValueExpressionParser.yy"
    {
            (yyval.tfield)=driver.getTensorField(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
                    }
    break;

  case 210:

/* Line 690 of lalr1.cc  */
#line 1262 "../CloudValueExpressionParser.yy"
    {
            (yyval.tfield) = CML::min(*(yysemantic_stack_[(6) - (3)].tfield),*(yysemantic_stack_[(6) - (5)].tfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].tfield); delete (yysemantic_stack_[(6) - (5)].tfield);
          }
    break;

  case 211:

/* Line 690 of lalr1.cc  */
#line 1266 "../CloudValueExpressionParser.yy"
    {
            (yyval.tfield) = CML::max(*(yysemantic_stack_[(6) - (3)].tfield),*(yysemantic_stack_[(6) - (5)].tfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].tfield); delete (yysemantic_stack_[(6) - (5)].tfield);
          }
    break;

  case 212:

/* Line 690 of lalr1.cc  */
#line 1270 "../CloudValueExpressionParser.yy"
    {
            (yyval.tfield)=driver.getFluidField<CML::tensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 213:

/* Line 690 of lalr1.cc  */
#line 1277 "../CloudValueExpressionParser.yy"
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

  case 214:

/* Line 690 of lalr1.cc  */
#line 1289 "../CloudValueExpressionParser.yy"
    { (yyval.yfield) = (yysemantic_stack_[(1) - (1)].yfield); }
    break;

  case 215:

/* Line 690 of lalr1.cc  */
#line 1290 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) + *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 216:

/* Line 690 of lalr1.cc  */
#line 1295 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) + *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 217:

/* Line 690 of lalr1.cc  */
#line 1300 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].hfield) + *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 218:

/* Line 690 of lalr1.cc  */
#line 1305 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 219:

/* Line 690 of lalr1.cc  */
#line 1310 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 220:

/* Line 690 of lalr1.cc  */
#line 1315 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(
                symm(*(yysemantic_stack_[(3) - (1)].yfield) & *(yysemantic_stack_[(3) - (3)].yfield))
            );
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 221:

/* Line 690 of lalr1.cc  */
#line 1326 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].hfield) & *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 222:

/* Line 690 of lalr1.cc  */
#line 1331 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) & *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 223:

/* Line 690 of lalr1.cc  */
#line 1336 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 224:

/* Line 690 of lalr1.cc  */
#line 1341 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) - *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 225:

/* Line 690 of lalr1.cc  */
#line 1346 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].yfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].yfield) - *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].yfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 226:

/* Line 690 of lalr1.cc  */
#line 1351 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].yfield));
            (yyval.yfield) = new CML::symmTensorField(*(yysemantic_stack_[(3) - (1)].hfield) - *(yysemantic_stack_[(3) - (3)].yfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].yfield);
          }
    break;

  case 227:

/* Line 690 of lalr1.cc  */
#line 1356 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField(-*(yysemantic_stack_[(2) - (2)].yfield));
            delete (yysemantic_stack_[(2) - (2)].yfield);
          }
    break;

  case 228:

/* Line 690 of lalr1.cc  */
#line 1360 "../CloudValueExpressionParser.yy"
    { (yyval.yfield) = (yysemantic_stack_[(3) - (2)].yfield); }
    break;

  case 229:

/* Line 690 of lalr1.cc  */
#line 1361 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::symm(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 230:

/* Line 690 of lalr1.cc  */
#line 1365 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::symm(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 231:

/* Line 690 of lalr1.cc  */
#line 1369 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::twoSymm(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 232:

/* Line 690 of lalr1.cc  */
#line 1373 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::twoSymm(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 233:

/* Line 690 of lalr1.cc  */
#line 1377 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::inv(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 234:

/* Line 690 of lalr1.cc  */
#line 1381 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::cof(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 235:

/* Line 690 of lalr1.cc  */
#line 1385 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::dev(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 236:

/* Line 690 of lalr1.cc  */
#line 1389 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::dev2(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 237:

/* Line 690 of lalr1.cc  */
#line 1393 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = new CML::symmTensorField( CML::sqr(*(yysemantic_stack_[(4) - (3)].vfield)) );
            delete (yysemantic_stack_[(4) - (3)].vfield);
          }
    break;

  case 238:

/* Line 690 of lalr1.cc  */
#line 1397 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = (yysemantic_stack_[(6) - (1)].yfield);
          }
    break;

  case 239:

/* Line 690 of lalr1.cc  */
#line 1400 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].yfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].yfield));
            (yyval.yfield) = driver.doConditional(*(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].yfield),*(yysemantic_stack_[(5) - (5)].yfield)).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].yfield); delete (yysemantic_stack_[(5) - (5)].yfield);
          }
    break;

  case 241:

/* Line 690 of lalr1.cc  */
#line 1406 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield)=driver.getSymmTensorField(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
                    }
    break;

  case 242:

/* Line 690 of lalr1.cc  */
#line 1410 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = CML::min(*(yysemantic_stack_[(6) - (3)].yfield),*(yysemantic_stack_[(6) - (5)].yfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].yfield); delete (yysemantic_stack_[(6) - (5)].yfield);
          }
    break;

  case 243:

/* Line 690 of lalr1.cc  */
#line 1414 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = CML::max(*(yysemantic_stack_[(6) - (3)].yfield),*(yysemantic_stack_[(6) - (5)].yfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].yfield); delete (yysemantic_stack_[(6) - (5)].yfield);
          }
    break;

  case 244:

/* Line 690 of lalr1.cc  */
#line 1418 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield)=driver.getFluidField<CML::symmTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 245:

/* Line 690 of lalr1.cc  */
#line 1425 "../CloudValueExpressionParser.yy"
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

  case 246:

/* Line 690 of lalr1.cc  */
#line 1437 "../CloudValueExpressionParser.yy"
    { (yyval.hfield) = (yysemantic_stack_[(1) - (1)].hfield); }
    break;

  case 247:

/* Line 690 of lalr1.cc  */
#line 1438 "../CloudValueExpressionParser.yy"
    {
            (yyval.hfield) = driver.makeField(CML::sphericalTensor(1)).ptr();
          }
    break;

  case 248:

/* Line 690 of lalr1.cc  */
#line 1441 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) + *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 249:

/* Line 690 of lalr1.cc  */
#line 1446 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].sfield) * *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 250:

/* Line 690 of lalr1.cc  */
#line 1451 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) * *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 251:

/* Line 690 of lalr1.cc  */
#line 1456 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) & *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 252:

/* Line 690 of lalr1.cc  */
#line 1461 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) / *(yysemantic_stack_[(3) - (3)].sfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 253:

/* Line 690 of lalr1.cc  */
#line 1466 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].hfield),(yysemantic_stack_[(3) - (3)].hfield));
            (yyval.hfield) = new CML::sphericalTensorField(*(yysemantic_stack_[(3) - (1)].hfield) - *(yysemantic_stack_[(3) - (3)].hfield));
            delete (yysemantic_stack_[(3) - (1)].hfield); delete (yysemantic_stack_[(3) - (3)].hfield);
          }
    break;

  case 254:

/* Line 690 of lalr1.cc  */
#line 1471 "../CloudValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField(-*(yysemantic_stack_[(2) - (2)].hfield));
            delete (yysemantic_stack_[(2) - (2)].hfield);
          }
    break;

  case 255:

/* Line 690 of lalr1.cc  */
#line 1475 "../CloudValueExpressionParser.yy"
    { (yyval.hfield) = (yysemantic_stack_[(3) - (2)].hfield); }
    break;

  case 256:

/* Line 690 of lalr1.cc  */
#line 1476 "../CloudValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField( CML::sph(*(yysemantic_stack_[(4) - (3)].tfield)) );
            delete (yysemantic_stack_[(4) - (3)].tfield);
          }
    break;

  case 257:

/* Line 690 of lalr1.cc  */
#line 1480 "../CloudValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField( CML::sph(*(yysemantic_stack_[(4) - (3)].yfield)) );
            delete (yysemantic_stack_[(4) - (3)].yfield);
          }
    break;

  case 258:

/* Line 690 of lalr1.cc  */
#line 1484 "../CloudValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField( CML::sph(*(yysemantic_stack_[(4) - (3)].hfield)) );
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 259:

/* Line 690 of lalr1.cc  */
#line 1488 "../CloudValueExpressionParser.yy"
    {
            (yyval.hfield) = new CML::sphericalTensorField( CML::inv(*(yysemantic_stack_[(4) - (3)].hfield)) );
            delete (yysemantic_stack_[(4) - (3)].hfield);
          }
    break;

  case 260:

/* Line 690 of lalr1.cc  */
#line 1492 "../CloudValueExpressionParser.yy"
    {
            (yyval.hfield) = (yysemantic_stack_[(6) - (1)].hfield);
          }
    break;

  case 261:

/* Line 690 of lalr1.cc  */
#line 1495 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (3)].hfield)); sameSize((yysemantic_stack_[(5) - (1)].lfield),(yysemantic_stack_[(5) - (5)].hfield));
            (yyval.hfield) = driver.doConditional(*(yysemantic_stack_[(5) - (1)].lfield),*(yysemantic_stack_[(5) - (3)].hfield),*(yysemantic_stack_[(5) - (5)].hfield)).ptr();
            delete (yysemantic_stack_[(5) - (1)].lfield); delete (yysemantic_stack_[(5) - (3)].hfield); delete (yysemantic_stack_[(5) - (5)].hfield);
          }
    break;

  case 263:

/* Line 690 of lalr1.cc  */
#line 1505 "../CloudValueExpressionParser.yy"
    {
            (yyval.hfield)=driver.getSphericalTensorField(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
                    }
    break;

  case 264:

/* Line 690 of lalr1.cc  */
#line 1509 "../CloudValueExpressionParser.yy"
    {
            (yyval.hfield) = CML::min(*(yysemantic_stack_[(6) - (3)].hfield),*(yysemantic_stack_[(6) - (5)].hfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].hfield); delete (yysemantic_stack_[(6) - (5)].hfield);
          }
    break;

  case 265:

/* Line 690 of lalr1.cc  */
#line 1513 "../CloudValueExpressionParser.yy"
    {
            (yyval.hfield) = CML::max(*(yysemantic_stack_[(6) - (3)].hfield),*(yysemantic_stack_[(6) - (5)].hfield)).ptr();
            delete (yysemantic_stack_[(6) - (3)].hfield); delete (yysemantic_stack_[(6) - (5)].hfield);
          }
    break;

  case 266:

/* Line 690 of lalr1.cc  */
#line 1517 "../CloudValueExpressionParser.yy"
    {
            (yyval.hfield)=driver.getFluidField<CML::sphericalTensor>(*(yysemantic_stack_[(4) - (3)].name)).ptr();
            delete (yysemantic_stack_[(4) - (3)].name);
          }
    break;

  case 267:

/* Line 690 of lalr1.cc  */
#line 1524 "../CloudValueExpressionParser.yy"
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

  case 268:

/* Line 690 of lalr1.cc  */
#line 1536 "../CloudValueExpressionParser.yy"
    { (yyval.lfield) = driver.makeField(true).ptr(); }
    break;

  case 269:

/* Line 690 of lalr1.cc  */
#line 1537 "../CloudValueExpressionParser.yy"
    { (yyval.lfield) = driver.makeField(false).ptr(); }
    break;

  case 270:

/* Line 690 of lalr1.cc  */
#line 1538 "../CloudValueExpressionParser.yy"
    {
        (yyval.lfield) = driver.makeCellSetField(*(yysemantic_stack_[(4) - (3)].name)).ptr();
        delete (yysemantic_stack_[(4) - (3)].name);
      }
    break;

  case 271:

/* Line 690 of lalr1.cc  */
#line 1542 "../CloudValueExpressionParser.yy"
    {
        (yyval.lfield) = driver.makeCellZoneField(*(yysemantic_stack_[(4) - (3)].name)).ptr();
        delete (yysemantic_stack_[(4) - (3)].name);
      }
    break;

  case 272:

/* Line 690 of lalr1.cc  */
#line 1546 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(*(yysemantic_stack_[(3) - (1)].sfield),std::less<CML::scalar>(),*(yysemantic_stack_[(3) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 273:

/* Line 690 of lalr1.cc  */
#line 1551 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(*(yysemantic_stack_[(3) - (1)].sfield),std::greater<CML::scalar>(),*(yysemantic_stack_[(3) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 274:

/* Line 690 of lalr1.cc  */
#line 1556 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(*(yysemantic_stack_[(3) - (1)].sfield),std::less_equal<CML::scalar>(),*(yysemantic_stack_[(3) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 275:

/* Line 690 of lalr1.cc  */
#line 1561 "../CloudValueExpressionParser.yy"
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

  case 276:

/* Line 690 of lalr1.cc  */
#line 1570 "../CloudValueExpressionParser.yy"
    {
            sameSize((yysemantic_stack_[(3) - (1)].sfield),(yysemantic_stack_[(3) - (3)].sfield));
            (yyval.lfield) = driver.doCompare(*(yysemantic_stack_[(3) - (1)].sfield),std::equal_to<CML::scalar>(),*(yysemantic_stack_[(3) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(3) - (1)].sfield); delete (yysemantic_stack_[(3) - (3)].sfield);
          }
    break;

  case 277:

/* Line 690 of lalr1.cc  */
#line 1575 "../CloudValueExpressionParser.yy"
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

  case 278:

/* Line 690 of lalr1.cc  */
#line 1584 "../CloudValueExpressionParser.yy"
    { (yyval.lfield) = (yysemantic_stack_[(3) - (2)].lfield); }
    break;

  case 279:

/* Line 690 of lalr1.cc  */
#line 1585 "../CloudValueExpressionParser.yy"
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

  case 280:

/* Line 690 of lalr1.cc  */
#line 1594 "../CloudValueExpressionParser.yy"
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

  case 281:

/* Line 690 of lalr1.cc  */
#line 1603 "../CloudValueExpressionParser.yy"
    {
            (yyval.lfield) = driver.doLogicalNot(*(yysemantic_stack_[(2) - (2)].lfield)).ptr();
            delete (yysemantic_stack_[(2) - (2)].lfield);
          }
    break;

  case 283:

/* Line 690 of lalr1.cc  */
#line 1608 "../CloudValueExpressionParser.yy"
    {
            (yyval.lfield)=driver.getBoolField(*(yysemantic_stack_[(1) - (1)].name)).ptr();
            delete (yysemantic_stack_[(1) - (1)].name);
      }
    break;

  case 284:

/* Line 690 of lalr1.cc  */
#line 1615 "../CloudValueExpressionParser.yy"
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

  case 285:

/* Line 690 of lalr1.cc  */
#line 1627 "../CloudValueExpressionParser.yy"
    {
            (yyval.vfield) = driver.composeVectorField(*(yysemantic_stack_[(8) - (3)].sfield),*(yysemantic_stack_[(8) - (5)].sfield),*(yysemantic_stack_[(8) - (7)].sfield)).ptr();
            delete (yysemantic_stack_[(8) - (3)].sfield); delete (yysemantic_stack_[(8) - (5)].sfield); delete (yysemantic_stack_[(8) - (7)].sfield);
          }
    break;

  case 286:

/* Line 690 of lalr1.cc  */
#line 1633 "../CloudValueExpressionParser.yy"
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

  case 287:

/* Line 690 of lalr1.cc  */
#line 1643 "../CloudValueExpressionParser.yy"
    {
            (yyval.yfield) = driver.composeSymmTensorField(
                *(yysemantic_stack_[(14) - (3)].sfield),*(yysemantic_stack_[(14) - (5)].sfield),*(yysemantic_stack_[(14) - (7)].sfield),
                *(yysemantic_stack_[(14) - (9)].sfield),*(yysemantic_stack_[(14) - (11)].sfield),
                *(yysemantic_stack_[(14) - (13)].sfield)
            ).ptr();
            delete (yysemantic_stack_[(14) - (3)].sfield); delete (yysemantic_stack_[(14) - (5)].sfield); delete (yysemantic_stack_[(14) - (7)].sfield); delete (yysemantic_stack_[(14) - (9)].sfield); delete (yysemantic_stack_[(14) - (11)].sfield); delete (yysemantic_stack_[(14) - (13)].sfield);
          }
    break;

  case 288:

/* Line 690 of lalr1.cc  */
#line 1652 "../CloudValueExpressionParser.yy"
    {
            (yyval.hfield) = driver.composeSphericalTensorField(*(yysemantic_stack_[(4) - (3)].sfield)).ptr();
            delete (yysemantic_stack_[(4) - (3)].sfield);
          }
    break;



/* Line 690 of lalr1.cc  */
#line 4356 "CloudValueExpressionParser.tab.cc"
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
  CloudValueExpressionParser::yysyntax_error_ (int yystate, int yytoken)
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
  const short int CloudValueExpressionParser::yypact_ninf_ = -262;
  const short int
  CloudValueExpressionParser::yypact_[] =
  {
      2478,   691,   691,   691,   691,   691,   691,   691,   691,   691,
     691,   691,   691,   691,  -145,  -136,    21,  -262,  -262,  -134,
    -262,  -262,  -262,  -262,  -262,  -262,  -117,  -114,  -108,  -104,
     -99,   -85,  -262,   -77,   -73,   -70,   -68,  -262,  -262,   -51,
    -262,  -262,   -47,   -46,   -43,   -42,   -39,   -30,   -26,     1,
       5,     7,     8,    49,    66,    67,    69,    73,    74,    75,
      83,    84,    85,    97,   111,   112,   114,   119,   123,   124,
     125,   128,   129,   130,   131,   133,   138,   143,   147,   148,
     150,   152,   158,   163,   166,   171,   172,   173,   175,   179,
     182,   184,   199,   200,   207,   208,   212,   215,   216,   220,
     221,   223,   224,   691,   691,   691,   691,  -262,  2562,    12,
    -262,  -262,  2557,    12,  2330,    12,  2343,    12,  2356,    12,
    -124,    12,  -262,  -262,  -262,  -262,  2562,  2030,  2330,  2343,
    2356,  -124,  1097,  2539,  2557,  2422,  1908,   -80,  1921,   -66,
    1934,   -28,   -82,   -90,  -262,  -262,  -262,   691,  -262,  -262,
    -262,  -262,  -262,  -262,   691,   691,   691,   691,   394,   -21,
     -20,   -81,   -19,   -18,   -79,   -59,     4,   691,   691,   691,
     691,   691,   691,   691,   691,   691,   691,   691,   691,   691,
     691,   691,   691,   691,   691,   691,   691,   691,   691,   691,
     691,   691,   691,   691,   691,   691,   691,   691,   691,   691,
     691,   691,   691,   691,   691,   691,   691,   691,   691,   691,
     691,   691,   691,   691,   691,   691,   178,   214,   364,   367,
     233,  -262,   237,   240,   244,   237,   240,  2435,  1116,   -15,
       3,    71,   -86,  -262,   691,   691,   691,   691,   691,   691,
    -262,  -262,    12,  -262,   691,   691,   691,   691,   691,   691,
     691,   691,   691,   691,   691,  -262,   691,   691,   691,   691,
     691,   691,  -262,  -262,   691,   691,   691,   691,   691,   691,
    -262,  -262,   691,   691,   691,   691,   691,   691,  -262,  -262,
     691,   691,   691,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  1135,  -262,  -262,  -262,
    -262,  -262,  -262,  2048,  2066,  2084,  1154,   243,   256,   278,
     281,   282,   288,  -262,   293,  -262,  -262,   294,  -262,   297,
    -262,  -262,  -262,  -262,  2102,  1173,  1192,  2448,  1211,   177,
     248,   298,  2461,  1230,   311,   371,   415,  1249,  1268,  1287,
    2396,   206,  1947,  1960,  1973,  2409,  1078,  1986,  1999,  2012,
    1306,  1325,  2474,  1344,  2487,  1363,  2500,  1382,  1401,  1420,
    1439,  1458,  1477,  1496,  1515,  1534,  1553,  1572,  1591,  1610,
    1629,  1648,  1667,  1686,  1705,  1724,  1743,  1762,  1781,   428,
     470,   483,   504,   517,   569,   582,   706,   719,   734,   747,
     760,   773,   786,   799,   812,   825,   838,   851,   864,   877,
     890,   903,   916,   929,   942,   955,   968,   981,  -262,  -262,
     301,   305,  -262,  -262,  -262,  -262,  -262,  -262,    63,    63,
     233,  -262,  -262,   233,   237,   240,   244,   233,    35,  -262,
    2557,  2557,  2557,  2557,  2557,  2557,   -23,   -23,  -262,   233,
    -262,   237,   240,   244,  -262,   116,   350,   605,    -5,    55,
     108,    -5,    55,   108,  -262,  -262,   233,   237,   240,   244,
     -31,   116,   350,   605,    -5,    55,   108,    -5,    55,   108,
    -262,  -262,   233,   237,   240,   244,   -22,   116,   350,   605,
      -5,    55,   108,    -5,    55,   108,  -262,  -262,   233,   237,
     240,   244,   -60,   -49,  2391,   154,   266,   692,   263,  -262,
    -262,   691,   691,   691,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,   691,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
     691,  -262,   691,  -262,   691,   691,   691,   691,  -262,   691,
    -262,   691,   691,   691,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,   279,
    -262,  -262,  -262,  -262,  -262,  -262,   306,  -262,   307,   691,
     691,   691,   691,   691,  2120,  2138,  2156,  1800,  2513,  1819,
     994,  1007,  1020,  2526,  1838,  1033,  1046,  1059,   312,   316,
     317,  2562,  2557,  2330,  2343,  2356,   691,   691,   691,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  1857,  2174,  2192,  -262,   691,   691,  2210,
    2228,   691,   691,  2246,  2264,   691,   691,  2282,  1876,   691,
    -262,  2300,   691,  2318,   691,  1895,  -262
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned short int
  CloudValueExpressionParser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     2,   171,     0,
     170,    57,   283,   209,   241,   263,     0,     0,     0,     0,
       0,     0,    72,     0,     0,     0,     0,   268,   269,     0,
     247,   155,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     3,    21,     0,
      73,    30,    20,     0,    23,     0,    24,     0,    25,     0,
      22,     0,    29,   177,   214,   246,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    16,    17,     1,     0,    28,    28,
      28,    28,    28,    28,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,    92,   197,   227,   254,    44,    45,     0,     0,     0,
       0,     0,     0,   281,     0,     0,     0,     0,     0,     0,
      26,    18,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   169,     0,     0,     0,     0,
       0,     0,    27,   208,     0,     0,     0,     0,     0,     0,
      27,   240,     0,     0,     0,     0,     0,     0,    27,   262,
       0,     0,     0,   282,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,     0,   176,    61,   213,
     245,   267,   284,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   159,     0,   163,   156,     0,   161,     0,
     165,    55,   167,   168,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   157,   158,
       0,     0,    46,    93,   198,   228,   255,   278,    42,    31,
     185,    33,    40,    82,    35,    37,    39,    41,     0,    19,
     277,   276,   272,   273,   275,   274,    75,    74,    77,    32,
      76,   183,   218,   249,    78,    83,    84,    85,   192,   193,
     194,   178,   179,   180,   184,   191,    34,   186,   188,   190,
       0,    86,    87,    88,   195,   224,   225,   181,   215,   216,
     219,   223,    36,   187,   220,   222,     0,    89,    90,    91,
     196,   226,   253,   182,   217,   248,   250,   252,    38,   189,
     221,   251,     0,     0,     0,     0,     0,     0,   280,   279,
     172,     0,     0,     0,   288,   175,    60,   212,   244,   266,
     160,   164,   162,   166,     0,    80,    81,   120,   119,   121,
     122,   123,   125,   124,   126,   127,   128,    96,    97,    98,
       0,    66,     0,    62,     0,     0,     0,     0,    67,     0,
      63,     0,     0,     0,    68,    69,    71,    65,    70,    64,
     237,    94,    95,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    52,    53,   129,   130,   131,   204,   235,
     230,   229,   199,   132,   133,   134,   203,   234,   202,   233,
     259,   256,   257,   258,   232,   231,   205,   236,    47,    48,
     200,   201,   270,   271,   135,   136,   137,    49,    50,    51,
     138,   139,   140,   141,   142,   143,   144,   145,   146,     0,
     147,   148,   149,   150,   151,   152,     0,   153,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,   154,   207,   239,   261,     0,     0,     0,    79,
      58,   173,   210,   242,   264,    59,   174,   211,   243,   265,
     206,   238,   260,     0,     0,     0,   285,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     287,     0,     0,     0,     0,     0,   286
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  CloudValueExpressionParser::yypgoto_[] =
  {
      -262,  -262,  -262,   -97,  -262,  -262,  -261,   280,    58,  -262,
    -262,  -262,    -1,  -262,   336,  -262,   213,  -262,   102,  -262,
       2,  -262,  -262,  -262,  -262,  -262
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  CloudValueExpressionParser::yydefgoto_[] =
  {
        -1,    16,    17,   243,   107,   428,   460,   297,   126,   109,
     110,   111,   134,   113,   128,   115,   129,   117,   130,   119,
     131,   121,   122,   123,   124,   125
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char CloudValueExpressionParser::yytable_ninf_ = -1;
  const unsigned short int
  CloudValueExpressionParser::yytable_[] =
  {
       112,   127,   132,   120,   312,   314,   317,   319,   627,   476,
     280,   144,   281,   282,   142,   143,   255,   492,   263,   145,
     271,   146,   279,   147,   283,   607,   608,   609,   610,   611,
     612,   613,   614,   615,   616,   617,   618,   620,   621,   622,
     148,   623,   624,   149,   280,   625,   281,   282,   280,   150,
     281,   282,   280,   151,   281,   282,   628,   256,   152,   108,
     241,   242,   133,   135,   257,   258,   295,   259,   260,   261,
     417,   264,   153,   294,   262,   316,   289,   321,   265,   266,
     154,   267,   268,   269,   155,   619,   629,   156,   270,   157,
     291,   604,   605,   606,   626,   234,   235,   322,   236,   237,
     238,   239,   221,   118,   228,   240,   158,   232,   233,   272,
     159,   160,   140,   141,   161,   162,   273,   274,   163,   275,
     276,   277,   256,   252,   253,   254,   278,   164,   293,   257,
     258,   165,   259,   260,   261,   313,   315,   318,   320,   262,
     264,   414,   259,   260,   261,   429,   296,   265,   266,   262,
     267,   268,   269,   303,   304,   305,   306,   270,   166,   415,
     323,   220,   167,   227,   168,   169,   324,   325,   326,   328,
     333,   337,   338,   339,   341,   346,   350,   351,   353,   355,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   267,   268,   269,   224,   170,   231,   272,   270,
     236,   237,   238,   239,   116,   273,   274,   240,   275,   276,
     277,   138,   139,   171,   172,   278,   173,   416,   327,   332,
     174,   175,   176,   340,   345,   421,   422,   352,   354,   356,
     177,   178,   179,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   440,   444,   180,   275,   276,   277,   454,   455,
     257,   258,   278,   259,   260,   261,   470,   471,   181,   182,
     262,   183,   331,   336,   486,   487,   184,   344,   349,   494,
     185,   186,   187,   498,   499,   188,   189,   190,   191,   631,
     192,   256,   418,   419,   420,   193,   423,   427,   257,   258,
     194,   259,   260,   261,   195,   196,   383,   197,   262,   198,
     391,   439,   396,   399,   256,   199,   223,   226,   230,   456,
     200,   257,   258,   201,   259,   260,   261,   472,   202,   203,
     204,   262,   205,   519,   408,   488,   206,   114,   493,   207,
     426,   208,   136,   137,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   443,   209,   210,   447,   450,
     453,   532,   533,   459,   211,   212,   463,   466,   469,   213,
     409,   475,   214,   215,   479,   482,   485,   216,   217,   491,
     218,   219,   497,   330,   335,   264,   410,   240,   343,   348,
     411,   262,   265,   266,   270,   267,   268,   269,   278,   505,
     282,   632,   270,   264,   520,   307,   308,   309,   310,   311,
     265,   266,   506,   267,   268,   269,   380,   382,   385,   387,
     270,   390,   393,   395,   398,   401,   403,   405,   407,   298,
     299,   300,   301,   302,   507,   272,   648,   508,   509,   222,
     225,   229,   273,   274,   510,   275,   276,   277,   256,   511,
     512,   425,   278,   513,   521,   257,   258,   602,   259,   260,
     261,   603,     0,   649,   650,   262,   442,   524,   670,   446,
     449,   452,   671,   672,   458,     0,     0,   462,   465,   468,
       0,     0,   474,     0,     0,   478,   481,   484,     0,     0,
     490,     0,     0,   496,   265,   266,     0,   267,   268,   269,
     634,   635,   636,     0,   270,     0,   329,   334,   264,     0,
       0,   342,   347,   637,     0,   265,   266,     0,   267,   268,
     269,     0,     0,     0,     0,   270,     0,   525,     0,     0,
       0,   639,     0,     0,     0,     0,     0,     0,   644,   379,
     381,   384,   386,   388,   389,   392,   394,   397,   400,   402,
     404,   406,   272,     0,     0,     0,     0,     0,     0,   273,
     274,     0,   275,   276,   277,   256,     0,     0,     0,   278,
       0,   526,   257,   258,   424,   259,   260,   261,     0,     0,
       0,     0,   262,     0,   573,     0,     0,     0,   638,   441,
       0,     0,   445,   448,   451,   643,     0,   457,     0,     0,
     461,   464,   467,     0,     0,   473,     0,   264,   477,   480,
     483,     0,     0,   489,   265,   266,   495,   267,   268,   269,
     256,     0,     0,     0,   270,     0,   574,   257,   258,   652,
     259,   260,   261,     0,     0,     0,     0,   262,   642,   575,
       0,   264,     0,     0,     0,   647,     0,     0,   265,   266,
       0,   267,   268,   269,   272,   673,   674,   675,   270,     0,
     576,   273,   274,     0,   275,   276,   277,     0,     0,     0,
       0,   278,     0,   577,     0,     0,   679,   680,     0,     0,
     683,   684,     0,     0,   687,   688,     0,   651,   691,     0,
       0,   693,     0,   695,    18,    19,    20,    21,    22,    23,
      24,    25,     0,     0,     0,     0,   256,    26,    27,    28,
      29,    30,    31,   257,   258,    32,   259,   260,   261,   264,
       0,     0,     0,   262,     0,   578,   265,   266,     0,   267,
     268,   269,     0,     0,     0,   655,   270,     0,   579,     0,
       0,    33,    34,    35,    36,    37,    38,     0,   641,   273,
     274,     0,   275,   276,   277,   646,     0,     0,     0,   278,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,     0,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,     0,     0,   633,     0,   272,
       0,     0,     0,     0,     0,   103,   273,   274,   104,   275,
     276,   277,     0,   256,     0,   654,   278,     0,   105,   106,
     257,   258,     0,   259,   260,   261,   264,     0,     0,     0,
     262,     0,   580,   265,   266,     0,   267,   268,   269,     0,
     640,   256,     0,   270,     0,   581,     0,   645,   257,   258,
       0,   259,   260,   261,   256,     0,     0,     0,   262,     0,
     582,   257,   258,     0,   259,   260,   261,   264,     0,     0,
       0,   262,     0,   583,   265,   266,     0,   267,   268,   269,
     272,     0,     0,     0,   270,     0,   584,   273,   274,     0,
     275,   276,   277,   256,     0,     0,     0,   278,     0,   585,
     257,   258,     0,   259,   260,   261,   264,     0,     0,     0,
     262,     0,   586,   265,   266,     0,   267,   268,   269,   256,
       0,     0,     0,   270,     0,   587,   257,   258,     0,   259,
     260,   261,   264,     0,     0,     0,   262,   653,   588,   265,
     266,     0,   267,   268,   269,   272,     0,     0,     0,   270,
       0,   589,   273,   274,     0,   275,   276,   277,   256,     0,
       0,     0,   278,     0,   590,   257,   258,     0,   259,   260,
     261,   264,     0,     0,     0,   262,     0,   591,   265,   266,
       0,   267,   268,   269,   272,     0,     0,     0,   270,     0,
     592,   273,   274,     0,   275,   276,   277,   256,     0,     0,
       0,   278,     0,   593,   257,   258,     0,   259,   260,   261,
     264,     0,     0,     0,   262,     0,   594,   265,   266,     0,
     267,   268,   269,   256,     0,     0,     0,   270,     0,   595,
     257,   258,     0,   259,   260,   261,   264,     0,     0,     0,
     262,     0,   596,   265,   266,     0,   267,   268,   269,   256,
       0,     0,     0,   270,     0,   597,   257,   258,     0,   259,
     260,   261,   264,     0,     0,     0,   262,     0,   598,   265,
     266,     0,   267,   268,   269,   256,     0,     0,     0,   270,
       0,   599,   257,   258,     0,   259,   260,   261,   264,     0,
       0,     0,   262,     0,   600,   265,   266,     0,   267,   268,
     269,   256,     0,     0,     0,   270,     0,   601,   257,   258,
       0,   259,   260,   261,   264,     0,     0,     0,   262,     0,
     662,   265,   266,     0,   267,   268,   269,   272,     0,     0,
       0,   270,     0,   663,   273,   274,     0,   275,   276,   277,
     256,     0,     0,     0,   278,     0,   664,   257,   258,     0,
     259,   260,   261,   264,     0,     0,     0,   262,     0,   667,
     265,   266,     0,   267,   268,   269,   272,     0,     0,     0,
     270,     0,   668,   273,   274,     0,   275,   276,   277,     0,
       0,     0,     0,   278,     0,   669,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,     0,     0,
       0,     0,     0,   539,   540,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,     0,     0,     0,
       0,     0,     0,   285,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,     0,     0,     0,     0,
       0,     0,   413,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,     0,     0,     0,     0,     0,
       0,   500,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,     0,     0,     0,     0,     0,     0,
     504,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,     0,     0,     0,     0,     0,     0,   515,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,     0,     0,     0,     0,     0,     0,   516,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,     0,     0,     0,     0,     0,     0,   518,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
       0,     0,     0,     0,     0,     0,   523,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,     0,
       0,     0,     0,     0,     0,   527,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,     0,     0,
       0,     0,     0,     0,   528,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,     0,     0,     0,
       0,     0,     0,   529,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,     0,     0,     0,     0,
       0,     0,   544,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,     0,     0,     0,     0,     0,
       0,   545,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,     0,     0,     0,     0,     0,     0,
     547,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,     0,     0,     0,     0,     0,     0,   549,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,     0,     0,     0,     0,     0,     0,   551,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,     0,     0,     0,     0,     0,     0,   552,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
       0,     0,     0,     0,     0,     0,   553,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,     0,
       0,     0,     0,     0,     0,   554,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,     0,     0,
       0,     0,     0,     0,   555,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,     0,     0,     0,
       0,     0,     0,   556,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,     0,     0,     0,     0,
       0,     0,   557,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,     0,     0,     0,     0,     0,
       0,   558,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,     0,     0,     0,     0,     0,     0,
     559,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,     0,     0,     0,     0,     0,     0,   560,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,     0,     0,     0,     0,     0,     0,   561,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,     0,     0,     0,     0,     0,     0,   562,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
       0,     0,     0,     0,     0,     0,   563,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,     0,
       0,     0,     0,     0,     0,   564,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,     0,     0,
       0,     0,     0,     0,   565,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,     0,     0,     0,
       0,     0,     0,   566,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,     0,     0,     0,     0,
       0,     0,   567,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,     0,     0,     0,     0,     0,
       0,   568,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,     0,     0,     0,     0,     0,     0,
     569,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,     0,     0,     0,     0,     0,     0,   570,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,     0,     0,     0,     0,     0,     0,   571,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,     0,     0,     0,     0,     0,     0,   572,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
       0,     0,     0,     0,     0,     0,   659,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,     0,
       0,     0,     0,     0,     0,   661,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,     0,     0,
       0,     0,     0,     0,   666,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,     0,     0,     0,
       0,     0,     0,   676,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,     0,     0,     0,     0,
       0,     0,   690,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,   256,     0,     0,     0,     0,
       0,   696,   257,   258,     0,   259,   260,   261,   264,     0,
       0,     0,   262,   288,     0,   265,   266,     0,   267,   268,
     269,   272,     0,     0,     0,   270,   290,     0,   273,   274,
       0,   275,   276,   277,   256,     0,     0,     0,   278,   292,
       0,   257,   258,     0,   259,   260,   261,   264,     0,     0,
       0,   262,   534,     0,   265,   266,     0,   267,   268,   269,
     272,     0,     0,     0,   270,   535,     0,   273,   274,     0,
     275,   276,   277,   256,     0,     0,     0,   278,   536,     0,
     257,   258,     0,   259,   260,   261,   264,     0,     0,     0,
     262,   541,     0,   265,   266,     0,   267,   268,   269,   272,
       0,     0,     0,   270,   542,     0,   273,   274,     0,   275,
     276,   277,     0,     0,     0,     0,   278,   543,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
       0,     0,     0,     0,     0,   284,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,     0,     0,
       0,     0,     0,   501,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,     0,     0,     0,     0,
       0,   502,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,     0,     0,     0,     0,     0,   503,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,     0,     0,     0,     0,     0,   514,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
       0,     0,     0,     0,     0,   656,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,     0,     0,
       0,     0,     0,   657,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,     0,     0,     0,     0,
       0,   658,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,     0,     0,     0,     0,     0,   677,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,     0,     0,     0,     0,     0,   678,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
       0,     0,     0,     0,     0,   681,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,     0,     0,
       0,     0,     0,   682,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,     0,     0,     0,     0,
       0,   685,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,     0,     0,     0,     0,     0,   686,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,     0,     0,     0,     0,     0,   689,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
       0,     0,     0,     0,     0,   692,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   256,     0,     0,
       0,     0,     0,   694,   257,   258,     0,   259,   260,   261,
     264,     0,     0,     0,   262,     0,     0,   265,   266,     0,
     267,   268,   269,   272,     0,     0,     0,   270,     0,     0,
     273,   274,     0,   275,   276,   277,     0,     0,     0,     0,
     278,     1,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,   630,     0,     0,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     234,   235,     0,   236,   237,   238,   239,     0,     0,     0,
     240,   530,   531,   234,   235,     0,   236,   237,   238,   239,
       0,     0,     0,   240,   537,   538,   234,   235,     0,   236,
     237,   238,   239,     0,     0,     0,   240,     0,   287,   234,
     235,     0,   236,   237,   238,   239,     0,     0,     0,   240,
       0,   412,   234,   235,     0,   236,   237,   238,   239,     0,
       0,     0,   240,     0,   517,   234,   235,     0,   236,   237,
     238,   239,     0,     0,     0,   240,     0,   522,   234,   235,
       0,   236,   237,   238,   239,     0,     0,     0,   240,     0,
     546,   234,   235,     0,   236,   237,   238,   239,     0,     0,
       0,   240,     0,   548,   234,   235,     0,   236,   237,   238,
     239,     0,     0,     0,   240,     0,   550,   234,   235,     0,
     236,   237,   238,   239,     0,     0,     0,   240,     0,   660,
     234,   235,     0,   236,   237,   238,   239,     0,     0,     0,
     240,     0,   665,   234,   235,     0,   236,   237,   238,   239,
       0,     0,     0,   240,   286,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   234,   235,     0,   236,
     237,   238,   239,     0,     0,     0,   240
  };

  /* YYCHECK.  */
  const short int
  CloudValueExpressionParser::yycheck_[] =
  {
         1,     2,     3,     1,    25,    25,    25,    25,    68,   270,
     134,   156,   136,   137,    12,    13,   113,   278,   115,   155,
     117,     0,   119,   157,   121,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    59,    60,    61,
     157,    63,    64,   157,   134,    67,   136,   137,   134,   157,
     136,   137,   134,   157,   136,   137,   116,   137,   157,     1,
      48,    49,     4,     5,   144,   145,   156,   147,   148,   149,
     156,   137,   157,   155,   154,   156,   156,   156,   144,   145,
     157,   147,   148,   149,   157,   116,   135,   157,   154,   157,
     156,    56,    57,    58,   116,   144,   145,   156,   147,   148,
     149,   150,   103,     1,   105,   154,   157,   105,   106,   137,
     157,   157,    10,    11,   157,   157,   144,   145,   157,   147,
     148,   149,   137,   146,   147,   148,   154,   157,   156,   144,
     145,   157,   147,   148,   149,   156,   156,   156,   156,   154,
     137,   156,   147,   148,   149,   242,   147,   144,   145,   154,
     147,   148,   149,   154,   155,   156,   157,   154,   157,   156,
     156,   103,   157,   105,   157,   157,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   147,   148,   149,   103,   157,   105,   137,   154,
     147,   148,   149,   150,     1,   144,   145,   154,   147,   148,
     149,     8,     9,   157,   157,   154,   157,   156,   170,   171,
     157,   157,   157,   175,   176,   236,   237,   179,   180,   181,
     157,   157,   157,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   157,   147,   148,   149,   259,   260,
     144,   145,   154,   147,   148,   149,   267,   268,   157,   157,
     154,   157,   170,   171,   275,   276,   157,   175,   176,   280,
     157,   157,   157,   281,   282,   157,   157,   157,   157,   135,
     157,   137,   234,   235,   236,   157,   238,   239,   144,   145,
     157,   147,   148,   149,   157,   157,   204,   157,   154,   157,
     208,   253,   210,   211,   137,   157,   103,   104,   105,   261,
     157,   144,   145,   157,   147,   148,   149,   269,   157,   157,
     157,   154,   157,   156,   156,   277,   157,     1,   280,   157,
     238,   157,     6,     7,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   253,   157,   157,   256,   257,
     258,   155,   156,   261,   157,   157,   264,   265,   266,   157,
     156,   269,   157,   157,   272,   273,   274,   157,   157,   277,
     157,   157,   280,   170,   171,   137,    22,   154,   175,   176,
      23,   154,   144,   145,   154,   147,   148,   149,   154,   156,
     137,   135,   154,   137,   156,    11,    12,    13,    14,    15,
     144,   145,   156,   147,   148,   149,   203,   204,   205,   206,
     154,   208,   209,   210,   211,   212,   213,   214,   215,   149,
     150,   151,   152,   153,   156,   137,   157,   156,   156,   103,
     104,   105,   144,   145,   156,   147,   148,   149,   137,   156,
     156,   238,   154,   156,   156,   144,   145,   156,   147,   148,
     149,   156,    -1,   157,   157,   154,   253,   156,   156,   256,
     257,   258,   156,   156,   261,    -1,    -1,   264,   265,   266,
      -1,    -1,   269,    -1,    -1,   272,   273,   274,    -1,    -1,
     277,    -1,    -1,   280,   144,   145,    -1,   147,   148,   149,
     501,   502,   503,    -1,   154,    -1,   170,   171,   137,    -1,
      -1,   175,   176,   514,    -1,   144,   145,    -1,   147,   148,
     149,    -1,    -1,    -1,    -1,   154,    -1,   156,    -1,    -1,
      -1,   532,    -1,    -1,    -1,    -1,    -1,    -1,   539,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   137,    -1,    -1,    -1,    -1,    -1,    -1,   144,
     145,    -1,   147,   148,   149,   137,    -1,    -1,    -1,   154,
      -1,   156,   144,   145,   238,   147,   148,   149,    -1,    -1,
      -1,    -1,   154,    -1,   156,    -1,    -1,    -1,   530,   253,
      -1,    -1,   256,   257,   258,   537,    -1,   261,    -1,    -1,
     264,   265,   266,    -1,    -1,   269,    -1,   137,   272,   273,
     274,    -1,    -1,   277,   144,   145,   280,   147,   148,   149,
     137,    -1,    -1,    -1,   154,    -1,   156,   144,   145,   630,
     147,   148,   149,    -1,    -1,    -1,    -1,   154,   536,   156,
      -1,   137,    -1,    -1,    -1,   543,    -1,    -1,   144,   145,
      -1,   147,   148,   149,   137,   656,   657,   658,   154,    -1,
     156,   144,   145,    -1,   147,   148,   149,    -1,    -1,    -1,
      -1,   154,    -1,   156,    -1,    -1,   677,   678,    -1,    -1,
     681,   682,    -1,    -1,   685,   686,    -1,   629,   689,    -1,
      -1,   692,    -1,   694,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    -1,   137,    16,    17,    18,
      19,    20,    21,   144,   145,    24,   147,   148,   149,   137,
      -1,    -1,    -1,   154,    -1,   156,   144,   145,    -1,   147,
     148,   149,    -1,    -1,    -1,   633,   154,    -1,   156,    -1,
      -1,    50,    51,    52,    53,    54,    55,    -1,   535,   144,
     145,    -1,   147,   148,   149,   542,    -1,    -1,    -1,   154,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,    -1,    -1,   135,    -1,   137,
      -1,    -1,    -1,    -1,    -1,   144,   144,   145,   147,   147,
     148,   149,    -1,   137,    -1,   632,   154,    -1,   157,   158,
     144,   145,    -1,   147,   148,   149,   137,    -1,    -1,    -1,
     154,    -1,   156,   144,   145,    -1,   147,   148,   149,    -1,
     534,   137,    -1,   154,    -1,   156,    -1,   541,   144,   145,
      -1,   147,   148,   149,   137,    -1,    -1,    -1,   154,    -1,
     156,   144,   145,    -1,   147,   148,   149,   137,    -1,    -1,
      -1,   154,    -1,   156,   144,   145,    -1,   147,   148,   149,
     137,    -1,    -1,    -1,   154,    -1,   156,   144,   145,    -1,
     147,   148,   149,   137,    -1,    -1,    -1,   154,    -1,   156,
     144,   145,    -1,   147,   148,   149,   137,    -1,    -1,    -1,
     154,    -1,   156,   144,   145,    -1,   147,   148,   149,   137,
      -1,    -1,    -1,   154,    -1,   156,   144,   145,    -1,   147,
     148,   149,   137,    -1,    -1,    -1,   154,   631,   156,   144,
     145,    -1,   147,   148,   149,   137,    -1,    -1,    -1,   154,
      -1,   156,   144,   145,    -1,   147,   148,   149,   137,    -1,
      -1,    -1,   154,    -1,   156,   144,   145,    -1,   147,   148,
     149,   137,    -1,    -1,    -1,   154,    -1,   156,   144,   145,
      -1,   147,   148,   149,   137,    -1,    -1,    -1,   154,    -1,
     156,   144,   145,    -1,   147,   148,   149,   137,    -1,    -1,
      -1,   154,    -1,   156,   144,   145,    -1,   147,   148,   149,
     137,    -1,    -1,    -1,   154,    -1,   156,   144,   145,    -1,
     147,   148,   149,   137,    -1,    -1,    -1,   154,    -1,   156,
     144,   145,    -1,   147,   148,   149,   137,    -1,    -1,    -1,
     154,    -1,   156,   144,   145,    -1,   147,   148,   149,   137,
      -1,    -1,    -1,   154,    -1,   156,   144,   145,    -1,   147,
     148,   149,   137,    -1,    -1,    -1,   154,    -1,   156,   144,
     145,    -1,   147,   148,   149,   137,    -1,    -1,    -1,   154,
      -1,   156,   144,   145,    -1,   147,   148,   149,   137,    -1,
      -1,    -1,   154,    -1,   156,   144,   145,    -1,   147,   148,
     149,   137,    -1,    -1,    -1,   154,    -1,   156,   144,   145,
      -1,   147,   148,   149,   137,    -1,    -1,    -1,   154,    -1,
     156,   144,   145,    -1,   147,   148,   149,   137,    -1,    -1,
      -1,   154,    -1,   156,   144,   145,    -1,   147,   148,   149,
     137,    -1,    -1,    -1,   154,    -1,   156,   144,   145,    -1,
     147,   148,   149,   137,    -1,    -1,    -1,   154,    -1,   156,
     144,   145,    -1,   147,   148,   149,   137,    -1,    -1,    -1,
     154,    -1,   156,   144,   145,    -1,   147,   148,   149,    -1,
      -1,    -1,    -1,   154,    -1,   156,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    -1,
      -1,    -1,    -1,   155,   156,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    -1,   137,    -1,    -1,    -1,    -1,
      -1,   156,   144,   145,    -1,   147,   148,   149,   137,    -1,
      -1,    -1,   154,   155,    -1,   144,   145,    -1,   147,   148,
     149,   137,    -1,    -1,    -1,   154,   155,    -1,   144,   145,
      -1,   147,   148,   149,   137,    -1,    -1,    -1,   154,   155,
      -1,   144,   145,    -1,   147,   148,   149,   137,    -1,    -1,
      -1,   154,   155,    -1,   144,   145,    -1,   147,   148,   149,
     137,    -1,    -1,    -1,   154,   155,    -1,   144,   145,    -1,
     147,   148,   149,   137,    -1,    -1,    -1,   154,   155,    -1,
     144,   145,    -1,   147,   148,   149,   137,    -1,    -1,    -1,
     154,   155,    -1,   144,   145,    -1,   147,   148,   149,   137,
      -1,    -1,    -1,   154,   155,    -1,   144,   145,    -1,   147,
     148,   149,    -1,    -1,    -1,    -1,   154,   155,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    -1,
      -1,    -1,    -1,   155,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,   155,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    -1,
      -1,    -1,    -1,   155,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,   155,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,    -1,    -1,
      -1,    -1,    -1,   155,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,   155,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,    -1,    -1,    -1,    -1,    -1,   155,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   137,    -1,    -1,
      -1,    -1,    -1,   155,   144,   145,    -1,   147,   148,   149,
     137,    -1,    -1,    -1,   154,    -1,    -1,   144,   145,    -1,
     147,   148,   149,   137,    -1,    -1,    -1,   154,    -1,    -1,
     144,   145,    -1,   147,   148,   149,    -1,    -1,    -1,    -1,
     154,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,   135,    -1,    -1,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     144,   145,    -1,   147,   148,   149,   150,    -1,    -1,    -1,
     154,   155,   156,   144,   145,    -1,   147,   148,   149,   150,
      -1,    -1,    -1,   154,   155,   156,   144,   145,    -1,   147,
     148,   149,   150,    -1,    -1,    -1,   154,    -1,   156,   144,
     145,    -1,   147,   148,   149,   150,    -1,    -1,    -1,   154,
      -1,   156,   144,   145,    -1,   147,   148,   149,   150,    -1,
      -1,    -1,   154,    -1,   156,   144,   145,    -1,   147,   148,
     149,   150,    -1,    -1,    -1,   154,    -1,   156,   144,   145,
      -1,   147,   148,   149,   150,    -1,    -1,    -1,   154,    -1,
     156,   144,   145,    -1,   147,   148,   149,   150,    -1,    -1,
      -1,   154,    -1,   156,   144,   145,    -1,   147,   148,   149,
     150,    -1,    -1,    -1,   154,    -1,   156,   144,   145,    -1,
     147,   148,   149,   150,    -1,    -1,    -1,   154,    -1,   156,
     144,   145,    -1,   147,   148,   149,   150,    -1,    -1,    -1,
     154,    -1,   156,   144,   145,    -1,   147,   148,   149,   150,
      -1,    -1,    -1,   154,   155,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   144,   145,    -1,   147,
     148,   149,   150,    -1,    -1,    -1,   154
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  CloudValueExpressionParser::yystos_[] =
  {
         0,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,   160,   161,     3,     4,
       5,     6,     7,     8,     9,    10,    16,    17,    18,    19,
      20,    21,    24,    50,    51,    52,    53,    54,    55,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   144,   147,   157,   158,   163,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   167,   171,   173,   175,
     177,   179,   171,   167,   171,   167,   173,   173,   175,   175,
     177,   177,   179,   179,   156,   155,     0,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     167,   171,   173,   175,   177,   173,   175,   167,   171,   173,
     175,   177,   179,   179,   144,   145,   147,   148,   149,   150,
     154,    48,    49,   162,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   162,   137,   144,   145,   147,
     148,   149,   154,   162,   137,   144,   145,   147,   148,   149,
     154,   162,   137,   144,   145,   147,   148,   149,   154,   162,
     134,   136,   137,   162,   155,   156,   155,   156,   155,   156,
     155,   156,   155,   156,   155,   156,   171,   166,   166,   166,
     166,   166,   166,   171,   171,   171,   171,    11,    12,    13,
      14,    15,    25,   156,    25,   156,   156,    25,   156,    25,
     156,   156,   156,   156,   171,   171,   171,   167,   171,   173,
     175,   177,   167,   171,   173,   175,   177,   171,   171,   171,
     167,   171,   173,   175,   177,   167,   171,   173,   175,   177,
     171,   171,   167,   171,   167,   171,   167,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   173,
     175,   173,   175,   177,   173,   175,   173,   175,   173,   173,
     175,   177,   173,   175,   173,   175,   177,   173,   175,   177,
     173,   175,   173,   175,   173,   175,   173,   175,   156,   156,
      22,    23,   156,   156,   156,   156,   156,   156,   167,   167,
     167,   171,   171,   167,   173,   175,   177,   167,   164,   162,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   167,
     171,   173,   175,   177,   171,   173,   175,   177,   173,   175,
     177,   173,   175,   177,   171,   171,   167,   173,   175,   177,
     165,   173,   175,   177,   173,   175,   177,   173,   175,   177,
     171,   171,   167,   173,   175,   177,   165,   173,   175,   177,
     173,   175,   177,   173,   175,   177,   171,   171,   167,   173,
     175,   177,   165,   167,   171,   173,   175,   177,   179,   179,
     156,   155,   155,   155,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   155,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     155,   156,   155,   156,   155,   155,   155,   155,   156,   155,
     156,   155,   155,   155,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,    56,    57,    58,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,   116,
      59,    60,    61,    63,    64,    67,   116,    68,   116,   135,
     135,   135,   135,   135,   171,   171,   171,   171,   167,   171,
     173,   175,   177,   167,   171,   173,   175,   177,   157,   157,
     157,   167,   171,   173,   175,   177,   155,   155,   155,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   171,   171,   171,   156,   155,   155,   171,
     171,   155,   155,   171,   171,   155,   155,   171,   171,   155,
     156,   171,   155,   171,   155,   171,   156
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  CloudValueExpressionParser::yytoken_number_[] =
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
     385,   386,   387,   388,    63,    58,   389,   390,   391,   392,
      60,    62,   393,   394,    45,    43,    37,    42,    47,    38,
      94,   395,   396,   397,    46,    44,    41,    40,    33
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  CloudValueExpressionParser::yyr1_[] =
  {
         0,   159,   160,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   162,   162,
     163,   163,   163,   163,   163,   163,   164,   165,   166,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   168,   169,   169,   169,   169,   170,   170,   170,   170,
     170,   170,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   172,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   174,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   176,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   178,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   180,   181,   182,   183,   184
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  CloudValueExpressionParser::yyr2_[] =
  {
         0,     2,     1,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     0,     0,     0,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     3,     4,     4,     4,
       4,     4,     4,     4,     5,     3,     2,     1,     6,     6,
       4,     3,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     1,     1,     3,     3,     3,     3,     3,     6,
       4,     4,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     5,     1,     3,     3,     3,     3,
       4,     3,     4,     3,     4,     3,     4,     3,     3,     2,
       1,     1,     4,     6,     6,     4,     3,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     3,     4,
       4,     4,     4,     4,     4,     4,     6,     5,     2,     1,
       6,     6,     4,     3,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     3,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     6,     5,
       2,     1,     6,     6,     4,     3,     1,     1,     3,     3,
       3,     3,     3,     3,     2,     3,     4,     4,     4,     4,
       6,     5,     2,     1,     6,     6,     4,     3,     1,     1,
       4,     4,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     1,     3,     8,    20,    14,     4
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const CloudValueExpressionParser::yytname_[] =
  {
    "$end", "error", "$undefined", "\"timeline\"", "\"lookup\"",
  "\"scalarID\"", "\"vectorID\"", "\"logicalID\"", "\"tensorID\"",
  "\"symmTensorID\"", "\"sphericalTensorID\"", "\"G_scalarID\"",
  "\"G_vectorID\"", "\"G_tensorID\"", "\"G_symmTensorID\"",
  "\"G_sphericalTensorID\"", "\"F_scalarID\"", "\"F_vectorID\"",
  "\"F_tensorID\"", "\"F_symmTensorID\"", "\"F_sphericalTensorID\"",
  "\"F_logicalID\"", "\"cellSetID\"", "\"cellZoneID\"", "\"value\"",
  "\"integer\"", "\"vector\"", "\"expression\"", "\"lexpression\"",
  "\"vexpression\"", "\"texpression\"", "\"yexpression\"",
  "\"hexpression\"", "START_DEFAULT", "START_CLOUD_SCALAR_COMMA",
  "START_CLOUD_SCALAR_CLOSE", "START_CLOUD_VECTOR_COMMA",
  "START_CLOUD_VECTOR_CLOSE", "START_CLOUD_TENSOR_COMMA",
  "START_CLOUD_TENSOR_CLOSE", "START_CLOUD_YTENSOR_COMMA",
  "START_CLOUD_YTENSOR_CLOSE", "START_CLOUD_HTENSOR_COMMA",
  "START_CLOUD_HTENSOR_CLOSE", "START_CLOUD_LOGICAL_COMMA",
  "START_CLOUD_LOGICAL_CLOSE", "START_CLOSE_ONLY", "START_COMMA_ONLY",
  "TOKEN_LAST_FUNCTION_CHAR", "TOKEN_IN_FUNCTION_CHAR", "TOKEN_VECTOR",
  "TOKEN_TENSOR", "TOKEN_SYMM_TENSOR", "TOKEN_SPHERICAL_TENSOR",
  "TOKEN_TRUE", "TOKEN_FALSE", "TOKEN_x", "TOKEN_y", "TOKEN_z", "TOKEN_xx",
  "TOKEN_xy", "TOKEN_xz", "TOKEN_yx", "TOKEN_yy", "TOKEN_yz", "TOKEN_zx",
  "TOKEN_zy", "TOKEN_zz", "TOKEN_ii", "TOKEN_fluidPhase",
  "TOKEN_unitTensor", "TOKEN_pi", "TOKEN_rand", "TOKEN_randFixed",
  "TOKEN_id", "TOKEN_randNormal", "TOKEN_randNormalFixed",
  "TOKEN_position", "TOKEN_deltaT", "TOKEN_time", "TOKEN_pow", "TOKEN_log",
  "TOKEN_exp", "TOKEN_mag", "TOKEN_magSqr", "TOKEN_sin", "TOKEN_cos",
  "TOKEN_tan", "TOKEN_min", "TOKEN_max", "TOKEN_minPosition",
  "TOKEN_maxPosition", "TOKEN_average", "TOKEN_sum", "TOKEN_sqr",
  "TOKEN_sqrt", "TOKEN_log10", "TOKEN_asin", "TOKEN_acos", "TOKEN_atan",
  "TOKEN_sinh", "TOKEN_cosh", "TOKEN_tanh", "TOKEN_asinh", "TOKEN_acosh",
  "TOKEN_atanh", "TOKEN_erf", "TOKEN_erfc", "TOKEN_lgamma",
  "TOKEN_besselJ0", "TOKEN_besselJ1", "TOKEN_besselY0", "TOKEN_besselY1",
  "TOKEN_sign", "TOKEN_pos", "TOKEN_neg", "TOKEN_transpose", "TOKEN_diag",
  "TOKEN_tr", "TOKEN_dev", "TOKEN_symm", "TOKEN_skew", "TOKEN_det",
  "TOKEN_cof", "TOKEN_inv", "TOKEN_sph", "TOKEN_twoSymm", "TOKEN_dev2",
  "TOKEN_eigenValues", "TOKEN_eigenVectors", "TOKEN_cpu", "TOKEN_weight",
  "TOKEN_set", "TOKEN_zone", "'?'", "':'", "TOKEN_OR", "TOKEN_AND",
  "TOKEN_NEQ", "TOKEN_EQ", "'<'", "'>'", "TOKEN_GEQ", "TOKEN_LEQ", "'-'",
  "'+'", "'%'", "'*'", "'/'", "'&'", "'^'", "TOKEN_HODGE", "TOKEN_NOT",
  "TOKEN_NEG", "'.'", "','", "')'", "'('", "'!'", "$accept",
  "switch_start", "switch_expr", "restOfFunction", "unit",
  "vectorComponentSwitch", "tensorComponentSwitch", "eatCharactersSwitch",
  "vexp", "evaluateVectorFunction", "sreduced", "vreduced", "exp",
  "evaluateScalarFunction", "texp", "evaluateTensorFunction", "yexp",
  "evaluateSymmTensorFunction", "hexp", "evaluateSphericalTensorFunction",
  "lexp", "evaluateLogicalFunction", "vector", "tensor", "symmTensor",
  "sphericalTensor", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const CloudValueExpressionParser::rhs_number_type
  CloudValueExpressionParser::yyrhs_[] =
  {
       160,     0,    -1,   161,    -1,    33,   163,    -1,    34,   171,
     155,    -1,    35,   171,   156,    -1,    36,   167,   155,    -1,
      37,   167,   156,    -1,    38,   173,   155,    -1,    39,   173,
     156,    -1,    40,   175,   155,    -1,    41,   175,   156,    -1,
      42,   177,   155,    -1,    43,   177,   156,    -1,    44,   179,
     155,    -1,    45,   179,   156,    -1,    46,   156,    -1,    47,
     155,    -1,    48,    -1,    49,   162,    -1,   171,    -1,   167,
      -1,   179,    -1,   173,    -1,   175,    -1,   177,    -1,    -1,
      -1,    -1,   181,    -1,   170,    -1,   167,   145,   167,    -1,
     171,   147,   167,    -1,   167,   147,   171,    -1,   173,   149,
     167,    -1,   167,   149,   173,    -1,   175,   149,   167,    -1,
     167,   149,   175,    -1,   177,   149,   167,    -1,   167,   149,
     177,    -1,   167,   148,   171,    -1,   167,   150,   167,    -1,
     167,   144,   167,    -1,   144,   167,    -1,   147,   173,    -1,
     147,   175,    -1,   157,   167,   156,    -1,   128,   157,   173,
     156,    -1,   128,   157,   175,   156,    -1,   173,   154,   165,
      56,    -1,   173,   154,   165,    57,    -1,   173,   154,   165,
      58,    -1,   117,   157,   173,   156,    -1,   117,   157,   175,
     156,    -1,   179,   134,   167,   135,   167,    -1,    77,   157,
     156,    -1,   168,   162,    -1,     6,    -1,    88,   157,   167,
     155,   167,   156,    -1,    89,   157,   167,   155,   167,   156,
      -1,    69,   157,    12,   156,    -1,    17,   157,   166,    -1,
      88,   157,   171,   156,    -1,    89,   157,   171,   156,    -1,
      93,   157,   171,   156,    -1,    92,   157,   171,   156,    -1,
      88,   157,   167,   156,    -1,    89,   157,   167,   156,    -1,
      90,   157,   171,   156,    -1,    91,   157,   171,   156,    -1,
      93,   157,   167,   156,    -1,    92,   157,   167,   156,    -1,
      24,    -1,   169,    -1,   171,   145,   171,    -1,   171,   144,
     171,    -1,   171,   147,   171,    -1,   171,   146,   171,    -1,
     171,   148,   171,    -1,    80,   157,   171,   155,   171,   156,
      -1,    81,   157,   171,   156,    -1,    82,   157,   171,   156,
      -1,   167,   149,   167,    -1,   173,   137,   173,    -1,   173,
     137,   175,    -1,   173,   137,   177,    -1,   175,   137,   173,
      -1,   175,   137,   175,    -1,   175,   137,   177,    -1,   177,
     137,   173,    -1,   177,   137,   175,    -1,   177,   137,   177,
      -1,   144,   171,    -1,   157,   171,   156,    -1,    94,   157,
     171,   156,    -1,    95,   157,   171,   156,    -1,    85,   157,
     171,   156,    -1,    86,   157,   171,   156,    -1,    87,   157,
     171,   156,    -1,    96,   157,   171,   156,    -1,    97,   157,
     171,   156,    -1,    98,   157,   171,   156,    -1,    99,   157,
     171,   156,    -1,   100,   157,   171,   156,    -1,   101,   157,
     171,   156,    -1,   102,   157,   171,   156,    -1,   103,   157,
     171,   156,    -1,   104,   157,   171,   156,    -1,   105,   157,
     171,   156,    -1,   106,   157,   171,   156,    -1,   107,   157,
     171,   156,    -1,   108,   157,   171,   156,    -1,   109,   157,
     171,   156,    -1,   110,   157,   171,   156,    -1,   111,   157,
     171,   156,    -1,   112,   157,   171,   156,    -1,   113,   157,
     171,   156,    -1,   114,   157,   171,   156,    -1,   115,   157,
     171,   156,    -1,    83,   157,   171,   156,    -1,    83,   157,
     167,   156,    -1,    83,   157,   173,   156,    -1,    83,   157,
     175,   156,    -1,    83,   157,   177,   156,    -1,    84,   157,
     171,   156,    -1,    84,   157,   167,   156,    -1,    84,   157,
     173,   156,    -1,    84,   157,   175,   156,    -1,    84,   157,
     177,   156,    -1,   118,   157,   173,   156,    -1,   118,   157,
     175,   156,    -1,   118,   157,   177,   156,    -1,   122,   157,
     173,   156,    -1,   122,   157,   175,   156,    -1,   122,   157,
     177,   156,    -1,   167,   154,   164,    56,    -1,   167,   154,
     164,    57,    -1,   167,   154,   164,    58,    -1,   173,   154,
     165,    59,    -1,   173,   154,   165,    60,    -1,   173,   154,
     165,    61,    -1,   173,   154,   165,    62,    -1,   173,   154,
     165,    63,    -1,   173,   154,   165,    64,    -1,   173,   154,
     165,    65,    -1,   173,   154,   165,    66,    -1,   173,   154,
     165,    67,    -1,   175,   154,   165,    59,    -1,   175,   154,
     165,    60,    -1,   175,   154,   165,    61,    -1,   175,   154,
     165,    63,    -1,   175,   154,   165,    64,    -1,   175,   154,
     165,    67,    -1,   177,   154,   165,    68,    -1,   179,   134,
     171,   135,   171,    -1,    71,    -1,    74,   157,   156,    -1,
     130,   157,   156,    -1,   131,   157,   156,    -1,    72,   157,
     156,    -1,    72,   157,    25,   156,    -1,    75,   157,   156,
      -1,    75,   157,    25,   156,    -1,    73,   157,   156,    -1,
      73,   157,    25,   156,    -1,    76,   157,   156,    -1,    76,
     157,    25,   156,    -1,    78,   157,   156,    -1,    79,   157,
     156,    -1,   172,   162,    -1,     5,    -1,     3,    -1,     4,
     157,   171,   156,    -1,    88,   157,   171,   155,   171,   156,
      -1,    89,   157,   171,   155,   171,   156,    -1,    69,   157,
      11,   156,    -1,    16,   157,   166,    -1,   182,    -1,   173,
     145,   173,    -1,   173,   145,   175,    -1,   173,   145,   177,
      -1,   175,   145,   173,    -1,   177,   145,   173,    -1,   171,
     147,   173,    -1,   173,   147,   171,    -1,   167,   147,   167,
      -1,   173,   149,   173,    -1,   175,   149,   173,    -1,   173,
     149,   175,    -1,   177,   149,   173,    -1,   173,   149,   177,
      -1,   173,   148,   171,    -1,   173,   144,   173,    -1,   173,
     144,   175,    -1,   173,   144,   177,    -1,   175,   144,   173,
      -1,   177,   144,   173,    -1,   144,   173,    -1,   157,   173,
     156,    -1,   121,   157,   173,   156,    -1,   129,   157,   173,
     156,    -1,   129,   157,   175,   156,    -1,   124,   157,   173,
     156,    -1,   123,   157,   173,   156,    -1,   119,   157,   173,
     156,    -1,   127,   157,   173,   156,    -1,   173,   154,   165,
     116,   157,   156,    -1,   179,   134,   173,   135,   173,    -1,
     174,   162,    -1,     8,    -1,    88,   157,   173,   155,   173,
     156,    -1,    89,   157,   173,   155,   173,   156,    -1,    69,
     157,    13,   156,    -1,    18,   157,   166,    -1,   183,    -1,
     175,   145,   175,    -1,   175,   145,   177,    -1,   177,   145,
     175,    -1,   171,   147,   175,    -1,   175,   147,   171,    -1,
     175,   149,   175,    -1,   177,   149,   175,    -1,   175,   149,
     177,    -1,   175,   148,   171,    -1,   175,   144,   175,    -1,
     175,   144,   177,    -1,   177,   144,   175,    -1,   144,   175,
      -1,   157,   175,   156,    -1,   120,   157,   175,   156,    -1,
     120,   157,   173,   156,    -1,   126,   157,   175,   156,    -1,
     126,   157,   173,   156,    -1,   124,   157,   175,   156,    -1,
     123,   157,   175,   156,    -1,   119,   157,   175,   156,    -1,
     127,   157,   175,   156,    -1,    94,   157,   167,   156,    -1,
     175,   154,   165,   116,   157,   156,    -1,   179,   134,   175,
     135,   175,    -1,   176,   162,    -1,     9,    -1,    88,   157,
     175,   155,   175,   156,    -1,    89,   157,   175,   155,   175,
     156,    -1,    69,   157,    14,   156,    -1,    19,   157,   166,
      -1,   184,    -1,    70,    -1,   177,   145,   177,    -1,   171,
     147,   177,    -1,   177,   147,   171,    -1,   177,   149,   177,
      -1,   177,   148,   171,    -1,   177,   144,   177,    -1,   144,
     177,    -1,   157,   177,   156,    -1,   125,   157,   173,   156,
      -1,   125,   157,   175,   156,    -1,   125,   157,   177,   156,
      -1,   124,   157,   177,   156,    -1,   177,   154,   165,   116,
     157,   156,    -1,   179,   134,   177,   135,   177,    -1,   178,
     162,    -1,    10,    -1,    88,   157,   177,   155,   177,   156,
      -1,    89,   157,   177,   155,   177,   156,    -1,    69,   157,
      15,   156,    -1,    20,   157,   166,    -1,    54,    -1,    55,
      -1,   132,   157,    22,   156,    -1,   133,   157,    23,   156,
      -1,   171,   140,   171,    -1,   171,   141,   171,    -1,   171,
     143,   171,    -1,   171,   142,   171,    -1,   171,   139,   171,
      -1,   171,   138,   171,    -1,   157,   179,   156,    -1,   179,
     137,   179,    -1,   179,   136,   179,    -1,   158,   179,    -1,
     180,   162,    -1,     7,    -1,    21,   157,   166,    -1,    50,
     157,   171,   155,   171,   155,   171,   156,    -1,    51,   157,
     171,   155,   171,   155,   171,   155,   171,   155,   171,   155,
     171,   155,   171,   155,   171,   155,   171,   156,    -1,    52,
     157,   171,   155,   171,   155,   171,   155,   171,   155,   171,
     155,   171,   156,    -1,    53,   157,   171,   156,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  CloudValueExpressionParser::yyprhs_[] =
  {
         0,     0,     3,     5,     8,    12,    16,    20,    24,    28,
      32,    36,    40,    44,    48,    52,    56,    59,    62,    64,
      67,    69,    71,    73,    75,    77,    79,    80,    81,    82,
      84,    86,    90,    94,    98,   102,   106,   110,   114,   118,
     122,   126,   130,   134,   137,   140,   143,   147,   152,   157,
     162,   167,   172,   177,   182,   188,   192,   195,   197,   204,
     211,   216,   220,   225,   230,   235,   240,   245,   250,   255,
     260,   265,   270,   272,   274,   278,   282,   286,   290,   294,
     301,   306,   311,   315,   319,   323,   327,   331,   335,   339,
     343,   347,   351,   354,   358,   363,   368,   373,   378,   383,
     388,   393,   398,   403,   408,   413,   418,   423,   428,   433,
     438,   443,   448,   453,   458,   463,   468,   473,   478,   483,
     488,   493,   498,   503,   508,   513,   518,   523,   528,   533,
     538,   543,   548,   553,   558,   563,   568,   573,   578,   583,
     588,   593,   598,   603,   608,   613,   618,   623,   628,   633,
     638,   643,   648,   653,   658,   664,   666,   670,   674,   678,
     682,   687,   691,   696,   700,   705,   709,   714,   718,   722,
     725,   727,   729,   734,   741,   748,   753,   757,   759,   763,
     767,   771,   775,   779,   783,   787,   791,   795,   799,   803,
     807,   811,   815,   819,   823,   827,   831,   835,   838,   842,
     847,   852,   857,   862,   867,   872,   877,   884,   890,   893,
     895,   902,   909,   914,   918,   920,   924,   928,   932,   936,
     940,   944,   948,   952,   956,   960,   964,   968,   971,   975,
     980,   985,   990,   995,  1000,  1005,  1010,  1015,  1020,  1027,
    1033,  1036,  1038,  1045,  1052,  1057,  1061,  1063,  1065,  1069,
    1073,  1077,  1081,  1085,  1089,  1092,  1096,  1101,  1106,  1111,
    1116,  1123,  1129,  1132,  1134,  1141,  1148,  1153,  1157,  1159,
    1161,  1166,  1171,  1175,  1179,  1183,  1187,  1191,  1195,  1199,
    1203,  1207,  1210,  1213,  1215,  1219,  1228,  1249,  1264
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  CloudValueExpressionParser::yyrline_[] =
  {
         0,   337,   337,   341,   342,   348,   354,   360,   366,   372,
     378,   384,   390,   396,   402,   408,   414,   419,   426,   427,
     429,   430,   431,   432,   433,   434,   439,   442,   445,   448,
     449,   453,   458,   463,   468,   473,   478,   483,   488,   493,
     498,   503,   508,   513,   517,   521,   525,   526,   530,   534,
     542,   550,   558,   567,   576,   581,   584,   585,   589,   593,
     597,   603,   615,   619,   623,   627,   633,   642,   651,   662,
     673,   677,   683,   684,   685,   690,   695,   700,   705,   715,
     720,   724,   728,   733,   738,   743,   748,   753,   758,   763,
     768,   773,   778,   782,   783,   787,   791,   795,   799,   803,
     807,   811,   815,   819,   823,   827,   831,   835,   839,   843,
     847,   851,   855,   859,   863,   867,   871,   875,   879,   883,
     887,   891,   895,   899,   903,   907,   911,   915,   919,   923,
     927,   931,   935,   939,   943,   947,   951,   955,   959,   963,
     967,   971,   975,   979,   983,   987,   991,   995,   999,  1003,
    1007,  1011,  1015,  1019,  1023,  1028,  1037,  1040,  1045,  1048,
    1049,  1052,  1055,  1058,  1061,  1064,  1067,  1070,  1073,  1080,
    1081,  1085,  1089,  1093,  1097,  1101,  1107,  1119,  1120,  1125,
    1130,  1135,  1140,  1145,  1150,  1155,  1160,  1165,  1170,  1175,
    1180,  1185,  1190,  1195,  1200,  1205,  1210,  1215,  1219,  1220,
    1224,  1228,  1232,  1236,  1240,  1244,  1248,  1252,  1257,  1258,
    1262,  1266,  1270,  1276,  1289,  1290,  1295,  1300,  1305,  1310,
    1315,  1326,  1331,  1336,  1341,  1346,  1351,  1356,  1360,  1361,
    1365,  1369,  1373,  1377,  1381,  1385,  1389,  1393,  1397,  1400,
    1405,  1406,  1410,  1414,  1418,  1424,  1437,  1438,  1441,  1446,
    1451,  1456,  1461,  1466,  1471,  1475,  1476,  1480,  1484,  1488,
    1492,  1495,  1504,  1505,  1509,  1513,  1517,  1523,  1536,  1537,
    1538,  1542,  1546,  1551,  1556,  1561,  1570,  1575,  1584,  1585,
    1594,  1603,  1607,  1608,  1614,  1627,  1633,  1643,  1652
  };

  // Print the state stack on the debug stream.
  void
  CloudValueExpressionParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  CloudValueExpressionParser::yy_reduce_print_ (int yyrule)
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
  CloudValueExpressionParser::token_number_type
  CloudValueExpressionParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   158,     2,     2,     2,   146,   149,     2,
     157,   156,   147,   145,   155,   144,   154,   148,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   135,     2,
     140,     2,   141,   134,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   150,     2,     2,     2,     2,     2,
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
     125,   126,   127,   128,   129,   130,   131,   132,   133,   136,
     137,   138,   139,   142,   143,   151,   152,   153
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int CloudValueExpressionParser::yyeof_ = 0;
  const int CloudValueExpressionParser::yylast_ = 2716;
  const int CloudValueExpressionParser::yynnts_ = 26;
  const int CloudValueExpressionParser::yyempty_ = -2;
  const int CloudValueExpressionParser::yyfinal_ = 146;
  const int CloudValueExpressionParser::yyterror_ = 1;
  const int CloudValueExpressionParser::yyerrcode_ = 256;
  const int CloudValueExpressionParser::yyntokens_ = 159;

  const unsigned int CloudValueExpressionParser::yyuser_token_number_max_ = 397;
  const CloudValueExpressionParser::token_number_type CloudValueExpressionParser::yyundef_token_ = 2;


} // parserCloud

/* Line 1136 of lalr1.cc  */
#line 5893 "CloudValueExpressionParser.tab.cc"


/* Line 1138 of lalr1.cc  */
#line 1657 "../CloudValueExpressionParser.yy"


void parserCloud::CloudValueExpressionParser::error (
    const parserCloud::CloudValueExpressionParser::location_type& l,
    const std::string& m
)
{
     driver.error (l, m);
}

