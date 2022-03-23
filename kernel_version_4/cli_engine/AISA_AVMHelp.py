#! /usr/bin/python
from AISA_AVMCore 		import *
from AISA_AVMGlobals 	import *

def dz_cli_help_create_vmvolsnap(argc, argv):
	argv[0] = "avm"
	help_create = '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_CREATE) + ' > '
	help_create += '< '+' | '.join(CLI_DEV_TYPE_VM_SNAPSHOT_READ) + ' > '
	help_create += '--name < VOL_NAME > --parent < PVOL_NAME >'
	help_create += '\n  OR\n'
	help_create += '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_CREATE) + ' > '
	help_create += '< '+' | '.join(CLI_DEV_TYPE_VM_SNAPSHOT_WRITE) + ' > '
	help_create += '--name < VOL_NAME > --parent < PVOL_NAME >'
	help_create += '\n  Where'
	help_create += '\n  VOL_NAME	: Name of Snapshot Volume'
	help_create += '\n  PVOL_NAME	: Name of Volume whose snapshot is being taken'
	cmdstr	= '\n\n  Examples:'
	cmdstr += '\n\tTo Create a ReadOnly Snapshot Volume testvolsr for VM volume testvmvol\n'
	cmdstr += '\t#> ' + argv[0] + ' --create --vmsread --name testvolsr --parent testvmvol\n'
	cmdstr += '\n\tTo Create a Writable Snapshot Volume testvolsw for VM volume testvmvol\n'
	cmdstr += '\t#> ' + argv[0] + ' --create --vmswrite --name testvolsw --parent testvmvol\n'
	return (help_create + cmdstr)

def dz_cli_help_create_volsnap(argc, argv):
	argv[0] = "avm"
	help_create = '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_CREATE) + ' > '
	help_create += '< '+' | '.join(CLI_DEV_TYPE_SNAPSHOT_READ) + ' > '
	help_create += '--name < VOL_NAME > --parent < PVOL_NAME >'
	help_create += '\n  OR\n'
	help_create += '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_CREATE) + ' > '
	help_create += '< '+' | '.join(CLI_DEV_TYPE_SNAPSHOT_WRITE) + ' > '
	help_create += '--name < VOL_NAME > --parent < PVOL_NAME >'
	help_create += '\n  Where'
	help_create += '\n  VOL_NAME	: Name of Snapshot Volume'
	help_create += '\n  PVOL_NAME	: Name of Volume whose snapshot is being taken'
	cmdstr	= '\n\n  Examples:'
	cmdstr += '\n\tTo Create a ReadOnly Snapshot Volume testvolsr for volume testvol\n'
	cmdstr += '\t#> ' + argv[0] + ' --create --sread --name testvolsr --parent testvol\n'
	cmdstr += '\n\tTo Create a Writable Snapshot Volume testvolsw for volume testvol\n'
	cmdstr += '\t#> ' + argv[0] + ' --create --swrite --name testvolsw --parent testvol\n'
	return (help_create + cmdstr)

def dz_cli_help_create_vmvol(argc, argv):
	argv[0] = "avm"
	help_create = '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_CREATE) + ' > '
	help_create += '< '+' | '.join(CLI_DEV_TYPE_VM_VOLUME) + ' > '
	help_create += '--name < VOL_NAME > --size < SIZE >'
	help_create += '\n  Where'
	help_create += '\n  VOL_NAME 	: Name of Virtual Machine Volume passed'
	help_create += '\n  SIZE 		: It is in GB. Optional argument'
	cmdstr	= '\n\n  Examples:'
	cmdstr += '\n\tTo Create a VM Volume testvmvol with size 1024\n'
	cmdstr += '\t#> ' + argv[0] + ' --create --vmvolume --name testvmvol --size 1024\n'
	cmdstr += '\n\tTo Create a VM Volume testvmvol with default size\n'
	cmdstr += '\t#> ' + argv[0] + ' --create --vmvolume --name testvmvol\n'
	return (help_create + cmdstr)

