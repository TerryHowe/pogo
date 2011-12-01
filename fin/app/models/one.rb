class One < ActiveRecord::Base
  def self.updater(price)
    rec = One.new()
    rec.value = price.to_f
    return rec.save
  end
end
