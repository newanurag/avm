
# Variables that are same for all platforms
DZ_PROJECT_BASE_PATH           := /mnt/laptopdisk/avm/infra_engine
DZ_PROJECT_PATH                := /mnt/laptopdisk/avm/infra_engine
DZ_HEADERS_PATH                := /mnt/laptopdisk/avm/infra_engine/headers
DZ_SYSFS_ENGINE_PATH           := /mnt/laptopdisk/avm/infra_engine/sysfs_engine
DZ_SYSFS_ENGINE_PATH_RELATIVE  := sysfs_engine
DZ_HASH_PATH                   := /mnt/laptopdisk/avm/infra_engine/hash_engine
DZ_HASH_PATH_RELATIVE          := hash_engine
DZ_SUBDIRS                     := sysfs
DZ_CURRENT_VERSION             := 1.0

# Kernel Module Names of their corresponding engines
# Dependency is represented as number sequence. Higher number value is dependent on lower number
DZ_LOG_ENGINE_KERNEL_MODULE_NAME       := datumsoft_zaidstor_avm_1_B_log_engine_module
DZ_INFRA_ENGINE_KERNEL_MODULE_NAME     := datumsoft_zaidstor_avm_2_B_infra_engine_module
DZ_LAB_ENGINE_KERNEL_MODULE_NAME       := datumsoft_zaidstor_avm_3_B_lab_engine_module
DZ_BEC_ENGINE_KERNEL_MODULE_NAME       := datumsoft_zaidstor_avm_4_B_bec_engine_module
DZ_METADATA_ENGINE_KERNEL_MODULE_NAME  := datumsoft_zaidstor_avm_5_B_metadata_engine_module
DZ_DEDUPE_ENGINE_KERNEL_MODULE_NAME    := datumsoft_zaidstor_avm_6_B_dedupe_engine_module
DZ_ALIGNMENT_ENGINE_KERNEL_MODULE_NAME := datumsoft_zaidstor_avm_7_B_alignment_engine_module
DZ_FEC_ENGINE_KERNEL_MODULE_NAME       := datumsoft_zaidstor_avm_8_B_fec_engine_module
DZ_TARGET_ENGINE_KERNEL_MODULE_NAME    := datumsoft_zaidstor_avm_9_B_target_engine_module
DZ_VOLUME_ENGINE_KERNEL_MODULE_NAME    := datumsoft_zaidstor_avm_A_B_volume_engine_module
DZ_SYSFS_ENGINE_KERNEL_MODULE_NAME     := datumsoft_zaidstor_avm_B_B_sysfs_engine_module


#Generated on Sat Mar 12 02:17:38 IST 2022
