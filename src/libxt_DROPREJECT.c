#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <xtables.h>
#include <limits.h> /* INT_MAX in ip_tables.h */
#include <linux/netfilter/nf_nat.h>
#include <arpa/inet.h>
#include "libxt_DROPREJECT.h"

static const struct xt_option_entry DROPREJECT_opts[] = {
	XTOPT_TABLEEND
};

static void DROPREJECT_help(void)
{
	printf(
"DROPREJECT target options: [none]\n"
);
}

static void DROPREJECT_print(const void *ip, const struct xt_entry_target *target,
                       int numeric)
{
	const struct xt_DROPREJECT *info = (void *) target->data;
	printf(" DROPREJECT");
}

static void DROPREJECT_save(const void *ip, const struct xt_entry_target *target)
{
}

static struct xtables_target DROPREJECT_tg_reg = {
	.name		= "DROPREJECT",
    .revision      = 0,
	.version       = XTABLES_VERSION,
	.family		= NFPROTO_IPV4,
	.size		= XT_ALIGN(sizeof(struct xt_DROPREJECT)),
	.userspacesize	= XT_ALIGN(sizeof(struct xt_DROPREJECT)),
	.print		= DROPREJECT_print,
	.help = DROPREJECT_help,
	.x6_options	= DROPREJECT_opts,
	.save = DROPREJECT_save
};

#ifndef _init
#define _init __attribute__((constructor)) _INIT
#endif
void _init(void)
{
	xtables_register_target(&DROPREJECT_tg_reg);
}
