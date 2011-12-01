class TacticFactory
  TACTIC_HILO = 'hilo'
  TACTIC_GAP = 'gap'

  def self.getTactics
    return [ TACTIC_HILO, TACTIC_GAP ]
  end

  def creator(company)
    case company.tactic
    when TACTIC_HILO
      return TacticHilo.new(company)
    when TACTIC_GAP
      return TacticHilo.new(company)
    else
      return TacticHilo.new(company)
    end
  end
end
