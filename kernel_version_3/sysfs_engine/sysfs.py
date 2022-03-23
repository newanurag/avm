#!/usr/bin/python

from __future__ import print_function
import 	os
import	sys
#from AISA_Utils import *
TAB 	= '\t'
NL		= '\n'
NL2		= '\n\n'
TAB2	= '\t\t'
TAB3	= '\t\t\t'

#This Parent is already created inside avm_sysfs_engine_init
KOBJECT_PARENT	= '/sys/avm/'
#KOBJECT_NAME	= 'avm'
#G_KOBJECT_NAME	= 'sysfs_' + KOBJECT_NAME

def Trim(arg_str):
	return arg_str.strip()

def	sysfs_declarations(arg_list, arg_kobj):
	str = ""
	str += "/* NOTE : This is a System generated file. Changes to this file will be overwritten */" + NL
	str += '#include "prototypes.h"' + NL2
	str += 'static struct kobject *dz_kobj = NULL;' + NL2
	str += 'extern struct kobject *sysfs_avm_kernel_kobj;' + NL2

	#KOBJECT_NAME	= arg_kobj
	#G_KOBJECT_NAME	= KOBJECT_NAME + '_sysfs_entry'
	G_KOBJECT_NAME	= 'avm_sysfs_entry'

	for line in arg_list:
		if line.startswith('#'):
			continue
		fields 				= Trim(line).split('|')
		name 				= Trim(fields[0])
		type 				= Trim(fields[1])
		ops 				= Trim(fields[2])
		val 				= Trim(fields[3])
		get_func 			= Trim(fields[4])
		set_func 			= Trim(fields[5])

		varname	= G_KOBJECT_NAME + '_' + name
		if type == 'STRING':
			str += "CHAR " + varname + "[DZ_SYSFS_PAGE_SIZE] = {'\\0'};" + NL

		elif type == 'INTEGER':
			str += "INT " + varname + " = 0;" + NL

		elif type == 'U64':
			str += "U64 " + varname + " = 0;" + NL

		name_attr = G_KOBJECT_NAME + '_' + name + '_attr'
		if ops == 'BOTH':
			fun_show 	= G_KOBJECT_NAME + '_' + name + '_show'
			fun_store 	= G_KOBJECT_NAME + '_' + name + '_store'
			str += 'static ssize_t ' + fun_show + '(struct kobject *kobj, struct kobj_attribute *attr, char *buf); ' + NL
			str += 'static ssize_t ' + fun_store + '(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); ' + NL

		elif ops == 'SHOW':
			fun_show 	= G_KOBJECT_NAME + '_' + name + '_show'
			fun_store 	= 'NULL'
			str += 'static ssize_t ' + fun_show + '(struct kobject *kobj, struct kobj_attribute *attr, char *buf); ' + NL

		elif ops == 'STORE':
			fun_store 	= G_KOBJECT_NAME + '_' + name + '_store'
			fun_show 	= 'NULL'
			str += 'static ssize_t ' + fun_store + '(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n); ' + NL

		str += 'static struct kobj_attribute ' + name_attr + ' =' + NL
		str += TAB + '__ATTR('+name+', 0644, '+fun_show +', ' + fun_store + ');' + NL2

	#print(str)

	return str

def sysfs_define_fun_show(fun_name, type, var_name, fun_name_from_raw_file):
	str = ""
	str += 'static ssize_t ' + fun_name + '(struct kobject *kobj, struct kobj_attribute *attr, char *buf) ' + NL
	str += '{' + NL
	str += TAB + 'INT count = 0;' + NL
	str += TAB + 'CHAR *ptr = buf;' + NL

	if 	fun_name_from_raw_file.startswith('#'):
		fun_name_from_raw_file = ""

	if 	fun_name_from_raw_file.startswith('NULL'):
		fun_name_from_raw_file = ""

	if fun_name_from_raw_file != "":
		str += TAB + 'count = ' + fun_name_from_raw_file + '(buf);' + NL

	if type == 'STRING':
		str += TAB + 'return sprintf((ptr += count), "#'+var_name+' : %s\\n",'+var_name+');' + NL

	if type == 'INTEGER':
		str += TAB + 'return sprintf((ptr += count), "#'+var_name+' : %d\\n",'+var_name+');' + NL

	if type == 'U64':
		str += TAB + 'return sprintf((ptr += count), "#'+var_name+' : %lli\\n",'+var_name+');' + NL

	str += '}' + NL2

	return str

