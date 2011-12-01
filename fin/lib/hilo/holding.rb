require 'state_base'

class Holding < StateBase
  def process(tactic)
    if tactic.xOpen() == false
      return ExchangeClosed.new()
    end
    if tactic.isTimeToSell()
      if (tactic.sell())
        return Pending.new()
      end
    end
    return Holding.new()
  end
end
