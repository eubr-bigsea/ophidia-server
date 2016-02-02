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

#include "oph_ophidiadb.h"
#include "oph_gather.h"

extern char* oph_server_location;
extern ophidiadb *ophDB;

#if defined(_POSIX_THREADS) || defined(_SC_THREADS)
extern pthread_mutex_t global_flag;
#endif

int oph_odb_read_config_ophidiadb(ophidiadb *oDB)
{
	if(!oDB) return OPH_ODB_NULL_PARAM;

	if (!ophDB)
	{
		ophDB = (ophidiadb*)malloc(sizeof(ophidiadb));
		if (!ophDB) return OPH_ODB_MEMORY_ERROR;

		char config[OPH_MAX_STRING_SIZE];
	  	snprintf(config, sizeof(config), OPH_DBMS_CONF_FILE, oph_server_location);

		FILE *file = fopen(config, "r");
		if(file == NULL) return OPH_ODB_ERROR;

		char buffer[OPH_MAX_STRING_SIZE];
		char *position;
		if( fscanf(file, "%s", buffer) == EOF)
		{
			fclose(file);
			return OPH_ODB_ERROR;
		}

		position = strchr(buffer, '=');
		if(position != NULL)
		{
			if(!(ophDB->name=(char*)malloc((strlen(position+1)+1)*sizeof(char))))
			{
				fclose(file);
				return OPH_ODB_MEMORY_ERROR;
			}
			strncpy(ophDB->name, position+1, strlen(position+1)+1);
			ophDB->name[strlen(position+1)] = '\0';
		}
		if( fscanf(file, "%s", buffer) == EOF)
		{
			fclose(file);
			return OPH_ODB_ERROR;
		}

		position = strchr(buffer, '=');
		if(position != NULL)
		{
			if(!(ophDB->hostname=(char*)malloc((strlen(position+1)+1)*sizeof(char))))
			{
				fclose(file);
				return OPH_ODB_MEMORY_ERROR;
			}
			strncpy(ophDB->hostname, position+1, strlen(position+1)+1);
			ophDB->hostname[strlen(position+1)] = '\0';
		}
		if( fscanf(file, "%s", buffer) == EOF)
		{
			fclose(file);
			return OPH_ODB_ERROR;
		}

		position = strchr(buffer, '=');
		if(position != NULL) ophDB->server_port = (int)strtol(position+1, NULL, 10);

		if( fscanf(file, "%s", buffer) == EOF)
		{
			fclose(file);
			return OPH_ODB_ERROR;
		}

		position = strchr(buffer, '=');
		if(position != NULL)
		{
			if(!(ophDB->username=(char*)malloc((strlen(position+1)+1)*sizeof(char))))
			{
				fclose(file);
				return OPH_ODB_MEMORY_ERROR;
			}
			strncpy(ophDB->username, position+1, strlen(position+1)+1);
			ophDB->username[strlen(position+1)] = '\0';
		}
		if(fscanf(file, "%s", buffer) == EOF )
		{
			fclose(file);
			return OPH_ODB_ERROR;
		}

		position = strchr(buffer, '=');
		if(position != NULL)
		{
			if(!(ophDB->pwd=(char*)malloc((strlen(position+1)+1)*sizeof(char))))
			{
				fclose(file);
				return OPH_ODB_MEMORY_ERROR;
			}
			strncpy(ophDB->pwd, position+1, strlen(position+1)+1);
			ophDB->pwd[strlen(position+1)] = '\0';
		}

		fclose(file);

		ophDB->conn=0;
	}

	oDB->name = ophDB->name;
	oDB->hostname = ophDB->hostname;
	oDB->server_port = ophDB->server_port;
	oDB->username = ophDB->username;
	oDB->pwd = ophDB->pwd;

	return OPH_ODB_SUCCESS;
}

int oph_odb_initialize_ophidiadb(ophidiadb *oDB)
{
	if(!oDB) return OPH_ODB_NULL_PARAM;

	oDB->name=NULL;
	oDB->hostname = NULL;
	oDB->username=NULL;
	oDB->pwd=NULL;
	oDB->conn=NULL;

	return OPH_ODB_SUCCESS;
}

