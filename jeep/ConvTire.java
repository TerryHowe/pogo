/*
 * Decompiled with CFR 0.152.
 * 
 * ConvTire - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.Event;
import java.awt.FlowLayout;
import java.awt.Label;
import java.awt.Panel;
import java.awt.TextField;

class ConvTire
extends Panel {
    ConvTireMain outerparent;
    TextField textTire;
    String tireStr;
    static final int MAXTIRESTR = 11;
    static final double MMTOINCH = 0.03937008;

    ConvTire(ConvTireMain convTireMain) {
        this.outerparent = convTireMain;
        this.setLayout(new FlowLayout(1));
        this.add(new Label("Enter Metric Tire Size ", 1));
        this.textTire = new TextField("", 12);
        this.add(this.textTire);
    }

    public String getvalTire() throws ConvTireEx {
        this.tireStr = this.textTire.getText().toUpperCase();
        if (this.tireStr.startsWith("P")) {
            return this.convertMetric(this.tireStr.substring(1, this.tireStr.length()));
        }
        if (this.tireStr.startsWith("LT")) {
            return this.convertMetric(this.tireStr.substring(2, this.tireStr.length()));
        }
        if (this.tireStr.length() > 8 && this.tireStr.charAt(3) == '/') {
            return this.convertMetric(this.tireStr);
        }
        throw new ConvTireEx("Tire: Invalid value for metric tire.  Valid value e.g.: 225/75R15");
    }

    public String gettireStr() {
        return this.tireStr;
    }

    public String convertMetric(String string) throws ConvTireEx {
        int n = string.length();
        if (n < 9) {
            throw new ConvTireEx("Tire: Input not long enough for metric type.  e.g.: 225/75R15");
        }
        if (string.charAt(3) != '/' && string.charAt(3) != '\\') {
            throw new ConvTireEx("Tire: A back slash(/) must appear as the fourth character.  e.g.: 225/75R15");
        }
        if (string.charAt(6) != 'r' && string.charAt(6) != 'R') {
            throw new ConvTireEx("Tire: A 'R' must appear as the seventh character.  e.g.: 225/75R15");
        }
        int n2 = 0;
        while (n2 < n) {
            if (n2 != 3 && n2 != 6 && (string.charAt(n2) < '0' || string.charAt(n2) > '9')) {
                throw new ConvTireEx("Tire: Character " + (n2 + 1) + " is not a digit");
            }
            ++n2;
        }
        String string2 = string.substring(0, 3);
        double d = Double.valueOf(string2);
        string2 = string.substring(4, 6);
        double d2 = Double.valueOf(string2);
        string2 = string.substring(7, 9);
        double d3 = Double.valueOf(string2);
        double d4 = d * d2 / 100.0 * 0.03937008 * 2.0 + d3;
        String string3 = new String(Double.toString(d4 += 1.0E-4));
        n2 = string3.length() < 4 ? string3.length() : 4;
        d *= 0.03937008;
        String string4 = new String(Double.toString(d += 1.0E-4));
        int n3 = string4.length() < 4 ? string4.length() : 4;
        return string3.substring(0, n2) + "\"X" + string4.substring(0, n3) + "\"";
    }

    public void setfocus() {
        this.textTire.requestFocus();
    }

    public boolean action(Event event, Object object) {
        if (event.target instanceof TextField) {
            this.outerparent.update(this);
            return true;
        }
        return false;
    }
}