def dz_cli_help_create_vol(argc, argv):
	argv[0] = "avm"
	help_create = '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_CREATE) + ' > '
	help_create += '< '+' | '.join(CLI_DEV_TYPE_VOLUME) + ' > '
	help_create += '--name < VOL_NAME > --size < SIZE >'
	help_create += '\n  Where'
	help_create += '\n  VOL_NAME 	: Name of volume passed'
	help_create += '\n  SIZE 		: It is in GB. Optional argument'
	cmdstr	= '\n\n  Examples:'
	cmdstr += '\n\tTo Create a Volume testvol with size 1024\n'
	cmdstr += '\t#> ' + argv[0] + ' --create --volume --name testvol --size 1024\n'
	cmdstr += '\n\tTo Create a Volume testvol with default size\n'
	cmdstr += '\t#> ' + argv[0] + ' --create --volume --name testvol\n'
	return (help_create + cmdstr)

def dz_cli_help_delete(argc, argv):
	argv[0] = "avm"
	help_delete = '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_DELETE) + ' > < VOL_NAME | all | AISA > '
	cmdstr	= '\n\n  Examples:'
	cmdstr += '\n\tTo Delete a Volume testvol\n'
	cmdstr += '\t#> ' + argv[0] + ' --delete testvol\n'

	cmdstr += '\n\tTo Delete all volumes\n'
	cmdstr += '\t#> ' + argv[0] + ' --delete all\n'

	cmdstr += '\n\tTo Delete AISA Target \n'
	cmdstr += '\t#> ' + argv[0] + ' --delete AISA\n'

	#print(help_delete)
	#print(cmdstr)
	return (help_delete + cmdstr)

def dz_cli_help_destroy(argc, argv):
	argv[0] = "avm"
	help_delete = '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_DESTROY) + ' > '
	cmdstr	= '\n\n  Examples:'
	cmdstr += '\n\tTo Destroy all volumes and Target\n'
	cmdstr += '\t#> ' + argv[0] + ' --destroy\n'

	#print(help_delete)
	#print(cmdstr)
	return (help_delete + cmdstr)



def dz_cli_help_size(argc, argv):
	argv[0] = "avm"
	help_size = '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_SIZE) + ' > [ NAME | all | default  ] '
	help_size += '\n  Where'
	help_size += '\n  NAME    : Name of volume passed or any block device present in the system'
	help_size += '\n          : It should be always provided with complete path'
	help_size += '\n  all     : Will print the sizes of all volumes created and AISA target'
	help_size += '\n  default : Will print the default size of AISA Target'
	help_size += '\n'
	help_size += '\n  This command will print the size in bytes, blocks, KB, MB and GBs'
	help_size += '\n  If none of the arugments is passed then it will print the sizes of '
	help_size += '\n  all volumes excluding AISA target. Size shown in blocks, KB, MB and GB '
	help_size += '\n  will always have integer value'
	cmdstr  = '\n\n  Examples:'
	cmdstr += '\n\tTo get size about Volume testvol\n'
	cmdstr += '\t#> ' + argv[0] + ' --size /dev/mapper/testvol\n'

	cmdstr += '\n\tTo get size of all volumes and AISA Target\n'
	cmdstr += '\t#> ' + argv[0] + ' --size all\n'

	cmdstr += '\n\tTo get size of all volumes only\n'
	cmdstr += '\t#> ' + argv[0] + ' --size\n'

	cmdstr += '\n\tTo get size for block device /dev/sda\n'
	cmdstr += '\t#> ' + argv[0] + ' --size /dev/sda\n'
                                                           
	#print(help_delete)
	#print(cmdstr)
	return (help_size + cmdstr)

