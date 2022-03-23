#! /usr/bin/python
from AISA_AVMGlobals 	import *
from AISA_AVMCore 		import *
from AISA_AVMHelp 		import *
from AISA_MDRaid 		import *


def dz_load_target_engine_config_param(arg_avm_ob, arg_avm_dict):
	avm 		= arg_avm_ob
	config_dict = arg_avm_dict

	LOG(INFO, "")
	LOG(INFO, "================================================")
	LOG(INFO, "Loading Target Engine Config Parameters")
	LOG(INFO, "================================================")
	# Create a MAP for keeping the corresponding sysfs entries
	tgt_dict = { };
	tgt_dict['TARGET_SWITCH_PASSTHROUGH_MODE'] 	= "target_engine/get_set_target_passthrough_mode"
	tgt_dict['TARGET_SWITCH_PASSTHROUGH_READ'] 	= "target_engine/get_set_target_passthrough_read"
	tgt_dict['TARGET_SWITCH_PASSTHROUGH_WRITE']	= "target_engine/get_set_target_passthrough_write"
	tgt_dict['TARGET_LBA_BLOCK_SIZE'] 			= "target_engine/get_set_target_lba_block_size"
	tgt_dict['TARGET_FAKE_READ'] 				= "target_engine/get_set_target_fake_read"
	tgt_dict['TARGET_FAKE_WRITE'] 				= "target_engine/get_set_target_fake_write"

	for key,value in sorted(config_dict.items()):
		if tgt_dict.has_key(key):
			avm.set_kernel_switch_tunable_value(tgt_dict[key], value)

def dz_load_alignment_engine_config_param(arg_avm_ob, arg_avm_dict):
	avm 		= arg_avm_ob
	config_dict = arg_avm_dict

	LOG(INFO, "")
	LOG(INFO, "================================================")
	LOG(INFO, "Loading Alignment Engine Config Parameters")
	LOG(INFO, "================================================")

	value = config_dict["DAE_SWITCH"]
	if (str(Trim(value)) == "0"):
		LOG(INFO, "Alignment Engine is DISABLED")
		LOG(INFO, "")
		return None

	dae_dict = { };
	dae_dict['DAE_SWITCH'] 			= "alignment_engine/get_set_switch_dae"
	dae_dict['DAE_SWITCH_MBR']		= "alignment_engine/get_set_switch_dae_mbr"
	dae_dict['DAE_SWITCH_MBW']		= "alignment_engine/get_set_switch_dae_mbw"
	dae_dict['DAE_SWITCH_PBR']		= "alignment_engine/get_set_switch_dae_pbr"
	dae_dict['DAE_SWITCH_PBW']		= "alignment_engine/get_set_switch_dae_pbw"
	dae_dict['DAE_READ_THREADS']	= "alignment_engine/get_set_tunable_dae_rthreads"
	dae_dict['DAE_WRITE_THREADS']	= "alignment_engine/get_set_tunable_dae_wthreads"

	for key,value in sorted(config_dict.items()):
		if dae_dict.has_key(key):
			avm.set_kernel_switch_tunable_value(dae_dict[key], value)

def dz_load_fec_engine_config_param(arg_avm_ob, arg_avm_dict):
	avm 		= arg_avm_ob
	config_dict = arg_avm_dict

	LOG(INFO, "")
	LOG(INFO, "================================================")
	LOG(INFO, "Loading Front End Cache Engine Config Parameters")
	LOG(INFO, "================================================")

	value = config_dict["FEC_SWITCH"]
	if (str(Trim(value)) == "0"):
		LOG(INFO, "Front End Cache Engine is DISABLED")
		LOG(INFO, "")
		return None

	fec_dict = { };
	fec_dict['FEC_SWITCH'] 			= "front_end_cache_engine/get_set_switch_fec"
	fec_dict['FEC_SIZE'] 			= "front_end_cache_engine/get_set_tunable_fec_size"
	fec_dict['FEC_READ_THREADS'] 	= "front_end_cache_engine/get_set_tunable_fec_rthreads"
	fec_dict['FEC_WRITE_THREADS'] 	= "front_end_cache_engine/get_set_tunable_fec_wthreads"
	fec_dict['FEC_FLUSH_THREADS'] 	= "front_end_cache_engine/get_set_tunable_fec_fthreads"
	fec_dict['FEC_FLUSH_TIMER'] 	= "front_end_cache_engine/get_set_tunable_fec_ftimer"

	for key,value in sorted(config_dict.items()):
		if fec_dict.has_key(key):
			avm.set_kernel_switch_tunable_value(fec_dict[key], value)

