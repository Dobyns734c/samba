/*
 *  Samba Unix/Linux SMB client library
 *  Distributed SMB/CIFS Server Management Utility
 *  Local configuration interface
 *  Copyright (C) Michael Adam 2007-2008
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This is an interface to Samba's configuration as made available
 * by the libnet_conf interface (source/libnet/libnet_conf.c).
 *
 * This currently supports local interaction with the configuration
 * stored in the registry. But other backends and remote access via
 * rpc might get implemented in the future.
 */

#include "includes.h"
#include "utils/net.h"
#include "libnet/libnet.h"

/**********************************************************************
 *
 * usage functions
 *
 **********************************************************************/

static int net_conf_list_usage(int argc, const char **argv)
{
	d_printf("USAGE: net conf list\n");
	return -1;
}

static int net_conf_import_usage(int argc, const char**argv)
{
	d_printf("USAGE: net conf import [--test|-T] <filename> "
		 "[<servicename>]\n"
		 "\t[--test|-T]    testmode - do not act, just print "
			"what would be done\n"
		 "\t<servicename>  only import service <servicename>, "
			"ignore the rest\n");
	return -1;
}

static int net_conf_listshares_usage(int argc, const char **argv)
{
	d_printf("USAGE: net conf listshares\n");
	return -1;
}

static int net_conf_drop_usage(int argc, const char **argv)
{
	d_printf("USAGE: net conf drop\n");
	return -1;
}

static int net_conf_showshare_usage(int argc, const char **argv)
{
	d_printf("USAGE: net conf showshare <sharename>\n");
	return -1;
}

static int net_conf_addshare_usage(int argc, const char **argv)
{
	d_printf("USAGE: net conf addshare <sharename> <path> "
		 "[writeable={y|N} [guest_ok={y|N} [<comment>]]\n"
		 "\t<sharename>      the new share name.\n"
		 "\t<path>           the path on the filesystem to export.\n"
		 "\twriteable={y|N}  set \"writeable to \"yes\" or "
		 "\"no\" (default) on this share.\n"
		 "\tguest_ok={y|N}   set \"guest ok\" to \"yes\" or "
		 "\"no\" (default)   on this share.\n"
		 "\t<comment>        optional comment for the new share.\n");
	return -1;
}

static int net_conf_delshare_usage(int argc, const char **argv)
{
	d_printf("USAGE: net conf delshare <sharename>\n");
	return -1;
}

static int net_conf_setparm_usage(int argc, const char **argv)
{
	d_printf("USAGE: net conf setparm <section> <param> <value>\n");
	return -1;
}

static int net_conf_getparm_usage(int argc, const char **argv)
{
	d_printf("USAGE: net conf getparm <section> <param>\n");
	return -1;
}

static int net_conf_delparm_usage(int argc, const char **argv)
{
	d_printf("USAGE: net conf delparm <section> <param>\n");
	return -1;
}


/**********************************************************************
 *
 * Helper functions
 *
 **********************************************************************/

/**
 * This formats an in-memory smbconf parameter to a string.
 * The result string is allocated with talloc.
 */
static char *parm_valstr(TALLOC_CTX *ctx, struct parm_struct *parm,
			 struct share_params *share)
{
	char *valstr = NULL;
	int i = 0;
	void *ptr = parm->ptr;

	if (parm->p_class == P_LOCAL && share->service >= 0) {
		ptr = lp_local_ptr(share->service, ptr);
	}

	switch (parm->type) {
	case P_CHAR:
		valstr = talloc_asprintf(ctx, "%c", *(char *)ptr);
		break;
	case P_STRING:
	case P_USTRING:
		valstr = talloc_asprintf(ctx, "%s", *(char **)ptr);
		break;
	case P_BOOL:
		valstr = talloc_asprintf(ctx, "%s", BOOLSTR(*(bool *)ptr));
		break;
	case P_BOOLREV:
		valstr = talloc_asprintf(ctx, "%s", BOOLSTR(!*(bool *)ptr));
		break;
	case P_ENUM:
		for (i = 0; parm->enum_list[i].name; i++) {
			if (*(int *)ptr == parm->enum_list[i].value)
			{
				valstr = talloc_asprintf(ctx, "%s",
					parm->enum_list[i].name);
				break;
			}
		}
		break;
	case P_OCTAL: {
		char *o = octal_string(*(int *)ptr);
		valstr = talloc_move(ctx, &o);
		break;
	}
	case P_LIST:
		valstr = talloc_strdup(ctx, "");
		if ((char ***)ptr && *(char ***)ptr) {
			char **list = *(char ***)ptr;
			for (; *list; list++) {
				/* surround strings with whitespace
				 * in double quotes */
				if (strchr_m(*list, ' '))
				{
					valstr = talloc_asprintf_append(
						valstr, "\"%s\"%s",
						*list,
						 ((*(list+1))?", ":""));
				} else {
					valstr = talloc_asprintf_append(
						valstr, "%s%s", *list,
						 ((*(list+1))?", ":""));
				}
			}
		}
		break;
	case P_INTEGER:
		valstr = talloc_asprintf(ctx, "%d", *(int *)ptr);
		break;
	case P_SEP:
		break;
	default:
		valstr = talloc_asprintf(ctx, "<type unimplemented>\n");
		break;
	}