def dz_cli_help_stats(argc, argv):
	argv[0] = "avm"
	help_stats = '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_STATS) + ' > [ -k | k | -m | m | -d | d |-f | -b | b | t | -t | -s | s | -all | all  ] '
	help_stats += '\n  Where'
	help_stats += '\n  -k/k     : Print Kernel Memory Utilization during initialization'
	help_stats += '\n  -m/m     : Print Metadata Engine Counters'
	help_stats += '\n  -d/d     : Print Dedupe Engine Counters'
	help_stats += '\n  -f/f     : Print FrontEnd Cache Engine Counters'
	help_stats += '\n  -b/b     : Print BackEnd Cache Engine Counters'
	help_stats += '\n  -t/t     : Print InMemory LBA, HASH and PBA tables'
	help_stats += '\n  -s/s     : Print SuperBlock Info'
	help_stats += '\n  -all/all : Print all of above'
	help_stats += '\n'
	cmdstr	= '\n\n  Examples:'
	cmdstr += '\n\tTo get kernel memory utilization during initialization\n'
	cmdstr += '\t#> ' + argv[0] + ' --stats -k\n'

	cmdstr += '\n\tTo get Metadata Engine Counters\n'
	cmdstr += '\t#> ' + argv[0] + ' --stats -m\n'

	cmdstr += '\n\tTo get FrontEnd Cache Engine Counters\n'
	cmdstr += '\t#> ' + argv[0] + ' --stats -f\n'

	cmdstr += '\n\tTo get BackEnd Cache Engine Counters\n'
	cmdstr += '\t#> ' + argv[0] + ' --stats -b\n'

	cmdstr += '\n\tTo get Dedupe Engine Engine Counters\n'
	cmdstr += '\t#> ' + argv[0] + ' --stats -d\n'

	cmdstr += '\n\tTo get memory and all Engine Counters\n'
	cmdstr += '\t#> ' + argv[0] + ' --stats -d\n'

	#print(help_delete)
	#print(cmdstr)
	return (help_stats + cmdstr)

def dz_cli_help_info(argc, argv):
	argv[0] = "avm"
	help_info = '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_INFO) + ' > [ VOL_NAME | all  ] '
	cmdstr	= '\n\n  Examples:'
	cmdstr += '\n\tTo get info about Volume testvol\n'
	cmdstr += '\t#> ' + argv[0] + ' --info testvol\n'

	cmdstr += '\n\tTo get info for all volumes and AISA Target\n'
	cmdstr += '\t#> ' + argv[0] + ' --info all\n'

	cmdstr += '\n\tTo get info for all volumes only\n'
	cmdstr += '\t#> ' + argv[0] + ' --info\n'

	#print(help_delete)
	#print(cmdstr)
	return (help_info + cmdstr)

def dz_cli_help_create_aisa(argc, argv):
	argv[0] = "avm"
	help_create = '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_CREATE) + ' > '
	help_create += '< '+' | '.join(CLI_DEV_TYPE_AISA) + ' > '
	help_create += '[ --size < SIZE > ]'
	help_create += '\n  Where'
	help_create += '\n  SIZE: It is in GB. Optional argument'
	help_create += '\n  NOTE: The default size is the size of the Data Device'
	help_create += '\n      : provided in avm.conf'
	cmdstr	= '\n\n  Examples:'
	cmdstr += '\n\tTo Create AISA Target with default size\n'
	cmdstr += '\t#> ' + argv[0] + ' --create AISA\n'
	cmdstr += '\n\tTo Create AISA Target with 100 GB size\n'
	cmdstr += '\t#> ' + argv[0] + ' --create AISA --size 100\n'
	return (help_create + cmdstr)

