#! /usr/bin/python
#from AISA_AVMCore 	import *

CLI_SUCCESS									= 0
CLI_FAILURE									= -1
CLI_NOOP									= 1
CLI_DATA_DEVICE_DOES_NOT_EXIST				= -2
CLI_METADATA_DEVICE_DOES_NOT_EXIST			= -3
CLI_DEVICES_DOES_NOT_EXIST					= -4

DZ_CLI_VERSION								= "1.1.a"
DZ_DRIVER_VERSION							= "3.10.101"
DZ_CLI_PRODUCT								= "ZAIDSTOR AISA Volume Manager (AVM)"
DZ_CLI_CONFIG_FILE_PATH 					= "/etc/datumsoft/avm.conf"	

AVM_SYSFS_PARENT_PATH						= "/sys/avm/"
AVM_SYSFS_PARENT							= "/sys/avm/"

KERNEL_MODULE_LIST				= [
"datumsoft_zaidstor_avm_B_B_sysfs_engine_module",
"datumsoft_zaidstor_avm_A_B_volume_engine_module",
"datumsoft_zaidstor_avm_9_B_target_engine_module",
"datumsoft_zaidstor_avm_8_B_fec_engine_module",
"datumsoft_zaidstor_avm_7_B_alignment_engine_module",
"datumsoft_zaidstor_avm_6_B_dedupe_engine_module",
"datumsoft_zaidstor_avm_5_B_metadata_engine_module",
"datumsoft_zaidstor_avm_4_B_bec_engine_module",
"datumsoft_zaidstor_avm_3_B_lab_engine_module",
"datumsoft_zaidstor_avm_2_B_infra_engine_module",
"datumsoft_zaidstor_avm_1_B_log_engine_module",]


CLI_OP_TYPE_LIST 							= ["--list", 	"-l",	"ls", "list"]
CLI_OP_TYPE_CREATE 							= ["--create", 	"-c", 	"create"	]
CLI_OP_TYPE_DELETE 							= ["--delete", 	"-d", 	"delete"	]
CLI_OP_TYPE_DESTROY 						= ["--destroy",	"-dy", 	"destroy"	]
CLI_OP_TYPE_INFO 							= ["--info", 	"-i",	"info"		]
CLI_OP_TYPE_STATUS 							= ["--status", 	"-s",	"status"	]
CLI_OP_TYPE_HELP 							= ["--help", 	"-h",	"help","-?"	]
CLI_OP_TYPE_VERSION							= ["--version", "-v",	"version"	]
CLI_OP_TYPE_KERNEL_MODULES					= ["--kernel-modules",	"--km", "km"	]
CLI_OP_TYPE_RMMOD							= ["--rmmod",	"-rm", "rm"			]
CLI_OP_TYPE_SIZE							= ["--size",	"-sz", "sz", "size"	]
CLI_OP_TYPE_STATS							= ["--stats",	"-st", "st", "stats"]
CLI_OP_TYPE_LBA								= ["--lba",		             "lba"  ]
CLI_OP_TYPE_PBA								= ["--pba",		             "pba"  ]
CLI_OP_TYPE_SIZEOF							= ["--sizeof", "-so", "sof", "sizeof"]
CLI_OP_TYPE_IO								= ["--io", "-io", "ior", "ioread"]
CLI_OP_TYPE_IO_READ							= ["--ior", "-ior", "ior", "ioread"]
CLI_OP_TYPE_IO_WRITE						= ["--iow", "-iow", "iow", "iowrite"]
CLI_OP_TYPE_IO_WRITE_READ					= ["--iowr", "-iowr", "iowr", "iowriteread"]
CLI_OP_TYPE_IO_READ_WRITE					= ["--iorw", "-iorw", "iorw", "ioreadwrite"]
CLI_OP_TYPE_IO_READ_WRITE_READ				= ["--iorwr", "-iorwr", "iorwr", "ioreadwriteread"]
CLI_OP_TYPE_IO_ZERO							= ["--ioz", "-ioz", "ioz", "iozero"]
CLI_OP_TYPE_IO_READ_ZERO_READ				= ["--iorzr", "-iorzr", "iorzr", "ioreadzeroread"]
CLI_OP_TYPE_LOG								= ["--log", "log"]
CLI_OP_TYPE_CONFIG							= ["--config", "conf", "config", "--conf"]
CLI_OP_TYPE_LOAD_CONFIG						= ["--loadconfig", "loadconf", "loadconfig", "--loadconf"]
CLI_OP_TYPE_DISKS							= ["--disks", "disk", "disks", "--disk"]
CLI_OP_TYPE_START							= ["--start", "start"]
CLI_OP_TYPE_STOP							= ["--stop", "stop"]
CLI_OP_TYPE_RESTART							= ["--restart", "restart"]
CLI_OP_TYPE_SYSFS							= ["--sysfs", "sysfs", "--sys", "sys"]
CLI_OP_TYPE_IOPATH							= ["--iopath", "iopath" ]


CLI_DEV_TYPE_AISA							= ["AISA",	"-a", "aisa"	]
CLI_DEV_TYPE_VOLUME							= ["--volume",	"-vol"		]
CLI_DEV_TYPE_SNAPSHOT_READ					= ["--sread",	"-srvol"	]
CLI_DEV_TYPE_SNAPSHOT_WRITE					= ["--swrite",	"-swvol"	]
CLI_DEV_TYPE_VM_VOLUME						= ["--vmvolume","-vmvol"	]
CLI_DEV_TYPE_VM_SNAPSHOT_READ				= ["--vmsread",	"-vmsr"		]
CLI_DEV_TYPE_VM_SNAPSHOT_WRITE				= ["--vmswrite","-vmsw"		]		

