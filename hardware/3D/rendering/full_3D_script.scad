
$fn= 15;

import("render_v9_top1_5.scad.stl", convexity=3);
import("render_v9_bottom.scad.stl", convexity=3);
translate([0,0,-1])linear_extrude(height = 0.6, convexity = 0)import("../DXF/MiniChord-User_front_plate_engraving.dxf", convexity=3);

translate([0,0,-3])linear_extrude(height = 4.5, convexity = 0)offset(r=0.2)offset(r=-0.3)import("../DXF/buttons.dxf", convexity=3);

translate([0,0,-3])linear_extrude(height = 11.5, convexity = 0)offset(r=-0.6)import("../DXF/potentiometers.dxf", convexity=3);

translate([61,-60,0]){
    translate([0,0,-3])linear_extrude(height = 4, convexity = 0)import("../DXF/touch_zone-Edge_Cuts.dxf", convexity=3);
}