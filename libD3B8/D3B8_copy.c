#include <libD3B8.h>
#include <malloc.h>
#include <string.h>

/* D3B8_copy_shallow is an "expert" macro in libD3B8.h */

void D3B8_copy_decend( D3B8cfg *target, D3B8cfg *origin ) {
	if ( target->initialized )
		D3B8_free( target );
	D3B8_init( target );

	D3B8_inheritance_add( target, origin );
}

#define local_cpvar( var ) target->var = origin->var
#define local_cprfx( dst, src, len ) {\
	for( int ipre = 0; ipre < ( len ); ipre++ ) {\
		if ( *(( src ) + ipre) == NULL ) {\
			*(( dst ) + ipre) = NULL;\
		} else {\
			D3B8_malloc( *(( dst ) + ipre), char, strlen( *(( src ) + ipre) ) + 1 );\
			strcpy( *(( dst ) + ipre), *(( src ) + ipre) );\
		}\
	}\
}

void D3B8_copy_deep( D3B8cfg *target, D3B8cfg *origin ) {
	if( target->initialized )
		D3B8_free( target );

	/* Custom Initialization Procedure */
	/*   Copy-in array lengths */
	local_cpvar( preShort_len   );
	local_cpvar( preLong_len    );
	local_cpvar( shortTrigs_len );
	local_cpvar( longTrigs_len  );
	local_cpvar( cbIndex_len    );

	/* Create Arrays */
	D3B8_malloc( target->preShort,   char *,                   target->preShort_len   );
	D3B8_malloc( target->preLong,    char *,                   target->preLong_len    );
	D3B8_malloc( target->shortTrigs, struct _D3B8_short *,     target->shortTrigs_len );
	D3B8_malloc( target->longTrigs,  struct _D3B8_long *,      target->longTrigs_len  );
	D3B8_malloc( target->longIndex,  struct _D3B8_longindex *, target->longTrigs_len  );
	D3B8_malloc( target->cbIndex,    struct _D3B8_cbindex *,   target->cbIndex_len    );

	/* Populate Arrays::wq */
	local_cprfx( target->preShort, origin->preShort, target->preShort_len );
	local_cprfx( target->preLong,  origin->preLong,  target->preLong_len  );
	
	/* TODO: Find way to merge into local_cprfx ? */
	for( int its = 0; its < target->shortTrigs_len; its++ ) {
		if( *(origin->shortTrigs + its) == NULL )
			*(target->shortTrigs + its) = NULL;
		else {
			D3B8_malloc( *(target->shortTrigs + its), struct _D3B8_short, 1 );
			memcpy( *(target->shortTrigs + its), *(origin->shortTrigs + its), sizeof( struct _D3B8_short ) );
		}
	}

	for( int itl = 0; itl < target->longTrigs_len; itl++ ) {
		if( *(origin->longTrigs + itl) == NULL )
			*(target->longTrigs + itl) = NULL;
		else {
			D3B8_malloc( *(target->longTrigs + itl), struct _D3B8_long, 1 );
			
			/* Manual memcopy (ensures regex_t internal pointers stay unique) */
			(*(target->longTrigs + itl))->priority = (*(origin->longTrigs + itl))->priority;
			(*(target->longTrigs + itl))->_consume = (*(origin->longTrigs + itl))->_consume;
			(*(target->longTrigs + itl))->_cb      = (*(origin->longTrigs + itl))->_cb;
			D3B8_malloc( (*(target->longTrigs + itl))->_trigger, char, strlen( (*(origin->longTrigs + itl))->_trigger) + 1 );
			strcpy( (*(target->longTrigs + itl))->_trigger, (*(origin->longTrigs + itl))->_trigger );
			regcomp( &(*(target->longTrigs + itl))->_rxc, (*(target->longTrigs + itl))->_trigger, 0 );
		}
	}

	/* Clone Indexes: */
	for( int itl = 0; itl < target->longTrigs_len; itl++ ) {
		if( *(origin->longIndex + itl) == NULL )
			*(target->longIndex + itl) = NULL;
		else {
			D3B8_malloc( *(target->longIndex + itl), struct _D3B8_longindex, 1 );
			
			/* Partial memcopy */
			(*(target->longIndex + itl))->index = (*(origin->longIndex + itl))->index;
			(*(target->longIndex + itl))->_trigger_ref = (*(target->longTrigs + (*(target->longIndex + itl))->index))->_trigger;
		}
	}

	for( int icb = 0; icb < target->cbIndex_len; icb++ ) {
		if( *(origin->cbIndex + icb) == NULL )
			*(target->cbIndex + icb) = NULL;
		else {
			D3B8_malloc( *(target->cbIndex + icb), struct _D3B8_cbindex, 1 );
			memcpy( *(target->cbIndex + icb), *(origin->cbIndex + icb), sizeof( struct _D3B8_cbindex ) );

			D3B8_malloc( (*(target->cbIndex + icb))->sid, int, (*(target->cbIndex + icb))->slen );
			D3B8_malloc( (*(target->cbIndex + icb))->lid, int, (*(target->cbIndex + icb))->llen );
			memcpy( (*(target->cbIndex + icb))->sid, (*(origin->cbIndex + icb))->sid, sizeof( int ) * (*(target->cbIndex + icb))->slen );
			memcpy( (*(target->cbIndex + icb))->lid, (*(origin->cbIndex + icb))->lid, sizeof( int ) * (*(target->cbIndex + icb))->llen );
		}
	}

	/* Initialialization Complete */
	target->initialized = true;
}

#undef local_cpvar
#undef local_cprfx

/* 4D6175726F6C657069734472656B69 */

