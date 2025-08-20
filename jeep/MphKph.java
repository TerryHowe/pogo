/*
 * Decompiled with CFR 0.152.
 * 
 * MphKph - Part of the Jeep Technical applets collection
 * Legacy Java 1.1 bytecode for tire calculations and speedometer gear analysis
 */
import java.awt.Checkbox;
import java.awt.CheckboxGroup;
import java.awt.Event;
import java.awt.Panel;

class MphKph
extends Panel {
    CheckboxGroup mphkph;
    Gear outerparent;
    public static final String Smph = "Miles per hour";
    public static final String Skph = "Kilometers per hour";

    MphKph(Gear gear, String string) {
        this.outerparent = gear;
        boolean bl = false;
        boolean bl2 = true;
        if (string.equals(Skph)) {
            bl = true;
            bl2 = false;
        }
        this.mphkph = new CheckboxGroup();
        this.add(new Checkbox(Smph, this.mphkph, bl2));
        this.add(new Checkbox(Skph, this.mphkph, bl));
    }

    public boolean action(Event event, Object object) {
        if (event.target instanceof CheckboxGroup) {
            this.outerparent.update(this);
            return true;
        }
        return false;
    }
}
