## v1.2.0 - 2018-02-16

### Fixed:

- Unit tests
- Bug [#13](https://github.com/OphidiaBigData/ophidia-server/issues/13) related to large variables causing server crash when used in OPH_SET [#21](https://github.com/OphidiaBigData/ophidia-server/pull/21)
- Bug [#20](https://github.com/OphidiaBigData/ophidia-server/issues/20)
- Update resource manager to set specific MPI type in case only one core is used [#16](https://github.com/OphidiaBigData/ophidia-server/pull/16)
- Bug [#14](https://github.com/OphidiaBigData/ophidia-server/issues/14)
- Bug [#12](https://github.com/OphidiaBigData/ophidia-server/issues/12)
- Improve metadata management to prevent deadlocks [#11](https://github.com/OphidiaBigData/ophidia-server/pull/11)
- Bug [#10](https://github.com/OphidiaBigData/ophidia-server/issues/10)
- Bug [#9](https://github.com/OphidiaBigData/ophidia-server/issues/9)
- Locking for libcurl

### Added:

- Add accounting log [#19](https://github.com/OphidiaBigData/ophidia-server/pull/19)
- Feature AAAaaS [#17](https://github.com/OphidiaBigData/ophidia-server/pull/17)
- URL to workflow header [#15](https://github.com/OphidiaBigData/ophidia-server/pull/15)
- Workflow execution time as extra field
- Fields 'category' and 'multivalue' to XML descriptions
- Default values for configuration parameters of OpenId Connect
- OPH_CDD as parameter of user.dat and output of OPH_GET_CONFIG
- Timer to check for revoked tokens

### Changed:

- Update fill partition query to use datacubecount field [#18](https://github.com/OphidiaBigData/ophidia-server/pull/18)
- Look of web pages of session space
- Use subject identifier as userid for OpenId Connect
- Default values for configuration parameters

## v1.1.0 - 2017-07-28

### Fixed:

- Unit tests
- Bug [#8](https://github.com/OphidiaBigData/ophidia-server/issues/8)
- Bug [#7](https://github.com/OphidiaBigData/ophidia-server/issues/7)
- Limit the allowed characters for 'username'
- OPH_FOR to avoid to re-check the workflows in case the option "parallel" is not used
- OPH_WAIT to check files
- Fix bug in handling on_error flag in loops

### Added:

- Support for OpenId Conncect (INDIGO-DataCloud Project)
- Add BASE_SRC_PATH in client configuration parameters
- Fill extra metadata in responses of known operators (INDIGO-DataCloud Project)
- Multi-user support
- Add fine progress ratio evaluation based on number of fragments
- Add status logging and monitoring feature

### Changed:

- Disable SSH for task submission by default
- Add sessionid and markerid to success notifications
- Import massive operations based on the operator OPH_FS (INDIGO-DataCloud Project)
- JSON Response schema (version 1.1): add field 'extra' (INDIGO-DataCloud Project)
- OPH_RESUME to get workflow progress
- OPH_IF to forward input cube (INDIGO-DataCloud Project)
- Allow embedded variables
- Allow to use @ even if no variable is not set
- Save JSON Responses on behalf of the framework
- Trace operator log only in debug mode

## v1.0.0 - 2017-03-23

### Fixed:

- Localtime function calls to be reentrant

## v0.11.0 - 2017-01-31

### Fixed:

- Unit tests
- Bug that raises an error message at the end of leaf tasks
- Bug when a waiting task is terminated by OPH_CANCEL
- Bug in job monitoring procedure
- Bugs in building the list of cubes to be processed by 'exit action'

### Added:

- Use of BASE_SRC_PATH as prefix of files involved in massive and waiting operations (INDIGO-DataCloud Project)
- Parameter 'status_filter' to OPH_RESUME
- Status OPH_RUNNING_ERROR in workflow in case of error while it is still running

### Changed:

- Disabled usage of libSSH by default
- Exit status saved in OphidiaDB when workflow is aborted
- System command in case libSSH is not used
- Number of cores to execute each light task of final task to 1
- SSH connection method
- OPH_SERVICE to list running tasks
- Configuration parameters names in ophidiadb.conf

## v0.10.7 - 2016-11-15

### Fixed:

- Bug in testing OPH_WAIT (INDIGO-DataCloud Project)
- Bug [#6](https://github.com/OphidiaBigData/ophidia-server/issues/6)

### Added:

- Option 'host_partition' in workflow schema

### Changed:

- Improve code coverage of unit tests (INDIGO-DataCloud Project)

## v0.10.6 - 2016-10-20

### Fixed:

- Memory leaks in testing program
- Inclusion of curl.h in code coverage (INDIGO-DataCloud Project)
- Warning messages set in JSON response in case of successful tasks
- Bug in handling wrong expression with OPH_IF (INDIGO-DataCloud Project)
- Bug in setting exit status of OPH_ENDFOR (INDIGO-DataCloud Project)
- Compact output format in resuming status of running workflow 
- Improved procedure to set final operation arguments 
- Bug in OphidiaDB update when a massive operation is retried
- Bug in handling OPH_IF when Matheval is not enabled (INDIGO-DataCloud Project)
- Bug in job monitoring procedure
- Bug in auto-retry feature

### Added:

- New unit tests for OPH_WAIT, OPH_INPUT and OPH_SET (INDIGO-DataCloud Project)
- Htacces to define url redirect for Ophidia web section
- Argument to define type of operation in OPH_CANCEL
- BASE_BACKOFF configuration parameter
- BASE_SRC_PATH configuration parameter (INDIGO-DataCloud Project)
- New operator OPH_INPUT (INDIGO-DataCloud Project)
- New operator OPH_WAIT (INDIGO-DataCloud Project)

### Changed:

- Query of OPH_RESUME to return OPH_STATUS_WAITING in case there is at least a waiting task (INDIGO-DataCloud Project)
- Workflow status list; added OPH_STATUS_WAITING and OPH_STATUS_UNSELECTED (INDIGO-DataCloud Project)
- Policy to set auto-retry interval (backoff-based policy)
- OPH_FOR and OPH_SET input parameter "name" to "key" 
- Default value for SERVER_FARM_SIZE to 128

## v0.10.5 - 2016-08-24

### Fixed:

- Handler of SSH library used for task submission, locking, sessions, etc.
- Bug in setting basic notification message for massive operations (INDIGO-DataCloud Project)
- Skip 'lib' in referring to libmatheval
- Job status when OPH_START_ERROR occurs
- Operation to be executed when OPH_START_ERROR occurs in case of the final task
- Counter update of OPH_FOR when default values are used (INDIGO-DataCloud Project)
- Bug in building function 'oph_if_impl' when MathEval library is disabled (INDIGO-DataCloud Project)
- Bug in handling filter 'path' in massive operations (INDIGO-DataCloud Project)
- Bug in massive operation handler (INDIGO-DataCloud Project)
- Several warnings when building

### Added:

- Monitor of the submitted jobs
- Support for auto-retry in case of resource manager errors
- Check for return values of SSH-submitted commands
- Compact output format for JSON Responses associated with workflows and massive operations (INDIGO-DataCloud Project)
- Task type OPH_STATUS_UNSELECTED for unselected branches of selection statement (INDIGO-DataCloud Project)
- Code coverage check (INDIGO-DataCloud Project)
- Support for data type 'string' in multigrids of JSON Responses
- Unit tests (INDIGO-DataCloud Project)
- Support for selection statement (INDIGO-DataCloud Project)

### Changed:

- Status of skipped tasks to OPH_STATUS_SKIPPED in case of errors (INDIGO-DataCloud Project)
- Code indentation style (INDIGO-DataCloud Project)
- Library 'known operators' to improve modularity (INDIGO-DataCloud Project)

## v0.10.1 - 2016-06-27

### Fixed:

- Version number in files

## v0.10.0 - 2016-06-23

### Fixed:

- Bug in OPH_FOR parsing
- Warning 'unset variable' in oph_ssh_submit.c
- Bug in function for massive operation checking
- Bug in file matching during directory scanning
- Bug in handling subset strings for argument 'counter' of OPH_FOR
- Bug in handling on_exit:oph_delete in case of massive operations
- Bug [#4](https://github.com/OphidiaBigData/ophidia-server/issues/4)
- Bug [#2](https://github.com/OphidiaBigData/ophidia-server/issues/2)
- Bug [#1](https://github.com/OphidiaBigData/ophidia-server/issues/1)

### Added:

- Support for pre-defined workflow variables
- Support for hostname in IP_TARGET_HOST configuration parameter
- Support for building on CentOS7 and Ubuntu
- Handling of DT_UNKNOWN in parsing directories for import massive operations
- Support for SHA-coded passwords

### Changed:

- Improved oph_for and oph_set to handle vectors as symbolic references

## v0.9.0 - 2016-02-01

- Initial public release
