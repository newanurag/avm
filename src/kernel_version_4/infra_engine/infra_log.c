/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "prototypes.h"

#define DZ_AVM_LOG_FILE     "/dev/dzlog"
struct file *dz_log_filep = NULL;
EXPORT_SYMBOL(dz_log_filep);

ATOMIC64 dz_avm_log_file_index;
EXPORT_SYMBOL(dz_avm_log_file_index);

RINT dz_avm_log_file_open(VOID)
{
	dz_log_filep = dz_file_open(DZ_AVM_LOG_FILE, O_WRONLY, 0); 
    if (!dz_log_filep) {
        LOGE("Error in Opening Log File %s\n", DZ_AVM_LOG_FILE);
		RETURNF;
    } else {
        LOG("Log File %s Opened successfully\n", DZ_AVM_LOG_FILE);
		RETURNS;
    }   
	ATOMIC64_SET(dz_avm_log_file_index, 0);
}
EXPORT_SYMBOL(dz_avm_log_file_open);

RINT dz_avm_log_file_write_raw(CHAR *data, UINT size )
{
	return dz_file_write(dz_log_filep, data, size, 0);
}
EXPORT_SYMBOL(dz_avm_log_file_write_raw);

RINT dz_avm_log_file_write(const char *format, ...)
{
	//TODO:This needs to be optimized
	//We also needs to protect this with some kind of lock
	CHAR logbuf[128]; 
	INT count;
	INT count2;
	va_list args;
	MEMSET(logbuf);
	ATOMIC64_INC(dz_avm_log_file_index);
	va_start(args, format);
	count = sprintf(logbuf,"%lli:[%lli]", (U64)GET_CURRENT_TIME(), (U64)ATOMIC64_READ(dz_avm_log_file_index));
	count2 = vsprintf(logbuf+count, format, args );
	//va_end(args);
	//LOGR("AVERMA:dz_log_filep=%p, logbuf=%s, count=%d\n", dz_log_filep, logbuf, count+count2);
	return dz_file_write(dz_log_filep, logbuf, count+count2, 0);
}

EXPORT_SYMBOL(dz_avm_log_file_write);

RVOID dz_avm_log_file_close()
{
	dz_file_close(dz_log_filep);
	return;
}
EXPORT_SYMBOL(dz_avm_log_file_close);
