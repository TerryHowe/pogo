class Two < ActiveRecord::Base
  def self.updater(price)
    rec = Two.new()
    rec.value = price.to_f
    return rec.save
  end
end
