include $(srctree)/mycompany/avm/mobile_kernel.mk

KBUILD_EXTRA_SYMBOLS := $(obj)/../infra_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../bec_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../metadata_engine/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(obj)/../dedupe_engine/Module.symvers


#########################################################################
# Frontend Cache Engine Source Files
#########################################################################

SRC_DIR                     := 
FEC_ENGINE_SRC_FILES        := $(SRC_DIR)fec_main.c
FEC_ENGINE_SRC_FILES        += $(SRC_DIR)fec_tables.c
FEC_ENGINE_SRC_FILES        += $(SRC_DIR)fec_align_read.c
FEC_ENGINE_SRC_FILES        += $(SRC_DIR)fec_align_read_single.c
FEC_ENGINE_SRC_FILES        += $(SRC_DIR)fec_align_read_partial.c
FEC_ENGINE_SRC_FILES        += $(SRC_DIR)fec_align_read_multi.c
FEC_ENGINE_SRC_FILES        += $(SRC_DIR)fec_align_write.c
FEC_ENGINE_SRC_FILES        += $(SRC_DIR)fec_align_write_single.c
FEC_ENGINE_SRC_FILES        += $(SRC_DIR)fec_align_write_partial.c
FEC_ENGINE_SRC_FILES        += $(SRC_DIR)fec_align_write_multi.c
#FEC_ENGINE_SRC_FILES        += $(SRC_DIR)/fec_write.c
FEC_ENGINE_SRC_FILES        += $(SRC_DIR)fec_flush.c
FEC_ENGINE_SRC_FILES        += $(SRC_DIR)fec_replay.c
FEC_ENGINE_SRC_FILES        += $(SRC_DIR)fec_thread_pool.c
SRC_FILES                   += $(FEC_ENGINE_SRC_FILES)
OBJ_FILES                   := $(SRC_FILES:.c=.o)


#########################################################################
# Kernel Module Generation
#########################################################################

KERNEL_MODULE_NAME          := $(DZ_FEC_ENGINE_KERNEL_MODULE_NAME)
$(KERNEL_MODULE_NAME)-y 	:= $(OBJ_FILES)
obj-m 						:= $(KERNEL_MODULE_NAME).o
