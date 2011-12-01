class Order < ActiveRecord::Base
  belongs_to :account
  belongs_to :company

  def self.TypeBuy
    return 'Buy'
  end
  def self.TypeSell
    return 'Sell'
  end
  def self.TypeSellShort
    return 'SellShort'
  end
  def self.TypeBuyCover
    return 'BuyCover'
  end

  def buying
    if ((xaction == Order.TypeBuy) || (xaction == Order.TypeBuyCover))
      return true
    end
    return false
  end

  def self.DurationDay
    return 0
  end
  def self.DurationGTC
    return 1
  end

  def self.StatePending
    return 0
  end
  def self.StateCancelPending
    return 1
  end
  def self.StateCancelled
    return 2
  end
  def self.StateComplete
    return 3
  end

  def self.getPendingOrder(acct)
    return find(:first, :conditions => ['account_id = ? AND (ostate = ? OR ostate = ?)', acct.id, Order.StatePending, Order.StateCancelPending])
  end

  def self.getLastOrder(acct)
    return find(:first, :conditions => { :account_id => acct.id, :ostate => Order.StateComplete })
  end
end