int oph_odb_free_ophidiadb(ophidiadb *oDB)
{
	if(!oDB) return OPH_ODB_NULL_PARAM;

	if(oDB->name){
		free(oDB->name);
		oDB->name=NULL;
	}
	if(oDB->hostname){
		free(oDB->hostname);
		oDB->hostname = NULL;
	}
	if(oDB->username){
		free(oDB->username);
		oDB->username=NULL;
	}
	if(oDB->pwd){
		free(oDB->pwd);
		oDB->pwd=NULL;
	}
	if(oDB->conn){
		oph_odb_disconnect_from_ophidiadb(oDB);
		oDB->conn=NULL;
	}

	free(oDB);

	return OPH_ODB_SUCCESS;
}

int oph_odb_connect_to_ophidiadb(ophidiadb *oDB)
{
	if(!oDB) return OPH_ODB_NULL_PARAM;

	oDB->conn = NULL;
	if(!(oDB->conn = mysql_init(NULL)))
	{
		oph_odb_disconnect_from_ophidiadb(oDB);
        	return OPH_ODB_MYSQL_ERROR;
	}

	/* Connect to database */
	if (!mysql_real_connect(oDB->conn, oDB->hostname, oDB->username, oDB->pwd, oDB->name, oDB->server_port, NULL, 0))
	{
		oph_odb_disconnect_from_ophidiadb(oDB);
        	return OPH_ODB_MYSQL_ERROR;
	}

	return OPH_ODB_SUCCESS;
}

int oph_odb_check_connection_to_ophidiadb(ophidiadb *oDB)
{
	if(!oDB) return OPH_ODB_NULL_PARAM;

	if(!(oDB->conn)) return OPH_ODB_MYSQL_ERROR;

	if(mysql_ping(oDB->conn))
	{
		mysql_close(oDB->conn);
		/* Connect to database */
		if(oph_odb_connect_to_ophidiadb(oDB))
		{
			oph_odb_disconnect_from_ophidiadb(oDB);
			return OPH_ODB_MYSQL_ERROR;
		}
	}
	return OPH_ODB_SUCCESS;
}

int oph_odb_disconnect_from_ophidiadb(ophidiadb *oDB)
{
	if(!oDB){	
		return OPH_ODB_NULL_PARAM;
	}

	if(oDB->conn){
		mysql_close(oDB->conn);
		oDB->conn = NULL;
	}
        
	return OPH_ODB_SUCCESS;
}

int oph_odb_retrieve_ids(ophidiadb *oDB, const char* command, int** id, char*** ctime, int* nn)
{
	if(!oDB || !command || !id || !nn)
	{
		return OPH_ODB_NULL_PARAM;
	}
	*nn=0;
	*id=NULL;

	if(oph_odb_check_connection_to_ophidiadb(oDB)) return OPH_ODB_MYSQL_ERROR;

	char query[MYSQL_BUFLEN];

	int n = snprintf(query, MYSQL_BUFLEN, command);
	if(n >= MYSQL_BUFLEN) return OPH_ODB_STR_BUFF_OVERFLOW;

	if(mysql_select_db(oDB->conn, oDB->name)) return OPH_ODB_MYSQL_ERROR;

	if (mysql_query(oDB->conn, query)) return OPH_ODB_MYSQL_ERROR;

	MYSQL_RES *res;
	MYSQL_ROW row;
	res = mysql_store_result(oDB->conn);

	if(mysql_field_count(oDB->conn) != 2)
	{
		mysql_free_result(res);
		return OPH_ODB_TOO_MANY_ROWS;
	}

	*nn = mysql_num_rows(res);
	if (!(*nn))
	{
		mysql_free_result(res);
		return OPH_ODB_NO_ROW_FOUND;
	}

	*id = (int*)malloc((*nn)*sizeof(int));
	if (ctime) *ctime = (char**)malloc((*nn)*sizeof(char*));

	int j=0;
	while((row = mysql_fetch_row(res)) != NULL)
	{
		(*id)[j] = (int)strtol(row[0], NULL, 10);
		if (ctime) (*ctime)[j] = row[1] ? strdup(row[1]) : NULL;
		j++;
	}
	mysql_free_result(res);

	return OPH_ODB_SUCCESS;
}