	return valstr;
}

/**
 * This functions imports a configuration that has previously
 * been loaded with lp_load() to registry.
 */
static int import_process_service(TALLOC_CTX *ctx,
				  struct libnet_conf_ctx *conf_ctx,
				  struct share_params *share)
{
	int ret = -1;
	struct parm_struct *parm;
	int pnum = 0;
	const char *servicename;
	WERROR werr;
	char *valstr = NULL;
	TALLOC_CTX *tmp_ctx = NULL;

	tmp_ctx = talloc_new(ctx);
	if (tmp_ctx == NULL) {
		werr = WERR_NOMEM;
		goto done;
	}

	servicename = (share->service == GLOBAL_SECTION_SNUM)?
		GLOBAL_NAME : lp_servicename(share->service);

	if (opt_testmode) {
		d_printf("[%s]\n", servicename);
	} else {
		if (libnet_conf_share_exists(conf_ctx, servicename)) {
			werr = libnet_conf_delete_share(conf_ctx, servicename);
			if (!W_ERROR_IS_OK(werr)) {
				goto done;
			}
		}
		werr = libnet_conf_create_share(conf_ctx, servicename);
		if (!W_ERROR_IS_OK(werr)) {
			goto done;
		}
	}

	while ((parm = lp_next_parameter(share->service, &pnum, 0)))
	{
		if ((share->service < 0) && (parm->p_class == P_LOCAL)
		    && !(parm->flags & FLAG_GLOBAL))
		{
			continue;
		}

		valstr = parm_valstr(tmp_ctx, parm, share);

		if (parm->type != P_SEP) {
			if (opt_testmode) {
				d_printf("\t%s = %s\n", parm->label, valstr);
			} else {
				werr = libnet_conf_set_parameter(conf_ctx,
								 servicename,
								 parm->label,
								 valstr);
				if (!W_ERROR_IS_OK(werr)) {
					d_fprintf(stderr,
						  "Error setting parameter '%s'"
						  ": %s\n", parm->label,
						   dos_errstr(werr));
					goto done;
				}
			}
		}
	}

	if (opt_testmode) {
		d_printf("\n");
	}

	ret = 0;

done:
	TALLOC_FREE(tmp_ctx);
	return ret;
}

/**
 * Return true iff there are nondefault globals in the
 * currently loaded configuration.
 */
static bool globals_exist(void)
{
	int i = 0;
	struct parm_struct *parm;

	while ((parm = lp_next_parameter(GLOBAL_SECTION_SNUM, &i, 0)) != NULL) {
		if (parm->type != P_SEP) {
			return true;
		}
	}
	return false;
}


/**********************************************************************
 *
 * the main conf functions
 *
 **********************************************************************/

static int net_conf_list(struct libnet_conf_ctx *conf_ctx,
			 int argc, const char **argv)
{
	WERROR werr = WERR_OK;
	int ret = -1;
	TALLOC_CTX *ctx;
	uint32_t num_shares;
	char **share_names;
	uint32_t *num_params;
	char ***param_names;
	char ***param_values;
	uint32_t share_count, param_count;

	ctx = talloc_init("list");

	if (argc != 0) {
		net_conf_list_usage(argc, argv);
		goto done;
	}

	werr = libnet_conf_get_config(ctx, conf_ctx, &num_shares, &share_names,
				      &num_params, &param_names, &param_values);
	if (!W_ERROR_IS_OK(werr)) {
		d_fprintf(stderr, "Error getting config: %s\n",
			  dos_errstr(werr));
		goto done;
	}

	for (share_count = 0; share_count < num_shares; share_count++) {
		d_printf("[%s]\n", share_names[share_count]);
		for (param_count = 0; param_count < num_params[share_count];
		     param_count++)
		{
			d_printf("\t%s = %s\n",
				 param_names[share_count][param_count],
				 param_values[share_count][param_count]);
		}
		d_printf("\n");
	}

	ret = 0;

done:
	TALLOC_FREE(ctx);
	return ret;
}

