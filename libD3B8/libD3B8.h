/* Public Header for libD3B8
 *   If you are not familiar with this library, checkout README.md
 * 
 * AUTHOR:
 * ___  ___                      _            _    ______          _    _ 
 * |  \/  |                     | |          (_)   |  _  \        | |  (_)
 * | .  . | __ _ _   _ _ __ ___ | | ___ _ __  _ ___| | | |_ __ ___| | ___ 
 * | |\/| |/ _` | | | | '__/ _ \| |/ _ \ '_ \| / __| | | | '__/ _ \ |/ / |
 * | |  | | (_| | |_| | | | (_) | |  __/ |_) | \__ \ |/ /| | |  __/   <| |
 * \_|  |_/\__,_|\__,_|_|  \___/|_|\___| .__/|_|___/___/ |_|  \___|_|\_\_|
 *                                     | |                                
 *                                     |_|                                
 * 20190518
 */

#ifndef INCLUDED_LIBD3B8_H
#define INCLUDED_LIBD3B8_H

#include <config.h>
#include <regex.h> /*< using GNU C Library for Regex */

/* Make sure we have bool at our disposal */
#ifdef HAVE_STDBOOL_H
# include <stdbool.h>
#else
# ifndef HAVE__BOOL
#  ifdef __cplusplus
typedef bool _Bool;
#  else
#   define _Bool signed char
#  endif /* __cplusplus */
# endif /* HAVE__BOOL */
# define bool _Bool
# define false 0
# define true 1
#endif /* HAVE_STDBOOL_H */

/* BEGIN SECTION D3B8cfg */
typedef bool (*_D3B8_CB)(int, char **); /*< callback [proto]type */

/* Short Argument Trigger Record */
struct _D3B8_short {
	char _trigger;
	unsigned short _consume;
	_D3B8_CB _cb;
};

/* Long Argument Triger Record */
struct _D3B8_long {
	char *_trigger; /*< regex c-string, retained for search */
	regex_t _rxc; /*< "compiled" regex structure */
	short priority;
	unsigned short _consume;
	_D3B8_CB _cb;
};

/* Callback Index Record, 
 *   Used to reverse lookup long and short processor records by callback */
struct _D3B8_cbindex {
	_D3B8_CB ref;
	
	/* short records, index by itr */
	int slen, *sid;

	/* long records, index by itr from */
	int llen, *lid;
};

/* Long Index Record */
struct _D3B8_longindex {
	char * _trigger_ref;
	int index;
};

typedef struct {
	bool initialized;
	short _pm; /* Purge Mode */
	_D3B8_CB _upcb; /* Unproc Callback */

	/* Prefixs */
	int preShort_len, preLong_len;
	char **preShort, **preLong;

	/* Short Argument Triggers */
	int shortTrigs_len;
	struct _D3B8_short **shortTrigs;

	/* Long Argument Triggers */
	int longTrigs_len;
	struct _D3B8_long **longTrigs;

	/* Indexes */
	int cbIndex_len;
	struct _D3B8_cbindex **cbIndex;
	struct _D3B8_longindex **longIndex;
} D3B8cfg;

/* Initialization */
void D3B8_init( D3B8cfg * );

/* Decommission */
void D3B8_free( D3B8cfg * );

/* Duplication / Assignment */
/* A shallow copy is defined by passing a refrence to an object to a new variable name
 *   We cheated this using macros in D3B8_prefix.c as we simulate it here 
 *   ** If you do not understand the implications of the macro, don't use it! ** */
#ifdef D3B8_EXPERT
/* TODO: Find a valid way of writing D3B8_copy_shallow */
#error "D3B8_copy_shallow: &(target) is not a valid lvalue"
#define D3B8_copy_shallow( target, origin ) { &(target) = &(origin); }
#endif /* D3B8_EXPERT */

void D3B8_copy_decend( D3B8cfg *target, D3B8cfg *origin ); /*< inheritance copy */
void D3B8_copy_deep( D3B8cfg *target, D3B8cfg *origin ); /*< use instead of assignment */

/* END SECTION D3B8cfg */
/* BEGIN SECTION TRIGGER INTERFACE */
/* Prefix Management */
#define D3B8_PREFIXID_LONG 0
#define D3B8_PREFIXID_SHORT 1
void D3B8_prefix_add( D3B8cfg *, short prefixid, const char * );
void D3B8_prefix_del( D3B8cfg *, short prefixid, const char * );

