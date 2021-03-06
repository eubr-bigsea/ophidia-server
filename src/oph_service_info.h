/*
    Ophidia Server
    Copyright (C) 2012-2018 CMCC Foundation

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

#ifndef OPH_SERVICE_INFO_H
#define OPH_SERVICE_INFO_H

typedef struct _oph_service_info {
	unsigned int incoming_requests;
	unsigned int accepted_requests;
	unsigned int authorized_requests;
	unsigned int incoming_workflows;
	unsigned int accepted_workflows;
	unsigned int incoming_tasks;
	unsigned int submitted_tasks;
	unsigned int incoming_notifications;
	unsigned int incoming_responses;
	unsigned int outcoming_responses;
} oph_service_info;

#endif				/* OPH_SERVICE_INFO_H */
