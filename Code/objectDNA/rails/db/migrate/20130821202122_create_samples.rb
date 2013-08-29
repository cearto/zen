class CreateSamples < ActiveRecord::Migration
  def change
    create_table :samples do |t|
    	t.column :rating, :int
    	t.column :fv, :string
    	t.column :img, :string
    end
  end
end
