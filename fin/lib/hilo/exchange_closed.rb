require 'state_base'

class ExchangeClosed < StateBase
  def process(tactic)
    if tactic.xOpen()
      return ExchangeClosed.new()
    end
    return ExchangeOpen.new().process(tactic)
  end
end
