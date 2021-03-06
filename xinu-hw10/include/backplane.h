/**
 * @file backplane.h
 *
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#ifndef _BACKPLANE_H__
#define _BACKPLANE_H__

#include <device.h>
#include <kernel.h>
#include <sbconfig.h>

devcall backplaneInit(device *);
int backplaneCoreUp(volatile struct backplaneConfig *);
int backplaneReset(volatile struct backplaneConfig *);
int backplaneCoreDisable(volatile struct backplaneConfig *);

#endif                          /* _BACKPLANE_H_ */
