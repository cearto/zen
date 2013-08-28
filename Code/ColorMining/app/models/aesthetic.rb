class Aesthetic < ActiveRecord::Base
  @@feature_attr =  [:innerH, :innerS, :innerL, :outerH, :outerS, :outerL, :rectH, :rectS, :rectL]
  def features
  	attr = @@feature_attr.collect{|x| self[x]}
  end
end
