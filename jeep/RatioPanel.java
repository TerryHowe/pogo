/*
 * Decompiled with CFR 0.152.
 * 
 * RatioPanel - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.Panel;

public class RatioPanel
extends Panel {
    /*
     * Enabled force condition propagation
     * Lifted jumps to return sites
     */
    double convertRatio(String string, String string2) throws NewEx {
        int n;
        int n2 = n = string2.length() < 4 ? string2.length() : 4;
        if (string2.indexOf(58) > 0) {
            n = string2.indexOf(58) < n ? string2.indexOf(58) : n;
        }
        String string3 = string2.substring(0, n);
        if (n == 0) {
            return 0.0;
        }
        int n3 = 0;
        int n4 = 0;
        while (n4 < n) {
            if (string3.charAt(n4) == '.') {
                if (n3 != 0) throw new NewEx(string + ": More than one decimal place");
                ++n3;
            } else if (string3.charAt(n4) < '0' || string3.charAt(n4) > '9') {
                throw new NewEx(string + ": Character " + (n4 + 1) + " is not a digit");
            }
            ++n4;
        }
        return Double.valueOf(string3);
    }
}