def dz_load_bec_engine_config_param(arg_avm_ob, arg_avm_dict):
	avm 		= arg_avm_ob
	config_dict = arg_avm_dict


	LOG(INFO, "")
	LOG(INFO, "================================================")
	LOG(INFO, "Loading Back End  Cache Engine Config Parameters")
	LOG(INFO, "================================================")

	value = config_dict["BEC_SWITCH"]
	if (str(Trim(value)) == "0"):
		LOG(INFO, "Back End Cache Engine is DISABLED")
		LOG(INFO, "")
		return None

	bec_dict = { };
	bec_dict['BEC_SWITCH'] 			= "back_end_cache_engine/get_set_switch_bec"
	bec_dict['BEC_SIZE'] 			= "back_end_cache_engine/get_set_tunable_bec_size"
	bec_dict['BEC_THREADS'] 		= "back_end_cache_engine/get_set_tunable_bec_threads"
	bec_dict['BEC_EVICT_PERCENTAGE']= "back_end_cache_engine/get_set_tunable_bec_ep"

	for key,value in sorted(config_dict.items()):
		if bec_dict.has_key(key):
			avm.set_kernel_switch_tunable_value(bec_dict[key], value)

def dz_load_dedupe_engine_config_param(arg_avm_ob, arg_avm_dict):
	avm 		= arg_avm_ob
	config_dict = arg_avm_dict

	LOG(INFO, "")
	LOG(INFO, "================================================")
	LOG(INFO, "Loading Dedupe Engine Config Parameters")
	LOG(INFO, "================================================")

	value = config_dict["DDE_SWITCH"]
	if (str(Trim(value)) == "0"):
		LOG(INFO, "Dedupe Engine is DISABLED")
		LOG(INFO, "")
		return None

	dde_dict = { };
	dde_dict['DDE_SWITCH'] 			= "dedupe_engine/get_set_switch_dde"
	dde_dict['DDE_PBA_TABLE_SIZE']	= "dedupe_engine/get_set_tunable_dde_pba_size"
	dde_dict['DDE_LBA_TABLE_SIZE']	= "dedupe_engine/get_set_tunable_dde_lba_size"
	dde_dict['DDE_HBA_TABLE_SIZE']	= "dedupe_engine/get_set_tunable_dde_hba_size"
	dde_dict['DDE_READ_THREADS']	= "dedupe_engine/get_set_tunable_dde_rthreads"
	dde_dict['DDE_WRITE_THREADS']	= "dedupe_engine/get_set_tunable_dde_wthreads"

	for key,value in sorted(config_dict.items()):
		if dde_dict.has_key(key):
			avm.set_kernel_switch_tunable_value(dde_dict[key], value)

def dz_load_metadata_engine_config_param(arg_avm_ob, arg_avm_dict):
	avm 		= arg_avm_ob
	config_dict = arg_avm_dict

	LOG(INFO, "")
	LOG(INFO, "================================================")
	LOG(INFO, "Loading Metadata Engine Config Parameters")
	LOG(INFO, "================================================")

	value = config_dict["MDE_SWITCH"]
	if (str(Trim(value)) == "0"):
		LOG(INFO, "MetaData Engine is DISABLED")
		LOG(INFO, "")
		return None

	mde_dict = { };
	mde_dict['MDE_SWITCH'] 			= "metadata_engine/get_set_switch_mde"

	for key,value in sorted(config_dict.items()):
		if mde_dict.has_key(key):
			avm.set_kernel_switch_tunable_value(mde_dict[key], value)

