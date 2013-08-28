class CreateAesthetics < ActiveRecord::Migration
  def change
        create_table :aesthetics do |t|
            t.column :rating,       :int
            t.column :rectH,       :float
            t.column :rectS,       :float
            t.column :rectL,       :float
            t.column :outerH,       :float
            t.column :outerS,       :float
            t.column :outerL,       :float
            t.column :innerH,       :float
            t.column :innerS,       :float
            t.column :innerL,       :float
            t.timestamps
        end
    end
end
