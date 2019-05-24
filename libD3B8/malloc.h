/* Memory Alloction Macros for libD3B8 */
#ifndef INCLUDED_MALLOC_H
#define INCLUDED_MALLOC_H

#include <stdio.h> /*< fprintf, stderr */
#include <stdlib.h> /*< malloc, realloc, free */

/* Kill Program when Memory allocation fails */
#define D3B8_MALLOC_FAIL {\
	/* TODO: log errors instead of print them?  More info? */\
	fprintf( stderr, "FATAL: libD3B8@%s[%i]: Memory Allocation Failed.\n", __FILE__, __LINE__ );\
	exit( EXIT_FAILURE );\
}

/* Usage:
 *   int *lst, len=3;
 *   D3B8_malloc( lst, int, len );
 */
#define D3B8_malloc( ref, type, len ) {\
	if( ( len ) == 0 ) {\
		ref = NULL;\
	} else {\
		type *d3b8_malloc_tmp = (type *)malloc( sizeof( type ) * ( len ) );\
		if( d3b8_malloc_tmp == NULL ) {\
			D3B8_MALLOC_FAIL;\
		} else {\
			ref = d3b8_malloc_tmp;\
		}\
	}\
}

/* Usage:
 *   len=4;
 *   D3B8_realloc( lst, int, len );
 */
#define D3B8_realloc( ref, type, len ) {\
	if( ( len ) == 0 ) {\
		free( ref ); ref = NULL;\
	} else {\
		type *d3b8_realloc_tmp = (type *)realloc( ref, sizeof( type ) * ( len ) );\
		if( d3b8_realloc_tmp == NULL ) {\
			D3B8_MALLOC_FAIL;\
		} else {\
			ref = d3b8_realloc_tmp;\
		}\
	}\
}

#endif /* INCLUDED_MALLOC_H */

/* 4D6175726F6C657069734472656B69 */

