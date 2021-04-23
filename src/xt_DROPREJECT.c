/*
 * (C) 2017 Mathew Heard <mheard@x4b.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netfilter.h>
#include <net/ip.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include "libxt_DROPREJECT.h"
#include <net/netfilter/ipv4/nf_reject.h>
#include <linux/ratelimit.h>

struct ratelimit_state rs;

static unsigned int
xt_DROPREJECT_target(struct sk_buff *skb, const struct xt_action_param *par)
{
	struct iphdr *iph = ip_hdr(skb);

	if(iph->ttl == 1 && __ratelimit(&rs)){
		return NF_ACCEPT;
	}

	return NF_DROP;
}

static struct xt_target xt_nat_target_reg[] __read_mostly = {
	{
		.name		= "DROPREJECT",
		.revision	= 0,
		.target		= xt_DROPREJECT_target,
		.targetsize	= sizeof(struct xt_DROPREJECT),
		.hooks		= (1 << NF_INET_PRE_ROUTING) |
				  (1 << NF_INET_LOCAL_IN) |
				  (1 << NF_INET_LOCAL_OUT) | 
				  (1 << NF_INET_POST_ROUTING),
		.me		= THIS_MODULE,
	}
};

static int __init xt_nat_init(void)
{
	// no more than 5k packets per second get rejected in this module
	ratelimit_state_init(&rs, HZ, 1000);

	return xt_register_targets(xt_nat_target_reg,
				   ARRAY_SIZE(xt_nat_target_reg));
}

static void __exit xt_nat_exit(void)
{
	xt_unregister_targets(xt_nat_target_reg, ARRAY_SIZE(xt_nat_target_reg));
}

module_init(xt_nat_init);
module_exit(xt_nat_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mathew Heard <mheard@x4b.net>");
MODULE_ALIAS("ipt_DROPREJECT");;
MODULE_ALIAS("ip6t_DROPREJECT");