CLI_DEV_TYPE_ALL							= 	[""]
CLI_DEV_TYPE_ALL							+= 	CLI_DEV_TYPE_VOLUME
CLI_DEV_TYPE_ALL							+= 	CLI_DEV_TYPE_SNAPSHOT_READ
CLI_DEV_TYPE_ALL							+= 	CLI_DEV_TYPE_SNAPSHOT_WRITE
CLI_DEV_TYPE_ALL							+= 	CLI_DEV_TYPE_VM_VOLUME
CLI_DEV_TYPE_ALL							+= 	CLI_DEV_TYPE_VM_SNAPSHOT_READ
CLI_DEV_TYPE_ALL							+= 	CLI_DEV_TYPE_VM_SNAPSHOT_WRITE


AISA_METADATA_DISK_MAGIC_NO		 			= "AISAMTDT"

##################################
# DATA DISKS CONFIGURATION
##################################

#All these data disks are actually pendrives (SanDisk Cruzer Blade)
#Each pendrive of 64GB
AISA_DATA_DISKS_CNT   = 6 

AISA_DATA_DISKS       = ''; 
AISA_DATA_DISKS       += '/dev/sdd ';
AISA_DATA_DISKS       += '/dev/sde ';
AISA_DATA_DISKS       += '/dev/sdf ';
AISA_DATA_DISKS       += '/dev/sdg ';
AISA_DATA_DISKS       += '/dev/sdh ';
AISA_DATA_DISKS       += '/dev/sdi '; 

AISA_DATA_DISKS_ARRAY = '/dev/md1'

##################################
# METADATA DISKS CONFIGURATION
##################################
#All these data disks are actually pendrives (SanDisk Cruzer Blade)
#Each pendrive of 64GB
AISA_METADATA_DISKS       =  ''; 
AISA_METADATA_DISKS       += '/dev/sdc '; # On 250GB Harddisk
AISA_METADATA_DISKS_CNT   = 2 
AISA_METADATA_DISKS_ARRAY = '/dev/md0'



##################################
# TARGET TYPE VARIABLES
##################################

DEVICE_TYPE_AISA                            = "DEVICE_TYPE_AISA"
DEVICE_TYPE_DATA_VOLUME                     = "DEVICE_TYPE_DATA_VOLUME"
DEVICE_TYPE_DATA_VOLUME_SNAPSHOT_READ       = "DEVICE_TYPE_DATA_VOLUME_SNAPSHOT_READ"
DEVICE_TYPE_DATA_VOLUME_SNAPSHOT_WRITE      = "DEVICE_TYPE_DATA_VOLUME_SNAPSHOT_WRITE"
DEVICE_TYPE_VM_VOLUME                       = "DEVICE_TYPE_VM_VOLUME"
DEVICE_TYPE_VM_VOLUME_SNAPSHOT_READ         = "DEVICE_TYPE_VM_VOLUME_SNAPSHOT_READ"
DEVICE_TYPE_VM_VOLUME_SNAPSHOT_WRITE        = "DEVICE_TYPE_VM_VOLUME_SNAPSHOT_WRITE"


# Target Type Table Name inside Kernel
TABLE_NAME_AISA                             = "AVM_TARGET"
TABLE_NAME_AISA_DATA_VOLUME                 = "AVM_VOLUME"
TABLE_NAME_AISA_DATA_VOLUME_SNAPSHOT_READ   = "AVM_VOLUME_SR"
TABLE_NAME_AISA_DATA_VOLUME_SNAPSHOT_WRITE  = "AVM_VOLUME_SW"

TABLE_NAME_AISA_VM_VOLUME                   = "AVM_VOLUME_VM"
TABLE_NAME_AISA_VM_VOLUME_SNAPSHOT_READ     = "AVM_VOLUME_VMSR"
TABLE_NAME_AISA_VM_VOLUME_SNAPSHOT_WRITE    = "AVM_VOLUME_VMSW"

AISA_DEVICE_HOME                            = "/dev/mapper"
AISA_VOLUME_PREFIX                          = "/dev/mapper"
AISA_DEVICE_NAME                            = "AISA"
AISA_DEVICE_PATH                            = AISA_DEVICE_HOME + "/" + AISA_DEVICE_NAME


##################################
# CONFIG VARIABLES
##################################

global CONFIG_AVM_DATA_DEVICE_NAME
global CONFIG_AVM_METADATA_DEVICE_NAME
global CONFIG_AVM_PASSTHROUGH_MODE
global CONFIG_AVM_PASSTHROUGH_MODE_READ
global CONFIG_AVM_PASSTHROUGH_MODE_WRITE

global CONFIG_SWITCH_FEC
global CONFIG_SWITCH_BEC
global CONFIG_SWITCH_DDE
global CONFIG_SWITCH_DAE
global CONFIG_SWITCH_DAE_MBR
global CONFIG_SWITCH_DAE_MBW
global CONFIG_SWITCH_DAE_PBR
global CONFIG_SWITCH_DAE_PBW


CONFIG_AVM_DATA_DEVICE_NAME 			= ""
CONFIG_AVM_METADATA_DEVICE_NAME			= ""
CONFIG_AVM_PASSTHROUGH_MODE				= ""
CONFIG_AVM_PASSTHROUGH_MODE_READ		= ""
CONFIG_AVM_PASSTHROUGH_MODE_WRITE		= ""

CONFIG_SWITCH_FEC						= ""
CONFIG_SWITCH_BEC						= ""
CONFIG_SWITCH_DDE						= ""
CONFIG_SWITCH_DAE						= ""
CONFIG_SWITCH_DAE_MBR					= ""
CONFIG_SWITCH_DAE_MBW					= ""
CONFIG_SWITCH_DAE_PBR					= ""
CONFIG_SWITCH_DAE_PBW					= ""
