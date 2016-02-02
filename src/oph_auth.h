/*
    Ophidia Server
    Copyright (C) 2012-2016 CMCC Foundation

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPH_AUTH_H
#define OPH_AUTH_H

#include "oph_parser.h"

// Files
#define OPH_AUTH_FILE "%s/users.dat"
#define OPH_AUTH_DN_FILE "%s/dn.dat"
#define OPH_USER_DIR "%s/users/%s"
#define OPH_USER_FILE "%s/users/%s/user.dat"
#define OPH_SESSION_DIR "%s/users/%s/sessions"
#define OPH_SESSION_FILE "%s/users/%s/sessions/%s.session"
#define OPH_SESSION_REAL_DIR "%s/sessions/%s"
#define OPH_SESSION_REAL_FILE "%s/%s.session"
#define OPH_USER_SESSION_FILE "%s/users/%s/sessions/%s.user"

// User parameters
#define OPH_USER_OPENED_SESSIONS "OPH_OPENED_SESSIONS"
#define OPH_USER_MAX_SESSIONS "OPH_MAX_SESSIONS"
#define OPH_USER_TIMEOUT_SESSION "OPH_TIMEOUT_SESSION"
#define OPH_USER_MAX_CORES "OPH_MAX_CORES"
#define OPH_USER_MAX_HOSTS "OPH_MAX_HOSTS"
#define OPH_USER_IS_ADMIN "OPH_IS_ADMIN"
// User status
#define OPH_USER_LAST_SESSION_ID "OPH_SESSION_ID"
#define OPH_USER_LAST_EXEC_MODE "OPH_EXEC_MODE"
#define OPH_USER_LAST_NCORES "OPH_NCORES"
#define OPH_USER_LAST_STATUS {OPH_USER_LAST_SESSION_ID,OPH_USER_LAST_EXEC_MODE,OPH_USER_LAST_NCORES}
#define OPH_USER_LAST_STATUS_SIZE 3

// Session parameters
#define OPH_SESSION_ID "OPH_SESSION_ID"
#define OPH_SESSION_LAST_MARKER "OPH_LAST_MARKER"
#define OPH_SESSION_LAST_WORKFLOW "OPH_LAST_WORKFLOW"
#define OPH_SESSION_OWNER "OPH_OWNER"
#define OPH_SESSION_LAST_COMMAND "OPH_LAST_COMMAND"
#define OPH_SESSION_CREATION_TIME "OPH_CREATION_TIME"
#define OPH_SESSION_LAST_ACCESS_TIME "OPH_LAST_ACCESS_TIME"
#define OPH_SESSION_ACTIVE "OPH_ACTIVE"
#define OPH_SESSION_AUTOREMOVE "OPH_AUTOREMOVE"
#define OPH_SESSION_USERS "OPH_USERS"
#define OPH_SESSION_LABEL "OPH_LABEL"
// User status
#define OPH_SESSION_CUBE "OPH_DATACUBE"
#define OPH_SESSION_LAST_STATUS {OPH_SESSION_CUBE}
#define OPH_SESSION_LAST_STATUS_SIZE 1

// User-specific session parameters
#define OPH_SESSION_CWD "OPH_CWD"
#define OPH_USER_SESSION_LAST_STATUS {OPH_SESSION_CWD}
#define OPH_USER_SESSION_LAST_STATUS_SIZE 1

// User roles
#define OPH_ROLE_NULL_STR "-----"
#define OPH_ROLE_READ_STR "read"
#define OPH_ROLE_READ_FORCE_STR "read_force"
#define OPH_ROLE_WRITE_STR "write"
#define OPH_ROLE_EXECUTE_STR "execute"
#define OPH_ROLE_ADMIN_STR "admin"
#define OPH_ROLE_OWNER_STR "owner"
// Role values
#define OPH_ROLE_NONE 0
#define OPH_ROLE_READ 1
#define OPH_ROLE_WRITE 2
#define OPH_ROLE_EXECUTE 4
#define OPH_ROLE_ADMIN 8
#define OPH_ROLE_OWNER 16
// Dependening values
#define OPH_ROLE_WRITER (OPH_ROLE_READ+OPH_ROLE_WRITE)
#define OPH_ROLE_ADMINISTRATOR (OPH_ROLE_ADMIN+OPH_ROLE_EXECUTE+OPH_ROLE_WRITER)
#define OPH_ROLE_ALL (OPH_ROLE_OWNER+OPH_ROLE_ADMINISTRATOR)

typedef int oph_auth_user_role;

// Default values
#define OPH_DEFAULT_DAY_TO_SEC 86400
#define OPH_DEFAULT_YES "yes"
#define OPH_DEFAULT_NO "no"
#define OPH_DEFAULT_CORES 1
#define OPH_DEFAULT_HOSTS 1
#define OPH_DEFAULT_USER_OPENED_SESSIONS 0 // No session
#define OPH_DEFAULT_USER_MAX_SESSIONS 0 // No limit
#define OPH_DEFAULT_USER_MAX_CORES 0 // No limit
#define OPH_DEFAULT_USER_MAX_HOSTS 0 // No limit
#define OPH_DEFAULT_SESSION_TIMEOUT 0 // No limit [days]
#define OPH_DEFAULT_SESSION_ACTIVE OPH_DEFAULT_YES
#define OPH_DEFAULT_SESSION_AUTOREMOVE OPH_DEFAULT_NO
#define OPH_DEFAULT_SESSION_ROLE OPH_ROLE_READ

#define OPH_AUTH_UNKNOWN 0
#define OPH_AUTH_DENIED 1
#define OPH_AUTH_READ 2
#define OPH_AUTH_READ_FORCE 3
#define OPH_AUTH_WRITE 4

typedef struct _oph_auth_user_bl
{
	char* userid;
	char* host;
	short count;
	int timestamp;
	struct _oph_auth_user_bl* next;
} oph_auth_user_bl;

int oph_load_file(const char* filename, oph_argument** args);
int oph_load_file2(const char* filename, oph_argument** args);
int oph_auth_user(const char* userid, const char* passwd, const char* host);
int oph_load_user(const char* userid, oph_argument** args, int* save_in_odb);
int oph_save_user(const char* userid, oph_argument* args);
int oph_auth_session(const char* userid, const char* sessionid, const char* serverid, oph_argument** args, int* active, oph_auth_user_role* role);
int oph_save_session(const char* userid, const char* sessionid, oph_argument* args, int type);
int oph_save_user_session(const char* userid, const char* sessionid, oph_argument* args);
int oph_get_session_code(const char* sessionid, char* code);

oph_auth_user_role oph_string_to_role(const char* role);
oph_auth_user_role oph_code_role(const char* role);
char* oph_role_to_string(oph_auth_user_role role); // The result has to be freed
char* oph_code_role_string(const char* role); // The result has to be freed
char* oph_expand_role_string(const char* role); // The result has to be freed

int oph_auth_check_role(oph_auth_user_role role, oph_auth_user_role permission);

#endif /* OPH_AUTH_H */
