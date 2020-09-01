% Displays data from 'frame.mat' previously saved with 'export_mat.py' example.
% It displays spacially adjacent measurements in matrix format.
% Actual point clouds (XYZ) are not covered in this example.

%
% Copyright (c) 2020 Blickfeld GmbH.
% All rights reserved.
%
% This source code is licensed under the BSD-style license found in the
% LICENSE.md file in the root directory of this source tree.

load('frame.mat')  

figure('Name','Azimuth');
imshow(azimuth, []);

figure('Name','Elevation');
imshow(elevation, []);

figure('Name','Time offset');
imshow(double(start_offset_ns), []);

figure('Name','Range');
imshow(range, []);

figure('Name','Ambient Light');
% double is the supremum primitive image type of uint32 in octave
imshow(double(ambient_light_level), []); 
