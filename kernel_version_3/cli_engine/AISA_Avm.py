#! /usr/bin/python
from AISA_AVMCore 		import *
from AISA_AVMHelp 		import *
from AISA_MDRaid 		import *
from AISA_AVMGlobals 	import *
from AISA_AVMConfig 	import *

###########################################################
# dz_cli_sizeof()
# This function show the sizeof of datastructures present
# in kernel modules of avm.
# It also shows some datastructures of bio framework
# 
###########################################################

def dz_cli_sizeof(argc, argv):
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)

	if( argc == 2) :	#{
		keyword	= argv[1]
		retobj = avm.kernel_sizeof_datastructures(argc, argv)
		if (retobj.retcode != 0):
			LOG(ERROR, "Unable to get kernel datastructures sizes. Looks like kernel module is NOT loaded")
			return CLI_FAILURE

		data = retobj.retstr
		print(data)
	#}
	else: #{
		print("")
		return CLI_FAILURE
	#}
	return CLI_SUCCESS

###########################################################
# dz_cli_io_read()
# This function issues READ IO to AISA devices
# through sysfs interface. It will bypass device mapper
# framework
# 
###########################################################

def dz_cli_io_read(argc, argv):
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	if (argc in( 5,6)):
		pass
	else:
		print(dz_cli_help_io(argc, argv))
		return CLI_FAILURE

	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)

	retobj = avm.kernel_io_read(argc, argv)
	if (retobj.retcode != 0):
		LOG(ERROR, "Unable to issue READ IO. Looks like kernel module is NOT loaded")
		return CLI_FAILURE

	data = retobj.retstr
	print(data)
	return CLI_SUCCESS



###########################################################
# dz_cli_io_write()
# This function issues WRITE IO to AISA devices
# through sysfs interface. It will bypass device mapper
# framework
# 
###########################################################

def dz_cli_io_write(argc, argv):
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	if (argc in( 5,6)):
		pass
	else:
		print(dz_cli_help_io(argc, argv))
		return CLI_FAILURE

	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)

	retobj = avm.kernel_io_write(argc, argv)
	if (retobj.retcode != 0):
		LOG(ERROR, "Unable to issue READ IO. Looks like kernel module is NOT loaded")
		return CLI_FAILURE

	data = retobj.retstr
	print(data)
	return CLI_SUCCESS



def dz_cli_io_read_write(argc, argv):
	dz_cli_io_read(argc, argv)
	dz_cli_io_write(argc, argv)
	

def dz_cli_io_write_read(argc, argv):
	dz_cli_io_write(argc, argv)
	dz_cli_io_read(argc, argv)

def dz_cli_io_read_write_read(argc, argv):
	dz_cli_io_read(argc, argv)
	dz_cli_io_write(argc, argv)
	dz_cli_io_read(argc, argv)

def dz_cli_io_zero(argc, argv):
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	if (argc in( 5,6)):
		pass
	else:
		print(dz_cli_help_io(argc, argv))
		return CLI_FAILURE

	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)

	retobj = avm.kernel_io_zero(argc, argv)
	if (retobj.retcode != 0):
		LOG(ERROR, "Unable to issue READ IO. Looks like kernel module is NOT loaded")
		return CLI_FAILURE

	data = retobj.retstr
	print(data)
	return CLI_SUCCESS


def dz_cli_io_read_zero_read(argc, argv):
	dz_cli_io_read(argc, argv)
	dz_cli_io_zero(argc, argv)
	dz_cli_io_read(argc, argv)


###########################################################
# dz_cli_log()
# This function prints the kernel circular buffer log
# 
###########################################################
def dz_cli_log(argc, argv):
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	DZ_AVM_LOG_FILE = "/dev/dzlog"

	cmdstr  = ''
	cmdstr  += 'dzcat ' + DZ_AVM_LOG_FILE
	#cmdstr  += 'avmlog'
	cmdstr  += ''

	# In order to convert jiffies, we are going to have to start by 
	# finding kernels constant value for HZ. It can be done by using os.sysconf:
	sc_clk_tck = os.sysconf_names['SC_CLK_TCK']
	HZ = os.sysconf(sc_clk_tck)

	#From here we want to find the system boot time
	with open('/proc/stat') as f:
		for ln in f.readlines():
			if ln.startswith('btime'):
				btime = int(ln.split()[1])



	#Fetch the logdata from the kernel using dzcat command
	retobj = ShellCmd.runr(cmdstr)

	if (retobj.retcode != 0):
		retobj.retcode

	logdata = retobj.retstr

	linedata = logdata.split("\n")
	for line in linedata:
		line_jiffy     = line.split(":")[0]
		line_jiffy_int = int(line.split(":")[0])
		line_time      = (line_jiffy_int / HZ) + btime
		print(time.ctime(line_time) + line[len(line_jiffy):])
		#print(line_jiffy)

	return CLI_SUCCESS


