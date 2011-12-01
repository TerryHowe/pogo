class TacticBase
  attr_accessor :company
  attr_reader :state
  attr_reader :initState
  attr_reader :broker
  attr_accessor :pending_order

  def initialize(company)
    @company = company
    @state = StateBase.new
    @initState = StateBase.new
    @broker = BrokerFactory.creator(company.account)
  end

  def process()
    previous = @state
    puts @state.class.name + ': ' + @company.to_s
    @state = @state.process(self)
    if (previous != @state)
       puts 'Going to ' + @state.class.name
    end
    return true
  end

  def account()
    return @company.account
  end

  def xOpen()
    return @company.exchange.isOpen();
  end

  def getPrice()
    return @broker.getPrice(@company.ticker);
  end

  def buyAt(price)
    return @broker.buy(@company.ticker, @company.quantity, price)
  end

  def sellAt(price)
    return @broker.sell(@company.ticker, @company.quantity, price)
  end
end
