/* 
   Unix SMB/CIFS implementation.
   SMB parameters and setup
   Copyright (C) Andrew Tridgell 1992-1997
   Copyright (C) Luke Kenneth Casson Leighton 1996-1997
   Copyright (C) Paul Ashton 1997
   Copyright (C) Jean François Micouleau 2002
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _RPC_NETLOGON_H /* _RPC_NETLOGON_H */
#define _RPC_NETLOGON_H 

/* Some flag values reverse engineered from NLTEST.EXE */
/* used in the NETLOGON_CONTROL[2] reply */

#define NL_CTRL_IN_SYNC          0x0000
#define NL_CTRL_REPL_NEEDED      0x0001
#define NL_CTRL_REPL_IN_PROGRESS 0x0002
#define NL_CTRL_FULL_SYNC        0x0004

#define LOGON_KRB5_FAIL_CLOCK_SKEW	0x02000000

/* Flags for controlling the behaviour of a particular logon */

/* sets NETLOGON_SERVER_TRUST_ACCOUNT user_flag */
#if 0
#define MSV1_0_ALLOW_SERVER_TRUST_ACCOUNT	0x00000020
#define MSV1_0_ALLOW_WORKSTATION_TRUST_ACCOUNT	0x00000800

/* updates the "logon time" on network logon */
#define MSV1_0_UPDATE_LOGON_STATISTICS		0x00000004

/* returns the user parameters in the driveletter */
#define MSV1_0_RETURN_USER_PARAMETERS		0x00000008

/* returns the profilepath in the driveletter and 
 * sets LOGON_PROFILE_PATH_RETURNED user_flag */
#define MSV1_0_RETURN_PROFILE_PATH		0x00000200
#endif

#define INTERACTIVE_LOGON_TYPE 1
#define NET_LOGON_TYPE 2

/* LOCKOUT_STRING */
typedef struct account_lockout_string {
	uint32 array_size;
	uint32 offset;
	uint32 length;
/*	uint16 *bindata;	*/
	uint64 lockout_duration;
	uint64 reset_count;
	uint32 bad_attempt_lockout;
	uint32 dummy;
} LOCKOUT_STRING;

/* HDR_LOCKOUT_STRING */
typedef struct hdr_account_lockout_string {
	uint16 size;
	uint16 length;
	uint32 buffer;
} HDR_LOCKOUT_STRING;

#define DSGETDC_VALID_FLAGS ( \
    DS_FORCE_REDISCOVERY | \
    DS_DIRECTORY_SERVICE_REQUIRED | \
    DS_DIRECTORY_SERVICE_PREFERRED | \
    DS_GC_SERVER_REQUIRED | \
    DS_PDC_REQUIRED | \
    DS_BACKGROUND_ONLY | \
    DS_IP_REQUIRED | \
    DS_KDC_REQUIRED | \
    DS_TIMESERV_REQUIRED | \
    DS_WRITABLE_REQUIRED | \
    DS_GOOD_TIMESERV_PREFERRED | \
    DS_AVOID_SELF | \
    DS_ONLY_LDAP_NEEDED | \
    DS_IS_FLAT_NAME | \
    DS_IS_DNS_NAME | \
    DS_RETURN_FLAT_NAME  | \
    DS_RETURN_DNS_NAME )

struct DS_DOMAIN_CONTROLLER_INFO {
	const char *domain_controller_name;
	const char *domain_controller_address;
	int32 domain_controller_address_type;
	struct GUID *domain_guid;
	const char *domain_name;
	const char *dns_forest_name;
	uint32 flags;
	const char *dc_site_name;
	const char *client_site_name;
};

#endif /* _RPC_NETLOGON_H */