def sysfs_define_fun_store(fun_name, type, var_name, set_fun_name_from_raw_file):
	str = ""
	str += 'static ssize_t ' + fun_name + '(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len) ' + NL
	str += '{' + NL
	str += TAB + 'INT ret = SUCCESS ;' + NL

	if 	set_fun_name_from_raw_file.startswith('#'):
		set_fun_name_from_raw_file = ""

	if 	set_fun_name_from_raw_file.startswith('NULL'):
		set_fun_name_from_raw_file = ""

	if type == 'STRING':
		str += TAB + 'ret = sscanf(buf, "%s", '+var_name+');' + NL

	if type == 'INTEGER':
		str += TAB + 'ret = sscanf(buf, "%d", &'+var_name+');' + NL
		if set_fun_name_from_raw_file != "":
			str += TAB + set_fun_name_from_raw_file + '(' + var_name + ');' + NL

	if type == 'U64':
		str += TAB + 'ret = sscanf(buf, "%lli", &'+var_name+');' + NL
		if set_fun_name_from_raw_file != "":
			str += TAB + set_fun_name_from_raw_file + '(' + var_name + ');' + NL

	str += TAB + 'return len;' + NL
	str += '}' + NL2

	return str

def	sysfs_function_definitions(arg_list, arg_kobj):
	str = ""

	#KOBJECT_NAME	= arg_kobj
	#G_KOBJECT_NAME	= KOBJECT_NAME + '_sysfs_entry'
	G_KOBJECT_NAME	= 'avm_sysfs_entry'

	for line in arg_list:
		if line.startswith('#'):
			continue
		fields 	= 			Trim(line).split('|')
		name 	= 			Trim(fields[0])
		type 	= 			Trim(fields[1])
		ops 	= 			Trim(fields[2])
		val 	= 			Trim(fields[3])
		c_lang_get_func 	= Trim(fields[4]) # C Function passed in raw file.
		c_lang_set_func 	= Trim(fields[5]) # C function passed in raw file.

		varname	= G_KOBJECT_NAME + '_' + name
		if type == 'STRING':
			str += ""

		elif type == 'INTEGER':
			str += ""

		name_attr = G_KOBJECT_NAME + '_' + name + '_attr'
		if ops == 'BOTH':
			fun_show 	= G_KOBJECT_NAME + '_' + name + '_show'
			fun_store 	= G_KOBJECT_NAME + '_' + name + '_store'
			str += sysfs_define_fun_show(fun_show, type, varname, c_lang_get_func)
			str += sysfs_define_fun_store(fun_store, type, varname, c_lang_set_func)

		elif ops == 'SHOW':
			fun_show 	= G_KOBJECT_NAME + '_' + name + '_show'
			fun_store 	= 'NULL'
			str += sysfs_define_fun_show(fun_show, type, varname, c_lang_get_func)

		elif ops == 'STORE':
			fun_store 	= G_KOBJECT_NAME + '_' + name + '_store'
			fun_show 	= 'NULL'
			str += sysfs_define_fun_store(fun_store, type, varname, c_lang_set_func)

	#print(str)

	return str


def	sysfs_init(arg_engine_name, arg_list, arg_kobj):
	str = ""
	KOBJECT_NAME	= arg_kobj

	if len(arg_engine_name) == 0:
		str += 'INT avm_sysfs_init(VOID)' + NL
	else:
		str += 'INT avm_sysfs_' + arg_engine_name + '_init(VOID)' + NL

	str += '{' + NL
	str += TAB + 'INT ret = SUCCESS;' + NL
	#str += TAB + 'dz_kobj = kobject_create_and_add("'+KOBJECT_NAME+'", kernel_kobj);' + NL
	str += TAB + 'dz_kobj = kobject_create_and_add("'+KOBJECT_NAME+'", sysfs_avm_kernel_kobj);' + NL
	str += TAB + 'if (!dz_kobj)' + NL
	str += TAB2 + 'return -ENOMEM;' + NL

	#G_KOBJECT_NAME	= KOBJECT_NAME + '_sysfs_entry'
	G_KOBJECT_NAME	= 'avm_sysfs_entry'

	for line in arg_list:
		if line.startswith('#'):
			continue
		fields = Trim(line).split('|')
		name 	= Trim(fields[0])
		type 	= Trim(fields[1])
		ops 	= Trim(fields[2])
		val 	= Trim(fields[3])
		func 	= Trim(fields[4])

		name_attr = G_KOBJECT_NAME + '_' + name + '_attr'
		name_entry = KOBJECT_PARENT + KOBJECT_NAME + '/' + name

		str += TAB + 'ret = sysfs_create_file(dz_kobj,&'+name_attr+'.attr);' + NL
		str += TAB + 'if (ret == SUCCESS) {' + NL
		str += TAB2 + 'LOGSYS("sysfs entry '+name_entry+' created \\n");' + NL
		str += TAB + '} else {' + NL
		str += TAB2 + 'LOGSYSE("sysfs entry '+name_entry+' creation failed \\n");' + NL
		str += TAB2 + 'RETURNF;' + NL
		str += TAB + '}' + NL2

	str += TAB + 'RETURNS;' + NL2
	str += '}' + NL
	#print(str)

	return str

