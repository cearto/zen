function ExpFab(obj, regions, pca, filepath){
  var filesplit = filepath.split('/');
  this.filename = filesplit[filesplit.length-1].split('.stl')[0]; 
  //scene variables
	this.object = obj;
  this.color = defaultMaterialSettings.color;
  this.wireframe = defaultMaterialSettings.wireframe;
  this.shininess = defaultMaterialSettings.shininess;
	
  // deformation variables
  this.operation = 'None';
	this.original = obj.geometry.clone().vertices;
  this.current = obj.geometry.clone().vertices;
	
	this.regions = [];
	for(var i in regions) this.regions.push(new Region(this, i, regions[i], pca[i]));
	
	this.id = scene.userData.objects.length;
  this.mgui = new MeshGUI();
  this.dgui = new dat.GUI();
}

ExpFab.prototype.load = function(fileinfo) {
  var that = this;
  
  var fileControl = this.dgui.add(mainExpFab, 'filename');
  var wireframeControl = this.dgui.add(mainExpFab, 'wireframe');
	var colorControl = this.dgui.addColor(mainExpFab, 'color');
  var operationControl = this.dgui.add(mainExpFab, 'operation', ['None', 'Balloon', 'Translate']);


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
   
  });

  GUIActivateListeners();
  GUIStreams();
  GUIOperations();
  GUIFile(fileinfo);
  GUIRegions(mainExpFab);
  loadDataStreams($('#streams'));
  animate();
  $("#nav").hide();

  mainExpFab.mgui.displayUIMessage("Model successfully loaded. ", fileinfo['name'], true);     
};