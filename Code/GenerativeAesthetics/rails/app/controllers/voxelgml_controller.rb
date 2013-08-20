class VoxelgmlController < ApplicationController

  def view
  end
  def voxel
  end
  def save_gml
  	filename = "public/json/" + params['filename']
  	gml = params['gml']
  	File.open(filename, "w+") do |f|
	  f.write(gml)
	end
  	render :json => params
  end
  def saved_files
    files = Dir["public/resources/*.jscad"]
    files.collect!{|f| f.split('/')[1..-1].join('/')}
  	render :json => files

  end 
  def load_file
  	fn = params['filename']
  	contents = File.read(fn)
  	render :json => JSON.parse(contents)
  end
end
