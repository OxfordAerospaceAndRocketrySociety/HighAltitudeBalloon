//2024 Oxford Aerospace and Rocketry Society
//Layout for the tracker / reciever PCB
//Comment out each individual part to print

$fn = 20; //20 Sides for each hole

difference(){
    union(){
        color("DodgerBlue",1) //PCB
        translate([0,0,3])
            cube([76.2,55.27,0.5]);
        
        translate([76.2-8,0,0])
            union(){
                color("ForestGreen", 1)
                    rightSide();
                color("FireBrick", 1)
                    topSide();
            }
        color("DarkSlateGray", 1)
            leftSide();
    }
        translate([2.54,2.54,0])//Mounting Holes
            cylinder(r = 1.5, h = 4);
        translate([2.54,50.8,0])
            cylinder(r = 1.5, h = 4);
        translate([73.66,50.8,0])
            cylinder(r = 1.5, h = 4);
        translate([73.66,2.54,0])
            cylinder(r = 1.5, h = 4);
}


module rightSide(){
    cube([8,55.27+15,1.5]);
    translate([8,55.27,0])
        cube([190,15,3]);
}

module topSide(){
    translate([0,0,1.5])
        cube([8,55.27+15,1.5]);
    translate([-22-7.5,55.27,0])
        cube([15,175,3]);
    translate([-22+7.5,55.27,0])
        cube([22-7.5,15,3]);
    
}

module leftSide(){
    cube([8,55.27,3]);
    translate([22-195+16.5,55.27,0])
        cube([195,15,3]);
    
}