###########################################################
# dz_cli_avm_start()
# This function starts AVM
# 
###########################################################
def dz_cli_avm_start(argc, argv):

	DZ_AVM_START_SCRIPT = "/opt/DATUMSOFT/zaidstor_databank/avm/scripts/avm_start.sh"

	cmdstr  = ''
	cmdstr  += DZ_AVM_START_SCRIPT
	cmdstr  += ''

	retobj = ShellCmd.runr(cmdstr)

	if (retobj.retcode == 255):
		LOG(ERROR, "ZAIDSTOR AVM already Started")
		return CLI_FAILURE

	logdata = retobj.retstr
	print(logdata)
	return retobj.retcode

###########################################################
# dz_cli_avm_iopath()
# This function sets the iopath function isndie AVM kernel
# 
###########################################################
def dz_cli_avm_iopath(argc, argv):

	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)

	switch_name = "target_engine/get_set_target_switch_iopath"
	switch_value = "1";
	avm.set_kernel_switch_tunable_value(switch_name, switch_value);

	return CLI_SUCCESS

###########################################################
# dz_cli_avm_stop()
# This function stops AVM
# 
###########################################################
def dz_cli_avm_stop(argc, argv):

	DZ_AVM_STOP_SCRIPT = "/opt/DATUMSOFT/zaidstor_databank/avm/scripts/avm_stop.sh"

	cmdstr  = ''
	cmdstr  += DZ_AVM_STOP_SCRIPT
	cmdstr  += ''

	retobj = ShellCmd.runr(cmdstr)

	if (retobj.retcode == 254):
		LOG(ERROR, "ZAIDSTOR AVM Already Stopped")
		return CLI_FAILURE

	logdata = retobj.retstr
	print(logdata)
	return retobj.retcode

###########################################################
# dz_cli_avm_restart()
# This function restart AVM
# 
###########################################################
def dz_cli_avm_restart(argc, argv):

	dz_cli_avm_stop(argc, argv)
	dz_cli_avm_start(argc, argv)

###########################################################
# dz_cli_lba()
# This function gets the lba table entry for a given index
# 
###########################################################

def dz_cli_lba_pba(argc, argv):
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)

	if( argc == 3) :	#{
		keyword	= argv[1]
		keyval	= argv[2]	
		if (keyval.isdigit()): #{
			if (keyword in (CLI_OP_TYPE_LBA)): #{
				avm.get_kernel_lba_entry(keyval)
			#}
			elif  (keyword in (CLI_OP_TYPE_PBA)): #{
				avm.get_kernel_pba_entry(keyval)
			#}
			else: #{
				print("")
				return CLI_FAILURE
			#}
		#}
		else: #{
			LOG(ERROR, "Index should be a an integer")
			return CLI_FAILURE
		#}
	#}
	else: #{
		print("")
		return CLI_FAILURE
	#}
	return CLI_SUCCESS

		

###########################################################
# dz_cli_stats()
# This function gets the size of a block device by
# sending an ioctl to the kernel from AISA kernel module
# 
###########################################################

