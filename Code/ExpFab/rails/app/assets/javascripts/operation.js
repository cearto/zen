
var currentOperation;

function Operation(container, name, icon, functionality){
	this.name = name;
	activeDataStream = datastreams[0];
	functionality = function(){
		var data = scene.userData.objects[0];
		activeDataStream = datastreams[0];
		currentOperation = name;

		var ampSlider = $(".op-slider[name='"+ currentOperation +"']");
		if(ampSlider.length > 0) ampSlider.focus();
		else{
			ampSlider = $('<input/>').addClass('op-slider').attr('type', 'range').attr('name', currentOperation).attr('min', -5).attr('max', 5).attr('step', 0.01)
			.change(function(){
				var val = $(this).val();
				if(currentOperation != $(this).attr('name') && !generateMode){
					activeDataStream.saveState();
				}
				currentOperation = $(this).attr('name');
				
				activeDataStream.applyToOperation(val);
			});
			$(this).parent().append(ampSlider);
		}
	}
	
	
	var node = GUINode(container, name, icon, functionality, true);
	container.append(node).css({
		width: '94%',
		padding: '3%',
		background:  'rgba(225, 225, 225, 0.8)'
	});

}
