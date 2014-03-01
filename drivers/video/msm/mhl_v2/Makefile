# drivers/video/msm/mhl_v2/Makefile
#
# Copyright (c) 2010 Samsung Electronics Co., Ltd.
# 		http://www.samsung.com/
# Aakash Manik
#	aakash.manik@samsung.com


obj-$(CONFIG_SAMSUNG_MHL_8240) += sii8240/

ifeq ($(CONFIG_VIDEO_MHL_V2),y)
ifneq ($(CONFIG_SAMSUNG_MHL_8240),y)
obj-$(CONFIG_VIDEO_MHL_V2) += sii9234.o
endif
endif