def dz_cli_stats(argc, argv):
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)

	if( argc > 2) :	#{
		devtype    = argv[2]
		dev_name = devtype
		if (dev_name in ("-all", "all")): #{
			print("")
			avm.get_kernel_memory()
			print("")
			avm.get_kernel_superblock()
			print("")
			avm.get_kernel_counters_metadata()
			print("")
			avm.get_kernel_counters_fec()
			print("")
			avm.get_kernel_counters_align()
			print("")
			avm.get_kernel_counters_bec()
			print("")
			avm.get_kernel_counters_dedupe()
			print("")
			avm.get_kernel_table_size()
			print("")

		#}
		elif (dev_name in ("-k", "k")): #{
			print("")
			avm.get_kernel_memory()
			print("")
		#}
		elif (dev_name in ("-s", "s")): #{
			print("")
			avm.get_kernel_superblock()
			print("")
		#}
		elif (dev_name in ("-m", "m")): #{
			print("")
			avm.get_kernel_counters_metadata()
			print("")
		#}
		elif (dev_name in ("-b", "b")): #{
			print("")
			avm.get_kernel_counters_bec()
			print("")
		#}
		elif (dev_name in ("-d", "d")): #{
			print("")
			avm.get_kernel_counters_dedupe()
			print("")
		#}
		elif (dev_name in ("-f", "f")): #{
			print("")
			avm.get_kernel_counters_fec()
			print("")
			avm.get_kernel_counters_align()
			print("")
		#}
		elif (dev_name in ("-t", "t")): #{
			print("")
			avm.get_kernel_table_size()
			print("")
		#}
		elif (dev_name in ("--help", "help", "-h")): #{
			print(dz_cli_help_stats(argc, argv))
			return CLI_SUCCESS
		#}
		else: #{
			print("")
			avm.get_kernel_memory()
			print("")
			avm.get_kernel_table_size()
			print("")
			avm.get_kernel_superblock()
			print("")
			avm.get_kernel_counters_metadata()
			print("")
			avm.get_kernel_counters_fec()
			print("")
			avm.get_kernel_counters_align()
			print("")
			avm.get_kernel_counters_bec()
			print("")
			avm.get_kernel_counters_dedupe()
			print("")
			avm.get_kernel_table_size()
			print("")

		#}
	#}
	else: #{
		print("")
		avm.get_kernel_memory()
		print("")
		avm.get_kernel_table_size()
		print("")
	#}
	return CLI_SUCCESS


###########################################################
# dz_cli_size()
# This function gets the size of a block device by
# sending an ioctl to the kernel from AISA kernel module
# 
###########################################################

def dz_cli_size(argc, argv):
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	#avm = AVM_CORE()
	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)
	if( argc > 2) :	#{
		devtype    = argv[2]
		dev_name = devtype
		if (dev_name == "all"): #{
			#Print the size of all the volumes present
			avm.print_volume_size_all()

			#Finally print the size of AISA Target
			avm.print_aisa_size()
		#}
		elif (dev_name == "default"): #{
			dev_name = AISA_DEVICE_PATH
			dev_size_blocks = avm.get_aisa_default_size_blocks()	
			dev_size_bytes  = avm.get_aisa_default_size_bytes()	
			LOGRAW("[ " + dev_name + " ]")
			LOGRAW("  " + dev_name +" size in Bytes : "+dev_size_bytes)
			LOGRAW("  " + dev_name +" size in Blocks: "+dev_size_blocks)
			LOGRAW("  " + dev_name +" size in KB    : " + convert_bytes_to_kb(dev_size_bytes))
			LOGRAW("  " + dev_name +" size in MB    : " + convert_bytes_to_mb(dev_size_bytes))
			LOGRAW("  " + dev_name +" size in GB    : " + convert_bytes_to_gb(dev_size_bytes))

		#}
		elif (dev_name in ("--help", "help", "-h")): #{
			print(dz_cli_help_size(argc, argv))
			return CLI_SUCCESS
		#}
		else: #{
			dev_size_blocks = avm.get_device_size_blocks(dev_name)	

			if (int(dev_size_blocks) <= 0 ):
				LOG(ERROR,"Device "+dev_name+ " does not exist. Try with full path (if not tried)")
				return CLI_FAILURE

			dev_size_bytes  = avm.get_device_size_bytes(dev_name)	
			LOGRAW("[ " + dev_name + " ]")
			LOGRAW("  " + dev_name +" size in Bytes : "+dev_size_bytes)
			LOGRAW("  " + dev_name +" size in Blocks: "+dev_size_blocks)
			LOGRAW("  " + dev_name +" size in KB    : " + convert_bytes_to_kb(dev_size_bytes))
			LOGRAW("  " + dev_name +" size in MB    : " + convert_bytes_to_mb(dev_size_bytes))
			LOGRAW("  " + dev_name +" size in GB    : " + convert_bytes_to_gb(dev_size_bytes))
		#}
	#}
	else: #{
		#Print the size of all the volumes present
		avm.print_volume_size_all()
	#}

###########################################################
# dz_cli_rmmod()
# 
###########################################################

def dz_cli_rmmod(argc, argv):
	for km in reversed(KERNEL_MODULE_LIST):
		LOG(INFO, "Unloading Kernel Module: "+km)
		retobj = ShellCmd.remove_module(km)
		retobj.LOG()

###########################################################
# dz_list_kernel_modules()
# This function lists whether all the required DatumSoft 
# kernel modules are loaded.
# It does this by checking the kernel module in the path
# /sys/module
# e.g.
# /sys/module/datumsoft_avm_aisa_target_module
###########################################################

