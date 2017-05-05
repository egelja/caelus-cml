/* Copyright 2004,2007,2009 ENSEIRB, INRIA & CNRS
**
** This file is part of the Scotch software package for static mapping,
** graph partitioning and sparse matrix ordering.
**
** This software is governed by the CeCILL-C license under French law
** and abiding by the rules of distribution of free software. You can
** use, modify and/or redistribute the software under the terms of the
** CeCILL-C license as circulated by CEA, CNRS and INRIA at the following
** URL: "http://www.cecill.info".
** 
** As a counterpart to the access to the source code and rights to copy,
** modify and redistribute granted by the license, users are provided
** only with a limited warranty and the software's author, the holder of
** the economic rights, and the successive licensors have only limited
** liability.
** 
** In this respect, the user's attention is drawn to the risks associated
** with loading, using, modifying and/or developing or reproducing the
** software by the user in light of its specific status of free software,
** that may mean that it is complicated to manipulate, and that also
** therefore means that it is reserved for developers and experienced
** professionals having in-depth computer knowledge. Users are therefore
** encouraged to load and test the software's suitability as regards
** their requirements in conditions enabling the security of their
** systems and/or data to be ensured and, more generally, to use and
** operate it in the same conditions as regards security.
** 
** The fact that you are presently reading this means that you have had
** knowledge of the CeCILL-C license and that you accept its terms.
*/
/************************************************************/
/**                                                        **/
/**   NAME       : hgraph_order_hd.h                       **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module contains the data declara-  **/
/**                tions for the block-oriented Halo       **/
/**                Approximate (Multiple) Minimum Degree   **/
/**                ordering routine.                       **/
/**                                                        **/
/**   DATES      : # Version 3.2  : from : 09 aug 1998     **/
/**                                 to   : 18 aug 1998     **/
/**                # Version 3.3  : from : 02 oct 1998     **/
/**                                 to   : 19 oct 1998     **/
/**                # Version 4.0  : from : 14 jan 2003     **/
/**                                 to   : 24 jan 2004     **/
/**                # Version 5.1  : from : 01 oct 2009     **/
/**                                 to   : 01 oct 2009     **/
/**                                                        **/
/************************************************************/

/*
**  The defines.
*/

#define HGRAPHORDERHDCOMPRAT        1.2L          /*+ Compression ratio +*/

/*
**  The type and structure definitions.
*/

/*+ This structure holds the method parameters. +*/

typedef struct HgraphOrderHdParam_ {
  INT                       colmin;               /*+ Minimum number of columns +*/
  INT                       colmax;               /*+ Maximum number of columns +*/
  double                    fillrat;              /*+ Fill-in ratio             +*/
} HgraphOrderHdParam;

/*
**  The function prototypes.
*/

#ifndef HGRAPH_ORDER_HD
#define static
#endif

int                         hgraphOrderHd       (const Hgraph * RESTRICT const, Order * RESTRICT const, const Gnum, OrderCblk * RESTRICT const, const HgraphOrderHdParam * RESTRICT const);

#undef static