int oph_odb_retrieve_list(ophidiadb *oDB, const char* command, ophidiadb_list *list)
{
	if(!oDB || !command || !list) return OPH_ODB_NULL_PARAM;

	if(oph_odb_check_connection_to_ophidiadb(oDB)) return OPH_ODB_MYSQL_ERROR;
	
	char query[MYSQL_BUFLEN];

	int n = snprintf(query, MYSQL_BUFLEN, command);
	if(n >= MYSQL_BUFLEN) return OPH_ODB_STR_BUFF_OVERFLOW;

	if(mysql_select_db(oDB->conn, oDB->name)) return OPH_ODB_MYSQL_ERROR;

	if (mysql_query(oDB->conn, query)) return OPH_ODB_MYSQL_ERROR;

	MYSQL_RES *res;
	MYSQL_ROW row;
	res = mysql_store_result(oDB->conn);

	if((mysql_field_count(oDB->conn) < 4) || (mysql_field_count(oDB->conn) > 5))
	{
		mysql_free_result(res);
		return OPH_ODB_TOO_MANY_ROWS;
	}

	list->size = mysql_num_rows(res);

	list->name = (char**)malloc(list->size*sizeof(char*));
	list->id = (int*)malloc(list->size*sizeof(int));
	list->pid = (int*)malloc(list->size*sizeof(int));
	list->wid = (int*)malloc(list->size*sizeof(int));
	list->ctime = (char**)malloc(list->size*sizeof(char*));
	if (!list->name || !list->id || !list->wid || !list->pid || !list->ctime)
	{
		mysql_free_result(res);
		return OPH_ODB_MEMORY_ERROR;
	}

	int j=0;
	while((row = mysql_fetch_row(res)) != NULL)
	{
  		list->id[j]=(int)strtol(row[0], NULL, 10);
		list->ctime[j] = strndup(row[1],OPH_MAX_STRING_SIZE);
		list->name[j] = strndup(row[2],OPH_MAX_STRING_SIZE);
		list->wid[j]= row[3] ? (int)strtol(row[3], NULL, 10) : 0;
		if (mysql_field_count(oDB->conn)>4) list->pid[j]= row[4] ? (int)strtol(row[4], NULL, 10) : 0;
		else list->pid[j]=0;
		j++;
	}
	mysql_free_result(res);

	return OPH_ODB_SUCCESS;
}

int oph_odb_initialize_ophidiadb_list(ophidiadb_list *list)
{
	if(!list) return OPH_ODB_NULL_PARAM;

	list->name=NULL;
	list->id=NULL;
	list->pid=NULL;
	list->wid=NULL;
	list->ctime=NULL;

	list->size=0;
	return OPH_ODB_SUCCESS;
}
int oph_odb_free_ophidiadb_list(ophidiadb_list* list)
{
	int j;
	if(!list) return OPH_ODB_NULL_PARAM;

	if(list->name)
	{
		for (j=0;j<list->size;++j) if (list->name[j])
		{
			free(list->name[j]);
			list->name[j]=NULL;
		}
		free(list->name);
		list->name=NULL;
	}
	if(list->id)
	{
		free(list->id);
		list->id = NULL;
	}
	if(list->pid)
	{
		free(list->pid);
		list->pid = NULL;
	}
	if(list->wid)
	{
		free(list->wid);
		list->wid = NULL;
	}
	if(list->ctime)
	{
		for (j=0;j<list->size;++j) if (list->ctime[j])
		{
			free(list->ctime[j]);
			list->ctime[j]=NULL;
		}
		free(list->ctime);
		list->ctime=NULL;
	}

	list->size = 0;
	return OPH_ODB_SUCCESS;
}