def dz_list_kernel_modules(argc, argv):
	prefix = "/sys/module/"
	for km in KERNEL_MODULE_LIST:
		if os.path.exists(prefix+km):
			LOG (INFO, "Kernel Module " + km + " Loaded")
			LOG (INFO, "Path " + prefix + km + " exist")
		else:
			LOG (INFO, "Kernel Module " + km + " Not Loaded")
			LOG (INFO, "Path " + prefix + km + " does not exist")

	return CLI_SUCCESS

###########################################################
# dz_status()
# This function checks the status of: 
#	1. All kernel modules
#	2. Whether AISA target is created
###########################################################

def dz_status(argc, argv):
	show = 1
	print("\n")
	print("AVM Kernel Module Information")
	print("-----------------------------")
	dz_validate_kernel_modules(show)
	print("\n")
	print("AVM AISA Target Information")
	print("---------------------------")
	dz_validate_aisa_target(show)
	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)
	print("\n")
	print("AVM sysfs entries in kernel")
	print("---------------------------")
	avm.get_sysfs_entries()
	return CLI_SUCCESS

###########################################################
# dz_populate_config_file(arg_show_key_val)
# This function populates the key value pairs present in
# the config file located in path /etc/datumsoft/avm.conf
# avm cli will not be started if there is no config file
# present.
# If arg_show_key_val is set to 1 then it will print
# all key value pairs present in config file
###########################################################

def dz_populate_config_file(arg_show_key_val):
	global CONFIG_AVM_DATA_DEVICE_NAME
	global CONFIG_AVM_METADATA_DEVICE_NAME

	if os.path.exists(DZ_CLI_CONFIG_FILE_PATH):
		LOG (DEBUG, "Config File " + DZ_CLI_CONFIG_FILE_PATH + " exist")
		pass		
	else:
		LOG (ERROR, "Config File " + DZ_CLI_CONFIG_FILE_PATH + " does not exist")
		return CLI_FAILURE

	dz_config_dict = {}

	with open(DZ_CLI_CONFIG_FILE_PATH, 'r') as config_file:
		for line in config_file:
			line = line.strip()
			if line:
				if not line.startswith("#"):
					key, value = line.split('=')
					dz_config_dict[key.strip()] = str(value.strip())


	if dz_config_dict.has_key('TARGET_DATA_DEVICE_NAME'):
		CONFIG_AVM_DATA_DEVICE_NAME 	= dz_config_dict['TARGET_DATA_DEVICE_NAME']
	else:
		LOG(ERROR,"Key TARGET_DATA_DEVICE_NAME does not exist in "+DZ_CLI_CONFIG_FILE_PATH)
		return CLI_FAILURE

	#print(CONFIG_AVM_DATA_DEVICE_NAME);

	if dz_config_dict.has_key('TARGET_METADATA_DEVICE_NAME'):
		CONFIG_AVM_METADATA_DEVICE_NAME 	= dz_config_dict['TARGET_METADATA_DEVICE_NAME']
	else:
		LOG(ERROR,"Key TARGET_METADATA_DEVICE_NAME does not exist in "+DZ_CLI_CONFIG_FILE_PATH)
		return CLI_FAILURE

	#print(CONFIG_AVM_METADATA_DEVICE_NAME);

	#print (CONFIG_AVM_DATA_DEVICE_NAME)
	#print (CONFIG_AVM_METADATA_DEVICE_NAME)
	LOG(INFO,"Config file " + DZ_CLI_CONFIG_FILE_PATH + " populated")

	for key,val in sorted(dz_config_dict.items()):
		keyvalstr = key.ljust(32) + " => " + val
		#if (val == "0"):
			#keyvalstr = key.ljust(25) + " => DISABLED"
		#else:
			#keyvalstr = key.ljust(25) + " => ENABLED"

		if (arg_show_key_val == 1):
			LOG(INFO, keyvalstr)

	return CLI_SUCCESS


###########################################################
# dz_validate_aisa_target()
# This function checks whether AISA target is created
###########################################################

def dz_validate_aisa_target(arg_show):

	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)
	return avm.validate_aisa_target(arg_show)
'''
	if os.path.exists(AISA_DEVICE_PATH):
		if (arg_show == 1):
			LOG (INFO, "AISA Target created. " + AISA_DEVICE_PATH + " exist")
			return CLI_SUCCESS
		else:
			return CLI_SUCCESS
	else:
		LOG (ERROR, "AISA Target Not created. " + AISA_DEVICE_PATH + " does not exist")
		return CLI_FAILURE
'''


