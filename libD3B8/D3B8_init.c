#include <libD3B8.h>
#include <malloc.h>

#define D3B8_INITLEN 0

#define D3B8_SETRANGE( start, len, val ) \
	for( int itr = 0; itr < len; itr++ ) {\
		*(start + itr) = val;\
	}

void D3B8_init( D3B8cfg *target ) {
	/* do not over initialize */
	if( target->initialized ) return;

	target->preShort_len   = D3B8_INITLEN;
	target->preLong_len    = D3B8_INITLEN;
	target->shortTrigs_len = D3B8_INITLEN;
	target->longTrigs_len  = D3B8_INITLEN;
	target->cbIndex_len    = D3B8_INITLEN;

	/* TODO: EXPAND MACRO TO DETECT ERROR */
	D3B8_malloc( target->preShort,   char *,                   target->preShort_len   );
	D3B8_malloc( target->preLong,    char *,                   target->preLong_len    );
	D3B8_malloc( target->shortTrigs, struct _D3B8_short *,     target->shortTrigs_len );
	D3B8_malloc( target->longTrigs,  struct _D3B8_long *,      target->longTrigs_len  );
	D3B8_malloc( target->longIndex,  struct _D3B8_longindex *, target->longTrigs_len  );
	D3B8_malloc( target->cbIndex,    struct _D3B8_cbindex *,   target->cbIndex_len    );

	/* Just in case some idiot ever changes D3B8_INITLEN from 0, &c. */
	D3B8_SETRANGE( target->preShort,   target->preShort_len,   NULL );
	D3B8_SETRANGE( target->preLong,    target->preLong_len,    NULL );
	D3B8_SETRANGE( target->shortTrigs, target->shortTrigs_len, NULL );
	D3B8_SETRANGE( target->longTrigs,  target->longTrigs_len,  NULL );
	D3B8_SETRANGE( target->longIndex,  target->longTrigs_len,  NULL );
	D3B8_SETRANGE( target->cbIndex,    target->cbIndex_len,    NULL );
	
	/* Assign Defaults */
	D3B8_purgemode_default( target );
	D3B8_unprocCB_unset( target );

	/* unless we forgot something, we are now initialized */
	target->initialized = true;
}

/* 4D6175726F6C657069734472656B69 */

