#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

void outb(u2 port, u1 value);
void outw(u2 port, u2 value);
u1 inb(u2 port);
u2 inw(u2 port);

#ifdef __cplusplus
}
#endif
