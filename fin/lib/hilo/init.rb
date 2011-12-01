require 'state_base'
require 'hilo/exchange_closed'
require 'hilo/holding'
require 'hilo/pending'
require 'hilo/shopping'

class Init < StateBase
  def process(tactic)
    if tactic.xOpen() == false
      return ExchangeClosed.new()
    end

    pendingOrder = Order.getPendingOrder(tactic.account)
    if pendingOrder.nil?
      lastOrder = Order.getLastOrder(tactic.account)
      unless lastOrder.nil?
        if (lastOrder.buying() == true)
          return Holding.new.process(tactic)
        end
      end
      return Shopping.new.process(tactic)
    end
    return Pending.new().process(tactic)
  end
end
