from __future__ import print_function
import os
import sys
import AISA_Error
from AISA_Error import *

#SUCCESS	= 0
#FAILURE	= 1

#ERROR	= "ERROR"
#INFO	= "INFO "
#WARN	= "WARN "
#DEBUG	= "DBUG "

AISA_LOCAL_URI_SUFFIX 		= ':///system'
AISA_DEFAULT_HYX 		= 'qemu'

#DM Target Defaults
AISA_DMTARGET_KERNEL_MODULE             = "aisa_dmtarget.ko"
AISA_DMTARGET_KERNEL_MODULE_PATH        = "/aisa/current/src/kernel/dmtarget"
AISA_DMTARGET_NAME                      = "aisa_dmtarget"
AISA_DMTARGET_BACKEND_DEVICE            = "/dev/md1"
AISA_DMTARGET_EXPORTED_DEVICE           = "aisp"
AISA_DMTARGET_EXPORTED_SIZE             = 2097152

# Storage Pool Information
AISA_AISP_PREFIX_DATA_LUN	= "lv_aidl_"
AISA_AISP_PREFIX_VM_LUN		= "lv_aivl_" #For Public VMs
AISA_AISP_PREFIX_VM_LUN_PVT	= "lvp_aivl_" #For Private VMs


AISA_STORAGE_POOL_PATH 			= '/dev/mapper/' + AISA_DMTARGET_EXPORTED_DEVICE
AISA_STORAGE_POOL_SIZE 			= 40 # in GB, Just like that
AISA_STORAGE_POOL_VG_NAME		= "vg_AISA_POOL"
AISA_STORAGE_POOL_LV_NAME_DATA_LUN	= "lv_AISA_POOL_DATA_LUN" # LV for creating Data Lun Pool
AISA_STORAGE_POOL_LV_NAME_VM_IMAGE	= "lv_AISA_POOL_VM_IMAGE" # LV for creating VirtualMachine Image Pool

AISA_DATA_LUN_POOL_PATH			= '/dev/' + AISA_STORAGE_POOL_VG_NAME + '/' + AISA_STORAGE_POOL_LV_NAME_DATA_LUN
AISA_VM_IMAGE_POOL_PATH			= '/dev/' + AISA_STORAGE_POOL_VG_NAME + '/' + AISA_STORAGE_POOL_LV_NAME_VM_IMAGE

AISA_DATA_LUN_POOL_VG_NAME		= "vg_DATA_LUN_POOL" # It will be created on AISA_DATA_LUN_POOL_PATH
AISA_VM_IMAGE_POOL_VG_NAME		= "vg_VM_IMAGE_POOL" # It will be created on AISA_VM_IMAGE_POOL_PATH

AISA_DATA_LUN_POOL_PERCENTAGE		= 60
AISA_VM_IMAGE_POOL_PERCENTAGE		= (100 - AISA_DATA_LUN_POOL_PERCENTAGE)

# VM Image Pool
AISA_STORAGE_PREFIX_VM_IMAGE_LUN	= "lv_aivi_"
AISA_STORAGE_VM_IMAGE_POOL_NAME		= "lv_aivi_pool"
AISA_STORAGE_VM_IMAGE_POOL_SIZE		= (AISA_STORAGE_POOL_SIZE / 2)


#MDRaid Data Disks
AISA_DATA_DISKS = 			'/dev/sda6  /dev/sda7  /dev/sda8  /dev/sda9  '
AISA_DATA_DISKS = AISA_DATA_DISKS +	'/dev/sda10 /dev/sda11 /dev/sda12 /dev/sda13 '
AISA_DATA_DISKS = AISA_DATA_DISKS +	'/dev/sda14 /dev/sda15 /dev/sda16 /dev/sda17 '
AISA_DATA_DISKS = AISA_DATA_DISKS +	'/dev/sda18 /dev/sda19 /dev/sda20 /dev/sda21 '

AISA_DATA_DISKS_CNT = 16

