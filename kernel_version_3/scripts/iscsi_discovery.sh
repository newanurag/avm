#During Login:
iscsiadm -m discovery -t sendtargets -p 192.168.1.5
service iscsid restartd
iscsiadm --mode node --targetname iqn.2003-01.org.linux-iscsi.aisa-machine.x8664:sn.2417e8cec631 --portal 192.168.1.5:3260 --login


#During Logout
iscsiadm --mode node --targetname iqn.2003-01.org.linux-iscsi.aisa-machine.x8664:sn.5135538688ba --portal 192.168.1.5:3260 --logout
service iscsid restartd

fdisk -l 
lsscsi

#Also on LIO Go to tpg and set authentication to 0

/iscsi/iqn.20...538688ba/tpg1> set attribute authentication=0
Parameter authentication is now '0'.
/iscsi/iqn.20...538688ba/tpg1> 

/iscsi/iqn.20...538688ba/tpg1> 
/iscsi/iqn.20...538688ba/tpg1> ls
o-tpg1 ..................................................................................................... [no-gen-acls, no-auth]
  o-acls ................................................................................................................ [ACLs: 1]
  | o- iqn.1994-05.com.redhat:8d3e34e697e6 ........................................................................ [Mapped LUNs: 1]
  |   o- mapped_lun121 ...................................................................... [lun121 block/aisa_backstore_121 (rw)]
  o- luns ................................................................................................................ [LUNs: 1]
  | o- lun121 .................................................................... [block/aisa_backstore_121 (/dev/vg_aisp/lv_aisp)]
  o- portals .......................................................................................................... [Portals: 1]
    o- 192.168.1.5:3260 ....................................................................................................... [OK]
/iscsi/iqn.20...538688ba/tpg1> 

