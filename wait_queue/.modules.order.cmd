cmd_/home/pi/driver_programs/wait_queue/modules.order := {   echo /home/pi/driver_programs/wait_queue/driver.ko; :; } | awk '!x[$$0]++' - > /home/pi/driver_programs/wait_queue/modules.order
