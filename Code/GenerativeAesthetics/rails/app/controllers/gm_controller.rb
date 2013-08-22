require 'libsvm'
	
class GmController < ApplicationController
	@@holdout_param = 0.6
	@@path = 'public/diamond.asthc'
	@@model = ''
	def view
	end
	def post_sample
		db = []
		params['data'].each do |i, s|
	  	ns = Sample.new
	  	ns.fv = s['fv'].to_s
	  	ns.img = s['img'].to_s
	  	ns.rating = s['rating'].to_i
	  	ns.save
	  	db << ns
		end
		render :json => db
	end

	def log
		@samples = Sample.all#where('rating=?', 0)
		@samples.collect!{|s| s.fv = JSON.parse(s.fv); s}
		@samples = @samples.to_json.html_safe
	end

	def train_svm
		data = Sample.all
		n = data.length

		# #CROSS-VALIDATE 2-FOLD
		holdout = (@@holdout_param * n).to_i

		#train = data[0..holdout]
		#test = data[(holdout + 1)..-1]

		train = data.sample(holdout)


		train_labels = train.collect{|s| s.rating.to_i}
		train_examples = train.collect{|s| JSON.parse(s.fv)}
		
		
		# test_labels = test.collect{|x| x['rating']}
		# test_examples = test.collect{|x| x.features}
		
		problem = Libsvm::Problem.new
		parameter = Libsvm::SvmParameter.new

		parameter.cache_size = 1 # in megabytes
		parameter.eps = 0.001
		parameter.c = 20

		train_examples = train_examples.map {|ary| Libsvm::Node.features(ary) }
		problem.set_examples(train_labels, train_examples)
		model = Libsvm::Model.train(problem, parameter)
		model.save(@@path)
		@@model = model
		render :json => 'SUCCESS'
	end

	def test_svm
		#test
		
		data = Sample.all
		n = data.length
		debug = []
		#CROSS-VALIDATE 2-FOLD
		holdout = (@@holdout_param * n).to_i
		holdout = 0
		test = data.sample(n-holdout)
		test_labels = test.collect{|s| s.rating.to_i}
		test_examples = test.collect{|s| JSON.parse(s.fv)}


		#LOAD RECENT MODEL
		model = Libsvm::Model.load(@@path)

		truepositive = 0.0
		falsepositive = 0.0
		truenegative = 0.0
		falsenegative = 0.0
		tester = []
		test_examples = test_examples.map {|ary| Libsvm::Node.features(ary) }
		test_examples.each_with_index do |t, i|
			tester << t
			pred = model.predict(t)
			
			if test_labels[i] == 0 && pred == 1
				falsenegative = falsenegative + 1
			elsif test_labels[i] == 0 && pred == 0
				truepositive = truepositive + 1
			elsif test_labels[i] == 1 && pred == 0
				falsepositive = falsepositive + 1
			elsif test_labels[i] == 1 && pred == 1
				truenegative = truenegative + 1
			end
			debug << pred
			

		end
		render :json => {"precision" => truepositive / (truepositive + falsepositive),
						 "recall" => truepositive / (truepositive + falsenegative)}
	end
	def generate
		valid = []
		timeout = 0
		model = Libsvm::Model.load(@@path)
		while valid.length < 1 do
		#test_examples.each_with_index do |e, i|
			r = randfv
			rtest = Libsvm::Node.features(r) 
			pred = model.predict(rtest)
			if pred == 1.0 then valid << r end
			valid << r
			if timeout > 100 then break end
			timeout = timeout + 1
		end
		
		render :json => valid
	end
	def randfv
		n = JSON.parse(Sample.first.fv).length
		r = []
		for i in 1..n
			r << rand
		end
		return r
	end
	def update_sample
		db = []
		params['data'].each do |i, s|
	  		ns = Sample.find(s['id'])
	  		ns.rating = s['rating'].to_i
	  		ns.save
	  		db << ns
		end
		render :json => db
	end

end
