require 'state_base'

class Shopping < StateBase
  def process(tactic)
    if tactic.xOpen() == false
      return ExchangeClosed.new()
    end
    if tactic.isTimeToBuy()
      if (tactic.buy())
        return Pending.new()
      end 
    end 
    return Shopping.new()
  end
end
