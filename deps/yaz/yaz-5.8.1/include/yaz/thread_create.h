/* This file is part of the YAZ toolkit.
 * Copyright (C) Index Data.
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Index Data nor the names of its contributors
 *       may be used to endorse or promote products derived from this
 *       software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file thread_create.h
 * \brief Implements thread creation wrappers
 */
#ifndef YAZ_THREAD_CREATE_H
#define YAZ_THREAD_CREATE_H

#include <stddef.h>
#include <time.h>
#include <yaz/yconfig.h>

YAZ_BEGIN_CDECL

/** \brief Thread Identifier opaque pointer */
typedef struct yaz_thread *yaz_thread_t;

/** \brief create thread
    \param start_routine thread handler
    \param arg user data to be passed to handler
    \returns thread_id identifier if successful; NULL on failure
 */
YAZ_EXPORT yaz_thread_t yaz_thread_create(void *(*start_routine)(void *p), void *arg);

/** \brief join thread
    \param tp thread_id reference .. Will be 0 upon completion
    \param value_ptr ref pointer to routine result (0 if not needed)
*/
YAZ_EXPORT void yaz_thread_join(yaz_thread_t *tp, void **value_ptr);

/** \brief detach thread
    \param tp thread_id reference .. Will be 0 upon completion
*/
void yaz_thread_detach(yaz_thread_t *tp);

YAZ_END_CDECL

#endif
/*
 * Local variables:
 * c-basic-offset: 4
 * c-file-style: "Stroustrup"
 * indent-tabs-mode: nil
 * End:
 * vim: shiftwidth=4 tabstop=8 expandtab
 */