def dz_cli_help_io(argc, argv):
	argv[0]  = "avm"

	help_ior  = "  [1] For READ IOs\n"
	help_ior += '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_IO_READ) + ' > < DEVICE_NAME >  < SECTOR > < IO SIZE >'

	rcmdstr	= '\n\n  Examples:'
	rcmdstr += '\n\tTo issue READ IO on a DEVICE /dev/sda on SECTOR 1 and SIZE 4K\n'
	rcmdstr += '\t#> ' + argv[0] + ' -ior /dev/sda 1 4K\n\n'

	help_iow  = "  [2] For WRITE IOs\n"
	help_iow += '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_IO_WRITE) + ' > < DEVICE_NAME >  < SECTOR > < IO SIZE >'

	wcmdstr	= '\n\n  Examples:'
	wcmdstr += '\n\tTo issue WRITE IO on a DEVICE /dev/sda on SECTOR 1 and SIZE 4K\n'
	wcmdstr += '\t#> ' + argv[0] + ' -iow /dev/sda 1 4K\n\n'

	help_ioz  = "  [3] For ZEROED IOs\n"
	help_ioz += '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_IO_ZERO) + ' > < DEVICE_NAME >  < SECTOR > < IO SIZE >'

	zcmdstr	= '\n\n  Examples:'
	zcmdstr += '\n\tTo issue ZERO WRITE IO on a DEVICE /dev/sda on SECTOR 1 and SIZE 4K\n'
	zcmdstr += '\t#> ' + argv[0] + ' -ioz /dev/sda 1 4K\n\n'

	help_iorw  = "  [4] For issuing READ and WRITE IOs sequentially\n"
	help_iorw += '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_IO_READ_WRITE) + ' > < DEVICE_NAME >  < SECTOR > < IO SIZE >'

	rwcmdstr	= '\n\n  Examples:'
	rwcmdstr += '\n\tTo issue READ and WRITE IO on a DEVICE /dev/sda on SECTOR 1 and SIZE 4K\n'
	rwcmdstr += '\t#> ' + argv[0] + ' -iorw /dev/sda 1 4K\n\n'

	help_iowr  = "  [5] For issuing WRITE and READ IOs sequentially\n"
	help_iowr += '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_IO_WRITE_READ) + ' > < DEVICE_NAME >  < SECTOR > < IO SIZE >'

	wrcmdstr	= '\n\n  Examples:'
	wrcmdstr += '\n\tTo issue WRITE and READ IO on a DEVICE /dev/sda on SECTOR 1 and SIZE 4K\n'
	wrcmdstr += '\t#> ' + argv[0] + ' -iowr /dev/sda 1 4K\n\n'

	help_iorwr  = "  [6] For issuing READ, WRITE and READ IOs sequentially\n"
	help_iorwr += '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_IO_READ_WRITE_READ) + ' > < DEVICE_NAME >  < SECTOR > < IO SIZE >'

	rwrcmdstr	= '\n\n  Examples:'
	rwrcmdstr += '\n\tTo issue READ, WRITE and READ IO on a DEVICE /dev/sda on SECTOR 1 and SIZE 4K\n'
	rwrcmdstr += '\t#> ' + argv[0] + ' -iorwr /dev/sda 1 4K\n\n'

	help_iorzr  = "  [7] For issuing READ, ZERO WRITE and READ IOs sequentially\n"
	help_iorzr += '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_IO_READ_ZERO_READ) + ' > < DEVICE_NAME >  < SECTOR > < IO SIZE >'

	rzrcmdstr	= '\n\n  Examples:'
	rzrcmdstr += '\n\tTo issue READ, ZERO WRITE and READ IOs on a DEVICE /dev/sda on SECTOR 1 and SIZE 4K\n'
	rzrcmdstr += '\t#> ' + argv[0] + ' -iorzr /dev/sda 1 4K\n\n'

	help_io = "  Note:                                 \n"
	help_io += "   [1]: Use dmesg for getting the output\n"
	help_io += "   [2]: Max WRITE IO size is 16K (Will be truncated if given greater)\n"
	help_io += "   [3]: PreDefined data(string) is used for issuing WRITE IOs\n"

	#help_io = help_ior + rcmdstr + help_iow + wcmdstr + + help_iorw + rwcmdstr help_io;
	tmpstr = help_ior + rcmdstr + help_iow + wcmdstr + help_ioz + zcmdstr 
	tmpstr += help_iorw + rwcmdstr + help_iowr + wrcmdstr
	tmpstr += help_iorwr + rwrcmdstr + help_iorzr + rzrcmdstr + help_io
	help_io = tmpstr

	#print(help_io+cmdstr)
	return (help_io)

def dz_cli_help_help(argc, argv):
	argv[0] = "avm"
	cmdstr = '  #> '+argv[0]+' < '+' | '.join(CLI_OP_TYPE_HELP) + ' > \\'
	cmdstr += '\n    [ '+' | '.join(CLI_OP_TYPE_CREATE) + ' ] \\'
	cmdstr += '\n    [ '+' | '.join(CLI_OP_TYPE_DELETE) + ' ] \\'
	cmdstr += '\n    [ '+' | '.join(CLI_OP_TYPE_INFO) + ' ] \\'
	cmdstr += '\n    [ '+' | '.join(CLI_OP_TYPE_SIZE) + ' ] \\'
	cmdstr += '\n    [ '+' | '.join(CLI_OP_TYPE_STATS) + ' ] \\'
	cmdstr += '\n    [ '+' | '.join(CLI_OP_TYPE_LIST) + ' ] \n'
	return (cmdstr)

