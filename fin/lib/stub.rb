class Stub < Broker
public
  attr_accessor :buy_return
  attr_accessor :sell_return
  attr_accessor :check_return
  attr_accessor :cancel_return
  attr_accessor :getPrice_return

  def initialize(user, password, name, subaccount)
    super(AppConfig.tdameritrade, user, password, name, subaccount)
    @buy_return = true
    @sell_return = true
    @check_return = 'Complete'
    @cancel_return = true
    @getPrice_return = 99
  end

  def open()
    return true
  end

  def login()
    return true
  end

  def buy(symbol, quantity, price, duration = DurationGTC)
    return @buy_return
  end

  def sell(symbol, quantity, price, duration = DurationGTC)
    return @sell_return
  end

  def check(order)
    return @check_return
  end

  def cancel(order)
    return @cancel_return
  end

  def getPrice(symbol)
    return @getPrice_return
  end
end
