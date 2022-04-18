# Makefile for the Linux kernel AVM device drivers.
#
# 10th April 2022, Anurag Verma <newanurag@gmail.com>
# Rewritten to use lists instead of if-statements.
#


# Variables that are same for all platforms
DZ_CURRENT_VERSION             := 1.0

DZ_PROJECT_PATH                := $(srctree)
DZ_HEADERS_PATH                :=$(DZ_PROJECT_PATH)/mycompany/avm/headers/
DZ_HASH_PATH                   :=$(DZ_PROJECT_PATH)/mycompany/avm/hash_engine/
$(info,$(DZ_PROJECT_PATH))

# Kernel Module Names of their corresponding engines
# Dependency is represented as number sequence. Higher number value is dependent on lower number



subdir-ccflags-y        := -g -I$(DZ_HEADERS_PATH) -I$(DZ_HASH_PATH)
subdir-ccflags-y        += -DDZ_BUILD_FROM_KERNEL_SOURCE_TREE -DDZ_BIT_SPIN_LOCK -DDZ_KERNEL_VERSION_4 -DDZ_ARCH_ARM64
subdir-ccflags-y        += -Wno-bitfield-constant-conversion -Wall -Wfatal-errors


obj-m				+= log_engine/
obj-m				+= infra_engine/
obj-m				+= lab_engine/
obj-m				+= bec_engine/
obj-m				+= metadata_engine/
obj-m				+= dedupe_engine/
obj-m				+= alignment_engine/
obj-m				+= fec_engine/
obj-m				+= target_engine/
obj-m				+= volume_engine/
#obj-m				+= sysfs_engine/
#obj-m				+= cli_engine/
#obj-y				+= hash_engine/
