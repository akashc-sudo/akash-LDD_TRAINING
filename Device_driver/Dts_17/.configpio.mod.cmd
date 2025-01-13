savedcmd_/home/pi/akash/Dts/configpio.mod := printf '%s\n'   configpio.o | awk '!x[$$0]++ { print("/home/pi/akash/Dts/"$$0) }' > /home/pi/akash/Dts/configpio.mod
