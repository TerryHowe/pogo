/*
 * Decompiled with CFR 0.152.
 * 
 * TableWin - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Event;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.Label;
import java.awt.Panel;
import java.awt.TextField;

class TableWin
extends Frame {
    Panel outPanel = new Panel();
    Panel outerPanel = new Panel();
    Panel titlePanel = new Panel();
    TextField rnpField;
    GraphWin gw;
    double savevalTire;
    double savevalfirst;
    double savevalsecond;
    double savevalthird;
    double savevalfourth;
    double savevalfifth;
    double savevalreverse;
    double savevalTransferCase;
    double savevalRnp;
    double savevalUnits;
    double savevalRpm;
    String savetireStr;
    public static final int MAXOUTWINX = 350;
    public static final int MAXOUTWINY = 400;
    static final double INCHTOMILE = 1.578283E-5;
    static final double MINTOHOUR = 60.0;

    TableWin() {
        this.resize(350, 400);
        this.savevalRpm = 2500.0;
        this.gw = null;
    }

    public void print(double d, double d2, double d3, double d4, double d5, double d6, double d7, double d8, double d9, double d10, String string) {
        this.savevalTire = d;
        this.savevalfirst = d2;
        this.savevalsecond = d3;
        this.savevalthird = d4;
        this.savevalfourth = d5;
        this.savevalfifth = d6;
        this.savevalreverse = d7;
        this.savevalTransferCase = d8;
        this.savevalRnp = d9;
        this.savevalUnits = d10;
        this.savetireStr = new String(string);
        this.removeAll();
        this.outPanel.removeAll();
        this.outerPanel.removeAll();
        this.titlePanel.removeAll();
        this.setLayout(new BorderLayout());
        this.outerPanel.setLayout(new BorderLayout());
        this.titlePanel.setLayout(new FlowLayout(1));
        this.rnpField = new TextField(Integer.toString((int)this.savevalRpm), 5);
        if (d10 != 1.0) {
            this.setTitle("Gear and Tire Table: KPH");
            this.titlePanel.add(new Label("KPH @ "));
            this.titlePanel.add(this.rnpField);
            this.titlePanel.add(new Label(" RPM"));
        } else {
            this.setTitle("Gear and Tire Table: MPH");
            this.titlePanel.add(new Label("MPH @ "));
            this.titlePanel.add(this.rnpField);
            this.titlePanel.add(new Label(" RPM"));
        }
        this.add("North", this.titlePanel);
        this.outerPanel.add("North", new Label("with " + string + " tires and " + d9 + ":1 Ring and Pinion", 1));
        this.outPanel.setLayout(new GridLayout(11, 4, 10, 10));
        double d11 = d * Math.PI * 1.578283E-5 / d9;
        double d12 = (d11 *= d10 * this.savevalRpm * 60.0) / d8;
        this.outPanel.add(new Label("", 0));
        this.outPanel.add(new Label("", 0));
        this.outPanel.add(new Label("", 0));
        this.outPanel.add(new Label("", 0));
        this.outPanel.add(new Label("", 0));
        this.outPanel.add(new Label("", 0));
        this.outPanel.add(new Label("High", 0));
        this.outPanel.add(new Label("Low", 0));
        this.outPanel.add(new Label("", 0));
        this.outPanel.add(new Label("", 0));
        this.outPanel.add(new Label(this.formatRatio(1.0)));
        this.outPanel.add(new Label(this.formatRatio(d8)));
        this.outPanel.add(new Label("First", 0));
        this.outPanel.add(new Label(this.formatRatio(d2)));
        this.outPanel.add(new Label(Integer.toString((int)(d11 / d2))));
        this.outPanel.add(new Label(Integer.toString((int)(d12 / d2))));
        this.outPanel.add(new Label("Second", 0));
        this.outPanel.add(new Label(this.formatRatio(d3)));
        this.outPanel.add(new Label(Integer.toString((int)(d11 / d3))));
        this.outPanel.add(new Label(Integer.toString((int)(d12 / d3))));
        this.outPanel.add(new Label("Third", 0));
        this.outPanel.add(new Label(this.formatRatio(d4)));
        this.outPanel.add(new Label(Integer.toString((int)(d11 / d4))));
        this.outPanel.add(new Label(Integer.toString((int)(d12 / d4))));
        this.outPanel.add(new Label("Fourth", 0));
        this.outPanel.add(new Label(this.formatRatio(d5)));
        this.outPanel.add(new Label(Integer.toString((int)(d11 / d5))));
        this.outPanel.add(new Label(Integer.toString((int)(d12 / d5))));
        this.outPanel.add(new Label("Fifth", 0));
        this.outPanel.add(new Label(this.formatRatio(d6)));
        this.outPanel.add(new Label(Integer.toString((int)(d11 / d6))));
        this.outPanel.add(new Label(Integer.toString((int)(d12 / d6))));
        this.outPanel.add(new Label("Reverse", 0));
        this.outPanel.add(new Label(this.formatRatio(d7)));
        this.outPanel.add(new Label(Integer.toString((int)(d11 / d7))));
        this.outPanel.add(new Label(Integer.toString((int)(d12 / d7))));
        this.outPanel.add(new Label("", 0));
        this.outPanel.add(new Label("", 0));
        this.outPanel.add(new Label("", 0));
        this.outPanel.add(new Label("", 0));
        this.outPanel.add(new Label("Crawl Ratio", 0));
        this.outPanel.add(new Label(Integer.toString((int)(d2 * d8 * d9)) + ":1"));
        this.outerPanel.add("Center", this.outPanel);
        if (d10 != 1.0) {
            this.outerPanel.add("South", new Label("* Your KPH may vary"));
        } else {
            this.outerPanel.add("South", new Label("* Your MPH may vary"));
        }
        this.add("Center", this.outerPanel);
        this.show();
        this.rnpField.requestFocus();
        if (this.gw != null) {
            return;
        }
        int n = 0;
        int n2 = 0;
        int n3 = 5;
        int n4 = 18;
        d11 /= this.savevalRpm;
        d12 /= this.savevalRpm;
        this.gw = new GraphWin(n3, n4);
        if (d10 != 1.0) {
            this.gw.setWindowTitle("Gear and Tire Graph: KPH");
            this.gw.setPageTitle("KPH with " + string + " tires and " + d9 + ":1 Ring and Pinion");
            n2 = 0;
            while (n2 < 140) {
                this.gw.setYlabel(n2);
                n2 += 10;
            }
            this.gw.setYname("KPH");
        } else {
            this.gw.setWindowTitle("Gear and Tire Graph: MPH");
            this.gw.setPageTitle("MPH with " + string + " tires and " + d9 + ":1 Ring and Pinion");
            n2 = 0;
            while (n2 <= 80) {
                this.gw.setYlabel(n2);
                n2 += 10;
            }
            this.gw.setYname("MPH");
        }
        this.gw.setLow(d8);
        int n5 = 0;
        while (n5 < n3) {
            double d13;
            switch (n5) {
                case 0: {
                    d13 = d2;
                    this.gw.setNames("first");
                    break;
                }
                case 1: {
                    d13 = d3;
                    this.gw.setNames("second");
                    break;
                }
                case 2: {
                    d13 = d4;
                    this.gw.setNames("third");
                    break;
                }
                case 3: {
                    d13 = d5;
                    this.gw.setNames("fourth");
                    break;
                }
                case 4: {
                    d13 = d6;
                    this.gw.setNames("fifth");
                    break;
                }
                default: {
                    d13 = d7;
                    this.gw.setNames("reverse");
                }
            }
            double d14 = 0.0;
            int n6 = 0;
            while (n6 < n4) {
                this.gw.setValues(d11 / d13 * d14);
                d14 += 250.0;
                ++n6;
            }
            ++n5;
        }
        n = 0;
        n5 = 0;
        while (n5 < n4) {
            this.gw.setXlabel(n);
            n += 250;
            ++n5;
        }
        this.gw.draw();
    }

    String formatRatio(double d) {
        String string = new String(Double.toString(d += 1.0E-4));
        int n = string.length() < 4 ? string.length() : 4;
        return string.substring(0, n);
    }

    public boolean handleEvent(Event event) {
        if (event.id == 201) {
            ((Component)this).hide();
        }
        return super.handleEvent(event);
    }

    public boolean action(Event event, Object object) {
        if (event.target instanceof TextField) {
            this.savevalRpm = Double.valueOf("0" + object);
            this.print(this.savevalTire, this.savevalfirst, this.savevalsecond, this.savevalthird, this.savevalfourth, this.savevalfifth, this.savevalreverse, this.savevalTransferCase, this.savevalRnp, this.savevalUnits, this.savetireStr);
            return true;
        }
        return false;
    }
}
