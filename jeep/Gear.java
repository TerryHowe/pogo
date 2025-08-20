/*
 * Decompiled with CFR 0.152.
 * 
 * Gear - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.applet.Applet;
import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.Event;
import java.awt.GridLayout;
import java.awt.Label;
import java.awt.Panel;
import java.awt.Point;
import java.net.URL;

public class Gear
extends Applet {
    GearTire panelTire;
    Transmission panelTransmission;
    TransferCase panelTransferCase;
    GearRnp panelRnp;
    MphKph panelMphKph;
    Panel SuperRnp;
    Panel SuperTransferCase;
    Panel SuperMphKph;
    Panel SuperButtons;
    Label msgLabel;
    String tireStr;
    double valTire;
    double valfirst;
    double valsecond;
    double valthird;
    double valfourth;
    double valfifth;
    double valreverse;
    double valTransferCase;
    double valRnp;
    double valUnits;
    boolean problem;
    URL cb;
    TableWin[] tableWin;
    int curTableWin;
    static final int MAXOUTWIN = 2;
    static final int MAXOUTWINX = 350;
    static final int MAXOUTWINY = 300;
    public static final double UNITS_MPH = 1.0;
    public static final double UNITS_KPH = 1.609344;
    static final int HELP_COUNT = 5;
    static final String[] HELP_BUTTONS = new String[]{"main", "tire", "transmission", "transfercase", "ring and pinion"};
    static final String[] HELP_FILES = new String[]{"main.txt", "tire.txt", "trans.txt", "xfer.txt", "rnp.txt"};

    public void init() {
        this.tableWin = new TableWin[2];
        this.tableWin[0] = new TableWin();
        this.tableWin[1] = new TableWin();
        this.curTableWin = 0;
        Point point = this.location();
        point.x = point.x - 350 > 0 ? (point.x -= 350) : (point.x += this.size().width);
        this.tableWin[0].move(point.x, point.y);
        point.y = point.y - 400 > 0 ? (point.y -= 400) : (point.y += 400);
        this.tableWin[1].move(point.x, point.y);
        this.setLayout(new BorderLayout());
        this.SuperRnp = new Panel();
        this.SuperRnp.setLayout(new BorderLayout());
        this.SuperTransferCase = new Panel();
        this.SuperTransferCase.setLayout(new BorderLayout());
        this.SuperMphKph = new Panel();
        this.SuperMphKph.setLayout(new BorderLayout());
        this.panelTire = new GearTire(this);
        this.SuperMphKph.add("North", this.panelTire);
        this.panelMphKph = new MphKph(this, "Miles per hour");
        this.SuperMphKph.add("South", this.panelMphKph);
        this.SuperTransferCase.add("North", new Label("Transmission", 1));
        this.panelTransmission = new Transmission(this, "6.32", "3.50", "2.50", "1.00", "0.75", "7.32");
        this.SuperTransferCase.add("Center", this.panelTransmission);
        this.panelTransferCase = new TransferCase(this, "2.62");
        this.SuperTransferCase.add("South", this.panelTransferCase);
        this.SuperRnp.add("Center", this.SuperTransferCase);
        this.panelRnp = new GearRnp(this, "3.54");
        this.SuperRnp.add("South", this.panelRnp);
        this.SuperMphKph.add("Center", this.SuperRnp);
        this.add("North", this.SuperMphKph);
        this.SuperButtons = new Panel();
        this.SuperButtons.setLayout(new GridLayout(1, 2, 10, 10));
        this.SuperButtons.add(new Button("Calculate"));
        this.add("Center", this.SuperButtons);
        this.msgLabel = new Label("", 0);
        this.add("South", this.msgLabel);
        this.panelTire.setfocus();
    }

    public boolean action(Event event, Object object) {
        if (event.target instanceof Button && object.toString().equals("Calculate")) {
            this.handleCalculation();
            return true;
        }
        return false;
    }

    public void printMsg(String string) {
        this.msgLabel.setText(string);
        this.show();
    }

    void update(GearTire gearTire) {
        try {
            this.valTire = gearTire.getvalTire();
        }
        catch (NewEx newEx) {
            this.panelTire.setfocus();
            this.printMsg(newEx.toString());
            this.problem = true;
            return;
        }
        this.panelTransmission.setfocus(0);
        this.tireStr = gearTire.gettireStr();
        this.printMsg("");
    }

    void update(Transmission transmission) {
        int n = 0;
        try {
            this.valfirst = transmission.getvalfirst();
            n = 1;
            this.valsecond = transmission.getvalsecond();
            n = 2;
            this.valthird = transmission.getvalthird();
            n = 3;
            this.valfourth = transmission.getvalfourth();
            n = 4;
            this.valfifth = transmission.getvalfifth();
            n = 5;
            this.valreverse = transmission.getvalreverse();
        }
        catch (NewEx newEx) {
            this.panelTransmission.setfocus(n);
            this.printMsg(newEx.toString());
            this.problem = true;
            return;
        }
        if (this.panelTransmission.setfocus()) {
            this.panelTransferCase.setfocus();
        }
        this.printMsg("");
    }

    void update(TransferCase transferCase) {
        try {
            this.valTransferCase = transferCase.getvalTransferCase();
        }
        catch (NewEx newEx) {
            this.printMsg(newEx.toString());
            this.problem = true;
            return;
        }
        this.panelRnp.setfocus();
        this.printMsg("");
    }

    void update(GearRnp gearRnp) {
        try {
            this.valRnp = gearRnp.getvalRnp();
        }
        catch (NewEx newEx) {
            this.printMsg(newEx.toString());
            this.problem = true;
            return;
        }
        this.panelTire.setfocus();
        this.printMsg("");
    }

    void update(MphKph mphKph) {
        this.valUnits = 1.0;
        if (mphKph.mphkph.getCurrent().getLabel().equals("Kilometers per hour")) {
            this.valUnits = 1.609344;
        }
    }

    void handleCalculation() {
        this.problem = false;
        this.update(this.panelTire);
        if (this.problem) {
            return;
        }
        this.update(this.panelTransmission);
        if (this.problem) {
            return;
        }
        this.update(this.panelTransferCase);
        if (this.problem) {
            return;
        }
        this.update(this.panelRnp);
        if (this.problem) {
            return;
        }
        this.update(this.panelMphKph);
        if (this.problem) {
            return;
        }
        this.tableWin[this.curTableWin].print(this.valTire, this.valfirst, this.valsecond, this.valthird, this.valfourth, this.valfifth, this.valreverse, this.valTransferCase, this.valRnp, this.valUnits, this.tireStr);
        this.curTableWin = (this.curTableWin + 1) % 2;
    }

    public String getAppletInfo() {
        return "Gear and Tire Form copyright 1996 Terry L. Howe";
    }
}