###########################################################
# dz_validate_kernel_modules()
# This function checks whether all the required kernel
# modules are loaded. It returns an error if any of the 
# kernel module is not loaded
###########################################################

def dz_validate_kernel_modules(arg_show):
	prefix = "/sys/module/"
	loaded = 1
	for km in KERNEL_MODULE_LIST:
		if os.path.exists(prefix+km):
			if (arg_show == 1):
				LOG (INFO, "Kernel Module " + km + " Loaded")
				LOG (INFO, "Path " + prefix + km + " exist")
			else:
				pass
		else:
			LOG (ERROR, "Kernel Module " + km + " Not Loaded")
			LOG (ERROR, "Path " + prefix + km + " does not exist")
			loaded = 0

	if (loaded == 0):
		return CLI_FAILURE
	else:
		return CLI_SUCCESS

###########################################################
# dz_cli_validate()
# This function checks whether all the prerequistes are
# set before starting the avm cli.
###########################################################

def dz_cli_validate(argc, argv):
	#print(len(argv))
 
	#print (CONFIG_AVM_DATA_DEVICE_NAME)
	#print (CONFIG_AVM_METADATA_DEVICE_NAME)

	if (argc == 1):
		dz_cli_help(argc, argv)
		return CLI_FAILURE

	operationtype	 = argv[1]
	if 	operationtype in (CLI_OP_TYPE_CONFIG):
		err = dz_populate_config_file(1)
		#err = dz_load_config_file(1)
	else:
		err = dz_populate_config_file(0)
		#err = dz_load_config_file(0)

	if (err != CLI_SUCCESS):
		return err

	return CLI_SUCCESS

def dz_cli_version(argc, argv):
	LOGR(DZ_CLI_PRODUCT)
	LOGR("CLI Version "  + DZ_CLI_VERSION)
	#avm = AVM_CORE()
	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)
	avm.get_kernel_version()
	return CLI_SUCCESS

###########################################################
# dz_cli_info()
# This function prints detailed information about each
# volume
###########################################################

def dz_cli_info(argc, argv):
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	#avm = AVM_CORE()
	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)
	if( argc > 2) :	
		devtype    = argv[2]
		dev_name = devtype
		if (dev_name is not None):
			if (dev_name in (CLI_DEV_TYPE_AISA)):
				retobj = avm.info_aisa()
				retobj.LOG()
			elif (dev_name == "all"):
				retobj = avm.list_volume_short()
				data = retobj.retstr
				devices = data.split("\n")
				for vol in devices:
					if (vol.startswith( '#' )):
						continue
					else:
						retobj = avm.info_volume(vol)
						retobj.LOGRAW()

				retobj = avm.info_aisa()
				retobj.LOGRAW()
			elif (dev_name in ("--help", "help", "-h")):
				print(dz_cli_help_info(argc, argv))
				return CLI_SUCCESS
				
			else:
				retobj = avm.info_volume(dev_name)
				retobj.LOGRAW()
		else:
			retobj = avm.list_volume_short()
			data = retobj.retstr
			devices = data.split("\n")
			for vol in devices:
				if (vol.startswith( '#' )):
					continue
				else:
					retobj = avm.info_volume(vol)
					retobj.LOG() 
	else:
		retobj = avm.list_volume_short()
		data = retobj.retstr
		devices = data.split("\n")
		for vol in devices:
			if (vol.startswith( '#' )):
				continue
			else :
				retobj = avm.info_volume(vol)
				retobj.LOGRAW()

###########################################################
# dz_cli_list_volumes()
# This function lists down the volumes available in the 
# system. It however, does not show anything about AISA Target
###########################################################

def dz_cli_list_volumes(argc, argv):
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	#avm = AVM_CORE()
	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)
	retobj = avm.list_volume(argc, argv)
	retobj.LOGRAW()
	return CLI_SUCCESS

###########################################################
# dz_cli_delete()
# This function is capable of deleting AISA Target,
# volumes, vm volumes, snapshot volumes
# This function does not check the dependency.
# e.g. it may allow deletion of parent volume without even
# deleting snapshot volumes
###########################################################

def dz_cli_delete(argc, argv):
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	if (argc == 2):
		print(dz_cli_help_delete(argc, argv))
		return CLI_FAILURE

	dev_name	= argv[2]

	if (dev_name == "all"):
		#avm 		= AVM_CORE()
		avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)
		avm.delete_all_volumes()
		return CLI_SUCCESS

	elif (dev_name in (CLI_DEV_TYPE_AISA)):
		#avm 	= AVM_CORE()
		avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)
		retobj 	= avm.delete_aisa()
		retobj.LOG()
		return CLI_SUCCESS

	elif (dev_name in ("help", "--help", "-h")):
		print(dz_cli_help_delete(argc, argv))
		return CLI_SUCCESS

	elif (dev_name is not None):
		#avm 		= AVM_CORE()
		avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)
		retobj = avm.delete_volume(dev_name)
		retobj.LOG()
		return CLI_SUCCESS

	else:
		print(dz_cli_help_delete(argc, argv))
		return CLI_FAILURE


