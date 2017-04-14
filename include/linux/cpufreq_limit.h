/*
 *  linux/include/linux/cpufreq_limit.h
 *
 *  Copyright (C) 2017 Paul Keith <javelinanddart@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _LINUX_CPUFREQ_LIMIT_H
#define _LINUX_CPUFREQ_LIMIT_H

/* Thermal throttling function for thermal driver */
extern void thermal_throttle(uint32_t max_freq, bool throttle);

#endif /* _LINUX_CPUFREQ_LIMIT_H */