int oph_odb_extract_datacube_ids(ophidiadb* oDB, char* query, cube** datacube, int* counter)
{
	if(!oDB || !query || !datacube || !counter) return OPH_ODB_NULL_PARAM;

	if( oph_odb_check_connection_to_ophidiadb(oDB)) return OPH_ODB_MYSQL_ERROR;

	if (mysql_query(oDB->conn, query)) return OPH_ODB_MYSQL_ERROR;

	MYSQL_RES *res;
	MYSQL_ROW row;
	res = mysql_store_result(oDB->conn);

	if(mysql_field_count(oDB->conn) != 2)
	{
		mysql_free_result(res);
		return OPH_ODB_TOO_MANY_ROWS;
	}

	*counter = mysql_num_rows(res);
	if(!(*counter))
	{
		mysql_free_result(res);
		return OPH_ODB_SUCCESS;
	}

	if(!(*datacube = (cube*)malloc(*counter*sizeof(cube))))
	{
		mysql_free_result(res);
		return OPH_ODB_MEMORY_ERROR;
	}

	int i=0;
	while ((row = mysql_fetch_row(res)) != NULL)
	{
  		(*datacube)[i].id_datacube=(row[0] ? (int)strtol(row[0], NULL, 10) : 0);
		(*datacube)[i].id_container=(row[1] ? (int)strtol(row[1], NULL, 10) : 0);
		i++;
	}
	mysql_free_result(res);
	return OPH_ODB_SUCCESS;
}

int oph_odb_insert_user(ophidiadb *oDB, const char* username)
{
	if(!oDB || !username) return OPH_ODB_NULL_PARAM;

	if( oph_odb_check_connection_to_ophidiadb(oDB)) return OPH_ODB_MYSQL_ERROR;

	char insertQuery[MYSQL_BUFLEN];
	int n = snprintf(insertQuery, MYSQL_BUFLEN, MYSQL_QUERY_INSERT_USER, username);
	if(n >= MYSQL_BUFLEN) return OPH_ODB_STR_BUFF_OVERFLOW;

	if (mysql_query(oDB->conn, insertQuery)) return OPH_ODB_MYSQL_ERROR;

	return OPH_ODB_SUCCESS;
}

int oph_odb_insert_user2(ophidiadb *oDB, const char* username, const char* password, const char* name, const char* surname, const char* email, const char* country)
{
	if(!oDB || !username) return OPH_ODB_NULL_PARAM;

	if( oph_odb_check_connection_to_ophidiadb(oDB)) return OPH_ODB_MYSQL_ERROR;

	char insertQuery[MYSQL_BUFLEN],tmp[MYSQL_BUFLEN];
	int n;
	if (country)
	{
		int idcountry = 0;

		n = snprintf(insertQuery, MYSQL_BUFLEN, MYSQL_QUERY_SELECT_COUNTRY, country);
		if(n >= MYSQL_BUFLEN) return OPH_ODB_STR_BUFF_OVERFLOW;

		if (mysql_query(oDB->conn, insertQuery)) return OPH_ODB_MYSQL_ERROR;

		MYSQL_RES *res;
		MYSQL_ROW row;
		res = mysql_store_result(oDB->conn);

		if((mysql_field_count(oDB->conn) != 1) || (mysql_num_rows(res) != 1))
		{
			mysql_free_result(res);
			return OPH_ODB_TOO_MANY_ROWS;
		}

		while ((row = mysql_fetch_row(res)) != NULL) idcountry = (row[0] ? (int)strtol(row[0], NULL, 10) : 0);
		mysql_free_result(res);

		snprintf(tmp,MYSQL_BUFLEN,"%d",idcountry);
	}
	else snprintf(tmp,MYSQL_BUFLEN,"NULL");

	n = snprintf(insertQuery, MYSQL_BUFLEN, MYSQL_QUERY_INSERT_USER2, username, password, name?"'":"", name?name:"NULL", name?"'":"", surname?"'":"", surname?surname:"NULL", surname?"'":"", email?"'":"", email?email:"NULL", email?"'":"", tmp);
	if(n >= MYSQL_BUFLEN) return OPH_ODB_STR_BUFF_OVERFLOW;

	if (mysql_query(oDB->conn, insertQuery)) return OPH_ODB_MYSQL_ERROR;

	return OPH_ODB_SUCCESS;
}

