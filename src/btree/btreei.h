#ifndef _BTREE_PRIV_H
#define _BTREE_PRIV_H

#include "btree.h"

/* addkey.c */ 
void addkey(BTREE, FKEY, RKEY, FKEY);

/* block.c */
BLOCK crtblock(BTREE);
BLOCK allocblock(void);

/* index.c */
INDEX crtindex(BTREE);
INDEX getindex(BTREE, FKEY);
void initcache(BTREE, INT);
void putheader(BTREE, BLOCK);
void putindex(BTREE, INDEX);
INDEX readindex(STRING, FKEY);
void writeindex(STRING, INDEX);

/* utils.c */
BOOLEAN newmaster(BTREE, INDEX);
FKEY path2fkey(STRING);
void nextfkey(BTREE);

#endif /* _BTREE_PRIV_H */
