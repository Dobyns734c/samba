/*
  this is a replacement config.h for building the heimdal parts of the
  Samba source tree
*/

#ifndef HAVE_HEIMDAL_CONFIG_H
#define HAVE_HEIMDAL_CONFIG_H

#include "include/config.h"
#include "../replace/replace.h"

#define RCSID(msg) struct __rcsid { int __rcsdi; }
#define KRB5

/* This needs to be defined for roken too */
#ifdef VOID_RETSIGTYPE
#define SIGRETURN(x) return
#else
#define SIGRETURN(x) return (RETSIGTYPE)(x)
#endif

#define HDB_DB_DIR ""

#undef HAVE_KRB5_ENCRYPT_BLOCK

/*Workaround for heimdal define vs samba define*/
#ifdef HAVE_LIBINTL_H
#define LIBINTL
#endif

#endif