###########################################################
# dz_cli_destroy()
# This function is capable of deleting AISA Target,
# volumes, vm volumes, snapshot volumes
# This function does not check the dependency.
# e.g. it may allow deletion of parent volume without even
# deleting snapshot volumes
###########################################################

def dz_cli_destroy(argc, argv):
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	if (argc != 2):
		print(dz_cli_help_destroy(argc, argv))
		return CLI_FAILURE

	dev_name	= argv[1]

	if (dev_name == "destroy"):
		avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)
		avm.delete_all_volumes()
		retobj 	= avm.delete_aisa()
		retobj.LOG()
		return CLI_SUCCESS

	elif (dev_name in ("help", "--help", "-h")):
		print(dz_cli_help_destroy(argc, argv))
		return CLI_SUCCESS

	elif (dev_name is not None):
		avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)
		avm.delete_all_volumes()
		retobj 	= avm.delete_aisa()
		retobj.LOG()
		return CLI_SUCCESS

	else:
		print(dz_cli_help_destroy(argc, argv))
		return CLI_FAILURE

###########################################################
# dz_cli_config()
# This function checks whether all the prerequistes are
# set including the accessiblity of data and metadata disks
###########################################################
def dz_cli_config(argc, argv):

	err = CLI_SUCCESS
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	# Check for existence of data disk
	if os.path.exists(CONFIG_AVM_DATA_DEVICE_NAME):
		LOG (DEBUG, "Data Device " + CONFIG_AVM_DATA_DEVICE_NAME + " exist")
		pass		
	else:
		LOG (ERROR, "Data Device " + CONFIG_AVM_DATA_DEVICE_NAME + " does not exist")
		err = CLI_DATA_DEVICE_DOES_NOT_EXIST

	# Check for existence of metadata disk
	if os.path.exists(CONFIG_AVM_METADATA_DEVICE_NAME):
		LOG (DEBUG, "MetaData Device " + CONFIG_AVM_METADATA_DEVICE_NAME + " exist")
		pass		
	else:
		LOG (ERROR, "MetaData Device " + CONFIG_AVM_METADATA_DEVICE_NAME + " does not exist")
		if (err == CLI_DATA_DEVICE_DOES_NOT_EXIST):
			err = CLI_DEVICES_DOES_NOT_EXIST
		else:
			err = CLI_METADATA_DEVICE_DOES_NOT_EXIST

	return err

###########################################################
# dz_cli_disks()
# This function returns data and metadata disks
###########################################################
def dz_cli_disks(argc, argv):

	#err = CLI_SUCCESS
	#if (dz_validate_kernel_modules(0) < 0):
	#	LOG(ERROR,"Kernel Modules are NOT loaded")
	#	return CLI_FAILURE

	disks = CONFIG_AVM_DATA_DEVICE_NAME
	disks += "\n"
	disks += CONFIG_AVM_METADATA_DEVICE_NAME
	#LOGR(disks)
	return disks

###########################################################
# dz_cli_sysfs()
# This function prints all sysfs entries created by AVM
###########################################################
def dz_cli_sysfs(argc, argv):

	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)
	print("\n")
	print("AVM sysfs entries in kernel")
	print("---------------------------")
	avm.get_sysfs_entries()
	return CLI_SUCCESS

###########################################################
# dz_cli_create()
# This function checks whether all the prerequistes are
# set before actually creating AISA target and volumes.
# Here it makes the instance of class AVM_CORE()
# and invokes actual target creation and volume creation
# functions
###########################################################

