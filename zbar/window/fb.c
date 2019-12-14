/*------------------------------------------------------------------------
 *  Copyright 2008-2009 (c) Jeff Brown <spadix@users.sourceforge.net>
 *
 *  This file is part of the ZBar Bar Code Reader.
 *
 *  The ZBar Bar Code Reader is free software; you can redistribute it
 *  and/or modify it under the terms of the GNU Lesser Public License as
 *  published by the Free Software Foundation; either version 2.1 of
 *  the License, or (at your option) any later version.
 *
 *  The ZBar Bar Code Reader is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser Public License
 *  along with the ZBar Bar Code Reader; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 *  Boston, MA  02110-1301  USA
 *
 *  http://sourceforge.net/projects/zbar
 *------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>

#include "window.h"

int global_framebuffer_device_fd = 0;
const char *framebuffer_device = "/dev/fb0";
struct fb_var_screeninfo var_framebuffer_info;
struct fb_fix_screeninfo var_framebuffer_fix_info;
size_t framebuffer_screensize = 0;
unsigned char *framebuffer_mappedmem = NULL;

static inline int null_error (void *m,
                              const char *func)
{
    return(err_capture(m, SEV_ERROR, ZBAR_ERR_UNSUPPORTED, func,
                       "not compiled with output window support"));
}

void fb_fill_black()
{
    if (framebuffer_mappedmem != NULL)
    {
        memset(framebuffer_mappedmem, 0x0, framebuffer_screensize);
    }
}

int _zbar_window_attach (zbar_window_t *w,
                         void *display,
                         unsigned long win)
{
    return(0);
}

int _zbar_window_expose (zbar_window_t *w,
                         int x,
                         int y,
                         int width,
                         int height)
{
    return(0);
}

int _zbar_window_resize (zbar_window_t *w)
{
    return(0);
}

int _zbar_window_clear (zbar_window_t *w)
{
    fb_fill_black();
    return(0);
}

int _zbar_window_begin (zbar_window_t *w)
{
    global_framebuffer_device_fd = open(framebuffer_device, O_RDWR);

    if (ioctl(global_framebuffer_device_fd, FBIOGET_VSCREENINFO, &var_framebuffer_info))
    {
        // ERROR
    }

    if (ioctl(global_framebuffer_device_fd, FBIOGET_FSCREENINFO, &var_framebuffer_fix_info))
    {
        // ERROR
    }

    // map framebuffer to user memory
    framebuffer_screensize = (size_t)var_framebuffer_fix_info.smem_len;
    framebuffer_mappedmem = NULL;
    framebuffer_mappedmem = (char *)mmap(NULL,
                                         (size_t)framebuffer_screensize,
                                         PROT_READ | PROT_WRITE,
                                         MAP_SHARED,
                                         global_framebuffer_device_fd, 0);

    if (framebuffer_mappedmem == NULL)
    {
        // dbg(0, "Failed to mmap Framebuffer\n");
    }
    else
    {
        // dbg(2, "mmap Framebuffer: %p\n", framebuffer_mappedmem);
    }

    return(0);
}

int _zbar_window_end (zbar_window_t *w)
{
    if (framebuffer_mappedmem != NULL)
    {
        int res = munmap(framebuffer_mappedmem, (size_t)framebuffer_screensize);
        framebuffer_mappedmem = NULL;
    }

    if (global_framebuffer_device_fd > 0)
    {
        close(global_framebuffer_device_fd);
        global_framebuffer_device_fd = 0;
    }

    return(0);
}

int _zbar_window_draw_marker (zbar_window_t *w,
                              uint32_t rgb,
                              point_t p)
{
    return(null_error(w, __func__));
}

int _zbar_window_draw_polygon (zbar_window_t *w,
                               uint32_t rgb,
                               const point_t *pts,
                               int npts)
{
    return(null_error(w, __func__));
}

int _zbar_window_draw_text (zbar_window_t *w,
                            uint32_t rgb,
                            point_t p,
                            const char *text)
{
    return(null_error(w, __func__));
}

int _zbar_window_fill_rect (zbar_window_t *w,
                            uint32_t rgb,
                            point_t org,
                            point_t size)
{
    return(null_error(w, __func__));
}

int _zbar_window_draw_logo (zbar_window_t *w)
{
    return(null_error(w, __func__));
}
