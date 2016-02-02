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

#ifndef __OPH_WORKFLOW_FUNCTIONS_H
#define __OPH_WORKFLOW_FUNCTIONS_H

#include "oph_workflow_structs.h"

/* \brief Function for JSON parsing and global variables substitution (ncores,cwd and cube)
 * \param json_string Input JSON string
 * \param username Input username
 * \param workflow Output OPH_WORKFLOW
 * \return 0 if successful
 */
int oph_workflow_load(char *json_string, const char *username, oph_workflow **workflow);

/* \brief Function for JSON parsing and global variables substitution (ncores,cwd and cube)
 * \param workflow Input OPH_WORKFLOW
 * \param json_string Output JSON string
 * \return 0 if successful
 */
int oph_workflow_store(oph_workflow *workflow, char **json_string);

/* \brief Function to index task names in deps and init dependents_indexes
 * \param tasks Array of tasks
 * \param tasks_num Number of tasks
 * \return 0 if successful
 */
int oph_workflow_indexing(oph_workflow_task *tasks, int tasks_num);

/* \brief Function for cyclic dependencies check
 * \param workflow Input OPH_WORKFLOW to be validated
 * \return 0 if successful
 */
int oph_workflow_validate(oph_workflow *workflow);

/* \brief Function for the setup of the initial array of independent tasks
 * \param tasks Array of tasks
 * \param tasks_num Number of tasks
 * \param initial_tasks_indexes Output array of initial tasks indexes
 * \param initial_tasks_indexes_num Number of initial tasks
 * \return 0 if successful
 */
int oph_workflow_init(oph_workflow_task *tasks, int tasks_num, int **initial_tasks_indexes, int *initial_tasks_indexes_num);

/* \brief Function for extracting the submission string
 * \param workflow Input OPH_WORKFLOW
 * \param task_index Input index of the target task
 * \param light_task_index Input index of the target light task
 * \param long_submission_string Output string to be submitted to resource manager; the string has to be freed
 * \param short_submission_string Output string in short format; the string has to be freed; it can NULL
 * \param error Output string indicating a possible error; the string has to be freed; it can NULL
 * \return 0 if successful
 */
int oph_workflow_get_submission_string(oph_workflow *workflow, int task_index, int light_task_index, char** long_submission_string, char** short_submission_string, char** error);

/* \brief Function for extracting the string submitted by the user
 * \param workflow Input OPH_WORKFLOW
 * \param task_index Input index of the target task
 * \param light_task_index Input index of the target light task
 * \param submitted_string Command string equivalent to workflow; the string has to be freed
 * \return 0 if successful
 */
int oph_workflow_get_submitted_string(oph_workflow *workflow, int task_index, int light_task_index, int show_callback, char** submitted_string);

/* \brief Function for checking dependences
 * \param workflow Input OPH_WORKFLOW
 * \param p Index of parent task
 * \param c Index of child task
 * \return 1 if the task c depends on the task p
 */
int oph_workflow_is_child_of(oph_workflow *wf, int p, int c);

#endif //__OPH_WORKFLOW_FUNCTIONS_H