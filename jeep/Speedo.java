/*
 * Decompiled with CFR 0.152.
 * 
 * Speedo - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.applet.Applet;
import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.Choice;
import java.awt.Event;
import java.awt.FlowLayout;
import java.awt.Label;
import java.awt.Panel;

public class Speedo
extends Applet {
    Panel superSpeedo;
    Panel superModels;
    Choice jeepModels;
    SpeedoTire panelTire;
    SpeedoRnp panelRnp;
    Label msgLabel;
    String tireStr;
    double valTire;
    double valRnp;
    boolean problem;

    public void init() {
        this.superSpeedo = new Panel();
        this.setLayout(new BorderLayout());
        this.superSpeedo.setLayout(new BorderLayout());
        this.superModels = new Panel();
        this.superModels.setLayout(new FlowLayout(1));
        this.jeepModels = new Choice();
        this.jeepModels.addItem("'87+ Wrangler");
        this.jeepModels.addItem("'72-'86 CJ");
        this.jeepModels.addItem("'45-'71 CJ");
        this.superModels.add(new Label("Model", 2));
        this.superModels.add(this.jeepModels);
        this.add("North", this.superModels);
        this.panelTire = new SpeedoTire(this);
        this.panelRnp = new SpeedoRnp(this, "3.54");
        this.superSpeedo.add("North", this.panelTire);
        this.superSpeedo.add("Center", this.panelRnp);
        this.superSpeedo.add("South", new Button("Calculate"));
        this.add("Center", this.superSpeedo);
        this.msgLabel = new Label("", 0);
        this.add("South", this.msgLabel);
        this.panelTire.setfocus();
        this.show();
    }

    public boolean action(Event event, Object object) {
        if (event.target instanceof Button) {
            this.handleCalculation();
            return true;
        }
        return false;
    }

    public void printMsg(String string) {
        this.msgLabel.setText(string);
        this.show();
    }

    void update(SpeedoTire speedoTire) {
        try {
            this.valTire = speedoTire.getvalTire();
        }
        catch (NewEx newEx) {
            this.printMsg(newEx.toString());
            this.problem = true;
            return;
        }
        this.panelRnp.setfocus();
        this.printMsg("");
    }

    void update(SpeedoRnp speedoRnp) {
        try {
            this.valRnp = speedoRnp.getvalRnp();
        }
        catch (NewEx newEx) {
            this.printMsg(newEx.toString());
            this.problem = true;
            return;
        }
        this.panelTire.setfocus();
        this.printMsg("");
    }

    void handleCalculation() {
        this.problem = false;
        this.update(this.panelTire);
        if (this.problem) {
            return;
        }
        this.update(this.panelRnp);
        if (this.problem) {
            return;
        }
        int n = this.jeepModels.getSelectedIndex() == 0 ? (int)(268.0 * this.valRnp / this.valTire + 0.5) : (this.jeepModels.getSelectedIndex() == 1 ? (int)(270.0 * this.valRnp / this.valTire + 0.5) : (int)(81.0 * this.valRnp / this.valTire + 0.5));
        this.printMsg("A " + n + " tooth speedo gear is required with " + this.valRnp + ":1 ring and pinion and " + this.panelTire.gettireStr() + " tires.");
    }

    public String getAppletInfo() {
        return "Speedo Gear Applet copyright 1997 Terry L. Howe";
    }
}
