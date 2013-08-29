function dDNA(genes){
	if(typeof(genes) == 'undefined') genes = []
	// A sequence of genes paired with their name
	this.genes = genes;
	this.survival = true;
	this.infected = false;
	this.cancerous = false;
	
	this.reproduce = function(partner, weight){
		if(typeof(weight) == 'undefined') weight = 0.5;
		var childDNA = this.genes.DNA.add(partner.genes.multiply(weight));
		if(infected) childDNA.add(this.virus);
		childDNA = childDNA.mutate(0.1)

		return new dDNA(childDNA);
	}

	this.mutate = function(exposure){
		jitter = Vector.random(this.dimensions).multiply(0.1);
		return this.genes.DNA.dot(jitter);
	}

	this.kill = function(){
		this.survival = 0;
	}
	this.cancer = function(virus){
		this.cancerous = true;
		this.genes.add(virus);
	}
	this.infect = function(virus){
		this.infected = true;
		this.virus = virus;
	}
}


dDNA.prototype.extractVirus = function(sick, healthy){
	return sick.genes.subtract(healthy.genes);
}