static int net_conf_import(struct libnet_conf_ctx *conf_ctx,
			   int argc, const char **argv)
{
	int ret = -1;
	const char *filename = NULL;
	const char *servicename = NULL;
	bool service_found = false;
	TALLOC_CTX *ctx;
	struct share_iterator *shares;
	struct share_params *share;
	struct share_params global_share = { GLOBAL_SECTION_SNUM };

	ctx = talloc_init("net_conf_import");

	switch (argc) {
		case 0:
		default:
			net_conf_import_usage(argc, argv);
			goto done;
		case 2:
			servicename = argv[1];
		case 1:
			filename = argv[0];
			break;
	}

	DEBUG(3,("net_conf_import: reading configuration from file %s.\n",
		filename));

	if (!lp_load(filename,
		     false,     /* global_only */
		     true,      /* save_defaults */
		     false,     /* add_ipc */
		     true))     /* initialize_globals */
	{
		d_fprintf(stderr, "Error parsing configuration file.\n");
		goto done;
	}

	if (opt_testmode) {
		d_printf("\nTEST MODE - "
			 "would import the following configuration:\n\n");
	}

	if (((servicename == NULL) && globals_exist()) ||
	    strequal(servicename, GLOBAL_NAME))
	{
		service_found = true;
		if (import_process_service(ctx, conf_ctx, &global_share) != 0) {
			goto done;
		}
	}

	if (service_found && (servicename != NULL)) {
		ret = 0;
		goto done;
	}

	if (!(shares = share_list_all(ctx))) {
		d_fprintf(stderr, "Could not list shares...\n");
		goto done;
	}
	while ((share = next_share(shares)) != NULL) {
		if ((servicename == NULL)
		    || strequal(servicename, lp_servicename(share->service)))
		{
			service_found = true;
			if (import_process_service(ctx, conf_ctx, share)!= 0) {
				goto done;
			}
		}
	}

	if ((servicename != NULL) && !service_found) {
		d_printf("Share %s not found in file %s\n",
			 servicename, filename);
		goto done;

	}

	ret = 0;

done:
	TALLOC_FREE(ctx);
	return ret;
}

static int net_conf_listshares(struct libnet_conf_ctx *conf_ctx,
			       int argc, const char **argv)
{
	WERROR werr = WERR_OK;
	int ret = -1;
	uint32_t count, num_shares = 0;
	char **share_names = NULL;
	TALLOC_CTX *ctx;

	ctx = talloc_init("listshares");

	if (argc != 0) {
		net_conf_listshares_usage(argc, argv);
		goto done;
	}

	werr = libnet_conf_get_share_names(ctx, conf_ctx, &num_shares,
					   &share_names);
	if (!W_ERROR_IS_OK(werr)) {
		goto done;
	}

	for (count = 0; count < num_shares; count++)
	{
		d_printf("%s\n", share_names[count]);
	}

	ret = 0;

done:
	TALLOC_FREE(ctx);
	return ret;
}

static int net_conf_drop(struct libnet_conf_ctx *conf_ctx,
			 int argc, const char **argv)
{
	int ret = -1;
	WERROR werr;

	if (argc != 0) {
		net_conf_drop_usage(argc, argv);
		goto done;
	}

	werr = libnet_conf_drop(conf_ctx);
	if (!W_ERROR_IS_OK(werr)) {
		d_fprintf(stderr, "Error deleting configuration: %s\n",
			  dos_errstr(werr));
		goto done;
	}

	ret = 0;

done:
	return ret;
}

