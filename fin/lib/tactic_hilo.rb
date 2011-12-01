require 'hilo/init'

class TacticHilo < TacticBase
  attr_reader :high
  attr_reader :low

  def initialize(company)
    super(company)
    @initState = Init.new
  end

  def isTimeToBuy()
    puts "buy? getPrice()=" + getPrice().to_s
    puts "buy? @company.p1=" + @company.p1.to_s
    return getPrice() <= @company.p1
  end

  def buy()
    buyAt(@company.p1)
  end

  def isTimeToSell()
    puts "sell? getPrice()=" + getPrice().to_s
    puts "sell? @company.p2=" + @company.p2.to_s
    return getPrice() >= @company.p2
  end

  def sell()
    sellAt(@company.p2)
  end
end
