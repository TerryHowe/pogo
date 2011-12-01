class BrokerFactory
  def self.creator(record)
    case record.broker
    when Account.TDAMERITRADE
      return Tdameritrade.new(record.user, record.password, record.name, record.subaccount)
    when Account.VSE
      return Vse.new(record.user, record.password, record.name, record.subaccount)
    when Account.STUB
      return Stub.new(record.user, record.password, record.name, record.subaccount)
    else
      return Tdameritrade.new(record.user, record.password, record.name, record.subaccount)
    end
  end
end
