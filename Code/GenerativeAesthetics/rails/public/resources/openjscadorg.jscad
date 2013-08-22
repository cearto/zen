

// Here we define the user editable parameters: 
function getParameterDefinitions() {
  return [
    { name: 'slength', caption: 'Side length:', type: 'int', default: 1, domain: [0, 5], cont: false},
    { name: 'resolution', caption: 'Resolution:', type: 'int', default: 8, domain: [8, 12], cont: false},
    { name: 'rounded', caption: 'Round:', type: 'float', default: 0.2, domain: [0, 0.2], cont: true},
    { name: 'scale', caption: 'Scale:', type: 'float', default: 100, domain: [90,  100], cont: false},
    { name: 'sradius', caption: 'Sphere radius:', type: 'float', default: 0.5, domain: [0, 2], cont: true},
    { name: 'cstart', caption: 'Cone start radius:', type: 'float', default: 1, domain: [0, 1.5], cont: true},
    { name: 'cend', caption: 'Cone end radius:', type: 'float', default: 1, domain: [0, 1.5], cont: true}
  ];
}

// Main entry point; here we construct our solid: 
function main(params)
{
var sphere = CSG.sphere({
       center: [0, 0, 0],
       radius: params.sradius,
       resolution: 32,
     });

var roundCube = CSG.roundedCube({   
       center: [0, 0, 0],
       radius: params.slength,
       resolution: params.resolution,
       roundradius: params.rounded
    });

var cone = CSG.cylinder({
  start: [0, 0, 0],
  end: [0, 0, 1],
  radiusStart: params.cstart,                   // start- and end radius defined, partial cones
  radiusEnd: params.cend,
  resolution: 16
});



cone = cone.scale(sf(params.scale * 0.8));
sphere = sphere.scale(sf(params.scale * 1.2));
roundCube = roundCube.scale(sf(params.scale * 0.5));
a = sphere.intersect(roundCube).intersect(cone);
b = sphere.intersect(roundCube).intersect(cone);
b = b.scale(sf(params.scale * 0.9));//.translate([0, 1, 0]);
return a;

// .union(
//       .union(
//      


//      ).setColor(1,0.5,0.3).scale([params.scale, params.scale, params.scale]);
//.union(cube({size: 1.5, center:true}));  // object-oriented
 //  return CSG.roundedCube({   
 //    center: [0, 0, 0],
 //    radius: params.slength,
 //    resolution: params.resolution,
 //    roundradius: params.rounded
 // } );
}
function sf(s){
  return [s,s,s];
}
