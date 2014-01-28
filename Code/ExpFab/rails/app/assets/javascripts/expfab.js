
function ExpFab(obj, regions, pca, filepath){
  var filesplit = filepath.split('/');
  this.filename = filesplit[filesplit.length-1].split('.stl')[0]; 
  //scene variables
	this.object = obj;
  this.color = defaultMaterialSettings.color;
  this.wireframe = defaultMaterialSettings.wireframe;
  this.shininess = defaultMaterialSettings.shininess;

  // deformation variables
  this.datastream = 'Uniform';
  this.operation = 'None';
	this.original = obj.geometry.clone().vertices;
  this.current = obj.geometry.clone().vertices;
	
  this.regions = []; for(var i in regions) this.regions.push(new Region(this, i, regions[i], pca[i]));
	
	this.id = scene.userData.objects.length;
  this.mgui = new MeshGUI();
  this.dgui = new dat.GUI();

  this.mutator = new Mutator(this);
  this.dna = null;
}

ExpFab.prototype.getOp = function(i){
  var n = this.mutator.headers[i];
  return this.mutator.headers[i].slice(0, - 1);
}

ExpFab.prototype.load = function(fileinfo) {
  var that = this;

  activeDataStream = this.datastream;

  var f1 = this.dgui.addFolder('Scene');
  var f2 = this.dgui.addFolder('Parameters');

  var fileControl = f1.add(mainExpFab, 'filename');
  var colorControl = f1.addColor(mainExpFab, 'color');
  var wireframeControl = f1.add(mainExpFab, 'wireframe');
  var operationControl = f2.add(mainExpFab, 'operation', ['None', 'Balloon', 'Translate', 'Rotate']);
  var dsControl = f2.add(mainExpFab, 'datastream').options(Object.keys(datastreams));
  f2.open();
  dsControl.onChange(function(value){
    that.dna.lookAt(this.property, value);
    activeDataStream = value; 
  });
  wireframeControl.onChange(function(value) {
    that.object.material.wireframe = value;
    render();
  });
  colorControl.onChange(function(value) {
    // cl(decimalToHexString(value));
    var nColor  =  decimalToHexString(value);
    if(nColor[0] != "#") nColor = "#" + nColor;
    that.object.material.color = new THREE.Color(nColor);
    render();
  });

  operationControl.onChange(function(value){
    //TODO change name
    if(value == 'Translate') value = 'Scale';
    activeOperation = value; 
    $(".op[name='"+ value +"x']").click();
    $(".op[name='"+ value +"y']").click();
    $(".op[name='"+ value +"z']").click();
    that.dna.lookAt(this.property, value);
  });

  this.mgui.GUIActivateListeners()
           .GUIStreams()
           .GUIOperations()
           .GUIFile(fileinfo)
           .GUIRegions(mainExpFab);

  animate();
  $("#nav").hide();

  mainExpFab.mgui.displayUIMessage("Model successfully loaded. ", fileinfo['name'], true);     
  this.dna = new DNA();
};