#define D3B8_prefix_addlong( cfg, cstr )  D3B8_prefix_add( cfg, D3B8_PREFIXID_LONG, cstr )
#define D3B8_prefix_dellong( cfg, cstr )  D3B8_prefix_del( cfg, D3B8_PREFIXID_LONG, cstr )
#define D3B8_prefix_addshort( cfg, cstr ) D3B8_prefix_add( cfg, D3B8_PREFIXID_SHORT, cstr )
#define D3B8_prefix_delshort( cfg, cstr ) D3B8_prefix_del( cfg, D3B8_PREFIXID_SHORT, cstr )

/* Trigger Management */
void D3B8_addlong( D3B8cfg *, const char *regex, unsigned short consumes, _D3B8_CB, short priority );
void D3B8_addshort( D3B8cfg *, char trig, unsigned short consumes, _D3B8_CB );

void D3B8_dellong( D3B8cfg *, const char *regex );
void D3B8_delshort( D3B8cfg *, char );
void D3B8_deltrigger( D3B8cfg *, _D3B8_CB );

/* Index Management */
#define _DEB8_TRIGGERTYPE_LONG 1
#define _DEB8_TRIGGERTYPE_SHORT 2
void _D3B8_index_addtrigger( D3B8cfg *, _D3B8_CB ref, int triggerType, int atIndex );
void _D3B8_index_deltrigger( D3B8cfg *, _D3B8_CB ref, int triggerType, int atIndex );

/* The long index uses the pointer from _D3B8_long._trigger as an ID but sorts by it's string value */
void _D3B8_index_addlong( D3B8cfg *, char *ref, int atIndex );
void _D3B8_index_dellong( D3B8cfg *, char *ref );

/* Index Management Wrapers */
#define _D3B8_index_addtrigger_short( cfg, ref, at ) _D3B8_index_addtrigger( cfg, ref, _D3B8_TRIGGERTYPE_SHORT, at )
#define _D3B8_index_addtrigger_long( cfg, ref, at ) _D3B8_index_addtrigger( cfg, ref, _D3B8_TRIGGERTYPE_LONG, at )
#define _D3B8_index_deltrigger_short( cfg, ref, at ) _D3B8_index_addtrigger( cfg, ref, _D3B8_TRIGGERTYPE_SHORT, at )
#define _D3B8_index_deltrigger_long( cfg, ref, at ) _D3B8_index_addtrigger( cfg, ref, _D3B8_TRIGGERTYPE_LONG, at )

/* END SECTION TRIGGER INTERFCE */
/* BEGIN SECTION UNHANDLED */
/* Unprocessed Callback */
void D3B8_unprocCB_set( D3B8cfg *, _D3B8_CB );
#define D3B8_unprocCB_unset( cfg ) D3B8_unprocCB_set( cfg, NULL )

/* Purge Funtion Management */
#define D3B8_PURGEMODE_ONSUCCESS 1
#define D3B8_PURGEMODE_ONFAILURE -1
#define D3B8_PURGEMODE_DISABLE 0
#define D3B8_PURGEMODE_DEFAULT D3B8_PURGEMODE_ONSUCCESS
void D3B8_purgemode_set( D3B8cfg *, short mode );

#define D3B8_purgemode_onSuccess( cfg ) D3B8_purgemode_set( cfg, D3B8_PURGEMODE_ONSUCCESS )
#define D3B8_purgemode_onFailure( cfg ) D3B8_purgemode_set( cfg, D3B8_PURGEMODE_ONFAILURE )
#define D3B8_purgemode_disable( cfg ) D3B8_purgemode_set( cfg, D3B8_PURGEMODE_DISABLE )
#define D3B8_purgemode_default( cfg ) D3B8_purgemode_set( cfg, D3B8_PURGEMODE_DEFAULT )
/* END SECTION UNHANDLED */
/* BEGIN SECTION INHERITANCE INTERFACE */
void D3B8_inheritance_add( D3B8cfg *base, D3B8cfg *target );
void D3B8_inheritance_del( D3B8cfg *base, D3B8cfg *target );
/* ^^ base inherits target */

/* END SECTION INHERITANCE INTERFACE */
/* BEGIN SECTION ENGINE */
/* C Hack to obtain C++ Syntax Sugar like:
 *    void D3B8_process( D3B8cfg *, int &, char **& ) */
void _D3B8_process( D3B8cfg *, int *, char *** );
#define D3B8_process( cfg, argc, argv ) _D3B8_process( cfg, &argc, &argv )

/* END SECTION ENGINE */
#endif /* INCLUDED_LIBD3B8_H */

/* 4D6175726F6C657069734472656B69 */

