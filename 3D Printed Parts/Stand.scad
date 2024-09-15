//2024 Oxford Aerospace and Rocketry Society
//Simple stand for displaying a tracker

cube([80,40,3],center = true); //Base

translate([0,0,56/2]) //Back stand
rotate([75,0,0])
cube([80,56,2],center = true);

translate([0,-12,2]) //Front rest
rotate([-15,0,0])
cube([80,10,2],center = true);