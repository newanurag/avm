from __future__ import print_function
import os
import sys
import trace

SUCCESS	= 0
FAILURE	= 1

CMD		= -1
ERROR	= 0
WARN	= 1
INFO	= 2
DEBUG	= 3

global LOG_LEVEL
LOG_LEVEL	= DEBUG

LOG_DICT = { ERROR : "ERROR", INFO : "INFO ", WARN : "WARN ", DEBUG : "DEBUG", CMD : "CMDLN"};


AISA_PV_DOES_NOT_EXIST	= 0
AISA_PV_EXIST		= 101

AISA_VG_DOES_NOT_EXIST	= 0
AISA_VG_EXIST		= 102

AISA_LV_DOES_NOT_EXIST	= 0
AISA_LV_EXIST		= 103

