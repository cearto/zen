var generateMode = false;
function DNA(){
	this.genes = [];
	var regids = [];
	for(var i in mainExpFab.regions){
		this.genes.push(mainExpFab.regions[i].gene);
		regids.push(i);
	}
	this.ROI = 0;
	this.operation = "Balloon";
	this.axis = "Red";
	this.datastream = "Uniform";
	this.min = 0;
	this.max = 0;
	this.lock = false;
	this.gui = new dat.GUI();

	var self = this;
	var ROIController = this.gui.add(this, 'ROI', regids);
	var opController = this.gui.add(this, 'operation', ops);
	var axisController = this.gui.add(this, 'axis', ["Red", "Green", "Blue"]);
	var datastreamController = this.gui.add(this, 'datastream').options(Object.keys(datastreams));

	datastreamController.getOptionID = function(name){
		var nodes = this.domElement.childNodes[0].childNodes;
		for(var i in nodes) if(nodes[i].innerHTML == name) return i;
	}
	datastreamController.setValue2 = function(name){
		var id = datastreamController.getOptionID(name);
		this.domElement.childNodes[0].selectedIndex = id;
	}
	
	var minController = this.gui.add(this, 'min');
	var maxController = this.gui.add(this, 'max');
	var lockController = this.gui.add(this, 'lock');

	
	ROIController.onChange(function(value){
		var axisid = toAxisID(self.axis);
		minController.setValue(self.genes[value][self.operation][axisid].min);
		maxController.setValue(self.genes[value][self.operation][axisid].max);
		lockController.setValue(self.genes[value][self.operation][axisid].lock);
		datastreamController.setValue2(self.genes[value][self.operation][axisid].ds.name);
	});

	axisController.onChange(function(value){
		var axisid = toAxisID(value);
		minController.setValue(self.genes[self.ROI][self.operation][axisid].min);
		maxController.setValue(self.genes[self.ROI][self.operation][axisid].max);
		lockController.setValue(self.genes[self.ROI][self.operation][axisid].lock);
		datastreamController.setValue2(self.genes[self.ROI][self.operation][axisid].ds.name);
	});
	opController.onChange (function(value){
		var axisid = toAxisID(self.axis);
		console.log(value);
		minController.setValue(self.genes[self.ROI][value][axisid].min);
		maxController.setValue(self.genes[self.ROI][value][axisid].max);
		lockController.setValue(self.genes[self.ROI][value][axisid].lock);
		datastreamController.setValue2(self.genes[self.ROI][value][axisid].ds.name);
	});
	datastreamController.onChange(function(value){
		var axisid = toAxisID(self.axis);
		self.genes[self.ROI][self.operation][axisid].ds = datastreams[value];
	});
	
	lockController.onChange(function(value){
		var axisid = toAxisID(self.axis);
		self.genes[self.ROI][self.operation][axisid].lock = value ;
	});
	this.valueSetter = function(hash){
		for(var key in hash)
			if(key == 'ROI')
				ROIController.setValue(hash[key]);
			else if (key == 'axis')
				axisController.setValue(hash[key]);
			if(key == 'ds')
				datastreamController.setValue(hash[key]);
			else if (key == 'min')
				minController.setValue(hash[key]);
			if(key == 'max')
				maxController.setValue(hash[key]);
			else if (key == 'lock')
				lockController.setValue(hash[key]);
	}
}
DNA.prototype.extract = function(region, axis){
	var op = mainExpFab.operation;	
}

function toAxisID(color){
	if("Red" == color) return 0;
	if("Green" == color) return 1;
	if("Blue" == color) return 2;
}
function toAxisColor(id){
	if(id == 0) return "Red";
	if(id == 1) return "Green";
	if(id == 2) return "Blue";
}
function Gene( op, tmin, tmax, ds, axis){
	this.op = op;
	this.min = tmin;
	this.max = tmax;
	this.ds = ds;
	this.axis = axis;
	this.lock = false;
}
