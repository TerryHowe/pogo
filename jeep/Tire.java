/*
 * Decompiled with CFR 0.152.
 * 
 * Tire - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.Event;
import java.awt.FlowLayout;
import java.awt.Label;
import java.awt.Panel;
import java.awt.TextField;

public abstract class Tire
extends Panel {
    TextField textTire;
    String tireStr;
    static final int MAXTIRESTR = 11;
    static final double MMTOINCH = 0.03937008;

    Tire() {
        this.setLayout(new FlowLayout(1));
        this.add(new Label("Enter tire size ", 1));
        this.textTire = new TextField("", 12);
        this.add(this.textTire);
    }

    public double getvalTire() throws NewEx {
        String string = this.textTire.getText().toUpperCase();
        if (string.startsWith("P")) {
            return this.convertMetric(string.substring(1, string.length()));
        }
        if (string.startsWith("LT")) {
            return this.convertMetric(string.substring(2, string.length()));
        }
        if (string.length() > 8 && string.charAt(3) == '/') {
            return this.convertMetric(string);
        }
        return this.convertEnglish(string);
    }

    public String gettireStr() {
        return this.tireStr;
    }

    public double convertMetric(String string) throws NewEx {
        int n = string.length();
        if (n < 9) {
            throw new NewEx("Tire: Input not long enough for metric type.  e.g.: 225/75R15");
        }
        if (string.charAt(3) != '/' && string.charAt(3) != '\\') {
            throw new NewEx("Tire: A back slash(/) must appear as the fourth character.  e.g.: 225/75R15");
        }
        if (string.charAt(6) != 'r' && string.charAt(6) != 'R') {
            throw new NewEx("Tire: A 'R' must appear as the seventh character.  e.g.: 225/75R15");
        }
        int n2 = 0;
        while (n2 < n) {
            if (n2 != 3 && n2 != 6 && (string.charAt(n2) < '0' || string.charAt(n2) > '9')) {
                throw new NewEx("Tire: Character " + (n2 + 1) + " is not a digit");
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
        this.tireStr = new String(d + "/" + d2 + "R" + d3);
        return d4;
    }

    /*
     * Enabled force condition propagation
     * Lifted jumps to return sites
     */
    public double convertEnglish(String string) throws NewEx {
        int n;
        if (string.length() < 1) {
            throw new NewEx("Tire: Tire size not entered");
        }
        int n2 = 0;
        int n3 = n = string.length() < 4 ? string.length() : 4;
        if (string.indexOf(88) > 0) {
            int n4 = n = string.indexOf(88) < n ? string.indexOf(88) : n;
        }
        if (string.indexOf(34) > 0) {
            int n5 = n = string.indexOf(34) < n ? string.indexOf(34) : n;
        }
        if (string.indexOf(83) > 0) {
            n = string.indexOf(83) < n ? string.indexOf(83) : n;
        }
        String string2 = string.substring(0, n);
        n = 0;
        while (n < string2.length()) {
            if (string2.charAt(n) == '.') {
                if (n2 != 0) throw new NewEx("Tire: More than one decimal place");
                ++n2;
            } else if (string2.charAt(n) < '0' || string2.charAt(n) > '9') {
                throw new NewEx("Tire: Character " + (n + 1) + " is not a digit");
            }
            ++n;
        }
        this.tireStr = new String(string2 + "\"");
        return Double.valueOf(string2);
    }

    public void setfocus() {
        this.textTire.requestFocus();
    }

    public abstract boolean action(Event var1, Object var2);
}
