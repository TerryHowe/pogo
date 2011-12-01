class Account < ActiveRecord::Base
  has_many :company
  has_many :order

  def self.getSelectable()
    return Account.find(:all).collect { |a| [a.broker, a.id] }
  end

  def self.TDAMERITRADE
    return 'TDAmeritrade'
  end
  def self.VSE
    return 'VSE'
  end
  def self.STUB
    return 'Stub'
  end

  def self.brokers
    return [ Account.TDAMERITRADE, Account.VSE, Account.STUB ]
  end
end