def dz_load_volume_engine_config_param(arg_avm_ob, arg_avm_dict):
	avm 		= arg_avm_ob
	config_dict = arg_avm_dict

	LOG(INFO, "")
	LOG(INFO, "================================================")
	LOG(INFO, "Loading  Volume Engine  Config Parameters")
	LOG(INFO, "================================================")

	value = config_dict["DVE_SWITCH"]
	if (str(Trim(value)) == "0"):
		LOG(INFO, "Volume Engine is DISABLED")
		LOG(INFO, "")
		return None

	dve_dict = { };
	dve_dict['DVE_SWITCH'] 			= "volume_engine/get_set_switch_dve"

	for key,value in sorted(config_dict.items()):
		if dve_dict.has_key(key):
			avm.set_kernel_switch_tunable_value(dve_dict[key], value)

def dz_load_kernel_config_param(arg_avm_ob, arg_avm_dict):
	avm 		= arg_avm_ob
	config_dict = arg_avm_dict

	kernel_dict = { };

	for key,value in sorted(config_dict.items()):
		if kernel_dict.has_key(key):
			avm.set_kernel_switch_tunable_value(kernel_dict[key], value)

###########################################################
# dz_load_config_file(arg_show_key_val)
# This function populates the key value pairs present in
# the config file located in path /etc/datumsoft/avm.conf
# avm cli will not be started if there is no config file
# present.
# If arg_show_key_val is set to 1 then it will print
# all key value pairs present in config file
###########################################################

def dz_load_config_file(arg_show_key_val):
	global CONFIG_AVM_DATA_DEVICE_NAME
	global CONFIG_AVM_METADATA_DEVICE_NAME

	if os.path.exists(DZ_CLI_CONFIG_FILE_PATH):
		LOG (DEBUG, "Config File " + DZ_CLI_CONFIG_FILE_PATH + " exist")
		pass		
	else:
		LOG (ERROR, "Config File " + DZ_CLI_CONFIG_FILE_PATH + " does not exist")
		return CLI_FAILURE

	avm_config_dict = {}

	with open(DZ_CLI_CONFIG_FILE_PATH, 'r') as config_file:
		for line in config_file:
			line = line.strip()
			if line:
				if not line.startswith("#"):
					key, value = line.split('=')
					avm_config_dict[key.strip()] = value.strip()

	if avm_config_dict.has_key('TARGET_DATA_DEVICE_NAME'):
		CONFIG_AVM_DATA_DEVICE_NAME 	= avm_config_dict['TARGET_DATA_DEVICE_NAME']
	else:
		LOG(ERROR,"Key TARGET_DATA_DEVICE_NAME does not exist in "+DZ_CLI_CONFIG_FILE_PATH)
		return CLI_FAILURE

	#print(CONFIG_AVM_DATA_DEVICE_NAME);

	if avm_config_dict.has_key('TARGET_METADATA_DEVICE_NAME'):
		CONFIG_AVM_METADATA_DEVICE_NAME 	= avm_config_dict['TARGET_METADATA_DEVICE_NAME']
	else:
		LOG(ERROR,"Key TARGET_METADATA_DEVICE_NAME does not exist in "+DZ_CLI_CONFIG_FILE_PATH)
		return CLI_FAILURE

	#print (CONFIG_AVM_DATA_DEVICE_NAME)
	#print (CONFIG_AVM_METADATA_DEVICE_NAME)

	avm = AVM_CORE(CONFIG_AVM_DATA_DEVICE_NAME, CONFIG_AVM_METADATA_DEVICE_NAME)


	#Start Populating into kernel from avm.conf file
	dz_load_target_engine_config_param(avm, avm_config_dict)
	dz_load_alignment_engine_config_param(avm, avm_config_dict)
	dz_load_bec_engine_config_param(avm, avm_config_dict)
	dz_load_metadata_engine_config_param(avm, avm_config_dict)
	dz_load_dedupe_engine_config_param(avm, avm_config_dict)
	dz_load_volume_engine_config_param(avm, avm_config_dict)
	dz_load_fec_engine_config_param(avm, avm_config_dict)
	dz_load_kernel_config_param(avm, avm_config_dict)

	LOG(INFO,"")
	LOG(INFO,"Config file " + DZ_CLI_CONFIG_FILE_PATH + " populated")

	for key,val in sorted(avm_config_dict.items()):
		keyvalstr = key.ljust(35) + " => " + val
		LOG(INFO, keyvalstr)

	return CLI_SUCCESS

def main(argv):
	argc = len(sys.argv)
	dz_load_config_file(1)

if __name__ == "__main__":
	main(sys.argv[0:])
