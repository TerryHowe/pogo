/*
 * Decompiled with CFR 0.152.
 * 
 * ConvTireMain - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.applet.Applet;
import java.awt.BorderLayout;
import java.awt.Event;
import java.awt.Label;

public class ConvTireMain
extends Applet {
    ConvTire panelTire;
    Label msgLabel;
    String tireStr;
    String valTire;
    boolean problem;

    public void init() {
        this.setLayout(new BorderLayout());
        this.panelTire = new ConvTire(this);
        this.msgLabel = new Label("", 0);
        this.add("North", this.panelTire);
        this.add("South", this.msgLabel);
        this.show();
    }

    public boolean action(Event event, Object object) {
        return false;
    }

    public void printMsg(String string) {
        this.msgLabel.setText(string);
        this.show();
    }

    void update(ConvTire convTire) {
        try {
            this.valTire = convTire.getvalTire();
        }
        catch (ConvTireEx convTireEx) {
            this.panelTire.setfocus();
            this.msgLabel.setAlignment(0);
            this.printMsg(convTireEx.toString());
            this.problem = true;
            return;
        }
        this.tireStr = convTire.gettireStr();
        this.msgLabel.setAlignment(1);
        this.printMsg("A " + this.tireStr + " metric tire is " + this.valTire);
    }

    void handleCalculation() {
        this.problem = false;
        this.update(this.panelTire);
        if (this.problem) {
            return;
        }
    }

    public String getAppletInfo() {
        return "Tire Conversion Form copyright 1997 Terry L. Howe";
    }
}