def dz_cli_create(argc, argv):
	if (dz_validate_kernel_modules(0) < 0):
		LOG(ERROR,"Kernel Modules are NOT loaded")
		return CLI_FAILURE

	operationtype	 = ''  # argv[1]
	devtype 	 = ''  # argv[2]
	name 		 = ''  # argv[3]
	vname		 = ''  # argv[4]
	size 		 = ''  # argv[5]
	vsize		 = ''  # argv[6]
	parent	 	 = ''  # argv[7]
	parent_vol          = ''  # argv[8]
	operationtype	 = argv[1]

	#avm = AVM_CORE()
	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)
	dev_name = AISA_DEVICE_NAME
	devtype 	 = argv[2]

	if devtype in (CLI_DEV_TYPE_ALL):
		#if (dz_validate_aisa_target(0) < 0):
		#	return CLI_FAILURE
		if devtype in (CLI_DEV_TYPE_VOLUME):

			dev_type 	= DEVICE_TYPE_DATA_VOLUME
			name		= argv[3]
			if name in ("--name"):
				dev_name         	= argv[4]

			if (argc > 4):	
				if(argv[5] is not None):
  					size			= argv[5]
					if size in ("--size"):
						vsize		= argv[6]
						dev_size	= vsize	
					else:
						LOG (INFO,"Missing volume size, default size of 1GB is set")
						dev_size 	= "2097152"  # 512b sectors i.e. 1GB
				else:
					pass
			else:
				print ("You have not entered the volume size")
				print ("Volume create command usage: ./avm --creat vol --name [VOL-NAME] --size [SIZE]")
				return -1

			dev_size = convert_gb_to_blocks(dev_size)
			LOG(INFO, "Creating Volume " + dev_name + " of size " + dev_size + " blocks")
			retobj = avm.create_volume(dev_type, dev_name, dev_size, parent_vol)
			retobj.LOG()

		if devtype in (CLI_DEV_TYPE_SNAPSHOT_READ):
			dev_type	 = DEVICE_TYPE_DATA_VOLUME_SNAPSHOT_READ
			name         = argv[3]
			if name in ("--name"):
				dev_name   = argv[4]
				parent_vol = argv[6]

			pvolpath = AISA_VOLUME_PREFIX + "/" + parent_vol
			dev_size_blocks = avm.get_device_size_blocks(pvolpath)

			if (int(dev_size_blocks) == -1):
				LOG(ERROR, "Parent Volume " + parent_vol + " size is -1. Check if it exists")
				return CLI_FAILURE

			LOG(INFO, "Creating ReadOnly Snapshot Volume " + dev_name)
			retobj = avm.create_volume(dev_type, dev_name, dev_size_blocks, parent_vol)
			retobj.LOG()

		if devtype in (CLI_DEV_TYPE_SNAPSHOT_WRITE):
			dev_type	 = DEVICE_TYPE_DATA_VOLUME_SNAPSHOT_WRITE
			name         = argv[3]
			if name in ("--name"):
				dev_name   = argv[4]
				parent_vol = argv[6]

			pvolpath = AISA_VOLUME_PREFIX + "/" + parent_vol
			dev_size_blocks = avm.get_device_size_blocks(pvolpath)
			if (int(dev_size_blocks) == -1):
				LOG(ERROR, "Parent Volume " + parent_vol + " size is -1. Check if it exists")
				return CLI_FAILURE

			LOG(INFO, "Creating Writeable Snapshot Volume " + dev_name)
			retobj = avm.create_volume(dev_type, dev_name, dev_size_blocks, parent_vol)
			retobj.LOG()

		if devtype in (CLI_DEV_TYPE_VM_VOLUME):
			dev_type	 = DEVICE_TYPE_VM_VOLUME
			#TODO

		if devtype in (CLI_DEV_TYPE_VM_SNAPSHOT_READ):
			dev_type	 = DEVICE_TYPE_VM_VOLUME_SNAPSHOT_READ
			name         = argv[3]
			if name in ("--name"):
				dev_name   = argv[4]
				parent_vol = argv[6]

			pvolpath = AISA_VOLUME_PREFIX + "/" + parent_vol
			dev_size_blocks = avm.get_device_size_blocks(pvolpath)
			if (int(dev_size_blocks) == -1):
				LOG(ERROR, "Parent Volume " + parent_vol + " size is -1. Check if it exists")
				return CLI_FAILURE

			LOG(INFO, "Creating ReadOnly Snapshot VM Volume " + dev_name)
			retobj = avm.create_volume(dev_type, dev_name, dev_size_blocks, parent_vol)
			retobj.LOG()

		if devtype in (CLI_DEV_TYPE_VM_SNAPSHOT_WRITE):
			dev_type	 = DEVICE_TYPE_VM_VOLUME_SNAPSHOT_WRITE
			name         = argv[3]
			if name in ("--name"):
				dev_name   = argv[4]
				parent_vol = argv[6]

			pvolpath = AISA_VOLUME_PREFIX + "/" + parent_vol
			dev_size_blocks = avm.get_device_size_blocks(pvolpath)
			if (int(dev_size_blocks) == -1):
				LOG(ERROR, "Parent Volume " + parent_vol + " size is -1. Check if it exists")
				return CLI_FAILURE

			LOG(INFO, "Creating Writeable Snapshot VM Volume " + dev_name)
			retobj = avm.create_volume(dev_type, dev_name, dev_size_blocks, parent_vol)
			retobj.LOG()

	elif devtype in (CLI_DEV_TYPE_AISA):
		dev_type 	= DEVICE_TYPE_AISA
		dev_name	= argv[2]
			
		if(argc == 5):
			size = argv[3]
			if size in ("--size"):
				vsize = argv[4]
				aisa_size_blocks = convert_gb_to_blocks(vsize)
				aisa_default_size = avm.get_aisa_default_size_blocks()
				if (int(aisa_size_blocks) > int(aisa_default_size)):
					LOG(WARN,"AISA Target Size is greater than default size ")
					LOG(INFO,"Default Size: " + aisa_default_size + " blocks")
					LOG(INFO,"Target  Size: " + aisa_size_blocks + " blocks")
					LOG(INFO,"Creating AISA Target with default size " + aisa_default_size + " blocks")

					aisa_size_blocks = aisa_default_size
					retobj = avm.create_aisa(dev_type, dev_name, aisa_size_blocks)
					retobj.LOG()
					return retobj.ret
				else:
					LOG(INFO,"Creating AISA Target of size " + aisa_size_blocks + " blocks")
					retobj = avm.create_aisa(dev_type, dev_name, aisa_size_blocks)
					retobj.LOG()
					return retobj.ret
			else:
				pass
		else:
			pass

		retobj = avm.create_aisa_default()
		retobj.LOG()
		return retobj.ret

