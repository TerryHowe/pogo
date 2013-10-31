# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rake db:seed (or created alongside the db with db:setup).
#
# Examples:
#
#   cities = City.create([{ :name => 'Chicago' }, { :name => 'Copenhagen' }])
#   Mayor.create(:name => 'Emanuel', :city => cities.first)

# [:admin, :user].each { |r| Role.create(:name => r) }
# rooms = Room.create(


Room.create([{ :name => 'Bear Cub' }, { :name => 'Butterfly' }, { :name => 'Caterpillar' }, { :name => 'Toddler' }, { :name => 'Infant' } ])
Photo.new().save()
