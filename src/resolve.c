/*
 * Copyright (c) 2014 - 2015 Marek Chalupa
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <assert.h>

/* for WL_SERVER_ID_START */
#include "wayland/wayland-private.h"

#include <wayland-server-protocol.h>
#include <wayland-client-protocol.h>

#include "wldbg.h"
#include "wldbg-pass.h"
#include "wldbg-private.h"
#include "wldbg-ids-map.h"
#include "passes.h"
#include "util.h"

const struct wl_interface *
resolved_objects_get(struct resolved_objects *ro, uint32_t id)
{
	if (id >= WL_SERVER_ID_START)
		return wldbg_ids_map_get(&ro->objects.server_objects,
					 id - WL_SERVER_ID_START);
	else
		return wldbg_ids_map_get(&ro->objects.client_objects, id);
}

const struct wl_interface *
resolved_objects_get_interface(struct resolved_objects *ro, const char *name)
{
	unsigned int i;
	const struct wl_interface *intf;

	for (i = 0; i < ro->objects.client_objects.count; ++i) {
		intf = wldbg_ids_map_get(&ro->objects.client_objects, i);
		if (intf && strcmp(intf->name, name) == 0)
			return intf;
	}

	for (i = 0; i < ro->objects.server_objects.count; ++i) {
		intf = wldbg_ids_map_get(&ro->objects.server_objects,
					 WL_SERVER_ID_START + i);
		if (intf && strcmp(intf->name, name) == 0)
			return intf;
	}

	return NULL;
}

void
resolved_objects_iterate(struct resolved_objects *ro,
			 void (*func)(uint32_t id,
				      const struct wl_interface *intf,
				      void *data),
			 void *data)
{
	unsigned int i;
	const struct wl_interface *intf;

	for (i = 0; i < ro->objects.client_objects.count; ++i) {
		intf = wldbg_ids_map_get(&ro->objects.client_objects, i);
		func(i, intf, data);
	}

	for (i = 0; i < ro->objects.server_objects.count; ++i) {
		intf = wldbg_ids_map_get(&ro->objects.server_objects,
					 WL_SERVER_ID_START + i);
		func( WL_SERVER_ID_START + i, intf, data);
	}
}

