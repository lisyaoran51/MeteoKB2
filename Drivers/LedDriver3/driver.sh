sudo insmod meteo_light_board_v1.ko
sudo mknod /dev/meteo_lightboard_v1 c 201 0
sudo chmod 666 /dev/meteo_lightboard_v1