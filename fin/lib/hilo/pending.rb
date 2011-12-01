require 'state_base'

class Pending < StateBase
  def process(tactic)
    if tactic.xOpen() == false
      return ExchangeClosed.new()
    end
    if (tactic.pending_order.nil?)
      tactic.pending_order = Order.getPendingOrder(tactic.account)
      if (tactic.pending_order.nil?)
        return Init.new()
      end
    end
    if tactic.broker.check(tactic.pending_order)
      if (tactic.pending_order.buying())
        return Holding.new()
      else
        return Shopping.new()
      end
    end 
    return Pending.new()
  end
end
