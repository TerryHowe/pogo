require 'test_helper'
require 'hilo/exchange_closed'
require 'hilo/holding'
require 'hilo/init'
require 'hilo/pending'
require 'hilo/shopping'

class HiloTest < ActiveSupport::TestCase
  def setup
    @account = Account.new()
    @account.broker = Account.STUB
    @account.save
    @company = Company.new
    @company.ticker = 'TST'
    @company.account = @account
    @company.exchange = Exchange.find(:first)
    @company.exchange.openit()
    @company.googid = 939
    @company.quantity = 400
    @company.table = 'One'
    @company.tactic = 'hilo'
    @company.p1 = 50
    @company.p2 = 100
    @company.p3 = 0
    @company.p4 = 0
    @company.p5 = 0
    @company.save

    @tactic = TacticHilo.new(@company)
  end

  def create_order
    ord = Order.new
    ord.account = @company.account
    ord.date = Date.new
    ord.xaction = Order.TypeBuy
    ord.company = @company
    ord.quantity = 400
    ord.ostate = Order.StatePending
    return ord
  end

  def test_init
    @company.exchange.openit()

    s = Init.new()
    assert_equal Shopping.new.class.name, s.process(@tactic).class.name

    ord = create_order()
    @tactic.pending_order = ord
    ord.save
    assert_equal Holding.new.class.name, s.process(@tactic).class.name

    ord.ostate = Order.StateCancelPending
    ord.save
    assert_equal Pending.new.class.name, s.process(@tactic).class.name

    ord.ostate = Order.StateCancelled
    ord.save
    assert_equal Shopping.new.class.name, s.process(@tactic).class.name

    ord.ostate = Order.StateComplete
    ord.save
    assert_equal Holding.new.class.name, s.process(@tactic).class.name

    ord.xaction = Order.TypeSell
    ord.save
    assert_equal Shopping.new.class.name, s.process(@tactic).class.name

    ord.xaction = Order.TypeSellShort
    ord.save
    assert_equal Shopping.new.class.name, s.process(@tactic).class.name

    ord.xaction = Order.TypeBuyCover
    ord.save
    assert_equal Holding.new.class.name, s.process(@tactic).class.name

    @company.exchange.closeit()
    assert_equal ExchangeClosed.new.class.name, s.process(@tactic).class.name
  end

  def test_holding
    @company.exchange.openit()

    s = Holding.new()
    assert_equal Holding.new.class.name, s.process(@tactic).class.name

    @tactic.broker.getPrice_return = 100
    assert_equal Pending.new.class.name, s.process(@tactic).class.name

    @tactic.broker.sell_return = false
    assert_equal Holding.new.class.name, s.process(@tactic).class.name
    @tactic.broker.sell_return = true

    @company.exchange.closeit()
    assert_equal ExchangeClosed.new.class.name, s.process(@tactic).class.name
  end

  def test_shopping
    pending = Order.getPendingOrder(@tactic.account)
    unless (pending.nil?)
      pending.ostate = Order.StateComplete
      pending.save()
    end
    assert_nil Order.getPendingOrder(@tactic.account)
    @company.exchange.openit()

    s = Shopping.new()
    @tactic.broker.getPrice_return = 51
    assert_equal Shopping.new.class.name, s.process(@tactic).class.name
    assert_nil Order.getPendingOrder(@tactic.account)

    @tactic.broker.getPrice_return = 50
    assert_equal Pending.new.class.name, s.process(@tactic).class.name
    assert_not_nil Order.getPendingOrder(@tactic.account)
    pending = Order.getPendingOrder(@tactic.account)
    pending.ostate = Order.StateComplete
    pending.save()

    @tactic.broker.buy_return = false
    assert_nil Order.getPendingOrder(@tactic.account)
    assert_equal Shopping.new.class.name, s.process(@tactic).class.name
    assert_nil Order.getPendingOrder(@tactic.account)
    @tactic.broker.buy_return = true

    @company.exchange.closeit()
    assert_equal ExchangeClosed.new.class.name, s.process(@tactic).class.name
  end

  def test_pending
    @company.exchange.openit()

    s = Pending.new()
    @tactic.broker.check_return = true
    @tactic.pending_order = create_order()
    @tactic.pending_order.xaction = Order.TypeBuy
    @tactic.pending_order.save()
    assert_equal Holding.new.class.name, s.process(@tactic).class.name

    @tactic.pending_order.xaction = Order.TypeSell
    @tactic.pending_order.save()
    assert_equal Shopping.new.class.name, s.process(@tactic).class.name

    @tactic.broker.check_return = false
    assert_equal Pending.new.class.name, s.process(@tactic).class.name
    @tactic.broker.check_return = true

    @company.exchange.closeit()
    assert_equal ExchangeClosed.new.class.name, s.process(@tactic).class.name
  end
end
