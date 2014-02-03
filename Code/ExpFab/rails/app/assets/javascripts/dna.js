var generateMode = false;

function DNA(){
	this.genes = [];
	var regids = []; for(var i in mainExpFab.regions){ this.genes.push(mainExpFab.regions[i].gene); regids.push(i);}

	this.gui = new dat.GUI();
	this.ROI = 0;
	this.operation = "Balloon";
	this.axis = "Red";
	this.datastream = "Uniform";
	this.min = 0;
	this.value = 0;
	this.max = 0;
	this.lock = true;
	
	this.activeGene = this.genes[this.ROI][this.operation][toAxisID(this.axis)];
	this.load(regids);
}
DNA.prototype.load = function(regids){
	var self = this;

	var ROIController = this.gui.add(this, 'ROI', regids);
	var opController = this.gui.add(this, 'operation', operations);
	var axisController = this.gui.add(this, 'axis', ["Red", "Green", "Blue"]);
	var datastreamController = this.gui.add(this, 'datastream').options(Object.keys(datastreams));
	var minController = this.gui.add(this, 'min', -5.0, 5.0).step(0.001);
	var valueController = this.gui.add(this, 'value', -5.0, 5.0).step(0.001);
	var maxController = this.gui.add(this, 'max', -5.0, 5.0).step(0.001);
	var lockController = this.gui.add(this, 'lock');
	

	datastreamController.getOptionID = function(name){
		var nodes = this.domElement.childNodes[0].childNodes;
		for(var i in nodes) if(nodes[i].innerHTML == name) return i;
	}
	datastreamController.setValue2 = function(name){
		var id = datastreamController.getOptionID(name);
		this.domElement.childNodes[0].selectedIndex = id;
	}

	// EVENT HANDLERS

	var controllers = self.gui.__controllers;
	for(var i in controllers){	
		controllers[i].onChange(function(value){
			if(self.updateLocked) return;
			self.activeGene = self.genes[self.ROI][self.operation][toAxisID(self.axis)];
			self.activeGene[this.property] = value;
			self.update(this.property);
			if(["min", "max", "value"].indexOf(this.property) != -1){
				currentTransform = self.activeGene.transform(this.property);
				var r = mainExpFab.regions[self.activeGene.ROI];
				currentTransform.transform(r, false);
			}
			if(["min", "max"].indexOf(this.property) != -1){
				self.activeGene.lock = false;
				self.update('lock');
			}
		});
		controllers[i].onFinishChange(function(value) {
		  if(["min", "max", "value"].indexOf(this.property) != -1){
		  		cl("Saving state");

				var r = mainExpFab.regions[self.activeGene.ROI];
				activeRegions = [self.activeGene.ROI];
				activeDataStream = self.activeGene.datastream;
				datastreams[activeDataStream].saveState();
			    mainExpFab.mutator.log(activeRegions);
			    
			    currentTransform = null;
			}
		});
	}
	this.updateLocked = false;
}
DNA.prototype.update = function(prop){
	var controllers = this.gui.__controllers;
	this.updateLocked = true;
	for(var i in controllers)
		if(controllers[i].property != prop)
			controllers[i].setValue(this.activeGene[controllers[i].property]);
	this.updateLocked = false;
}
DNA.prototype.lookAt = function(prop, value){
	this.activeGene = this.genes[this.ROI][this.operation][toAxisID(this.axis)];

	this[prop] = value;
	this.activeGene[prop] = value;
	this.update("");
}
DNA.prototype.extract = function(){
	active = [];
	for(var regid in this.genes)
		for(var op in this.genes[regid])
			for(var ax in this.genes[regid][op])
				if(!(this.genes[regid][op][ax].lock))
					active.push(this.genes[regid][op][ax]);
	return active;
}
DNA.prototype.generate = function(){
	var unlockedGenes = this.extract();
	for(var i in unlockedGenes){
		unlockedGenes[i].generate();
	}
}
	function Gene( regid, op, tmin, tmax, ds, axis, value){
		this.ROI = regid;
		this.operation = op;
		this.min = tmin;
		this.max = tmax;
		this.datastream = ds;
		this.value = value;
		this.axis = axis;
		this.lock = true;
	}
	Gene.prototype.transform = function(type){
		return new Transform(this.operation, toAxisID(this.axis), this.datastream, this[type], false);
	}
	Gene.prototype.generate = function(){
		var range = this.max - this.min;
		var gen = Math.random() * range;
		var value = this.min + gen;
		
		this.value = value;
		currentTransform = this.transform("value");
		var r = mainExpFab.regions[this.ROI];
		currentTransform.transform(r, false); 
	}