def dz_cli_help_all(argc, argv):
	argv[0] = "avm"

	cmdstr = '  ' + DZ_CLI_PRODUCT + " CLI Version    " + DZ_CLI_VERSION
	cmdstr += '\n  ' + DZ_CLI_PRODUCT + " Driver Version " + DZ_DRIVER_VERSION
	cmdstr += "\n  Current Date: "+getCurrentDateAndTime()
	cmdstr += '\n\n  Usage: \n'
	cmdstr += '\n  To Create AISA Target Device'
	cmdstr += '\n  ----------------------------\n'
	cmdstr += dz_cli_help_create_aisa(argc, argv)

	cmdstr += '\n  To Create Volumes'
	cmdstr += '\n  -----------------\n'
	cmdstr += dz_cli_help_create_vol(argc, argv)

	cmdstr += '\n  To Create ReadOnly and Writable Snapshot Volumes'
	cmdstr += '\n  ------------------------------------------------\n'
	cmdstr += dz_cli_help_create_volsnap(argc, argv)

	cmdstr += '\n  To Create Volumes dedicated for Virtual Machines'
	cmdstr += '\n  ------------------------------------------------\n'
	cmdstr += dz_cli_help_create_vmvol(argc, argv)

	cmdstr += '\n  To Create ReadOnly and Writable Snapshot Volumes for VM Volumes'
	cmdstr += '\n  ---------------------------------------------------------------\n'
	cmdstr += dz_cli_help_create_vmvolsnap(argc, argv)

	cmdstr += '\n  To Delete a Volume (Be it volume, snapshot, virtual machine volume)'
	cmdstr += '\n  -------------------------------------------------------------------\n'
	cmdstr += dz_cli_help_delete(argc, argv)

	cmdstr += '\n  To Destroy everything in one go'
	cmdstr += '\n  -------------------------------\n'
	cmdstr += dz_cli_help_destroy(argc, argv)

	cmdstr += '\n  To Issued READ and WRITE IOs on block devices'
	cmdstr += '\n  ---------------------------------------------\n'
	cmdstr += dz_cli_help_io(argc, argv)

	cmdstr += '\n  To List all Volumes'
	cmdstr += '\n  -------------------\n'
	cmdstr += '  #> ' + argv[0] + ' < '+' | '.join(CLI_OP_TYPE_LIST) + ' >\n'
		
	cmdstr += '\n  To show details/info of each volumes'
	cmdstr += '\n  ------------------------------------\n'
	cmdstr += dz_cli_help_info(argc, argv)

	cmdstr += '\n  To show sizes of volumes and block devices'
	cmdstr += '\n  ------------------------------------------\n'
	cmdstr += dz_cli_help_size(argc, argv)

	cmdstr += '\n  To show memory utilization and all engine counters'
	cmdstr += '\n  --------------------------------------------------\n'
	cmdstr += dz_cli_help_stats(argc, argv)

	cmdstr += '\n  To List all kernel modules'
	cmdstr += '\n  --------------------------\n'
	cmdstr += '  #> ' + argv[0] + ' < '+' | '.join(CLI_OP_TYPE_KERNEL_MODULES) + ' >\n'

	cmdstr += '\n  To Show status of all kernel modules and target device'
	cmdstr += '\n  ------------------------------------------------------\n'
	cmdstr += '  #> ' + argv[0] + ' < '+' | '.join(CLI_OP_TYPE_STATUS) + ' >\n'

	cmdstr += '\n  To Show LBA Table Entry for a given index'
	cmdstr += '\n  ------------------------------------------------------\n'
	cmdstr += '  #> ' + argv[0] + ' < '+' | '.join(CLI_OP_TYPE_LBA) + ' > < INDEX_NUMBER >\n'

	cmdstr += '\n  To Show PBA Table Entry for a given index'
	cmdstr += '\n  ------------------------------------------------------\n'
	cmdstr += '  #> ' + argv[0] + ' < '+' | '.join(CLI_OP_TYPE_PBA) + ' > < INDEX_NUMBER >\n'

	cmdstr += '\n  To Show sizeof of data structures present in system'
	cmdstr += '\n  ------------------------------------------------------\n'
	cmdstr += '  #> ' + argv[0] + ' < '+' | '.join(CLI_OP_TYPE_SIZEOF) + ' >\n'

	cmdstr += '\n  To List all sysfs entries created by AVM in Kerel'
	cmdstr += '\n  ------------------------------------------------------\n'
	cmdstr += '  #> ' + argv[0] + ' < '+' | '.join(CLI_OP_TYPE_SYSFS) + ' >\n'

	cmdstr += '\n  To Start AVM'
	cmdstr += '\n  ------------------------------------------------------\n'
	cmdstr += '  #> ' + argv[0] + ' < '+' | '.join(CLI_OP_TYPE_START) + ' >\n'

	cmdstr += '\n  To Stop AVM'
	cmdstr += '\n  ------------------------------------------------------\n'
	cmdstr += '  #> ' + argv[0] + ' < '+' | '.join(CLI_OP_TYPE_STOP) + ' >\n'

	cmdstr += '\n  To Restart (Stop and Start) AVM'
	cmdstr += '\n  ------------------------------------------------------\n'
	cmdstr += '  #> ' + argv[0] + ' < '+' | '.join(CLI_OP_TYPE_RESTART) + ' >\n'

	cmdstr += '\n  To show help'
	cmdstr += '\n  ------------\n'
	#cmdstr += '  #> ' + argv[0] + ' < '+' | '.join(CLI_OP_TYPE_HELP) + ' >\n'
	cmdstr += dz_cli_help_help(argc, argv)

	cmdstr += '  Note: Keywords and arguments are strictly in order.\n'
	        
	print ("%s" % cmdstr)


