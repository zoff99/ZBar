/*------------------------------------------------------------------------
 *  Copyright 2007-2008 (c) Jeff Brown <spadix@users.sourceforge.net>
 *
 *  This file is part of the Zebra Barcode Library.
 *
 *  The Zebra Barcode Library is free software; you can redistribute it
 *  and/or modify it under the terms of the GNU Lesser Public License as
 *  published by the Free Software Foundation; either version 2.1 of
 *  the License, or (at your option) any later version.
 *
 *  The Zebra Barcode Library is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser Public License
 *  along with the Zebra Barcode Library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 *  Boston, MA  02110-1301  USA
 *
 *  http://sourceforge.net/projects/zebra
 *------------------------------------------------------------------------*/

#include "error.h"

int _zebra_verbosity = 0;

static const char * const sev_str[] = {
    "FATAL ERROR", "ERROR", "OK", "WARNING", "NOTE"
};
#define SEV_MAX (strlen(sev_str[0]))

static const char * const mod_str[] = {
    "processor", "video", "window", "image scanner", "<unknown>"
};
#define MOD_MAX (strlen(mod_str[ZEBRA_MOD_IMAGE_SCANNER]))

static const char const * err_str[] = {
    "no error",                 /* OK */
    "out of memory",            /* NOMEM */
    "internal library error",   /* INTERNAL */
    "unsupported request",      /* UNSUPPORTED */
    "invalid request",          /* INVALID */
    "system error",             /* SYSTEM */
    "locking error",            /* LOCKING */
    "all resources busy",       /* BUSY */
    "X11 display error",        /* XDISPLAY */
    "X11 protocol error",       /* XPROTO */
    "output window is closed",  /* CLOSED */
    "unknown error"             /* NUM */
};
#define ERR_MAX (strlen(err_str[ZEBRA_ERR_CLOSED]))

int zebra_version (unsigned *major,
                   unsigned *minor)
{
    if(major)
        *major = ZEBRA_VERSION_MAJOR;
    if(minor)
        *minor = ZEBRA_VERSION_MINOR;
    return(0);
}

void zebra_set_verbosity (int level)
{
    _zebra_verbosity = level;
}

void zebra_increase_verbosity ()
{
    if(!_zebra_verbosity)
        _zebra_verbosity++;
    else
        _zebra_verbosity <<= 1;
}

int _zebra_error_spew (const void *container,
                       int verbosity)
{
    const errinfo_t *err = container;
    assert(err->magic == ERRINFO_MAGIC);
    fprintf(stderr, "%s", _zebra_error_string(err, verbosity));
    return(-err->sev);
}

zebra_error_t _zebra_get_error_code (const void *container)
{
    const errinfo_t *err = container;
    assert(err->magic == ERRINFO_MAGIC);
    return(err->type);
}

/* ERROR: zebra video in v4l1_set_format():
 *     system error: blah[: blah]
 */

const char *_zebra_error_string (const void *container,
                                 int verbosity)
{
    errinfo_t *err = (errinfo_t*)container;
    assert(err->magic == ERRINFO_MAGIC);

    const char *sev;
    if(err->sev >= SEV_FATAL && err->sev <= SEV_NOTE)
        sev = sev_str[err->sev + 2];
    else
        sev = sev_str[1];

    const char *mod;
    if(err->module >= ZEBRA_MOD_PROCESSOR &&
       err->module < ZEBRA_MOD_UNKNOWN)
        mod = mod_str[err->module];
    else
        mod = mod_str[ZEBRA_MOD_UNKNOWN];

    const char *func = (err->func) ? err->func : "<unknown>";

    const char *type;
    if(err->type >= 0 && err->type < ZEBRA_ERR_NUM)
        type = err_str[err->type];
    else
        type = err_str[ZEBRA_ERR_NUM];

    char basefmt[] = "%s: zebra %s in %s():\n    %s: ";
    int len = SEV_MAX + MOD_MAX + ERR_MAX + strlen(func) + sizeof(basefmt);
    err->buf = realloc(err->buf, len);
    len = sprintf(err->buf, basefmt, sev, mod, func, type);
    if(len <= 0)
        return("<unknown>");

    if(err->detail) {
        int newlen = len + strlen(err->detail) + 1;
        if(strstr(err->detail, "%s")) {
            if(!err->arg_str)
                err->arg_str = "<?>";
            err->buf = realloc(err->buf, newlen + strlen(err->arg_str));
            len += sprintf(err->buf + len, err->detail, err->arg_str);
        }
        else if(strstr(err->detail, "%d")) {
            err->buf = realloc(err->buf, newlen + 32);
            len += sprintf(err->buf + len, err->detail, err->arg_int);
        }
        else {
            err->buf = realloc(err->buf, newlen);
            len += sprintf(err->buf + len, "%s", err->detail);
        }
        if(len <= 0)
            return("<unknown>");
    }

    if(err->type == ZEBRA_ERR_SYSTEM) {
        char sysfmt[] = ": %s (%d)\n";
        const char *syserr = strerror(err->errnum);
        err->buf = realloc(err->buf, len + strlen(sysfmt) + strlen(syserr));
        len += sprintf(err->buf + len, sysfmt, syserr, err->errnum);
    }
    else {
        err->buf = realloc(err->buf, len + 2);
        len += sprintf(err->buf + len, "\n");
    }
    return(err->buf);
}
