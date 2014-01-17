function Mutator(ef){
	this.history = [];
	this.headers = []; 
	this.marker = 0;
	this.ef = ef;

	this.extract = function(){
		var fv = [];
		for(var i in this.ef.regions)
			fv[i] = clone(this.ef.regions[i].weights);
		return fv;
	}

	this.generate = function(){
		generateMode = true;
		$.each($('.region'), function(i,v){
			$('.region').removeClass('selected');
			$(v).click();
			$($('.op-slider')[0]).val(Math.random() * 2 - 1).change();
			$($('.op-slider')[1]).val(Math.random() * 2 - 1).change();
			$($('.op-slider')[2]).val(Math.random() * 2 - 1).change();
		});
		generateMode = false;
	}
	this.load = function(){
		//for(var i in operations) this.headers.push(operations[i].name);
		//for(var i in mainExpFab.regions) this.history.push([]);
		//this.log();
	}
}

Mutator.prototype.undo = function(){
	if(this.marker == 0){
		mainExpFab.mgui.displayUIMessage("Nothing to undo.", "", false);     
		return; 
	}

	var now = this.history[ this.marker - 1];

	for(var regid in now.regions){
		var region = mainExpFab.regions[now.regions[regid]];
		//region.updateGeom();
		now.transform.transform(region, true);

	}
	now.transform.ds.saveState();

	this.marker--;
}
Mutator.prototype.log = function(rs){
	var t = currentTransform;
	if( t == null) return;
	
	this.history[this.marker] = {"regions": rs, "transform": t};
	cl(this.history);
	this.marker++;
}
