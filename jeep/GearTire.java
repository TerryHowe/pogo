/*
 * Decompiled with CFR 0.152.
 * 
 * GearTire - A component class for the Gear and Tire applet
 * Extends Tire to handle tire-related input events in the main Gear applet
 */
import java.awt.Event;
import java.awt.TextField;

class GearTire extends Tire {
    Gear outerparent;

    GearTire(Gear gear) {
        this.outerparent = gear;
    }

    public boolean action(Event event, Object object) {
        if (event.target instanceof TextField) {
            this.outerparent.update(this);
            return true;
        }
        return false;
    }
}