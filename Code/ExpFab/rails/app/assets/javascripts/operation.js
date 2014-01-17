
var currentOperation;

function Operation(container, name, icon, functionality){
	this.name = name;

	var ampSlider = $('<input/>')
		.addClass('op-slider')
		.attr('type', 'range')
		.attr('name', name)
		.attr('min', -5).attr('max', 5)
		.attr('step', 0.01)
		.change(function(){
			var val = $(this).val();
			if(currentOperation != $(this).attr('name') && !generateMode){
				activeDataStream.saveState();
			}
			currentOperation = $(this).attr('name');
			activeDataStream.applyToOperation(val);
		});
	
	functionality = function(){
		currentOperation = name;
		var ampSlider = $(".op-slider[name='"+ currentOperation +"']");
		if(ampSlider.length > 0) ampSlider.focus();
	}
	
	
	var node = GUINode(container, name, icon, functionality, true);
	container.append(node).css({
		width: '94%',
		padding: '3%',
		background:  'rgba(225, 225, 225, 0.8)'
	});
	node.parent().append(ampSlider);

}
