// aura.js

var gCurrentFile = null;
var gProcessor=null;

// Show all exceptions to the user:
OpenJsCad.AlertUserOfUncaughtExceptions();


function setupDragDrop()
{
  // Check for the various File API support.
  if (window.File && window.FileReader && window.FileList) {
    // Great success! All the File APIs are supported.
  } else {
    throw new Error("Error: Your browser does not fully support the HTML File API");
  }
  var dropZone = document.getElementById('filedropzone');
  dropZone.addEventListener('dragover', function(evt) {
    evt.stopPropagation();
    evt.preventDefault();
    evt.dataTransfer.dropEffect = 'copy';
  }, false);
  dropZone.addEventListener('drop', handleFileSelect, false);
}

function handleFileSelect(evt)
{
  evt.stopPropagation();
  evt.preventDefault();
  
  if(!evt.dataTransfer) throw new Error("Not a datatransfer (1)");
  if(!evt.dataTransfer.files) throw new Error("Not a datatransfer (2)");
  if(evt.dataTransfer.files.length != 1)
  {
    throw new Error("Please drop a single .jscad file");
  }
  var file = evt.dataTransfer.files[0];
  if(!file.name.match(/\.jscad$/i))
  {
    throw new Error("Please drop a file with .jscad extension");
  }
  if(file.size == 0)
  {
    throw new Error("You have dropped an empty file");
  }              
  gCurrentFile = file;
  gPreviousModificationTime = "";
  fileChanged();
}

function fileChanged()
{
  var dropZone = document.getElementById('filedropzone');
  if(gCurrentFile)
  {
    var txt = "Current file: "+gCurrentFile.name;
    document.getElementById("currentfile").innerHTML = txt;
    document.getElementById("filedropzone_filled").style.display = "block";
    document.getElementById("filedropzone_empty").style.display = "none";
  }
  else
  {
    document.getElementById("filedropzone_filled").style.display = "none";
    document.getElementById("filedropzone_empty").style.display = "block";
  }
  parseFile(false,false);
}

var autoReloadTimer = null;
function toggleAutoReload() {
	if (document.getElementById("autoreload").checked) {
		autoReloadTimer = setInterval(function(){
		  parseFile(false,true);
    }, 1000);
	} else {
		if (autoReloadTimer !== null) {
			clearInterval(autoReloadTimer);
			autoReloadTimer = null;
		}
	}
}

var previousScript = null;
function parseFile(debugging, onlyifchanged)
{
  if(gCurrentFile)
  {
    var reader = new FileReader();
    reader.onload = function(evt) {
      var txt = evt.target.result;
    };
    reader.onloadend = function(evt) {
      if (evt.target.readyState == FileReader.DONE)
      {
        var jscadscript = evt.target.result;
        if(jscadscript == "")
        {
          if(document.location.toString().match(/^file\:\//i))
          {
            throw new Error("Could not read file. You are using a local copy of OpenJsCad; if you are using Chrome, you need to launch it with the following command line option:\n\n--allow-file-access-from-files\n\notherwise the browser will not have access to uploaded files due to security restrictions."); 
          }
          else
          {
            throw new Error("Could not read file.");
          }            
        }
        else
        {         
          if(gProcessor && ((!onlyifchanged) || (previousScript !== jscadscript)))
          {
            var filename = gCurrentFile.name;
            filename = filename.replace(/^.*\/([^\/]*)$/, "$1");
            gProcessor.setDebugging(debugging); 
            gProcessor.setJsCad(jscadscript, filename);
						previousScript = jscadscript;
          }
        }
      }
      else
      {
        throw new Error("Failed to read file");
        if(gProcessor) gProcessor.clearViewer();
				previousScript = null;
      }
    };
    reader.readAsText(gCurrentFile, "UTF-8");
  }
}
