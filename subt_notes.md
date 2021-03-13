## DARPA SubT CHALLENGE NOTES

- ### TODO

1. install simulator
2. install MATLAB
3. run simple SLAM

---
1. run SubT simulator in docker
> ``` shell
> ./run.bash osrf/subt-virtual-testbed tunnel_circuit_practice.ign worldName:=tunnel_circuit_practice_01 robotName1:=X1 robotConfig1:=X1_SENSOR_CONFIG_1

> ``` shell
> docker exec -it `docker ps --filter ancestor=osrf/subt-virtual-testbed --format {{.ID}}` /bin/bash


2. run SubT simulator locally
> ``` shell
> # Build and install into workspace
> cd ~/subt_ws/
> catkin_make install
> source ~/subt_ws/install/setup.bash

- launch X1
> ign launch -v 4 cave_circuit.ign worldName:=cave_circuit_practice_01 robotName1:=X1 robotConfig1:=X1_SENSOR_CONFIG_1

- launch COSTAR_HUSKY
> ign launch -v 4 cave_circuit.ign worldName:=cave_circuit_practice_01 robotName1:=COSTAR_HUSKY robotConfig1:=COSTAR_HUSKY_SENSOR_CONFIG_1


3. vscodexros
> catkin_make -DCMAKE_EXPORT_COMPILE_COMMANDS=Yes

4. packages
> pointcloud-to-laserscan
> octomap_mapping: https://github.com/OctoMap/octomap_mapping
> loam: https://github.com/laboshinl/loam_velodyne
