clc;
clear;
close all;
rosshutdown;

rosinit('localhost')

% load test_map.mat;

gmapping = rossubscriber('/map');
map_data = receive(gmapping);

width = map_data.Info.Width;
height = map_data.Info.Height;

raw_data = reshape(map_data.Data,[width, height])';
raw_data=flip(raw_data);
raw_data=rot90(raw_data);

raw_data(raw_data == -1) = 50;     % unkown
raw_data(raw_data == 0) = 255;     % free
raw_data(raw_data == 100) = 0;    % obstacle

imshow(raw_data)


