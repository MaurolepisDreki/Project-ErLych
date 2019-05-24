#include <libD3B8.h>
#include <malloc.h>
#include <string.h>

/* long vs short abstraction handling
 *   The algorithm is the same for either list, so it does not make sense to write it twice */
#define D3B8_PREFIX_INIT( cfg, pid ) \
	int *len; char ***list;\
	switch( pid ) {\
		case D3B8_PREFIXID_LONG:\
			len = &cfg->preLong_len;\
			list = &cfg->preLong;\
			break;\
		case D3B8_PREFIXID_SHORT:\
			len = &cfg->preShort_len;\
			list = &cfg->preShort;\
			break;\
		default:\
			fprintf( stderr, "FATAL: libD3B8: Invalid Prefix-ID:" #pid "\n" );\
			exit( EXIT_FAILURE );\
	}

void D3B8_prefix_add( D3B8cfg *target, short prefixid, const char *prefix) {
	D3B8_PREFIX_INIT( target, prefixid );

	/* If we have preallocated space, use it
	 *   otherwise reallocate */
	if( *(*list + *len - 1) != NULL ) {
		(*len)++; /*< must be seperate from the macro below */
		D3B8_realloc( *list, char *, *len );
		*(*list + *len - 1) = NULL;
	}

	/* Binary search list for insertion point; 
	 *   (It's faster then a Bubble sort ) 
	 *   (If we wanted to Bubble sort we would have used a linked list) */
	int start = 0, end = *len - 1, mid = (start + end) / 2;
	while ( start + 1 >= end ) {
		int sc = strcmp( *(*list + mid), prefix );
		
		if ( sc == 0 )
			/* we have an exact match, 
			 *   adding would result in a duplicate entry */
			return;
		else if ( sc > 0 )
			end = mid;
		else /* if ( sc < 0 ) */
			start = mid;
		mid = (start + end) / 2;
	}
	
	/* All things in order, our insert point will be at end if end >= 0 */
	const int insertAt = (end >= 0) ? end : 0;
	const int prefix_len = strlen( prefix ) + 1;
	memmove( *list + insertAt + 1, *list + insertAt, *len - insertAt - 1 * sizeof(char *) );
	D3B8_malloc( *(*list + insertAt), char, prefix_len );
	strcpy( *(*list + insertAt), prefix );
}

void D3B8_prefix_del( D3B8cfg *target, short prefixid, const char *prefix ) {
	D3B8_PREFIX_INIT( target, prefixid );

	/* Binary search list for target prefix */
	int start = 0, end = *len - 1, mid = (start + end) / 2;
	while ( start + 1 >= end ) {
		int sc = strcmp( *(*list + mid), prefix );

		if ( sc == 0 )
			/* We have found our match */
			start = end = mid;
		else if ( sc > 0 )
			end = mid;
		else /* if ( sc < 0 ) */
			start = mid;
		mid = (start + end) / 2;
	}

	/* If it went the way we expect it to, mid should be the item to be removed */
	const int insertAt = (strcmp( *(*list + mid), prefix ) == 0) ? mid : -1;
	if ( insertAt < 0 )
		/* failed to find specified prefix */
		return;
	
	free( *(*list + insertAt) );
	memmove( *list + insertAt, *list + insertAt + 1, *len - insertAt - 1 * sizeof(char*) );
	*(*list + *len - 1) = NULL;

	/* CLEANUP: if at least half the array is unused, remove half the unused space 
	 *   NOTE: This method will always leave at least 1 unused space */
	start = 0; end = *len - 1; mid = (start + end ) / 2;
	while ( start + 1 >= end ) {
		if ( *(*list + mid) == NULL )
			end = mid;
		else
			start = mid;
		mid = (start + end) / 2;
	}

	if ( end <= *len / 2 ) {
		*len -= ( *len - end ) / 2;
		D3B8_realloc( *list, char *, *len );
	}
}

/* 4D6175726F6C657069734472656B69 */

