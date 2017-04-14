/*
 * Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * TODO:
 * Use standard cpufreq APIs to limit CPU min/max frequencies
 *
 */

#include <linux/cpu.h>
#include <linux/cpufreq.h>
#include <linux/module.h>
#include <mach/cpufreq.h>

static struct kobject *kobj;
static uint32_t scaling_max_freq = CONFIG_MSM_CPU_FREQ_MAX;
static uint32_t scaling_min_freq = CONFIG_MSM_CPU_FREQ_MIN;

static int update_cpu_freq_limits(unsigned int cpu,
			uint32_t min_freq, uint32_t max_freq)
{
	int ret;

	ret = msm_cpufreq_set_freq_limits(cpu, min_freq, max_freq);
	if (ret)
		goto err;

	ret = cpufreq_update_policy(cpu);

err:
	return ret;
}

static ssize_t show_scaling_min_freq(struct kobject *kobj,
			struct attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", scaling_min_freq);
}

static ssize_t store_scaling_min_freq(struct kobject *kobj,
			struct attribute *attr, const char *buf, size_t count)
{
	int ret;
	unsigned long new_freq;
	uint32_t cpu;

	ret = kstrtoul(buf, 0, &new_freq);
	if (ret < 0)
		return ret;

	for_each_possible_cpu(cpu) {
		ret = update_cpu_freq_limits(cpu, new_freq, scaling_max_freq);
		if (ret)
			pr_debug("%s: Failed to limit cpu%u min freq to %lu\n",
				__func__, cpu, new_freq);
	}

	scaling_min_freq = new_freq;

	return count;
}

static struct global_attr scaling_min_freq_attr = __ATTR(scaling_min_freq,
		S_IRUGO | S_IWUSR | S_IWGRP,
		show_scaling_min_freq,
		store_scaling_min_freq);

static ssize_t show_scaling_max_freq(struct kobject *kobj,
			struct attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", scaling_max_freq);
}

static ssize_t store_scaling_max_freq(struct kobject *kobj,
			struct attribute *attr, const char *buf, size_t count)
{
	int ret;
	unsigned long new_freq;
	uint32_t cpu;

	ret = kstrtoul(buf, 0, &new_freq);
	if (ret < 0)
		return ret;

	for_each_possible_cpu(cpu) {
		ret = update_cpu_freq_limits(cpu, scaling_min_freq, new_freq);
		if (ret)
			pr_debug("%s: Failed to limit cpu%u max freq to %lu\n",
				__func__, cpu, new_freq);
	}

	scaling_max_freq = new_freq;

	return count;
}

static struct global_attr scaling_max_freq_attr = __ATTR(scaling_max_freq,
		S_IRUGO | S_IWUSR | S_IWGRP,
		show_scaling_max_freq,
		store_scaling_max_freq);

static struct attribute *cpufreq_limit_attributes[] = {
	&scaling_max_freq_attr.attr,
	&scaling_min_freq_attr.attr,
	NULL
};

static struct attribute_group cpufreq_limit_attr_group = {
	.attrs = cpufreq_limit_attributes,
	.name = "cpufreq",
};

static int cpufreq_limit_init(void)
{
	int ret;

	kobj = kobject_create_and_add("cpufreq_limit", kernel_kobj);
	ret = sysfs_create_group(kobj, &cpufreq_limit_attr_group);
	if (ret)
		pr_err("%s: sysfs_creation failed, ret=%d\n", __func__, ret);

	return ret;
}

static void cpufreq_limit_exit(void)
{
	sysfs_remove_group(kobj, &cpufreq_limit_attr_group);
	kobject_del(kobj);
}

module_init(cpufreq_limit_init);
module_exit(cpufreq_limit_exit);
