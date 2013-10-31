class Photo < ActiveRecord::Base
  has_attached_file :image, :styles => { :original => "100x100>" }, :default_style => :original, :default_url => "smiley.jpg"

  def create_default
    @smiley_file = File.join(Rails.root, 'public', 'system', 'images', 'smiley.jpg')
    @file_directory = File.join(Rails.root, 'public', 'system', 'images', self.id.to_s, 'original')
    @file_name = File.join(@file_directory,  '.jpg')
    FileUtils.mkdir_p(@file_directory)
    FileUtils.copy(@smiley_file, @file_name)
    self.image = File.new(@file_name)
  end

end
