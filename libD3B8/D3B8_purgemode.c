/* Purge Mode Interface */
#include <libD3B8.h>

void D3B8_purgemode_set( D3B8cfg *target, short mode ) {
	/* TODO: verify mode (and do what?) */
	target->_pm = mode;
}

/* 4D6175726F6C657069734472656B69 */