# def main(self, argc = len(sys.argv), arg = sys.argv):
def main(argv):

	cmdline = ""
	for token in sys.argv:
		cmdline = cmdline + " " + token

	LOG(CMD, Trim(cmdline))

	argc = len(sys.argv)
	err = dz_cli_validate(argc, argv);

	if (err != CLI_SUCCESS):
		exit(err)


	operationtype	= argv[1]
	
	if 		operationtype in (CLI_OP_TYPE_CREATE):
			exit(dz_cli_create(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_DELETE):
			exit(dz_cli_delete(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_DESTROY):
			exit(dz_cli_destroy(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_LIST):
			exit(dz_cli_list_volumes(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_INFO):
			exit(dz_cli_info(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_VERSION):
			exit(dz_cli_version(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_STATUS):
			dz_status(argc, argv)
			exit(CLI_SUCCESS)

	elif 	operationtype in (CLI_OP_TYPE_KERNEL_MODULES):
			exit(dz_list_kernel_modules(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_RMMOD):
			exit(dz_cli_rmmod(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_SIZE):
			exit(dz_cli_size(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_STATS):
			exit(dz_cli_stats(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_LBA):
			exit(dz_cli_lba_pba(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_PBA):
			exit(dz_cli_lba_pba(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_SIZEOF):
			exit(dz_cli_sizeof(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_IO):
			exit(dz_cli_io_read(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_IO_READ):
			exit(dz_cli_io_read(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_IO_READ_WRITE):
			exit(dz_cli_io_read_write(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_IO_WRITE):
			exit(dz_cli_io_write(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_IO_WRITE_READ):
			exit(dz_cli_io_write_read(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_IO_READ_WRITE_READ):
			exit(dz_cli_io_read_write_read(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_IO_ZERO):
			exit(dz_cli_io_zero(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_IO_READ_ZERO_READ):
			exit(dz_cli_io_read_zero_read(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_LOG):
			exit(dz_cli_log(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_CONFIG):
			exit(dz_cli_config(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_LOAD_CONFIG):
			exit(dz_load_config_file(1))

	elif 	operationtype in (CLI_OP_TYPE_DISKS):
			exit(dz_cli_disks(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_SYSFS):
			exit(dz_cli_sysfs(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_START):
			exit(dz_cli_avm_start(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_STOP):
			exit(dz_cli_avm_stop(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_RESTART):
			exit(dz_cli_avm_restart(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_IOPATH):
			exit(dz_cli_avm_iopath(argc, argv))

	elif 	operationtype in (CLI_OP_TYPE_HELP):
			dz_cli_help(argc, argv)
			exit(CLI_SUCCESS)

	else:
			dz_cli_help(argc, argv)
			exit(CLI_FAILURE)

if __name__ == "__main__":
	main(sys.argv[0:])
	#main(sys.argv)
