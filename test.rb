require 'rubygems'
require 'hornetseye_ffmpeg'
require 'hornetseye_xorg'
include Hornetseye
input = AVInput.new 'http://mirrorblender.top-ix.org/movies/sintel-1024-surround.mp4'
w, h = (input.width * input.aspect_ratio).to_i, input.height
X11Display.show(w, h, :frame_rate => input.frame_rate) { input.read }