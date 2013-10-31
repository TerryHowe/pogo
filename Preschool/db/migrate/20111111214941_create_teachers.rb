class CreateTeachers < ActiveRecord::Migration
  def change
    create_table :teachers do |t|
      t.string :name
      t.references :room
      t.references :photo

      t.timestamps
    end
    add_index :teachers, :room_id
    add_index :teachers, :photo_id
  end
end
