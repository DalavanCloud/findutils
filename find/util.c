/* util.c -- functions for initializing new tree elements, and other things.
   Copyright (C) 1990, 91, 92, 93, 94, 2000, 2003, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.
*/

#include "defs.h"
#include "../gnulib/lib/xalloc.h"

#if ENABLE_NLS
# include <libintl.h>
# define _(Text) gettext (Text)
#else
# define _(Text) Text
#endif
#ifdef gettext_noop
# define N_(String) gettext_noop (String)
#else
/* See locate.c for explanation as to why not use (String) */
# define N_(String) String
#endif


/* Return a pointer to a new predicate structure, which has been
   linked in as the last one in the predicates list.

   Set `predicates' to point to the start of the predicates list.
   Set `last_pred' to point to the new last predicate in the list.
   
   Set all cells in the new structure to the default values. */

struct predicate *
get_new_pred (void)
{
  register struct predicate *new_pred;

  if (predicates == NULL)
    {
      predicates = (struct predicate *)
	xmalloc (sizeof (struct predicate));
      last_pred = predicates;
    }
  else
    {
      new_pred = (struct predicate *) xmalloc (sizeof (struct predicate));
      last_pred->pred_next = new_pred;
      last_pred = new_pred;
    }
  last_pred->pred_func = NULL;
#ifdef	DEBUG
  last_pred->p_name = NULL;
#endif	/* DEBUG */
  last_pred->p_type = NO_TYPE;
  last_pred->p_prec = NO_PREC;
  last_pred->side_effects = false;
  last_pred->no_default_print = false;
  last_pred->need_stat = true;
  last_pred->need_type = true;
  last_pred->args.str = NULL;
  last_pred->pred_next = NULL;
  last_pred->pred_left = NULL;
  last_pred->pred_right = NULL;
  return (last_pred);
}

/* Return a pointer to a new predicate, with operator check.
   Like get_new_pred, but it checks to make sure that the previous
   predicate is an operator.  If it isn't, the AND operator is inserted. */

struct predicate *
get_new_pred_chk_op (void)
{
  struct predicate *new_pred;

  if (last_pred)
    switch (last_pred->p_type)
      {
      case NO_TYPE:
	error (1, 0, _("oops -- invalid default insertion of and!"));
	break;

      case PRIMARY_TYPE:
      case CLOSE_PAREN:
	new_pred = get_new_pred ();
	new_pred->pred_func = pred_and;
#ifdef	DEBUG
	new_pred->p_name = find_pred_name (pred_and);
#endif	/* DEBUG */
	new_pred->p_type = BI_OP;
	new_pred->p_prec = AND_PREC;
	new_pred->need_stat = false;
	new_pred->need_type = false;
	new_pred->args.str = NULL;

      default:
	break;
      }
  return (get_new_pred ());
}

/* Add a primary of predicate type PRED_FUNC to the predicate input list.

   Return a pointer to the predicate node just inserted.

   Fills in the following cells of the new predicate node:

   pred_func	    PRED_FUNC
   args(.str)	    NULL
   p_type	    PRIMARY_TYPE
   p_prec	    NO_PREC

   Other cells that need to be filled in are defaulted by
   get_new_pred_chk_op, which is used to insure that the prior node is
   either not there at all (we are the very first node) or is an
   operator. */

struct predicate *
insert_primary (boolean (*pred_func) (/* ??? */))
{
  struct predicate *new_pred;

  new_pred = get_new_pred_chk_op ();
  new_pred->pred_func = pred_func;
#ifdef	DEBUG
  new_pred->p_name = find_pred_name (pred_func);
#endif	/* DEBUG */
  new_pred->args.str = NULL;
  new_pred->p_type = PRIMARY_TYPE;
  new_pred->p_prec = NO_PREC;
  return (new_pred);
}

void
usage (char *msg)
{
  if (msg)
    fprintf (stderr, "%s: %s\n", program_name, msg);
  fprintf (stderr, _("\
Usage: %s [-H] [-L] [-P] [path...] [expression]\n"), program_name);
  exit (1);
}
