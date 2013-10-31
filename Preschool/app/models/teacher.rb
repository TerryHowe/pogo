class Teacher < ActiveRecord::Base
  belongs_to :room
  belongs_to :photo
  validates :name, :presence => true
  validates :room, :presence => true
  validates :photo, :presence => true

  def initialize(attributes = {})
    super
    self.photo = Photo.new()
  end

end