static int net_conf_showshare(struct libnet_conf_ctx *conf_ctx,
			      int argc, const char **argv)
{
	int ret = -1;
	WERROR werr = WERR_OK;
	const char *sharename = NULL;
	TALLOC_CTX *ctx;
	uint32_t num_params;
	uint32_t count;
	char **param_names;
	char **param_values;

	ctx = talloc_init("showshare");

	if (argc != 1) {
		net_conf_showshare_usage(argc, argv);
		goto done;
	}

	sharename = argv[0];

	werr = libnet_conf_get_share(ctx, conf_ctx, sharename, &num_params,
				     &param_names, &param_values);
	if (!W_ERROR_IS_OK(werr)) {
		d_printf("error getting share parameters: %s\n",
			 dos_errstr(werr));
		goto done;
	}

	d_printf("[%s]\n", sharename);

	for (count = 0; count < num_params; count++) {
		d_printf("\t%s = %s\n", param_names[count],
			 param_values[count]);
	}

	ret = 0;

done:
	TALLOC_FREE(ctx);
	return ret;
}

/**
 * Add a share, with a couple of standard parameters, partly optional.
 *
 * This is a high level utility function of the net conf utility,
 * not a direct frontend to the libnet_conf API.
 */
static int net_conf_addshare(struct libnet_conf_ctx *conf_ctx,
			     int argc, const char **argv)
{
	int ret = -1;
	WERROR werr = WERR_OK;
	char *sharename = NULL;
	const char *path = NULL;
	const char *comment = NULL;
	const char *guest_ok = "no";
	const char *writeable = "no";
	SMB_STRUCT_STAT sbuf;

	switch (argc) {
		case 0:
		case 1:
		default:
			net_conf_addshare_usage(argc, argv);
			goto done;
		case 5:
			comment = argv[4];
		case 4:
			if (!strnequal(argv[3], "guest_ok=", 9)) {
				net_conf_addshare_usage(argc, argv);
				goto done;
			}
			switch (argv[3][9]) {
				case 'y':
				case 'Y':
					guest_ok = "yes";
					break;
				case 'n':
				case 'N':
					guest_ok = "no";
					break;
				default:
					net_conf_addshare_usage(argc, argv);
					goto done;
			}
		case 3:
			if (!strnequal(argv[2], "writeable=", 10)) {
				net_conf_addshare_usage(argc, argv);
				goto done;
			}
			switch (argv[2][10]) {
				case 'y':
				case 'Y':
					writeable = "yes";
					break;
				case 'n':
				case 'N':
					writeable = "no";
					break;
				default:
					net_conf_addshare_usage(argc, argv);
					goto done;
			}
		case 2:
			path = argv[1];
			sharename = strdup_lower(argv[0]);
			break;
	}

	/*
	 * validate arguments
	 */

	/* validate share name */

	if (!validate_net_name(sharename, INVALID_SHARENAME_CHARS,
			       strlen(sharename)))
	{
		d_fprintf(stderr, "ERROR: share name %s contains "
                        "invalid characters (any of %s)\n",
                        sharename, INVALID_SHARENAME_CHARS);
		goto done;
	}

	if (getpwnam(sharename)) {
		d_fprintf(stderr, "ERROR: share name %s is already a valid "
			  "system user name.\n", sharename);
		goto done;
	}

	if (strequal(sharename, GLOBAL_NAME)) {
		d_fprintf(stderr,
			  "ERROR: 'global' is not a valid share name.\n");
		goto done;
	}

	if (libnet_conf_share_exists(conf_ctx, sharename)) {
		d_fprintf(stderr, "ERROR: share %s already exists.\n",
			  sharename);
		goto done;
	}

	/* validate path */

	if (path[0] != '/') {
		d_fprintf(stderr,
			  "Error: path '%s' is not an absolute path.\n",
			  path);
		goto done;
	}

	if (sys_stat(path, &sbuf) != 0) {
		d_fprintf(stderr,
			  "ERROR: cannot stat path '%s' to ensure "
			  "this is a directory.\n"
			  "Error was '%s'.\n",
			  path, strerror(errno));
		goto done;
	}

	if (!S_ISDIR(sbuf.st_mode)) {
		d_fprintf(stderr,
			  "ERROR: path '%s' is not a directory.\n",
			  path);
		goto done;
	}

	/*
	 * create the share
	 */

	werr = libnet_conf_create_share(conf_ctx, sharename);
	if (!W_ERROR_IS_OK(werr)) {
		d_fprintf(stderr, "Error creating share %s: %s\n",
			  sharename, dos_errstr(werr));
		goto done;
	}

	/*
	 * fill the share with parameters
	 */

	werr = libnet_conf_set_parameter(conf_ctx, sharename, "path", path);
	if (!W_ERROR_IS_OK(werr)) {
		d_fprintf(stderr, "Error setting parameter %s: %s\n",
			  "path", dos_errstr(werr));
		goto done;
	}

	if (comment != NULL) {
		werr = libnet_conf_set_parameter(conf_ctx, sharename, "comment",
						 comment);
		if (!W_ERROR_IS_OK(werr)) {
			d_fprintf(stderr, "Error setting parameter %s: %s\n",
				  "comment", dos_errstr(werr));
			goto done;
		}
	}

	werr = libnet_conf_set_parameter(conf_ctx, sharename, "guest ok",
					 guest_ok);
	if (!W_ERROR_IS_OK(werr)) {
		d_fprintf(stderr, "Error setting parameter %s: %s\n",
			  "'guest ok'", dos_errstr(werr));
		goto done;
	}

	werr = libnet_conf_set_parameter(conf_ctx, sharename, "writeable",
					 writeable);
	if (!W_ERROR_IS_OK(werr)) {
		d_fprintf(stderr, "Error setting parameter %s: %s\n",
			  "writeable", dos_errstr(werr));
		goto done;
	}

	ret = 0;

done:
	SAFE_FREE(sharename);
	return ret;
}

