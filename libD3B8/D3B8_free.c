#include <libD3B8.h>
#include <malloc.h>

#define D3B8_FREERANGE( target, len ) \
	for( int itr = 0; itr < len; itr++ ) \
		if( *(target + itr) != NULL ) \
			free( *(target + itr) );\
	free( target ); target = NULL; len = 0;

void D3B8_free( D3B8cfg *target ) {
	/* do not decomission an uninitialized object */
	if( ! target->initialized ) return;

	/* Optomized to clear cbIndex structures before freeing them to prevent memory leak */
	for( struct _D3B8_cbindex **itr = target->cbIndex; itr < target->cbIndex + target->cbIndex_len; itr++ ) 
		if( *itr != NULL ) {
			free( (*itr)->sid );
			free( (*itr)->lid );
			free( *itr );
		}
	free( target->cbIndex ); target->cbIndex = NULL; target->cbIndex_len = 0;

	/* Optomized to clear longTrigs and longIndex simultaniously w/o memory leak */
	for( int i = 0; i < target->longTrigs_len; i++ ) {
		if( *(target->longIndex + i) != NULL ) free( *(target->longIndex + i) );
		if( *(target->longTrigs + i) != NULL ) {
			regfree( &(*(target->longTrigs + i))->_rxc );
			free( *(target->longTrigs + i) );
		}
	}
	free( target->longTrigs ); target->longTrigs = NULL;
	free( target->longIndex ); target->longIndex = NULL;
	target->longTrigs_len = 0;

	/* Macroized cleanup */
	D3B8_FREERANGE( target->shortTrigs, target->shortTrigs_len );
	D3B8_FREERANGE( target->preLong,    target->preLong_len    );
	D3B8_FREERANGE( target->preShort,   target->preShort_len   );

	/* Should be clean if we've forgotten nothing */
	target->initialized = false;
}

/* 4D6175726F6C657069734472656B69 */

