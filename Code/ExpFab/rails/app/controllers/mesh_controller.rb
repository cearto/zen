require 'rubygems' 
require 'csv'
require 'json'

class MeshController < ApplicationController
  
  #retrieve pca file for object file_num
  def pca file_num
    begin
      f = File.open("public/stl/pca/#{file_num}.pca")
      data = JSON.parse(f.read())['data'];
      size = data["region"]["_ArraySize_"];
      pc = data["region"]["_ArrayData_"].each_slice(size[0] * size[1]).to_a
      pc.collect!{|p| p.each_slice(3).to_a}
      rescue then return [[0, 1, 0],[1, 0, 0],[0, 0, 1]];
      ensure  f.close unless f.nil?
    end
    
    return pc
  end

  def segment
    file_num = params['n'] ? params['n'].to_i : 0;
  	directory = 'icons/operations';
  	@operations =  Dir.entries('public/' + directory)[3..-1].collect!{|c| {'name'=> c.split('.')[0].humanize, 'img'=> '/' + directory +'/'+ c} }
	  @operations = @operations.to_json.html_safe;
    @pca = pca(file_num).to_json.html_safe;
    @seg = seg("#{file_num}/#{file_num}_0.seg").to_json.html_safe;
  end

  def weather
    data = [];
    CSV.foreach("public/data/232761.csv", :headers => true, :header_converters => :symbol) do |row|
      data << row.to_hash;
    end
    data = data.group_by { |d| d[:station]}
    data = data["GHCND:USC00043714"].collect{|d| d[:mlytavgnormal].to_f / 10.0}
    render :json => data
    # render :json => CSV.read('public/data/232761.csv').parse().to_json;
  end
  def operations
    directory = 'icons/operations';
  	render :json => Dir.entries('public/' + directory)[3..-1].collect!{|c| {'name'=> c.humanize, 'img'=> '/' + directory +'/'+ c} }
  end

  def seg filename
    seg = {}
    index = 0
    File.open("public/stl/seg/#{filename}", "r").each_line do |line|
      region = line.strip
      if not seg[region]
        seg[region] = []
      end
      seg[region] << index
      index = index + 1
    end
    return seg.sort.collect{|s| s[1]}
  end
  def save
    f = File.open("public/save#{params['filename']}", "w");
    f.write(params['mesh']);
    render :json => "/save/#{params['filename']}"
  end
end
