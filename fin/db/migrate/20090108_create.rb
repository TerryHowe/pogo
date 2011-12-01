class Create < ActiveRecord::Migration
  def self.up
    create_table :exchanges do |t|
      t.string :name
      t.string :openhour
      t.string :openminute
      t.string :closehour
      t.string :closeminute
      t.string :timezone

      t.timestamps
    end
    create_table :companies do |t|
      t.string :ticker
      t.references :account
      t.references :exchange
      t.string :googid
      t.float :quantity
      t.string :table
      t.string :tactic
      t.float :p1
      t.float :p2
      t.float :p3
      t.float :p4
      t.float :p5

      t.timestamps
    end
    create_table :accounts do |t|
      t.string :broker
      t.string :user
      t.string :password
      t.string :name
      t.string :subaccount

      t.timestamps
    end
    create_table :orders do |t|
      t.string :number
      t.references :account
      t.datetime :date
      t.string :xaction
      t.references :company
      t.integer :quantity
      t.integer :ostate

      t.timestamps
    end
    create_table :ones do |t|
      t.float :value
      t.timestamps
    end
    create_table :twos do |t|
      t.float :value
      t.timestamps
    end
    create_table :threes do |t|
      t.float :value
      t.timestamps
    end
    create_table :fours do |t|
      t.float :value
      t.timestamps
    end
  end

  def self.down
    drop_table :exchanges
    drop_table :companies
    drop_table :ones
    drop_table :twos
    drop_table :threes
    drop_table :fours
    drop_table :accounts
    drop_table :orders
  end
end
