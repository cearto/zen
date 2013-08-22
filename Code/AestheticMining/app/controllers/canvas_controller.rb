require 'libsvm'
class CanvasController < ApplicationController
	@@path = 'public/model.asthc'
	@@holdout_param = 0.6
	@@param_attr = {'cache_size' => 1, 
		'eps' => 0.001,
		'c' => 20
	}
	def project
		
	end

	
	def model
		@num = {'N' => Aesthetic.all.length, 
			'N_0' => Aesthetic.where('rating=?', 0).length,
			'N_1' => Aesthetic.where('rating=?', 1).length
		}
		@holdout = ('%i' % (@@holdout_param * @num['N'])).to_s + " (#{@@holdout_param})"
		@attr = @@param_attr
	end

	def index
	end
	def post_aesthetic
		input = params['aesthetic']
		a = Aesthetic.new
		a.rating  =  input["rating"].to_i;
    	a.rectH =  input["rect"]["H"].to_f;
        a.rectS =  input["rect"]["S"].to_f;
        a.rectL =  input["rect"]["L"].to_f;
        a.outerH = input["outer"]["H"].to_f;
        a.outerS = input["outer"]["S"].to_f;
        a.outerL = input["outer"]["L"].to_f;
        a.innerH = input["inner"]["H"].to_f;
        a.innerS = input["inner"]["S"].to_f;
        a.innerL = input["inner"]["L"].to_f;
        a.save
        render :json => a
	end
	def log
		@data = Aesthetic.where('rating = ?', params['t'].to_i).to_json.html_safe;
	end
	def predictions
	end
	def predict
		input = params['aesthetic']
		a = Aesthetic.new
		a.innerH = input["inner"]["H"].to_f;
        a.innerS = input["inner"]["S"].to_f;
        a.innerL = input["inner"]["L"].to_f;
        a.outerH = input["outer"]["H"].to_f;
        a.outerS = input["outer"]["S"].to_f;
        a.outerL = input["outer"]["L"].to_f;
        a.rectH =  input["rect"]["H"].to_f;
        a.rectS =  input["rect"]["S"].to_f;
        a.rectL =  input["rect"]["L"].to_f;

        model = Libsvm::Model.load(@@path)
        pred = model.predict(Libsvm::Node.features(a.features))
		render :json => [pred, a.features]       
	end
	def train_svm
		data = Aesthetic.all
		n = data.length

		#CROSS-VALIDATE 2-FOLD
		holdout = (@@holdout_param * n).to_i

		#train = data[0..holdout]
		#test = data[(holdout + 1)..-1]

		train = data.sample(holdout)
		test = data.sample(n-holdout)

		train_labels = train.collect{|x| x['rating']}
		train_examples = train.collect{|x| x.features}
		test_labels = test.collect{|x| x['rating']}
		test_examples = test.collect{|x| x.features}
		
		problem = Libsvm::Problem.new
		parameter = Libsvm::SvmParameter.new

		parameter.cache_size = 1 # in megabytes
		parameter.eps = 0.001
		parameter.c = 20

		train_examples = train_examples.map {|ary| Libsvm::Node.features(ary) }
		problem.set_examples(train_labels, train_examples)

		model = Libsvm::Model.train(problem, parameter)
		model.save(@@path)
		#cv = Libsvm::Model.cross_validation(problem, parameter, 2)
		render :json => 1

		
	end
	def test_model
		#test
		
		data = Aesthetic.all
		n = data.length
		debug = []
		#CROSS-VALIDATE 2-FOLD
		holdout = (0.6 * n).to_i

		test = data.sample(n-holdout)
		test_labels = test.collect{|x| x['rating']}
		test_examples = test.collect{|x| x.features}

		#LOAD RECENT MODEL
		model = Libsvm::Model.load(@@path)

		truepositive = 0.0
		falsepositive = 0.0
		truenegative = 0.0
		falsenegative = 0.0

		test_examples = test_examples.map {|ary| Libsvm::Node.features(ary) }
		test_examples.each_with_index do |t, i|
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
						 "recall" => truepositive / (truepositive + falsenegative)
						}
	end
end
