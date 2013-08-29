

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


return CSG.cube({
  center: [0, 0, 0],
  radius: [8, 8, 8]
});


}
function sf(s){
  return [s,s,s];
}