static int net_conf_delshare(struct libnet_conf_ctx *conf_ctx,
			     int argc, const char **argv)
{
	int ret = -1;
	const char *sharename = NULL;
	WERROR werr = WERR_OK;

	if (argc != 1) {
		net_conf_delshare_usage(argc, argv);
		goto done;
	}
	sharename = argv[0];

	werr = libnet_conf_delete_share(conf_ctx, sharename);
	if (!W_ERROR_IS_OK(werr)) {
		d_fprintf(stderr, "Error deleting share %s: %s\n",
			  sharename, dos_errstr(werr));
		goto done;
	}

	ret = 0;
done:
	return ret;
}

static int net_conf_setparm(struct libnet_conf_ctx *conf_ctx,
			    int argc, const char **argv)
{
	int ret = -1;
	WERROR werr = WERR_OK;
	char *service = NULL;
	char *param = NULL;
	const char *value_str = NULL;

	if (argc != 3) {
		net_conf_setparm_usage(argc, argv);
		goto done;
	}
	service = strdup_lower(argv[0]);
	param = strdup_lower(argv[1]);
	value_str = argv[2];

	if (!libnet_conf_share_exists(conf_ctx, service)) {
		werr = libnet_conf_create_share(conf_ctx, service);
		if (!W_ERROR_IS_OK(werr)) {
			d_fprintf(stderr, "Error creating share '%s': %s\n",
				  service, dos_errstr(werr));
			goto done;
		}
	}

	werr = libnet_conf_set_parameter(conf_ctx, service, param, value_str);

	if (!W_ERROR_IS_OK(werr)) {
		d_fprintf(stderr, "Error setting value '%s': %s\n",
			  param, dos_errstr(werr));
		goto done;
	}

	ret = 0;

done:
	SAFE_FREE(service);
	SAFE_FREE(param);
	return ret;
}

static int net_conf_getparm(struct libnet_conf_ctx *conf_ctx,
			    int argc, const char **argv)
{
	int ret = -1;
	WERROR werr = WERR_OK;
	char *service = NULL;
	char *param = NULL;
	char *valstr = NULL;
	TALLOC_CTX *ctx;

	ctx = talloc_init("getparm");

	if (argc != 2) {
		net_conf_getparm_usage(argc, argv);
		goto done;
	}
	service = strdup_lower(argv[0]);
	param = strdup_lower(argv[1]);

	werr = libnet_conf_get_parameter(ctx, conf_ctx, service, param, &valstr);

	if (W_ERROR_EQUAL(werr, WERR_NO_SUCH_SERVICE)) {
		d_fprintf(stderr,
			  "Error: given service '%s' does not exist.\n",
			  service);
		goto done;
	} else if (W_ERROR_EQUAL(werr, WERR_INVALID_PARAM)) {
		d_fprintf(stderr,
			  "Error: given parameter '%s' is not set.\n",
			  param);
		goto done;
	} else if (!W_ERROR_IS_OK(werr)) {
		d_fprintf(stderr, "Error getting value '%s': %s.\n",
			  param, dos_errstr(werr));
		goto done;
	}

	d_printf("%s\n", valstr);

	ret = 0;
done:
	SAFE_FREE(service);
	SAFE_FREE(param);
	TALLOC_FREE(ctx);
	return ret;
}

