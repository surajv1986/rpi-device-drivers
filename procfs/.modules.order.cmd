cmd_/home/pi/driver_programs/procfs/modules.order := {   echo /home/pi/driver_programs/procfs/driver.ko; :; } | awk '!x[$$0]++' - > /home/pi/driver_programs/procfs/modules.order
