class Exchange < ActiveRecord::Base
  has_many :companies
  validates_presence_of :name
  validates_presence_of :openhour
  validates_presence_of :openminute
  validates_presence_of :closehour
  validates_presence_of :closeminute
  validates_presence_of :timezone

  def self.getSelectable()
    return Exchange.find(:all).collect { |e| [e.name, e.id] }
  end

  def self.timezones
    ra = Array.new
    ActiveSupport::TimeZone.us_zones().each { |zone| ra << zone.name }
    return ra
  end

  def self.hours
    return [ '1', '2', '3', '4', '5', '6',
             '7', '8', '9', '10', '11', '12',
             '13', '14', '15', '16', '17', '18',
             '19', '20', '21', '22', '23', '24']
  end

  def self.minutes
    return [ '00', '30' ]
  end

  def opentime
    return openhour + ':' + openminute
  end

  def closetime
    return closehour + ':' + closeminute
  end

  def isOpen
    zo = ActiveSupport::TimeZone.new(timezone);
    now = zo.now()
    opening = zo.parse(opentime)
    closing = zo.parse(closetime)
    if (((opening <=> now) == -1) && ((now <=> closing) == -1))
        return true;
    end
    return false
  end

  def openit
    self.openhour = "00"
    self.openminute = "00"
    self.closehour = "23"
    self.closeminute = "59"
  end

  def closeit
    self.openhour = "01"
    self.openminute = "00"
    self.closehour = "00"
    self.closeminute = "00"
  end
end
