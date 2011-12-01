class Three < ActiveRecord::Base
  def self.updater(price)
    rec = Three.new()
    rec.value = price.to_f
    return rec.save
  end
end