def dz_cli_help(argc, argv):

	if (argc == 1):
		dz_cli_help_all(argc, argv)
		return CLI_SUCCESS

	if (argc == 2):
		if (argv[1] in (CLI_OP_TYPE_HELP)):
			dz_cli_help_all(argc, argv)
			return CLI_SUCCESS
		else:
			dz_cli_help_all(argc, argv)
			return CLI_FAILURE

	if (argc == 3):
		if ((argv[1] in (CLI_OP_TYPE_HELP)) and (argv[2] in (CLI_OP_TYPE_CREATE))):
			cmdstr = '  ' + DZ_CLI_PRODUCT + " CLI Version    " + DZ_CLI_VERSION
			cmdstr += '\n  ' + DZ_CLI_PRODUCT + " Driver Version " + DZ_DRIVER_VERSION
			cmdstr += "\n  Current Date: "+getCurrentDateAndTime()
			cmdstr += '\n\n  Usage: \n'
			cmdstr += '\n  To Create AISA Target Device'
			cmdstr += '\n  ----------------------------\n'
			cmdstr += dz_cli_help_create_aisa(argc, argv)

			cmdstr += '\n  To Create Volumes'
			cmdstr += '\n  -----------------\n'
			cmdstr += dz_cli_help_create_vol(argc, argv)

			cmdstr += '\n  To Create ReadOnly and Writable Snapshot Volumes'
			cmdstr += '\n  ------------------------------------------------\n'
			cmdstr += dz_cli_help_create_volsnap(argc, argv)

			cmdstr += '\n  To Create Volumes dedicated for Virtual Machines'
			cmdstr += '\n  ------------------------------------------------\n'
			cmdstr += dz_cli_help_create_vmvol(argc, argv)

			cmdstr += '\n  To Create ReadOnly and Writable Snapshot Volumes for VM Volumes'
			cmdstr += '\n  ---------------------------------------------------------------\n'
			cmdstr += dz_cli_help_create_vmvolsnap(argc, argv)
			print(cmdstr)
			return CLI_SUCCESS
		elif (argv[1] in (CLI_OP_TYPE_HELP) and argv[2] in (CLI_OP_TYPE_DELETE)):
			cmdstr = '  ' + DZ_CLI_PRODUCT + " CLI Version    " + DZ_CLI_VERSION
			cmdstr += '\n  ' + DZ_CLI_PRODUCT + " Driver Version " + DZ_DRIVER_VERSION
			cmdstr += "\n  Current Date: "+getCurrentDateAndTime()
			cmdstr += '\n\n  Usage: \n'
			cmdstr += '\n  To Delete a Volume (Be it volume, snapshot, virtual machine volume)'
			cmdstr += '\n  -------------------------------------------------------------------\n'
			cmdstr += dz_cli_help_delete(argc, argv)
			print(cmdstr)
			return CLI_SUCCESS

		elif (argv[1] in (CLI_OP_TYPE_HELP) and argv[2] in (CLI_OP_TYPE_INFO)):
			cmdstr = '  ' + DZ_CLI_PRODUCT + " CLI Version    " + DZ_CLI_VERSION
			cmdstr += '\n  ' + DZ_CLI_PRODUCT + " Driver Version " + DZ_DRIVER_VERSION
			cmdstr += "\n  Current Date: "+getCurrentDateAndTime()
			cmdstr += '\n\n  Usage: \n'
			cmdstr += '\n  To show details/info of each volumes'
			cmdstr += '\n  ------------------------------------\n'
			cmdstr += dz_cli_help_info(argc, argv)
			print(cmdstr)
			return CLI_SUCCESS

		elif (argv[1] in (CLI_OP_TYPE_HELP) and argv[2] in (CLI_OP_TYPE_LIST)):
			cmdstr = '  ' + DZ_CLI_PRODUCT + " CLI Version    " + DZ_CLI_VERSION
			cmdstr += '\n  ' + DZ_CLI_PRODUCT + " Driver Version " + DZ_DRIVER_VERSION
			cmdstr += "\n  Current Date: "+getCurrentDateAndTime()
			cmdstr += '\n\n  Usage: \n'
			cmdstr += '\n  To List all Volumes'
			cmdstr += '\n  -------------------\n'
			cmdstr += '  #> ' + argv[0] + ' < '+' | '.join(CLI_OP_TYPE_LIST) + ' >\n'
			print(cmdstr)
			return CLI_SUCCESS

		elif (argv[1] in (CLI_OP_TYPE_HELP) and argv[2] in (CLI_OP_TYPE_SIZE)):
			cmdstr = '  ' + DZ_CLI_PRODUCT + " CLI Version    " + DZ_CLI_VERSION
			cmdstr += '\n  ' + DZ_CLI_PRODUCT + " Driver Version " + DZ_DRIVER_VERSION
			cmdstr += "\n  Current Date: "+getCurrentDateAndTime()
			cmdstr += '\n\n  Usage: \n'
			cmdstr += '\n  To show sizes of volumes and block devices'
			cmdstr += '\n  ------------------------------------------\n'
			cmdstr += dz_cli_help_size(argc, argv)
			print(cmdstr)
			return CLI_SUCCESS

		elif (argv[1] in (CLI_OP_TYPE_HELP) and argv[2] in (CLI_OP_TYPE_STATS)):
			cmdstr = '  ' + DZ_CLI_PRODUCT + " CLI Version    " + DZ_CLI_VERSION
			cmdstr += '\n  ' + DZ_CLI_PRODUCT + " Driver Version " + DZ_DRIVER_VERSION
			cmdstr += "\n  Current Date: "+getCurrentDateAndTime()
			cmdstr += '\n\n  Usage: \n'
			cmdstr += '\n  To show memory utilization and all engine counters'
			cmdstr += '\n  --------------------------------------------------\n'
			cmdstr += dz_cli_help_size(argc, argv)
			print(cmdstr)
			return CLI_SUCCESS
		else:
			print(dz_cli_help_help(argc, argv))
			return CLI_FAILURE

	else:
		dz_cli_help_all(argc, argv)
		return CLI_FAILURE