static int net_conf_delparm(struct libnet_conf_ctx *conf_ctx,
			    int argc, const char **argv)
{
	int ret = -1;
	WERROR werr = WERR_OK;
	char *service = NULL;
	char *param = NULL;

	if (argc != 2) {
		net_conf_delparm_usage(argc, argv);
		goto done;
	}
	service = strdup_lower(argv[0]);
	param = strdup_lower(argv[1]);

	werr = libnet_conf_delete_parameter(conf_ctx, service, param);

	if (W_ERROR_EQUAL(werr, WERR_NO_SUCH_SERVICE)) {
		d_fprintf(stderr,
			  "Error: given service '%s' does not exist.\n",
			  service);
		goto done;
	} else if (W_ERROR_EQUAL(werr, WERR_INVALID_PARAM)) {
		d_fprintf(stderr,
			  "Error: given parameter '%s' is not set.\n",
			  param);
		goto done;
	} else if (!W_ERROR_IS_OK(werr)) {
		d_fprintf(stderr, "Error deleting value '%s': %s.\n",
			  param, dos_errstr(werr));
		goto done;
	}

	ret = 0;

done:
	SAFE_FREE(service);
	SAFE_FREE(param);
	return ret;
}


/**********************************************************************
 *
 * Wrapper and net_conf_run_function mechanism.
 *
 **********************************************************************/

/**
 * Wrapper function to call the main conf functions.
 * The wrapper calls handles opening and closing of the
 * configuration.
 */
static int net_conf_wrap_function(int (*fn)(struct libnet_conf_ctx *,
					    int, const char **),
				  int argc, const char **argv)
{
	WERROR werr;
	TALLOC_CTX *mem_ctx = talloc_stackframe();
	struct libnet_conf_ctx *conf_ctx;
	int ret = -1;

	werr = libnet_conf_open(mem_ctx, &conf_ctx);

	if (!W_ERROR_IS_OK(werr)) {
		return -1;
	}

	ret = fn(conf_ctx, argc, argv);

	libnet_conf_close(conf_ctx);

	return ret;
}

/*
 * We need a functable struct of our own, because the
 * functions are called through a wrapper that handles
 * the opening and closing of the configuration, and so on.
 */
struct conf_functable {
	const char *funcname;
	int (*fn)(struct libnet_conf_ctx *ctx, int argc, const char **argv);
	const char *helptext;
};

/**
 * This imitates net_run_function2 but calls the main functions
 * through the wrapper net_conf_wrap_function().
 */
static int net_conf_run_function(int argc, const char **argv,
				 const char *whoami,
				 struct conf_functable *table)
{
	int i;

	if (argc != 0) {
		for (i=0; table[i].funcname; i++) {
			if (StrCaseCmp(argv[0], table[i].funcname) == 0)
				return net_conf_wrap_function(table[i].fn,
							      argc-1,
							      argv+1);
		}
	}

	for (i=0; table[i].funcname; i++) {
		d_printf("%s %-15s %s\n", whoami, table[i].funcname,
			 table[i].helptext);
	}

	return -1;
}

/*
 * Entry-point for all the CONF functions.
 */

int net_conf(int argc, const char **argv)
{
	int ret = -1;
	struct conf_functable func_table[] = {
		{"list", net_conf_list,
		 "Dump the complete configuration in smb.conf like format."},
		{"import", net_conf_import,
		 "Import configuration from file in smb.conf format."},
		{"listshares", net_conf_listshares,
		 "List the share names."},
		{"drop", net_conf_drop,
		 "Delete the complete configuration."},
		{"showshare", net_conf_showshare,
		 "Show the definition of a share."},
		{"addshare", net_conf_addshare,
		 "Create a new share."},
		{"delshare", net_conf_delshare,
		 "Delete a share."},
		{"setparm", net_conf_setparm,
		 "Store a parameter."},
		{"getparm", net_conf_getparm,
		 "Retrieve the value of a parameter."},
		{"delparm", net_conf_delparm,
		 "Delete a parameter."},
		{NULL, NULL, NULL}
	};

	ret = net_conf_run_function(argc, argv, "net conf", func_table);

	return ret;
}

