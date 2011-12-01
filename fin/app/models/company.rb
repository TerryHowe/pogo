class Company < ActiveRecord::Base
  DataTables = [ 'One', 'Two', 'Three', 'Four', 'Five' ]
  belongs_to :account
  belongs_to :exchange
  has_many :order
  validates_presence_of :ticker
  validates_presence_of :account_id
  validates_presence_of :exchange_id
  validates_presence_of :googid
  validates_presence_of :quantity
  validates_presence_of :table
  validates_presence_of :tactic

  def self.getDataTables()
    remaining = DataTables
    Company.find(:all).each { |company|
      remaining.delete(company.table)
    }
    return remaining
  end

  def self.getSelectable()
    return Company.find(:all).collect { |a| [a.ticker, a.id] }
  end


  def updater(price)
    case table
    when 'One'
      return One.updater(price)
    when 'Two'
      return Two.updater(price)
    when 'Three'
      return Three.updater(price)
    when 'Four'
      return Four.new(price)
    else
      raise 'Table not found'
    end
  end

  def to_s
      return 'Company: ' + ticker.to_s + ' quantity: $' + quantity.to_s + ' tactic: ' + tactic.to_s
  end
end