def	sysfs_exit(arg_engine_name, arg_str, arg_kobj):
	se_str = ''

	if len(arg_engine_name) == 0:
		se_str += 'RVOID avm_sysfs_exit(VOID)' + NL
	else:
		se_str += 'RVOID avm_sysfs_' + arg_engine_name + '_exit(VOID)' + NL

	se_str += '{' + NL
	se_str += TAB + 'kobject_put(dz_kobj);' + NL
	se_str += TAB + 'LOGSYS("dz_sysfs freed\\n");' + NL
	se_str += '}'
	#print(se_str)
	return se_str


def read_raw_file(fname):
	with open(fname, "r") as filep:
		lines = filep.readlines()

	return lines

#arg_kobj will be visible in /sys/avm/ directory
def autogen_code_for_file(arg_engine_name, arg_raw_file_name, arg_out_c_file_name, arg_kobj):

	#Open Raw file for reading
	lines = read_raw_file(arg_raw_file_name)

	#Open Out file for writing
	with open(arg_out_c_file_name, "w") as filep:
		filep.write(sysfs_declarations(lines, arg_kobj))
		filep.write(sysfs_function_definitions(lines, arg_kobj))
		filep.write(sysfs_init(arg_engine_name, lines, arg_kobj))
		filep.write(sysfs_exit(arg_engine_name, lines, arg_kobj))


def main():

	engine_name			= "kernel"
	raw_file_name 		= "sysfs.raw"				# Input Raw file
	autogen_c_file		= "sysfs_kernel_autogen.c" 	# This is the output file being generated
	sysfs_entry_name 	= "kernel"					# This is the sys entry name present in /sys/avm
	autogen_code_for_file(engine_name, raw_file_name, autogen_c_file, sysfs_entry_name)
	ostr = ("Generated .c file for " + engine_name).ljust(40) + " : "+autogen_c_file
	print(ostr)

	engine_name			= "target_engine"
	raw_file_name 		= "sysfs_target_engine.raw"
	autogen_c_file		= "sysfs_target_engine_autogen.c"
	sysfs_entry_name 	= "target_engine"
	autogen_code_for_file(engine_name, raw_file_name, autogen_c_file, sysfs_entry_name)
	ostr = ("Generated .c file for " + engine_name).ljust(40) + " : "+autogen_c_file
	print(ostr)

	engine_name			= "alignment_engine"
	raw_file_name 		= "sysfs_alignment_engine.raw"
	autogen_c_file		= "sysfs_alignment_engine_autogen.c"
	sysfs_entry_name 	= "alignment_engine"
	autogen_code_for_file(engine_name, raw_file_name, autogen_c_file, sysfs_entry_name)
	ostr = ("Generated .c file for " + engine_name).ljust(40) + " : "+autogen_c_file
	print(ostr)

	engine_name			= "bec_engine"
	raw_file_name 		= "sysfs_bec_engine.raw"
	autogen_c_file		= "sysfs_bec_engine_autogen.c"
	sysfs_entry_name 	= "back_end_cache_engine"
	autogen_code_for_file(engine_name, raw_file_name, autogen_c_file, sysfs_entry_name)
	ostr = ("Generated .c file for " + engine_name).ljust(40) + " : "+autogen_c_file
	print(ostr)

	engine_name			= "fec_engine"
	raw_file_name 		= "sysfs_fec_engine.raw"
	autogen_c_file		= "sysfs_fec_engine_autogen.c"
	sysfs_entry_name 	= "front_end_cache_engine"
	autogen_code_for_file(engine_name, raw_file_name, autogen_c_file, sysfs_entry_name)
	ostr = ("Generated .c file for " + engine_name).ljust(40) + " : "+autogen_c_file
	print(ostr)

	engine_name			= "dedupe_engine"
	raw_file_name 		= "sysfs_dedupe_engine.raw"
	autogen_c_file		= "sysfs_dedupe_engine_autogen.c"
	sysfs_entry_name 	= "dedupe_engine"
	autogen_code_for_file(engine_name, raw_file_name, autogen_c_file, sysfs_entry_name)
	ostr = ("Generated .c file for " + engine_name).ljust(40) + " : "+autogen_c_file
	print(ostr)

	engine_name			= "volume_engine"
	raw_file_name 		= "sysfs_volume_engine.raw"
	autogen_c_file		= "sysfs_volume_engine_autogen.c"
	sysfs_entry_name 	= "volume_engine"
	autogen_code_for_file(engine_name, raw_file_name, autogen_c_file, sysfs_entry_name)
	ostr = ("Generated .c file for " + engine_name).ljust(40) + " : "+autogen_c_file
	print(ostr)

	engine_name			= "metadata_engine"
	raw_file_name 		= "sysfs_metadata_engine.raw"
	autogen_c_file		= "sysfs_metadata_engine_autogen.c"
	sysfs_entry_name 	= "metadata_engine"
	autogen_code_for_file(engine_name, raw_file_name, autogen_c_file, sysfs_entry_name)
	ostr = ("Generated .c file for " + engine_name).ljust(40) + " : "+autogen_c_file
	print(ostr)

main()
