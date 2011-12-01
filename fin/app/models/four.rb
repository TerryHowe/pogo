class Four < ActiveRecord::Base
  def self.updater(price)
    rec = Four.new()
    rec.value = price.to_f
    return rec.save
  end
end
