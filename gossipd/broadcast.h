#ifndef LIGHTNING_LIGHTNINGD_GOSSIP_BROADCAST_H
#define LIGHTNING_LIGHTNINGD_GOSSIP_BROADCAST_H
#include "config.h"

#include <ccan/intmap/intmap.h>
#include <ccan/list/list.h>
#include <ccan/short_types/short_types.h>
#include <ccan/tal/tal.h>

/* Common functionality to implement staggered broadcasts with replacement. */

struct queued_message {
	int type;

	/* Unique tag specifying the msg origin */
	void *tag;

	/* Serialized payload */
	u8 *payload;
};

struct broadcast_state {
	u64 next_index;
	UINTMAP(struct queued_message *) broadcasts;
};

struct broadcast_state *new_broadcast_state(tal_t *ctx);

/* Replace a queued message with @index, if it matches the type and
 * tag for the new message. The new message will be queued with the
 * next highest index. @index is updated to hold the index of the
 * newly queued message*/
bool replace_broadcast(struct broadcast_state *bstate,
		       u64 *index,
		       const int type,
		       const u8 *tag,
		       const u8 *payload);


struct queued_message *next_broadcast_message(struct broadcast_state *bstate, u64 last_index);

#endif /* LIGHTNING_LIGHTNINGD_GOSSIP_BROADCAST_H */
