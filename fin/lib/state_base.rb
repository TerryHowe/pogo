class StateBase
  def process(tactic)
    if (tactic.record.exchange.isOpen())
      return @initState;
    end
    return self
  end
end
