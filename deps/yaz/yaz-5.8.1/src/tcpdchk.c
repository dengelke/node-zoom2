/* This file is part of the YAZ toolkit.
 * Copyright (C) Index Data
 * See the file LICENSE for details.
 */
/**
 * \file tcpdchk.c
 * \brief Implements TCP WRAPPER check.
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <winsock.h>
#endif

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#if HAVE_SYS_SOCKET_H
/* freebsd wants this for AF_INET */
#include <sys/socket.h>
#endif

#if HAVE_NETDB_H
#include <netdb.h>
#endif

#include <yaz/comstack.h>
#include <yaz/statserv.h>
#include <yaz/log.h>


#if HAVE_TCPD_H
#include <syslog.h>
#include <tcpd.h>

int allow_severity = LOG_INFO;  /* not YLOG !! */
int deny_severity = LOG_WARNING;

#endif

int check_ip_tcpd(void *cd, const char *addr, int len, int type)
{
    const char *daemon_name = (const char *) cd;

    if (type == AF_INET)
    {
        if (daemon_name && *daemon_name)
        {
#if HAVE_TCPD_H
            struct request_info request_info;
            int i;
            char *host_name = 0, *host_addr = 0;
            struct hostent *host;

            struct sockaddr_in *addr_in = (struct sockaddr_in *) addr;

            if ((host = gethostbyaddr((char*)&addr_in->sin_addr,
                                      sizeof(addr_in->sin_addr),
                                      AF_INET)))
                host_name = (char*) host->h_name;
            host_addr = inet_ntoa(addr_in->sin_addr);
            if (host_addr && host_name)
                request_init(&request_info, RQ_DAEMON, daemon_name,
                             RQ_CLIENT_NAME, host_name,
                             RQ_CLIENT_SIN, addr_in,
                             RQ_CLIENT_ADDR, host_addr, 0);
            else
                request_init(&request_info, RQ_DAEMON, daemon_name,
                             RQ_CLIENT_SIN, addr_in, 0);
            i = hosts_access(&request_info);
            if (!i)
            {
                yaz_log(YLOG_DEBUG, "access denied from %s",
                        host_name ? host_name : host_addr);
                return 1;
            }
            yaz_log(YLOG_DEBUG, "access granted from %s",
                    host_name ? host_name : host_addr);
#endif
        }
    }
    return 0;
}

/*
 * Local variables:
 * c-basic-offset: 4
 * c-file-style: "Stroustrup"
 * indent-tabs-mode: nil
 * End:
 * vim: shiftwidth=4 tabstop=8 expandtab
 */