int oph_odb_delete_user(ophidiadb *oDB, const char* username)
{
	if(!oDB || !username) return OPH_ODB_NULL_PARAM;

	if( oph_odb_check_connection_to_ophidiadb(oDB)) return OPH_ODB_MYSQL_ERROR;

	char insertQuery[MYSQL_BUFLEN];
	int n = snprintf(insertQuery, MYSQL_BUFLEN, MYSQL_QUERY_DELETE_USER, username);
	if(n >= MYSQL_BUFLEN) return OPH_ODB_STR_BUFF_OVERFLOW;

	if (mysql_query(oDB->conn, insertQuery)) return OPH_ODB_MYSQL_ERROR;

	return OPH_ODB_SUCCESS;
}

int oph_odb_update_user(ophidiadb *oDB, const char* username, const char* password, const char* name, const char* surname, const char* email, const char* country)
{
	if(!oDB || !username) return OPH_ODB_NULL_PARAM;

	if( oph_odb_check_connection_to_ophidiadb(oDB)) return OPH_ODB_MYSQL_ERROR;

	char updateQuery[MYSQL_BUFLEN];
	int n;

	if (password)
	{
		n = snprintf(updateQuery, MYSQL_BUFLEN, MYSQL_QUERY_UPDATE_USER2, "password", password, username);
		if(n >= MYSQL_BUFLEN) return OPH_ODB_STR_BUFF_OVERFLOW;

		if (mysql_query(oDB->conn, updateQuery)) return OPH_ODB_MYSQL_ERROR;
	}
	if (name)
	{
		n = snprintf(updateQuery, MYSQL_BUFLEN, MYSQL_QUERY_UPDATE_USER, "name", name, username);
		if(n >= MYSQL_BUFLEN) return OPH_ODB_STR_BUFF_OVERFLOW;

		if (mysql_query(oDB->conn, updateQuery)) return OPH_ODB_MYSQL_ERROR;
	}
	if (surname)
	{
		n = snprintf(updateQuery, MYSQL_BUFLEN, MYSQL_QUERY_UPDATE_USER, "surname", surname, username);
		if(n >= MYSQL_BUFLEN) return OPH_ODB_STR_BUFF_OVERFLOW;

		if (mysql_query(oDB->conn, updateQuery)) return OPH_ODB_MYSQL_ERROR;
	}
	if (email)
	{
		n = snprintf(updateQuery, MYSQL_BUFLEN, MYSQL_QUERY_UPDATE_USER, "mail", email, username);
		if(n >= MYSQL_BUFLEN) return OPH_ODB_STR_BUFF_OVERFLOW;

		if (mysql_query(oDB->conn, updateQuery)) return OPH_ODB_MYSQL_ERROR;
	}
	if (country)
	{
		int idcountry = 0;

		n = snprintf(updateQuery, MYSQL_BUFLEN, MYSQL_QUERY_SELECT_COUNTRY, country);
		if(n >= MYSQL_BUFLEN) return OPH_ODB_STR_BUFF_OVERFLOW;

		if (mysql_query(oDB->conn, updateQuery)) return OPH_ODB_MYSQL_ERROR;

		MYSQL_RES *res;
		MYSQL_ROW row;
		res = mysql_store_result(oDB->conn);

		if((mysql_field_count(oDB->conn) != 1) || (mysql_num_rows(res) != 1))
		{
			mysql_free_result(res);
			return OPH_ODB_TOO_MANY_ROWS;
		}

		while ((row = mysql_fetch_row(res)) != NULL) idcountry = (row[0] ? (int)strtol(row[0], NULL, 10) : 0);
		mysql_free_result(res);

		char tmp[MYSQL_BUFLEN];
		snprintf(tmp,MYSQL_BUFLEN,"%d",idcountry);
		n = snprintf(updateQuery, MYSQL_BUFLEN, MYSQL_QUERY_UPDATE_USER, "idcountry", tmp, username);
		if(n >= MYSQL_BUFLEN) return OPH_ODB_STR_BUFF_OVERFLOW;

		if (mysql_query(oDB->conn, updateQuery)) return OPH_ODB_MYSQL_ERROR;
	}

	return OPH_ODB_SUCCESS;
}
