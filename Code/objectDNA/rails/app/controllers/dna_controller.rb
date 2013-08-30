class DnaController < ApplicationController
	def view
		if not params['t'] then params['t'] = 'all' end
		
		if params['t'] == 'all'
			@samples = Sample.last(30);#where('rating=?', 0)
		else 
			@samples = Sample.where('rating=?', params['t'].to_i)
		end

		#@samples = Sample.all#where('rating=?', 0)
		@samples.collect!{|s| s.fv = JSON.parse(s.fv); s}
		@samples = @samples.to_json.html_safe
	end
end
